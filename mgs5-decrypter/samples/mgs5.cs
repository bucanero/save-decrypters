using System;
using System.Security.Cryptography;
using System.Text;
using MasterLib.IO;

namespace MasterLib.Games.MGS5
{
	// Token: 0x02000007 RID: 7
	public class SecureFile
	{
		// Token: 0x17000015 RID: 21
		// (get) Token: 0x06000069 RID: 105 RVA: 0x00002786 File Offset: 0x00000986
		public string Name
		{
			get
			{
				return this.name;
			}
		}

		// Token: 0x17000016 RID: 22
		// (get) Token: 0x0600006A RID: 106 RVA: 0x0000278E File Offset: 0x0000098E
		// (set) Token: 0x0600006B RID: 107 RVA: 0x00002796 File Offset: 0x00000996
		public byte[] Data
		{
			get
			{
				return this.data;
			}
			set
			{
				this.data = value;
			}
		}

		// Token: 0x0600006C RID: 108 RVA: 0x0000279F File Offset: 0x0000099F
		public SecureFile(string name) : this(name, null)
		{
		}

		// Token: 0x0600006D RID: 109 RVA: 0x000027A9 File Offset: 0x000009A9
		public SecureFile(string name, byte[] data)
		{
			if (name == null)
			{
				throw new ArgumentNullException("name", "SecureFile.Error: The name is null!");
			}
			this.name = name;
			this.data = data;
			this.Initialize();
		}

		// Token: 0x0600006E RID: 110 RVA: 0x00002CB4 File Offset: 0x00000EB4
		private void Initialize()
		{
			this.CalculateSalt();
			if (this.data != null)
			{
				this.data = this.EncData(this.data);
				if (!this.CalculateMD5(this.data, 16, null).Compare(this.data.Extract(0, 16)))
				{
					throw new Exception("SecureFile.Error: The data is corrupt!");
				}
				this.data = this.data.Extract(16);
			}
		}

		// Token: 0x0600006F RID: 111 RVA: 0x00002D2C File Offset: 0x00000F2C
		private void CalculateSalt()
		{
			using (MD5CryptoServiceProvider md5CryptoServiceProvider = new MD5CryptoServiceProvider())
			{
				md5CryptoServiceProvider.ComputeHash(Encoding.ASCII.GetBytes(this.name));
				this.salt = (uint)((int)md5CryptoServiceProvider.Hash[3] << 24 | (int)md5CryptoServiceProvider.Hash[2] << 16 | (int)md5CryptoServiceProvider.Hash[1] << 8 | (int)md5CryptoServiceProvider.Hash[0]);
			}
		}

		// Token: 0x06000070 RID: 112 RVA: 0x00002DA4 File Offset: 0x00000FA4
		private byte[] CalculateMD5(byte[] source, int offset, int? size = null)
		{
			byte[] hash;
			using (MD5CryptoServiceProvider md5CryptoServiceProvider = new MD5CryptoServiceProvider())
			{
				md5CryptoServiceProvider.ComputeHash(source, offset, size ?? (source.Length - offset));
				hash = md5CryptoServiceProvider.Hash;
			}
			return hash;
		}

		// Token: 0x06000071 RID: 113 RVA: 0x00002E00 File Offset: 0x00001000
		private byte[] EncData(byte[] source)
		{
			uint num = this.salt;
			byte[] buffer;
			using (MasterIO masterIO = new MasterIO(source, Endian.Big))
			{
				using (MasterIO masterIO2 = new MasterIO(Endian.Big))
				{
					for (int i = 0; i < source.Length >> 2; i++)
					{
						uint num2 = num;
						num = (num2 ^ num2 << 13);
						uint num3 = num;
						num = (num3 ^ num3 >> 7);
						uint num4 = num;
						num = (num4 ^ num4 << 5);
						masterIO2.Writer.WriteUInt32(masterIO.Reader.ReadUInt32() ^ num);
					}
					buffer = masterIO2.Buffer;
				}
			}
			return buffer;
		}

		// Token: 0x06000072 RID: 114 RVA: 0x00002E9C File Offset: 0x0000109C
		public byte[] Write()
		{
			byte[] array = new byte[this.data.Length + 16];
			Buffer.BlockCopy(this.CalculateMD5(this.data, 0, null), 0, array, 0, 16);
			Buffer.BlockCopy(this.data, 0, array, 16, this.data.Length);
			return this.EncData(array);
		}

		// Token: 0x06000073 RID: 115 RVA: 0x000027D8 File Offset: 0x000009D8
		public byte[] Write(byte[] data)
		{
			this.data = data;
			return this.Write();
		}

		// Token: 0x0400000E RID: 14
		private string name;

		// Token: 0x0400000F RID: 15
		private byte[] data;

		// Token: 0x04000010 RID: 16
		private uint salt;
	}
}
