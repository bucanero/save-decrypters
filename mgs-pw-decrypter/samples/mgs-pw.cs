using System;
using MasterLib.IO;

namespace MGS_Peace_Walker___SecFixer
{
	// Token: 0x02000003 RID: 3
	public class Routines
	{
		// Token: 0x06000006 RID: 6 RVA: 0x00002410 File Offset: 0x00000610
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

		// Token: 0x06000007 RID: 7 RVA: 0x00002450 File Offset: 0x00000650
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

		// Token: 0x06000008 RID: 8 RVA: 0x0000248C File Offset: 0x0000068C
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

		// Token: 0x06000009 RID: 9 RVA: 0x000024C4 File Offset: 0x000006C4
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

		// Token: 0x0600000A RID: 10 RVA: 0x000024F7 File Offset: 0x000006F7
		public static int GetNextSalt(int salt)
		{
			return 0;
		}

		// Token: 0x0600000B RID: 11 RVA: 0x000024FC File Offset: 0x000006FC
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

		// Token: 0x0600000C RID: 12 RVA: 0x00002558 File Offset: 0x00000758
		public static byte[] DecodeBase64(byte[] data, string chars)
		{
			for (int i = 0; i < 28; i++)
			{
				if (data[1 + i] == 95)
				{
					data[1 + i] = 43;
				}
			}
			byte[] array = new byte[256];
			for (int j = 0; j < array.Length; j++)
			{
				array[j] = byte.MaxValue;
			}
			for (int k = 0; k < 64; k++)
			{
				array[(int)chars[k]] = (byte)k;
			}
			int num = 0;
			byte[] array2 = new byte[32];
			int num2;
			for (int l = 0; l < 196; l += 7)
			{
				num2 = (int)array[(int)data[1 + num]];
				if (num2 == 255)
				{
					return null;
				}
				array2[num++] = (byte)(num2 - l & 63);
			}
			int num3 = 0;
			num2 = 0;
			byte[] array3 = new byte[21];
			for (int m = 0; m < array3.Length; m++)
			{
				if (num3 <= 5)
				{
					array3[m] = (byte)(((int)array2[num2] & 63 >> num3) << 2 + num3);
				}
				int num4 = -num3 + 6;
				num3 = 0;
				num2++;
				if (num4 < 2)
				{
					int num5 = ~num4 + 3;
					num4 += 6;
					byte[] array4 = array3;
					int num6 = m;
					array4[num6] |= (byte)(array2[num2] << num5);
					num2++;
				}
				num4 -= 2;
				if (num4 == 0)
				{
					num3 = 0;
					byte[] array5 = array3;
					int num7 = m;
					array5[num7] |= array2[num2++];
				}
				else if (num4 <= 5)
				{
					num3 = 6 - num4;
					byte[] array6 = array3;
					int num8 = m;
					array6[num8] |= (byte)(array2[num2] >> num4);
				}
			}
			return array3;
		}

