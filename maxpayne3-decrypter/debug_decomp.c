#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define ROTL32(x, n) (((x) << (n)) | ((x) >> (32 - (n))))

static uint8_t peek_byte(uint8_t* data, size_t pos)
{
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
    
    // Read header
    int a = input_data[ip_pos++];
    int b = input_data[ip_pos++];
    int c = (b << 8);
    
    printf("Header check: %d + %d + 2 = %d (expected %zu)\n", a, c, a+c+2, input_len);
    
    if (a + c + 2 != input_len) {
        printf("Header mismatch!\n");
        return 1;
    }
    
    // Process first 2 flag bytes
    for (int iter = 0; iter < 2 && ip_pos < ip_end; iter++) {
        a = input_data[ip_pos++];
        int m_pos = ip_pos + 1;
        
        printf("\n[Iter %d] Flag byte at pos %d: %02x, m_pos=%d\n", iter, ip_pos-1, a, m_pos);
        
        for (int d = 0; d < 8 && ip_pos < ip_end; d++) {
            c = a & 0x80;
            int e = a & 0xFF;
            
            printf("  Bit %d: c=%02x, a=%02x, ip_pos=%d ", d, c, a, ip_pos);
            
            if (c == 0x00 && m_pos < ip_end) {
                // Back reference
                a = input_data[ip_pos];
                uint8_t peek = peek_byte(input_data, m_pos);
                int f = (a >> 4) | ((peek << 4) | (peek >> 4));
                c = op_pos - f - 1;
                
                printf("BackRef: a=%02x, peek=%02x, f=%d, c=%d\n", a, peek, f, c);
                
                if (c >= 0) {
                    int copy_len = (a & 0xF) + 3;
                    ip_pos++;
                    
                    while (copy_len > 0 && op_pos < 0xC800) {
                        output_data[op_pos++] = output_data[c++];
                        copy_len--;
                    }
                }
            } else if (op_pos < 0xC800) {
                // Literal
                printf("Literal: %02x\n", input_data[ip_pos]);
                output_data[op_pos++] = input_data[ip_pos++];
                m_pos++;
            }
            
            a = (e << 1) & 0xFE;
        }
    }
    
    printf("\nFirst 32 output bytes:\n");
    for (int i = 0; i < 32 && i < op_pos; i++) {
        printf("%02x ", output_data[i]);
        if ((i+1) % 16 == 0) printf("\n");
    }
    printf("\n");
    
    free(input_data);
    free(output_data);
    return 0;
}
