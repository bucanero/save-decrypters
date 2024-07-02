/*
*	No Mans Sky PS4 Unpacker - (c) 2024 by hzh
*	https://github.com/hzhreal/
*/

#include "../common/iofile.c"
#include "../common/lz4.c"
#include "../common/cJSON.c"

/* 	header: 
	magic value, compressed size, decompressed size, null bytes
	all 4 bytes long (little endian unsigned)
	json keys are obfuscated
*/

#ifndef PATH_TO_MAPPING_JSON
#define PATH_TO_MAPPING_JSON "mapping.json"
#endif

const uint32_t LZ4_MAGIC = 0xFEEDA1E5;
const uint32_t CHUNK_SIZE = 0x80000;

#define CAPACITY_FOR_CHUNKSIZE LZ4_COMPRESSBOUND(CHUNK_SIZE)

size_t find_u32(const uint8_t *data, size_t len, uint32_t value) {
	if (len < sizeof(uint32_t)) {
		return (size_t)-1;
	}

	for (size_t i = 0; i <= len - sizeof(uint32_t); i++) {
		const uint32_t *cur_val = (const uint32_t *)(data + i);
		if (*cur_val == value) {
			return i;
		}
	}
	return (size_t)-1;
}

void obfuscate(cJSON *data, cJSON *mapping) {
	cJSON *child;
	cJSON *next;
	cJSON *key_item;
	cJSON *replacement;
	cJSON *new_item;
	cJSON *value;
	cJSON *element;
	cJSON *mapping_element = NULL;

    if (data->type == cJSON_Object) {
        child = data->child;
        while (child) {  
			next = child->next;

			cJSON_ArrayForEach(mapping_element, mapping) { // iterate through mapping array 
				value = cJSON_GetObjectItem(mapping_element, "Value");
				if (strcmp(value->valuestring, child->string) == 0) {
					replacement = cJSON_GetObjectItem(mapping_element, "Key");
					if (child->string) free(child->string);
					child->string = strdup(replacement->valuestring);
					break; // break array loop after match
				}
			}

			if (child->type == cJSON_Object || child->type == cJSON_Array) {
				obfuscate(child, mapping);
			}
		
		iterate:
			child = next;
		}
    } 
    else if (data->type == cJSON_Array) {
        element = data->child;
        while (element) {
            obfuscate(element, mapping);
            element = element->next;
        }
    }
}

void deobfuscate(cJSON *data, cJSON *mapping) {
	cJSON *child;
	cJSON *next;
	cJSON *key_item;
	cJSON *replacement;
	cJSON *new_item;
	cJSON *key;
	cJSON *element;
	cJSON *mapping_element = NULL;

    if (data->type == cJSON_Object) {
        child = data->child;
        while (child) {  
			next = child->next;

			cJSON_ArrayForEach(mapping_element, mapping) { // iterate through mapping array 
				key = cJSON_GetObjectItem(mapping_element, "Key");
				if (strcmp(key->valuestring, child->string) == 0) {
					replacement = cJSON_GetObjectItem(mapping_element, "Value");
					if (child->string) free(child->string);
					child->string = strdup(replacement->valuestring);
					break; // break array loop after match
				}
			}

			if (child->type == cJSON_Object || child->type == cJSON_Array) {
				deobfuscate(child, mapping);
			}
		
		iterate:
			child = next;
		}
    } 
    else if (data->type == cJSON_Array) {
        element = data->child;
        while (element) {
            deobfuscate(element, mapping);
            element = element->next;
        }
    }
}

int compress(const uint8_t *data, size_t len, uint8_t **comp, size_t *comp_len) {
	int i, quotient, remainder, max_capacity;
	uint32_t written_len;
	*comp_len = 0;

	quotient = len / CHUNK_SIZE;
	remainder = len % CHUNK_SIZE;

	for (i = 0; i < quotient; i++) {
		*comp = (uint8_t *)realloc(*comp, *comp_len + 16 + CAPACITY_FOR_CHUNKSIZE);

		written_len = LZ4_compress_default(
			(const char *)(data + i * CHUNK_SIZE), (char *)(*comp + *comp_len + 16),
			CHUNK_SIZE, CAPACITY_FOR_CHUNKSIZE
		);
		if (written_len < 0) return -1;

		// construct header
		memcpy(*comp + *comp_len + sizeof(uint32_t) * 0, &LZ4_MAGIC, 	sizeof(uint32_t));
		memcpy(*comp + *comp_len + sizeof(uint32_t) * 1, &written_len, 	sizeof(uint32_t));
		memcpy(*comp + *comp_len + sizeof(uint32_t) * 2, &CHUNK_SIZE, 	sizeof(uint32_t));
		memset(*comp + *comp_len + sizeof(uint32_t) * 3, 0, 			sizeof(uint32_t));

		*comp_len += 16 + written_len;
	}

	if (remainder > 0) {
		max_capacity = LZ4_compressBound(remainder);
		*comp = (uint8_t *)realloc(*comp, *comp_len + 16 + max_capacity);

		written_len = LZ4_compress_default(
			(const char *)(data + i * CHUNK_SIZE), (char *)(*comp + *comp_len + 16),
			remainder, max_capacity
		);
		if (written_len < 0) return -1;

		// construct header
		memcpy(*comp + *comp_len + sizeof(uint32_t) * 0, &LZ4_MAGIC, 	sizeof(uint32_t));
		memcpy(*comp + *comp_len + sizeof(uint32_t) * 1, &written_len, 	sizeof(uint32_t));
		memcpy(*comp + *comp_len + sizeof(uint32_t) * 2, &remainder, 	sizeof(uint32_t));
		memset(*comp + *comp_len + sizeof(uint32_t) * 3, 0, 			sizeof(uint32_t));

		*comp_len += 16 + written_len;
	}

	*comp = (uint8_t *)realloc(*comp, *comp_len);

	return 0;
}