		// Token: 0x0600000D RID: 13 RVA: 0x000026F4 File Offset: 0x000008F4
		public static byte[] EncodeBase64(byte[] data, string chars)
		{
			data[20] = 0;
			for (int i = 0; i < 20; i++)
			{
				int num = 20;
				data[num] ^= data[i];
			}
			byte[] array = new byte[data.Length * 4 / 3];
			int j = 0;
			int num2 = 0;
			int num3 = 0;
			while (j < array.Length)
			{
				if (num2 <= 7)
				{
					switch (num2)
					{
					case 0:
						array[j] = (byte)(data[num3] >> 2);
						break;
					case 1:
						array[j] = (byte)(data[num3] >> 1);
						break;
					case 2:
						array[j] = (data[num3] & 63);
						break;
					case 3:
						array[j] = (byte)((int)(data[num3] & 31) << 1 | data[num3 + 1] >> 7);
						break;
					case 4:
						array[j] = (byte)((int)(data[num3] & 15) << 2 | data[num3 + 1] >> 6);
						break;
					case 5:
						array[j] = (byte)((int)(data[num3] & 7) << 3 | data[num3 + 1] >> 5);
						break;
					case 6:
						array[j] = (byte)((int)(data[num3] & 3) << 4 | data[num3 + 1] >> 4);
						break;
					case 7:
						array[j] = (byte)((int)(data[num3] & 1) << 5 | data[num3 + 1] >> 3);
						break;
					}
				}
				num2 += 6;
				if (num2 >= 8)
				{
					num2 -= 8;
					num3++;
				}
				j++;
			}
			byte[] array2 = new byte[32];
			array2[0] = 95;
			int k = 0;
			int num4 = 0;
			int num5 = 0;
			while (k < 7)
			{
				int num6 = num5 + 21;
				array2[num4 + 1] = (byte)chars[(int)array[num4] + num5 & 63];
				array2[num4 + 2] = (byte)chars[(int)array[num4 + 1] + num5 + 7 & 63];
				array2[num4 + 3] = (byte)chars[(int)array[num4 + 2] + num6 - 7 & 63];
				array2[num4 + 4] = (byte)chars[(int)array[num4 + 3] + num6 & 63];
				num5 += 28;
				k++;
				num4 += 4;
			}
			for (int l = 0; l < 28; l++)
			{
				if (array2[1 + l] == 43)
				{
					array2[1 + l] = 95;
				}
			}
			return array2;
		}

		// Token: 0x0600000E RID: 14 RVA: 0x00002908 File Offset: 0x00000B08
		public static void PW_Decrypt(byte[] data)
		{
			using (MasterIO masterIO = new MasterIO(data, Endian.Big))
			{
				for (int i = 0; i < 16; i++)
				{
					masterIO.Writer.WriteUInt32(BitConverter.ToUInt32(data, 4 * i));
				}
				masterIO.Position = 4L;
				uint num = masterIO.Reader.ReadUInt32() | 2907168399U;
				masterIO.Position = 0L;
				uint num2 = num ^ masterIO.Reader.ReadUInt32();
				masterIO.Position = (long)((ulong)((ulong)(num2 + 7U) << 2));
				uint num3 = masterIO.Reader.ReadUInt32() ^ 3159224226U;
				masterIO.Position = (long)((ulong)((ulong)(num2 + 3U) << 2));
				uint num4 = masterIO.Reader.ReadUInt32() ^ 762434156U;
				masterIO.Position = (long)((ulong)((ulong)(num2 + 2U) << 2));
				num = (masterIO.Reader.ReadUInt32() ^ 321379955U);
				Routines.SetSalts(num, num4, num3);
				masterIO.Position = 64L;
				for (int j = 0; j < 54886; j++)
				{
					masterIO.Writer.WriteUInt32(BitConverter.ToUInt32(data, (int)masterIO.Position));
				}
				Routines.DeEncryptBlock(masterIO, 64, 219544);
				uint num5 = 219608U;
				masterIO.Position = (long)((ulong)num5);
				uint num6 = masterIO.Reader.ReadUInt32();
				masterIO.Position = (long)((ulong)(num5 + 4U));
				uint num7 = masterIO.Reader.ReadUInt32();
				num2 = ((num7 | 2907168399U) ^ num6);
				masterIO.Position = (long)((ulong)((num2 + 7U << 2) + num5));
				num3 = (masterIO.Reader.ReadUInt32() ^ 3159224226U);
				masterIO.Position = (long)((ulong)((num2 + 3U << 2) + num5));
				num4 = (masterIO.Reader.ReadUInt32() ^ 762434156U);
				masterIO.Position = (long)((ulong)((num2 + 2U << 2) + num5));
				num = (masterIO.Reader.ReadUInt32() ^ 321379955U);
				Routines.SetSalts(num, num4, num3);
				Routines.DeEncryptBlock(masterIO, 219672, 61648);
				masterIO.Position = 68L;
				for (int k = 0; k < 54885; k++)
				{
					masterIO.Writer.WriteUInt32(BitConverter.ToUInt32(data, (int)masterIO.Position));
				}
				masterIO.Position = 56L;
				num4 = masterIO.Reader.ReadUInt32();
				num3 = Routines.CalculateChecksum(data, 68, 110372);
				if (num3 != num4)
				{
					throw new Exception();
				}
				masterIO.Position = 60L;
				num = masterIO.Reader.ReadUInt32();
				uint num8 = Routines.CalculateChecksum(data, 110440, 7168);
				if (num8 != num)
				{
					throw new Exception();
				}
				masterIO.Position = 48L;
				num3 = masterIO.Reader.ReadUInt32();
				num8 = Routines.CalculateChecksum(data, 117608, 101992);
				if (num8 != num3)
				{
					throw new Exception();
				}
				masterIO.Position = (long)((ulong)(num5 + 52U));
				num3 = masterIO.Reader.ReadUInt32();
				num2 = Routines.CalculateChecksum(data, 219672, 61648);
				if (num2 != num3)
				{
					throw new Exception();
				}
			}
		}

