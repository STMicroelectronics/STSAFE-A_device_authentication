/******************************************************************************
 * \file	rng.h
 * \brief   Random Number Generator driver for STM32L452
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

#ifndef RNG_H_
#define RNG_H_

#include "stm32l4xx.h"

void rng_start(void);
uint32_t rng_generate_random_number(void);
void rng_stop(void);

#endif /* RNG_H_ */
