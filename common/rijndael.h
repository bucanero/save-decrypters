/* $Id: rijndael.h 1.17 2018-09-19 09:58:50-04 Ron Exp $ */
#ifndef RIJNDAEL_H_
#define RIJNDAEL_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	uint32_t erk[128];	/* encryption round keys */
	uint32_t drk[128];	/* decryption round keys */
	int nr;				/* number of rounds */
	int blocklen;		/* length in bytes of an input and output block */
} rijn_context;

int rijn_set_key( rijn_context *ctx, uint8_t *key, int nkeybits,
				int nblockbits );

void rijn_encrypt( rijn_context *ctx, uint8_t *input, uint8_t *output );

void rijn_decrypt( rijn_context *ctx, uint8_t *input, uint8_t *output );

int rijn_cbc_encrypt( rijn_context *ctx, uint8_t *iv, uint8_t *input,
						uint8_t *output, size_t nbytes );

int rijn_cbc_decrypt( rijn_context *ctx, uint8_t *iv, uint8_t *input,
						uint8_t *output, size_t nbytes );

/* AES equivalent defines */
#define aes_set_key(ctx, key, nkeybits) rijn_set_key(ctx, key, nkeybits, 128)

#define aes_encrypt(ctx, input, output) rijn_encrypt(ctx, input, output)

#define aes_decrypt(ctx, input, output) rijn_decrypt(ctx, input, output)

#define aes_cbc_encrypt(ctx, iv, input, output, nbytes) \
					rijn_cbc_encrypt(ctx, iv, input, output, nbytes)

#define aes_cbc_decrypt(ctx, iv, input, output, nbytes) \
					rijn_cbc_decrypt(ctx, iv, input, output, nbytes)

#define aes_context rijn_context

#ifdef __cplusplus
}
#endif

#endif /* RIJNDAEL_H_ */



