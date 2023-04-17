using System;
using System.ComponentModel;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Runtime.CompilerServices;
using System.Windows.Forms;
using Microsoft.VisualBasic;
using Microsoft.VisualBasic.CompilerServices;
using PackageIO;

namespace WindowsApplication1
{
	// Token: 0x02000009 RID: 9
	[DesignerGenerated]
	public class Form1 : Form
	{
		// Token: 0x0600001F RID: 31 RVA: 0x0000245A File Offset: 0x0000065A
		public Form1()
		{
			Class14.rpqhRGszkN9lW();
			base..ctor();
			base.Load += this.Form1_Load;
			this.InitializeComponent();
		}

		// Token: 0x06000020 RID: 32 RVA: 0x00006B48 File Offset: 0x00004D48
		[DebuggerNonUserCode]
		protected override void Dispose(bool disposing)
		{
			try
			{
				if (disposing && this.icontainer_0 != null)
				{
					this.icontainer_0.Dispose();
				}
			}
			finally
			{
				base.Dispose(disposing);
			}
		}

		// Token: 0x06000021 RID: 33 RVA: 0x00006B88 File Offset: 0x00004D88
		[DebuggerStepThrough]
		private void InitializeComponent()
		{
			ComponentResourceManager componentResourceManager = new ComponentResourceManager(typeof(Form1));
			this.MenuStrip1 = new MenuStrip();
			this.OpenToolStripMenuItem = new ToolStripMenuItem();
			this.AboutToolStripMenuItem = new ToolStripMenuItem();
			this.DonateToolStripMenuItem = new ToolStripMenuItem();
			this.Button1 = new Button();
			this.Button2 = new Button();
			this.PictureBox1 = new PictureBox();
			this.MenuStrip1.SuspendLayout();
			((ISupportInitialize)this.PictureBox1).BeginInit();
			this.SuspendLayout();
			this.MenuStrip1.Items.AddRange(new ToolStripItem[]
			{
				this.OpenToolStripMenuItem,
				this.AboutToolStripMenuItem,
				this.DonateToolStripMenuItem
			});
			Control menuStrip = this.MenuStrip1;
			Point location = new Point(0, 0);
			menuStrip.Location = location;
			this.MenuStrip1.Name = "MenuStrip1";
			Control menuStrip2 = this.MenuStrip1;
			Size size = new Size(330, 24);
			menuStrip2.Size = size;
			this.MenuStrip1.TabIndex = 0;
			this.MenuStrip1.Text = "MenuStrip1";
			this.OpenToolStripMenuItem.Name = "OpenToolStripMenuItem";
			ToolStripItem openToolStripMenuItem = this.OpenToolStripMenuItem;
			size = new Size(48, 20);
			openToolStripMenuItem.Size = size;
			this.OpenToolStripMenuItem.Text = "Open";
			this.AboutToolStripMenuItem.Name = "AboutToolStripMenuItem";
			ToolStripItem aboutToolStripMenuItem = this.AboutToolStripMenuItem;
			size = new Size(52, 20);
			aboutToolStripMenuItem.Size = size;
			this.AboutToolStripMenuItem.Text = "About";
			this.DonateToolStripMenuItem.Name = "DonateToolStripMenuItem";
			ToolStripItem donateToolStripMenuItem = this.DonateToolStripMenuItem;
			size = new Size(57, 20);
			donateToolStripMenuItem.Size = size;
			this.DonateToolStripMenuItem.Text = "Donate";
			Control button = this.Button1;
			location = new Point(58, 177);
			button.Location = location;
			this.Button1.Name = "Button1";
			Control button2 = this.Button1;
			size = new Size(81, 36);
			button2.Size = size;
			this.Button1.TabIndex = 1;
			this.Button1.Text = "Decrypt";
			this.Button1.UseVisualStyleBackColor = true;
			Control button3 = this.Button2;
			location = new Point(190, 177);
			button3.Location = location;
			this.Button2.Name = "Button2";
			Control button4 = this.Button2;
			size = new Size(81, 36);
			button4.Size = size;
			this.Button2.TabIndex = 2;
			this.Button2.Text = "Encrypt";
			this.Button2.UseVisualStyleBackColor = true;
			this.PictureBox1.BorderStyle = BorderStyle.FixedSingle;
			this.PictureBox1.Image = Class6.smethod_0();
			Control pictureBox = this.PictureBox1;
			location = new Point(35, 39);
			pictureBox.Location = location;
			this.PictureBox1.Name = "PictureBox1";
			Control pictureBox2 = this.PictureBox1;
			size = new Size(256, 117);
			pictureBox2.Size = size;
			this.PictureBox1.TabIndex = 3;
			this.PictureBox1.TabStop = false;
			SizeF autoScaleDimensions = new SizeF(6f, 13f);
			this.AutoScaleDimensions = autoScaleDimensions;
			this.AutoScaleMode = AutoScaleMode.Font;
			size = new Size(330, 233);
			this.ClientSize = size;
			this.Controls.Add(this.PictureBox1);
			this.Controls.Add(this.Button2);
			this.Controls.Add(this.Button1);
			this.Controls.Add(this.MenuStrip1);
			this.Icon = (Icon)componentResourceManager.GetObject("$this.Icon");
			this.MainMenuStrip = this.MenuStrip1;
			size = new Size(346, 271);
			this.MaximumSize = size;
			size = new Size(346, 271);
			this.MinimumSize = size;
			this.Name = "Form1";
			this.StartPosition = FormStartPosition.CenterScreen;
			this.Text = "DW8XL Decryptor 1.1 by chaoszage";
			this.MenuStrip1.ResumeLayout(false);
			this.MenuStrip1.PerformLayout();
			((ISupportInitialize)this.PictureBox1).EndInit();
			this.ResumeLayout(false);
			this.PerformLayout();
		}

