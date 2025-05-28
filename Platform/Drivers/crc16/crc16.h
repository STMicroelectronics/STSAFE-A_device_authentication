/******************************************************************************
 * \file	crc16.h
 * \brief   CRC16 driver for STM32L452
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

#ifndef CRC16_H_
#define CRC16_H_

#include "stm32l4xx.h"

//#define CRC16_HW_IMP

#define CRC16_POLY 0x1021
#define CRC_INITVALUE 0xFFFF
#define CRC16_REV_IN 1
#define CRC16_REV_OUT 1

void crc16_Init(void);
uint16_t crc16_Calculate(uint8_t *address, uint16_t length);
uint16_t crc16_Accumulate(uint8_t *address, uint16_t length);

#endif /* CRC16_H_ */
