using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

namespace APA_Partition_Header_Checksumer
{
    class Program
    {
        static byte[] Header;
        static void Main(string[] args)
        {
            if (args.Length == 1)
            {
                if (args[0] != "Help")
                {
                    Read_Header(args[0]);
                }
                else
                    Help();
            }
            else
                Help();
            
        }

        static void Help()
        {
            Console.WriteLine("");
            Console.WriteLine("APA Partition Header Checksumer v1.0a");
            Console.WriteLine("usage: <app> <file>");
            Console.WriteLine("");
        }

        static void Read_Header(string F)
        {
            using (Stream fs = File.OpenRead(F))
            {
                Header = new byte[1024];
                fs.Read(Header, 0, Header.Length);
            }
            uint sum = 0;
            for (uint i = 4, old_sum = 0; i < Header.Length; i += 4)
            {
                byte[] element = new byte[4];
                Array.Copy(Header, i, element, 0, element.Length);
                uint e = BitConverter.ToUInt32(element, 0);
                sum += e;

                if (old_sum != sum)
                {
                    //byte[] bytes4 = BitConverter.GetBytes(sum);
                    //Console.WriteLine("CheckSum: {0}", getHexString(bytes4));
                    old_sum = sum;
                }

            }
            byte[] bytes4 = BitConverter.GetBytes(sum);
            // Console.WriteLine("CheckSum: {0}", getHexString(bytes4));
            Console.WriteLine("{0}", getHexString(bytes4));
            // Console.WriteLine("");
            // Console.WriteLine("Press any key to exit");
            // Console.ReadKey();
        }

        static byte[] HEX_CHAR_TABLE = {
        (byte) '0', (byte) '1', (byte) '2', (byte) '3',
        (byte) '4', (byte) '5', (byte) '6', (byte) '7',
        (byte) '8', (byte) '9', (byte) 'A', (byte) 'B',
        (byte) 'C', (byte) 'D', (byte) 'E', (byte) 'F'
    };

        public static String getHexString(byte[] raw)
        {
            byte[] hex = new byte[2 * raw.Length];
            int index = 0;

            foreach (byte b in raw)
            {
                uint v = (uint)b & 0xFF;
                hex[index++] = HEX_CHAR_TABLE[v >> 4];
                hex[index++] = HEX_CHAR_TABLE[v & 0xF];
            }
            return new String(bytesToChar(hex));
        }

        public static char[] bytesToChar(byte[] b)
        {
            char[] c = new char[b.Length];
            for (int i = 0; i < b.Length; i++)
                c[i] = (char)b[i];
            return c;
        }

    }
}