		// Token: 0x17000008 RID: 8
		// (get) Token: 0x06000022 RID: 34 RVA: 0x00006FA8 File Offset: 0x000051A8
		// (set) Token: 0x06000023 RID: 35 RVA: 0x00002480 File Offset: 0x00000680
		internal virtual MenuStrip MenuStrip1
		{
			get
			{
				return this._MenuStrip1;
			}
			[MethodImpl(MethodImplOptions.Synchronized)]
			set
			{
				this._MenuStrip1 = value;
			}
		}

		// Token: 0x17000009 RID: 9
		// (get) Token: 0x06000024 RID: 36 RVA: 0x00006FBC File Offset: 0x000051BC
		// (set) Token: 0x06000025 RID: 37 RVA: 0x00006FD0 File Offset: 0x000051D0
		internal virtual ToolStripMenuItem OpenToolStripMenuItem
		{
			get
			{
				return this._OpenToolStripMenuItem;
			}
			[MethodImpl(MethodImplOptions.Synchronized)]
			set
			{
				EventHandler value2 = new EventHandler(this.OpenToolStripMenuItem_Click);
				if (this._OpenToolStripMenuItem != null)
				{
					this._OpenToolStripMenuItem.Click -= value2;
				}
				this._OpenToolStripMenuItem = value;
				if (this._OpenToolStripMenuItem != null)
				{
					this._OpenToolStripMenuItem.Click += value2;
				}
			}
		}

		// Token: 0x1700000A RID: 10
		// (get) Token: 0x06000026 RID: 38 RVA: 0x0000701C File Offset: 0x0000521C
		// (set) Token: 0x06000027 RID: 39 RVA: 0x00007030 File Offset: 0x00005230
		internal virtual Button Button1
		{
			get
			{
				return this._Button1;
			}
			[MethodImpl(MethodImplOptions.Synchronized)]
			set
			{
				EventHandler value2 = new EventHandler(this.Button1_Click);
				if (this._Button1 != null)
				{
					this._Button1.Click -= value2;
				}
				this._Button1 = value;
				if (this._Button1 != null)
				{
					this._Button1.Click += value2;
				}
			}
		}

		// Token: 0x1700000B RID: 11
		// (get) Token: 0x06000028 RID: 40 RVA: 0x0000707C File Offset: 0x0000527C
		// (set) Token: 0x06000029 RID: 41 RVA: 0x00007090 File Offset: 0x00005290
		internal virtual Button Button2
		{
			get
			{
				return this.ppFotdQad;
			}
			[MethodImpl(MethodImplOptions.Synchronized)]
			set
			{
				EventHandler value2 = new EventHandler(this.Button2_Click);
				if (this.ppFotdQad != null)
				{
					this.ppFotdQad.Click -= value2;
				}
				this.ppFotdQad = value;
				if (this.ppFotdQad != null)
				{
					this.ppFotdQad.Click += value2;
				}
			}
		}