		// Token: 0x0600000F RID: 15 RVA: 0x00002C24 File Offset: 0x00000E24
		public static void PW_Encrypt(byte[] data)
		{
			using (MasterIO masterIO = new MasterIO(data, Endian.Big))
			{
				masterIO.Position = 219660L;
				masterIO.Writer.WriteUInt32(Routines.CalculateChecksum(data, 219672, 61648));
				masterIO.Position = 48L;
				masterIO.Writer.WriteUInt32(Routines.CalculateChecksum(data, 117608, 101992));
				masterIO.Position = 60L;
				masterIO.Writer.WriteUInt32(Routines.CalculateChecksum(data, 110440, 7168));
				masterIO.Position = 56L;
				masterIO.Writer.WriteUInt32(Routines.CalculateChecksum(data, 68, 110372));
				masterIO.Position = 68L;
				for (int i = 0; i < 54885; i++)
				{
					masterIO.Writer.WriteUInt32(BitConverter.ToUInt32(data, (int)masterIO.Position));
				}
				masterIO.Position = 219608L;
				uint num = masterIO.Reader.ReadUInt32();
				masterIO.Position = 219612L;
				uint num2 = masterIO.Reader.ReadUInt32();
				uint num3 = (num2 | 2907168399U) ^ num;
				masterIO.Position = (long)((ulong)((num3 + 7U << 2) + 219608U));
				uint r = masterIO.Reader.ReadUInt32() ^ 3159224226U;
				masterIO.Position = (long)((ulong)((num3 + 3U << 2) + 219608U));
				uint r2 = masterIO.Reader.ReadUInt32() ^ 762434156U;
				masterIO.Position = (long)((ulong)((num3 + 2U << 2) + 219608U));
				uint r3 = masterIO.Reader.ReadUInt32() ^ 321379955U;
				Routines.SetSalts(r3, r2, r);
				Routines.DeEncryptBlock(masterIO, 219672, 61648);
				masterIO.Position = 4L;
				num2 = (masterIO.Reader.ReadUInt32() | 2907168399U);
				masterIO.Position = 0L;
				num3 = (num2 ^ masterIO.Reader.ReadUInt32());
				masterIO.Position = (long)((ulong)((ulong)(num3 + 7U) << 2));
				r = (masterIO.Reader.ReadUInt32() ^ 3159224226U);
				masterIO.Position = (long)((ulong)((ulong)(num3 + 3U) << 2));
				r2 = (masterIO.Reader.ReadUInt32() ^ 762434156U);
				masterIO.Position = (long)((ulong)((ulong)(num3 + 2U) << 2));
				r3 = (masterIO.Reader.ReadUInt32() ^ 321379955U);
				Routines.SetSalts(r3, r2, r);
				Routines.DeEncryptBlock(masterIO, 64, 219544);
				masterIO.Position = 64L;
				for (int j = 0; j < 54886; j++)
				{
					masterIO.Writer.WriteUInt32(BitConverter.ToUInt32(data, (int)masterIO.Position));
				}
				masterIO.Position = 0L;
				for (int k = 0; k < 16; k++)
				{
					masterIO.Writer.WriteUInt32(BitConverter.ToUInt32(data, 4 * k));
				}
			}
		}

		// Token: 0x06000010 RID: 16 RVA: 0x00002F1C File Offset: 0x0000111C
		public static void SetSalts(uint r3, uint r4, uint r5)
		{
			uint num = r3 ^ r4;
			Routines.SALTS[1] = num * r5;
			Routines.SALTS[0] = ((num ^ 25974U) << 16 | num);
		}

