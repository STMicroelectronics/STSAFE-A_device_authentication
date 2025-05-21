/**
  *******************************************************************************
  * @file    legacy_v3_hash.h
  * @author  MCD Application Team
  * @brief   Header file of HASH helper for migration of cryptographics
  *          library V3 to V4
  *******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  *******************************************************************************
  */

#ifndef LEGACY_V3_HASH_H
#define LEGACY_V3_HASH_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#define E_HASH_DEFAULT (HashFlags_et)(0x00000000U) /*!< User Flag: No flag specified.
                                                        This is the default value that should be set to this flag. */
#define E_HASH_DONT_PERFORM_KEY_SCHEDULE (HashFlags_et)(0x00000001U) /*!< User Flag: Used to force the init to not
                                                                          reperform key processing in HMAC mode. */

/**
  * @brief  Type definition for Hash/Hmac Flags
  */
typedef uint32_t HashFlags_et;


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* LEGACY_V3_HASH_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