		// Token: 0x1700000C RID: 12
		// (get) Token: 0x0600002A RID: 42 RVA: 0x000070DC File Offset: 0x000052DC
		// (set) Token: 0x0600002B RID: 43 RVA: 0x00002489 File Offset: 0x00000689
		internal virtual PictureBox PictureBox1
		{
			get
			{
				return this._PictureBox1;
			}
			[MethodImpl(MethodImplOptions.Synchronized)]
			set
			{
				this._PictureBox1 = value;
			}
		}

		// Token: 0x1700000D RID: 13
		// (get) Token: 0x0600002C RID: 44 RVA: 0x000070F0 File Offset: 0x000052F0
		// (set) Token: 0x0600002D RID: 45 RVA: 0x00007104 File Offset: 0x00005304
		internal virtual ToolStripMenuItem AboutToolStripMenuItem
		{
			get
			{
				return this._AboutToolStripMenuItem;
			}
			[MethodImpl(MethodImplOptions.Synchronized)]
			set
			{
				EventHandler value2 = new EventHandler(this.AboutToolStripMenuItem_Click);
				if (this._AboutToolStripMenuItem != null)
				{
					this._AboutToolStripMenuItem.Click -= value2;
				}
				this._AboutToolStripMenuItem = value;
				if (this._AboutToolStripMenuItem != null)
				{
					this._AboutToolStripMenuItem.Click += value2;
				}
			}
		}

		// Token: 0x1700000E RID: 14
		// (get) Token: 0x0600002E RID: 46 RVA: 0x00007150 File Offset: 0x00005350
		// (set) Token: 0x0600002F RID: 47 RVA: 0x00007164 File Offset: 0x00005364
		internal virtual ToolStripMenuItem DonateToolStripMenuItem
		{
			get
			{
				return this.EeyxXkpxI;
			}
			[MethodImpl(MethodImplOptions.Synchronized)]
			set
			{
				EventHandler value2 = new EventHandler(this.DonateToolStripMenuItem_Click);
				if (this.EeyxXkpxI != null)
				{
					this.EeyxXkpxI.Click -= value2;
				}
				this.EeyxXkpxI = value;
				if (this.EeyxXkpxI != null)
				{
					this.EeyxXkpxI.Click += value2;
				}
			}
		}

		// Token: 0x06000030 RID: 48 RVA: 0x000071B0 File Offset: 0x000053B0
		private void Button1_Click(object sender, EventArgs e)
		{
			Reader reader = new Reader(this.string_0, Endian.Big, 0L, FileAccess.Read, FileShare.ReadWrite);
			Writer writer = new Writer(this.string_0, Endian.Big, 0L, FileMode.Open, FileAccess.Write, FileShare.ReadWrite);
			ulong num = 319816192UL;
			ulong num2 = 1103515245UL;
			uint num3 = 255U;
			reader.Position = 0L;
			writer.Position = 0L;
			int num4 = 0;
			do
			{
				ulong num5 = num * num2;
				num = Convert.ToUInt64(decimal.Add(new decimal(num5), 12345m));
				num5 = num >> 16;
				ulong num6 = (ulong)reader.method_32();
				num5 = (num6 ^ num5);
				num5 &= (ulong)num3;
				writer.method_1((byte)num5);
				num4++;
			}
			while (num4 <= 753475);
			reader.Close(true, false);
			writer.Close(true, false);
			this.Button1.Enabled = false;
			this.Button2.Enabled = true;
			Interaction.MsgBox("Decrypted 2nd layer encryption successfully,please use BSD to apply cheats", MsgBoxStyle.OkOnly, null);
		}

		// Token: 0x06000031 RID: 49 RVA: 0x00002492 File Offset: 0x00000692
		private void Form1_Load(object sender, EventArgs e)
		{
			this.Button1.Enabled = false;
			this.Button2.Enabled = false;
		}

		// Token: 0x06000032 RID: 50 RVA: 0x000072D0 File Offset: 0x000054D0
		private void OpenToolStripMenuItem_Click(object sender, EventArgs e)
		{
			OpenFileDialog openFileDialog = new OpenFileDialog();
			openFileDialog.Title = "Open your Save";
			openFileDialog.Filter = "BIN|*.BIN*";
			if (openFileDialog.ShowDialog() == DialogResult.OK)
			{
				this.string_0 = openFileDialog.FileName;
				this.method_0();
				return;
			}
		}

