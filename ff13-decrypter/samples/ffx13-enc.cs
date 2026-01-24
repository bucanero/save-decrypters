using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.IO;
using System.Net;
using System.Reflection;
using System.Resources;
using System.Runtime.InteropServices;
using System.Security.Cryptography;
using System.Text;
using System.Threading;
using System.Windows.Forms;
using DevComponents.DotNetBar;
using DevComponents.DotNetBar.Controls;
using DevComponents.DotNetBar.Metro;
using DevComponents.DotNetBar.Metro.ColorTables;
using FFX13_3;
using ns0;
using ns12;
using ns17;
using ns18;
using ns2;
using ns4;
using ns5;
using ns6;
using ns7;
using ns8;
using SmartAssembly.MemoryManagement;

namespace ns10
{
	// Token: 0x0200060B RID: 1547
	internal sealed class Class362
	{
		// Token: 0x06006769 RID: 26473 RVA: 0x00220B28 File Offset: 0x0021ED28
		static string smethod_0(Class5 class5_0)
		{
			if (class5_0.method_2() == null)
			{
				return "";
			}
			foreach (Class5.Struct9 @struct in class5_0.method_2())
			{
				if (@struct.string_0 == "SAVEDATA_DIRECTORY")
				{
					return @struct.string_1;
				}
			}
			return "";
		}

		// Token: 0x0600676A RID: 26474 RVA: 0x0004BCC3 File Offset: 0x00049EC3
		static void smethod_1(FF_Encryption ff_Encryption_0)
		{
			ff_Encryption_0.ffgame_0 = FF_Encryption.FFGame.None;
			FF_Encryption.Struct0.byte_0 = null;
			Class362.smethod_24(ff_Encryption_0);
		}

		// Token: 0x0600676B RID: 26475 RVA: 0x00220B8C File Offset: 0x0021ED8C
		static void smethod_2(Class371.Class374 class374_0, int int_0, int int_1)
		{
			if ((class374_0.int_1 += int_0) > 32768)
			{
				throw new InvalidOperationException();
			}
			int num = class374_0.int_0 - int_1 & 32767;
			int num2 = 32768 - int_0;
			if (num > num2 || class374_0.int_0 >= num2)
			{
				Class362.smethod_30(class374_0, num, int_0, int_1);
				return;
			}
			if (int_0 <= int_1)
			{
				Array.Copy(class374_0.byte_0, num, class374_0.byte_0, class374_0.int_0, int_0);
				class374_0.int_0 += int_0;
				return;
			}
			while (int_0-- > 0)
			{
				class374_0.byte_0[class374_0.int_0++] = class374_0.byte_0[num++];
			}
		}

		// Token: 0x0600676C RID: 26476 RVA: 0x00220C40 File Offset: 0x0021EE40
		unsafe static void smethod_3(FF_Encryption ff_Encryption_0, ulong ulong_0)
		{
			void* ptr = stackalloc byte[108];
			*(long*)ptr = -4294967296L;
			*(long*)((byte*)ptr + 8) = 4294967295L;
			*(long*)((byte*)ptr + 16) = 0L;
			*(long*)((byte*)ptr + 24) = 0L;
			*(long*)((byte*)ptr + 32) = 0L;
			*(long*)((byte*)ptr + 40) = 0L;
			*(long*)((byte*)ptr + 48) = 0L;
			*(long*)((byte*)ptr + 56) = 0L;
			*(long*)((byte*)ptr + 64) = 0L;
			Class362.smethod_77(ulong_0 >> 32, ref *(ulong*)((byte*)ptr + 56), ff_Encryption_0);
			Class362.smethod_77(ulong_0 & 4294967295UL, ref *(ulong*)((byte*)ptr + 64), ff_Encryption_0);
			*(long*)((byte*)ptr + 40) = ((*(long*)((byte*)ptr + 56) & 4294967295L & ~(*(long*)ptr)) | (long)(Class362.smethod_32(32, ff_Encryption_0, (ulong)(*(long*)((byte*)ptr + 64))) & (ulong)(*(long*)ptr)));
			*(long*)((byte*)ptr + 56) = (long)((Class362.smethod_32(24, ff_Encryption_0, (ulong)(*(long*)((byte*)ptr + 40) << 8 & *(long*)((byte*)ptr + 8))) & (ulong)(*(long*)ptr)) | (ulong)(*(long*)((byte*)ptr + 40) & 4278190080L));
			*(long*)((byte*)ptr + 64) = (long)(Class362.smethod_32(16, ff_Encryption_0, (ulong)(*(long*)((byte*)ptr + 40))) & 65535UL);
			*(long*)((byte*)ptr + 16) = (long)((Class362.smethod_32(8, ff_Encryption_0, (ulong)(*(long*)((byte*)ptr + 56))) & 18446744073709551360UL) | (ulong)(*(long*)((byte*)ptr + 64)) | ((Class362.smethod_32(32, ff_Encryption_0, (ulong)(*(long*)((byte*)ptr + 40))) & (ulong)(*(long*)((byte*)ptr + 8))) << 16 & (ulong)(*(long*)((byte*)ptr + 8))));
			*(long*)((byte*)ptr + 56) = (long)((Class362.smethod_60((ulong)(*(long*)((byte*)ptr + 16)) >> 32, ff_Encryption_0) >> 32 & (ulong)(~(ulong)(*(long*)ptr))) | (Class362.smethod_32(32, ff_Encryption_0, Class362.smethod_60((ulong)(*(long*)((byte*)ptr + 16) & *(long*)((byte*)ptr + 8)), ff_Encryption_0) >> 32) & (ulong)(*(long*)ptr)));
			byte[] bytes = BitConverter.GetBytes((ulong)(*(long*)((byte*)ptr + 56)));
			Array.Reverse(bytes);
			byte[] array = bytes;
			int num = 0;
			array[num] += 69;
			ref byte byte_ = ref bytes[1];
			*(long*)((byte*)ptr + 80) = (long)((ulong)bytes[2]);
			*(long*)((byte*)ptr + 88) = (long)Class362.smethod_32(2, ff_Encryption_0, (ulong)bytes[0]);
			*(long*)((byte*)ptr + 96) = (long)(bytes[0] + bytes[1]);
			Class362.smethod_82((ulong)(*(long*)((byte*)ptr + 80)), (ulong)(*(long*)((byte*)ptr + 96)), ref *(ulong*)((byte*)ptr + 64), (ulong)(*(long*)((byte*)ptr + 16)), ref *(ulong*)((byte*)ptr + 16), ref byte_, ff_Encryption_0, (ulong)(*(long*)((byte*)ptr + 88)));
			byte_ = ref bytes[2];
			*(long*)((byte*)ptr + 80) = (long)((ulong)bytes[3]);
			Class362.smethod_82((ulong)(*(long*)((byte*)ptr + 80)), (ulong)(*(long*)((byte*)ptr + 64)), ref *(ulong*)((byte*)ptr + 64), (ulong)(*(long*)((byte*)ptr + 16)), ref *(ulong*)((byte*)ptr + 16), ref byte_, ff_Encryption_0, (ulong)(*(long*)((byte*)ptr + 16)));
			byte_ = ref bytes[3];
			*(long*)((byte*)ptr + 80) = (long)((ulong)bytes[4]);
			Class362.smethod_82((ulong)(*(long*)((byte*)ptr + 80)), (ulong)(*(long*)((byte*)ptr + 64)), ref *(ulong*)((byte*)ptr + 56), (ulong)(*(long*)((byte*)ptr + 16)), ref *(ulong*)((byte*)ptr + 48), ref byte_, ff_Encryption_0, (ulong)(*(long*)((byte*)ptr + 16)));
			byte_ = ref bytes[4];
			*(long*)((byte*)ptr + 80) = (long)((ulong)bytes[5]);
			Class362.smethod_82((ulong)(*(long*)((byte*)ptr + 80)), (ulong)(*(long*)((byte*)ptr + 56)), ref *(ulong*)((byte*)ptr + 24), (ulong)(*(long*)((byte*)ptr + 16)), ref *(ulong*)((byte*)ptr + 40), ref byte_, ff_Encryption_0, (ulong)(*(long*)((byte*)ptr + 48)));
			byte_ = ref bytes[5];
			*(long*)((byte*)ptr + 80) = (long)((ulong)bytes[6]);
			Class362.smethod_82((ulong)(*(long*)((byte*)ptr + 80)), (ulong)(*(long*)((byte*)ptr + 24)), ref *(ulong*)((byte*)ptr + 32), (ulong)(*(long*)((byte*)ptr + 16)), ref *(ulong*)((byte*)ptr + 64), ref byte_, ff_Encryption_0, (ulong)(*(long*)((byte*)ptr + 40)));
			byte_ = ref bytes[6];
			*(long*)((byte*)ptr + 80) = (long)((ulong)bytes[7]);
			Class362.smethod_82((ulong)(*(long*)((byte*)ptr + 80)), (ulong)(*(long*)((byte*)ptr + 32)), ref *(ulong*)((byte*)ptr + 56), (ulong)(*(long*)((byte*)ptr + 32)), ref *(ulong*)((byte*)ptr + 32), ref byte_, ff_Encryption_0, (ulong)(*(long*)((byte*)ptr + 64)));
			bytes[7] = (byte)(((*(long*)((byte*)ptr + 56) + 212L ^ *(long*)((byte*)ptr + 32)) & 255L) ^ 69L);
			Array.Copy(bytes, 0, FF_Encryption.Struct0.byte_0, 0, bytes.Length);
			*(int*)((byte*)ptr + 104) = 0;
			while (*(int*)((byte*)ptr + 104) < 31)
			{
				byte[] array2 = new byte[8];
				Array.Copy(FF_Encryption.Struct0.byte_0, *(int*)((byte*)ptr + 104) << 3, array2, 0, 8);
				*(long*)((byte*)ptr + 56) = (long)Class362.smethod_60(Class362.smethod_28(ff_Encryption_0, ref array2), ff_Encryption_0);
				*(long*)((byte*)ptr + 56) = *(long*)((byte*)ptr + 56) + (long)(Class362.smethod_32(2, ff_Encryption_0, (ulong)(*(long*)((byte*)ptr + 56))) & 18446744073709551612UL);
				*(long*)((byte*)ptr + 72) = (long)((Class362.smethod_60((ulong)(*(long*)((byte*)ptr + 56)) >> 32, ff_Encryption_0) >> 32 & (ulong)(~(ulong)(*(long*)ptr))) | (Class362.smethod_32(32, ff_Encryption_0, Class362.smethod_60((ulong)(*(long*)((byte*)ptr + 56) & *(long*)((byte*)ptr + 8)), ff_Encryption_0) >> 32) & (ulong)(*(long*)ptr)));
				array2 = BitConverter.GetBytes((ulong)(*(long*)((byte*)ptr + 72)));
				Array.Reverse(array2);
				Array.Copy(array2, 0, FF_Encryption.Struct0.byte_0, (*(int*)((byte*)ptr + 104) << 3) + 8, array2.Length);
				*(int*)((byte*)ptr + 104) = *(int*)((byte*)ptr + 104) + 1;
			}
			Class362.smethod_24(ff_Encryption_0);
		}

		// Token: 0x0600676D RID: 26477 RVA: 0x0004BCD8 File Offset: 0x00049ED8
		static bool smethod_4(string string_0, Class4 class4_0, bool bool_0)
		{
			return Class362.smethod_27(string_0, bool_0, class4_0) && Class362.smethod_63(class4_0, bool_0) && class4_0.method_0(bool_0) && Class362.smethod_70(bool_0, class4_0) && Class362.smethod_8(bool_0, class4_0);
		}

		// Token: 0x0600676E RID: 26478 RVA: 0x0022106C File Offset: 0x0021F26C
		static void smethod_5()
		{
			try
			{
				Class362.smethod_41();
			}
			catch (Exception)
			{
			}
		}

		// Token: 0x0600676F RID: 26479 RVA: 0x00221094 File Offset: 0x0021F294
		static byte[] smethod_6(Class4 class4_0)
		{
			byte[] array = Class4.Struct6.smethod_0();
			return Class362.smethod_15(array.Length, array, 0, Class4.byte_5);
		}

		// Token: 0x06006770 RID: 26480 RVA: 0x002210B8 File Offset: 0x0021F2B8
		unsafe static bool smethod_7(Class371.Class372 class372_0)
		{
			void* ptr = stackalloc byte[20];
			*(int*)((byte*)ptr + 12) = class372_0.int_4;
			switch (*(int*)((byte*)ptr + 12))
			{
			case 2:
				if (class372_0.bool_0)
				{
					class372_0.int_4 = 12;
					return false;
				}
				*(int*)ptr = Class362.smethod_98(class372_0.class373_0, 3);
				if (*(int*)ptr < 0)
				{
					return false;
				}
				Class362.smethod_13(class372_0.class373_0, 3);
				if ((*(int*)ptr & 1) != 0)
				{
					class372_0.bool_0 = true;
				}
				*(int*)((byte*)ptr + 16) = *(int*)ptr >> 1;
				switch (*(int*)((byte*)ptr + 16))
				{
				case 0:
					Class362.smethod_49(class372_0.class373_0);
					class372_0.int_4 = 3;
					break;
				case 1:
					class372_0.class375_0 = Class371.Class375.class375_0;
					class372_0.class375_1 = Class371.Class375.class375_1;
					class372_0.int_4 = 7;
					break;
				case 2:
					class372_0.class376_0 = new Class371.Class376();
					class372_0.int_4 = 6;
					break;
				}
				return true;
			case 3:
				if ((class372_0.int_8 = Class362.smethod_98(class372_0.class373_0, 16)) < 0)
				{
					return false;
				}
				Class362.smethod_13(class372_0.class373_0, 16);
				class372_0.int_4 = 4;
				break;
			case 4:
				break;
			case 5:
				goto IL_153;
			case 6:
				if (!Class362.smethod_21(class372_0.class376_0, class372_0.class373_0))
				{
					return false;
				}
				class372_0.class375_0 = Class362.smethod_53(class372_0.class376_0);
				class372_0.class375_1 = Class362.smethod_46(class372_0.class376_0);
				class372_0.int_4 = 7;
				goto IL_1DD;
			case 7:
			case 8:
			case 9:
			case 10:
				goto IL_1DD;
			case 11:
				return false;
			case 12:
				return false;
			default:
				return false;
			}
			*(int*)((byte*)ptr + 4) = Class362.smethod_98(class372_0.class373_0, 16);
			if (*(int*)((byte*)ptr + 4) < 0)
			{
				return false;
			}
			Class362.smethod_13(class372_0.class373_0, 16);
			class372_0.int_4 = 5;
			IL_153:
			*(int*)((byte*)ptr + 8) = Class362.smethod_35(class372_0.class374_0, class372_0.class373_0, class372_0.int_8);
			class372_0.int_8 -= *(int*)((byte*)ptr + 8);
			if (class372_0.int_8 == 0)
			{
				class372_0.int_4 = 2;
				return true;
			}
			return !Class362.smethod_48(class372_0.class373_0);
			IL_1DD:
			return Class362.smethod_22(class372_0);
		}

		// Token: 0x06006771 RID: 26481 RVA: 0x002212AC File Offset: 0x0021F4AC
		static bool smethod_8(bool bool_0, Class4 class4_0)
		{
			byte[] byte_ = Class362.smethod_6(class4_0);
			if (!Class362.smethod_111(byte_, Class4.Struct2.byte_0))
			{
				if (!bool_0)
				{
					return false;
				}
				Class4.Struct2.byte_0 = byte_;
			}
			return true;
		}

		// Token: 0x06006772 RID: 26482 RVA: 0x0004BD08 File Offset: 0x00049F08
		static byte[] smethod_9(Class4 class4_0)
		{
			return Class4.byte_6;
		}

		// Token: 0x06006773 RID: 26483 RVA: 0x002212DC File Offset: 0x0021F4DC
		unsafe static ulong smethod_10(Class4 class4_0, string string_0)
		{
			void* ptr = stackalloc byte[16];
			*(int*)((byte*)ptr + 8) = string_0.Length;
			*(long*)ptr = 0L;
			*(int*)((byte*)ptr + 12) = 0;
			while (*(int*)((byte*)ptr + 12) < *(int*)((byte*)ptr + 8))
			{
				*(long*)ptr = (*(long*)ptr << 5) - *(long*)ptr + (long)((ulong)((byte)string_0[*(int*)((byte*)ptr + 12)]));
				*(int*)((byte*)ptr + 12) = *(int*)((byte*)ptr + 12) + 1;
			}
			return (ulong)(*(long*)ptr % (long)Class4.Struct3.ulong_0);
		}

		// Token: 0x06006774 RID: 26484 RVA: 0x00221344 File Offset: 0x0021F544
		static byte[] smethod_11(Class4 class4_0, Class4.Struct5 struct5_0)
		{
			byte[] byte_ = Class362.smethod_74(class4_0, struct5_0, 0);
			byte[] byte_2 = struct5_0.byte_1;
			int int_ = struct5_0.byte_1.Length;
			return Class362.smethod_25(int_, byte_2, byte_);
		}

		// Token: 0x06006775 RID: 26485 RVA: 0x0004BD0F File Offset: 0x00049F0F
		static int smethod_12(Class371.Stream0 stream0_0)
		{
			return stream0_0.ReadByte() | stream0_0.ReadByte() << 8;
		}

		// Token: 0x06006776 RID: 26486 RVA: 0x0004BD20 File Offset: 0x00049F20
		static void smethod_13(Class371.Class373 class373_0, int int_0)
		{
			class373_0.uint_0 >>= int_0;
			class373_0.int_2 -= int_0;
		}

		// Token: 0x06006777 RID: 26487 RVA: 0x0004BD41 File Offset: 0x00049F41
		static Class4.Struct5[] smethod_14(Class4 class4_0)
		{
			return Class4.Struct4.list_0.ToArray();
		}

		// Token: 0x06006778 RID: 26488 RVA: 0x0004BD4D File Offset: 0x00049F4D
		static byte[] smethod_15(int int_0, byte[] byte_0, int int_1, byte[] byte_1)
		{
			return new HMACSHA1(byte_1).ComputeHash(byte_0, int_1, int_0);
		}

		// Token: 0x06006779 RID: 26489 RVA: 0x00221374 File Offset: 0x0021F574
		static bool smethod_16(Class7 class7_0)
		{
			Class4 class4_ = class7_0.method_2();
			string string_ = class7_0.method_0();
			return Class362.smethod_80(false, string_, class4_);
		}

		// Token: 0x0600677A RID: 26490 RVA: 0x00221398 File Offset: 0x0021F598
		static Class6 smethod_17(string string_0, Class7 class7_0)
		{
			if (class7_0.method_6() != null && class7_0.method_6().Length != 0)
			{
				foreach (Class6 @class in class7_0.method_6())
				{
					if (@class.method_2().string_0.ToLower() == string_0.ToLower())
					{
						return @class;
					}
				}
				return null;
			}
			return null;
		}

		// Token: 0x0600677B RID: 26491 RVA: 0x002213F8 File Offset: 0x0021F5F8
		static void smethod_18()
		{
			try
			{
				if (Environment.OSVersion.Platform == PlatformID.Win32NT)
				{
					MemoryManager.memoryManager_0 = new MemoryManager();
				}
			}
			catch
			{
			}
		}

		// Token: 0x0600677C RID: 26492 RVA: 0x0004BD5D File Offset: 0x00049F5D
		static int smethod_19(Class371.Class374 class374_0)
		{
			return class374_0.int_1;
		}

