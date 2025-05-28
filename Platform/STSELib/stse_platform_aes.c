/******************************************************************************
 * \file	stse_platform_crypto.c
 * \brief   STSecureElement cryptographic platform file
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

cmox_mac_handle_t *pMAC_Handler;
cmox_cmac_handle_t CMAC_Handler;

#if defined(STSE_CONF_USE_HOST_KEY_ESTABLISHMENT) || defined(STSE_CONF_USE_SYMMETRIC_KEY_ESTABLISHMENT) || defined(STSE_CONF_USE_HOST_SESSION)

stse_ReturnCode_t stse_platform_aes_cmac_init(const PLAT_UI8 *pKey,
                                              PLAT_UI16 key_length,
                                              PLAT_UI16 exp_tag_size) {
    cmox_mac_retval_t retval;

    /* - Call CMAC constructor */
    pMAC_Handler = cmox_cmac_construct(&CMAC_Handler, CMOX_CMAC_AESSMALL);

    /* - Init MAC */
    retval = cmox_mac_init(pMAC_Handler);
    if (retval != CMOX_MAC_SUCCESS) {
        return STSE_PLATFORM_AES_CMAC_COMPUTE_ERROR;
    }
    /* - Set Tag length */
    retval = cmox_mac_setTagLen(pMAC_Handler, exp_tag_size);
    if (retval != CMOX_MAC_SUCCESS) {
        return STSE_PLATFORM_AES_CMAC_COMPUTE_ERROR;
    }
    /* - Set Key  */
    retval = cmox_mac_setKey(pMAC_Handler, pKey, key_length);
    if (retval != CMOX_MAC_SUCCESS) {
        return STSE_PLATFORM_AES_CMAC_COMPUTE_ERROR;
    }

    return STSE_OK;
}

stse_ReturnCode_t stse_platform_aes_cmac_append(PLAT_UI8 *pInput,
                                                PLAT_UI16 lenght) {
    cmox_mac_retval_t retval;

    retval = cmox_mac_append(pMAC_Handler, pInput, lenght);

    if (retval != CMOX_MAC_SUCCESS) {
        return STSE_PLATFORM_AES_CMAC_COMPUTE_ERROR;
    }

    return STSE_OK;
}

stse_ReturnCode_t stse_platform_aes_cmac_compute_finish(PLAT_UI8 *pTag, PLAT_UI8 *pTagLen) {
    cmox_mac_retval_t retval;

    retval = cmox_mac_generateTag(pMAC_Handler, pTag, (size_t *)pTagLen);
    if (retval != CMOX_MAC_SUCCESS) {
        return STSE_PLATFORM_AES_CMAC_COMPUTE_ERROR;
    }

    retval = cmox_mac_cleanup(pMAC_Handler);
    if (retval != CMOX_MAC_SUCCESS) {
        return STSE_PLATFORM_AES_CMAC_COMPUTE_ERROR;
    }

    return STSE_OK;
}

stse_ReturnCode_t stse_platform_aes_cmac_verify_finish(PLAT_UI8 *pTag) {
    cmox_mac_retval_t retval;
    uint32_t cmox_mac_fault_check = 0;

    retval = cmox_mac_verifyTag(
        pMAC_Handler,
        pTag,
        &cmox_mac_fault_check);

    cmox_mac_cleanup(pMAC_Handler);

    if ((retval != CMOX_MAC_AUTH_SUCCESS) || (cmox_mac_fault_check != CMOX_MAC_AUTH_SUCCESS)) {
        return STSE_PLATFORM_AES_CMAC_VERIFY_ERROR;
    }

    return STSE_OK;
}

