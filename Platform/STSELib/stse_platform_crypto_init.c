/******************************************************************************
 * \file	stse_platform_crypto_init.c
 * \brief   STSecureElement cryptographic platform file
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

#include "Middleware/STM32_Cryptographic/include/cmox_crypto.h"
#include "stse_conf.h"
#include "stselib.h"

stse_ReturnCode_t stse_platform_crypto_init(void) {
    stse_ReturnCode_t ret = STSE_OK;

    /* - Initialize STM32 CMOX library */
    if (cmox_initialize(NULL) != CMOX_INIT_SUCCESS) {
        ret = STSE_PLATFORM_CRYPTO_INIT_ERROR;
    }

    return ret;
}
