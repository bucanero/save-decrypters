/*
Philymaster’s Security Archive
https://community.wemod.com/t/philymasters-security-archive/3923

-> Custom Checksum(s)
. . . Offset: Stored after every 16 Byte chunk. (offset 16, 36, 56, …, EOF - 4)
. . . Calc. Start: First Byte of chunk.
. . . Calc. Length: 16

Additional:
You can calculate the checksum count with: Filesize / 20.

Like mentioned the game saves after every 16 Byte chunk of save data an Int32 value, which consist of two Int16 values.
The first is just the sum of all bytes, whilst the second the sum of all even bytes minus the sum of all uneven bytes is.

Here the formula:

First: x[0] + x[1] + …
Second: x[0] - x[1] + x[2] - x[3] + x[4] - x[5] + x[6] - x[7] + x[8] - …
*/

public unsafe void FixHashes(byte[] data, int offset, int size) {
    fixed (byte* x = data) {
        byte* e = x + offset + size;
                
        ushort sumL, sumH;
        for (byte* s = e - size; s < e; s += 4) {

            sumL = sumH = 0;
            for (int n = 0; n < 16; ++n, ++s) {
                sumL += *s;
                sumH += (ushort) ((n % 2) == 0 ? *s : -*s);
            }

            *(ushort*) s = sumL.Swap();
            *(ushort*) (s + 2) = sumH.Swap();
        }
    }
}