stse_ReturnCode_t stse_platform_aes_cmac_compute(const PLAT_UI8 *pPayload,
                                                 PLAT_UI16 payload_length,
                                                 const PLAT_UI8 *pKey,
                                                 PLAT_UI16 key_length,
                                                 PLAT_UI16 exp_tag_size,
                                                 PLAT_UI8 *pTag,
                                                 PLAT_UI16 *pTag_length) {
    cmox_mac_retval_t retval;

    retval = cmox_mac_compute(CMOX_CMAC_AESSMALL_ALGO, /* Use AES CMAC algorithm */
                              pPayload,                /* Message */
                              payload_length,          /* Message length*/
                              pKey,                    /* AES key */
                              key_length,              /* AES key length */
                              NULL,                    /* Custom Data */
                              0,                       /* Custom Data length */
                              pTag,                    /* Tag */
                              exp_tag_size,            /* Expected Tag size */
                              (size_t *)pTag_length    /* Generated Tag size */
    );

    if (retval != CMOX_MAC_SUCCESS) {
        return STSE_PLATFORM_AES_CMAC_VERIFY_ERROR;
    }

    return STSE_OK;
}

stse_ReturnCode_t stse_platform_aes_cmac_verify(const PLAT_UI8 *pPayload,
                                                PLAT_UI16 payload_length,
                                                const PLAT_UI8 *pKey,
                                                PLAT_UI16 key_length,
                                                const PLAT_UI8 *pTag,
                                                PLAT_UI16 tag_length) {
    cmox_mac_retval_t retval;

    /* - Perform CMAC verification */
    retval = cmox_mac_verify(CMOX_CMAC_AESSMALL_ALGO, /* Use AES CMAC algorithm */
                             pPayload,                /* Message length */
                             payload_length,          /* Message length */
                             pKey,                    /* AES key */
                             key_length,              /* AES key length */
                             NULL,                    /* Custom data */
                             0,                       /* Custom data length*/
                             pTag,                    /* Tag */
                             tag_length               /* Tag size */
    );

    if (retval != CMOX_MAC_AUTH_SUCCESS) {
        return STSE_PLATFORM_AES_CMAC_VERIFY_ERROR;
    }

    return STSE_OK;
}
#endif /* defined(STSE_CONF_USE_HOST_KEY_ESTABLISHMENT) || defined(STSE_CONF_USE_SYMMETRIC_KEY_ESTABLISHMENT) */

#if defined(STSE_CONF_USE_HOST_KEY_ESTABLISHMENT) || defined(STSE_CONF_USE_HOST_SESSION)
stse_ReturnCode_t stse_platform_aes_cbc_enc(const PLAT_UI8 *pPlaintext,
                                            PLAT_UI16 plaintext_length,
                                            PLAT_UI8 *pInitial_value,
                                            const PLAT_UI8 *pKey,
                                            PLAT_UI16 key_length,
                                            PLAT_UI8 *pEncryptedtext,
                                            PLAT_UI16 *pEncryptedtext_length) {
    cmox_cipher_retval_t retval;

    /*- Perform AES ECB Encryption */
    retval = cmox_cipher_encrypt(CMOX_AESSMALL_CBC_ENC_ALGO,     /* Use AES CBC algorithm */
                                 pPlaintext,                     /* Plain Text */
                                 plaintext_length,               /* Plain Text length*/
                                 pKey,                           /* AES Key */
                                 key_length,                     /* AES Key length*/
                                 pInitial_value,                 /* Initial Value */
                                 16,                             /* Initial Value length */
                                 pEncryptedtext,                 /* Ciphered Text */
                                 (size_t *)pEncryptedtext_length /* Ciphered Text length*/
    );

    /*- Verify AES ECB Encryption status */
    if (retval != CMOX_CIPHER_SUCCESS) {
        return STSE_PLATFORM_AES_CBC_ENCRYPT_ERROR;
    }

    return STSE_OK;
}