		// Token: 0x0600677D RID: 26493 RVA: 0x00221434 File Offset: 0x0021F634
		unsafe static void smethod_20(ref byte[] byte_0, byte[] byte_1, FF_Encryption ff_Encryption_0, int int_0)
		{
			void* ptr = stackalloc byte[100];
			*(long*)ptr = -1024L;
			*(long*)((byte*)ptr + 8) = -4294967296L;
			*(long*)((byte*)ptr + 16) = 4294967295L;
			*(long*)((byte*)ptr + 24) = 0L;
			*(long*)((byte*)ptr + 32) = 0L;
			*(long*)((byte*)ptr + 40) = 0L;
			*(long*)((byte*)ptr + 48) = 0L;
			*(long*)((byte*)ptr + 56) = 0L;
			*(long*)((byte*)ptr + 64) = 0L;
			*(long*)((byte*)ptr + 72) = 0L;
			FF_Encryption.Struct0.int_0 += 8;
			*(int*)((byte*)ptr + 88) = 0;
			while (*(int*)((byte*)ptr + 88) < int_0 >> 3)
			{
				*(long*)((byte*)ptr + 24) = (long)((Class362.smethod_100(ff_Encryption_0, (ulong)((long)FF_Encryption.Struct0.int_0), 29) & 255UL) ^ 69UL);
				*(long*)((byte*)ptr + 72) = 0L;
				while (*(long*)((byte*)ptr + 72) < 8L)
				{
					*(long*)((byte*)ptr + 40) = (*(long*)((byte*)ptr + 24) & 255L);
					*(long*)((byte*)ptr + 24) = (long)((ulong)byte_1[(int)(*(long*)((byte*)ptr + 72))]);
					Class362.smethod_68(FF_Encryption.Struct0.byte_0[Class362.smethod_47(FF_Encryption.Struct0.int_0, ff_Encryption_0)], ff_Encryption_0, ref *(ulong*)((byte*)ptr + 48), (ulong)(*(long*)((byte*)ptr + 24) ^ *(long*)((byte*)ptr + 40)));
					Class362.smethod_68(FF_Encryption.Struct0.byte_0[Class362.smethod_47(FF_Encryption.Struct0.int_0, ff_Encryption_0) + 1], ff_Encryption_0, ref *(ulong*)((byte*)ptr + 32), (ulong)(*(long*)((byte*)ptr + 48)));
					Class362.smethod_68(FF_Encryption.Struct0.byte_0[Class362.smethod_47(FF_Encryption.Struct0.int_0, ff_Encryption_0) + 2], ff_Encryption_0, ref *(ulong*)((byte*)ptr + 40), (ulong)(*(long*)((byte*)ptr + 32)));
					Class362.smethod_68(FF_Encryption.Struct0.byte_0[Class362.smethod_47(FF_Encryption.Struct0.int_0, ff_Encryption_0) + 3], ff_Encryption_0, ref *(ulong*)((byte*)ptr + 32), (ulong)(*(long*)((byte*)ptr + 40)));
					Class362.smethod_68(FF_Encryption.Struct0.byte_0[Class362.smethod_47(FF_Encryption.Struct0.int_0, ff_Encryption_0) + 4], ff_Encryption_0, ref *(ulong*)((byte*)ptr + 40), (ulong)(*(long*)((byte*)ptr + 32)));
					Class362.smethod_68(FF_Encryption.Struct0.byte_0[Class362.smethod_47(FF_Encryption.Struct0.int_0, ff_Encryption_0) + 5], ff_Encryption_0, ref *(ulong*)((byte*)ptr + 32), (ulong)(*(long*)((byte*)ptr + 40)));
					Class362.smethod_68(FF_Encryption.Struct0.byte_0[Class362.smethod_47(FF_Encryption.Struct0.int_0, ff_Encryption_0) + 6], ff_Encryption_0, ref *(ulong*)((byte*)ptr + 40), (ulong)(*(long*)((byte*)ptr + 32)));
					byte_1[(int)(*(long*)((byte*)ptr + 72))] = ff_Encryption_0.byte_1[(int)(checked((IntPtr)(*(long*)(unchecked((byte*)ptr + 40)) & 255L)))] - FF_Encryption.Struct0.byte_0[Class362.smethod_47(FF_Encryption.Struct0.int_0, ff_Encryption_0) + 7];
					*(long*)((byte*)ptr + 72) = *(long*)((byte*)ptr + 72) + 1L;
				}
				*(long*)((byte*)ptr + 80) = (long)FF_Encryption.Struct0.int_0;
				*(long*)((byte*)ptr + 24) = (long)((Class362.smethod_32(10, ff_Encryption_0, Class362.smethod_32(10, ff_Encryption_0, (ulong)(*(long*)((byte*)ptr + 80))) | (ulong)(*(long*)((byte*)ptr + 80))) & (ulong)(*(long*)ptr)) | (ulong)(*(long*)((byte*)ptr + 80)));
				*(long*)((byte*)ptr + 56) = (long)Class362.smethod_47(FF_Encryption.Struct0.int_0, ff_Encryption_0);
				*(long*)((byte*)ptr + 64) = (long)(((Class362.smethod_32(10, ff_Encryption_0, (ulong)(*(long*)((byte*)ptr + 24))) & (ulong)(*(long*)ptr)) | (ulong)(*(long*)((byte*)ptr + 80))) + 2707759943UL);
				byte[] array = new byte[8];
				Array.Copy(byte_1, 0, array, 0, array.Length);
				ref long ptr2 = ref *(long*)((byte*)ptr + 24);
				ulong num = Class362.smethod_60(Class362.smethod_28(ff_Encryption_0, ref array), ff_Encryption_0);
				((byte*)ptr)[92] = FF_Encryption.Struct0.byte_0[(int)(checked((IntPtr)(unchecked(*(long*)((byte*)ptr + 56) + 7L))))];
				((byte*)ptr)[93] = FF_Encryption.Struct0.byte_0[(int)(checked((IntPtr)(unchecked(*(long*)((byte*)ptr + 56) + 6L))))];
				((byte*)ptr)[94] = FF_Encryption.Struct0.byte_0[(int)(checked((IntPtr)(unchecked(*(long*)((byte*)ptr + 56) + 5L))))];
				((byte*)ptr)[95] = FF_Encryption.Struct0.byte_0[(int)(checked((IntPtr)(unchecked(*(long*)((byte*)ptr + 56) + 4L))))];
				((byte*)ptr)[96] = FF_Encryption.Struct0.byte_0[(int)(checked((IntPtr)(unchecked(*(long*)((byte*)ptr + 56) + 3L))))];
				((byte*)ptr)[97] = FF_Encryption.Struct0.byte_0[(int)(checked((IntPtr)(unchecked(*(long*)((byte*)ptr + 56) + 2L))))];
				((byte*)ptr)[98] = FF_Encryption.Struct0.byte_0[(int)(checked((IntPtr)(unchecked(*(long*)((byte*)ptr + 56) + 1L))))];
				((byte*)ptr)[99] = FF_Encryption.Struct0.byte_0[(int)(checked((IntPtr)(*(long*)(unchecked((byte*)ptr + 56)))))];
				ptr2 = (long)Class362.smethod_60(num - Class362.smethod_58(((byte*)ptr)[96], ((byte*)ptr)[98], ff_Encryption_0, ((byte*)ptr)[94], ((byte*)ptr)[92], ((byte*)ptr)[99], ((byte*)ptr)[97], ((byte*)ptr)[95], ((byte*)ptr)[93]), ff_Encryption_0);
				Class362.smethod_77((ulong)(*(long*)((byte*)ptr + 64) & *(long*)((byte*)ptr + 16)), ref *(ulong*)((byte*)ptr + 32), ff_Encryption_0);
				Class362.smethod_77((ulong)(*(long*)((byte*)ptr + 64)) >> 32, ref *(ulong*)((byte*)ptr + 64), ff_Encryption_0);
				Array.Copy(FF_Encryption.Struct0.byte_0, Class362.smethod_47(FF_Encryption.Struct0.int_0, ff_Encryption_0), array, 0, array.Length);
				*(long*)((byte*)ptr + 32) = (long)(Class362.smethod_28(ff_Encryption_0, ref array) ^ (ulong)(*(long*)((byte*)ptr + 24) ^ ((*(long*)((byte*)ptr + 64) & *(long*)((byte*)ptr + 16) & ~(*(long*)((byte*)ptr + 8))) | (long)(Class362.smethod_32(32, ff_Encryption_0, (ulong)(*(long*)((byte*)ptr + 32))) & (ulong)(*(long*)((byte*)ptr + 8))))));
				array = BitConverter.GetBytes(((ulong)(*(long*)((byte*)ptr + 32)) >> 32 & (ulong)(*(long*)((byte*)ptr + 16))) | (Class362.smethod_32(32, ff_Encryption_0, (ulong)(*(long*)((byte*)ptr + 32))) & (ulong)(*(long*)((byte*)ptr + 8))));
				Array.Reverse(array);
				Array.Copy(array, 0, byte_0, 0, array.Length);
				*(int*)((byte*)ptr + 88) = *(int*)((byte*)ptr + 88) + 1;
			}
		}

		// Token: 0x0600677E RID: 26494 RVA: 0x002218F0 File Offset: 0x0021FAF0
		unsafe static bool smethod_21(Class371.Class376 class376_0, Class371.Class373 class373_0)
		{
			void* ptr = stackalloc byte[12];
			for (;;)
			{
				*(int*)((byte*)ptr + 8) = class376_0.int_2;
				switch (*(int*)((byte*)ptr + 8))
				{
				case 0:
					class376_0.int_3 = Class362.smethod_98(class373_0, 5);
					if (class376_0.int_3 >= 0)
					{
						class376_0.int_3 += 257;
						Class362.smethod_13(class373_0, 5);
						class376_0.int_2 = 1;
						goto IL_1C6;
					}
					return false;
				case 1:
					goto IL_1C6;
				case 2:
					goto IL_178;
				case 3:
					goto IL_13F;
				case 4:
					break;
				case 5:
					goto IL_0B;
				default:
					continue;
				}
				IL_C7:
				int num;
				while (((num = Class362.smethod_23(class376_0.class375_0, class373_0)) & -16) == 0)
				{
					class376_0.byte_1[class376_0.int_8++] = (class376_0.byte_2 = (byte)num);
					if (class376_0.int_8 == class376_0.int_6)
					{
						return true;
					}
				}
				if (num >= 0)
				{
					if (num >= 17)
					{
						class376_0.byte_2 = 0;
					}
					class376_0.int_7 = num - 16;
					class376_0.int_2 = 5;
					goto IL_0B;
				}
				return false;
				IL_13F:
				while (class376_0.int_8 < class376_0.int_5)
				{
					*(int*)ptr = Class362.smethod_98(class373_0, 3);
					if (*(int*)ptr < 0)
					{
						return false;
					}
					Class362.smethod_13(class373_0, 3);
					class376_0.byte_0[Class371.Class376.int_9[class376_0.int_8]] = (byte)(*(int*)ptr);
					class376_0.int_8++;
				}
				class376_0.class375_0 = new Class371.Class375(class376_0.byte_0);
				class376_0.byte_0 = null;
				class376_0.int_8 = 0;
				class376_0.int_2 = 4;
				goto IL_C7;
				IL_0B:
				*(int*)((byte*)ptr + 4) = Class371.Class376.int_1[class376_0.int_7];
				int num2 = Class362.smethod_98(class373_0, *(int*)((byte*)ptr + 4));
				if (num2 < 0)
				{
					return false;
				}
				Class362.smethod_13(class373_0, *(int*)((byte*)ptr + 4));
				num2 += Class371.Class376.int_0[class376_0.int_7];
				while (num2-- > 0)
				{
					class376_0.byte_1[class376_0.int_8++] = class376_0.byte_2;
				}
				if (class376_0.int_8 == class376_0.int_6)
				{
					break;
				}
				class376_0.int_2 = 4;
				continue;
				IL_178:
				class376_0.int_5 = Class362.smethod_98(class373_0, 4);
				if (class376_0.int_5 >= 0)
				{
					class376_0.int_5 += 4;
					Class362.smethod_13(class373_0, 4);
					class376_0.byte_0 = new byte[19];
					class376_0.int_8 = 0;
					class376_0.int_2 = 3;
					goto IL_13F;
				}
				return false;
				IL_1C6:
				class376_0.int_4 = Class362.smethod_98(class373_0, 5);
				if (class376_0.int_4 >= 0)
				{
					class376_0.int_4++;
					Class362.smethod_13(class373_0, 5);
					class376_0.int_6 = class376_0.int_3 + class376_0.int_4;
					class376_0.byte_1 = new byte[class376_0.int_6];
					class376_0.int_2 = 2;
					goto IL_178;
				}
				return false;
			}
			return true;
		}

		// Token: 0x0600677F RID: 26495 RVA: 0x00221B9C File Offset: 0x0021FD9C
		unsafe static bool smethod_22(Class371.Class372 class372_0)
		{
			void* ptr = stackalloc byte[12];
			int i = Class362.smethod_42(class372_0.class374_0);
			while (i >= 258)
			{
				*(int*)((byte*)ptr + 8) = class372_0.int_4;
				int num;
				switch (*(int*)((byte*)ptr + 8))
				{
				case 7:
					while (((num = Class362.smethod_23(class372_0.class375_0, class372_0.class373_0)) & -256) == 0)
					{
						Class362.smethod_88(class372_0.class374_0, num);
						if (--i < 258)
						{
							return true;
						}
					}
					if (num >= 257)
					{
						class372_0.int_6 = Class371.Class372.int_0[num - 257];
						class372_0.int_5 = Class371.Class372.int_1[num - 257];
						goto IL_A8;
					}
					if (num < 0)
					{
						return false;
					}
					class372_0.class375_1 = null;
					class372_0.class375_0 = null;
					class372_0.int_4 = 2;
					return true;
				case 8:
					goto IL_A8;
				case 9:
					goto IL_FB;
				case 10:
					break;
				default:
					continue;
				}
				IL_12E:
				if (class372_0.int_5 > 0)
				{
					class372_0.int_4 = 10;
					*(int*)((byte*)ptr + 4) = Class362.smethod_98(class372_0.class373_0, class372_0.int_5);
					if (*(int*)((byte*)ptr + 4) < 0)
					{
						return false;
					}
					Class362.smethod_13(class372_0.class373_0, class372_0.int_5);
					class372_0.int_7 += *(int*)((byte*)ptr + 4);
				}
				Class362.smethod_2(class372_0.class374_0, class372_0.int_6, class372_0.int_7);
				i -= class372_0.int_6;
				class372_0.int_4 = 7;
				continue;
				IL_FB:
				num = Class362.smethod_23(class372_0.class375_1, class372_0.class373_0);
				if (num >= 0)
				{
					class372_0.int_7 = Class371.Class372.int_2[num];
					class372_0.int_5 = Class371.Class372.int_3[num];
					goto IL_12E;
				}
				return false;
				IL_A8:
				if (class372_0.int_5 > 0)
				{
					class372_0.int_4 = 8;
					*(int*)ptr = Class362.smethod_98(class372_0.class373_0, class372_0.int_5);
					if (*(int*)ptr < 0)
					{
						return false;
					}
					Class362.smethod_13(class372_0.class373_0, class372_0.int_5);
					class372_0.int_6 += *(int*)ptr;
				}
				class372_0.int_4 = 9;
				goto IL_FB;
			}
			return true;
		}

		// Token: 0x06006780 RID: 26496 RVA: 0x00221D80 File Offset: 0x0021FF80
		unsafe static int smethod_23(Class371.Class375 class375_0, Class371.Class373 class373_0)
		{
			void* ptr = stackalloc byte[16];
			int num;
			if ((num = Class362.smethod_98(class373_0, 9)) >= 0)
			{
				int num2;
				if ((num2 = (int)class375_0.short_0[num]) >= 0)
				{
					Class362.smethod_13(class373_0, num2 & 15);
					return num2 >> 4;
				}
				*(int*)ptr = -(num2 >> 4);
				*(int*)((byte*)ptr + 4) = (num2 & 15);
				if ((num = Class362.smethod_98(class373_0, *(int*)((byte*)ptr + 4))) >= 0)
				{
					num2 = (int)class375_0.short_0[*(int*)ptr | num >> 9];
					Class362.smethod_13(class373_0, num2 & 15);
					return num2 >> 4;
				}
				*(int*)((byte*)ptr + 8) = class373_0.int_2;
				num = Class362.smethod_98(class373_0, *(int*)((byte*)ptr + 8));
				num2 = (int)class375_0.short_0[*(int*)ptr | num >> 9];
				if ((num2 & 15) <= *(int*)((byte*)ptr + 8))
				{
					Class362.smethod_13(class373_0, num2 & 15);
					return num2 >> 4;
				}
				return -1;
			}
			else
			{
				*(int*)((byte*)ptr + 12) = class373_0.int_2;
				num = Class362.smethod_98(class373_0, *(int*)((byte*)ptr + 12));
				int num2 = (int)class375_0.short_0[num];
				if (num2 >= 0 && (num2 & 15) <= *(int*)((byte*)ptr + 12))
				{
					Class362.smethod_13(class373_0, num2 & 15);
					return num2 >> 4;
				}
				return -1;
			}
		}

		// Token: 0x06006781 RID: 26497 RVA: 0x0004BD65 File Offset: 0x00049F65
		static void smethod_24(FF_Encryption ff_Encryption_0)
		{
			FF_Encryption.Struct0.uint_0 = 0U;
			FF_Encryption.Struct0.int_0 = 0;
		}

		// Token: 0x06006782 RID: 26498 RVA: 0x00221E74 File Offset: 0x00220074
		static byte[] smethod_25(int int_0, byte[] byte_0, byte[] byte_1)
		{
			AesCryptoServiceProvider aesCryptoServiceProvider = new AesCryptoServiceProvider();
			aesCryptoServiceProvider.Mode = CipherMode.CBC;
			aesCryptoServiceProvider.Padding = PaddingMode.Zeros;
			byte[] rgbKey = Class362.smethod_71("syscon_manager_key");
			if (byte_1.Length != 16)
			{
				Array.Resize<byte>(ref byte_1, 16);
			}
			return aesCryptoServiceProvider.CreateDecryptor(rgbKey, byte_1).TransformFinalBlock(byte_0, 0, int_0);
		}

