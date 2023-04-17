using System;
using System.ComponentModel;
using System.Drawing;
using System.IO;
using System.Security.Cryptography;
using System.Windows.Forms;
using ns0;

namespace Need_For_Speed_Undercover_Save_Decrypter
{
	// Token: 0x02000006 RID: 6
	public class MainForm : Form
	{
		// Token: 0x0600006D RID: 109 RVA: 0x00002427 File Offset: 0x00000627
		public MainForm()
		{
			this.InitializeComponent();
		}

		// Token: 0x0600006E RID: 110 RVA: 0x000031DC File Offset: 0x000013DC
		private void DecryptButton_Click(object sender, EventArgs e)
		{
			OpenFileDialog openFileDialog = new OpenFileDialog();
			if (openFileDialog.ShowDialog() == DialogResult.OK)
			{
				this.endianIO_0 = new EndianIO(openFileDialog.FileName, EndianType.BigEndian, true);
				byte[] array = new byte[16];
				byte[] array2 = new byte[16];
				this.endianIO_0.SeekTo(100);
				uint num = this.endianIO_0.EndianReader_0.ReadUInt32() - 20U;
				this.endianIO_0.SeekTo(112);
				array = this.endianIO_0.EndianReader_0.ReadBytes(16);
				int i;
				for (i = 0; i <= array.Length - 1; i++)
				{
					byte[] array3 = array;
					int num2 = i;
					array3[num2] ^= this.byte_0[i];
				}
				MD5CryptoServiceProvider md5CryptoServiceProvider = new MD5CryptoServiceProvider();
				array = md5CryptoServiceProvider.ComputeHash(array);
				EndianIO endianIO = new EndianIO(new MemoryStream(), EndianType.BigEndian, true);
				i = 0;
				while ((long)i <= (long)((ulong)(num - 1U)))
				{
					byte[] array4 = this.endianIO_0.EndianReader_0.ReadBytes(16);
					array2 = md5CryptoServiceProvider.ComputeHash(array4);
					for (int j = 0; j < 16; j++)
					{
						byte[] array5 = array4;
						int num3 = j;
						array5[num3] ^= array[j];
					}
					array = array2;
					endianIO.Out.Write(array4);
					i += 16;
				}
				SaveFileDialog saveFileDialog = new SaveFileDialog();
				saveFileDialog.Filter = "NFS Undercover Database Files (*.db3)|*.db3";
				if (saveFileDialog.ShowDialog() == DialogResult.OK)
				{
					File.WriteAllBytes(saveFileDialog.FileName, endianIO.ToArray());
					this.DecryptButton.Enabled = false;
					this.EncryptButton.Enabled = true;
					MessageBox.Show("Successfully Exported Database To File:\n" + Path.GetFullPath(saveFileDialog.FileName), "DB Export OK", MessageBoxButtons.OK, MessageBoxIcon.Asterisk);
				}
			}
		}

		// Token: 0x0600006F RID: 111 RVA: 0x000033B4 File Offset: 0x000015B4
		private void EncryptButton_Click(object sender, EventArgs e)
		{
			OpenFileDialog openFileDialog = new OpenFileDialog();
			openFileDialog.Title = "Open Decrypted File.";
			if (openFileDialog.ShowDialog() == DialogResult.OK && this.endianIO_0 != null)
			{
				byte[] array = new byte[16];
				this.endianIO_0.SeekTo(100);
				uint num = this.endianIO_0.EndianReader_0.ReadUInt32() - 20U;
				this.endianIO_0.SeekTo(112);
				array = this.endianIO_0.EndianReader_0.ReadBytes(16);
				int i;
				for (i = 0; i <= array.Length - 1; i++)
				{
					byte[] array2 = array;
					int num2 = i;
					array2[num2] ^= this.byte_0[i];
				}
				MD5CryptoServiceProvider md5CryptoServiceProvider = new MD5CryptoServiceProvider();
				array = md5CryptoServiceProvider.ComputeHash(array);
				EndianIO endianIO = new EndianIO(openFileDialog.FileName, EndianType.BigEndian, true);
				i = 0;
				while ((long)i <= (long)((ulong)(num - 1U)))
				{
					byte[] array3 = endianIO.EndianReader_0.ReadBytes(16);
					for (int j = 0; j < 16; j++)
					{
						byte[] array4 = array3;
						int num3 = j;
						array4[num3] ^= array[j];
					}
					array = md5CryptoServiceProvider.ComputeHash(array3);
					this.endianIO_0.Out.Write(array3);
					i += 16;
				}
				endianIO.Close();
				byte[] buffer = this.endianIO_0.ToArray();
				this.endianIO_0.SeekTo(0);
				this.endianIO_0.Out.Write(buffer);
				this.endianIO_0.Close();
				this.DecryptButton.Enabled = true;
				this.EncryptButton.Enabled = false;
				MessageBox.Show("File Saved!");
			}
		}

