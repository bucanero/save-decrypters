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
    
    // Main loop - trace first 5 iterations
    while (ip_pos < ip_end && iter_count < 5) {
        a = input_data[ip_pos++];
        int m_pos = ip_pos + 1;
        iter_count++;
        
        printf("\n[Iter %d] ip=%d (after reading flag), flag=%02x, m_pos=%d, out_len=%d\n", 
               iter_count, ip_pos, a, m_pos, op_pos);
        
        for (int d = 0; d < 8 && ip_pos < ip_end; d++) {
            c = a & 0x80;
            int e = a & 0xFF;
            
            printf("  Bit %d: c=%02x, ip=%d ", d, c, ip_pos);
            
            if (c == 0x00 && m_pos < ip_end) {
                int byte_val = input_data[ip_pos];
                uint8_t peek = peek_byte(input_data, m_pos);
                int f = (byte_val >> 4) | ((peek << 4) | (peek >> 4));
                c = op_pos - f - 1;
                
                printf("BackRef: byte=%02x, peek=%02x, f=%d, ref_pos=%d", byte_val, peek, f, c);
                
                if (c >= 0) {
                    int copy_len = (byte_val & 0xF) + 3;
                    ip_pos++;
                    printf(" -> Copy %d bytes\n", copy_len);
                    
                    while (copy_len > 0 && op_pos < 0xC800) {
                        output_data[op_pos++] = output_data[c++];
                        copy_len--;
                    }
                } else {
                    printf(" -> Invalid (skipped)\n");
                }
            } else if (op_pos < 0xC800) {
                printf("Literal: %02x\n", input_data[ip_pos]);
                output_data[op_pos++] = input_data[ip_pos++];
                m_pos++;
            } else {
                printf("Output full\n");
            }
            
            a = (e << 1) & 0xFE;
        }
        
        printf("  End of iter: ip=%d, out_len=%d\n", ip_pos, op_pos);
    }
    
    printf("\nFirst 35 output bytes:\n");
    for (int i = 0; i < 35 && i < op_pos; i++) {
        printf("%02x ", output_data[i]);
    }
    printf("\n");
    
    free(input_data);
    free(output_data);
    return 0;
}
