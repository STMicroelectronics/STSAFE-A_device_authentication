/**
  ******************************************************************************
  * @file    legacy_v3_ed25519.h
  * @author  MCD Application Team
  * @brief   Header file of ED25519 helper for migration of cryptographics
  *          library V3 to V4
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

#ifndef CMOX_HELPER_ED25519_H
#define CMOX_HELPER_ED25519_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/**
  * @brief         Ed25519 Key Generation
  * @param[in,out] *P_pPrivateKey            Buffer of 64 bytes, holding random values in the first 32 bytes
  * @param[out]    *P_pPublicKey             Buffer of 32 bytes which will hold the Public Key
  * @retval        ED25519_SUCCESS           Key Pair generated successfully
  * @retval        ED25519_ERR_BAD_PARAMETER P_pPrivateKey == NULL or P_pPublicKey == NULL
  * @retval        HASH_ERR_INTERNAL         Generic HASH internal error
  * @note          The first 32 byte of P_pPrivateKey must contain random bytes
  *                that will be used for the key pair generation
  */
int32_t ED25519keyGen(uint8_t *P_pPrivateKey, uint8_t *P_pPublicKey);

/**
  * @brief       Ed25519 Signature Generation
  * @param[out]  *P_pSignature             Buffer of 64 bytes which will hold the signature of P_pInput
  * @param[in]   *P_pInput                 Message to be signed
  * @param[in]    P_InputSize              Size of the Message to be signed
  * @param[in]   *P_pPrivateKey            Private key to be used to generate the signature
  * @retval      ED25519_SUCCESS           Signature generated successfully
  * @retval      ED25519_ERR_BAD_PARAMETER P_pRandomState == NULL or P_pPrivateKey == NULL or P_pPublicKey == NULL
  * @retval      HASH_ERR_INTERNAL         Generic HASH internal error
  */
int32_t ED25519sign(uint8_t       *P_pSignature,
                    const uint8_t *P_pInput,
                    int32_t       P_InputSize,
                    const uint8_t *P_pPrivateKey);

/**
  * @brief     Ed25519 Signature Verification
  * @param[in] *P_pInput        Message whose signature is to be verified
  * @param[in]  P_InputSize     Size of the Message whose signature is to be verified
  * @param[in] *P_pSignature    64 bytes signature of P_pInput
  * @param[in] *P_pPublicKey    Public Key to be used to verify the signature
  * @retval    SIGNATURE_VALID  Signature is VALID
  * @retval    Anything_else    Signature is INVALID
  */
int32_t ED25519verify(const uint8_t *P_pInput,
                      int32_t       P_InputSize,
                      const uint8_t *P_pSignature,
                      const uint8_t *P_pPublicKey);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CMOX_HELPER_ED25519_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
