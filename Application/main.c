/**
 ******************************************************************************
 * \file    		main.c
 * \author  		https://github.com/Grom-
 ******************************************************************************
 *           			COPYRIGHT 2022 STMicroelectronics
 *
 * This software is licensed under terms that can be found in the LICENSE file in
 * the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/

#include "Drivers/rng/rng.h"
#include "Drivers/uart/uart.h"
#include "stselib.h"
#include <stdio.h>
#include <string.h>

/* Defines -------------------------------------------------------------------*/
/* Root CA key used for STSE-A SPL02/SPL03 */
#define CA_SELF_SIGNED_CERTIFICATE_01                                                                   \
    0x30, 0x82, 0x01, 0xA0, 0x30, 0x82, 0x01, 0x46, 0xA0, 0x03, 0x02, 0x01, 0x02, 0x02, 0x01, 0x01,     \
        0x30, 0x0A, 0x06, 0x08, 0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x04, 0x03, 0x02, 0x30, 0x4F, 0x31, 0x0B, \
        0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13, 0x02, 0x4E, 0x4C, 0x31, 0x1E, 0x30, 0x1C, 0x06, \
        0x03, 0x55, 0x04, 0x0A, 0x0C, 0x15, 0x53, 0x54, 0x4D, 0x69, 0x63, 0x72, 0x6F, 0x65, 0x6C, 0x65, \
        0x63, 0x74, 0x72, 0x6F, 0x6E, 0x69, 0x63, 0x73, 0x20, 0x6E, 0x76, 0x31, 0x20, 0x30, 0x1E, 0x06, \
        0x03, 0x55, 0x04, 0x03, 0x0C, 0x17, 0x53, 0x54, 0x4D, 0x20, 0x53, 0x54, 0x53, 0x41, 0x46, 0x45, \
        0x2D, 0x41, 0x20, 0x50, 0x52, 0x4F, 0x44, 0x20, 0x43, 0x41, 0x20, 0x30, 0x31, 0x30, 0x1E, 0x17, \
        0x0D, 0x31, 0x38, 0x30, 0x37, 0x32, 0x37, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x5A, 0x17, 0x0D, \
        0x34, 0x38, 0x30, 0x37, 0x32, 0x37, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x5A, 0x30, 0x4F, 0x31, \
        0x0B, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13, 0x02, 0x4E, 0x4C, 0x31, 0x1E, 0x30, 0x1C, \
        0x06, 0x03, 0x55, 0x04, 0x0A, 0x0C, 0x15, 0x53, 0x54, 0x4D, 0x69, 0x63, 0x72, 0x6F, 0x65, 0x6C, \
        0x65, 0x63, 0x74, 0x72, 0x6F, 0x6E, 0x69, 0x63, 0x73, 0x20, 0x6E, 0x76, 0x31, 0x20, 0x30, 0x1E, \
        0x06, 0x03, 0x55, 0x04, 0x03, 0x0C, 0x17, 0x53, 0x54, 0x4D, 0x20, 0x53, 0x54, 0x53, 0x41, 0x46, \
        0x45, 0x2D, 0x41, 0x20, 0x50, 0x52, 0x4F, 0x44, 0x20, 0x43, 0x41, 0x20, 0x30, 0x31, 0x30, 0x59, \
        0x30, 0x13, 0x06, 0x07, 0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x02, 0x01, 0x06, 0x08, 0x2A, 0x86, 0x48, \
        0xCE, 0x3D, 0x03, 0x01, 0x07, 0x03, 0x42, 0x00, 0x04, 0x82, 0x19, 0x4F, 0x26, 0xCC, 0xA3, 0x6E, \
        0x0E, 0x82, 0x19, 0x5C, 0xE6, 0x66, 0x58, 0xEC, 0x64, 0xA4, 0x66, 0x92, 0x2F, 0x58, 0xC9, 0xE6, \
        0x4B, 0x5D, 0xE1, 0xA2, 0x9E, 0x7F, 0x39, 0x86, 0x3D, 0x04, 0x26, 0x92, 0xE4, 0xC8, 0xAC, 0x79, \
        0xF9, 0x6D, 0x2F, 0xED, 0x52, 0x77, 0x4D, 0x52, 0x81, 0x95, 0x39, 0xF2, 0x1F, 0x3E, 0xCD, 0x19, \
        0x38, 0xF8, 0x3D, 0x70, 0xAE, 0xE0, 0x9C, 0xCD, 0x8D, 0xA3, 0x13, 0x30, 0x11, 0x30, 0x0F, 0x06, \
        0x03, 0x55, 0x1D, 0x13, 0x01, 0x01, 0xFF, 0x04, 0x05, 0x30, 0x03, 0x01, 0x01, 0xFF, 0x30, 0x0A, \
        0x06, 0x08, 0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x04, 0x03, 0x02, 0x03, 0x48, 0x00, 0x30, 0x45, 0x02, \
        0x20, 0x6E, 0xE5, 0x43, 0x32, 0x47, 0xAC, 0x72, 0x34, 0xFC, 0x9D, 0x17, 0x5A, 0xA5, 0x1E, 0x83, \
        0x27, 0x69, 0x01, 0xAD, 0xEC, 0x1F, 0x00, 0x5E, 0x37, 0x1F, 0x40, 0x73, 0x4D, 0xE3, 0x8C, 0xC5, \
        0x2E, 0x02, 0x21, 0x00, 0xB1, 0xD9, 0x51, 0x6A, 0xAD, 0x9A, 0x3E, 0x86, 0xD2, 0x2B, 0x8E, 0x3B, \
        0x3B, 0xD0, 0x14, 0x6F, 0xAB, 0xB9, 0xB9, 0x22, 0xF0, 0x45, 0x26, 0x34, 0xFE, 0x92, 0x7F, 0xF5, \
        0xD6, 0x36, 0xCD, 0x90
