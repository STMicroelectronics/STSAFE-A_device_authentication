/******************************************************************************
 * \file	stse_platform_random.c
 * \brief   STSecureElement random number generator platform file
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

#include "Drivers/rng/rng.h"
#include "stse_conf.h"
#include "stselib.h"

stse_ReturnCode_t stse_platform_generate_random_init(void) {
    rng_start();

    return (STSE_OK);
}

PLAT_UI32 stse_platform_generate_random(void) {
    return rng_generate_random_number();
}
