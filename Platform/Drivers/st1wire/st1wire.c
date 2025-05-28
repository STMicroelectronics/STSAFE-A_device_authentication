/**
 ******************************************************************************
 * \file    st1wire.c
 * \brief	st1wie bit banging driver (sources)
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

/* Platform configuration parameters */
#include "st1wire.h"

/* ---------- Static functions Definition ---------- */
static int8_t _st1wire_SendByte(uint8_t bus_addr, uint8_t speed, uint8_t byte);
static int8_t _st1wire_ReceiveByte(uint8_t bus_addr, uint8_t speed, uint8_t *rcv_byte);
static int8_t _st1wire_Idle_detection(uint8_t bus_addr);
static int8_t _st1wire_SendStart(uint8_t bus_addr, uint8_t speed);

/* ---------- Static functions Declarations ---------- */
static int8_t _st1wire_Idle_detection(uint8_t bus_addr) {
    st1wire_platform_start_timeout(ST1WIRE_IDLE);
    while (st1wire_platform_io_get(bus_addr)) {
        if (st1wire_platform_is_timeout_exceeded()) {
            return ST1WIRE_BUS_ARBITRATION_FAULT;
        }
    }
    return ST1WIRE_OK;
}

static int8_t _st1wire_SendStart(uint8_t bus_addr, uint8_t speed) {
    uint8_t ret = ST1WIRE_OK;
    uint16_t start_t = ST1WIRE_3C_START_PULSE;

    if (speed == 0) {
        start_t = ST1WIRE_2C_START_PULSE;
    }

    st1wire_platform_io_in(bus_addr);
    while (!_st1wire_Idle_detection(bus_addr))
        ;
    if (st1wire_platform_io_get(bus_addr) == 0x00) {
        ret = ST1WIRE_BUS_ARBITRATION_FAULT;
    }
    /* - Set bus to low level */
    st1wire_platform_io_out(bus_addr);
    st1wire_platform_io_clear(bus_addr);
    st1wire_platform_delay(start_t);
    st1wire_platform_io_set(bus_addr);
    if (speed == 0) {
        st1wire_platform_delay(ST1WIRE_2C_INTER_BYTE_DELAY);
    }

    return ret;
}

static int8_t _st1wire_ReceiveByte(uint8_t bus_addr, uint8_t speed, uint8_t *rcv_byte) {
    uint32_t i, DelayHigh, DelayLow, byteReceived = 0;

    uint16_t long_t = ST1WIRE_3C_LONG_PULSE;
    uint16_t ack_t = ST1WIRE_3C_ACK_PULSE;
    uint16_t receive_timout_t = ST1WIRE_RECEIVE_TIMEOUT;

    if (speed == 0) {
        long_t = ST1WIRE_2C_LONG_PULSE;
        ack_t = ST1WIRE_2C_ACK_PULSE;
    }

    ST1WIRE_START_CRITICAL_SECTION
    i = 0;

    /* - Send sync bit('1') */
    st1wire_platform_io_out(bus_addr);
    st1wire_platform_io_set(bus_addr);
    st1wire_platform_delay(long_t);
    st1wire_platform_io_clear(bus_addr);
    st1wire_platform_delay(long_t);
    st1wire_platform_io_set(bus_addr);
    // Handle byte reception
    st1wire_platform_io_in(bus_addr);
    for (i = 0; i < 8; i++) {
        // - Clear SW counters
        DelayHigh = 0;
        DelayLow = 0;
        // - Count High level duration
        while (st1wire_platform_io_get(bus_addr)) /* while line value is high*/
        {
            DelayHigh++;
            if (DelayHigh >= receive_timout_t) {
                ST1WIRE_END_CRITICAL_SECTION
                return ST1WIRE_BUS_RECEIVE_TIMEOUT;
            }
        }
        // - Count Low level duration
        while (!(st1wire_platform_io_get(bus_addr))) /* while line value is low */
        {
            DelayLow++;
            if (DelayLow >= receive_timout_t) {
                ST1WIRE_END_CRITICAL_SECTION
                return ST1WIRE_BUS_RECEIVE_TIMEOUT;
            }
        }
        // - Store bit value depending on High/low delays duration
        if (DelayHigh > DelayLow) {
            byteReceived += 1;
        }
        byteReceived <<= 1;
    }
    byteReceived >>= 1; // don't do the last shift
    // - Acknowledge the byte reception
    st1wire_platform_io_out(bus_addr);
    st1wire_platform_io_clear(bus_addr);
    st1wire_platform_delay(ack_t);
    st1wire_platform_io_set(bus_addr);
    ST1WIRE_END_CRITICAL_SECTION
    *rcv_byte = (uint8_t)byteReceived;

    return ST1WIRE_OK;
}

