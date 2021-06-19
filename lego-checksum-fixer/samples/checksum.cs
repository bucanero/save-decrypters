/*

Security:
-> Custom Checksum
. . . Offset: 12
. . . Calc. Start: 16
. . . Calc. Length: Filesize - 16

Additional:
This checksum works for all newer LEGO games.
Hobbit use the same one, but save it at 16 and the Calc. Start is 20.

*/

private int CalculateChecksum(byte[] data, int offset, int size) {
    int sum = -1;

    for (int i = 0; i < size; ++i) {
        sum *= 0x1000193;
        sum ^= data[offset++];
    }

    return ~sum;
}
