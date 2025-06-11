/******************************************************************************
 * \file	i2c.c
 * \brief   I2C driver for STM32L452
 * \author  STMicroelectronics - CS application team
 *
 ******************************************************************************
 * \attention
 *
 * <h2><center>&copy; COPYRIGHT 2022 STMicroelectronics</center></h2>
 *
 * This software is licensed under terms that can be found in the LICENSE file in
 * the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include "Drivers/i2c/i2c.h"
#include "Drivers/delay_ms/delay_ms.h"

static uint16_t i2c_speed = 100;

void i2c_deinit(I2C_TypeDef *pI2C) {
    // Do nothing
    (void)pI2C;
}

uint8_t i2c_init(I2C_TypeDef *pI2C) {
    /* - Clear PE bit */
    pI2C->CR1 &= ~(I2C_CR1_PE);

    /* - Configure ANFOFF, DFN & Clock stretching */
    pI2C->CR1 |= (0b1 << I2C_CR1_ANFOFF_Pos) |   // Analog  Noise Filtering disabled
                 (0x0 << I2C_CR1_DNF_Pos) |      // Digital Noise Filtering disabled
                 (0b1 << I2C_CR1_NOSTRETCH_Pos); // Clock stretching disabled

    if (i2c_speed == 400) {
        /* - Set I2C1 Timings for 400kHz (Fast mode) */
        pI2C->TIMINGR = (0x01 << I2C_TIMINGR_PRESC_Pos) |
                        (0x26 << I2C_TIMINGR_SCLL_Pos) |
                        (0x1D << I2C_TIMINGR_SCLH_Pos) |
                        (0x01 << I2C_TIMINGR_SDADEL_Pos) |
                        (0x0A << I2C_TIMINGR_SCLDEL_Pos);
    } else {
        /* - Set I2C1 Timings for 100kHz (Standard mode) */
        pI2C->TIMINGR = (0x03 << I2C_TIMINGR_PRESC_Pos) |
                        (0x50 << I2C_TIMINGR_SCLL_Pos) |
                        (0x47 << I2C_TIMINGR_SCLH_Pos) |
                        (0x01 << I2C_TIMINGR_SDADEL_Pos) |
                        (0x09 << I2C_TIMINGR_SCLDEL_Pos);
    }

    /* - Enable pI2C */
    pI2C->CR1 |= I2C_CR1_PE;

    return 0;
}

int8_t i2c_write(I2C_TypeDef *pI2C, uint8_t slave_address, uint16_t speed, uint8_t *pbuffer, uint16_t size) {
    uint16_t i = 0;
    uint16_t offset = 0;

    uint16_t xfer_length = size;
    uint8_t xfer_size;

    i2c_speed = speed;
    i2c_init(pI2C);

    if (xfer_length > 0xFF) {
        xfer_size = 0xFF;
    } else {
        xfer_size = xfer_length;
    }

    /* - Xfer Configuration  */
    pI2C->CR2 = (0x00 << I2C_CR2_ADD10_Pos) |
                (0x00 << I2C_CR2_RD_WRN_Pos) |
                (xfer_size << I2C_CR2_NBYTES_Pos) |
                (0x01 << I2C_CR2_AUTOEND_Pos) |
                (slave_address << (I2C_CR2_SADD_Pos + 1));
    if (xfer_length > 0xFF) {
        pI2C->CR2 |= I2C_CR2_RELOAD;
    }
    /* - Start Xfer */
    pI2C->CR2 |= I2C_CR2_START;
    while (pI2C->ISR & I2C_ISR_NACKF) {
        pI2C->CR2 = (0x00 << I2C_CR2_ADD10_Pos) |
                    (0x00 << I2C_CR2_RD_WRN_Pos) |
                    (xfer_size << I2C_CR2_NBYTES_Pos) |
                    (0x01 << I2C_CR2_AUTOEND_Pos) |
                    (slave_address << (I2C_CR2_SADD_Pos + 1));
        pI2C->CR2 |= I2C_CR2_START;
        if (xfer_length > 0xFF) {
            pI2C->CR2 |= I2C_CR2_RELOAD;
        }
    }

    while (xfer_length > 0) {
        /* - Send data */
        for (i = 0; i < xfer_size; i++) {
            /* - Wait for previous data to be sent */
            while ((pI2C->ISR & I2C_ISR_TXE) != 1) {
                /* - Return error in case of NACK */
                if (pI2C->ISR & I2C_ISR_NACKF) {
                    return -1;
                }
            }
            pI2C->TXDR = (uint8_t)*(pbuffer + (i + offset));
        }
        xfer_length = (xfer_length - xfer_size);
        if (xfer_length > 0) {
            while (!(pI2C->ISR & I2C_ISR_TCR))
                ;
            if (xfer_length > 0xFF) {
                offset += 0xFF;
                xfer_size = 0xFF;
                pI2C->CR2 |= I2C_CR2_RELOAD;
                pI2C->CR2 &= ~(I2C_CR2_NBYTES_Msk);
                pI2C->CR2 |= (xfer_size << I2C_CR2_NBYTES_Pos);
            } else {
                offset += 0xFF;
                xfer_size = xfer_length;
                pI2C->CR2 &= ~(I2C_CR2_NBYTES_Msk);
                pI2C->CR2 |= (xfer_size << I2C_CR2_NBYTES_Pos);
                pI2C->CR2 &= ~(I2C_CR2_RELOAD);
            }
        }
    }
    return 0;
}