static int8_t _st1wire_SendByte(uint8_t bus_addr, uint8_t speed, uint8_t byte) {
    volatile uint32_t i = 0;
    uint16_t long_t = ST1WIRE_3C_LONG_PULSE;
    uint16_t short_t = ST1WIRE_3C_SHORT_PULSE;

    if (speed == 0) {
        long_t = ST1WIRE_2C_LONG_PULSE;
        short_t = ST1WIRE_2C_SHORT_PULSE;
    }

    ST1WIRE_START_CRITICAL_SECTION
    st1wire_platform_io_out(bus_addr);
    /* - Send sync bit('1') */
    st1wire_platform_io_set(bus_addr);
    st1wire_platform_delay(short_t);
    st1wire_platform_io_clear(bus_addr);
    st1wire_platform_delay(long_t);
    // - Send Byte
    for (i = 0; i < 8; i++) {
        /* Mask each bit value*/
        if (byte & (1 << (7 - i))) {
            /* - Send '1' */
            st1wire_platform_io_set(bus_addr);
            st1wire_platform_delay(long_t);
            st1wire_platform_io_clear(bus_addr);
            st1wire_platform_delay(short_t);
        } else {
            /* - Send '0' */
            st1wire_platform_io_set(bus_addr);
            st1wire_platform_delay(short_t);
            st1wire_platform_io_clear(bus_addr);
            st1wire_platform_delay(long_t);
        }
    }
    /* - Release the STWire line*/
    st1wire_platform_io_set(bus_addr);
    st1wire_platform_io_in(bus_addr);

    /* - Wait for a low level on STWire*/
    i = 0;
    while (st1wire_platform_io_get(bus_addr)) {
        i++;
        if (i >= 0xFF) {
            ST1WIRE_END_CRITICAL_SECTION
            return ST1WIRE_BUS_ACK_ERROR;
        }
    }
    // - Wait for a high level on STWire
    i = 0;
    while (!(st1wire_platform_io_get(bus_addr))) {
        i++;
        if (i >= 0xFF) {
            ST1WIRE_END_CRITICAL_SECTION
            return ST1WIRE_BUS_ACK_ERROR;
        }
    }
    ST1WIRE_END_CRITICAL_SECTION

    return ST1WIRE_OK;
}

/* ---------- Exported functions Declarations ---------- */

st1wire_ReturnCode_t st1wire_init(void) {
    st1wire_platform_init();
    return ST1WIRE_OK;
}

st1wire_ReturnCode_t st1wire_deinit(void) {
    st1wire_platform_deinit();
    return ST1WIRE_OK;
}