		// Token: 0x06000011 RID: 17 RVA: 0x00002F4C File Offset: 0x0000114C
		public static void SetSaltsOLD(uint r3, uint r4, uint r5)
		{
			uint num = r3 ^ r4;
			uint num2 = num << 16;
			uint num3 = num * r5;
			Routines.SALTS[1] = num3;
			uint num4 = num2 ^ 1702232064U;
			num3 = (num4 | num);
			Routines.SALTS[0] = num3;
		}

		// Token: 0x06000012 RID: 18 RVA: 0x00002F84 File Offset: 0x00001184
		public static void DeEncryptBlock(MasterIO xIO, int offset, int size)
		{
			xIO.Position = (long)offset;
			for (int i = 0; i < size >> 2; i++)
			{
				uint value = xIO.Reader.ReadUInt32() ^ Routines.SALTS[0];
				xIO.Position -= 4L;
				xIO.Writer.WriteUInt32(value);
				Routines.SALTS[0] = Routines.SALTS[0] * 48828125U + Routines.SALTS[1];
			}
		}

		// Token: 0x06000013 RID: 19 RVA: 0x00002FF4 File Offset: 0x000011F4
		public static void DeEncryptBlockOLD(MasterIO xIO, int size)
		{
			uint num = Routines.SALTS[0];
			if (size > 0)
			{
				uint num2 = Routines.SALTS[1];
				uint num3 = 48828125U;
				for (int i = 0; i < size / 4; i++)
				{
					uint num4 = xIO.Reader.ReadUInt32();
					xIO.Position -= 4L;
					uint value = num4 ^ num;
					num *= num3;
					xIO.Writer.WriteUInt32(value);
					num += num2;
				}
			}
			Routines.SALTS[0] = num;
		}

		// Token: 0x06000014 RID: 20 RVA: 0x0000306C File Offset: 0x0000126C
		public static uint CalculateChecksum(byte[] data, int offset, int size)
		{
			uint num = uint.MaxValue;
			for (int i = offset; i < offset + size; i++)
			{
				num = (Routines.TABLE[(int)((byte)((uint)data[i] ^ num))] ^ num >> 8 ^ 1354258273U);
			}
			return ~num;
		}