stse_ReturnCode_t stse_platform_aes_cbc_dec(const PLAT_UI8 *pEncryptedtext,
                                            PLAT_UI16 encryptedtext_length,
                                            PLAT_UI8 *pInitial_value,
                                            const PLAT_UI8 *pKey,
                                            PLAT_UI16 key_length,
                                            PLAT_UI8 *pPlaintext,
                                            PLAT_UI16 *pPlaintext_length) {
    cmox_cipher_retval_t retval;

    /*- Perform AES ECB decryption */
    retval = cmox_cipher_decrypt(CMOX_AESSMALL_CBC_DEC_ALGO, /* Use AES CBC algorithm */
                                 pEncryptedtext,             /* Ciphered Text */
                                 encryptedtext_length,       /* Ciphered Text length */
                                 pKey,                       /* AES key length */
                                 key_length,                 /* AES key */
                                 pInitial_value,             /* Initial Value */
                                 16,                         /* Initial Value length*/
                                 pPlaintext,                 /* Plain Text */
                                 (size_t *)pPlaintext_length /* Plain Text length*/
    );

    /*- Verify AES ECB decrypt return */
    if (retval != CMOX_CIPHER_SUCCESS) {
        return STSE_PLATFORM_AES_CBC_DECRYPT_ERROR;
    }

    return STSE_OK;
}

stse_ReturnCode_t stse_platform_aes_ecb_enc(const PLAT_UI8 *pPlaintext,
                                            PLAT_UI16 plaintext_length,
                                            const PLAT_UI8 *pKey,
                                            PLAT_UI16 key_length,
                                            PLAT_UI8 *pEncryptedtext,
                                            PLAT_UI16 *pEncryptedtext_length) {
    cmox_cipher_retval_t retval;
    PLAT_UI8 IV[16] = {0};

    /*- Perform AES ECB Encryption */
    retval = cmox_cipher_encrypt(CMOX_AESSMALL_ECB_ENC_ALGO,     /* Use AES ECB algorithm */
                                 pPlaintext,                     /* Plain Text */
                                 plaintext_length,               /* Plain Text length*/
                                 pKey,                           /* AES Key */
                                 key_length,                     /* AES Key length*/
                                 IV,                             /* Initial Value */
                                 16,                             /* Initial Value length */
                                 pEncryptedtext,                 /* Ciphered Text */
                                 (size_t *)pEncryptedtext_length /* Ciphered Text length*/
    );

    /*- Verify AES ECB Encryption status */
    if (retval != CMOX_CIPHER_SUCCESS) {
        return STSE_PLATFORM_AES_ECB_ENCRYPT_ERROR;
    }

    return STSE_OK;
}

stse_ReturnCode_t stse_platform_aes_ecb_dec(const PLAT_UI8 *pEncryptedtext,
                                            PLAT_UI16 encryptedtext_length,
                                            const PLAT_UI8 *pKey,
                                            PLAT_UI16 key_length,
                                            PLAT_UI8 *pPlaintext,
                                            PLAT_UI16 *pPlaintext_length) {
    cmox_cipher_retval_t retval;
    PLAT_UI8 IV[16] = {0};

    /*- Perform AES ECB decryption */
    retval = cmox_cipher_decrypt(CMOX_AESSMALL_ECB_DEC_ALGO, /* Use AES ECB algorithm */
                                 pEncryptedtext,             /* Ciphered Text */
                                 encryptedtext_length,       /* Ciphered Text length */
                                 pKey,                       /* AES key length */
                                 key_length,                 /* AES key */
                                 IV,                         /* Initial Value */
                                 16,                         /* Initial Value length*/
                                 pPlaintext,                 /* Plain Text */
                                 (size_t *)pPlaintext_length /* Plain Text length*/
    );

    /*- Verify AES ECB decrypt return */
    if (retval != CMOX_CIPHER_SUCCESS) {
        return STSE_PLATFORM_AES_ECB_DECRYPT_ERROR;
    }

    return STSE_OK;
}
#endif /* defined(STSE_CONF_USE_HOST_KEY_ESTABLISHMENT)*/