st1wire_ReturnCode_t st1wire_SendFrame(uint8_t bus_addr,
                                       uint8_t dev_addr,
                                       uint8_t speed,
                                       uint8_t *frame,
                                       uint16_t frame_length) {
    uint8_t recv_byte;
    int8_t ret;
    uint16_t i;

#ifdef ST1WIRE_ENABLE_DEBUG_LOG
    ST1WIRE_DEBUG_PRINTF("\n\r; ST1Wire %d >", bus_addr);
#endif

    /* - Get bus Arbitration and send Start of frame */
    ret = _st1wire_SendStart(bus_addr, speed);
    if (ret == ST1WIRE_OK) {
        if (dev_addr != 0) {
            /* - Send device Address */
            ret = _st1wire_SendByte(bus_addr, speed, dev_addr);
            if (ret != ST1WIRE_OK) {
#ifdef ST1WIRE_ENABLE_DEBUG_LOG
                ST1WIRE_DEBUG_PRINTF(" ADDR ACK ERROR ");
#endif
                return ST1WIRE_BUS_ACK_ERROR;
            }
            if (speed == 0) {
                st1wire_platform_delay(ST1WIRE_2C_INTER_BYTE_DELAY);
            } else {
                st1wire_platform_delay(ST1WIRE_3C_INTER_BYTE_DELAY);
            }
        }
        /* - Send Frame length */
#ifndef ST1WIRE_NO_LEN_FIX
        ret = _st1wire_SendByte(bus_addr, speed, ((frame_length >> 8) & 0b111));
        if (ret == ST1WIRE_OK) {
            if (speed == 0) {
                st1wire_platform_delay(ST1WIRE_2C_INTER_BYTE_DELAY);
            } else {
                st1wire_platform_delay(ST1WIRE_3C_INTER_BYTE_DELAY);
            }
#endif
            ret = _st1wire_SendByte(bus_addr, speed, (frame_length & 0xFF));
#ifndef ST1WIRE_NO_LEN_FIX
        }
#endif
        if (ret == ST1WIRE_OK) {
            /* - Send Frame content */
            for (i = 0; i < frame_length; i++) {
                if (speed == 0) {
                    st1wire_platform_delay(ST1WIRE_2C_INTER_BYTE_DELAY);
                } else {
                    st1wire_platform_delay(ST1WIRE_3C_INTER_BYTE_DELAY);
                }
                ret = _st1wire_SendByte(bus_addr, speed, frame[i]);
                if (ret == ST1WIRE_BUS_ACK_ERROR) {
#ifdef ST1WIRE_ENABLE_DEBUG_LOG
                    ST1WIRE_DEBUG_PRINTF(" DATA %d ACK ERROR ", i);
#endif
                    break;
                }
            }
            /* - Get Frame Ack */
            if (ret == ST1WIRE_OK) {
                if (speed == 0) {
                    st1wire_platform_delay(ST1WIRE_2C_INTER_BYTE_DELAY);
                } else {
                    st1wire_platform_delay(ST1WIRE_3C_INTER_BYTE_DELAY);
                }
                ret = _st1wire_ReceiveByte(bus_addr, speed, &recv_byte);
                if ((ret == ST1WIRE_OK) && (recv_byte != 0x20)) {
#ifdef ST1WIRE_ENABLE_DEBUG_LOG
                    ST1WIRE_DEBUG_PRINTF(" Frame ACK ERROR ");
#endif
                    ret = ST1WIRE_BUS_ACK_ERROR;
                }
            }
        }
    }

#ifdef ST1WIRE_ENABLE_DEBUG_LOG
    for (i = 0; i < frame_length; i++) {
        ST1WIRE_DEBUG_PRINTF(" %02X", *(frame + i));
    }
#endif

    // Delay in ST1Wire slow to allow STICK Vcc to stabilize
    if (speed == 0) {
        st1wire_platform_delay(ST1WIRE_2C_INTER_FRAME_DELAY);
    } else {
        st1wire_platform_delay(ST1WIRE_3C_INTER_BYTE_DELAY);
    }

    return (st1wire_ReturnCode_t)ret;
}

