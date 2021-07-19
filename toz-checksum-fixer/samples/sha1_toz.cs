using System;
using System.Security.Cryptography;
using System.Text;

namespace EditorTester.Classes
{
	// Token: 0x02000010 RID: 16
	public class SHA1_TOZ
	{
		// Token: 0x060000C6 RID: 198 RVA: 0x000200BC File Offset: 0x0001E2BC
		public static byte[] Compute(byte[] data)
		{
			string s = "TO12";
			string[] array = new string[]
			{
				"SRA",
				"ROS",
				"MIC",
				"LAI",
				"EDN",
				"DEZ",
				"ZAB",
				"ALI"
			};
			byte[] array2 = SHA1_TOZ.Compute(data, Encoding.ASCII.GetBytes(s));
			int num = 0;
			for (int i = 0; i < 100; i++)
			{
				array2 = SHA1_TOZ.Compute(array2, Encoding.ASCII.GetBytes(array[num++]));
				if (num > 7)
				{
					num = 0;
				}
			}
			return array2;
		}

		// Token: 0x060000C7 RID: 199 RVA: 0x00020168 File Offset: 0x0001E368
		private static byte[] Compute(byte[] data, byte[] key)
		{
			SHA1CryptoServiceProvider sha1CryptoServiceProvider = new SHA1CryptoServiceProvider();
			sha1CryptoServiceProvider.TransformBlock(data, 0, data.Length, data, 0);
			sha1CryptoServiceProvider.TransformFinalBlock(key, 0, key.Length);
			return sha1CryptoServiceProvider.Hash;
		}
	}
}
