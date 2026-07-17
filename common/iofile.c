/*
* Common IO functions
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t
#define u64 uint64_t

#define ES16(_val) \
	((u16)(((((u16)_val) & 0xff00) >> 8) | \
	       ((((u16)_val) & 0x00ff) << 8)))

#define ES32(_val) \
	((u32)(((((u32)_val) & 0xff000000) >> 24) | \
	       ((((u32)_val) & 0x00ff0000) >> 8 ) | \
	       ((((u32)_val) & 0x0000ff00) << 8 ) | \
	       ((((u32)_val) & 0x000000ff) << 24)))

#define ES64(_val) \
	((u64)(((((u64)_val) & 0xff00000000000000ull) >> 56) | \
	       ((((u64)_val) & 0x00ff000000000000ull) >> 40) | \
	       ((((u64)_val) & 0x0000ff0000000000ull) >> 24) | \
	       ((((u64)_val) & 0x000000ff00000000ull) >> 8 ) | \
	       ((((u64)_val) & 0x00000000ff000000ull) << 8 ) | \
	       ((((u64)_val) & 0x0000000000ff0000ull) << 24) | \
	       ((((u64)_val) & 0x000000000000ff00ull) << 40) | \
	       ((((u64)_val) & 0x00000000000000ffull) << 56)))

#define U_U16_LE(a, n)           \
(                                \
	((u16)((a)[(n) + 1]) << 8) | \
	((u16)((a)[(n)    ])     )   \
)

#define P_U16_LE(a, n, v)          \
do {                               \
	(a)[(n) + 1] = (u8)((v) >> 8); \
	(a)[(n)    ] = (u8)((v)     ); \
} while (0)

#define U_U16_BE(a, n)           \
(                                \
	((u16)((a)[(n)    ]) << 8) | \
	((u16)((a)[(n) + 1])     )   \
)

#define P_U16_BE(a, n, v)          \
do {                               \
	(a)[(n)    ] = (u8)((v)     ); \
	(a)[(n) + 1] = (u8)((v) >> 8); \
} while (0)

#define U_U32_LE(a, n)            \
(                                 \
	((u32)((a)[(n) + 3]) << 24) | \
	((u32)((a)[(n) + 2]) << 16) | \
	((u32)((a)[(n) + 1]) <<  8) | \
	((u32)((a)[(n)    ])      )   \
)

#define P_U32_LE(a, n, v)           \
do {                                \
	(a)[(n) + 3] = (u8)((v) >> 24); \
	(a)[(n) + 2] = (u8)((v) >> 16); \
	(a)[(n) + 1] = (u8)((v) >>  8); \
	(a)[(n)    ] = (u8)((v)      ); \
} while (0)

#define U_U32_BE(a, n)            \
(                                 \
	((u32)((a)[(n)    ]) << 24) | \
	((u32)((a)[(n) + 1]) << 16) | \
	((u32)((a)[(n) + 2]) <<  8) | \
	((u32)((a)[(n) + 3])      )   \
)

#define P_U32_BE(a, n, v)           \
do {                                \
	(a)[(n)    ] = (u8)((v) >> 24); \
	(a)[(n) + 1] = (u8)((v) >> 16); \
	(a)[(n) + 2] = (u8)((v) >>  8); \
	(a)[(n) + 3] = (u8)((v)      ); \
} while (0)

#define U_U64_LE(a, n)            \
(                                 \
	((u64)((a)[(n) + 7]) << 56) | \
	((u64)((a)[(n) + 6]) << 48) | \
	((u64)((a)[(n) + 5]) << 40) | \
	((u64)((a)[(n) + 4]) << 32) | \
	((u64)((a)[(n) + 3]) << 24) | \
	((u64)((a)[(n) + 2]) << 16) | \
	((u64)((a)[(n) + 1]) <<  8) | \
	((u64)((a)[(n)    ])      )   \
)

#define P_U64_LE(a, n, v)           \
do {                                \
	(a)[(n) + 7] = (u8)((v) >> 56); \
	(a)[(n) + 6] = (u8)((v) >> 48); \
	(a)[(n) + 5] = (u8)((v) >> 40); \
	(a)[(n) + 4] = (u8)((v) >> 32); \
	(a)[(n) + 3] = (u8)((v) >> 24); \
	(a)[(n) + 2] = (u8)((v) >> 16); \
	(a)[(n) + 1] = (u8)((v) >>  8); \
	(a)[(n)    ] = (u8)((v)      ); \
} while (0)

#define U_U64_BE(a, n)            \
(                                 \
	((u64)((a)[(n)    ]) << 56) | \
	((u64)((a)[(n) + 1]) << 48) | \
	((u64)((a)[(n) + 2]) << 40) | \
	((u64)((a)[(n) + 3]) << 32) | \
	((u64)((a)[(n) + 4]) << 24) | \
	((u64)((a)[(n) + 5]) << 16) | \
	((u64)((a)[(n) + 6]) <<  8) | \
	((u64)((a)[(n) + 7])      )   \
)

#define P_U64_BE(a, n, v)           \
do {                                \
	(a)[(n)    ] = (u8)((v) >> 56); \
	(a)[(n) + 1] = (u8)((v) >> 48); \
	(a)[(n) + 2] = (u8)((v) >> 40); \
	(a)[(n) + 3] = (u8)((v) >> 32); \
	(a)[(n) + 4] = (u8)((v) >> 24); \
	(a)[(n) + 5] = (u8)((v) >> 16); \
	(a)[(n) + 6] = (u8)((v) >>  8); \
	(a)[(n) + 7] = (u8)((v)      ); \
} while (0)

int read_buffer(const char *file_path, u8 **buf, size_t *size)
{
	FILE *fp;
	u8 *file_buf;
	size_t file_size;
	
	if ((fp = fopen(file_path, "rb")) == NULL)
        return -1;
	fseek(fp, 0, SEEK_END);
	file_size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	file_buf = (u8 *)malloc(file_size);
	fread(file_buf, 1, file_size, fp);
	fclose(fp);
	
	if (buf)
        *buf = file_buf;
	else
        free(file_buf);
	if (size)
        *size = file_size;
	
	return 0;
}

int write_buffer(const char *file_path, u8 *buf, size_t size)
{
	FILE *fp;
	
	if ((fp = fopen(file_path, "wb")) == NULL)
        return -1;
	fwrite(buf, 1, size, fp);
	fclose(fp);
	
	return 0;
}