		// Token: 0x06006783 RID: 26499 RVA: 0x00221EC0 File Offset: 0x002200C0
		unsafe static void smethod_26(Class4 class4_0, Stream stream_0)
		{
			void* ptr = stackalloc byte[36];
			Class362.smethod_52(class4_0, "Initializing Param.PFD stream..", Enum0.const_0);
			using (BinaryReader binaryReader = new BinaryReader(stream_0))
			{
				Class4.Struct1.ulong_0 = binaryReader.ReadUInt64().smethod_1();
				if (Class4.Struct1.ulong_0 != 1346782274UL)
				{
					Class362.smethod_52(class4_0, "Invalid PFD File!", Enum0.const_2);
					throw new Exception("Invalid PFD File!");
				}
				Class4.Struct1.ulong_1 = binaryReader.ReadUInt64().smethod_1();
				if (Class4.Struct1.ulong_1 != 3UL && Class4.Struct1.ulong_1 != 4UL)
				{
					Class362.smethod_52(class4_0, "Unsupported PFD version!", Enum0.const_2);
					throw new Exception("Unsupported PFD version!");
				}
				Class362.smethod_52(class4_0, "Allocating Header Data..", Enum0.const_0);
				Class4.byte_0 = binaryReader.ReadBytes(16);
				byte[] array = binaryReader.ReadBytes(64);
				array = Class362.smethod_25(array.Length, array, Class4.byte_0);
				Class4.Struct2.byte_0 = new byte[20];
				Array.Copy(array, 0, Class4.Struct2.byte_0, 0, 20);
				Class4.Struct2.byte_1 = new byte[20];
				Array.Copy(array, 20, Class4.Struct2.byte_1, 0, 20);
				Class4.Struct2.byte_2 = new byte[20];
				Array.Copy(array, 40, Class4.Struct2.byte_2, 0, 20);
				Class4.Struct2.byte_3 = new byte[4];
				Array.Copy(array, 60, Class4.Struct2.byte_3, 0, 4);
				if (Class4.Struct1.ulong_1 == 4UL)
				{
					byte[] byte_ = Class362.smethod_71("keygen_key");
					byte[] byte_2 = Class4.Struct2.byte_2;
					Class4.byte_5 = Class362.smethod_15(20, byte_2, 0, byte_);
				}
				else
				{
					Class4.byte_5 = Class4.Struct2.byte_2;
				}
				Class362.smethod_52(class4_0, "Reading Entries..", Enum0.const_0);
				Class4.Struct3.ulong_0 = binaryReader.ReadUInt64().smethod_1();
				Class4.Struct3.ulong_1 = binaryReader.ReadUInt64().smethod_1();
				Class4.Struct3.ulong_2 = binaryReader.ReadUInt64().smethod_1();
				Class4.Struct3.list_0 = new List<ulong>();
				Class362.smethod_52(class4_0, "Reading table capicity (" + Class4.Struct3.ulong_0 + " entries)..", Enum0.const_0);
				*(long*)ptr = 0L;
				while (*(long*)ptr < (long)Class4.Struct3.ulong_0)
				{
					Class4.Struct3.list_0.Add(binaryReader.ReadUInt64().smethod_1());
					*(long*)ptr = *(long*)ptr + 1L;
				}
				Class4.Struct4.list_0 = new List<Class4.Struct5>();
				Class362.smethod_52(class4_0, "Reading used tables (" + Class4.Struct3.ulong_2 + " entries)..", Enum0.const_0);
				*(long*)((byte*)ptr + 8) = 0L;
				while (*(long*)((byte*)ptr + 8) < (long)Class4.Struct3.ulong_2)
				{
					Class4.Struct5 item = default(Class4.Struct5);
					item.ulong_0 = binaryReader.ReadUInt64().smethod_1();
					item.string_0 = Encoding.ASCII.GetString(binaryReader.ReadBytes(65)).Replace("\0", "");
					item.byte_0 = binaryReader.ReadBytes(7);
					item.byte_1 = binaryReader.ReadBytes(64);
					item.list_0 = new List<byte[]>();
					*(int*)((byte*)ptr + 32) = 0;
					while (*(int*)((byte*)ptr + 32) < 4)
					{
						item.list_0.Add(binaryReader.ReadBytes(20));
						*(int*)((byte*)ptr + 32) = *(int*)((byte*)ptr + 32) + 1;
					}
					item.byte_2 = binaryReader.ReadBytes(40);
					item.ulong_1 = binaryReader.ReadUInt64().smethod_1();
					Class4.Struct4.list_0.Add(item);
					*(long*)((byte*)ptr + 8) = *(long*)((byte*)ptr + 8) + 1L;
				}
				*(long*)((byte*)ptr + 16) = binaryReader.BaseStream.Position + (long)(272UL * (Class4.Struct3.ulong_1 - Class4.Struct3.ulong_2));
				binaryReader.BaseStream.Position = *(long*)((byte*)ptr + 16);
				Class4.Struct6.list_0 = new List<byte[]>();
				Class362.smethod_52(class4_0, "Reading file table hashes (" + Class4.Struct3.ulong_0 + " entries)..", Enum0.const_0);
				*(long*)((byte*)ptr + 24) = 0L;
				while (*(long*)((byte*)ptr + 24) < (long)Class4.Struct3.ulong_0)
				{
					Class4.Struct6.list_0.Add(binaryReader.ReadBytes(20));
					*(long*)((byte*)ptr + 24) = *(long*)((byte*)ptr + 24) + 1L;
				}
				binaryReader.Close();
			}
		}

		// Token: 0x06006784 RID: 26500 RVA: 0x002222D8 File Offset: 0x002204D8
		static bool smethod_27(string string_0, bool bool_0, Class4 class4_0)
		{
			for (int i = 0; i < Class4.Struct4.list_0.Count; i++)
			{
				string text = string_0 + "\\" + Class4.Struct4.list_0[i].string_0;
				if (!File.Exists(text))
				{
					return false;
				}
				if (!class4_0.method_2(text, bool_0))
				{
					return false;
				}
			}
			return true;
		}

		// Token: 0x06006785 RID: 26501 RVA: 0x00222330 File Offset: 0x00220530
		unsafe static ulong smethod_28(FF_Encryption ff_Encryption_0, ref byte[] byte_0)
		{
			void* ptr = stackalloc byte[16];
			*(long*)ptr = (long)((int)byte_0[3] | (int)byte_0[2] << 8 | (int)byte_0[1] << 16 | (int)byte_0[0] << 24);
			*(long*)((byte*)ptr + 8) = (long)((int)byte_0[7] | (int)byte_0[6] << 8 | (int)byte_0[5] << 16 | (int)byte_0[4] << 24);
			return (ulong)(*(long*)ptr << 32 | (*(long*)((byte*)ptr + 8) & 4294967295L));
		}

		// Token: 0x06006786 RID: 26502 RVA: 0x0004BD73 File Offset: 0x00049F73
		static byte[] smethod_29(Class4 class4_0)
		{
			return Class4.byte_8;
		}

		// Token: 0x06006787 RID: 26503 RVA: 0x00222398 File Offset: 0x00220598
		static void smethod_30(Class371.Class374 class374_0, int int_0, int int_1, int int_2)
		{
			while (int_1-- > 0)
			{
				class374_0.byte_0[class374_0.int_0++] = class374_0.byte_0[int_0++];
				class374_0.int_0 &= 32767;
				int_0 &= 32767;
			}
		}

		// Token: 0x06006788 RID: 26504 RVA: 0x0004BD7A File Offset: 0x00049F7A
		static int smethod_31(int int_0)
		{
			return int_0 + 16 - 1 & -16;
		}

		// Token: 0x06006789 RID: 26505 RVA: 0x0004BD85 File Offset: 0x00049F85
		static ulong smethod_32(int int_0, FF_Encryption ff_Encryption_0, ulong ulong_0)
		{
			return ulong_0 << int_0 | ulong_0 >> 64 - int_0;
		}

		// Token: 0x0600678A RID: 26506 RVA: 0x002223F4 File Offset: 0x002205F4
		static byte[] smethod_33(int int_0, byte[] byte_0, byte[] byte_1)
		{
			AesCryptoServiceProvider aesCryptoServiceProvider = new AesCryptoServiceProvider();
			aesCryptoServiceProvider.Mode = CipherMode.CBC;
			aesCryptoServiceProvider.Padding = PaddingMode.Zeros;
			byte[] rgbKey = Class362.smethod_71("syscon_manager_key");
			if (byte_0.Length != 16)
			{
				Array.Resize<byte>(ref byte_0, 16);
			}
			return aesCryptoServiceProvider.CreateEncryptor(rgbKey, byte_0).TransformFinalBlock(byte_1, 0, int_0);
		}

		// Token: 0x0600678B RID: 26507 RVA: 0x00222440 File Offset: 0x00220640
		static ICryptoTransform smethod_34(bool bool_0, byte[] byte_0, byte[] byte_1)
		{
			ICryptoTransform result;
			using (DESCryptoServiceProvider descryptoServiceProvider = new DESCryptoServiceProvider())
			{
				result = (bool_0 ? descryptoServiceProvider.CreateDecryptor(byte_1, byte_0) : descryptoServiceProvider.CreateEncryptor(byte_1, byte_0));
			}
			return result;
		}

		// Token: 0x0600678C RID: 26508 RVA: 0x00222488 File Offset: 0x00220688
		unsafe static int smethod_35(Class371.Class374 class374_0, Class371.Class373 class373_0, int int_0)
		{
			void* ptr = stackalloc byte[8];
			int_0 = Math.Min(Math.Min(int_0, 32768 - class374_0.int_1), Class362.smethod_81(class373_0));
			*(int*)((byte*)ptr + 4) = 32768 - class374_0.int_0;
			if (int_0 > *(int*)((byte*)ptr + 4))
			{
				*(int*)ptr = Class362.smethod_90(class373_0, class374_0.byte_0, class374_0.int_0, *(int*)((byte*)ptr + 4));
				if (*(int*)ptr == *(int*)((byte*)ptr + 4))
				{
					*(int*)ptr = *(int*)ptr + Class362.smethod_90(class373_0, class374_0.byte_0, 0, int_0 - *(int*)((byte*)ptr + 4));
				}
			}
			else
			{
				*(int*)ptr = Class362.smethod_90(class373_0, class374_0.byte_0, class374_0.int_0, int_0);
			}
			class374_0.int_0 = (class374_0.int_0 + *(int*)ptr & 32767);
			class374_0.int_1 += *(int*)ptr;
			return *(int*)ptr;
		}

		// Token: 0x0600678D RID: 26509 RVA: 0x00222548 File Offset: 0x00220748
		static int smethod_36(Class7 class7_0, string string_0)
		{
			int result;
			try
			{
				int num = (Class7.class8_0 = Class362.smethod_59(string_0)).Length;
				byte[] array = Class362.smethod_39(Class362.smethod_56(class7_0.method_4()));
				if (array != null)
				{
					Class362.smethod_95(class7_0.method_2(), array);
				}
				result = num;
			}
			catch
			{
				result = -1;
			}
			return result;
		}

		// Token: 0x0600678E RID: 26510
		[DllImport("kernel32")]
		static extern bool MoveFileEx(string string_0, string string_1, int int_0);

		// Token: 0x0600678F RID: 26511 RVA: 0x002225A0 File Offset: 0x002207A0
		unsafe static byte[] smethod_37(Class4 class4_0, int int_0)
		{
			void* ptr = stackalloc byte[16];
			if (int_0 >= Class4.Struct4.list_0.Count)
			{
				throw new Exception("entryindex is out of bounds");
			}
			*(long*)ptr = (long)Class362.smethod_10(class4_0, Class4.Struct4.list_0[int_0].string_0);
			*(long*)((byte*)ptr + 8) = (long)Class4.Struct3.list_0[(int)(*(long*)ptr)];
			if (*(long*)((byte*)ptr + 8) < (long)Class4.Struct3.ulong_1)
			{
				HMACSHA1 hmacsha = new HMACSHA1(Class4.byte_5);
				while (*(long*)((byte*)ptr + 8) < (long)Class4.Struct3.ulong_1)
				{
					Class4.Struct5 @struct = Class4.Struct4.list_0[(int)(*(long*)((byte*)ptr + 8))];
					hmacsha.ComputeHash(@struct.method_1());
					*(long*)((byte*)ptr + 8) = (long)@struct.ulong_0;
				}
				return hmacsha.Hash;
			}
			return null;
		}

		// Token: 0x06006790 RID: 26512 RVA: 0x0022264C File Offset: 0x0022084C
		unsafe static void smethod_38(byte[] byte_0, int int_0, ref byte[] byte_1, FF_Encryption ff_Encryption_0)
		{
			void* ptr = stackalloc byte[100];
			Class362.smethod_24(ff_Encryption_0);
			*(long*)ptr = -1024L;
			*(long*)((byte*)ptr + 8) = -4294967296L;
			*(long*)((byte*)ptr + 16) = 4294967295L;
			*(long*)((byte*)ptr + 24) = 0L;
			*(long*)((byte*)ptr + 32) = 0L;
			*(long*)((byte*)ptr + 40) = 0L;
			*(long*)((byte*)ptr + 48) = 0L;
			*(long*)((byte*)ptr + 56) = 0L;
			*(long*)((byte*)ptr + 64) = 0L;
			*(long*)((byte*)ptr + 72) = 0L;
			*(int*)((byte*)ptr + 88) = 0;
			while (*(int*)((byte*)ptr + 88) < int_0 >> 3)
			{
				FF_Encryption.Struct0.int_0 = *(int*)((byte*)ptr + 88) << 3;
				*(long*)((byte*)ptr + 24) = (long)((Class362.smethod_100(ff_Encryption_0, (ulong)((long)FF_Encryption.Struct0.int_0), 29) & 255UL) ^ 69UL);
				*(long*)((byte*)ptr + 72) = 0L;
				while (*(long*)((byte*)ptr + 72) < 8L)
				{
					*(long*)((byte*)ptr + 40) = (*(long*)((byte*)ptr + 24) & 255L);
					*(long*)((byte*)ptr + 24) = (long)((ulong)byte_0[FF_Encryption.Struct0.int_0 + (int)(*(long*)((byte*)ptr + 72))]);
					Class362.smethod_68(FF_Encryption.Struct0.byte_0[Class362.smethod_47(FF_Encryption.Struct0.int_0, ff_Encryption_0)], ff_Encryption_0, ref *(ulong*)((byte*)ptr + 48), (ulong)(*(long*)((byte*)ptr + 24) ^ *(long*)((byte*)ptr + 40)));
					Class362.smethod_68(FF_Encryption.Struct0.byte_0[Class362.smethod_47(FF_Encryption.Struct0.int_0, ff_Encryption_0) + 1], ff_Encryption_0, ref *(ulong*)((byte*)ptr + 32), (ulong)(*(long*)((byte*)ptr + 48)));
					Class362.smethod_68(FF_Encryption.Struct0.byte_0[Class362.smethod_47(FF_Encryption.Struct0.int_0, ff_Encryption_0) + 2], ff_Encryption_0, ref *(ulong*)((byte*)ptr + 40), (ulong)(*(long*)((byte*)ptr + 32)));
					Class362.smethod_68(FF_Encryption.Struct0.byte_0[Class362.smethod_47(FF_Encryption.Struct0.int_0, ff_Encryption_0) + 3], ff_Encryption_0, ref *(ulong*)((byte*)ptr + 32), (ulong)(*(long*)((byte*)ptr + 40)));
					Class362.smethod_68(FF_Encryption.Struct0.byte_0[Class362.smethod_47(FF_Encryption.Struct0.int_0, ff_Encryption_0) + 4], ff_Encryption_0, ref *(ulong*)((byte*)ptr + 40), (ulong)(*(long*)((byte*)ptr + 32)));
					Class362.smethod_68(FF_Encryption.Struct0.byte_0[Class362.smethod_47(FF_Encryption.Struct0.int_0, ff_Encryption_0) + 5], ff_Encryption_0, ref *(ulong*)((byte*)ptr + 32), (ulong)(*(long*)((byte*)ptr + 40)));
					Class362.smethod_68(FF_Encryption.Struct0.byte_0[Class362.smethod_47(FF_Encryption.Struct0.int_0, ff_Encryption_0) + 6], ff_Encryption_0, ref *(ulong*)((byte*)ptr + 40), (ulong)(*(long*)((byte*)ptr + 32)));
					byte_0[FF_Encryption.Struct0.int_0 + (int)(*(long*)((byte*)ptr + 72))] = ff_Encryption_0.byte_1[(int)(checked((IntPtr)(*(long*)(unchecked((byte*)ptr + 40)) & 255L)))] - FF_Encryption.Struct0.byte_0[Class362.smethod_47(FF_Encryption.Struct0.int_0, ff_Encryption_0) + 7];
					*(long*)((byte*)ptr + 72) = *(long*)((byte*)ptr + 72) + 1L;
				}
				*(long*)((byte*)ptr + 80) = (long)FF_Encryption.Struct0.int_0;
				*(long*)((byte*)ptr + 24) = (long)((Class362.smethod_32(10, ff_Encryption_0, Class362.smethod_32(10, ff_Encryption_0, (ulong)(*(long*)((byte*)ptr + 80))) | (ulong)(*(long*)((byte*)ptr + 80))) & (ulong)(*(long*)ptr)) | (ulong)(*(long*)((byte*)ptr + 80)));
				*(long*)((byte*)ptr + 56) = (long)Class362.smethod_47(FF_Encryption.Struct0.int_0, ff_Encryption_0);
				*(long*)((byte*)ptr + 64) = (long)(((Class362.smethod_32(10, ff_Encryption_0, (ulong)(*(long*)((byte*)ptr + 24))) & (ulong)(*(long*)ptr)) | (ulong)(*(long*)((byte*)ptr + 80))) + 2707759943UL);
				byte[] array = new byte[8];
				Array.Copy(byte_0, FF_Encryption.Struct0.int_0, array, 0, array.Length);
				ref long ptr2 = ref *(long*)((byte*)ptr + 24);
				ulong num = Class362.smethod_60(Class362.smethod_28(ff_Encryption_0, ref array), ff_Encryption_0);
				((byte*)ptr)[92] = FF_Encryption.Struct0.byte_0[(int)(checked((IntPtr)(unchecked(*(long*)((byte*)ptr + 56) + 7L))))];
				((byte*)ptr)[93] = FF_Encryption.Struct0.byte_0[(int)(checked((IntPtr)(unchecked(*(long*)((byte*)ptr + 56) + 6L))))];
				((byte*)ptr)[94] = FF_Encryption.Struct0.byte_0[(int)(checked((IntPtr)(unchecked(*(long*)((byte*)ptr + 56) + 5L))))];
				((byte*)ptr)[95] = FF_Encryption.Struct0.byte_0[(int)(checked((IntPtr)(unchecked(*(long*)((byte*)ptr + 56) + 4L))))];
				((byte*)ptr)[96] = FF_Encryption.Struct0.byte_0[(int)(checked((IntPtr)(unchecked(*(long*)((byte*)ptr + 56) + 3L))))];
				((byte*)ptr)[97] = FF_Encryption.Struct0.byte_0[(int)(checked((IntPtr)(unchecked(*(long*)((byte*)ptr + 56) + 2L))))];
				((byte*)ptr)[98] = FF_Encryption.Struct0.byte_0[(int)(checked((IntPtr)(unchecked(*(long*)((byte*)ptr + 56) + 1L))))];
				((byte*)ptr)[99] = FF_Encryption.Struct0.byte_0[(int)(checked((IntPtr)(*(long*)(unchecked((byte*)ptr + 56)))))];
				ptr2 = (long)Class362.smethod_60(num - Class362.smethod_58(((byte*)ptr)[96], ((byte*)ptr)[98], ff_Encryption_0, ((byte*)ptr)[94], ((byte*)ptr)[92], ((byte*)ptr)[99], ((byte*)ptr)[97], ((byte*)ptr)[95], ((byte*)ptr)[93]), ff_Encryption_0);
				Class362.smethod_77((ulong)(*(long*)((byte*)ptr + 64) & *(long*)((byte*)ptr + 16)), ref *(ulong*)((byte*)ptr + 32), ff_Encryption_0);
				Class362.smethod_77((ulong)(*(long*)((byte*)ptr + 64)) >> 32, ref *(ulong*)((byte*)ptr + 64), ff_Encryption_0);
				Array.Copy(FF_Encryption.Struct0.byte_0, Class362.smethod_47(FF_Encryption.Struct0.int_0, ff_Encryption_0), array, 0, array.Length);
				*(long*)((byte*)ptr + 32) = (long)(Class362.smethod_28(ff_Encryption_0, ref array) ^ (ulong)(*(long*)((byte*)ptr + 24) ^ ((*(long*)((byte*)ptr + 64) & *(long*)((byte*)ptr + 16) & ~(*(long*)((byte*)ptr + 8))) | (long)(Class362.smethod_32(32, ff_Encryption_0, (ulong)(*(long*)((byte*)ptr + 32))) & (ulong)(*(long*)((byte*)ptr + 8))))));
				array = BitConverter.GetBytes(((ulong)(*(long*)((byte*)ptr + 32)) >> 32 & (ulong)(*(long*)((byte*)ptr + 16))) | (Class362.smethod_32(32, ff_Encryption_0, (ulong)(*(long*)((byte*)ptr + 32))) & (ulong)(*(long*)((byte*)ptr + 8))));
				Array.Reverse(array);
				Array.Copy(array, 0, byte_1, FF_Encryption.Struct0.int_0, array.Length);
				FF_Encryption.Struct0.uint_0 = (uint)byte_1[4 + (*(int*)((byte*)ptr + 88) << 3)] + ((uint)byte_1[*(int*)((byte*)ptr + 88) << 3] + FF_Encryption.Struct0.uint_0);
				*(int*)((byte*)ptr + 88) = *(int*)((byte*)ptr + 88) + 1;
			}
		}