		// Token: 0x06000033 RID: 51 RVA: 0x00007318 File Offset: 0x00005518
		private void method_0()
		{
			uint num = new Reader(this.string_0, Endian.Big, 0L, FileAccess.Read, FileShare.ReadWrite)
			{
				Position = 0L
			}.method_43();
			if (num == 2644696416U)
			{
				Interaction.MsgBox("Loaded Successfully", MsgBoxStyle.OkOnly, null);
				this.Button1.Enabled = true;
				this.Button2.Enabled = false;
			}
			else if (num == 4026667027U)
			{
				Interaction.MsgBox("Loaded Successfully", MsgBoxStyle.OkOnly, null);
				this.Button2.Enabled = true;
				this.Button1.Enabled = false;
			}
			else
			{
				Interaction.MsgBox("Please choose a decrypted Dynasty Warriors 8 Xtreme Legend save", MsgBoxStyle.OkOnly, null);
			}
		}

		// Token: 0x06000034 RID: 52 RVA: 0x000073C0 File Offset: 0x000055C0
		private void Button2_Click(object sender, EventArgs e)
		{
			Reader reader = new Reader(this.string_0, Endian.Big, 0L, FileAccess.Read, FileShare.ReadWrite);
			Writer writer = new Writer(this.string_0, Endian.Big, 0L, FileMode.Open, FileAccess.Write, FileShare.ReadWrite);
			ulong num = 319816192UL;
			ulong num2 = 1103515245UL;
			ulong num3 = 0UL;
			uint num4 = 255U;
			reader.Position = 0L;
			writer.Position = 0L;
			int num5 = 0;
			do
			{
				ulong num6 = num * num2;
				num = Convert.ToUInt64(decimal.Add(new decimal(num6), 12345m));
				num6 = num >> 16;
				ulong num7 = (ulong)reader.method_32();
				num3 += num7;
				num6 = (num7 ^ num6);
				num6 &= (ulong)num4;
				num3 &= (ulong)num4;
				writer.method_1((byte)num6);
				num5++;
			}
			while (num5 <= 753475);
			writer.Position = 753476L;
			writer.method_1((byte)num3);
			reader.Close(true, false);
			writer.Close(true, false);
			this.Button2.Enabled = false;
			Interaction.MsgBox("Encrypted 2nd layer encryption successfully,please encrypt via BSD later", MsgBoxStyle.OkOnly, null);
		}

		// Token: 0x06000035 RID: 53 RVA: 0x000024AC File Offset: 0x000006AC
		private void AboutToolStripMenuItem_Click(object sender, EventArgs e)
		{
			Class2.Class3_0.Form2.Show();
		}

		// Token: 0x06000036 RID: 54 RVA: 0x000024BD File Offset: 0x000006BD
		private void DonateToolStripMenuItem_Click(object sender, EventArgs e)
		{
			Process.Start("https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=2VF66NZ2ZEYWS");
		}

		// Token: 0x0400000A RID: 10
		private IContainer icontainer_0;

		// Token: 0x0400000B RID: 11
		[AccessedThroughProperty("MenuStrip1")]
		private MenuStrip _MenuStrip1;

		// Token: 0x0400000C RID: 12
		[AccessedThroughProperty("OpenToolStripMenuItem")]
		private ToolStripMenuItem _OpenToolStripMenuItem;

		// Token: 0x0400000D RID: 13
		[AccessedThroughProperty("Button1")]
		private Button _Button1;

		// Token: 0x0400000E RID: 14
		[AccessedThroughProperty("Button2")]
		private Button ppFotdQad;

		// Token: 0x0400000F RID: 15
		[AccessedThroughProperty("PictureBox1")]
		private PictureBox _PictureBox1;

		// Token: 0x04000010 RID: 16
		[AccessedThroughProperty("AboutToolStripMenuItem")]
		private ToolStripMenuItem _AboutToolStripMenuItem;

		// Token: 0x04000011 RID: 17
		[AccessedThroughProperty("DonateToolStripMenuItem")]
		private ToolStripMenuItem EeyxXkpxI;

		// Token: 0x04000012 RID: 18
		private string string_0;
	}
}
