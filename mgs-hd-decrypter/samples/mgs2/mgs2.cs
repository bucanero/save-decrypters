using System;

namespace MGS_2_Sons_of_Liberty___SecFixer
{
	// Token: 0x02000003 RID: 3
	public class Routines
	{
		// Token: 0x06000007 RID: 7 RVA: 0x00002A58 File Offset: 0x00000C58
		public static byte[] Decrypt(string key, byte[] data, int offset, int size)
		{
			byte b = 0;
			for (int i = offset; i < offset + size; i++)
			{
				byte b2 = (byte)((char)data[i] - key[(i - offset) % key.Length] - (char)b);
				b = data[i];
				data[i] = b2;
			}
			return data;
		}

		// Token: 0x06000008 RID: 8 RVA: 0x00002A98 File Offset: 0x00000C98
		public static byte[] Encrypt(string key, byte[] data, int offset, int size)
		{
			byte b = 0;
			for (int i = offset; i < offset + size; i++)
			{
				b = (byte)((char)data[i] + key[(i - offset) % key.Length] + (char)b);
				data[i] = b;
			}
			return data;
		}

		// Token: 0x06000009 RID: 9 RVA: 0x00002AD4 File Offset: 0x00000CD4
		public static byte[] DecryptLayer2(byte[] data, int offset, int size, int salt)
		{
			byte b = 0;
			for (int i = offset; i < offset + size; i++)
			{
				salt = Routines.GetNextSalt(salt);
				byte b2 = (byte)((int)data[i] - salt - (int)b);
				b = data[i];
				data[i] = b2;
			}
			return data;
		}

		// Token: 0x0600000A RID: 10 RVA: 0x00002B0C File Offset: 0x00000D0C
		public static byte[] EncryptLayer2(byte[] data, int offset, int size, int salt)
		{
			byte b = 0;
			for (int i = offset; i < offset + size; i++)
			{
				salt = Routines.GetNextSalt(salt);
				b = (byte)((int)data[i] + salt + (int)b);
				data[i] = b;
			}
			return data;
		}

		// Token: 0x0600000B RID: 11 RVA: 0x0000247D File Offset: 0x0000067D
		public static int GetNextSalt(int salt)
		{
			return 0;
		}

		// Token: 0x0600000C RID: 12 RVA: 0x00002B40 File Offset: 0x00000D40
		public static int CalculateCRC32(byte[] data, int offset, int size)
		{
			int num = -1;
			if (size > 0)
			{
				int num2 = -306674912;
				for (int i = offset; i < offset + size; i++)
				{
					num ^= data[i] << 24 >> 24;
					for (int j = 0; j < 8; j++)
					{
						int num3 = num & 1;
						num = (num >> 1 & int.MaxValue);
						if (num3 != 0)
						{
							num ^= num2;
						}
					}
				}
			}
			return ~num;
		}

		// Token: 0x0600000D RID: 13 RVA: 0x00002B9C File Offset: 0x00000D9C
		public static byte[] DecodeBase64(byte[] data, string chars)
		{
			byte[] array = new byte[128];
			for (int i = 0; i < array.Length; i++)
			{
				array[i] = byte.MaxValue;
			}
			for (int j = 0; j < 64; j++)
			{
				array[(int)chars[j]] = (byte)j;
			}
			int num = 0;
			byte[] array2 = new byte[32];
			int num2;
			for (int k = 0; k < 196; k += 7)
			{
				num2 = (int)array[(int)data[1 + num]];
				if (num2 == 255)
				{
					return null;
				}
				array2[num++] = (byte)(num2 - k & 63);
			}
			int num3 = 0;
			num2 = 0;
			byte[] array3 = new byte[21];
			for (int l = 0; l < array3.Length; l++)
			{
				if (num3 <= 5)
				{
					array3[l] = (byte)(((int)array2[num2] & 63 >> num3) << 2 + num3);
				}
				int num4 = -num3 + 6;
				num3 = 0;
				num2++;
				if (num4 < 2)
				{
					int num5 = ~num4 + 3;
					num4 += 6;
					byte[] array4 = array3;
					int num6 = l;
					array4[num6] |= (byte)(array2[num2] << num5);
					num2++;
				}
				num4 -= 2;
				if (num4 == 0)
				{
					num3 = 0;
					byte[] array5 = array3;
					int num7 = l;
					array5[num7] |= array2[num2++];
				}
				else if (num4 <= 5)
				{
					num3 = 6 - num4;
					byte[] array6 = array3;
					int num8 = l;
					array6[num8] |= (byte)(array2[num2] >> num4);
				}
			}
			return array3;
		}