		// Token: 0x06006791 RID: 26513 RVA: 0x00222B44 File Offset: 0x00220D44
		unsafe static byte[] smethod_39(string string_0)
		{
			void* ptr = stackalloc byte[8];
			if (Class7.class8_0 != null && Class7.class8_0.Length != 0)
			{
				Class8[] class8_ = Class7.class8_0;
				*(int*)ptr = 0;
				while (*(int*)ptr < class8_.Length)
				{
					Class8 @class = class8_[*(int*)ptr];
					string[] array = @class.method_1();
					*(int*)((byte*)ptr + 4) = 0;
					while (*(int*)((byte*)ptr + 4) < array.Length)
					{
						string text = array[*(int*)((byte*)ptr + 4)];
						if (text.ToLower() == string_0.ToLower() && @class.method_3() != null && @class.method_3().Length == 32)
						{
							return @class.method_3().smethod_2();
						}
						*(int*)((byte*)ptr + 4) = *(int*)((byte*)ptr + 4) + 1;
					}
					*(int*)ptr = *(int*)ptr + 1;
				}
				return null;
			}
			return null;
		}

		// Token: 0x06006792 RID: 26514 RVA: 0x0004BD97 File Offset: 0x00049F97
		static bool smethod_40(Class6 class6_0)
		{
			return Class362.smethod_55(class6_0.method_4(), class6_0.method_0());
		}

		// Token: 0x06006793 RID: 26515 RVA: 0x00222BF8 File Offset: 0x00220DF8
		static void smethod_41()
		{
			try
			{
				AppDomain.CurrentDomain.AssemblyResolve += Class362.smethod_96;
			}
			catch
			{
			}
		}

		// Token: 0x06006794 RID: 26516 RVA: 0x0004BDAA File Offset: 0x00049FAA
		static int smethod_42(Class371.Class374 class374_0)
		{
			return 32768 - class374_0.int_1;
		}

		// Token: 0x06006795 RID: 26517 RVA: 0x0004BDB8 File Offset: 0x00049FB8
		static string smethod_43()
		{
			return Class362.smethod_75().GetString("ps3keys", Class9.cultureInfo_0);
		}

		// Token: 0x06006796 RID: 26518 RVA: 0x00222C30 File Offset: 0x00220E30
		unsafe static byte[] smethod_44(FF_Encryption ff_Encryption_0)
		{
			void* ptr = stackalloc byte[56];
			*(long*)((byte*)ptr + 24) = 0L;
			*(long*)((byte*)ptr + 32) = 4294967295L;
			*(long*)((byte*)ptr + 40) = -4294967296L;
			*(long*)((byte*)ptr + 48) = (long)FF_Encryption.Struct0.int_0;
			*(long*)ptr = ((*(long*)((byte*)ptr + 48) & *(long*)((byte*)ptr + 32) & ~(*(long*)((byte*)ptr + 40))) | (long)(Class362.smethod_32(32, ff_Encryption_0, (ulong)FF_Encryption.Struct0.uint_0) & (ulong)(*(long*)((byte*)ptr + 40))));
			*(long*)((byte*)ptr + 8) = (long)((ulong)(*(long*)ptr & *(long*)((byte*)ptr + 40)) >> 32);
			*(long*)((byte*)ptr + 16) = (long)((ulong)(*(long*)((byte*)ptr + 8)) >> 24);
			Class362.smethod_77((ulong)(*(long*)((byte*)ptr + 8)), ref *(ulong*)((byte*)ptr + 16), ff_Encryption_0);
			Class362.smethod_77((ulong)(*(long*)ptr & *(long*)((byte*)ptr + 32)), ref *(ulong*)((byte*)ptr + 24), ff_Encryption_0);
			byte[] bytes = BitConverter.GetBytes((ulong)((*(long*)((byte*)ptr + 16) & 4294967295L & ~(*(long*)((byte*)ptr + 40))) | (long)(Class362.smethod_32(32, ff_Encryption_0, (ulong)(*(long*)((byte*)ptr + 24))) & (ulong)(*(long*)((byte*)ptr + 40)))));
			Array.Reverse(bytes);
			return bytes;
		}

		// Token: 0x06006797 RID: 26519 RVA: 0x0004BDCE File Offset: 0x00049FCE
		static byte[] smethod_45(Class4 class4_0)
		{
			return Class4.byte_10;
		}

		// Token: 0x06006798 RID: 26520 RVA: 0x00222D14 File Offset: 0x00220F14
		static Class371.Class375 smethod_46(Class371.Class376 class376_0)
		{
			byte[] array = new byte[class376_0.int_4];
			Array.Copy(class376_0.byte_1, class376_0.int_3, array, 0, class376_0.int_4);
			return new Class371.Class375(array);
		}

		// Token: 0x06006799 RID: 26521 RVA: 0x0004BDD5 File Offset: 0x00049FD5
		static int smethod_47(int int_0, FF_Encryption ff_Encryption_0)
		{
			return int_0 & 248;
		}

		// Token: 0x0600679A RID: 26522 RVA: 0x0004BDDE File Offset: 0x00049FDE
		static bool smethod_48(Class371.Class373 class373_0)
		{
			return class373_0.int_0 == class373_0.int_1;
		}

		// Token: 0x0600679B RID: 26523 RVA: 0x0004BDEE File Offset: 0x00049FEE
		static void smethod_49(Class371.Class373 class373_0)
		{
			class373_0.uint_0 >>= (class373_0.int_2 & 7);
			class373_0.int_2 &= -8;
		}

		// Token: 0x0600679C RID: 26524 RVA: 0x00222D4C File Offset: 0x00220F4C
		static string smethod_50(BinaryReader binaryReader_0, Class5 class5_0, Class5.Struct8 struct8_0)
		{
			binaryReader_0.BaseStream.Position = (long)((ulong)(Class5.Struct7.uint_0 + (uint)struct8_0.ushort_0));
			string text = "";
			while ((byte)binaryReader_0.PeekChar() != 0)
			{
				text += binaryReader_0.ReadChar();
			}
			binaryReader_0.BaseStream.Position += 1L;
			return text;
		}

		// Token: 0x0600679D RID: 26525 RVA: 0x00222DB4 File Offset: 0x00220FB4
		static Class8[] smethod_51()
		{
			Class1.Class3 @class = new Class1.Class3();
			@class.class8_0 = new Class8[0];
			Thread thread = new Thread(new ThreadStart(@class.method_0));
			thread.Start();
			while (thread.ThreadState != ThreadState.Stopped)
			{
				Application.DoEvents();
			}
			return @class.class8_0;
		}

		// Token: 0x0600679E RID: 26526 RVA: 0x0004BE17 File Offset: 0x0004A017
		static void smethod_52(Class4 class4_0, string string_0, Enum0 enum0_0)
		{
			if (class4_0.delegate0_0 != null)
			{
				class4_0.delegate0_0(class4_0, string_0, enum0_0);
			}
		}

		// Token: 0x0600679F RID: 26527 RVA: 0x00222E04 File Offset: 0x00221004
		static Class371.Class375 smethod_53(Class371.Class376 class376_0)
		{
			byte[] array = new byte[class376_0.int_3];
			Array.Copy(class376_0.byte_1, 0, array, 0, class376_0.int_3);
			return new Class371.Class375(array);
		}

		// Token: 0x060067A0 RID: 26528 RVA: 0x00222E38 File Offset: 0x00221038
		static FF_Encryption.FFGame smethod_54(Form0 form0_0, int int_0, ref bool bool_0)
		{
			if (int_0 == 300216)
			{
				return FF_Encryption.FFGame.FFXIII;
			}
			if (int_0 == 307552)
			{
				bool_0 = true;
				return FF_Encryption.FFGame.FFXIII;
			}
			if (int_0 == 548176)
			{
				return FF_Encryption.FFGame.FFXIII_2;
			}
			if (int_0 == 547616)
			{
				return FF_Encryption.FFGame.FFXIII_3;
			}
			if (int_0 == 560736)
			{
				bool_0 = true;
				if (BitConverter.ToUInt32(form0_0.byte_0, form0_0.byte_0.Length - 8) != 0U)
				{
					return FF_Encryption.FFGame.FFXIII_2;
				}
				return FF_Encryption.FFGame.FFXIII_3;
			}
			else
			{
				if (int_0 == 547904)
				{
					bool_0 = true;
					return FF_Encryption.FFGame.FFXIII_3;
				}
				return FF_Encryption.FFGame.None;
			}
		}

		// Token: 0x060067A1 RID: 26529 RVA: 0x00222EA4 File Offset: 0x002210A4
		unsafe static bool smethod_55(Class4 class4_0, string string_0)
		{
			void* ptr = stackalloc byte[5];
			try
			{
				if (!File.Exists(string_0))
				{
					Class362.smethod_52(class4_0, string_0 + " Does not exist!", Enum0.const_2);
					((byte*)ptr)[4] = 0;
				}
				else
				{
					string name = new FileInfo(string_0).Name;
					if (class4_0.method_6(name))
					{
						Class362.smethod_52(class4_0, "Initializing file stream..", Enum0.const_0);
						using (FileStream fileStream = File.Open(string_0, FileMode.Open, FileAccess.ReadWrite, FileShare.None))
						{
							*(int*)ptr = 0;
							while (*(int*)ptr < Class4.Struct4.list_0.Count)
							{
								if (Class4.Struct4.list_0[*(int*)ptr].string_0.ToLower() == name.ToLower())
								{
									Class4.Struct5 value = Class4.Struct4.list_0[*(int*)ptr];
									value.ulong_1 = (ulong)fileStream.Length;
									Class4.Struct4.list_0[*(int*)ptr] = value;
								}
								*(int*)ptr = *(int*)ptr + 1;
							}
							byte[] bytes = class4_0.method_4(fileStream, name);
							Class362.smethod_52(class4_0, "Rehashing PARAM.PFD..", Enum0.const_0);
							bool flag = class4_0.method_1(fileStream, name, true) && class4_0.method_0(true) && Class362.smethod_70(true, class4_0) && Class362.smethod_8(true, class4_0);
							fileStream.Close();
							Class362.smethod_52(class4_0, "Writing Encrypted data to : " + string_0, Enum0.const_0);
							File.WriteAllBytes(string_0, bytes);
							Class362.smethod_52(class4_0, name + " is succesfully encrypted", Enum0.const_0);
							((byte*)ptr)[4] = (flag ? 1 : 0);
							goto IL_175;
						}
					}
					Class362.smethod_52(class4_0, "There is no \"" + name + "\" inside the PARAM.PFD Entries!", Enum0.const_2);
					((byte*)ptr)[4] = 0;
				}
			}
			catch (Exception ex)
			{
				Class362.smethod_52(class4_0, ex.Message, Enum0.const_2);
				((byte*)ptr)[4] = 0;
			}
			IL_175:
			return *(sbyte*)((byte*)ptr + 4) != 0;
		}

		// Token: 0x060067A2 RID: 26530 RVA: 0x00223060 File Offset: 0x00221260
		static string smethod_56(Class5 class5_0)
		{
			string text = Class362.smethod_0(class5_0);
			if (text == "")
			{
				return "";
			}
			return text.Split(new char[]
			{
				'-'
			})[0];
		}

		// Token: 0x060067A3 RID: 26531 RVA: 0x0022309C File Offset: 0x0022129C
		unsafe static ulong smethod_57(FF_Encryption ff_Encryption_0, byte[] byte_0)
		{
			void* ptr = stackalloc byte[16];
			*(long*)ptr = (long)Class362.smethod_60(BitConverter.ToUInt64(byte_0, 0), ff_Encryption_0);
			*(long*)((byte*)ptr + 8) = (long)Class362.smethod_60(BitConverter.ToUInt64(byte_0, 8), ff_Encryption_0);
			ulong num = (ulong)(*(long*)ptr ^ *(long*)((byte*)ptr + 8));
			num |= 1UL;
			return num ^ ((ff_Encryption_0.ffgame_0 == FF_Encryption.FFGame.FFXIII_2) ? 11177653904406231216UL : 3914857797994049631UL);
		}

		// Token: 0x060067A4 RID: 26532 RVA: 0x0004BE2F File Offset: 0x0004A02F
		static ulong smethod_58(byte byte_0, byte byte_1, FF_Encryption ff_Encryption_0, byte byte_2, byte byte_3, byte byte_4, byte byte_5, byte byte_6, byte byte_7)
		{
			return (ulong)byte_3 << 56 | (ulong)byte_7 << 48 | (ulong)byte_2 << 40 | (ulong)byte_6 << 32 | (ulong)byte_0 << 24 | (ulong)byte_5 << 16 | (ulong)byte_1 << 8 | (ulong)byte_4;
		}

		// Token: 0x060067A5 RID: 26533 RVA: 0x00223108 File Offset: 0x00221308
		static Class8[] smethod_59(string string_0)
		{
			List<Class8> list = new List<Class8>();
			using (StringReader stringReader = new StringReader(string_0))
			{
				while (stringReader.Peek() > -1 && !stringReader.ReadLine().Equals("; -- UNPROTECTED GAMES --"))
				{
					Application.DoEvents();
				}
				string text;
				while (stringReader.Peek() > -1 && (text = stringReader.ReadLine()).StartsWith(";"))
				{
					list.Add(new Class8(text.Replace(";", ""), "", "", "", false));
				}
				while (stringReader.Peek() > -1)
				{
					while (stringReader.Peek() > -1 && (text = stringReader.ReadLine()).StartsWith(";"))
					{
						string string_ = text.Replace(";", "");
						if ((text = stringReader.ReadLine()).StartsWith("["))
						{
							string string_2 = text;
							string string_3 = stringReader.ReadLine().Split(new char[]
							{
								'='
							})[1];
							string text2 = stringReader.ReadLine().Split(new char[]
							{
								'='
							})[1];
							stringReader.ReadLine();
							list.Add(new Class8(string_, string_2, text2, string_3, !string.IsNullOrEmpty(text2) && text2.Length == 32));
						}
					}
				}
				stringReader.Close();
			}
			return list.ToArray();
		}

		// Token: 0x060067A6 RID: 26534 RVA: 0x002232A4 File Offset: 0x002214A4
		static ulong smethod_60(ulong ulong_0, FF_Encryption ff_Encryption_0)
		{
			return (ulong_0 & 255UL) << 56 | (ulong_0 & 65280UL) << 40 | (ulong_0 & 16711680UL) << 24 | (ulong_0 & 4278190080UL) << 8 | (ulong_0 & 1095216660480UL) >> 8 | (ulong_0 & 280375465082880UL) >> 24 | (ulong_0 & 71776119061217280UL) >> 40 | (ulong_0 & 18374686479671623680UL) >> 56;
		}

		// Token: 0x060067A7 RID: 26535 RVA: 0x0004BE61 File Offset: 0x0004A061
		static short smethod_61(int int_0)
		{
			return (short)((int)Class371.Class377.byte_0[int_0 & 15] << 12 | (int)Class371.Class377.byte_0[int_0 >> 4 & 15] << 8 | (int)Class371.Class377.byte_0[int_0 >> 8 & 15] << 4 | (int)Class371.Class377.byte_0[int_0 >> 12]);
		}

		// Token: 0x060067A8 RID: 26536 RVA: 0x0004BE9A File Offset: 0x0004A09A
		static byte[] smethod_62(Class6 class6_0)
		{
			return Class362.smethod_84(class6_0.method_4(), class6_0.method_0());
		}

		// Token: 0x060067A9 RID: 26537 RVA: 0x00223328 File Offset: 0x00221528
		static bool smethod_63(Class4 class4_0, bool bool_0)
		{
			byte[] array = Class362.smethod_110(class4_0);
			if (!Class362.smethod_111(array, Class4.Struct2.byte_1))
			{
				if (!bool_0)
				{
					return false;
				}
				Class4.Struct2.byte_1 = array;
			}
			return true;
		}

		// Token: 0x060067AA RID: 26538 RVA: 0x00223358 File Offset: 0x00221558
		static ICryptoTransform smethod_64(bool bool_0, byte[] byte_0, byte[] byte_1)
		{
			ICryptoTransform result;
			using (SymmetricAlgorithm symmetricAlgorithm = new RijndaelManaged())
			{
				result = (bool_0 ? symmetricAlgorithm.CreateDecryptor(byte_0, byte_1) : symmetricAlgorithm.CreateEncryptor(byte_0, byte_1));
			}
			return result;
		}

		// Token: 0x060067AB RID: 26539 RVA: 0x002233A0 File Offset: 0x002215A0
		unsafe static byte[] smethod_65(int int_0, byte[] byte_0, byte[] byte_1)
		{
			void* ptr = stackalloc byte[12];
			Array.Resize<byte>(ref byte_0, 16);
			Aes aes = Aes.Create();
			aes.Key = byte_0;
			aes.BlockSize = 128;
			aes.Mode = CipherMode.ECB;
			aes.Padding = PaddingMode.Zeros;
			Aes aes2 = Aes.Create();
			aes2.Key = byte_0;
			aes.BlockSize = 128;
			aes2.Mode = CipherMode.ECB;
			aes2.Padding = PaddingMode.Zeros;
			*(int*)ptr = int_0 / 16;
			byte[] array = new byte[int_0];
			*(int*)((byte*)ptr + 4) = 0;
			while (*(int*)((byte*)ptr + 4) < *(int*)ptr)
			{
				byte[] array2 = new byte[16];
				Array.Copy(byte_1, *(int*)((byte*)ptr + 4) * 16, array2, 0, 16);
				byte[] array3 = new byte[16];
				Array.Copy(BitConverter.GetBytes(((ulong)((long)(*(int*)((byte*)ptr + 4)))).smethod_1()), 0, array3, 0, 8);
				array3 = aes.CreateEncryptor().TransformFinalBlock(array3, 0, array3.Length);
				array2 = aes2.CreateDecryptor().TransformFinalBlock(array2, 0, array2.Length);
				*(int*)((byte*)ptr + 8) = 0;
				while (*(int*)((byte*)ptr + 8) < 16)
				{
					byte[] array4 = array2;
					int num = *(int*)((byte*)ptr + 8);
					array4[num] ^= array3[*(int*)((byte*)ptr + 8)];
					*(int*)((byte*)ptr + 8) = *(int*)((byte*)ptr + 8) + 1;
				}
				Array.Copy(array2, 0, array, *(int*)((byte*)ptr + 4) * 16, 16);
				*(int*)((byte*)ptr + 4) = *(int*)((byte*)ptr + 4) + 1;
			}
			return array;
		}

