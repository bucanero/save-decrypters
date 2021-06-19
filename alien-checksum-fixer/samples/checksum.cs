byte[] data = File.ReadAllBytes(o.FileName);

int calcLen;
using (var r = new Reader(data, Endian.BIG)) {
    r.Position = 24;
    calcLen = r.ReadInt32() + r.ReadInt32() - 32;

    byte[] hash = new byte[24];
    using (var sha = new SHA1Managed()) {
        sha.TransformFinalBlock(data, 32, calcLen);
        Buffer.BlockCopy(sha.Hash, 0, hash, 0, 20);
    }

    long checksum = 0;
    fixed (byte* x = hash) {
        long* hashP = (long*) x;

        for (int i = 0; i < 3; ++i, ++hashP) {
            checksum ^= *hashP;
        }
    }
}