int decompress(const uint8_t *data, size_t len, uint8_t **decomp, size_t *decomp_len) {
	*decomp_len = 0;
	size_t off = 0;
	uint32_t chunk_compsize, chunk_decompsize;

	while (1) {
		off = find_u32(data, len, LZ4_MAGIC);
		if (off == (size_t)-1) {
			break;
		}

		chunk_compsize   = *(uint32_t *)(data + sizeof(uint32_t) * 1);
		chunk_decompsize = *(uint32_t *)(data + sizeof(uint32_t) * 2);

		*decomp = (uint8_t *)realloc(*decomp, *decomp_len + chunk_decompsize);
		
		if (LZ4_decompress_safe(
			(const char *)(data + sizeof(uint32_t) * 4), (char *)(*decomp + *decomp_len), 
			chunk_compsize, chunk_decompsize) != chunk_decompsize) 
		{
			return -1;
		}

		data += sizeof(uint32_t) * 4 + chunk_compsize;
		len -= sizeof(uint32_t) * 4 + chunk_compsize;
		*decomp_len += chunk_decompsize;
	}
	return 0;
}

void print_usage(const char* argv0)
{
	printf("USAGE: %s [option] filename\n\n", argv0);
	printf("OPTIONS        Explanation:\n");
	printf(" -u            Unpack File\n");
	printf(" -p            Pack File\n\n");
	return;
}

int main(int argc, char **argv)
{
	uint8_t *data;
	size_t len;
	char *opt, *bak;
	int ret = 0;

	printf("\nNo Man's Sky (PS4) unpacker\n\n");
 
	if (--argc < 2)
	{
		print_usage(argv[0]);
		return -1;
	}
	char *filename = argv[2];

	opt = argv[1];
	if (*opt++ != '-' || (*opt != 'u' && *opt != 'p'))
	{
		print_usage(argv[0]);
		return -1;
	}

	if (read_buffer(filename, &data, &len) != 0)
	{
		printf("[*] Could Not Access The File (%s)\n", filename);
		return -1;
	}
	// Save a file backup
	asprintf(&bak, "%s.bak", filename);
	write_buffer(bak, data, len);

	// load mapping.json
	char *mapping;
	size_t mapping_len;
	cJSON *mapping_json_root, *mapping_json;

	read_buffer(PATH_TO_MAPPING_JSON, (uint8_t **)&mapping, &mapping_len);
	mapping_json_root = cJSON_ParseWithLength((const char *)mapping, mapping_len);
	free(mapping); // dont need this anymore
	mapping_json = cJSON_GetObjectItem(mapping_json_root, "Mapping");

	// decompress or compress file
	if (*opt == 'u') {
		uint8_t *decomp;
		size_t decomp_len;

		if (decompress(data, len, &decomp, &decomp_len) != 0) {
			printf("Decompression failed!\n");
			if (decomp) free(decomp);
			ret = -1;
			goto exit;
		}

		cJSON *decomp_json;
		char *deobfuscated_data;

		decomp_json = cJSON_ParseWithLength((const char *)decomp, decomp_len);
		if (!decomp_json) {
			printf("Decompressed save is not JSON!\n");
			free(decomp);
			ret = -1;
			goto exit;
		}
		deobfuscate(decomp_json, mapping_json);
		deobfuscated_data = cJSON_PrintUnformatted(decomp_json);

		write_buffer(filename, (uint8_t *)deobfuscated_data, strlen(deobfuscated_data));
		printf("Successfully unpacked %s!\n", filename);

		free(decomp);
		free(deobfuscated_data);
		cJSON_Delete(decomp_json);
	}
	else {
		cJSON *comp_json;
		char *obfuscated_data;

		comp_json = cJSON_ParseWithLength((const char *)data, len);
		if (!comp_json) {
			printf("Save is not JSON!\n");;
			ret = -1;
			goto exit;
		}

		obfuscate(comp_json, mapping_json);
		obfuscated_data = cJSON_PrintUnformatted(comp_json);
		
		uint8_t *comp;
		size_t comp_len;

		if (compress((const uint8_t *)obfuscated_data, strlen(obfuscated_data), &comp, &comp_len) != 0) {
			printf("Compression failed!\n");
			free(obfuscated_data);
			if (comp) free(comp);
			ret = -1;
			goto exit;
		}

		write_buffer(filename, comp, comp_len);
		printf("Successfully packed %s!\n", filename);

		free(comp);
		free(obfuscated_data);
		cJSON_Delete(comp_json);
	}

exit:
	if (data)
		free(data);
	if (bak)
		free(bak);
	if (mapping_json_root)
		cJSON_Delete(mapping_json_root);
	return ret;
}