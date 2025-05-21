/**
  ******************************************************************************
  * @file    cmox_check_default_gcm.h
  * @author  MCD Application Team
  * @brief   Header file for checking that the default values for AES are
  *          correctly set
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
#ifndef CMOX_CHECK_DEFAULT_GCM_H
#define CMOX_CHECK_DEFAULT_GCM_H

#if !defined(CMOX_DEFAULT_FILE)
#include "cmox_default_config.h"
#else
#include CMOX_DEFAULT_FILE
#endif /* CMOX_DEFAULT_FILE */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#ifndef CMOX_GCM_IMPLEMENTATION
#error CMOX_GCM_IMPLEMENTATION must be defined for using GCM
#endif /* CMOX_GCM_IMPLEMENTATION */

#ifndef CMOX_GCM_FAST
#error CMOX_GCM_FAST must be defined for using GCM
#endif /* CMOX_GCM_FAST */

#ifndef CMOX_GCM_SMALL
#error CMOX_GCM_SMALL must be defined for using GCM
#endif /* CMOX_GCM_SMALL */

#if ((CMOX_GCM_IMPLEMENTATION != CMOX_GCM_FAST) && (CMOX_GCM_IMPLEMENTATION != CMOX_GCM_SMALL))
#error CMOX_GCM_IMPLEMENTATION value must be CMOX_GCM_FAST or CMOX_GCM_SMALL
#endif /* ((CMOX_GCM_IMPLEMENTATION != CMOX_GCM_FAST) && (CMOX_GCM_IMPLEMENTATION != CMOX_GCM_SMALL)) */

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* CMOX_CHECK_DEFAULT_GCM_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
