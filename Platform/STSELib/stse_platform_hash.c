/******************************************************************************
 * \file	stse_platform_hash.c
 * \brief   STSecureElement HASH platform file
 * \author  STMicroelectronics - CS application team
 *
 ******************************************************************************
 * \attention
 *
 * <h2><center>&copy; COPYRIGHT 2022 STMicroelectronics</center></h2>
 *
 * This software is licensed under terms that can be found in the LICENSE file in
 * the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include "Middleware/STM32_Cryptographic/include/cmox_crypto.h"
#include "stse_conf.h"
#include "stselib.h"

static cmox_hash_algo_t stse_platform_get_cmox_hash_algo(stse_hash_algorithm_t hash_algo) {
    switch (hash_algo) {
#ifdef STSE_CONF_HASH_SHA_1
    case STSE_SHA_1:
        return CMOX_SHA1_ALGO;
#endif
#ifdef STSE_CONF_HASH_SHA_224
    case STSE_SHA_224:
        return CMOX_SHA224_ALGO;
#endif
#ifdef STSE_CONF_HASH_SHA_256
    case STSE_SHA_256:
        return CMOX_SHA256_ALGO;
#endif
#ifdef STSE_CONF_HASH_SHA_384
    case STSE_SHA_384:
        return CMOX_SHA384_ALGO;
#endif
#ifdef STSE_CONF_HASH_SHA_512
    case STSE_SHA_512:
        return CMOX_SHA512_ALGO;
#endif
#ifdef STSE_CONF_HASH_SHA_3_256
    case STSE_SHA3_256:
        return CMOX_SHA3_256_ALGO;
#endif
#ifdef STSE_CONF_HASH_SHA_3_384
    case STSE_SHA3_384:
        return CMOX_SHA3_384_ALGO;
#endif
#ifdef STSE_CONF_HASH_SHA_3_512
    case STSE_SHA3_512:
        return CMOX_SHA3_512_ALGO;
#endif
    default:
        return NULL;
    }
}

stse_ReturnCode_t stse_platform_hash_compute(stse_hash_algorithm_t hash_algo,
                                             PLAT_UI8 *pPayload, PLAT_UI32 payload_length,
                                             PLAT_UI8 *pHash, PLAT_UI32 *hash_length) {
#if defined(STSE_CONF_HASH_SHA_1) || defined(STSE_CONF_HASH_SHA_224) ||                                      \
    defined(STSE_CONF_HASH_SHA_256) || defined(STSE_CONF_HASH_SHA_384) || defined(STSE_CONF_HASH_SHA_512) || \
    defined(STSE_CONF_HASH_SHA_3_256) || defined(STSE_CONF_HASH_SHA_3_284) || defined(STSE_CONF_HASH_SHA_3_512)

    cmox_hash_retval_t retval;

    retval = cmox_hash_compute(
        stse_platform_get_cmox_hash_algo(hash_algo),
        pPayload,
        payload_length,
        pHash,
        *hash_length,
        (size_t *)hash_length);

    /*- Verify Hash compute return */
    if (retval != CMOX_HASH_SUCCESS) {
        return STSE_PLATFORM_HASH_ERROR;
    }

    return STSE_OK;
#else
    return STSE_PLATFORM_HASH_ERROR;
#endif /* STSE_CONF_HASH_SHA_1 || STSE_CONF_HASH_SHA_224 ||\
          STSE_CONF_HASH_SHA_256 || STSE_CONF_HASH_SHA_384 || STSE_CONF_HASH_SHA_512 ||\
          STSE_CONF_HASH_SHA_3_256 || STSE_CONF_HASH_SHA_3_284 || STSE_CONF_HASH_SHA_3_512 */
}

stse_ReturnCode_t stse_platform_hmac_sha256_extract(PLAT_UI8 *pSalt, PLAT_UI16 salt_length,
                                                    PLAT_UI8 *pInput_keying_material, PLAT_UI16 input_keying_material_length,
                                                    PLAT_UI8 *pPseudorandom_key, PLAT_UI16 pseudorandom_key_expected_length) {
    cmox_mac_retval_t retval;

    PLAT_UI16 pseudorandom_key_length = pseudorandom_key_expected_length;

    retval = cmox_mac_compute(CMOX_HMAC_SHA256_ALGO,
                              pInput_keying_material,
                              input_keying_material_length,
                              pSalt,
                              salt_length,
                              NULL,
                              0,
                              pPseudorandom_key,
                              pseudorandom_key_expected_length,
                              (size_t *)&pseudorandom_key_length);

    /*- Verify MAC compute return */
    if (retval != CMOX_MAC_SUCCESS) {
        return STSE_PLATFORM_HKDF_ERROR;
    }

    return STSE_OK;
}

stse_ReturnCode_t stse_platform_hmac_sha256_expand(PLAT_UI8 *pPseudorandom_key, PLAT_UI16 pseudorandom_key_length,
                                                   PLAT_UI8 *pInfo, PLAT_UI16 info_length,
                                                   PLAT_UI8 *pOutput_keying_material, PLAT_UI16 output_keying_material_length) {
    cmox_mac_retval_t retval;

    PLAT_UI8 tmp[CMOX_SHA256_SIZE];
    PLAT_UI16 tmp_length = 0;
    PLAT_UI16 out_index = 0;
    PLAT_UI8 n = 0x1;

    cmox_mac_handle_t *pMac_handle;
    cmox_hmac_handle_t hmac_handle;

    /*	RFC 5869 : output keying material must be
	 * 		- L <= 255*HashLen
	 * 		- N = ceil(L/HashLen) */
    if (pOutput_keying_material == NULL || ((output_keying_material_length / CMOX_SHA256_SIZE) + ((output_keying_material_length % CMOX_SHA256_SIZE) != 0)) > 255) {
        return STSE_PLATFORM_HKDF_ERROR;
    }

    pMac_handle = cmox_hmac_construct(&hmac_handle, CMOX_HMAC_SHA256);
    retval = cmox_mac_init(pMac_handle);

    if (retval != CMOX_MAC_SUCCESS) {
        return STSE_PLATFORM_HKDF_ERROR;
    }

    while (out_index < output_keying_material_length) {
        PLAT_UI16 left = output_keying_material_length - out_index;

        retval = cmox_mac_setKey(pMac_handle, pPseudorandom_key, pseudorandom_key_length);
        if (retval != CMOX_MAC_SUCCESS)
            break;
        retval = cmox_mac_append(pMac_handle, tmp, tmp_length);
        if (retval != CMOX_MAC_SUCCESS)
            break;
        retval = cmox_mac_append(pMac_handle, pInfo, info_length);
        if (retval != CMOX_MAC_SUCCESS)
            break;
        retval = cmox_mac_append(pMac_handle, &n, 1);
        if (retval != CMOX_MAC_SUCCESS)
            break;
        retval = cmox_mac_generateTag(pMac_handle, tmp, NULL);
        if (retval != CMOX_MAC_SUCCESS)
            break;

        left = left < CMOX_SHA256_SIZE ? left : CMOX_SHA256_SIZE;
        memcpy(pOutput_keying_material + out_index, tmp, left);

        tmp_length = CMOX_SHA256_SIZE;
        out_index += CMOX_SHA256_SIZE;
        n++;
    }

    cmox_mac_cleanup(pMac_handle);

    /*- Verify MAC compute return */
    if (retval != CMOX_MAC_SUCCESS) {
        memset(tmp, 0, CMOX_SHA256_SIZE);
        memset(pOutput_keying_material, 0, output_keying_material_length);
        return STSE_PLATFORM_HKDF_ERROR;
    }

    return STSE_OK;
}
