/******************************************************************************
 * \file	delay_us.h
 * \brief   Microsecond delay driver for STM32L452
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

#ifndef DELAY_US_H_
#define DELAY_US_H_

#include "stm32l4xx.h"

void delay_us_init(void);
void delay_us(uint16_t us);
void timeout_us_start(uint16_t us);
uint8_t timeout_us_get_status(void);

#endif /* DELAY_US_H_ */