#define STSAFE_CERTIFICATE_ZONE_0 0U
#define STSE_STATIC_PRIVATE_KEY_SLOT_0 0U
/* STDIO redirect */
#if defined(__GNUC__) && !defined(__ARMCC_VERSION)
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#define GETCHAR_PROTOTYPE int __io_getchar()
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#define GETCHAR_PROTOTYPE int fgetc(FILE *f)
#endif /* __GNUC__ */
PUTCHAR_PROTOTYPE {
    uart_putc(ch);
    return ch;
}
GETCHAR_PROTOTYPE {
    return uart_getc();
}

void apps_terminal_init(uint32_t baudrate) {
    (void)baudrate;
    /* - Initialize UART for example output log (baud 115200)  */
    uart_init(115200);
    /* Disable I/O buffering for STDOUT stream*/
    setvbuf(stdout, NULL, _IONBF, 0);
    /* - Clear terminal */
    printf("\x1B[1;1H\x1B[2J");
}

void apps_randomize_buffer(uint8_t *pBuffer, uint16_t buffer_length) {
    for (uint16_t i = 0; i < buffer_length; i++) {
        *(pBuffer + i) = (rng_generate_random_number() & 0xFF);
    }
}

void apps_print_hex_buffer(uint8_t *buffer, uint16_t buffer_size) {
    uint16_t i;
    for (i = 0; i < buffer_size; i++) {
        if (i % 16 == 0) {
            printf(" \n\r ");
        }
        printf(" 0x%02X", buffer[i]);
    }
}

void HardFault_Handler(void) {
    __BKPT();
    while (1)
        ;
}

