/******************************************************************************
 * \file	delay_ms.c
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

#include "Drivers/delay_ms/delay_ms.h"

volatile uint16_t delay_ms_timer_prescaler;

void delay_ms_init(void) {
    /* - Disable TIM6 */
    TIM6->CR1 &= ~(TIM_CR1_CEN);

    TIM6->CR1 |= (TIM_CR1_OPM);

    /* - Configure TIM6 prescaler */
    delay_ms_timer_prescaler = SystemCoreClock / 1000;
}

void delay_ms(uint16_t ms) {
    /* - Disable TIM6 */
    TIM6->CR1 &= ~(TIM_CR1_CEN);

    /* - Load timer prescaler value for delay millisecond */
    TIM6->PSC = delay_ms_timer_prescaler;

    /*- Force prescaler update by setting UG bit */
    TIM6->EGR |= TIM_EGR_UG;
    /*- Clear TIM6 Update interrupt flag */
    TIM6->SR &= ~(TIM_SR_UIF);

    /* - Reset counter value */
    TIM6->CNT = 0x0000;

    /* - Set reload value */
    TIM6->ARR = ms;

    /* - Enable TIM6 */
    TIM6->CR1 |= TIM_CR1_CEN;

    /* - Wait until TIM6 Update interrupt flag*/
    while (!(TIM6->SR & TIM_SR_UIF))
        ;

    /* - Disable TIM6 */
    TIM6->CR1 &= ~(TIM_CR1_CEN);

    /*- Clear TIM6 Update interrupt flag*/
    TIM6->SR &= ~(TIM_SR_UIF);
}

void timeout_ms_start(uint16_t ms) {
    /* - Disable TIM3 */
    TIM6->CR1 &= ~(TIM_CR1_CEN);

    /* - Load timer prescaler value for delay millisecond */
    TIM6->PSC = delay_ms_timer_prescaler;

    /*- Force prescaler update by setting UG bit */
    TIM6->EGR |= TIM_EGR_UG;
    /*- Clear TIM6 Update interrupt flag */
    TIM6->SR &= ~(TIM_SR_UIF);

    /* - Reset counter value */
    TIM6->CNT = 0x0000;

    /* - Set reload value */
    TIM6->ARR = ms;

    /* - Enable TIM6 */
    TIM6->CR1 |= TIM_CR1_CEN;
}

uint8_t timeout_ms_get_status(void) {
    if (TIM6->SR & TIM_SR_UIF) {
        /* - Disable TIM6 */
        TIM6->CR1 &= ~(TIM_CR1_CEN);

        /*- Clear TIM6 Update interrupt flag*/
        TIM6->SR &= ~(TIM_SR_UIF);

        return 1;
    }
    return 0;
}
