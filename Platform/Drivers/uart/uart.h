/******************************************************************************
 * \file	uart.h
 * \brief   UART driver for STM32L452
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

#ifndef UART_H_
#define UART_H_

#include "stm32l4xx.h"

void uart_init(uint32_t baudrate);
void uart_putc(uint8_t c);
uint8_t uart_getc(void);

#endif /* UART_H_ */
