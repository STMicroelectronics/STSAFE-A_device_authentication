/******************************************************************************
 * \file	delay_us.c
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
#include "Drivers/delay_us/delay_us.h"

volatile uint16_t delay_us_timer_prescaler;

void delay_us_init(void) {
    /* - Disable TIM6 */
    TIM6->CR1 &= ~(TIM_CR1_CEN);

    TIM6->CR1 |= (TIM_CR1_OPM);

    /* - Configure TIM6 prescaler */
    delay_us_timer_prescaler = SystemCoreClock / 1000000;
}

void delay_us(uint16_t us) {
    /* - Disable TIM6 */
    TIM6->CR1 &= ~(TIM_CR1_CEN);

    /* - Load timer prescaler value for delay microsecond */
    TIM6->PSC = delay_us_timer_prescaler;

    /*- Force prescaler update by setting UG bit */
    TIM6->EGR |= TIM_EGR_UG;
    /*- Clear TIM6 Update interrupt flag */
    TIM6->SR &= ~(TIM_SR_UIF);

    /* - Reset counter value */
    TIM6->CNT = 0x0000;

    /* - Set reload value */
    TIM6->ARR = us;

    /* - Enable TIM6 */
    TIM6->CR1 |= TIM_CR1_CEN;

    /* - Wait until TIM6 Update interrupt flag*/
    while (!(TIM6->SR & TIM_SR_UIF))
        ;

    /* - Disable TIM6 */
    TIM6->CR1 &= ~(TIM_CR1_CEN);

    /*- Clear TIM6 Update interrupt flag */
    TIM6->SR &= ~(TIM_SR_UIF);
}

void timeout_us_start(uint16_t us) {
    /* - Disable TIM3 */
    TIM6->CR1 &= ~(TIM_CR1_CEN);

    /* - Load timer prescaler value for delay microsecond */
    TIM6->PSC = delay_us_timer_prescaler;

    /*- Force prescaler update by setting UG bit */
    TIM6->EGR |= TIM_EGR_UG;
    /*- Clear TIM6 Update interrupt flag */
    TIM6->SR &= ~(TIM_SR_UIF);

    /* - Reset counter value */
    TIM6->CNT = 0x0000;

    /* - Set reload value */
    TIM6->ARR = us;

    /* - Enable TIM6 */
    TIM6->CR1 |= TIM_CR1_CEN;
}

uint8_t timeout_us_get_status(void) {
    if (TIM6->SR & TIM_SR_UIF) {
        /* - Disable TIM6 */
        TIM6->CR1 &= ~(TIM_CR1_CEN);

        /*- Clear TIM6 Update interrupt flag*/
        TIM6->SR &= ~(TIM_SR_UIF);

        return 1;
    }
    return 0;
}
