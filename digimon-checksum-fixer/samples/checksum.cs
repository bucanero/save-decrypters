/*

Security:
-> Checksum32 (Signed)
. . . Offset: 20
. . . Calc. Start: 0
. . . Calc. Length: 0x185E

Additional:
Before the new checksum is calculated, the old one is overwritten with 0.

*/

private int CalculateChecksum32(byte[] data, int offset, int size) {
    int sum = 0;

    while (size-- > 0)
        sum += (int) (sbyte) data[offset++];

    return sum;
}
