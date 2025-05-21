/**
  ******************************************************************************
  * @file    cmox_sponge.h
  * @author  MCD Application Team
  * @brief   Header file for the Keccak sponge definitions
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

#ifndef CMOX_SPONGE_H
#define CMOX_SPONGE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/**
  * @addtogroup CMOX_SHA3
  * @{
  */

/**
  * @defgroup CMOX_SPONGE Sponge module
  * @{
  */

/* Macros --------------------------------------------------------------------*/

/** @defgroup CMOX_SPONGE_MACROS Sponge macros
  * @{
  */
#define CMOX_SHA3_STATE_SIZE    200U /*!< Keccak-P 1600 state size in bytes */
/**
  * @}
  */

/* Public types --------------------------------------------------------------*/

/** @defgroup CMOX_SPONGE_PUBLIC_TYPES Sponge module public types
  * @{
  */

/**
  * @brief Structure for Keccak-P 1600 sponge
  */
typedef struct
{
  uint8_t state[CMOX_SHA3_STATE_SIZE];  /*!< Internal state for Keccak P-1600 */
  uint32_t rate;                        /*!< Rate */
  uint32_t byteIOIndex;                 /*!< Index for the IO bytes */
  uint32_t squeezing;                   /*!< flag identifying if the operation is a squeezing */
} cmox_sponge_handle_t;

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

#endif /* CMOX_SPONGE_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
