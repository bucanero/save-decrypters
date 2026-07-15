/**********************************************************************
 * Description: Slow and fast implementations of the CRC standards.
 * Copyright (c) 2000 by Michael Barr.  This software is placed into
 * the public domain and may be used for any purpose.  However, this
 * notice must not be changed or removed and no warranty is either
 * expressed or implied by its publication or distribution.
 **********************************************************************/

#include <stdint.h>

typedef struct
{
    uint8_t width;
    uint64_t poly;
    uint64_t init;
    uint64_t xor;
    uint8_t refIn;
    uint8_t refOut;
} custom_crc_t;

#define CREATE_CRC_FUNCTION(UINT, CRC_WIDTH) \
    UINT crc##CRC_WIDTH##_hash (const uint8_t* data, uint32_t len, custom_crc_t* cfg) \
    { \
        UINT crc = (UINT)cfg->init; \
        /* Perform modulo-2 division, a byte at a time. */ \
        while (len--) { \
            /* Bring the next byte into the remainder. */ \
            crc ^= ((UINT)(cfg->refIn ? (uint8_t)reflect(*data++, 8) : *data++) << (CRC_WIDTH - 8)); \
            /* Perform modulo-2 division, a bit at a time. */ \
            for (uint8_t bit = 8; bit > 0; --bit) \
                /* Try to divide the current data bit. */ \
                crc = (crc & ((UINT)1 << (CRC_WIDTH - 1))) ? (crc << 1) ^ (UINT)cfg->poly : (crc << 1); \
        } \
        /* The final remainder is the CRC result. */ \
        if (cfg->refOut) \
            crc = (UINT)reflect(crc, CRC_WIDTH); \
        return (crc ^ (UINT)cfg->xor); \
    }

static uint64_t reflect(uint64_t data, uint8_t nBits)
{
    uint64_t reflection = 0;
    uint8_t bit;
    /*
     * Reflect the data about the center bit.
     */
    for (bit = 0; bit < nBits; ++bit)
    {
        /*
         * If the LSB bit is set, set the reflection of it.
         */
        if (data & 0x01)
            reflection |= ((uint64_t)1 << ((nBits - 1) - bit));

        data = (data >> 1);
    }

    return (reflection);

}   /* reflect() */

/* crc16_hash() */
CREATE_CRC_FUNCTION(uint16_t, 16)

/* crc32_hash() */
CREATE_CRC_FUNCTION(uint32_t, 32)

/* crc64_hash() */
CREATE_CRC_FUNCTION(uint64_t, 64)

