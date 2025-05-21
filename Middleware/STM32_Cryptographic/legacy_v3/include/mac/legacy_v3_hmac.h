/**
  *******************************************************************************
  * @file    legacy_v3_hmac.h
  * @author  MCD Application Team
  * @brief   Header file of HMAC helper for migration of cryptographics
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

#ifndef LEGACY_V3_HMAC_H
#define LEGACY_V3_HMAC_H

#include <stdint.h>
#include "mac/cmox_hmac.h"
#include "hash/legacy_v3_hash.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */


/**
  * @brief  Structure for HMAC context
  */
typedef struct
{
  cmox_hmac_handle_t hmac_handle;
  cmox_mac_handle_t *mac_handle;
  uint32_t   mContextId;   /*!< Unique ID of this context. \b Not \b used in current implementation. */
  HashFlags_et mFlags;     /*!< 32 bit mFlags, used to perform keyschedule */
  int32_t  mTagSize;       /*!< Size of the required Digest */
  const uint8_t  *pmKey;   /*!< Pointer for the HMAC key */
  int32_t  mKeySize;       /*!< Size, in uint8_t (bytes) of the HMAC key */
} HMACctx_stt;

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* LEGACY_V3_HMAC_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