		// Token: 0x0600000E RID: 14 RVA: 0x00002D14 File Offset: 0x00000F14
		public static byte[] EncodeBase64(byte[] data, string chars)
		{
			byte b = 0;
			for (int i = 0; i < 20; i++)
			{
				b ^= data[i];
			}
			data[20] = b;
			byte[] array = new byte[28];
			int num = 0;
			int num2 = 0;
			int num3 = 0;
			int num4 = 0;
			int j;
			for (j = 0; j < 28; j++)
			{
				if (num <= 7)
				{
					int num5;
					switch (num)
					{
					case 0:
					{
						num5 = (int)data[num2];
						int num6 = num5 >> 2;
						array[j] = (byte)num6;
						goto IL_160;
					}
					case 1:
					{
						num5 = (int)data[num2];
						int num6 = num5 >> 1;
						array[j] = (byte)num6;
						goto IL_160;
					}
					case 2:
					{
						num5 = (int)data[num2];
						int num6 = num5 & 63;
						array[j] = (byte)num6;
						goto IL_160;
					}
					case 3:
					{
						num5 = (int)data[num2 + 1];
						int num6 = (int)data[num2];
						num4 = num5 >> 7;
						num3 = (num6 & 31) << 1;
						break;
					}
					case 4:
					{
						num5 = (int)data[num2 + 1];
						int num6 = (int)data[num2];
						num4 = num5 >> 6;
						num3 = (num6 & 15) << 2;
						break;
					}
					case 5:
					{
						num5 = (int)data[num2 + 1];
						int num6 = (int)data[num2];
						num4 = num5 >> 5;
						num3 = (num6 & 7) << 3;
						break;
					}
					case 6:
					{
						num5 = (int)data[num2 + 1];
						int num6 = (int)data[num2];
						num4 = num5 >> 4;
						num3 = (num6 & 3) << 4;
						break;
					}
					case 7:
					{
						num5 = (int)data[num2 + 1];
						int num6 = (int)data[num2];
						num4 = num5 >> 3;
						num3 = (num6 & 1) << 5;
						break;
					}
					case 8:
						break;
					default:
						goto IL_160;
					}
					num5 = (num3 | num4);
					array[j] = (byte)num5;
				}
				IL_160:
				num += 6;
				if (num >= 8)
				{
					num -= 8;
					num2++;
				}
			}
			num2 = 0;
			j = 0;
			byte[] array2 = new byte[32];
			array2[0] = 68;
			for (int k = 0; k < 7; k++)
			{
				int num7 = j + 21;
				int num6 = (int)array[num2];
				num4 = (int)array[num2 + 1];
				int num8 = num6 + j;
				num6 = (int)array[num2 + 2];
				num4 += j;
				num3 = (int)array[num2 + 3];
				num8 &= 63;
				num = num6 + num7;
				num6 = num4 + 7;
				num4 = num - 7;
				num = (num6 & 63);
				num6 = (int)chars[num8];
				num3 += num7;
				num7 = (num4 & 63);
				num4 = (num3 & 63);
				j += 28;
				array2[num2 + 1] = (byte)num6;
				num3 = (int)chars[num];
				array2[num2 + 2] = (byte)num3;
				num = (int)chars[num7];
				array2[num2 + 3] = (byte)num;
				num7 = (int)chars[num4];
				array2[num2 + 4] = (byte)num7;
				num2 += 4;
			}
			return array2;
		}

		// Token: 0x04000005 RID: 5
		public static string MGS2_KEY = "2MetalOfSolidSonsLibertyGear";

		// Token: 0x04000006 RID: 6
		public static string MGS3_KEY = "3MetalSolidSnakeEaterGear";

		// Token: 0x04000007 RID: 7
		public static string MGS2_ALPHABET = "ghijklmn01234567opqrstuvEFGHIJKL89abcdefUVWXYZ_.wxyzABCDMNOPQRST";

		// Token: 0x04000008 RID: 8
		public static string MGS3_ALPHABET = "ghijklmn01234567opqrstuvEFGHIJKL89abcdefUVWXYZ+-wxyzABCDMNOPQRST";
	}
}
