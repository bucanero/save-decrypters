/*
 * Max Payne 3 Save Data Decompression
 * Based on reference code by Red-EyeX32
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#define MP3_MAX_DECOMPRESSION_BUFFER 0x0C800

// rotl32 is defined in sha1.c which is already included
extern uint32_t rotl32(uint32_t n, uint8_t bits);

/*
 * Mp3_DecompressData
 * 
 * Decompresses Max Payne 3 save data using a custom LZ-style algorithm
 * 
 * Parameters:
 *   data - Input compressed data
 *   len - Length of input data
 *   maxlen - Maximum length of output buffer
 *   outBuf - Output buffer for decompressed data
 *   outLen - Pointer to store actual decompressed size
 * 
 * Returns: 0 on success, -1 on error
 */
int Mp3_DecompressData(const uint8_t* data, int len, int maxlen, uint8_t* outBuf, int* outLen)
{
    if (!data || !outBuf || !outLen || len < 2)
        return -1;

    memset(outBuf, 0, MP3_MAX_DECOMPRESSION_BUFFER);

    int op_pos = 0;          // Output position
    int ip_end = len;
    int pos = 0;             // Input position
    
    // Read first two bytes
    int a = data[pos++];
    int b = data[pos++];
    
    // Calculate initial check value using rotl32
    int c = rotl32(b, 8);
    a += c + 2;
    
    // Verify the compressed data header
    if (a != len)
        return -1;
    
    // Main decompression loop
    while (pos < ip_end)
    {
        // Read control byte
        a = data[pos++];
        int m_pos = pos + 1;  // m_pos points one byte ahead
        
        // Process 8 bits in the control byte
        for (int d = 0; d < 8 && pos < ip_end; d++)
        {
            c = a & 0x80;  // Check high bit
            int e = a & 0xFF;
            
            if (c == 0x00 && m_pos < ip_end)
            {
                // Back-reference: uses 2 consecutive bytes
                a = data[pos++];
                int f = (a >> 4) | rotl32(data[m_pos], 4);
                c = (op_pos - f) - 1;
                
                // Handle invalid offsets: when c < 0, use mod 16
                if (c < 0)
                {
                    f = f % 16;
                    c = op_pos - f - 1;
                }
                
                if (c >= 0 && c < op_pos)
                {
                    // Valid back-reference
                    a = (a & 0xF) + 3;  // Length in lower 4 bits + 3
                    pos++;  // Skip the second byte
                    
                    // Copy bytes from earlier in output
                    while (a-- > 0 && a <= maxlen)
                    {
                        outBuf[op_pos++] = outBuf[c++];
                    }
                }
                else
                {
                    // Invalid back-reference - this shouldn't happen in valid compressed data
                    // But if it does, we need to handle it gracefully
                    // According to reference code, we still consume the bytes
                    a = (a & 0xF) + 3;
                    pos++;  // Skip the second byte
                    
                    // Don't write anything, just advance the output position
                    // This leaves zeros in the buffer (from memset)
                    op_pos += a;
                }
            }
            else if (op_pos < maxlen)
            {
                // Literal byte: copy directly from input
                outBuf[op_pos++] = data[pos++];
                m_pos++;
            }
            
            // Shift control byte left for next bit
            a = (e << 1) & 0xFE;
        }
    }
    
    *outLen = op_pos;
    return 0;
}