		// Token: 0x060067AC RID: 26540 RVA: 0x002234E4 File Offset: 0x002216E4
		unsafe static void smethod_66(byte[] byte_0, int int_0, FF_Encryption ff_Encryption_0, byte[] byte_1)
		{
			void* ptr = stackalloc byte[112];
			*(long*)ptr = -1024L;
			*(long*)((byte*)ptr + 8) = -4294967296L;
			*(long*)((byte*)ptr + 16) = 4294967295L;
			*(int*)((byte*)ptr + 96) = 0;
			while (*(int*)((byte*)ptr + 96) < int_0 >> 3)
			{
				byte[] array = new byte[8];
				Array.Copy(byte_1, 0, array, 0, 8);
				*(long*)((byte*)ptr + 64) = (long)Class362.smethod_28(ff_Encryption_0, ref array);
				*(long*)((byte*)ptr + 88) = (long)FF_Encryption.Struct0.int_0;
				*(long*)((byte*)ptr + 40) = (long)((Class362.smethod_32(10, ff_Encryption_0, Class362.smethod_32(10, ff_Encryption_0, (ulong)(*(long*)((byte*)ptr + 88))) | (ulong)(*(long*)((byte*)ptr + 88))) & (ulong)(*(long*)ptr)) | (ulong)(*(long*)((byte*)ptr + 88)));
				*(long*)((byte*)ptr + 56) = (long)(2707759943UL + ((Class362.smethod_32(10, ff_Encryption_0, (ulong)(*(long*)((byte*)ptr + 40))) & (ulong)(*(long*)ptr)) | (ulong)(*(long*)((byte*)ptr + 88))));
				*(long*)((byte*)ptr + 32) = 0L;
				Class362.smethod_77((ulong)(*(long*)((byte*)ptr + 56)) >> 32, ref *(ulong*)((byte*)ptr + 32), ff_Encryption_0);
				Class362.smethod_77((ulong)(*(long*)((byte*)ptr + 56) & *(long*)((byte*)ptr + 16)), ref *(ulong*)((byte*)ptr + 40), ff_Encryption_0);
				array = new byte[8];
				Array.Copy(FF_Encryption.Struct0.byte_0, Class362.smethod_47(FF_Encryption.Struct0.int_0, ff_Encryption_0), array, 0, 8);
				*(long*)((byte*)ptr + 24) = (((*(long*)((byte*)ptr + 32) & *(long*)((byte*)ptr + 16) & ~(*(long*)((byte*)ptr + 8))) | (long)(Class362.smethod_32(32, ff_Encryption_0, (ulong)(*(long*)((byte*)ptr + 40))) & (ulong)(*(long*)((byte*)ptr + 8)))) ^ (long)Class362.smethod_28(ff_Encryption_0, ref array) ^ (long)((Class362.smethod_32(32, ff_Encryption_0, (ulong)(*(long*)((byte*)ptr + 64))) & (ulong)(*(long*)((byte*)ptr + 16))) | (Class362.smethod_32(32, ff_Encryption_0, (ulong)(*(long*)((byte*)ptr + 64))) & (ulong)(*(long*)((byte*)ptr + 8)))));
				*(long*)((byte*)ptr + 80) = (long)Class362.smethod_47(FF_Encryption.Struct0.int_0, ff_Encryption_0);
				((byte*)ptr)[104] = FF_Encryption.Struct0.byte_0[(int)(checked((IntPtr)(unchecked(*(long*)((byte*)ptr + 80) + 7L))))];
				((byte*)ptr)[105] = FF_Encryption.Struct0.byte_0[(int)(checked((IntPtr)(unchecked(*(long*)((byte*)ptr + 80) + 6L))))];
				((byte*)ptr)[106] = FF_Encryption.Struct0.byte_0[(int)(checked((IntPtr)(unchecked(*(long*)((byte*)ptr + 80) + 5L))))];
				((byte*)ptr)[107] = FF_Encryption.Struct0.byte_0[(int)(checked((IntPtr)(unchecked(*(long*)((byte*)ptr + 80) + 4L))))];
				((byte*)ptr)[108] = FF_Encryption.Struct0.byte_0[(int)(checked((IntPtr)(unchecked(*(long*)((byte*)ptr + 80) + 3L))))];
				((byte*)ptr)[109] = FF_Encryption.Struct0.byte_0[(int)(checked((IntPtr)(unchecked(*(long*)((byte*)ptr + 80) + 2L))))];
				((byte*)ptr)[110] = FF_Encryption.Struct0.byte_0[(int)(checked((IntPtr)(unchecked(*(long*)((byte*)ptr + 80) + 1L))))];
				((byte*)ptr)[111] = FF_Encryption.Struct0.byte_0[(int)(checked((IntPtr)(*(long*)(unchecked((byte*)ptr + 80)))))];
				*(long*)((byte*)ptr + 48) = (long)Class362.smethod_58(((byte*)ptr)[108], ((byte*)ptr)[110], ff_Encryption_0, ((byte*)ptr)[106], ((byte*)ptr)[104], ((byte*)ptr)[111], ((byte*)ptr)[109], ((byte*)ptr)[107], ((byte*)ptr)[105]);
				array = BitConverter.GetBytes(Class362.smethod_60(Class362.smethod_60((ulong)(*(long*)((byte*)ptr + 24)), ff_Encryption_0) + (ulong)(*(long*)((byte*)ptr + 48)), ff_Encryption_0));
				Array.Reverse(array);
				Array.Copy(array, 0, byte_0, 0, array.Length);
				*(long*)((byte*)ptr + 32) = (long)((Class362.smethod_100(ff_Encryption_0, (ulong)(*(long*)((byte*)ptr + 88)), 29) & 255UL) ^ 69UL);
				*(int*)((byte*)ptr + 100) = 0;
				while (*(int*)((byte*)ptr + 100) < 8)
				{
					*(long*)((byte*)ptr + 72) = (*(long*)((byte*)ptr + 32) & 255L);
					*(long*)((byte*)ptr + 32) = (long)((ulong)FF_Encryption.Struct0.byte_0[Class362.smethod_47(FF_Encryption.Struct0.int_0, ff_Encryption_0)] + (ulong)byte_1[*(int*)((byte*)ptr + 100)]);
					Class362.smethod_101(FF_Encryption.Struct0.byte_0[Class362.smethod_47(FF_Encryption.Struct0.int_0, ff_Encryption_0) + 1], (ulong)(*(long*)((byte*)ptr + 32)), ff_Encryption_0, ref *(ulong*)((byte*)ptr + 32));
					Class362.smethod_101(FF_Encryption.Struct0.byte_0[Class362.smethod_47(FF_Encryption.Struct0.int_0, ff_Encryption_0) + 2], (ulong)(*(long*)((byte*)ptr + 32)), ff_Encryption_0, ref *(ulong*)((byte*)ptr + 32));
					Class362.smethod_101(FF_Encryption.Struct0.byte_0[Class362.smethod_47(FF_Encryption.Struct0.int_0, ff_Encryption_0) + 3], (ulong)(*(long*)((byte*)ptr + 32)), ff_Encryption_0, ref *(ulong*)((byte*)ptr + 32));
					Class362.smethod_101(FF_Encryption.Struct0.byte_0[Class362.smethod_47(FF_Encryption.Struct0.int_0, ff_Encryption_0) + 4], (ulong)(*(long*)((byte*)ptr + 32)), ff_Encryption_0, ref *(ulong*)((byte*)ptr + 32));
					Class362.smethod_101(FF_Encryption.Struct0.byte_0[Class362.smethod_47(FF_Encryption.Struct0.int_0, ff_Encryption_0) + 5], (ulong)(*(long*)((byte*)ptr + 32)), ff_Encryption_0, ref *(ulong*)((byte*)ptr + 32));
					Class362.smethod_101(FF_Encryption.Struct0.byte_0[Class362.smethod_47(FF_Encryption.Struct0.int_0, ff_Encryption_0) + 6], (ulong)(*(long*)((byte*)ptr + 32)), ff_Encryption_0, ref *(ulong*)((byte*)ptr + 32));
					Class362.smethod_101(FF_Encryption.Struct0.byte_0[Class362.smethod_47(FF_Encryption.Struct0.int_0, ff_Encryption_0) + 7], (ulong)(*(long*)((byte*)ptr + 32)), ff_Encryption_0, ref *(ulong*)((byte*)ptr + 32));
					Class362.smethod_101(0, (ulong)(*(long*)((byte*)ptr + 32)), ff_Encryption_0, ref *(ulong*)((byte*)ptr + 32));
					*(long*)((byte*)ptr + 32) = (*(long*)((byte*)ptr + 32) ^ *(long*)((byte*)ptr + 72));
					byte_0[*(int*)((byte*)ptr + 100)] = (byte)(*(long*)((byte*)ptr + 32));
					*(int*)((byte*)ptr + 100) = *(int*)((byte*)ptr + 100) + 1;
				}
				*(int*)((byte*)ptr + 96) = *(int*)((byte*)ptr + 96) + 1;
			}
		}

		// Token: 0x060067AD RID: 26541 RVA: 0x00223940 File Offset: 0x00221B40
		static void smethod_67()
		{
			try
			{
				Class369.smethod_0();
			}
			catch (Exception)
			{
			}
		}

		// Token: 0x060067AE RID: 26542 RVA: 0x0004BEAD File Offset: 0x0004A0AD
		static void smethod_68(byte byte_0, FF_Encryption ff_Encryption_0, ref ulong ulong_0, ulong ulong_1)
		{
			ulong_0 = (ulong)((long)(ff_Encryption_0.byte_1[(int)(checked((IntPtr)(ulong_1 & 255UL)))] - byte_0));
		}

		// Token: 0x060067AF RID: 26543 RVA: 0x00223968 File Offset: 0x00221B68
		static void smethod_69(byte[] byte_0, Class371.Class373 class373_0, int int_0, int int_1)
		{
			if (class373_0.int_0 < class373_0.int_1)
			{
				throw new InvalidOperationException();
			}
			int num = int_1 + int_0;
			if (0 <= int_1 && int_1 <= num && num <= byte_0.Length)
			{
				if ((int_0 & 1) != 0)
				{
					class373_0.uint_0 |= (uint)((uint)(byte_0[int_1++] & byte.MaxValue) << class373_0.int_2);
					class373_0.int_2 += 8;
				}
				class373_0.byte_0 = byte_0;
				class373_0.int_0 = int_1;
				class373_0.int_1 = num;
				return;
			}
			throw new ArgumentOutOfRangeException();
		}

		// Token: 0x060067B0 RID: 26544 RVA: 0x002239F0 File Offset: 0x00221BF0
		unsafe static bool smethod_70(bool bool_0, Class4 class4_0)
		{
			void* ptr = stackalloc byte[8];
			*(int*)ptr = 0;
			while (*(int*)ptr < Class4.Struct4.list_0.Count)
			{
				byte[] array = Class362.smethod_37(class4_0, *(int*)ptr);
				*(int*)((byte*)ptr + 4) = (int)Class362.smethod_10(class4_0, Class4.Struct4.list_0[*(int*)ptr].string_0);
				if (!Class362.smethod_111(array, Class4.Struct6.list_0[*(int*)((byte*)ptr + 4)]))
				{
					if (!bool_0)
					{
						return false;
					}
					Class4.Struct6.list_0[*(int*)((byte*)ptr + 4)] = array;
				}
				*(int*)ptr = *(int*)ptr + 1;
			}
			return true;
		}

		// Token: 0x060067B1 RID: 26545 RVA: 0x00223A6C File Offset: 0x00221C6C
		static byte[] smethod_71(string string_0)
		{
			foreach (string text in Class362.smethod_103())
			{
				string text2 = text.Split(new char[]
				{
					'='
				})[0];
				if (text2.ToLower() == string_0.ToLower())
				{
					string string_ = text.Split(new char[]
					{
						'='
					})[1];
					return string_.smethod_2();
				}
			}
			return null;
		}

		// Token: 0x060067B2 RID: 26546 RVA: 0x00223AEC File Offset: 0x00221CEC
		static byte[] smethod_72(Class4 class4_0, int int_0)
		{
			if (int_0 > 3)
			{
				return null;
			}
			switch (int_0)
			{
			case 0:
				return Class362.smethod_71("savegame_param_sfo_key");
			case 1:
				return Class362.smethod_9(class4_0);
			case 2:
				return Class362.smethod_45(class4_0);
			case 3:
				return Class362.smethod_107(class4_0);
			default:
				return null;
			}
		}

		// Token: 0x060067B3 RID: 26547 RVA: 0x0004BEC7 File Offset: 0x0004A0C7
		static byte[] smethod_73(byte[] byte_0, Stream stream_0)
		{
			return new HMACSHA1(byte_0).ComputeHash(stream_0);
		}

		// Token: 0x060067B4 RID: 26548 RVA: 0x00223B38 File Offset: 0x00221D38
		static byte[] smethod_74(Class4 class4_0, Class4.Struct5 struct5_0, int int_0)
		{
			string a;
			if ((a = struct5_0.string_0.ToLower()) != null)
			{
				if (a == "param.sfo")
				{
					return Class362.smethod_72(class4_0, int_0);
				}
				if (a == "tropsys.dat")
				{
					return Class362.smethod_71("tropsys_dat_key");
				}
				if (a == "tropusr.dat")
				{
					return Class362.smethod_71("tropusr_dat_key");
				}
				if (a == "troptrns.dat")
				{
					return Class362.smethod_71("troptrns_dat_key");
				}
				if (a == "tropconf.sfm")
				{
					return Class362.smethod_71("tropconf_sfm_key");
				}
			}
			return Class362.smethod_93(class4_0, Class362.smethod_29(class4_0));
		}

		// Token: 0x060067B5 RID: 26549 RVA: 0x00223BD8 File Offset: 0x00221DD8
		static ResourceManager smethod_75()
		{
			if (object.ReferenceEquals(Class9.resourceManager_0, null))
			{
				ResourceManager resourceManager_ = new ResourceManager("ns8.Class9", typeof(Class9).Assembly);
				Class9.resourceManager_0 = resourceManager_;
			}
			return Class9.resourceManager_0;
		}

		// Token: 0x060067B6 RID: 26550 RVA: 0x00223C18 File Offset: 0x00221E18
		unsafe static byte[] smethod_76(byte[] byte_0, int int_0, byte[] byte_1)
		{
			void* ptr = stackalloc byte[12];
			Array.Resize<byte>(ref byte_1, 16);
			Aes aes = Aes.Create();
			aes.Key = byte_1;
			aes.BlockSize = 128;
			aes.Mode = CipherMode.ECB;
			aes.Padding = PaddingMode.Zeros;
			Aes aes2 = Aes.Create();
			aes2.Key = byte_1;
			aes.BlockSize = 128;
			aes2.Mode = CipherMode.ECB;
			aes2.Padding = PaddingMode.Zeros;
			*(int*)ptr = int_0 / 16;
			byte[] array = new byte[int_0];
			*(int*)((byte*)ptr + 4) = 0;
			while (*(int*)((byte*)ptr + 4) < *(int*)ptr)
			{
				byte[] array2 = new byte[16];
				Array.Copy(byte_0, *(int*)((byte*)ptr + 4) * 16, array2, 0, 16);
				byte[] array3 = new byte[16];
				Array.Copy(BitConverter.GetBytes(((ulong)((long)(*(int*)((byte*)ptr + 4)))).smethod_1()), 0, array3, 0, 8);
				array3 = aes.CreateEncryptor().TransformFinalBlock(array3, 0, array3.Length);
				*(int*)((byte*)ptr + 8) = 0;
				while (*(int*)((byte*)ptr + 8) < 16)
				{
					byte[] array4 = array2;
					int num = *(int*)((byte*)ptr + 8);
					array4[num] ^= array3[*(int*)((byte*)ptr + 8)];
					*(int*)((byte*)ptr + 8) = *(int*)((byte*)ptr + 8) + 1;
				}
				array2 = aes2.CreateEncryptor().TransformFinalBlock(array2, 0, array2.Length);
				Array.Copy(array2, 0, array, *(int*)((byte*)ptr + 4) * 16, 16);
				*(int*)((byte*)ptr + 4) = *(int*)((byte*)ptr + 4) + 1;
			}
			return array;
		}

		// Token: 0x060067B7 RID: 26551 RVA: 0x00223D5C File Offset: 0x00221F5C
		static void smethod_77(ulong ulong_0, ref ulong ulong_1, FF_Encryption ff_Encryption_0)
		{
			ulong_1 = ((ulong_0 >> 24 & 18446744073709486335UL) | (Class362.smethod_100(ff_Encryption_0, ulong_0, 24) & 65280UL));
			ulong_1 = ((ulong_1 & 18446744073692839935UL) | (Class362.smethod_100(ff_Encryption_0, ulong_0, 8) & 16711680UL));
			ulong_1 = ((ulong_1 & 18446744069431361535UL) | (Class362.smethod_100(ff_Encryption_0, ulong_0, 24) & 4278190080UL));
		}

		// Token: 0x060067B8 RID: 26552 RVA: 0x00223DD0 File Offset: 0x00221FD0
		unsafe static byte[] smethod_78(byte[] byte_0)
		{
			void* ptr = stackalloc byte[46];
			Assembly callingAssembly = Assembly.GetCallingAssembly();
			Assembly executingAssembly = Assembly.GetExecutingAssembly();
			if (callingAssembly != executingAssembly && !Class362.smethod_79(callingAssembly, executingAssembly))
			{
				return null;
			}
			Class371.Stream0 stream = new Class371.Stream0(byte_0);
			byte[] array = new byte[0];
			*(int*)ptr = Class362.smethod_94(stream);
			if (*(int*)ptr == 67324752)
			{
				*(short*)((byte*)ptr + 44) = (short)Class362.smethod_12(stream);
				*(int*)((byte*)ptr + 4) = Class362.smethod_12(stream);
				*(int*)((byte*)ptr + 8) = Class362.smethod_12(stream);
				if (*(int*)ptr == 67324752 && *(short*)((byte*)ptr + 44) == 20 && *(int*)((byte*)ptr + 4) == 0)
				{
					if (*(int*)((byte*)ptr + 8) == 8)
					{
						Class362.smethod_94(stream);
						Class362.smethod_94(stream);
						Class362.smethod_94(stream);
						*(int*)((byte*)ptr + 12) = Class362.smethod_94(stream);
						*(int*)((byte*)ptr + 16) = Class362.smethod_12(stream);
						*(int*)((byte*)ptr + 20) = Class362.smethod_12(stream);
						if (*(int*)((byte*)ptr + 16) > 0)
						{
							byte[] buffer = new byte[*(int*)((byte*)ptr + 16)];
							stream.Read(buffer, 0, *(int*)((byte*)ptr + 16));
						}
						if (*(int*)((byte*)ptr + 20) > 0)
						{
							byte[] buffer2 = new byte[*(int*)((byte*)ptr + 20)];
							stream.Read(buffer2, 0, *(int*)((byte*)ptr + 20));
						}
						byte[] array2 = new byte[stream.Length - stream.Position];
						stream.Read(array2, 0, array2.Length);
						Class371.Class372 class372_ = new Class371.Class372(array2);
						array = new byte[*(int*)((byte*)ptr + 12)];
						Class362.smethod_104(0, array, class372_, array.Length);
						goto IL_2EC;
					}
				}
				throw new FormatException("Wrong Header Signature");
			}
			*(int*)((byte*)ptr + 24) = *(int*)ptr >> 24;
			*(int*)ptr = *(int*)ptr - (*(int*)((byte*)ptr + 24) << 24);
			if (*(int*)ptr == 8223355)
			{
				if (*(int*)((byte*)ptr + 24) == 1)
				{
					*(int*)((byte*)ptr + 28) = Class362.smethod_94(stream);
					array = new byte[*(int*)((byte*)ptr + 28)];
					*(int*)((byte*)ptr + 32) = 0;
					while (*(int*)((byte*)ptr + 32) < *(int*)((byte*)ptr + 28))
					{
						*(int*)((byte*)ptr + 36) = Class362.smethod_94(stream);
						*(int*)((byte*)ptr + 40) = Class362.smethod_94(stream);
						byte[] array3 = new byte[*(int*)((byte*)ptr + 36)];
						stream.Read(array3, 0, array3.Length);
						Class371.Class372 class372_2 = new Class371.Class372(array3);
						Class362.smethod_104(*(int*)((byte*)ptr + 32), array, class372_2, *(int*)((byte*)ptr + 40));
						*(int*)((byte*)ptr + 32) = *(int*)((byte*)ptr + 32) + *(int*)((byte*)ptr + 40);
					}
				}
				if (*(int*)((byte*)ptr + 24) == 2)
				{
					byte[] byte_ = new byte[]
					{
						189,
						143,
						115,
						223,
						21,
						116,
						127,
						45
					};
					byte[] byte_2 = new byte[]
					{
						137,
						246,
						185,
						31,
						171,
						203,
						6,
						76
					};
					using (ICryptoTransform cryptoTransform = Class362.smethod_34(true, byte_2, byte_))
					{
						byte[] byte_3 = cryptoTransform.TransformFinalBlock(byte_0, 4, byte_0.Length - 4);
						array = Class362.smethod_78(byte_3);
					}
				}
				if (*(int*)((byte*)ptr + 24) != 3)
				{
					goto IL_2EC;
				}
				byte[] byte_4 = new byte[]
				{
					1,
					1,
					1,
					1,
					1,
					1,
					1,
					1,
					1,
					1,
					1,
					1,
					1,
					1,
					1,
					1
				};
				byte[] byte_5 = new byte[]
				{
					2,
					2,
					2,
					2,
					2,
					2,
					2,
					2,
					2,
					2,
					2,
					2,
					2,
					2,
					2,
					2
				};
				using (ICryptoTransform cryptoTransform2 = Class362.smethod_64(true, byte_4, byte_5))
				{
					byte[] byte_6 = cryptoTransform2.TransformFinalBlock(byte_0, 4, byte_0.Length - 4);
					array = Class362.smethod_78(byte_6);
					goto IL_2EC;
				}
			}
			throw new FormatException("Unknown Header");
			IL_2EC:
			stream.Close();
			stream = null;
			return array;
		}