		// Token: 0x04000005 RID: 5
		public static uint[] TABLE = new uint[]
		{
			1354258273U,
			666855415U,
			3199612493U,
			3383822043U,
			1473614712U,
			550675438U,
			3118199380U,
			3470574274U,
			1583603539U,
			694480837U,
			2959982207U,
			3345649385U,
			1494096714U,
			772352988U,
			3070261862U,
			3221703408U,
			1292846853U,
			973633427U,
			2734761513U,
			3557168831U,
			1247970076U,
			1030075274U,
			2758599216U,
			3547058854U,
			1138020151U,
			886308769U,
			2916789787U,
			3671957133U,
			1152977710U,
			868105144U,
			2864146946U,
			3719322260U,
			1809217449U,
			483477311U,
			2245531269U,
			4074710547U,
			1824240560U,
			465339174U,
			2192954012U,
			4122141194U,
			1695416219U,
			302698253U,
			2332270263U,
			4228165153U,
			1650473858U,
			359074580U,
			2356042414U,
			4217989688U,
			1986095053U,
			23484251U,
			2557413089U,
			4016584311U,
			1896653780U,
			101421890U,
			2667758328U,
			3892703854U,
			2025517055U,
			264101737U,
			2528415443U,
			3786653253U,
			2144807910U,
			147856240U,
			2446936778U,
			3873339996U,
			644093681U,
			1365452391U,
			3362420701U,
			3211609931U,
			554291944U,
			1443816062U,
			3473388484U,
			3087041362U,
			683646659U,
			1605938773U,
			3333554159U,
			2981547897U,
			802314970U,
			1490381388U,
			3252435958U,
			3067808608U,
			1003685525U,
			1288975875U,
			3588057017U,
			2732218159U,
			1019134604U,
			1270477338U,
			3534791584U,
			2780271414U,
			889753255U,
			1108327985U,
			3674664843U,
			2885803805U,
			845499070U,
			1164081704U,
			3698011026U,
			2875988740U,
			487208505U,
			1779238575U,
			4077147925U,
			2214815619U,
			443019808U,
			1835057846U,
			4100559628U,
			2205066138U,
			332512779U,
			1691782813U,
			4259307303U,
			2329472945U,
			347896338U,
			1673218692U,
			4205976382U,
			2377460648U,
			12396125U,
			2008684235U,
			4004726641U,
			2578741223U,
			131129924U,
			1893192402U,
			3923673960U,
			2665067518U,
			241610351U,
			2036440825U,
			3764965187U,
			2540699605U,
			151743094U,
			2114738912U,
			3875867482U,
			2416065484U,
			3170948161U,
			3389514967U,
			1393472877U,
			604603899U,
			3127709784U,
			3446284494U,
			1415803252U,
			593773026U,
			3017497715U,
			3302780133U,
			1574255967U,
			718409161U,
			3031865450U,
			3283200252U,
			1522071878U,
			767543760U,
			2696397861U,
			3618698419U,
			1320789257U,
			968791455U,
			2816081980U,
			3504156842U,
			1238655248U,
			1054036358U,
			2926332951U,
			3647700097U,
			1080175931U,
			929373613U,
			2835449870U,
			3724982424U,
			1192225058U,
			805886388U,
			2255418505U,
			4050240543U,
			1751160229U,
			526886195U,
			2164469904U,
			4127457286U,
			1863143868U,
			403333418U,
			2293595323U,
			4289874989U,
			1723538839U,
			297544961U,
			2413344930U,
			4175398964U,
			1641470350U,
			382855448U,
			2614748397U,
			3974026363U,
			1977058753U,
			47232343U,
			2629050612U,
			3954380898U,
			1924809176U,
			96301390U,
			2499964127U,
			3792002121U,
			2064387571U,
			202063205U,
			2456791238U,
			3848837200U,
			2086783466U,
			191297916U,
			3420231121U,
			3168511303U,
			634582269U,
			1389741163U,
			3434172872U,
			3149291870U,
			582955236U,
			1438122098U,
			3305577955U,
			2986356085U,
			722042063U,
			1544440921U,
			3261782522U,
			3043879276U,
			744798422U,
			1533249600U,
			3597370805U,
			2708256035U,
			946201753U,
			1331876879U,
			3506848172U,
			2785112378U,
			1057497216U,
			1208946710U,
			3635416455U,
			2948021521U,
			918449323U,
			1102666813U,
			3755854238U,
			2832922888U,
			835954866U,
			1188337700U,
			4038243609U,
			2276820367U,
			515691573U,
			1773921443U,
			4158615808U,
			2161656214U,
			433131564U,
			1859526842U,
			4268309803U,
			2305691069U,
			275209223U,
			1734372497U,
			4177852722U,
			2382612900U,
			386570270U,
			1611507848U,
			3976570237U,
			2583860715U,
			51102801U,
			1947006151U,
			3932709220U,
			2641318386U,
			73793608U,
			1935749342U,
			3822988623U,
			2497256921U,
			231754851U,
			2060942581U,
			3836995926U,
			2478102976U,
			180193402U,
			2109389036U
		};

		// Token: 0x04000006 RID: 6
		public static uint[] SALTS = new uint[2];

		// Token: 0x04000007 RID: 7
		public static string MGS2_KEY = "2MetalOfSolidSonsLibertyGear";

		// Token: 0x04000008 RID: 8
		public static string MGS3_KEY = "3MetalSolidSnakeEaterGear";

		// Token: 0x04000009 RID: 9
		public static string MGS2_ALPHABET = "ghijklmn01234567opqrstuvEFGHIJKL89abcdefUVWXYZ_.wxyzABCDMNOPQRST";

		// Token: 0x0400000A RID: 10
		public static string MGS3_ALPHABET = "ghijklmn01234567opqrstuvEFGHIJKL89abcdefUVWXYZ+-wxyzABCDMNOPQRST";
	}
}