st1wire_ReturnCode_t st1wire_ReceiveFrame(uint8_t bus_addr, uint8_t dev_addr, uint8_t speed, uint8_t *frame, uint16_t *pframe_length) {
    volatile uint8_t ret = ST1WIRE_BUS_ACK_ERROR;
    volatile uint16_t i;
    uint8_t rcv_byte;

    /* - Get bus Arbitration and send Start of frame */
    ret = _st1wire_SendStart(bus_addr, speed);
    /* - Request Frame reception (frame length = 0x00) */
    if (ret == ST1WIRE_OK) {

        if (dev_addr != 0) {
            /* - Send device Address */
            ret = _st1wire_SendByte(bus_addr, speed, dev_addr);
            if (ret != ST1WIRE_OK) // Target STICK Addr)
            {
#ifdef ST1WIRE_ENABLE_DEBUG_LOG
                ST1WIRE_DEBUG_PRINTF("\n\r; ST1Wire %d < DEV ADDR ACK ERROR", bus_addr);
#endif
                return ST1WIRE_BUS_ACK_ERROR;
            }
            if (speed == 0) {
                st1wire_platform_delay(ST1WIRE_2C_INTER_FRAME_DELAY);
            } else {
                st1wire_platform_delay(ST1WIRE_3C_INTER_BYTE_DELAY);
            }
        }
        ret = _st1wire_SendByte(bus_addr, speed, 0x00);
#ifndef ST1WIRE_NO_LEN_FIX
        if (ret == ST1WIRE_OK) {
            if (speed == 0) {
                st1wire_platform_delay(ST1WIRE_2C_INTER_FRAME_DELAY);
            } else {
                st1wire_platform_delay(ST1WIRE_3C_INTER_BYTE_DELAY);
            }
            ret = _st1wire_SendByte(bus_addr, speed, 0x00);
        }
#endif
    }

    if (ret != ST1WIRE_OK) {
#ifdef ST1WIRE_ENABLE_DEBUG_LOG
        ST1WIRE_DEBUG_PRINTF("\n\r; ST1Wire %d < STATUS FRAME Send ERROR", bus_addr);
#endif
        return (st1wire_ReturnCode_t)ret;
    }

    /* - Get Request ACK */
    if (speed == 0) {
        st1wire_platform_delay(ST1WIRE_2C_INTER_BYTE_DELAY);
    } else {
        st1wire_platform_delay(ST1WIRE_3C_INTER_BYTE_DELAY);
    }
    if (ret == ST1WIRE_OK) {
        ret = _st1wire_ReceiveByte(bus_addr, speed, &rcv_byte);
    }
    if ((ret == ST1WIRE_OK) && (rcv_byte == 0x20)) {
        if (speed == 0) {
            st1wire_platform_delay(ST1WIRE_2C_INTER_BYTE_DELAY);
        } else {
            st1wire_platform_delay(ST1WIRE_3C_INTER_BYTE_DELAY);
        }
        /* - Get Frame length */
        ret = _st1wire_ReceiveByte(bus_addr, speed, &rcv_byte);
#ifndef ST1WIRE_NO_LEN_FIX
        if (ret == ST1WIRE_OK) {
            *pframe_length = rcv_byte << 8;
            if (speed == 0) {
                st1wire_platform_delay(ST1WIRE_2C_INTER_BYTE_DELAY);
            } else {
                st1wire_platform_delay(ST1WIRE_3C_INTER_BYTE_DELAY);
            }
            ret = _st1wire_ReceiveByte(bus_addr, speed, &rcv_byte);
        }
#endif
        if (ret == ST1WIRE_OK) {
            *pframe_length += rcv_byte;
            for (i = 0; i < *pframe_length; i++) {
                if (speed == 0) {
                    st1wire_platform_delay(ST1WIRE_2C_INTER_BYTE_DELAY);
                } else {
                    st1wire_platform_delay(ST1WIRE_3C_INTER_BYTE_DELAY);
                }
                ret = _st1wire_ReceiveByte(bus_addr, speed, frame + i);
                if (ret != ST1WIRE_OK) {
                    break;
                }
            }
        }
    } else {
#ifdef ST1WIRE_ENABLE_DEBUG_LOG
        ST1WIRE_DEBUG_PRINTF("\n\r; ST1Wire %d < BYTE ACK ERROR", bus_addr);
#endif
        return ST1WIRE_BUS_ACK_ERROR;
    }

    // Delay in ST1Wire slow to allow STICK Vcc to stabilize
    if (speed == 0) {
        st1wire_platform_delay(ST1WIRE_2C_INTER_FRAME_DELAY);
    } else {
        st1wire_platform_delay(ST1WIRE_3C_INTER_BYTE_DELAY);
    }

#ifdef ST1WIRE_ENABLE_DEBUG_LOG
    ST1WIRE_DEBUG_PRINTF("\n\r; ST1Wire %d <", bus_addr);
    for (i = 0; i < *pframe_length; i++) {
        ST1WIRE_DEBUG_PRINTF(" %02X", *(frame + i));
    }
#endif

    return (st1wire_ReturnCode_t)ret;
}

void st1wire_wake(uint8_t bus_addr) {
    st1wire_platform_wake(bus_addr);
}

void st1wire_recovery(uint8_t bus_addr, uint8_t speed) {
    if (speed == 0) {
        st1wire_platform_io_clear(bus_addr);
        st1wire_platform_delay(100000);
        st1wire_platform_io_set(bus_addr);
        st1wire_platform_delay(100000);
    } else {
        // Do nothing for speed = 1
    }
}