		// Token: 0x060067B9 RID: 26553 RVA: 0x002240F0 File Offset: 0x002222F0
		static bool smethod_79(Assembly assembly_0, Assembly assembly_1)
		{
			byte[] publicKey = assembly_1.GetName().GetPublicKey();
			byte[] publicKey2 = assembly_0.GetName().GetPublicKey();
			if (publicKey2 == null != (publicKey == null))
			{
				return false;
			}
			if (publicKey2 != null)
			{
				for (int i = 0; i < publicKey2.Length; i++)
				{
					if (publicKey2[i] != publicKey[i])
					{
						return false;
					}
				}
			}
			return true;
		}

		// Token: 0x060067BA RID: 26554 RVA: 0x00224144 File Offset: 0x00222344
		static bool smethod_80(bool bool_0, string string_0, Class4 class4_0)
		{
			bool result;
			try
			{
				if (!File.Exists(string_0 + "\\PARAM.SFO"))
				{
					result = false;
				}
				else
				{
					Class362.smethod_52(class4_0, "Rebuilding Param.PFD..", Enum0.const_0);
					if (bool_0)
					{
						Class362.smethod_52(class4_0, "ReEncrypting Files..", Enum0.const_0);
						if (Class362.smethod_86(string_0, class4_0) == -1)
						{
							return false;
						}
					}
					Class362.smethod_52(class4_0, "Validating Param.PFD Hashes..", Enum0.const_0);
					if (!Class362.smethod_4(string_0, class4_0, true))
					{
						result = false;
					}
					else
					{
						Class362.smethod_52(class4_0, "Writing new Param.PFD..", Enum0.const_0);
						File.WriteAllBytes(string_0 + "\\PARAM.PFD", class4_0.method_5());
						Class362.smethod_52(class4_0, "Param.PFD Rebuilding complete! Rebuilded FilePath => " + string_0 + "\\PARAM.PFD", Enum0.const_0);
						result = true;
					}
				}
			}
			catch (Exception ex)
			{
				Class362.smethod_52(class4_0, ex.Message, Enum0.const_2);
				result = false;
			}
			return result;
		}

		// Token: 0x060067BB RID: 26555 RVA: 0x0004BED5 File Offset: 0x0004A0D5
		static int smethod_81(Class371.Class373 class373_0)
		{
			return class373_0.int_1 - class373_0.int_0 + (class373_0.int_2 >> 3);
		}

		// Token: 0x060067BC RID: 26556 RVA: 0x00224210 File Offset: 0x00222410
		static void smethod_82(ulong ulong_0, ulong ulong_1, ref ulong ulong_2, ulong ulong_3, ref ulong ulong_4, ref byte byte_0, FF_Encryption ff_Encryption_0, ulong ulong_5)
		{
			ulong_3 = (((ulong_1 + 212UL ^ ulong_5) & 255UL) ^ 69UL);
			byte_0 = (byte)ulong_3;
			ulong_2 = ulong_0 + ulong_3;
			ulong_4 = (Class362.smethod_100(ff_Encryption_0, ulong_3, 2) & 1020UL);
		}

		// Token: 0x060067BD RID: 26557 RVA: 0x00224264 File Offset: 0x00222464
		unsafe static int smethod_83(byte[] byte_0, int int_0, Class371.Class374 class374_0, int int_1)
		{
			void* ptr = stackalloc byte[12];
			*(int*)ptr = class374_0.int_0;
			if (int_1 > class374_0.int_1)
			{
				int_1 = class374_0.int_1;
			}
			else
			{
				*(int*)ptr = (class374_0.int_0 - class374_0.int_1 + int_1 & 32767);
			}
			*(int*)((byte*)ptr + 4) = int_1;
			*(int*)((byte*)ptr + 8) = int_1 - *(int*)ptr;
			if (*(int*)((byte*)ptr + 8) > 0)
			{
				Array.Copy(class374_0.byte_0, 32768 - *(int*)((byte*)ptr + 8), byte_0, int_0, *(int*)((byte*)ptr + 8));
				int_0 += *(int*)((byte*)ptr + 8);
				int_1 = *(int*)ptr;
			}
			Array.Copy(class374_0.byte_0, *(int*)ptr - int_1, byte_0, int_0, int_1);
			class374_0.int_1 -= *(int*)((byte*)ptr + 4);
			if (class374_0.int_1 < 0)
			{
				throw new InvalidOperationException();
			}
			return *(int*)((byte*)ptr + 4);
		}

		// Token: 0x060067BE RID: 26558 RVA: 0x0022431C File Offset: 0x0022251C
		static byte[] smethod_84(Class4 class4_0, string string_0)
		{
			if (!File.Exists(string_0))
			{
				return null;
			}
			string name = new FileInfo(string_0).Name;
			FileStream fileStream = File.Open(string_0, FileMode.Open, FileAccess.ReadWrite, FileShare.None);
			byte[] array = class4_0.method_3(fileStream, name);
			fileStream.Close();
			fileStream.Dispose();
			if (array != null)
			{
				return array;
			}
			return File.ReadAllBytes(string_0);
		}

		// Token: 0x060067BF RID: 26559 RVA: 0x0022436C File Offset: 0x0022256C
		unsafe static void smethod_85(byte[] byte_0, Class371.Class375 class375_0)
		{
			void* ptr = stackalloc byte[68];
			int[] array = new int[16];
			int[] array2 = new int[16];
			*(int*)ptr = 0;
			while (*(int*)ptr < byte_0.Length)
			{
				*(int*)((byte*)ptr + 4) = (int)byte_0[*(int*)ptr];
				if (*(int*)((byte*)ptr + 4) > 0)
				{
					array[*(int*)((byte*)ptr + 4)]++;
				}
				*(int*)ptr = *(int*)ptr + 1;
			}
			*(int*)((byte*)ptr + 8) = 0;
			*(int*)((byte*)ptr + 12) = 512;
			*(int*)((byte*)ptr + 16) = 1;
			while (*(int*)((byte*)ptr + 16) <= 15)
			{
				array2[*(int*)((byte*)ptr + 16)] = *(int*)((byte*)ptr + 8);
				*(int*)((byte*)ptr + 8) = *(int*)((byte*)ptr + 8) + (array[*(int*)((byte*)ptr + 16)] << 16 - *(int*)((byte*)ptr + 16));
				if (*(int*)((byte*)ptr + 16) >= 10)
				{
					*(int*)((byte*)ptr + 20) = (array2[*(int*)((byte*)ptr + 16)] & 130944);
					*(int*)((byte*)ptr + 24) = (*(int*)((byte*)ptr + 8) & 130944);
					*(int*)((byte*)ptr + 12) = *(int*)((byte*)ptr + 12) + (*(int*)((byte*)ptr + 24) - *(int*)((byte*)ptr + 20) >> 16 - *(int*)((byte*)ptr + 16));
				}
				*(int*)((byte*)ptr + 16) = *(int*)((byte*)ptr + 16) + 1;
			}
			class375_0.short_0 = new short[*(int*)((byte*)ptr + 12)];
			*(int*)((byte*)ptr + 28) = 512;
			*(int*)((byte*)ptr + 32) = 15;
			while (*(int*)((byte*)ptr + 32) >= 10)
			{
				*(int*)((byte*)ptr + 36) = (*(int*)((byte*)ptr + 8) & 130944);
				*(int*)((byte*)ptr + 8) = *(int*)((byte*)ptr + 8) - (array[*(int*)((byte*)ptr + 32)] << 16 - *(int*)((byte*)ptr + 32));
				*(int*)((byte*)ptr + 40) = (*(int*)((byte*)ptr + 8) & 130944);
				*(int*)((byte*)ptr + 44) = *(int*)((byte*)ptr + 40);
				while (*(int*)((byte*)ptr + 44) < *(int*)((byte*)ptr + 36))
				{
					class375_0.short_0[(int)Class362.smethod_61(*(int*)((byte*)ptr + 44))] = (short)(-(*(int*)((byte*)ptr + 28)) << 4 | *(int*)((byte*)ptr + 32));
					*(int*)((byte*)ptr + 28) = *(int*)((byte*)ptr + 28) + (1 << *(int*)((byte*)ptr + 32) - 9);
					*(int*)((byte*)ptr + 44) = *(int*)((byte*)ptr + 44) + 128;
				}
				*(int*)((byte*)ptr + 32) = *(int*)((byte*)ptr + 32) - 1;
			}
			*(int*)((byte*)ptr + 48) = 0;
			while (*(int*)((byte*)ptr + 48) < byte_0.Length)
			{
				*(int*)((byte*)ptr + 52) = (int)byte_0[*(int*)((byte*)ptr + 48)];
				if (*(int*)((byte*)ptr + 52) != 0)
				{
					*(int*)((byte*)ptr + 8) = array2[*(int*)((byte*)ptr + 52)];
					*(int*)((byte*)ptr + 56) = (int)Class362.smethod_61(*(int*)((byte*)ptr + 8));
					if (*(int*)((byte*)ptr + 52) <= 9)
					{
						do
						{
							class375_0.short_0[*(int*)((byte*)ptr + 56)] = (short)(*(int*)((byte*)ptr + 48) << 4 | *(int*)((byte*)ptr + 52));
							*(int*)((byte*)ptr + 56) = *(int*)((byte*)ptr + 56) + (1 << *(int*)((byte*)ptr + 52));
						}
						while (*(int*)((byte*)ptr + 56) < 512);
					}
					else
					{
						*(int*)((byte*)ptr + 60) = (int)class375_0.short_0[*(int*)((byte*)ptr + 56) & 511];
						*(int*)((byte*)ptr + 64) = 1 << (*(int*)((byte*)ptr + 60) & 15);
						*(int*)((byte*)ptr + 60) = -(*(int*)((byte*)ptr + 60) >> 4);
						do
						{
							class375_0.short_0[*(int*)((byte*)ptr + 60) | *(int*)((byte*)ptr + 56) >> 9] = (short)(*(int*)((byte*)ptr + 48) << 4 | *(int*)((byte*)ptr + 52));
							*(int*)((byte*)ptr + 56) = *(int*)((byte*)ptr + 56) + (1 << *(int*)((byte*)ptr + 52));
						}
						while (*(int*)((byte*)ptr + 56) < *(int*)((byte*)ptr + 64));
					}
					array2[*(int*)((byte*)ptr + 52)] = *(int*)((byte*)ptr + 8) + (1 << 16 - *(int*)((byte*)ptr + 52));
				}
				*(int*)((byte*)ptr + 48) = *(int*)((byte*)ptr + 48) + 1;
			}
		}

		// Token: 0x060067C0 RID: 26560 RVA: 0x00224670 File Offset: 0x00222870
		unsafe static int smethod_86(string string_0, Class4 class4_0)
		{
			void* ptr = stackalloc byte[12];
			try
			{
				*(int*)ptr = 0;
				*(int*)((byte*)ptr + 4) = 0;
				while (*(int*)((byte*)ptr + 4) < Class4.Struct4.list_0.Count)
				{
					Class4.Struct5 @struct = Class4.Struct4.list_0[*(int*)((byte*)ptr + 4)];
					if (!(@struct.string_0.ToLower() == "param.sfo"))
					{
						string text = string_0 + "\\" + @struct.string_0;
						if (File.Exists(text) && !class4_0.method_2(text, false) && Class362.smethod_55(class4_0, text))
						{
							if (!class4_0.method_2(text, true))
							{
								*(int*)((byte*)ptr + 8) = -1;
								goto IL_A6;
							}
							*(int*)ptr = *(int*)ptr + 1;
						}
					}
					*(int*)((byte*)ptr + 4) = *(int*)((byte*)ptr + 4) + 1;
				}
				*(int*)((byte*)ptr + 8) = *(int*)ptr;
			}
			catch
			{
				*(int*)((byte*)ptr + 8) = -1;
			}
			IL_A6:
			return *(int*)((byte*)ptr + 8);
		}

		// Token: 0x060067C1 RID: 26561 RVA: 0x00224738 File Offset: 0x00222938
		static byte[] smethod_87(byte[] byte_0, FF_Encryption ff_Encryption_0)
		{
			int num = (ff_Encryption_0.ffgame_0 == FF_Encryption.FFGame.FFXIII) ? 264 : 272;
			if (ff_Encryption_0.bool_0)
			{
				num = 0;
			}
			int num2 = byte_0.Length - (num + 8);
			byte[] array = new byte[num2];
			Array.Copy(byte_0, num, array, 0, num2);
			Class362.smethod_99(num2, array, ff_Encryption_0, array);
			byte[] array2 = Class362.smethod_44(ff_Encryption_0);
			Class362.smethod_66(array2, array2.Length, ff_Encryption_0, array2);
			byte[] array3 = new byte[byte_0.Length];
			Array.Copy(byte_0, 0, array3, 0, num);
			Array.Copy(array, 0, array3, num, array.Length);
			Array.Copy(array2, 0, array3, num + array.Length, array2.Length);
			if (ff_Encryption_0.bool_0 && ff_Encryption_0.ffgame_0 == FF_Encryption.FFGame.FFXIII_3 && byte_0.Length == 547904)
			{
				Array.Resize<byte>(ref array3, 560736);
			}
			return array3;
		}

		// Token: 0x060067C2 RID: 26562 RVA: 0x002247F8 File Offset: 0x002229F8
		static void smethod_88(Class371.Class374 class374_0, int int_0)
		{
			if (class374_0.int_1++ == 32768)
			{
				throw new InvalidOperationException();
			}
			class374_0.byte_0[class374_0.int_0++] = (byte)int_0;
			class374_0.int_0 &= 32767;
		}

		// Token: 0x060067C3 RID: 26563 RVA: 0x0004BEED File Offset: 0x0004A0ED
		static int smethod_89(Class371.Class373 class373_0)
		{
			return class373_0.int_2;
		}

		// Token: 0x060067C4 RID: 26564 RVA: 0x00224850 File Offset: 0x00222A50
		unsafe static int smethod_90(Class371.Class373 class373_0, byte[] byte_0, int int_0, int int_1)
		{
			void* ptr = stackalloc byte[8];
			*(int*)ptr = 0;
			while (class373_0.int_2 > 0 && int_1 > 0)
			{
				byte_0[int_0++] = (byte)class373_0.uint_0;
				class373_0.uint_0 >>= 8;
				class373_0.int_2 -= 8;
				int_1--;
				*(int*)ptr = *(int*)ptr + 1;
			}
			if (int_1 == 0)
			{
				return *(int*)ptr;
			}
			*(int*)((byte*)ptr + 4) = class373_0.int_1 - class373_0.int_0;
			if (int_1 > *(int*)((byte*)ptr + 4))
			{
				int_1 = *(int*)((byte*)ptr + 4);
			}
			Array.Copy(class373_0.byte_0, class373_0.int_0, byte_0, int_0, int_1);
			class373_0.int_0 += int_1;
			if ((class373_0.int_0 - class373_0.int_1 & 1) != 0)
			{
				class373_0.uint_0 = (uint)(class373_0.byte_0[class373_0.int_0++] & byte.MaxValue);
				class373_0.int_2 = 8;
			}
			return *(int*)ptr + int_1;
		}

		// Token: 0x060067C5 RID: 26565 RVA: 0x00224930 File Offset: 0x00222B30
		static Class8[] smethod_91()
		{
			Class8[] result;
			try
			{
				string text = new WebClient().DownloadString("http://ps3tools.aldostools.org/games.conf");
				if (text != null && text.Length >= 100)
				{
					result = Class362.smethod_59(text);
				}
				else
				{
					result = new Class8[0];
				}
			}
			catch
			{
				result = new Class8[0];
			}
			return result;
		}

		// Token: 0x060067C6 RID: 26566 RVA: 0x00224988 File Offset: 0x00222B88
		unsafe static ulong smethod_92(FF_Encryption ff_Encryption_0)
		{
			void* ptr = stackalloc byte[16];
			OpenFileDialog openFileDialog = new OpenFileDialog();
			openFileDialog.Filter = "KEY.DAT File|*KEY.DAT";
			if (openFileDialog.ShowDialog() == DialogResult.OK)
			{
				byte[] value = File.ReadAllBytes(openFileDialog.FileName);
				*(long*)ptr = (long)Class362.smethod_60(BitConverter.ToUInt64(value, 0), ff_Encryption_0);
				*(long*)((byte*)ptr + 8) = (long)Class362.smethod_60(BitConverter.ToUInt64(value, 8), ff_Encryption_0);
				ulong num = (ulong)(*(long*)ptr ^ *(long*)((byte*)ptr + 8));
				num |= 1UL;
				return num ^ ((ff_Encryption_0.ffgame_0 == FF_Encryption.FFGame.FFXIII_2) ? 11177653904406231216UL : 3914857797994049631UL);
			}
			return 0UL;
		}

		// Token: 0x060067C7 RID: 26567 RVA: 0x00224A20 File Offset: 0x00222C20
		unsafe static byte[] smethod_93(Class4 class4_0, byte[] byte_0)
		{
			void* ptr = stackalloc byte[8];
			if (byte_0.Length != 16)
			{
				throw new Exception("SecureFileID must be 16 bytes in length");
			}
			byte[] array = new byte[20];
			Array.Copy(byte_0, 0, array, 0, 16);
			*(int*)ptr = 0;
			int num = 0;
			while (*(int*)ptr < array.Length)
			{
				*(int*)((byte*)ptr + 4) = *(int*)ptr;
				switch (*(int*)((byte*)ptr + 4))
				{
				case 1:
					array[*(int*)ptr] = 11;
					break;
				case 2:
					array[*(int*)ptr] = 15;
					break;
				case 3:
				case 4:
					goto IL_77;
				case 5:
					array[*(int*)ptr] = 14;
					break;
				default:
					if (*(int*)((byte*)ptr + 4) != 8)
					{
						goto IL_77;
					}
					array[*(int*)ptr] = 10;
					break;
				}
				IL_8A:
				*(int*)ptr = *(int*)ptr + 1;
				continue;
				IL_77:
				array[*(int*)ptr] = byte_0[num++];
				goto IL_8A;
			}
			return array;
		}

		// Token: 0x060067C8 RID: 26568 RVA: 0x0004BEF5 File Offset: 0x0004A0F5
		static int smethod_94(Class371.Stream0 stream0_0)
		{
			return Class362.smethod_12(stream0_0) | Class362.smethod_12(stream0_0) << 16;
		}

		// Token: 0x060067C9 RID: 26569 RVA: 0x0004BF07 File Offset: 0x0004A107
		static void smethod_95(Class4 class4_0, byte[] byte_0)
		{
			if (byte_0.Length != 16)
			{
				throw new Exception("SecureFileID must nbe 16 bytes in length");
			}
			Class4.byte_8 = byte_0;
		}

