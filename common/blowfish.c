#include "blowfish.h"

static uint8_t key_buffer[KEYSIZE];

void crypt_64bit_up(const uint32_t* keybuf, uint32_t* ptr)
{
	uint32_t x = ptr[0];
	uint32_t y = ptr[1];
	uint32_t z;
	int i;

	for (i = 0; i < 0x10; i++) {
		z = keybuf[i] ^ x;
		x = keybuf[0x012 + ((z>>24)&0xff)];
		x = keybuf[0x112 + ((z>>16)&0xff)] + x;
		x = keybuf[0x212 + ((z>> 8)&0xff)] ^ x;
		x = keybuf[0x312 + ((z>> 0)&0xff)] + x;
		x = y ^ x;
		y = z;
	}

	ptr[1] = x ^ keybuf[0x10];
	ptr[0] = y ^ keybuf[0x11];
}

void crypt_64bit_down(const uint32_t* keybuf, uint32_t* ptr)
{
	uint32_t x = ptr[0];
	uint32_t y = ptr[1];
	uint32_t z;
	int i;

	for (i = 0x11; i > 0x01; i--) {
		z = keybuf[i] ^ x;
		x = keybuf[0x012 + ((z>>24)&0xff)];
		x = keybuf[0x112 + ((z>>16)&0xff)] + x;
		x = keybuf[0x212 + ((z>> 8)&0xff)] ^ x;
		x = keybuf[0x312 + ((z>> 0)&0xff)] + x;
		x = y ^ x;
		y = z;
	}

	ptr[1] = x ^ keybuf[0x01];
	ptr[0] = y ^ keybuf[0x00];
}

void apply_keycode(uint32_t* keybuf, const uint32_t* keydata, const char* keycode)
{
	int i;
	uint32_t scratch[2] = {0, 0};
	char tmp[4];
	int len = strlen(keycode);

	memcpy(keybuf + 0x12, keydata, 0x1000);

	for (i = 0; i < 0x12; i++)
	{
		//Little-Endian
		tmp[3]=keycode[(i*4   ) % len];
		tmp[2]=keycode[(i*4 +1) % len];
		tmp[1]=keycode[(i*4 +2) % len];
		tmp[0]=keycode[(i*4 +3) % len];

		keybuf[i] = keydata[0x400 + i] ^ *(uint32_t*)tmp;
	}

	for (i = 0; i < 0x412; i += 2)
	{
		crypt_64bit_up(keybuf, scratch);
		keybuf[i] = scratch[0];
		keybuf[i+1] = scratch[1];
	}
}

void blowfish_init_key(const char* key)
{
	apply_keycode((uint32_t*) key_buffer, (uint32_t*) KEY_DATA, key);
}

void blowfish_decrypt_buffer(void* bf_data, uint32_t size)
{
	uint32_t buf[2];
	uint32_t *data = bf_data;
	size /= 4;

	for (int i = 0; i < size; i+= 2)
	{
		buf[0] = ES32(data[i]);
		buf[1] = ES32(data[i+1]);
		crypt_64bit_down((uint32_t*) key_buffer, buf);

		data[i] = ES32(buf[0]);
		data[i+1] = ES32(buf[1]);
	}
}

void blowfish_encrypt_buffer(void* bf_data, uint32_t size)
{
	uint32_t buf[2];
	uint32_t *data = bf_data;
	size /= 4;

	for (int i = 0; i < size; i+= 2)
	{
		buf[0] = ES32(data[i]);
		buf[1] = ES32(data[i+1]);
		crypt_64bit_up((uint32_t*) key_buffer, buf);

		data[i] = ES32(buf[0]);
		data[i+1] = ES32(buf[1]);
	}
}

void blowfish_decrypt_buffer_cbc(void *bf_data, uint32_t size, uint64_t iv)
{
	uint32_t buf[2];
	uint32_t *data = bf_data;
	size /= 4;
	uint32_t l, r;
	uint32_t l_ = (uint32_t)(iv >> 32);
	uint32_t r_ = (uint32_t)(iv & 0xFFFFFFFF);

	for (int i = 0; i < size; i += 2)
	{
		buf[0] = ES32(data[i]);
		buf[1] = ES32(data[i+1]);
		l = buf[0];
		r = buf[1];

		crypt_64bit_down((uint32_t *)key_buffer, buf);
		data[i] = ES32(buf[0] ^ l_);
		data[i+1] = ES32(buf[1] ^ r_);

		l_ = l;
		r_ = r;
	}
}

void blowfish_encrypt_buffer_cbc(void *bf_data, uint32_t size, uint64_t iv)
{
	uint32_t buf[2];
	uint32_t *data = bf_data;
	size /= 4;
	uint32_t l_ = (uint32_t)(iv >> 32);
	uint32_t r_ = (uint32_t)(iv & 0xFFFFFFFF);

	for (int i = 0; i < size; i += 2)
	{
		buf[0] = ES32(data[i]) ^ l_;
		buf[1] = ES32(data[i+1]) ^ r_;

		crypt_64bit_up((uint32_t *)key_buffer, buf);
		data[i] = ES32(buf[0]);
		data[i+1] = ES32(buf[1]);

		l_ = ES32(data[i]);
		r_ = ES32(data[i+1]);
	}
}
