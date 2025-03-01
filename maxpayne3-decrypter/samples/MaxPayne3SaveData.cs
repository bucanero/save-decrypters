using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Security.Cryptography;

namespace MaxPayne3
{
    public class MaxPayne3SaveData
    {
        public enum MP3FileType
        {
            SP = 0x06
        }

        private EndianIO IO;

        private int Version;
        private int HeaderLength;

        private byte[] ProfileKey;

        // Max Payne 3 Save data encryption key
        private static readonly byte[] EncAesKey = new byte[0x20] {
            0x1A, 0xB5, 0x6F, 0xED, 0x7E, 0xC3, 0xFF, 0x01, 0x22, 0x7B, 0x69, 0x15, 0x33, 0x97, 0x5D, 0xCE,
            0x47, 0xD7, 0x69, 0x65, 0x3F, 0xF7, 0x75, 0x42, 0x6A, 0x96, 0xCD, 0x6D, 0x53, 0x07, 0x56, 0x5D
        };

        public MaxPayne3SaveData(EndianIO io)
        {
            if (io == null)
                return;

            IO = io;

            Read();
        }

        private void Read()
        {
            IO.In.BaseStream.Position = 0;

            if (IO.In.ReadUInt32() != 0x6D703300 || (HeaderLength = IO.In.ReadInt32()) < 0x1A)
                throw new MaxPayne3Exception("invalid header detected.");

            var saveBufferLen = IO.In.ReadInt32();
            var saveDataLen = IO.In.ReadInt32();

            if ((saveBufferLen + HeaderLength) != saveDataLen)
                throw new MaxPayne3Exception("invalid save size detected.");

            int storedSum = IO.In.SeekNReadInt32(0x10), sum = 0;
            Version = IO.In.SeekNReadInt32(0x14);

            IO.In.SeekTo(0x44);

            for (var x = 0; x < saveBufferLen; x++)
                sum += IO.In.ReadByte();

            if (sum != storedSum)
                throw new MaxPayne3Exception("save has been tampered with!");

            // Create the save key
            // PlayStation: PS35675hA
            // XBOX: XEN43156A
            // PC: PCgh64rwA
            var strProfileKey = string.Format("PS35675hA{0:D2}{1:D1}", HeaderLength, Version);
            ProfileKey = new byte[0x32];

            Array.Copy(Encoding.ASCII.GetBytes(strProfileKey), 0, ProfileKey, 0, strProfileKey.Length);

            IO.SeekTo(0x18);
            byte[] cData = DecryptSaveData(IO.In.ReadBytes(0x2C + saveBufferLen), saveBufferLen);
            //var ms = new MemoryStream();
        }

        private byte[] DecryptSaveData(byte[] saveBuffer, int saveLen)
        {
            byte[] pbData1 = saveBuffer.ReadBytes(0, 0x2C);
            byte[] saveData = saveBuffer.ReadBytes(0x2C, saveLen);
            byte[] pbSaveDataLen = Global.convertToBigEndian(BitConverter.GetBytes(saveLen));

            HMACSHA1 sha = new HMACSHA1(ProfileKey);
            sha.TransformBlock(saveData, 0, saveLen, null, 0x00);
            sha.TransformFinalBlock(pbSaveDataLen, 0, 0x04);

            byte[] digestKey = sha.Hash;
            byte[] hmacBuffer = new byte[0x10] { 0x0F, 0xC9, 0x19, 0xE8, 0x9A, 0x17, 0xC4, 0x5F, 0xE7, 0x16, 0xD4, 0x6C, 0x3A, 0x15, 0x9C, 0x75 };
            byte[] aesKey = Mp3_HmacSha(digestKey, 0x14, hmacBuffer, 0x10, 0x20, 0x7D0);

            digestKey = Mp3_AesEcb(aesKey, pbData1, 0x2C, false);

            hmacBuffer = new byte[0x10] { 0xE1, 0x09, 0xA5, 0x42, 0xF6, 0x0A, 0x13, 0x3B, 0x81, 0xAC, 0x02, 0x55, 0xCC, 0x39, 0x40, 0x1B };
            byte[] hmacDigest = Mp3_HmacSha(digestKey, 0x2C, hmacBuffer, 0x10, 0x20, 0x7D0);

            hmacDigest = Mp3_AesEcb(EncAesKey, hmacDigest, 0x20, true);

            saveData = Mp3_AesEcb2(hmacDigest, saveData, saveLen, false);

            // verifying the data
            SHA1 hasher = SHA1.Create();
            hasher.TransformBlock(ProfileKey, 0, 0x32, null, 0);
            hasher.TransformFinalBlock(pbSaveDataLen, 0, 4);

            Array.Copy(HmacSha(hasher.Hash, saveData), 0, hmacDigest, 0, 0x14);
            Array.Copy(digestKey, 0x20, hmacDigest, 0x14, 0x04);
            Array.Copy(digestKey, 0x24, hmacDigest, 0x18, 0x08);

            hmacBuffer = new byte[0x10] { 0x15, 0x08, 0xE9, 0x6F, 0x47, 0xB8, 0x47, 0xD1, 0x3A, 0x65, 0x8C, 0x71, 0x00, 0x00, 0x00, 0x00 };
            hmacBuffer.WriteInt32(0x0C, digestKey.ReadInt32(0x20));

            aesKey = Mp3_HmacSha(hmacDigest, 0x20, hmacBuffer, 0x10, 0x20, 0x7D0);
            var pbData2 = new byte[0x20];
            Array.Copy(digestKey, 0, pbData2, 0, 0x20);
            pbData2 = Mp3_AesEcb(EncAesKey, pbData2, 0x20, false);
            pbData2 = Mp3_AesEcb(aesKey, pbData2, 0x20, false);

            if (!Global.ArrayEquals(pbData2, hmacDigest))
                throw new MaxPayne3Exception("save data could not be verified.");

            return saveData;
        }

