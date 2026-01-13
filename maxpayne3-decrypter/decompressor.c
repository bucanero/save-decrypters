/*
 *
 *	Max Payne 3 Save Decompressor - (c) 2025 by Bucanero
 *
 * This tool decompresses Max Payne 3 decrypted save data based on the
 * Decompress.cs reference implementation.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define ROTL32(x, n) (((x) << (n)) | ((x) >> (32 - (n))))

// Read byte at specific position without advancing the file pointer
static uint8_t peek_byte(uint8_t* data, size_t pos)
{
    return data[pos];
}

// Decompression function based on Mp3_DecompressData from Decompress.cs
static int Mp3_DecompressData(uint8_t* input_data, int input_len, int max_output_len, 
                              uint8_t* output_data, int* output_len)
{
    int ip_pos = 0;      // input position
    int op_pos = 0;      // output position
    int ip_end = input_len;
    int a, b, c, e, f, m_pos;
    
    // Read first two bytes
    if (ip_pos + 1 >= ip_end)
        return -1;
    
    a = input_data[ip_pos++];
    b = input_data[ip_pos++];
    
    // ROTL32(b, 8) for byte value is equivalent to (b << 8)
    c = (b << 8);
    
    // Validate header
    a += c + 2;
    if (a != input_len)
    {
        return -1;
    }
    
    // Main decompression loop
    while (ip_pos < ip_end)
    {
        a = input_data[ip_pos++];
        m_pos = ip_pos + 1;
        
        // Process 8 bits
        for (int d = 0; d < 8 && ip_pos < ip_end; d++)
        {
            c = a & 0x80;
            e = a & 0xFF;
            
            // Check if bit is 0 (back reference)
            if (c == 0x00 && m_pos < ip_end)
            {
                if (ip_pos >= ip_end)
                    break;
                
                a = input_data[ip_pos];
                
                // Calculate offset: f = (a >> 4) | ROTL32(peek_byte(m_pos), 4)
                // ROTL32(x, 4) for 8-bit value rotated in 8-bit space: (x << 4) | (x >> 4)
                uint8_t peek = peek_byte(input_data, m_pos);
                f = (a >> 4) | ((peek << 4) | (peek >> 4));
                
                // Calculate back reference position
                c = op_pos - f - 1;
                
                // Validate back reference is within bounds
                if (c >= 0 && c < op_pos)
                {
                    // Calculate length: (a & 0xF) + 3
                    int copy_len = (a & 0xF) + 3;
                    ip_pos++;
                    
                    // Copy from back reference
                    while (copy_len > 0 && op_pos < max_output_len)
                    {
                        output_data[op_pos++] = output_data[c++];
                        copy_len--;
                    }
                }
            }
            else if (op_pos < max_output_len)
            {
                // Literal byte - copy directly
                if (ip_pos >= ip_end)
                    break;
                
                output_data[op_pos++] = input_data[ip_pos++];
                m_pos++;
            }
            
            // Shift bit flag left
            a = (e << 1) & 0xFE;
        }
    }
    
    *output_len = op_pos;
    return 0;
}

// Compare two files
static int compare_files(const char* file1, const char* file2)
{
    FILE *f1 = fopen(file1, "rb");
    FILE *f2 = fopen(file2, "rb");
    
    if (!f1 || !f2)
    {
        if (f1) fclose(f1);
        if (f2) fclose(f2);
        return -1;
    }
    
    // Get file sizes
    fseek(f1, 0, SEEK_END);
    long size1 = ftell(f1);
    fseek(f1, 0, SEEK_SET);
    
    fseek(f2, 0, SEEK_END);
    long size2 = ftell(f2);
    fseek(f2, 0, SEEK_SET);
    
    if (size1 != size2)
    {
        printf("[*] File size mismatch: %ld vs %ld bytes\n", size1, size2);
        fclose(f1);
        fclose(f2);
        return 1;
    }
    
    // Compare contents
    uint8_t buf1[4096], buf2[4096];
    size_t bytes_read1, bytes_read2;
    int result = 0;
    
    while ((bytes_read1 = fread(buf1, 1, sizeof(buf1), f1)) > 0)
    {
        bytes_read2 = fread(buf2, 1, sizeof(buf2), f2);
        
        if (bytes_read1 != bytes_read2 || memcmp(buf1, buf2, bytes_read1) != 0)
        {
            result = 1;
            break;
        }
    }
    
    fclose(f1);
    fclose(f2);
    
    return result;
}

int main(int argc, char** argv)
{
    FILE *input_file, *output_file;
    uint8_t *input_data, *output_data;
    size_t input_size;
    int output_size;
    int result;
    
    printf("\nmax-payne3-decompressor 0.1.0 - (c) 2025 by Bucanero\n\n");
    
    if (argc < 3)
    {
        printf("USAGE: %s <input_file> <output_file>\n", argv[0]);
        printf("\n");
        printf("Example: %s decrypted decrypted-decomp\n", argv[0]);
        return -1;
    }
    
    // Read input file
    input_file = fopen(argv[1], "rb");
    if (!input_file)
    {
        printf("[!] Could not open input file: %s\n", argv[1]);
        return -1;
    }
    
    fseek(input_file, 0, SEEK_END);
    input_size = ftell(input_file);
    fseek(input_file, 0, SEEK_SET);
    
    printf("[*] Input file size: %zu bytes\n", input_size);
    
    input_data = malloc(input_size);
    if (!input_data)
    {
        printf("[!] Memory allocation failed\n");
        fclose(input_file);
        return -1;
    }
    
    if (fread(input_data, 1, input_size, input_file) != input_size)
    {
        printf("[!] Failed to read input file\n");
        free(input_data);
        fclose(input_file);
        return -1;
    }
    fclose(input_file);
    
    // Allocate output buffer (max 0xC7BC as per Decompress.cs comment)
    int max_output_size = 0xC7BC;
    output_data = malloc(max_output_size);
    if (!output_data)
    {
        printf("[!] Memory allocation failed for output buffer\n");
        free(input_data);
        return -1;
    }
    
    // Decompress
    printf("[*] Decompressing data...\n");
    result = Mp3_DecompressData(input_data, input_size, max_output_size, 
                                 output_data, &output_size);
    
    if (result != 0)
    {
        printf("[!] Decompression failed\n");
        free(input_data);
        free(output_data);
        return -1;
    }
    
    printf("[*] Decompressed size: %d bytes\n", output_size);
    
    // Write output file
    output_file = fopen(argv[2], "wb");
    if (!output_file)
    {
        printf("[!] Could not create output file: %s\n", argv[2]);
        free(input_data);
        free(output_data);
        return -1;
    }
    
    if (fwrite(output_data, 1, output_size, output_file) != (size_t)output_size)
    {
        printf("[!] Failed to write output file\n");
        fclose(output_file);
        free(input_data);
        free(output_data);
        return -1;
    }
    fclose(output_file);
    
    printf("[*] Successfully wrote output to: %s\n", argv[2]);
    
    // Check if reference file exists for validation
    char reference_file[256];
    snprintf(reference_file, sizeof(reference_file), "samples/%s", argv[2]);
    
    FILE* ref = fopen(reference_file, "rb");
    if (ref)
    {
        fclose(ref);
        printf("[*] Validating against reference file...\n");
        
        if (compare_files(argv[2], reference_file) == 0)
        {
            printf("[*] SUCCESS: Output matches reference file!\n");
        }
        else
        {
            printf("[!] WARNING: Output does not match reference file\n");
        }
    }
    
    free(input_data);
    free(output_data);
    
    return 0;
}