		// Token: 0x060067CA RID: 26570 RVA: 0x00224AC8 File Offset: 0x00222CC8
		unsafe static Assembly smethod_96(object object_0, ResolveEventArgs resolveEventArgs_0)
		{
			void* ptr = stackalloc byte[18];
			Class366.Struct43 @struct = new Class366.Struct43(resolveEventArgs_0.Name);
			string s = @struct.method_0(false);
			string b = Convert.ToBase64String(Encoding.UTF8.GetBytes(s));
			string[] array = "ezgxOWNkYmY4LTE4ODQtNDljZS1hM2Y3LTVlYTRkOTgyMGI0Mn0sIEN1bHR1cmU9bmV1dHJhbCwgUHVibGljS2V5VG9rZW49M2U1NjM1MDY5M2Y3MzU1ZQ==,[z]{f236d73a-3d29-43b4-9913-e7ecf8a411f2},ezgxOWNkYmY4LTE4ODQtNDljZS1hM2Y3LTVlYTRkOTgyMGI0Mn0=,[z]{f236d73a-3d29-43b4-9913-e7ecf8a411f2}".Split(new char[]
			{
				','
			});
			string text = string.Empty;
			((byte*)ptr)[16] = 0;
			((byte*)ptr)[17] = 0;
			*(int*)ptr = 0;
			while (*(int*)ptr < array.Length - 1)
			{
				if (array[*(int*)ptr] == b)
				{
					text = array[*(int*)ptr + 1];
					IL_89:
					if (text.Length == 0 && @struct.string_2.Length == 0)
					{
						b = Convert.ToBase64String(Encoding.UTF8.GetBytes(@struct.string_0));
						*(int*)((byte*)ptr + 4) = 0;
						while (*(int*)((byte*)ptr + 4) < array.Length - 1)
						{
							if (array[*(int*)((byte*)ptr + 4)] == b)
							{
								text = array[*(int*)((byte*)ptr + 4) + 1];
								break;
							}
							*(int*)((byte*)ptr + 4) = *(int*)((byte*)ptr + 4) + 2;
						}
					}
					if (text.Length > 0)
					{
						if (text[0] == '[')
						{
							*(int*)((byte*)ptr + 8) = text.IndexOf(']');
							string text2 = text.Substring(1, *(int*)((byte*)ptr + 8) - 1);
							((byte*)ptr)[16] = ((text2.IndexOf('z') >= 0) ? 1 : 0);
							((byte*)ptr)[17] = ((text2.IndexOf('t') >= 0) ? 1 : 0);
							text = text.Substring(*(int*)((byte*)ptr + 8) + 1);
						}
						Assembly result;
						lock (Class366.dictionary_0)
						{
							if (Class366.dictionary_0.ContainsKey(text))
							{
								result = Class366.dictionary_0[text];
							}
							else
							{
								Stream manifestResourceStream = Assembly.GetExecutingAssembly().GetManifestResourceStream(text);
								if (manifestResourceStream == null)
								{
									goto IL_2A1;
								}
								*(int*)((byte*)ptr + 12) = (int)manifestResourceStream.Length;
								byte[] array2 = new byte[*(int*)((byte*)ptr + 12)];
								manifestResourceStream.Read(array2, 0, *(int*)((byte*)ptr + 12));
								if (*(sbyte*)((byte*)ptr + 16) != 0)
								{
									array2 = Class362.smethod_78(array2);
								}
								Assembly assembly = null;
								if (*(sbyte*)((byte*)ptr + 17) == 0)
								{
									try
									{
										assembly = Assembly.Load(array2);
									}
									catch (FileLoadException)
									{
										((byte*)ptr)[17] = 1;
									}
									catch (BadImageFormatException)
									{
										((byte*)ptr)[17] = 1;
									}
								}
								if (*(sbyte*)((byte*)ptr + 17) != 0)
								{
									try
									{
										string text3 = string.Format("{0}{1}\\", Path.GetTempPath(), text);
										Directory.CreateDirectory(text3);
										string text4 = text3 + @struct.string_0 + ".dll";
										if (!File.Exists(text4))
										{
											FileStream fileStream = File.OpenWrite(text4);
											fileStream.Write(array2, 0, array2.Length);
											fileStream.Close();
											Class362.MoveFileEx(text4, null, 4);
											Class362.MoveFileEx(text3, null, 4);
										}
										assembly = Assembly.LoadFile(text4);
									}
									catch
									{
									}
								}
								Class366.dictionary_0[text] = assembly;
								result = assembly;
							}
						}
						return result;
					}
					IL_2A1:
					return null;
				}
				*(int*)ptr = *(int*)ptr + 2;
			}
			goto IL_89;
		}

		// Token: 0x060067CB RID: 26571 RVA: 0x00224DDC File Offset: 0x00222FDC
		unsafe static byte[] smethod_97(byte[] byte_0, FF_Encryption ff_Encryption_0)
		{
			void* ptr = stackalloc byte[8];
			int num = (ff_Encryption_0.ffgame_0 == FF_Encryption.FFGame.FFXIII) ? 264 : 272;
			if (ff_Encryption_0.bool_0)
			{
				num = 0;
			}
			*(int*)ptr = byte_0.Length;
			if (ff_Encryption_0.bool_0 && ff_Encryption_0.ffgame_0 == FF_Encryption.FFGame.FFXIII_3 && byte_0.Length > 547904)
			{
				*(int*)ptr = 547904;
			}
			*(int*)ptr = *(int*)ptr - (num + 8);
			byte[] array = new byte[0];
			if (!ff_Encryption_0.bool_0)
			{
				array = new byte[num];
				Array.Copy(byte_0, 0, array, 0, num);
			}
			byte[] array2 = new byte[*(int*)ptr];
			byte[] array3 = new byte[*(int*)ptr];
			byte[] array4 = new byte[8];
			Array.Copy(byte_0, num, array3, 0, *(int*)ptr);
			Array.Copy(byte_0, num + *(int*)ptr, array4, 0, 8);
			Class362.smethod_38(array3, *(int*)ptr, ref array2, ff_Encryption_0);
			Class362.smethod_20(ref array4, array4, ff_Encryption_0, 8);
			*(int*)((byte*)ptr + 4) = (int)BitConverter.ToUInt32(array4, 4);
			if (*(uint*)((byte*)ptr + 4) != FF_Encryption.Struct0.uint_0)
			{
				throw new Exception("Decrypted data did not pass file integrity check.");
			}
			array3 = new byte[byte_0.Length];
			if (!ff_Encryption_0.bool_0)
			{
				Array.Copy(array, 0, array3, 0, array.Length);
			}
			Array.Copy(array2, 0, array3, array.Length, array2.Length);
			Array.Copy(array4, 0, array3, array.Length + *(int*)ptr, array4.Length);
			array2 = null;
			array4 = null;
			return array3;
		}

		// Token: 0x060067CC RID: 26572 RVA: 0x00224F14 File Offset: 0x00223114
		static int smethod_98(Class371.Class373 class373_0, int int_0)
		{
			if (class373_0.int_2 < int_0)
			{
				if (class373_0.int_0 == class373_0.int_1)
				{
					return -1;
				}
				class373_0.uint_0 |= (uint)((uint)((int)(class373_0.byte_0[class373_0.int_0++] & byte.MaxValue) | (int)(class373_0.byte_0[class373_0.int_0++] & byte.MaxValue) << 8) << class373_0.int_2);
				class373_0.int_2 += 16;
			}
			return (int)((ulong)class373_0.uint_0 & (ulong)((long)((1 << int_0) - 1)));
		}

		// Token: 0x060067CD RID: 26573 RVA: 0x00224FB4 File Offset: 0x002231B4
		unsafe static void smethod_99(int int_0, byte[] byte_0, FF_Encryption ff_Encryption_0, byte[] byte_1)
		{
			void* ptr = stackalloc byte[112];
			Class362.smethod_24(ff_Encryption_0);
			*(long*)ptr = -1024L;
			*(long*)((byte*)ptr + 8) = -4294967296L;
			*(long*)((byte*)ptr + 16) = 4294967295L;
			*(int*)((byte*)ptr + 96) = 0;
			while (*(int*)((byte*)ptr + 96) < int_0 >> 3)
			{
				FF_Encryption.Struct0.uint_0 = (uint)byte_1[4 + (*(int*)((byte*)ptr + 96) << 3)] + ((uint)byte_1[*(int*)((byte*)ptr + 96) << 3] + FF_Encryption.Struct0.uint_0);
				FF_Encryption.Struct0.int_0 = *(int*)((byte*)ptr + 96) << 3;
				byte[] array = new byte[8];
				Array.Copy(byte_0, FF_Encryption.Struct0.int_0, array, 0, 8);
				*(long*)((byte*)ptr + 64) = (long)Class362.smethod_28(ff_Encryption_0, ref array);
				*(long*)((byte*)ptr + 88) = (long)FF_Encryption.Struct0.int_0;
				*(long*)((byte*)ptr + 40) = (long)((Class362.smethod_32(10, ff_Encryption_0, Class362.smethod_32(10, ff_Encryption_0, (ulong)(*(long*)((byte*)ptr + 88))) | (ulong)(*(long*)((byte*)ptr + 88))) & (ulong)(*(long*)ptr)) | (ulong)(*(long*)((byte*)ptr + 88)));
				*(long*)((byte*)ptr + 56) = (long)(2707759943UL + ((Class362.smethod_32(10, ff_Encryption_0, (ulong)(*(long*)((byte*)ptr + 40))) & (ulong)(*(long*)ptr)) | (ulong)(*(long*)((byte*)ptr + 88))));
				*(long*)((byte*)ptr + 32) = 0L;
				Class362.smethod_77((ulong)(*(long*)((byte*)ptr + 56)) >> 32, ref *(ulong*)((byte*)ptr + 32), ff_Encryption_0);
				Class362.smethod_77((ulong)(*(long*)((byte*)ptr + 56) & *(long*)((byte*)ptr + 16)), ref *(ulong*)((byte*)ptr + 40), ff_Encryption_0);
				array = new byte[8];
				Array.Copy(FF_Encryption.Struct0.byte_0, Class362.smethod_47(FF_Encryption.Struct0.int_0, ff_Encryption_0), array, 0, 8);
				*(long*)((byte*)ptr + 24) = (((*(long*)((byte*)ptr + 32) & *(long*)((byte*)ptr + 16) & ~(*(long*)((byte*)ptr + 8))) | (long)(Class362.smethod_32(32, ff_Encryption_0, (ulong)(*(long*)((byte*)ptr + 40))) & (ulong)(*(long*)((byte*)ptr + 8)))) ^ (long)Class362.smethod_28(ff_Encryption_0, ref array) ^ (long)((Class362.smethod_32(32, ff_Encryption_0, (ulong)(*(long*)((byte*)ptr + 64))) & (ulong)(*(long*)((byte*)ptr + 16))) | (Class362.smethod_32(32, ff_Encryption_0, (ulong)(*(long*)((byte*)ptr + 64))) & (ulong)(*(long*)((byte*)ptr + 8)))));
				*(long*)((byte*)ptr + 80) = (long)Class362.smethod_47(FF_Encryption.Struct0.int_0, ff_Encryption_0);
				((byte*)ptr)[104] = FF_Encryption.Struct0.byte_0[(int)(checked((IntPtr)(unchecked(*(long*)((byte*)ptr + 80) + 7L))))];
				((byte*)ptr)[105] = FF_Encryption.Struct0.byte_0[(int)(checked((IntPtr)(unchecked(*(long*)((byte*)ptr + 80) + 6L))))];
				((byte*)ptr)[106] = FF_Encryption.Struct0.byte_0[(int)(checked((IntPtr)(unchecked(*(long*)((byte*)ptr + 80) + 5L))))];
				((byte*)ptr)[107] = FF_Encryption.Struct0.byte_0[(int)(checked((IntPtr)(unchecked(*(long*)((byte*)ptr + 80) + 4L))))];
				((byte*)ptr)[108] = FF_Encryption.Struct0.byte_0[(int)(checked((IntPtr)(unchecked(*(long*)((byte*)ptr + 80) + 3L))))];
				((byte*)ptr)[109] = FF_Encryption.Struct0.byte_0[(int)(checked((IntPtr)(unchecked(*(long*)((byte*)ptr + 80) + 2L))))];
				((byte*)ptr)[110] = FF_Encryption.Struct0.byte_0[(int)(checked((IntPtr)(unchecked(*(long*)((byte*)ptr + 80) + 1L))))];
				((byte*)ptr)[111] = FF_Encryption.Struct0.byte_0[(int)(checked((IntPtr)(*(long*)(unchecked((byte*)ptr + 80)))))];
				*(long*)((byte*)ptr + 48) = (long)Class362.smethod_58(((byte*)ptr)[108], ((byte*)ptr)[110], ff_Encryption_0, ((byte*)ptr)[106], ((byte*)ptr)[104], ((byte*)ptr)[111], ((byte*)ptr)[109], ((byte*)ptr)[107], ((byte*)ptr)[105]);
				array = BitConverter.GetBytes(Class362.smethod_60(Class362.smethod_60((ulong)(*(long*)((byte*)ptr + 24)), ff_Encryption_0) + (ulong)(*(long*)((byte*)ptr + 48)), ff_Encryption_0));
				Array.Reverse(array);
				Array.Copy(array, 0, byte_1, FF_Encryption.Struct0.int_0, array.Length);
				*(long*)((byte*)ptr + 32) = (long)((Class362.smethod_100(ff_Encryption_0, (ulong)(*(long*)((byte*)ptr + 88)), 29) & 255UL) ^ 69UL);
				*(int*)((byte*)ptr + 100) = 0;
				while (*(int*)((byte*)ptr + 100) < 8)
				{
					*(long*)((byte*)ptr + 72) = (*(long*)((byte*)ptr + 32) & 255L);
					*(long*)((byte*)ptr + 32) = (long)((ulong)FF_Encryption.Struct0.byte_0[Class362.smethod_47(FF_Encryption.Struct0.int_0, ff_Encryption_0)] + (ulong)byte_0[FF_Encryption.Struct0.int_0 + *(int*)((byte*)ptr + 100)]);
					Class362.smethod_101(FF_Encryption.Struct0.byte_0[Class362.smethod_47(FF_Encryption.Struct0.int_0, ff_Encryption_0) + 1], (ulong)(*(long*)((byte*)ptr + 32)), ff_Encryption_0, ref *(ulong*)((byte*)ptr + 32));
					Class362.smethod_101(FF_Encryption.Struct0.byte_0[Class362.smethod_47(FF_Encryption.Struct0.int_0, ff_Encryption_0) + 2], (ulong)(*(long*)((byte*)ptr + 32)), ff_Encryption_0, ref *(ulong*)((byte*)ptr + 32));
					Class362.smethod_101(FF_Encryption.Struct0.byte_0[Class362.smethod_47(FF_Encryption.Struct0.int_0, ff_Encryption_0) + 3], (ulong)(*(long*)((byte*)ptr + 32)), ff_Encryption_0, ref *(ulong*)((byte*)ptr + 32));
					Class362.smethod_101(FF_Encryption.Struct0.byte_0[Class362.smethod_47(FF_Encryption.Struct0.int_0, ff_Encryption_0) + 4], (ulong)(*(long*)((byte*)ptr + 32)), ff_Encryption_0, ref *(ulong*)((byte*)ptr + 32));
					Class362.smethod_101(FF_Encryption.Struct0.byte_0[Class362.smethod_47(FF_Encryption.Struct0.int_0, ff_Encryption_0) + 5], (ulong)(*(long*)((byte*)ptr + 32)), ff_Encryption_0, ref *(ulong*)((byte*)ptr + 32));
					Class362.smethod_101(FF_Encryption.Struct0.byte_0[Class362.smethod_47(FF_Encryption.Struct0.int_0, ff_Encryption_0) + 6], (ulong)(*(long*)((byte*)ptr + 32)), ff_Encryption_0, ref *(ulong*)((byte*)ptr + 32));
					Class362.smethod_101(FF_Encryption.Struct0.byte_0[Class362.smethod_47(FF_Encryption.Struct0.int_0, ff_Encryption_0) + 7], (ulong)(*(long*)((byte*)ptr + 32)), ff_Encryption_0, ref *(ulong*)((byte*)ptr + 32));
					Class362.smethod_101(0, (ulong)(*(long*)((byte*)ptr + 32)), ff_Encryption_0, ref *(ulong*)((byte*)ptr + 32));
					*(long*)((byte*)ptr + 32) = (*(long*)((byte*)ptr + 32) ^ *(long*)((byte*)ptr + 72));
					byte_1[FF_Encryption.Struct0.int_0 + *(int*)((byte*)ptr + 100)] = (byte)(*(long*)((byte*)ptr + 32));
					*(int*)((byte*)ptr + 100) = *(int*)((byte*)ptr + 100) + 1;
				}
				*(int*)((byte*)ptr + 96) = *(int*)((byte*)ptr + 96) + 1;
			}
			FF_Encryption.Struct0.int_0 += 8;
		}

		// Token: 0x060067CE RID: 26574 RVA: 0x0004BF21 File Offset: 0x0004A121
		static ulong smethod_100(FF_Encryption ff_Encryption_0, ulong ulong_0, int int_0)
		{
			return ulong_0 << int_0 | ulong_0 >> 32 - int_0;
		}

		// Token: 0x060067CF RID: 26575 RVA: 0x0004BF33 File Offset: 0x0004A133
		static void smethod_101(byte byte_0, ulong ulong_0, FF_Encryption ff_Encryption_0, ref ulong ulong_1)
		{
			ulong_1 = (ulong)((long)((ff_Encryption_0.byte_2[(int)(checked((IntPtr)(ulong_0 & 255UL)))] & byte.MaxValue) + byte_0));
		}

		// Token: 0x060067D0 RID: 26576 RVA: 0x00225460 File Offset: 0x00223660
		static ulong smethod_102(FF_Encryption.FFGame ffgame_0, FF_Encryption ff_Encryption_0, bool bool_0)
		{
			switch (ffgame_0)
			{
			case FF_Encryption.FFGame.FFXIII:
				if (bool_0)
				{
					return 1375844230373843072UL;
				}
				return 242319337685132394UL;
			case FF_Encryption.FFGame.FFXIII_2:
				if (bool_0)
				{
					if (ff_Encryption_0.byte_0 != null)
					{
						if (ff_Encryption_0.byte_0.Length == 16)
						{
							return Class362.smethod_57(ff_Encryption_0, ff_Encryption_0.byte_0);
						}
					}
					return Class362.smethod_92(ff_Encryption_0);
				}
				return 7575267975146914831UL;
			case FF_Encryption.FFGame.FFXIII_3:
				if (bool_0)
				{
					if (ff_Encryption_0.byte_0 != null)
					{
						if (ff_Encryption_0.byte_0.Length == 16)
						{
							return Class362.smethod_57(ff_Encryption_0, ff_Encryption_0.byte_0);
						}
					}
					return Class362.smethod_92(ff_Encryption_0);
				}
				return 716306939262946301UL;
			default:
				return 0UL;
			}
		}

		// Token: 0x060067D1 RID: 26577 RVA: 0x00225510 File Offset: 0x00223710
		static string[] smethod_103()
		{
			return new string[]
			{
				"syscon_manager_key=D413B89663E1FE9F75143D3BB4565274",
				"keygen_key=6B1ACEA246B745FD8F93763B920594CD53483B82",
				"savegame_param_sfo_key=0C08000E090504040D010F000406020209060D03",
				"trophy_param_sfo_key=5D5B647917024E9BB8D330486B996E795D7F4392",
				"tropsys_dat_key=B080C40FF358643689281736A6BF15892CFEA436",
				"tropusr_dat_key=8711EFF406913F0937F115FAB23DE1A9897A789A",
				"troptrns_dat_key=91EE81555ACC1C4FB5AAE5462CFE1C62A4AF36A5",
				"tropconf_sfm_key=E2ED33C71C444EEBC1E23D635AD8E82F4ECA4E94",
				"fallback_disc_hash_key=D1C1E10B9C547E689B805DCD9710CE8D"
			};
		}

