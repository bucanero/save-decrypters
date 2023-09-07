using System;

namespace MGS_3_Snake_Eater___SecFixer
{
	// Token: 0x02000003 RID: 3
	public class Routines
	{
		// Token: 0x06000007 RID: 7 RVA: 0x00002948 File Offset: 0x00000B48
		public static byte[] Decrypt(byte[] data)
		{
			byte b = 0;
			for (int i = 0; i < 18812; i++)
			{
				byte b2 = (byte)((char)data[i] - Routines.KEY[i % Routines.KEY.Length] - (char)b);
				b = data[i];
				data[i] = b2;
			}
			return data;
		}

		// Token: 0x06000008 RID: 8 RVA: 0x00002990 File Offset: 0x00000B90
		public static byte[] Encrypt(byte[] data)
		{
			byte b = 0;
			for (int i = 0; i < 18812; i++)
			{
				b = (byte)((char)data[i] + Routines.KEY[i % Routines.KEY.Length] + (char)b);
				data[i] = b;
			}
			return data;
		}

		// Token: 0x06000009 RID: 9 RVA: 0x000029D4 File Offset: 0x00000BD4
		public static byte[] DecodeBase64(byte[] data)
		{
			byte[] array = (byte[])data.Clone();
			for (int i = 0; i < 28; i++)
			{
				if (array[1 + i] == 95)
				{
					array[1 + i] = 43;
				}
			}
			byte[] array2 = new byte[256];
			for (int j = 0; j < 256; j++)
			{
				array2[j] = byte.MaxValue;
			}
			string text = "ghijklmn01234567opqrstuvEFGHIJKL89abcdefUVWXYZ+-wxyzABCDMNOPQRST";
			for (int k = 0; k < 64; k++)
			{
				array2[(int)text[k]] = (byte)k;
			}
			int l = 0;
			byte[] array3 = new byte[32];
			int m;
			int num2;
			for (m = 0; m < 196; m += 7)
			{
				int num = (int)array[1 + l];
				num2 = (int)array2[num];
				array3[l] = (byte)num2;
				if (num2 == 255)
				{
					return null;
				}
				num = num2 - m;
				int num3 = num & 63;
				array3[l++] = (byte)num3;
			}
			byte[] array4 = new byte[21];
			m = 0;
			num2 = 0;
			for (l = 0; l < 21; l++)
			{
				int num4;
				if (m <= 5)
				{
					num4 = (int)array3[num2];
					int num5 = (num4 & 63 >> m) << 2 + m;
					array4[l] = (byte)num5;
				}
				num4 = ~m + 7;
				m = 0;
				num2++;
				if (num4 < 2)
				{
					int num3 = (int)array3[num2];
					int num = ~num4 + 3;
					num4 += 6;
					int num5 = num3 << num;
					num = (int)array4[l];
					num2++;
					num3 = (num5 | num);
					array4[l] = (byte)num3;
				}
				num4 -= 2;
				if (num4 == 0)
				{
					int num6 = (int)array3[num2++];
					m = 0;
					int num5 = (int)array4[l];
					int num = num6 | num5;
					array4[l] = (byte)num;
				}
				else if (num4 <= 5)
				{
					int num6 = (int)array3[num2];
					m = 6 - num4;
					int num5 = (int)((byte)(num6 >> num4));
					int num = (int)array4[l];
					int num3 = num5 | num;
					array4[l] = (byte)num3;
				}
			}
			byte[] array5 = new byte[20];
			num2 = 0;
			for (int n = 0; n < 20; n++)
			{
				int num4 = (int)array4[n];
				num2 = (num4 ^ num2);
				array5[n] = (byte)num4;
			}
			return array5;
		}