int main(void) {
    stse_ReturnCode_t stse_ret = STSE_API_INVALID_PARAMETER;
    stse_Handler_t stse_handler;
    PLAT_UI16 certificate_size;
    stse_certificate_t parsed_stse_certificate;
    stse_certificate_t parsed_ca_selfsigned_cert;
    static const uint8_t ca_selfsigned_cert[] = {CA_SELF_SIGNED_CERTIFICATE_01};

    /* - Initialize Terminal */
    apps_terminal_init(115200);

    /* - Print Example instruction on terminal */
    printf("----------------------------------------------------------------------------------------------------------------");
    printf("\n\r-                          STSAFE-A110 Multi-Steps Device Authentication Example                               -");
    printf("\n\r----------------------------------------------------------------------------------------------------------------");
    printf("\n\r- This example illustrates STSAFE-A110 device authentication process using Multi-Step approach.                -");
    printf("\n\r- it can be taken as reference for building distant server authentication use cases.                           -");
    printf("\n\r----------------------------------------------------------------------------------------------------------------");

    /* ## Initialize STSAFE-A1xx device handler */
    stse_ret = stse_set_default_handler_value(&stse_handler);
    if (stse_ret != STSE_OK) {
        printf("\n\r ## stse_set_default_handler_value ERROR : 0x%04X\n\r", stse_ret);
        while (1)
            ;
    }
    stse_handler.device_type = STSAFE_A110;

    printf("\n\r - Initialize target STSAFE-A110");
    stse_ret = stse_init(&stse_handler);
    if (stse_ret != STSE_OK) {
        printf("\n\r ## stse_init ERROR : 0x%04X\n\r", stse_ret);
        while (1)
            ;
    }

    /* ## Parse CA self-signed certificate */
    stse_ret = stse_certificate_parse(ca_selfsigned_cert, &parsed_ca_selfsigned_cert, NULL);
    if (stse_ret != STSE_OK) {
        printf("\n\n\r## ST CA stse_certificate_parse ERROR : 0x%04X", stse_ret);
        while (1)
            ;
    }
    printf("\n\n\r## CA self-signed certificate : \n\r");
    stse_certificate_print_parsed_cert(&parsed_ca_selfsigned_cert);

    /* ## Get target STSE-Axxx Certificate  */

    stse_ret = stse_get_device_certificate_size(&stse_handler, STSAFE_CERTIFICATE_ZONE_0, &certificate_size);
    if (stse_ret != STSE_OK) {
        printf("\n\n\r## stse_get_device_certificate_size ERROR : 0x%04X", stse_ret);
        while (1)
            ;
    }

    PLAT_UI8 stse_certificate[certificate_size];

    stse_ret = stse_get_device_certificate(&stse_handler, STSAFE_CERTIFICATE_ZONE_0, certificate_size, stse_certificate);
    if (stse_ret != STSE_OK) {
        printf("\n\n\r## stse_get_device_certificate ERROR : 0x%04X", stse_ret);
        while (1)
            ;
    }

    /* ## Parse target STSAFE-Axxx certificate */

    stse_ret = stse_certificate_parse(stse_certificate, &parsed_stse_certificate, NULL);
    if (stse_ret != STSE_OK) {
        printf("\n\n\r## STSE stse_certificate_parse ERROR : 0x%04X", stse_ret);
        while (1)
            ;
    }
    printf("\n\n\r## Target STSAFE-Axxx certificate : \n\r");
    stse_certificate_print_parsed_cert(&parsed_stse_certificate);

    /* - Verify STSE Certificate with CA self-signed Certificate */
    stse_ret = stse_certificate_is_parent(&parsed_ca_selfsigned_cert, &parsed_stse_certificate, NULL);
    if (stse_ret != STSE_OK) {
        printf("\n\n\r## stse_certificate_is_parent ERROR : 0x%04X", stse_ret);
        while (1) {
            // infinite loop
        }
    } else {
        printf("\n\n\r## Device Certificate Verified\n\r");
    }

    /* ## Extract certificate key type */

    stse_ecc_key_type_t key_type = stse_certificate_get_key_type(&parsed_stse_certificate);
    PLAT_UI16 signature_size = stse_ecc_info_table[key_type].signature_size;
    PLAT_UI8 signature[signature_size];

    /* ## Generate a challenge ( aligned with key type size requirement) */

    printf("\n\n\r## Host random challenge : \n\r");
    PLAT_UI16 challenge_size = stse_ecc_info_table[key_type].private_key_size;
    PLAT_UI8 challenge[challenge_size];
    apps_randomize_buffer(challenge, challenge_size);
    apps_print_hex_buffer(challenge, challenge_size);

    /* ##  Signature over challenge */

    stse_ret = stse_ecc_generate_signature(
        &stse_handler,                  /* STSE handler */
        STSE_STATIC_PRIVATE_KEY_SLOT_0, /* Slot number */
        key_type,                       /* Certificate key type */
        challenge,                      /* Challenge */
        challenge_size,                 /* Challenge size */
        signature);                     /* Challenge signature */

    if (stse_ret != STSE_OK) {
        printf("\n\r## stse_ecc_generate_signature ERROR : 0x%04X", stse_ret);
        while (1) {
            // infinite loop
        }
    }
    printf("\n\n\r## Device signature over Host challenge: ");
    apps_print_hex_buffer(signature, signature_size);

    /*## Verify The Signature of the Random Number*/

    stse_ret = stse_certificate_verify_signature(
        &parsed_stse_certificate,
        challenge,
        challenge_size,
        signature, (signature_size >> 1),
        &signature[signature_size >> 1], (signature_size >> 1));
    if (stse_ret != STSE_OK) {
        printf("\n\n\r## stse_certificate_verify_signature ERROR : 0x%04X", stse_ret);
        while (1) {
            // infinite loop
        }
    } else {
        printf("\n\n\r# ## Device Authenticated (Challenge signature verified successfully)");
    }

    while (1) {
        // infinite loop
    }

    return 0;
}