		// Token: 0x060067D2 RID: 26578 RVA: 0x00225570 File Offset: 0x00223770
		unsafe static int smethod_104(int int_0, byte[] byte_0, Class371.Class372 class372_0, int int_1)
		{
			void* ptr = stackalloc byte[8];
			*(int*)ptr = 0;
			for (;;)
			{
				if (class372_0.int_4 != 11)
				{
					*(int*)((byte*)ptr + 4) = Class362.smethod_83(byte_0, int_0, class372_0.class374_0, int_1);
					int_0 += *(int*)((byte*)ptr + 4);
					*(int*)ptr = *(int*)ptr + *(int*)((byte*)ptr + 4);
					int_1 -= *(int*)((byte*)ptr + 4);
					if (int_1 == 0)
					{
						goto Block_4;
					}
				}
				if (!Class362.smethod_7(class372_0))
				{
					if (class372_0.class374_0.int_1 <= 0)
					{
						break;
					}
					if (class372_0.int_4 == 11)
					{
						break;
					}
				}
			}
			goto IL_6B;
			Block_4:
			return *(int*)ptr;
			IL_6B:
			return *(int*)ptr;
		}

		// Token: 0x060067D3 RID: 26579 RVA: 0x002255EC File Offset: 0x002237EC
		static void smethod_105(Form0 form0_0)
		{
			form0_0.icontainer_1 = new Container();
			ComponentResourceManager componentResourceManager = new ComponentResourceManager(typeof(Form0));
			form0_0.styleManager_0 = new StyleManager(form0_0.icontainer_1);
			form0_0.qatCustomizeItem_0 = new QatCustomizeItem();
			form0_0.buttonItem_0 = new ButtonItem();
			form0_0.metroTabItem_0 = new MetroTabItem();
			form0_0.metroTabItem_1 = new MetroTabItem();
			form0_0.metroAppButton_0 = new MetroAppButton();
			form0_0.metroShell_0 = new MetroShell();
			form0_0.metroTabPanel_0 = new MetroTabPanel();
			form0_0.warningBox_0 = new WarningBox();
			form0_0.metroTilePanel_0 = new MetroTilePanel();
			form0_0.itemContainer_0 = new ItemContainer();
			form0_0.metroTileItem_0 = new MetroTileItem();
			form0_0.metroTileItem_1 = new MetroTileItem();
			form0_0.metroAppButton_1 = new MetroAppButton();
			form0_0.metroTabItem_2 = new MetroTabItem();
			form0_0.buttonItem_2 = new ButtonItem();
			form0_0.buttonItem_1 = new ButtonItem();
			form0_0.metroStatusBar_0 = new MetroStatusBar();
			form0_0.labelItem_0 = new LabelItem();
			form0_0.metroAppButton_2 = new MetroAppButton();
			form0_0.metroShell_0.SuspendLayout();
			form0_0.metroTabPanel_0.SuspendLayout();
			form0_0.SuspendLayout();
			form0_0.styleManager_0.ManagerStyle = eStyle.Metro;
			form0_0.styleManager_0.MetroColorParameters = new MetroColorGeneratorParameters(Color.FromArgb(255, 255, 255), Color.FromArgb(69, 150, 232));
			form0_0.qatCustomizeItem_0.BeginGroup = true;
			form0_0.qatCustomizeItem_0.Name = "qatCustomizeItem1";
			form0_0.buttonItem_0.Name = "buttonItem1";
			form0_0.buttonItem_0.Text = "360HAVEN";
			form0_0.metroTabItem_0.Name = "metroTabItem2";
			form0_0.metroTabItem_0.Text = "&VIEW";
			form0_0.metroTabItem_1.Checked = true;
			form0_0.metroTabItem_1.Name = "metroTabItem1";
			form0_0.metroTabItem_1.Text = "&HOME";
			form0_0.metroAppButton_0.AutoExpandOnClick = true;
			form0_0.metroAppButton_0.CanCustomize = false;
			form0_0.metroAppButton_0.ImageFixedSize = new Size(16, 16);
			form0_0.metroAppButton_0.ImagePaddingHorizontal = 0;
			form0_0.metroAppButton_0.ImagePaddingVertical = 0;
			form0_0.metroAppButton_0.Name = "metroAppButton1";
			form0_0.metroAppButton_0.ShowSubItems = false;
			form0_0.metroAppButton_0.Text = "&File";
			form0_0.metroShell_0.BackColor = Color.White;
			form0_0.metroShell_0.BackgroundStyle.CornerType = eCornerType.Square;
			form0_0.metroShell_0.CanCustomize = false;
			form0_0.metroShell_0.CaptionFont = new Font("Microsoft Sans Serif", 9.75f, FontStyle.Regular, GraphicsUnit.Point, 0);
			form0_0.metroShell_0.CaptionVisible = true;
			form0_0.metroShell_0.Controls.Add(form0_0.metroTabPanel_0);
			form0_0.metroShell_0.Dock = DockStyle.Fill;
			form0_0.metroShell_0.ForeColor = Color.Black;
			form0_0.metroShell_0.HelpButtonText = null;
			form0_0.metroShell_0.HelpButtonVisible = false;
			form0_0.metroShell_0.Items.AddRange(new BaseItem[]
			{
				form0_0.metroAppButton_1,
				form0_0.metroAppButton_2,
				form0_0.metroTabItem_2,
				form0_0.buttonItem_2
			});
			form0_0.metroShell_0.KeyTipsFont = new Font("Tahoma", 7f);
			form0_0.metroShell_0.Location = new Point(5, 1);
			form0_0.metroShell_0.Name = "metroShell1";
			form0_0.metroShell_0.QuickToolbarItems.AddRange(new BaseItem[]
			{
				form0_0.buttonItem_1
			});
			form0_0.metroShell_0.SettingsButtonVisible = false;
			form0_0.metroShell_0.Size = new Size(576, 319);
			form0_0.metroShell_0.SystemText.MaximizeRibbonText = "&Maximize the Ribbon";
			form0_0.metroShell_0.SystemText.MinimizeRibbonText = "Mi&nimize the Ribbon";
			form0_0.metroShell_0.SystemText.QatAddItemText = "&Add to Quick Access Toolbar";
			form0_0.metroShell_0.SystemText.QatCustomizeMenuLabel = "<b>Customize Quick Access Toolbar</b>";
			form0_0.metroShell_0.SystemText.QatCustomizeText = "&Customize Quick Access Toolbar...";
			form0_0.metroShell_0.SystemText.QatDialogAddButton = "&Add >>";
			form0_0.metroShell_0.SystemText.QatDialogCancelButton = "Cancel";
			form0_0.metroShell_0.SystemText.QatDialogCaption = "Customize Quick Access Toolbar";
			form0_0.metroShell_0.SystemText.QatDialogCategoriesLabel = "&Choose commands from:";
			form0_0.metroShell_0.SystemText.QatDialogOkButton = "OK";
			form0_0.metroShell_0.SystemText.QatDialogPlacementCheckbox = "&Place Quick Access Toolbar below the Ribbon";
			form0_0.metroShell_0.SystemText.QatDialogRemoveButton = "&Remove";
			form0_0.metroShell_0.SystemText.QatPlaceAboveRibbonText = "&Place Quick Access Toolbar above the Ribbon";
			form0_0.metroShell_0.SystemText.QatPlaceBelowRibbonText = "&Place Quick Access Toolbar below the Ribbon";
			form0_0.metroShell_0.SystemText.QatRemoveItemText = "&Remove from Quick Access Toolbar";
			form0_0.metroShell_0.TabIndex = 0;
			form0_0.metroShell_0.TabStripFont = new Font("Segoe UI", 10.25f, FontStyle.Bold, GraphicsUnit.Point, 0);
			form0_0.metroShell_0.Text = "metroShell1";
			form0_0.metroTabPanel_0.ColorSchemeStyle = eDotNetBarStyle.StyleManagerControlled;
			form0_0.metroTabPanel_0.Controls.Add(form0_0.warningBox_0);
			form0_0.metroTabPanel_0.Controls.Add(form0_0.metroTilePanel_0);
			form0_0.metroTabPanel_0.Dock = DockStyle.Fill;
			form0_0.metroTabPanel_0.Location = new Point(0, 56);
			form0_0.metroTabPanel_0.Name = "metroTabPanel1";
			form0_0.metroTabPanel_0.Padding = new System.Windows.Forms.Padding(3, 0, 3, 3);
			form0_0.metroTabPanel_0.Size = new Size(576, 263);
			form0_0.metroTabPanel_0.Style.CornerType = eCornerType.Square;
			form0_0.metroTabPanel_0.StyleMouseDown.CornerType = eCornerType.Square;
			form0_0.metroTabPanel_0.StyleMouseOver.CornerType = eCornerType.Square;
			form0_0.metroTabPanel_0.TabIndex = 1;
			form0_0.warningBox_0.BackColor = Color.FromArgb(255, 255, 255);
			form0_0.warningBox_0.CloseButtonVisible = false;
			form0_0.warningBox_0.Dock = DockStyle.Top;
			form0_0.warningBox_0.ForeColor = Color.Black;
			form0_0.warningBox_0.Image = (Image)componentResourceManager.GetObject("warningBox1.Image");
			form0_0.warningBox_0.Location = new Point(3, 0);
			form0_0.warningBox_0.Name = "warningBox1";
			form0_0.warningBox_0.OptionsButtonVisible = false;
			form0_0.warningBox_0.Size = new Size(570, 108);
			form0_0.warningBox_0.TabIndex = 1;
			form0_0.warningBox_0.Text = componentResourceManager.GetString("warningBox1.Text");
			form0_0.metroTilePanel_0.BackgroundStyle.Class = "MetroTilePanel";
			form0_0.metroTilePanel_0.BackgroundStyle.CornerType = eCornerType.Square;
			form0_0.metroTilePanel_0.ContainerControlProcessDialogKey = true;
			form0_0.metroTilePanel_0.Dock = DockStyle.Bottom;
			form0_0.metroTilePanel_0.HorizontalItemAlignment = eHorizontalItemsAlignment.Center;
			form0_0.metroTilePanel_0.Items.AddRange(new BaseItem[]
			{
				form0_0.itemContainer_0
			});
			form0_0.metroTilePanel_0.ItemSpacing = 60;
			form0_0.metroTilePanel_0.Location = new Point(3, 94);
			form0_0.metroTilePanel_0.Name = "metroTilePanel1";
			form0_0.metroTilePanel_0.Size = new Size(570, 166);
			form0_0.metroTilePanel_0.Style = eDotNetBarStyle.Metro;
			form0_0.metroTilePanel_0.TabIndex = 0;
			form0_0.metroTilePanel_0.Text = "metroTilePanel1";
			form0_0.itemContainer_0.BackgroundStyle.CornerType = eCornerType.Square;
			form0_0.itemContainer_0.MultiLine = true;
			form0_0.itemContainer_0.Name = "itemContainer1";
			form0_0.itemContainer_0.SubItems.AddRange(new BaseItem[]
			{
				form0_0.metroTileItem_0,
				form0_0.metroTileItem_1
			});
			form0_0.itemContainer_0.TitleStyle.Class = "MetroTileGroupTitle";
			form0_0.itemContainer_0.TitleStyle.CornerType = eCornerType.Square;
			form0_0.metroTileItem_0.Image = (Image)componentResourceManager.GetObject("xdec.Image");
			form0_0.metroTileItem_0.Name = "xdec";
			form0_0.metroTileItem_0.TileColor = eMetroTileColor.Blue;
			form0_0.metroTileItem_0.TileSize = new Size(248, 124);
			form0_0.metroTileItem_0.TileStyle.BackColor = Color.DarkCyan;
			form0_0.metroTileItem_0.TileStyle.BackColor2 = Color.RoyalBlue;
			form0_0.metroTileItem_0.TileStyle.CornerType = eCornerType.Square;
			form0_0.metroTileItem_0.TitleText = "Decrypt";
			form0_0.metroTileItem_0.TitleTextAlignment = ContentAlignment.BottomRight;
			form0_0.metroTileItem_0.TitleTextColor = Color.Black;
			form0_0.metroTileItem_0.TitleTextFont = new Font("Times New Roman", 18f, FontStyle.Bold, GraphicsUnit.Point, 0);
			form0_0.metroTileItem_0.Click += form0_0.method_24;
			form0_0.metroTileItem_1.Image = (Image)componentResourceManager.GetObject("xec.Image");
			form0_0.metroTileItem_1.Name = "xec";
			form0_0.metroTileItem_1.TileColor = eMetroTileColor.Blue;
			form0_0.metroTileItem_1.TileSize = new Size(248, 124);
			form0_0.metroTileItem_1.TileStyle.BackColor = Color.DarkCyan;
			form0_0.metroTileItem_1.TileStyle.BackColor2 = Color.RoyalBlue;
			form0_0.metroTileItem_1.TileStyle.CornerType = eCornerType.Square;
			form0_0.metroTileItem_1.TitleText = "Encrypt";
			form0_0.metroTileItem_1.TitleTextAlignment = ContentAlignment.BottomRight;
			form0_0.metroTileItem_1.TitleTextColor = Color.Black;
			form0_0.metroTileItem_1.TitleTextFont = new Font("Times New Roman", 18f, FontStyle.Bold, GraphicsUnit.Point, 0);
			form0_0.metroTileItem_1.Click += form0_0.method_25;
			form0_0.metroAppButton_1.AutoExpandOnClick = true;
			form0_0.metroAppButton_1.CanCustomize = false;
			form0_0.metroAppButton_1.FixedSize = new Size(80, 30);
			form0_0.metroAppButton_1.ImageFixedSize = new Size(16, 16);
			form0_0.metroAppButton_1.ImagePaddingHorizontal = 0;
			form0_0.metroAppButton_1.ImagePaddingVertical = 0;
			form0_0.metroAppButton_1.Name = "metroAppButton2";
			form0_0.metroAppButton_1.ShowSubItems = false;
			form0_0.metroAppButton_1.Text = "&File";
			form0_0.metroAppButton_1.Tooltip = "Load xbox 360 APP.DAT";
			form0_0.metroAppButton_1.Click += form0_0.method_22;
			form0_0.metroTabItem_2.Checked = true;
			form0_0.metroTabItem_2.Name = "xmaintab";
			form0_0.metroTabItem_2.Panel = form0_0.metroTabPanel_0;
			form0_0.metroTabItem_2.Text = "ENCRYPTOR";
			form0_0.metroTabItem_2.Click += form0_0.method_27;
			form0_0.metroTabItem_2.MouseHover += form0_0.method_28;
			form0_0.buttonItem_2.ButtonStyle = eButtonStyle.ImageAndText;
			form0_0.buttonItem_2.ForeColor = Color.Red;
			form0_0.buttonItem_2.Image = (Image)componentResourceManager.GetObject("xclose.Image");
			form0_0.buttonItem_2.Name = "xclose";
			form0_0.buttonItem_2.Text = "Close";
			form0_0.buttonItem_2.Tooltip = "Close File";
			form0_0.buttonItem_2.Visible = false;
			form0_0.buttonItem_2.Click += form0_0.method_26;
			form0_0.buttonItem_1.Name = "buttonItem2";
			form0_0.buttonItem_1.Text = "360HAVEN";
			form0_0.buttonItem_1.Click += form0_0.method_23;
			form0_0.metroStatusBar_0.BackgroundStyle.CornerType = eCornerType.Square;
			form0_0.metroStatusBar_0.ContainerControlProcessDialogKey = true;
			form0_0.metroStatusBar_0.Dock = DockStyle.Bottom;
			form0_0.metroStatusBar_0.Font = new Font("Segoe UI", 10.25f, FontStyle.Bold, GraphicsUnit.Point, 0);
			form0_0.metroStatusBar_0.Items.AddRange(new BaseItem[]
			{
				form0_0.labelItem_0
			});
			form0_0.metroStatusBar_0.Location = new Point(5, 298);
			form0_0.metroStatusBar_0.Name = "metroStatusBar1";
			form0_0.metroStatusBar_0.Size = new Size(576, 22);
			form0_0.metroStatusBar_0.TabIndex = 1;
			form0_0.labelItem_0.Name = "xstatus";
			form0_0.labelItem_0.Text = "Idle";
			form0_0.metroAppButton_2.AutoExpandOnClick = true;
			form0_0.metroAppButton_2.CanCustomize = false;
			form0_0.metroAppButton_2.FixedSize = new Size(80, 30);
			form0_0.metroAppButton_2.ImageFixedSize = new Size(16, 16);
			form0_0.metroAppButton_2.ImagePaddingHorizontal = 0;
			form0_0.metroAppButton_2.ImagePaddingVertical = 0;
			form0_0.metroAppButton_2.Name = "metroAppButton3";
			form0_0.metroAppButton_2.ShowSubItems = false;
			form0_0.metroAppButton_2.Text = "&Directory";
			form0_0.metroAppButton_2.Tooltip = "Load PS3 Save Directory";
			form0_0.metroAppButton_2.Click += form0_0.method_29;
			form0_0.AutoScaleDimensions = new SizeF(6f, 13f);
			form0_0.AutoScaleMode = AutoScaleMode.Font;
			form0_0.ClientSize = new Size(586, 322);
			form0_0.Controls.Add(form0_0.metroStatusBar_0);
			form0_0.Controls.Add(form0_0.metroShell_0);
			form0_0.FormBorderStyle = FormBorderStyle.Fixed3D;
			form0_0.Icon = (Icon)componentResourceManager.GetObject("$this.Icon");
			form0_0.MaximizeBox = false;
			form0_0.Name = "Form1";
			form0_0.StartPosition = FormStartPosition.CenterParent;
			form0_0.Text = "Final Fantasy XIII Encryptor By Jappi88";
			form0_0.metroShell_0.ResumeLayout(false);
			form0_0.metroShell_0.PerformLayout();
			form0_0.metroTabPanel_0.ResumeLayout(false);
			form0_0.ResumeLayout(false);
		}

		// Token: 0x060067D4 RID: 26580 RVA: 0x0004BF53 File Offset: 0x0004A153
		static bool smethod_106(Class6 class6_0)
		{
			return class6_0.method_4().method_2(class6_0.method_0(), false);
		}

		// Token: 0x060067D5 RID: 26581 RVA: 0x0004BF67 File Offset: 0x0004A167
		static byte[] smethod_107(Class4 class4_0)
		{
			return Class4.byte_9;
		}

		// Token: 0x060067D6 RID: 26582 RVA: 0x0004BF6E File Offset: 0x0004A16E
		static byte[] smethod_108(Class4 class4_0)
		{
			return new HMACSHA1(Class4.byte_5).ComputeHash(new byte[0]);
		}

		// Token: 0x060067D7 RID: 26583 RVA: 0x00226408 File Offset: 0x00224608
		static bool smethod_109(ref byte[] byte_0, Class4 class4_0, string string_0)
		{
			bool result;
			try
			{
				byte[] array = class4_0.method_4(new MemoryStream(byte_0), string_0);
				if (array == null)
				{
					result = false;
				}
				else
				{
					Array.Resize<byte>(ref byte_0, array.Length);
					Array.Copy(array, 0, byte_0, 0, array.Length);
					result = true;
				}
			}
			catch
			{
				result = false;
			}
			return result;
		}

		// Token: 0x060067D8 RID: 26584 RVA: 0x00226460 File Offset: 0x00224660
		static byte[] smethod_110(Class4 class4_0)
		{
			byte[] array = Class4.Struct3.smethod_0();
			return Class362.smethod_15(array.Length, array, 0, Class4.byte_5);
		}

		// Token: 0x060067D9 RID: 26585 RVA: 0x00226484 File Offset: 0x00224684
		static bool smethod_111(byte[] byte_0, byte[] byte_1)
		{
			if (byte_0.Length != byte_1.Length)
			{
				return false;
			}
			for (int i = 0; i < byte_0.Length; i++)
			{
				if (byte_0[i] != byte_1[i])
				{
					return false;
				}
			}
			return true;
		}
	}
}
