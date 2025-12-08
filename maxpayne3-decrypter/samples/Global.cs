using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MaxPayne3
{
    internal static class Global
    {
        public static byte[] ReadBytes(this byte[] buffer, int offset, int length)
        {
            byte[] data = new byte[length];
            Buffer.BlockCopy(buffer, offset, data, 0, length);
            return data;
        }

        internal static byte[] convertToBigEndian(byte[] input)
        {
            if (BitConverter.IsLittleEndian)
                Array.Reverse(input);

            return input;
        }

        public static int ReadInt32(this byte[] buffer, int offset)
        {
            return (buffer[offset] << 24) |
                   (buffer[offset + 1] << 16) |
                   (buffer[offset + 2] << 8) |
                   (buffer[offset + 3]);
        }

        public static void WriteInt32(this byte[] buffer, int offset, int value)
        {
            buffer[offset] = (byte)((value >> 24) & 0xFF);
            buffer[offset + 1] = (byte)((value >> 16) & 0xFF);
            buffer[offset + 2] = (byte)((value >> 8) & 0xFF);
            buffer[offset + 3] = (byte)(value & 0xFF);
        }

        internal static bool ArrayEquals<T>(T[] arr1, T[] arr2)
        {
            if (arr1.Length != arr2.Length)
                return false;
            for (int x = 0; x < arr1.Length; x++)
                if (!arr1[x].Equals(arr2[x]))
                    return false;
            return true;
        }
    }
}