		// Token: 0x06000070 RID: 112 RVA: 0x00002454 File Offset: 0x00000654
		protected override void Dispose(bool disposing)
		{
			if (disposing && this.icontainer_0 != null)
			{
				this.icontainer_0.Dispose();
			}
			base.Dispose(disposing);
		}

		// Token: 0x06000071 RID: 113 RVA: 0x00003578 File Offset: 0x00001778
		private void InitializeComponent()
		{
			ComponentResourceManager componentResourceManager = new ComponentResourceManager(typeof(MainForm));
			this.DecryptButton = new Button();
			this.EncryptButton = new Button();
			this.label1 = new Label();
			base.SuspendLayout();
			this.DecryptButton.Location = new Point(12, 12);
			this.DecryptButton.Name = "DecryptButton";
			this.DecryptButton.Size = new Size(335, 35);
			this.DecryptButton.TabIndex = 0;
			this.DecryptButton.Text = "Decrypt";
			this.DecryptButton.UseVisualStyleBackColor = true;
			this.DecryptButton.Click += this.DecryptButton_Click;
			this.EncryptButton.Location = new Point(12, 53);
			this.EncryptButton.Name = "EncryptButton";
			this.EncryptButton.Size = new Size(335, 35);
			this.EncryptButton.TabIndex = 1;
			this.EncryptButton.Text = "Encrypt";
			this.EncryptButton.UseVisualStyleBackColor = true;
			this.EncryptButton.Click += this.EncryptButton_Click;
			this.label1.AutoSize = true;
			this.label1.Location = new Point(12, 94);
			this.label1.Name = "label1";
			this.label1.Size = new Size(121, 13);
			this.label1.TabIndex = 2;
			this.label1.Text = "Made By: Red-EyeX32";
			base.AutoScaleDimensions = new SizeF(6f, 13f);
			base.AutoScaleMode = AutoScaleMode.Font;
			base.ClientSize = new Size(359, 116);
			base.Controls.Add(this.label1);
			base.Controls.Add(this.EncryptButton);
			base.Controls.Add(this.DecryptButton);
			this.Font = new Font("Consolas", 8.25f, FontStyle.Regular, GraphicsUnit.Point, 0);
			base.Icon = (Icon)componentResourceManager.GetObject("$this.Icon");
			base.MaximizeBox = false;
			base.MinimizeBox = false;
			base.Name = "MainForm";
			base.StartPosition = FormStartPosition.CenterScreen;
			this.Text = "Need For Speed: Undercover Save Decrypter";
			base.ResumeLayout(false);
			base.PerformLayout();
		}

		// Token: 0x04000010 RID: 16
		private EndianIO endianIO_0;

		// Token: 0x04000011 RID: 17
		private byte[] byte_0 = new byte[]
		{
			33,
			243,
			198,
			210,
			8,
			99,
			170,
			172,
			56,
			226,
			32,
			98,
			13,
			13,
			77,
			82
		};

		// Token: 0x04000012 RID: 18
		private IContainer icontainer_0 = null;

		// Token: 0x04000013 RID: 19
		private Button DecryptButton;

		// Token: 0x04000014 RID: 20
		private Button EncryptButton;

		// Token: 0x04000015 RID: 21
		private Label label1;
	}
}