int8_t i2c_read(I2C_TypeDef *pI2C, uint8_t slave_address, uint16_t speed, uint8_t *pbuffer, uint16_t size) {
    uint32_t i = 0;
    uint16_t xfer_length;
    uint16_t xfer_size;

    (void)(speed);

    xfer_length = size;
    if (xfer_length > 0xFF) {
        xfer_size = 0xFF;
    } else {
        xfer_size = xfer_length;
    }
    /* - Xfer Configuration  */
    pI2C->CR2 = (0x00 << I2C_CR2_ADD10_Pos) |
                (0x01 << I2C_CR2_RD_WRN_Pos) |
                (xfer_size << I2C_CR2_NBYTES_Pos) |
                (0x01 << I2C_CR2_AUTOEND_Pos) |
                (slave_address << (I2C_CR2_SADD_Pos + 1));
    if (xfer_length > 0xFF) {
        pI2C->CR2 |= I2C_CR2_RELOAD;
    }

    /* - Start Xfer */
    pI2C->CR2 |= I2C_CR2_START;

    while (!(pI2C->ISR & I2C_ISR_RXNE)) {
        /*- Check if NACK */
        if ((pI2C->ISR & I2C_ISR_STOPF) && (pI2C->ISR & I2C_ISR_NACKF)) {
            pI2C->ICR |= I2C_ICR_NACKCF | I2C_ICR_STOPCF;
            return -1;
        }
    }

    while (xfer_length > 0) {
        /* - Read data */
        for (i = 0; i < xfer_size; i++) {
            /*- Wait for data reception */
            while (!(pI2C->ISR & I2C_ISR_RXNE))
                ;
            /*- Store data  */
            *(pbuffer++) = (uint8_t)pI2C->RXDR;
        }
        xfer_length = (xfer_length - xfer_size);
        if (xfer_length > 0) {
            while (!(pI2C->ISR & I2C_ISR_TCR))
                ;
            if (xfer_length > 0xFF) {
                xfer_size = 0xFF;
                pI2C->CR2 |= I2C_CR2_RELOAD;
                pI2C->CR2 &= ~(I2C_CR2_NBYTES_Msk);
                pI2C->CR2 |= (xfer_size << I2C_CR2_NBYTES_Pos);
            } else {
                xfer_size = xfer_length;
                pI2C->CR2 &= ~(I2C_CR2_NBYTES_Msk);
                pI2C->CR2 |= (xfer_size << I2C_CR2_NBYTES_Pos);
                pI2C->CR2 &= ~(I2C_CR2_RELOAD);
            }
        }
    }

    return 0;
}

void i2c_wake(I2C_TypeDef *pI2C, uint8_t slave_address) {

    /* - Xfer Configuration  */
    pI2C->CR2 = (0x00 << I2C_CR2_ADD10_Pos) |
                (0x00 << I2C_CR2_RD_WRN_Pos) |
                (0x00 << I2C_CR2_NBYTES_Pos) |
                (0x01 << I2C_CR2_AUTOEND_Pos) |
                (slave_address << (I2C_CR2_SADD_Pos + 1));
    /* - Start Xfer */
    pI2C->CR2 |= I2C_CR2_START;
}
