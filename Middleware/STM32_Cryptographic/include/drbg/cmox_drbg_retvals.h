/**
  ******************************************************************************
  * @file    cmox_drbg_retvals.h
  * @author  MCD Application Team
  * @brief   Header file for the DRBG return values
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
#ifndef CMOX_DRBG_RETVALS_H
#define CMOX_DRBG_RETVALS_H

/* Include files -------------------------------------------------------------*/
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/** @addtogroup CMOX_DRBG
  * @{
  */

/** @defgroup CMOX_DRBG_RETVALS DRBG return values
  * @{
  */

/* Macros --------------------------------------------------------------------*/

/**
  * @brief DRBG operation successfully performed
  */
#define CMOX_DRBG_SUCCESS                  ((cmox_drbg_retval_t)0x00040000U)

/**
  * @brief DRBG generic internal error
  */
#define CMOX_DRBG_ERR_INTERNAL             ((cmox_drbg_retval_t)0x00040001U)

/**
  * @brief DRBG One of the expected function parameters is invalid
  */
#define CMOX_DRBG_ERR_BAD_PARAMETER        ((cmox_drbg_retval_t)0x00040003U)

/**
  * @brief DRBG Invalid operation
  */
#define CMOX_DRBG_ERR_BAD_OPERATION        ((cmox_drbg_retval_t)0x00040004U)

/**
  * @brief DRBG has not been correctly initialized
  */
#define CMOX_DRBG_ERR_UNINIT_STATE         ((cmox_drbg_retval_t)0x0004000DU)

/**
  * @brief DRBG Reseed is needed
  */
#define CMOX_DRBG_ERR_RESEED_NEEDED        ((cmox_drbg_retval_t)0x0004000EU)

/**
  * @brief DRBG Check the size of the entropy string
  */
#define CMOX_DRBG_ERR_BAD_ENTROPY_SIZE     ((cmox_drbg_retval_t)0x0004000FU)

/**
  * @brief DRBG Check the size of the personalization string
  */
#define CMOX_DRBG_ERR_BAD_PERS_STR_LEN     ((cmox_drbg_retval_t)0x00040010U)

/**
  * @brief DRBG Check the size of the additional input string
  */
#define CMOX_DRBG_ERR_BAD_ADD_INPUT_LEN    ((cmox_drbg_retval_t)0x00040011U)

/**
  * @brief DRBG Check the size of the random request
  */
#define CMOX_DRBG_ERR_BAD_REQUEST          ((cmox_drbg_retval_t)0x00040012U)

/**
  * @brief DRBG Check the size of the nonce
  */
#define CMOX_DRBG_ERR_BAD_NONCE_SIZE       ((cmox_drbg_retval_t)0x00040013U)

/* Public types --------------------------------------------------------------*/

/**
  * @brief DRBG module return value type
  */
typedef uint32_t cmox_drbg_retval_t;

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* CMOX_DRBG_RETVALS_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
