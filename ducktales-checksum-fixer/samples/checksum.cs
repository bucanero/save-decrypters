private ulong CalculateChecksum(byte[] data, int offset, int size) {
    uint  r1, r2, r3, r4, r5, r6, r7, r8;

    r4 = (uint) size + 0xDEADC3C1;
    r6 = r4 + 0x162E;
    r5 = r4;
                
    if (size > 12) {
        for (int i = size / 12; i > 0; --i) {
            r7 =  (uint) (data[offset++] + (data[offset++] << 8) + (data[offset++] << 16) + (data[offset++] << 24));
            r4 += (uint) (data[offset++] + (data[offset++] << 8) + (data[offset++] << 16) + (data[offset++] << 24));
            r6 += (uint) (data[offset++] + (data[offset++] << 8) + (data[offset++] << 16) + (data[offset++] << 24));  
                 
            r5 = (r5 + r7 - r6) ^ ((r6 << 4) | (r6 >> 28));
            r2 = r4 - r5;                      
            r4 += r6;
            r6 = r2 ^ ((r5 << 6) | (r5 >> 26));
            r5 += r4;         
            r3 = r4 - r6;                  
            r4 = r6 + r5;
            r6 = r3 ^ ((r6 << 8) | (r6 >> 24));                                   
            r3 = r4 + r6;
            r6 = (r5 - r6) ^ ((r6 << 16) | (r6 >> 16));                                  
            r5 = r3 + r6;
            r6 = (r4 - r6) ^ ((r6 >> 13) | (r6 << 19));      
            r4 = r5 + r6;
            r6 = (r3 - r6) ^ ((r6 <<  4) | (r6 >> 28));      
        }

        for (int i = 0; i < 4; ++i)
            r5 += (uint) data[offset++] << (i * 8);

        uint x0, x1;
        r3 = (r6 ^ r4) - ((r4 << 14) | (r4 >> 18));    
        r1 = (r5 ^ r3) - ((r3 << 11) | (r3 >> 21));        
        r4 = (r4 ^ r1) - ((r1 >>  7) | (r1 << 25));
        r6 = (r3 ^ r4) - ((r4 >> 16) | (r4 << 16));
        r8 = (r1 ^ r6) - ((r6 <<  4) | (r6 >> 28));
        r5 = (r4 ^ r8) - ((r8 << 14) | (r8 >> 18));
        x1 = (r6 ^ r5) - ((r5 >>  8) | (r5 << 24));
        x0 = r5;
                
        return ((ulong) x1 << 32) | x0;
    }
    return 0;
}
