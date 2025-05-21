/**
  ******************************************************************************
  * @file    cmox_cipher_retvals.h
  * @author  MCD Application Team
  * @brief   Header file containing the return values for the cipher module
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
#ifndef CMOX_CIPHER_RETVALS_H
#define CMOX_CIPHER_RETVALS_H

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/* Include files -------------------------------------------------------------*/
#include <stdint.h>

/** @addtogroup CMOX_CIPHER
  * @{
  */

/** @defgroup CMOX_CIPHER_RETVALS Cipher return values
  * @{
  */

/* Macros --------------------------------------------------------------------*/

/**
  * @brief Cipher operation successfully performed
  */
#define CMOX_CIPHER_SUCCESS                  ((cmox_cipher_retval_t)0x00010000U)

/**
  * @brief Some error happens internally in the cipher module
  */
#define CMOX_CIPHER_ERR_INTERNAL             ((cmox_cipher_retval_t)0x00010001U)

/**
  * @brief The function is not implemented for the current algorithm
  */
#define CMOX_CIPHER_ERR_NOT_IMPLEMENTED      ((cmox_cipher_retval_t)0x00010002U)

/**
  * @brief One or more parameter has been wrongly passed to the function
  *   (e.g. pointer to NULL)
  */
#define CMOX_CIPHER_ERR_BAD_PARAMETER        ((cmox_cipher_retval_t)0x00010003U)

/**
  * @brief Error on performing the operation
  *        (e.g. an operation has been called before initializing the handle)
  */
#define CMOX_CIPHER_ERR_BAD_OPERATION        ((cmox_cipher_retval_t)0x00010004U)

/**
  * @brief A buffer with a worng size has been passed to the function
  */
#define CMOX_CIPHER_ERR_BAD_INPUT_SIZE       ((cmox_cipher_retval_t)0x00010005U)

/**
  * @brief Authentication of the tag has been successful
  */
#define CMOX_CIPHER_AUTH_SUCCESS             ((cmox_cipher_retval_t)0x0001C726U)

/**
  * @brief Authentication of the tag failed
  */
#define CMOX_CIPHER_AUTH_FAIL                ((cmox_cipher_retval_t)0x00016E93U)

/* Public types --------------------------------------------------------------*/

/**
  * @brief Cipher module return value type
  */
typedef uint32_t cmox_cipher_retval_t;

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CMOX_CIPHER_RETVALS_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
