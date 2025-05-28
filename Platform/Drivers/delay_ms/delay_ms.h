/******************************************************************************
 * \file	delay_ms.h
 * \brief   Millisecond delay driver for STM32L452
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

#ifndef DELAY_MS_H_
#define DELAY_MS_H_

#include "stm32l4xx.h"

void delay_ms_init(void);
void delay_ms(uint16_t ms);
void timeout_ms_start(uint16_t ms);
uint8_t timeout_ms_get_status(void);

#endif /* DELAY_MS_H_ */