        private byte[] Mp3_HmacSha(byte[] pbKey, int cbKey, byte[] pbInp, int cbInp, int cbOut, int cbRounds)
        {
            if (cbOut == 0x00)
                return null;

            var hash = new byte[cbOut];
            int idx = 1, len = cbOut, offset = 0;

            do
            {
                var pbData1 = new byte[0x04];
                pbData1.WriteInt32(0, idx);
                byte[] sKey;
                if (pbKey.Length != cbKey)
                {
                    sKey = new byte[cbKey];
                    Array.Copy(pbKey, 0, sKey, 0, cbKey);
                }
                else
                {
                    sKey = pbKey;
                }
                var sha = new HMACSHA1(sKey);
                sha.TransformBlock(pbInp, 0, cbInp, null, 0x00);
                sha.TransformFinalBlock(pbData1, 0, 0x04);
                byte[] tmpHash = sha.Hash;
                byte[] digest = tmpHash;

                if (cbRounds > 1)
                {
                    int rounds = cbRounds - 1;
                    do
                    {
                        sha = new HMACSHA1(sKey);
                        sha.TransformFinalBlock(digest, 0, 0x14);
                        digest = sha.Hash;

                        int j = 0;
                        for (int x = 0; x < 4; x++)
                        {
                            int a = digest[j];
                            int b = tmpHash[j];
                            int c = tmpHash[j + 1];
                            int d = digest[j + 1];
                            int e = tmpHash[j + 2];
                            a ^= b;
                            int f = tmpHash[j + 3];
                            b = digest[j + 3];
                            d ^= c;
                            int g = digest[j + 2];
                            int h = tmpHash[j + 4];
                            b ^= f;
                            c = digest[j + 4];
                            g ^= e;
                            tmpHash[j] = (byte)(a);
                            a = b & 0xFF;
                            h ^= c;
                            tmpHash[j + 1] = (byte)(d);
                            int i = g & 0xFF;
                            tmpHash[j + 3] = (byte)a;
                            d = h & 0xFF;
                            tmpHash[j + 2] = (byte)i;
                            tmpHash[j + 4] = (byte)d;
                            j += 5;
                        }

                    } while (--rounds != 0);
                }

                int dlen = len < 0x14 ? len : 0x14;

                idx++;
                Array.Copy(tmpHash, 0, hash, offset, dlen);
                offset += dlen;
                len -= dlen;

            } while (len != 0);

            return hash;
        }

        private byte[] HmacSha(byte[] key, byte[] buffer)
        {
            var sha = new HMACSHA1(key);
            sha.TransformFinalBlock(buffer, 0, buffer.Length);
            return sha.Hash;
        }

        private static byte[] Mp3_AesEcb(byte[] key, byte[] pbInp, int cbInp, bool encrypt)
        {
            // Create our Rijndael class
            Rijndael rj = Rijndael.Create();
            rj.BlockSize = 128;
            rj.KeySize = 256;
            rj.Mode = CipherMode.ECB;
            rj.Key = key;
            rj.IV = new byte[16];
            rj.Padding = PaddingMode.None;

            var transform = !encrypt ? rj.CreateDecryptor() : rj.CreateEncryptor();

            int dataLen = cbInp & ~0x0F;

            if (dataLen > 0) {
                for (int i = 0; i < 16; i++) {
                    transform.TransformBlock(pbInp, 0, dataLen, pbInp, 0);
                }
            }
            return pbInp;
        }

        private static byte[] Mp3_AesEcb2(byte[] key, byte[] pbInp, int cbInp, bool encrypt)
        {
            // Create our Rijndael class
            Rijndael rj = Rijndael.Create();
            rj.BlockSize = 128;
            rj.KeySize = 256;
            rj.Mode = CipherMode.ECB;
            rj.Key = key;
            rj.IV = new byte[16];
            rj.Padding = PaddingMode.None;

            var transform = !encrypt ? rj.CreateDecryptor() : rj.CreateEncryptor();

            int dataLen = pbInp.Length & ~0x0F;

            if (dataLen > 0)
            {
                for (int i = 0; i < 16; i++)
                {
                    transform.TransformBlock(pbInp, cbInp - 0x10, 0x10, pbInp, cbInp - 0x10);
                }
                for (int i = 0; i < 16; i++)
                {
                    transform.TransformBlock(pbInp, 0, dataLen, pbInp, 0);
                }
            }
            return pbInp;
        }

        // Encrypt data by a ten byte boundary
        private static byte[] Mp3_AesEcb3(byte[] key, byte[] pbInp, int cbInp, bool encrypt)
        {
            // Create our Rijndael class
            Rijndael rj = Rijndael.Create();
            rj.BlockSize = 128;
            rj.KeySize = 256;
            rj.Mode = CipherMode.ECB;
            rj.Key = key;
            rj.IV = new byte[16];
            rj.Padding = PaddingMode.None;

            var transform = !encrypt ? rj.CreateDecryptor() : rj.CreateEncryptor();

            int dataLen = pbInp.Length & ~0x0F;

            if (dataLen > 0)
            {
                for (int i = 0; i < 16; i++)
                {
                    transform.TransformBlock(pbInp, 0, dataLen, pbInp, 0);
                }
                for (int i = 0; i < 16; i++)
                {
                    transform.TransformBlock(pbInp, cbInp - 0x10, 0x10, pbInp, cbInp - 0x10);
                }
            }
            return pbInp;
        }
    }

    public class MaxPayne3Exception : Exception
    {
        internal MaxPayne3Exception(string message)
            : base("Max Payne 3:" + message)
        {
        }
    }
}