/******************************************************************************
 * \file	stse_platform_crc.c
 * \brief   STSecureElement CRC16 platform file
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

#include "Drivers/crc16/crc16.h"
#include "stse_conf.h"
#include "stselib.h"

stse_ReturnCode_t stse_platform_crc16_init(void) {
    crc16_Init();

    return STSE_OK;
}

PLAT_UI16 stse_platform_Crc16_Calculate(PLAT_UI8 *pbuffer, PLAT_UI16 length) {
    return crc16_Calculate(pbuffer, length);
}

PLAT_UI16 stse_platform_Crc16_Accumulate(PLAT_UI8 *pbuffer, PLAT_UI16 length) {
    return crc16_Accumulate(pbuffer, length);
}
