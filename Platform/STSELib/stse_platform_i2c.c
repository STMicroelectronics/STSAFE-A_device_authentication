/******************************************************************************
 * \file	stse_platform_i2c.c
 * \brief   STSecureElement Services platform (source)
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

#include "core/stse_platform.h"
#include "drivers/i2c/I2C.h"
#include <stdlib.h>

//#define STSE_PLATFORM_I2C_DYNAMIC_BUFFER_ALLOCATION

#ifdef STSE_PLATFORM_I2C_DYNAMIC_BUFFER_ALLOCATION
static PLAT_UI8 *pI2c_buffer;
#else
static PLAT_UI8 I2c_buffer[755U]; // Set to A120 max input buffer size + 2 bytes needed for response length + 1 byte for command or response header. Shall be adapted to applicative use case!
#endif
static PLAT_UI16 i2c_frame_size;
static volatile PLAT_UI16 i2c_frame_offset;

stse_ReturnCode_t stse_platform_i2c_init(PLAT_UI8 busID) {
    (void)busID;
    return (stse_ReturnCode_t)i2c_init(I2C1);
}

stse_ReturnCode_t stse_platform_i2c_wake(PLAT_UI8 busID,
                                         PLAT_UI8 devAddr,
                                         PLAT_UI16 speed) {
    (void)busID;
    (void)speed;

    i2c_wake(I2C1, devAddr);

    return (STSE_OK);
}

stse_ReturnCode_t stse_platform_i2c_send_start(
    PLAT_UI8 busID,
    PLAT_UI8 devAddr,
    PLAT_UI16 speed,
    PLAT_UI16 FrameLength) {
    (void)busID;
    (void)devAddr;
    (void)speed;

#ifdef STSE_PLATFORM_I2C_DYNAMIC_BUFFER_ALLOCATION
    /* - Allocate Communication buffer */
    pI2c_buffer = malloc(FrameLength);

    /* - Check buffer overflow */
    if (pI2c_buffer == NULL) {
        return STSE_PLATFORM_BUFFER_ERR;
    }
#else
    /* - Check buffer overflow */
    if (FrameLength > sizeof(I2c_buffer) / sizeof(I2c_buffer[0])) {
        return STSE_PLATFORM_BUFFER_ERR;
    }
#endif

    i2c_frame_size = FrameLength;
    i2c_frame_offset = 0;

    return STSE_OK;
}

stse_ReturnCode_t stse_platform_i2c_send_continue(
    PLAT_UI8 busID,
    PLAT_UI8 devAddr,
    PLAT_UI16 speed,
    PLAT_UI8 *pData,
    PLAT_UI16 data_size) {
    (void)busID;
    (void)devAddr;
    (void)speed;

    if (data_size != 0) {
        if (pData == NULL) {
#ifdef STSE_PLATFORM_I2C_DYNAMIC_BUFFER_ALLOCATION
            memset((pI2c_buffer + i2c_frame_offset), 0x00, data_size);
#else
            memset((I2c_buffer + i2c_frame_offset), 0x00, data_size);
#endif
        } else {
#ifdef STSE_PLATFORM_I2C_DYNAMIC_BUFFER_ALLOCATION
            memcpy((pI2c_buffer + i2c_frame_offset), pData, data_size);
#else
            memcpy((I2c_buffer + i2c_frame_offset), pData, data_size);
#endif
        }
        i2c_frame_offset += data_size;
    }

    return STSE_OK;
}

stse_ReturnCode_t stse_platform_i2c_send_stop(
    PLAT_UI8 busID,
    PLAT_UI8 devAddr,
    PLAT_UI16 speed,
    PLAT_UI8 *pData,
    PLAT_UI16 data_size) {
    stse_ReturnCode_t ret;

    ret = stse_platform_i2c_send_continue(
        busID,
        devAddr,
        speed,
        pData,
        data_size);

    /* - Send I2C frame buffer */
    if (ret == STSE_OK) {
#ifdef STSE_PLATFORM_I2C_DYNAMIC_BUFFER_ALLOCATION
        ret = (stse_ReturnCode_t)i2c_write(I2C1, devAddr, speed, pI2c_buffer, i2c_frame_size);
#else
        ret = (stse_ReturnCode_t)i2c_write(I2C1, devAddr, speed, I2c_buffer, i2c_frame_size);
#endif
    }

#ifdef STSE_PLATFORM_I2C_DYNAMIC_BUFFER_ALLOCATION
    /* - Free memory allocated to i2c buffer*/
    free(pI2c_buffer);
#endif

    if (ret != STSE_OK) {
        ret = STSE_PLATFORM_BUS_ACK_ERROR;
    }

    return ret;
}

stse_ReturnCode_t stse_platform_i2c_receive_start(
    PLAT_UI8 busID,
    PLAT_UI8 devAddr,
    PLAT_UI16 speed,
    PLAT_UI16 frameLength) {
    (void)busID;
    PLAT_I8 ret = 0;

    /* - Store response Length */
    i2c_frame_size = frameLength;

#ifdef STSE_PLATFORM_I2C_DYNAMIC_BUFFER_ALLOCATION
    /* - Allocate Communication buffer */
    pI2c_buffer = malloc(frameLength);

    /* - Check buffer overflow */
    if (pI2c_buffer == NULL) {
        return STSE_PLATFORM_BUFFER_ERR;
    }
#endif

    /* - Read full Frame */
#ifdef STSE_PLATFORM_I2C_DYNAMIC_BUFFER_ALLOCATION
    ret = i2c_read(I2C1, devAddr, speed, pI2c_buffer, i2c_frame_size);
#else
    ret = i2c_read(I2C1, devAddr, speed, I2c_buffer, i2c_frame_size);
#endif
    if (ret != 0) {
        return STSE_PLATFORM_BUS_ACK_ERROR;
    }

    /* - Reset read offset */
    i2c_frame_offset = 0;

    return STSE_OK;
}

stse_ReturnCode_t stse_platform_i2c_receive_continue(
    PLAT_UI8 busID,
    PLAT_UI8 devAddr,
    PLAT_UI16 speed,
    PLAT_UI8 *pData,
    PLAT_UI16 data_size) {
    (void)busID;
    (void)devAddr;
    (void)speed;

    if (pData != NULL) {

        /* Check read overflow */
        if ((i2c_frame_size - i2c_frame_offset) < data_size) {
            return STSE_PLATFORM_BUFFER_ERR;
        }

        /* Copy buffer content */
#ifdef STSE_PLATFORM_I2C_DYNAMIC_BUFFER_ALLOCATION
        memcpy(pData, (pI2c_buffer + i2c_frame_offset), data_size);
#else
        memcpy(pData, (I2c_buffer + i2c_frame_offset), data_size);
#endif
    }

    i2c_frame_offset += data_size;

    return STSE_OK;
}

stse_ReturnCode_t stse_platform_i2c_receive_stop(
    PLAT_UI8 busID,
    PLAT_UI8 devAddr,
    PLAT_UI16 speed,
    PLAT_UI8 *pData,
    PLAT_UI16 data_size) {
    stse_ReturnCode_t ret;

    /*- Copy last element*/
    ret = stse_platform_i2c_receive_continue(busID, devAddr, speed, pData, data_size);

    i2c_frame_offset = 0;

#ifdef STSE_PLATFORM_I2C_DYNAMIC_BUFFER_ALLOCATION
    /*- Free i2c buffer*/
    free(pI2c_buffer);
#endif
    return (ret);
}
