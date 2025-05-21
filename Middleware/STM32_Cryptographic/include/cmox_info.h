/**
  ******************************************************************************
  * @file    cmox_info.h
  * @author  MCD Application Team
  * @brief   This file exports symbols needed to use information module
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

#ifndef CMOX_INFO_H
#define CMOX_INFO_H

#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */


/** @addtogroup CMOX_CRYPTO
  * @{
  */

/** @addtogroup CMOX_INFO Information module
  * @{
  */

/**
  *  @brief Structure to store information
  */
typedef struct
{
  uint32_t version;      /*!< Library version */
  uint32_t build[7];     /*!< Build info */
} cmox_info_st;

/**
  * @brief          Get library information
  * @param          pInfo  Library information
  * @retval         None
  */
void cmox_getInfos(cmox_info_st *pInfo);

/**
  * @}
  */
/**
  * @}
  */

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* CMOX_INFO_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
