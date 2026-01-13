#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define ROTL32(x, n) (((x) << (n)) | ((x) >> (32 - (n))))

static uint8_t peek_byte(uint8_t* data, size_t pos) {
    return data[pos];
}

int main() {
    FILE *f = fopen("samples/decrypted", "rb");
    fseek(f, 0, SEEK_END);
    size_t input_len = ftell(f);
    fseek(f, 0, SEEK_SET);
    
    uint8_t *input_data = malloc(input_len);
    fread(input_data, 1, input_len, f);
    fclose(f);
    
    uint8_t *output_data = calloc(1, 0xC800);
    int op_pos = 0;
    int ip_pos = 0;
    int ip_end = input_len;
    int iter_count = 0;
    
    // Read header
    int a = input_data[ip_pos++];
    int b = input_data[ip_pos++];
    int c = (b << 8);
    
    if (a + c + 2 != input_len) {
        printf("Header mismatch!\n");
        return 1;
    }
    
    // Main loop
    while (ip_pos < ip_end) {
        a = input_data[ip_pos++];
        int m_pos = ip_pos + 1;
        iter_count++;
        
        if (iter_count <= 5 || iter_count >= 2257) {
            printf("[%d] ip=%d/%d, op=%d, flag=%02x\n", iter_count, ip_pos-1, ip_end, op_pos, a);
        }
        
        for (int d = 0; d < 8 && ip_pos < ip_end; d++) {
            c = a & 0x80;
            int e = a & 0xFF;
            
            if (c == 0x00 && m_pos < ip_end) {
                a = input_data[ip_pos];
                uint8_t peek = peek_byte(input_data, m_pos);
                int f = (a >> 4) | ((peek << 4) | (peek >> 4));
                c = op_pos - f - 1;
                
                if (c >= 0) {
                    int copy_len = (a & 0xF) + 3;
                    ip_pos++;
                    
                    while (copy_len > 0 && op_pos < 0xC800) {
                        output_data[op_pos++] = output_data[c++];
                        copy_len--;
                    }
                }
            } else if (op_pos < 0xC800) {
                output_data[op_pos++] = input_data[ip_pos++];
                m_pos++;
            }
            
            a = (e << 1) & 0xFE;
        }
        
        if (iter_count == 2260) break;  // Safety limit
    }
    
    printf("\nTotal iterations: %d\n", iter_count);
    printf("Final ip_pos: %d/%d\n", ip_pos, ip_end);
    printf("Final op_pos: %d\n", op_pos);
    
    free(input_data);
    free(output_data);
    return 0;
}
