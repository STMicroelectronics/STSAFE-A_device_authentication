/******************************************************************************
 * \file	i2c.h
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
#ifndef DRIVERS_I2C_I2C_H_
#define DRIVERS_I2C_I2C_H_

#include "stm32l4xx.h"

uint8_t i2c_init(I2C_TypeDef *pI2C);
void i2c_deinit(I2C_TypeDef *pI2C);
int8_t i2c_write(I2C_TypeDef *pI2C, uint8_t slave_address, uint16_t speed, uint8_t *pbuffer, uint16_t size);
int8_t i2c_read(I2C_TypeDef *pI2C, uint8_t slave_address, uint16_t speed, uint8_t *pbuffer, uint16_t size);
void i2c_wake(I2C_TypeDef *pI2C, uint8_t slave_address);

#endif /* DRIVERS_I2C_I2C_H_ */
