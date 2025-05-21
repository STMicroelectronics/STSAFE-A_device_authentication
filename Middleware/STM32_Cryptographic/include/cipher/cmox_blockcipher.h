/**
  ******************************************************************************
  * @file    cmox_blockcipher.h
  * @author  MCD Application Team
  * @brief   Header file for the block cipher definitions and functions
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef CMOX_BLOCKCIPHER_H
#define CMOX_BLOCKCIPHER_H

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* Include files -------------------------------------------------------------*/
#include <stdint.h>

#include "cmox_cipher_retvals.h"
#include "cmox_cipher.h"

/** @addtogroup CMOX_CIPHER
  * @{
  */

/** @defgroup CMOX_BLOCKCIPHER Block cipher definitions
  * @{
  */

/* Macros --------------------------------------------------------------------*/

/** @defgroup CMOX_BLOCKCIPHER_MACROS Block cipher macros
  * @{
  */

#define CMOX_CIPHER_BLOCK_SIZE           4U /*!< Block size, in words, of a block cipher (AES and SM4) */

#define CMOX_SM4_EXPKEY_SIZE             32U /*!< Size, in words, of the expanded SM4 key */
#define CMOX_AES128_EXPKEY_SIZE          44U /*!< Size, in words, of the expanded AES128 key */
#define CMOX_AES192_EXPKEY_SIZE          52U /*!< Size, in words, of the expanded AES192 key */
#define CMOX_AES256_EXPKEY_SIZE          60U /*!< Size, in words, of the expanded AES256 key */

/**
  * @}
  */

/* Public types --------------------------------------------------------------*/

/** @defgroup CMOX_BLOCKCIPHER_PUBLIC_TYPES Block cipher public types
  * @{
  */

/**
  * @brief Block cipher algorithm type
  * This type is defined as a pointer to a structure, that contains the
  * functions needed for the specific algorithm, defined in the library internally
  */
typedef const struct cmox_blockcipher_vtableStruct_st *cmox_blockcipher_vtable_t;

/**
  * @brief Block cipher context handle
  */
typedef struct
{
  cmox_blockcipher_vtable_t table; /*!< Used block cipher algorithm */

  cmox_cipher_keyLen_t keyLen; /*!< Size, in bytes, of the block cipher's key */
  uint32_t internalState; /*!< Internal state of the block cipher */
  uint32_t expandedKey[CMOX_AES256_EXPKEY_SIZE]; /*!< Buffer containing the expanded key */
} cmox_blockcipher_handle_t;

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CMOX_BLOCKCIPHER_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