		// Token: 0x0600000A RID: 10 RVA: 0x00002BEC File Offset: 0x00000DEC
		public static byte[] EncodeBase64(byte[] data, byte[] reg5)
		{
			int num = 0;
			int num2 = 0;
			byte[] array = new byte[21];
			int num3;
			for (int i = 0; i < 16; i++)
			{
				num3 = (int)data[i];
				num2 = (num3 ^ num2);
				array[i] = (byte)num3;
			}
			byte[] array2 = new byte[4];
			for (int j = 0; j < 4; j++)
			{
				int num4 = (int)reg5[j];
				num2 = (num4 ^ num2);
				array2[j] = (byte)num4;
			}
			Buffer.BlockCopy(array2, 0, array, 16, 4);
			byte[] array3 = new byte[28];
			byte b = (byte)num2;
			array[20] = b;
			num3 = 0;
			num2 = 0;
			int num5 = 0;
			int num6 = 0;
			int k;
			for (k = 0; k < 28; k++)
			{
				if (num3 <= 7)
				{
					int num4;
					switch (num3)
					{
					case 0:
					{
						num4 = (int)array[num2];
						int num7 = num4 >> 2;
						array3[k] = (byte)num7;
						goto IL_1C1;
					}
					case 1:
					{
						num4 = (int)array[num2];
						int num7 = num4 >> 1;
						array3[k] = (byte)num7;
						goto IL_1C1;
					}
					case 2:
					{
						num4 = (int)array[num2];
						int num7 = num4 & 63;
						array3[k] = (byte)num7;
						goto IL_1C1;
					}
					case 3:
					{
						num4 = (int)array[num2 + 1];
						int num7 = (int)array[num2];
						num6 = num4 >> 7;
						num5 = (num7 & 31) << 1;
						break;
					}
					case 4:
					{
						num4 = (int)array[num2 + 1];
						int num7 = (int)array[num2];
						num6 = num4 >> 6;
						num5 = (num7 & 15) << 2;
						break;
					}
					case 5:
					{
						num4 = (int)array[num2 + 1];
						int num7 = (int)array[num2];
						num6 = num4 >> 5;
						num5 = (num7 & 7) << 3;
						break;
					}
					case 6:
					{
						num4 = (int)array[num2 + 1];
						int num7 = (int)array[num2];
						num6 = num4 >> 4;
						num5 = (num7 & 3) << 4;
						break;
					}
					case 7:
					{
						num4 = (int)array[num2 + 1];
						int num7 = (int)array[num2];
						num6 = num4 >> 3;
						num5 = (num7 & 1) << 5;
						break;
					}
					case 8:
						break;
					default:
						goto IL_1C1;
					}
					num4 = (num5 | num6);
					array3[k] = (byte)num4;
				}
				IL_1C1:
				num3 += 6;
				if (num3 >= 8)
				{
					num3 -= 8;
					num2++;
				}
			}
			num2 = num;
			k = num;
			byte[] array4 = new byte[32];
			array4[0] = 95;
			string text = "ghijklmn01234567opqrstuvEFGHIJKL89abcdefUVWXYZ+-wxyzABCDMNOPQRST";
			for (int l = 0; l < 7; l++)
			{
				int num8 = k + 21;
				int num7 = (int)array3[num2];
				num6 = (int)array3[num2 + 1];
				int num9 = num7 + k;
				num7 = (int)array3[num2 + 2];
				num6 += k;
				num5 = (int)array3[num2 + 3];
				num9 &= 63;
				num3 = num7 + num8;
				num7 = num6 + 7;
				num6 = num3 - 7;
				num3 = (num7 & 63);
				num7 = (int)text[num9];
				num5 += num8;
				num8 = (num6 & 63);
				num6 = (num5 & 63);
				k += 28;
				array4[num2 + 1] = (byte)num7;
				num5 = (int)text[num3];
				array4[num2 + 2] = (byte)num5;
				num3 = (int)text[num8];
				array4[num2 + 3] = (byte)num3;
				num8 = (int)text[num6];
				array4[num2 + 4] = (byte)num8;
				num2 += 4;
			}
			num2 = num;
			for (int m = 0; m < 28; m++)
			{
				num3 = (int)array4[1 + num2];
				if (num3 == 43)
				{
					array4[1 + num2] = 95;
				}
				num2++;
			}
			return array4;
		}

		// Token: 0x04000005 RID: 5
		private static string KEY = "3MetalSolidSnakeEaterGear";
	}
}
