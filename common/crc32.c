/*
 * CRC32 common implementation
 * Shared across multiple save-decrypter tools.
 */

#ifndef CRC32_POLY
#define CRC32_POLY    0xEDB88320
#endif

#ifndef CRC32C_POLY
#define CRC32C_POLY   0x82F63B78
#endif

#ifndef CRC32_INIT
#define CRC32_INIT    0xFFFFFFFF
#endif

/*
 * Initialize a 256-entry CRC32 lookup table using the reflected
 * (little-endian / Koopman) algorithm.
 */
void crc32_init_table(u32 *table, u32 poly)
{
	for (int b = 0; b < 256; ++b)
	{
		u32 r = b;

		for (int i = 0; i < 8; ++i)
			r = (r & 1) ? (r >> 1) ^ poly : (r >> 1);

		table[b] = r;
	}
}

/*
 * Compute reflected CRC32 over data.
 * Returns the final ~crc value (standard CRC32 output).
 */
u32 crc32_calculate(const u8 *data, u32 len, u32 poly, u32 init)
{
	u32 table[256];
	u32 crc = init;

	crc32_init_table(table, poly);

	while (len--)
		crc = table[(crc ^ *data++) & 0xFF] ^ (crc >> 8);

	return ~crc;
}

/*
 * Initialize a 256-entry CRC32 lookup table using the big-endian
 * (normal / MSB-first) algorithm.
 */
void crc32_init_table_be(u32 *table, u32 poly)
{
	for (int i = 0; i < 256; ++i)
	{
		u32 r = (u32)i << 24;

		for (int j = 0; j < 8; ++j)
			r = (r & 0x80000000) ? (r << 1) ^ poly : (r << 1);

		table[i] = r;
	}
}

/*
 * Compute big-endian CRC32 over data using a pre-built lookup table.
 * Returns the raw accumulated crc value (no final inversion).
 */
u32 crc32_calculate_be(const u8 *data, u32 len, const u32 *table, u32 init)
{
	u32 crc = init;

	while (len--)
		crc = (crc << 8) ^ table[((crc >> 24) ^ *data++) & 0xFF];

	return crc;
}
