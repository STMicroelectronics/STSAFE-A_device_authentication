/**
  ******************************************************************************
  * @file    legacy_v3_membuf.h
  * @author  MCD Application Team
  * @brief   Header file of memory management (RSA and ECC) helper for migration
  *          of cryptographics library V3 to V4
  ******************************************************************************
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
  ******************************************************************************
  */

#ifndef CMOX_HELPER_MEMBUF_H
#define CMOX_HELPER_MEMBUF_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/**
  * @brief MemBuf structure definition
  */
typedef struct
{
  uint8_t *pmBuf; /*!< Pointer to the pre-allocated memory buffer, this must be set by the user*/
  uint16_t mSize; /*!< Total size of the pre-allocated memory buffer */
  uint16_t mUsed; /*!< Currently used portion of the buffer, should be inititalized by user to zero */
} membuf_stt;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CMOX_HELPER_MEMBUF_H */
