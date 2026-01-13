#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define ROTL32(x, n) (((x) << (n)) | ((x) >> (32 - (n))))

int main() {
    FILE *f = fopen("samples/decrypted", "rb");
    uint8_t data[100];
    fread(data, 1, 100, f);
    fclose(f);
    
    printf("First bytes: %02x %02x %02x %02x %02x\n", 
           data[0], data[1], data[2], data[3], data[4]);
    
    int a = data[0];
    int b = data[1];
    int c = (b << 8);
    
    printf("a=%d, b=%d, c=%d\n", a, b, c);
    printf("a + c + 2 = %d\n", a + c + 2);
    
    // Start decompression
    int ip = 2;
    uint8_t output[100];
    int op = 0;
    
    a = data[ip++];
    int m_pos = ip + 1;
    
    printf("\nFirst flag byte: %02x\n", a);
    
    for (int d = 0; d < 8; d++) {
        c = a & 0x80;
        int e = a & 0xFF;
        
        printf("Bit %d: c=%02x, ", d, c);
        
        if (c == 0x00) {
            printf("back ref\n");
            break;
        } else {
            printf("literal %02x\n", data[ip]);
            output[op++] = data[ip++];
            m_pos++;
        }
        
        a = (e << 1) & 0xFE;
    }
    
    printf("\nOutput: ");
    for (int i = 0; i < op; i++)
        printf("%02x ", output[i]);
    printf("\n");
    
    return 0;
}
