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
		// Token: 0x0600001D RID: 29 RVA: 0x0000241B File Offset: 0x0000061B
		public Form1()
		{
			Class14.ynupaLQzEPxLu();
			base..ctor();
			base.Load += this.Form1_Load;
			this.long_0 = new long[1025];
			this.InitializeComponent();
		}

		// Token: 0x0600001E RID: 30 RVA: 0x00006B10 File Offset: 0x00004D10
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

		// Token: 0x0600001F RID: 31 RVA: 0x00006B50 File Offset: 0x00004D50
		[DebuggerStepThrough]
		private void InitializeComponent()
		{
			ComponentResourceManager componentResourceManager = new ComponentResourceManager(typeof(Form1));
			this.MenuStrip1 = new MenuStrip();
			this.OpenToolStripMenuItem = new ToolStripMenuItem();
			this.HowToUseToolStripMenuItem = new ToolStripMenuItem();
			this.AboutToolStripMenuItem = new ToolStripMenuItem();
			this.DonateToolStripMenuItem = new ToolStripMenuItem();
			this.Button1 = new Button();
			this.PictureBox1 = new PictureBox();
			this.Label1 = new Label();
			this.NumericUpDown1 = new NumericUpDown();
			this.MenuStrip1.SuspendLayout();
			((ISupportInitialize)this.PictureBox1).BeginInit();
			((ISupportInitialize)this.NumericUpDown1).BeginInit();
			this.SuspendLayout();
			this.MenuStrip1.Items.AddRange(new ToolStripItem[]
			{
				this.OpenToolStripMenuItem,
				this.HowToUseToolStripMenuItem,
				this.AboutToolStripMenuItem,
				this.DonateToolStripMenuItem
			});
			Control menuStrip = this.MenuStrip1;
			Point location = new Point(0, 0);
			menuStrip.Location = location;
			this.MenuStrip1.Name = "MenuStrip1";
			Control menuStrip2 = this.MenuStrip1;
			Size size = new Size(377, 24);
			menuStrip2.Size = size;
			this.MenuStrip1.TabIndex = 0;
			this.MenuStrip1.Text = "MenuStrip1";
			this.OpenToolStripMenuItem.Name = "OpenToolStripMenuItem";
			ToolStripItem openToolStripMenuItem = this.OpenToolStripMenuItem;
			size = new Size(48, 20);
			openToolStripMenuItem.Size = size;
			this.OpenToolStripMenuItem.Text = "Open";
			this.HowToUseToolStripMenuItem.Name = "HowToUseToolStripMenuItem";
			ToolStripItem howToUseToolStripMenuItem = this.HowToUseToolStripMenuItem;
			size = new Size(80, 20);
			howToUseToolStripMenuItem.Size = size;
			this.HowToUseToolStripMenuItem.Text = "How to Use";
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
			location = new Point(126, 278);
			button.Location = location;
			this.Button1.Name = "Button1";
			Control button2 = this.Button1;
			size = new Size(121, 26);
			button2.Size = size;
			this.Button1.TabIndex = 1;
			this.Button1.Text = "Fix checksum";
			this.Button1.UseVisualStyleBackColor = true;
			this.PictureBox1.BorderStyle = BorderStyle.FixedSingle;
			this.PictureBox1.Image = Class6.smethod_0();
			Control pictureBox = this.PictureBox1;
			location = new Point(60, 27);
			pictureBox.Location = location;
			this.PictureBox1.Name = "PictureBox1";
			Control pictureBox2 = this.PictureBox1;
			size = new Size(257, 194);
			pictureBox2.Size = size;
			this.PictureBox1.TabIndex = 2;
			this.PictureBox1.TabStop = false;
			this.Label1.AutoSize = true;
			Control label = this.Label1;
			location = new Point(234, 241);
			label.Location = location;
			this.Label1.Name = "Label1";
			Control label2 = this.Label1;
			size = new Size(39, 13);
			label2.Size = size;
			this.Label1.TabIndex = 3;
			this.Label1.Text = "Munny";
			Control numericUpDown = this.NumericUpDown1;
			location = new Point(108, 239);
			numericUpDown.Location = location;
			NumericUpDown numericUpDown2 = this.NumericUpDown1;
			decimal maximum = new decimal(new int[]
			{
				999999,
				0,
				0,
				0
			});
			numericUpDown2.Maximum = maximum;
			this.NumericUpDown1.Name = "NumericUpDown1";
			Control numericUpDown3 = this.NumericUpDown1;
			size = new Size(120, 20);
			numericUpDown3.Size = size;
			this.NumericUpDown1.TabIndex = 4;
			this.NumericUpDown1.TextAlign = HorizontalAlignment.Right;
			SizeF autoScaleDimensions = new SizeF(6f, 13f);
			this.AutoScaleDimensions = autoScaleDimensions;
			this.AutoScaleMode = AutoScaleMode.Font;
			size = new Size(377, 316);
			this.ClientSize = size;
			this.Controls.Add(this.NumericUpDown1);
			this.Controls.Add(this.Label1);
			this.Controls.Add(this.PictureBox1);
			this.Controls.Add(this.Button1);
			this.Controls.Add(this.MenuStrip1);
			this.Icon = (Icon)componentResourceManager.GetObject("$this.Icon");
			this.MainMenuStrip = this.MenuStrip1;
			size = new Size(393, 315);
			this.MinimumSize = size;
			this.Name = "Form1";
			this.StartPosition = FormStartPosition.CenterScreen;
			this.Text = "KH 2.5 HD checksum fixer 1.0.0.3 by chaoszage";
			this.MenuStrip1.ResumeLayout(false);
			this.MenuStrip1.PerformLayout();
			((ISupportInitialize)this.PictureBox1).EndInit();
			((ISupportInitialize)this.NumericUpDown1).EndInit();
			this.ResumeLayout(false);
			this.PerformLayout();
		}

		// Token: 0x17000007 RID: 7
		// (get) Token: 0x06000020 RID: 32 RVA: 0x00007068 File Offset: 0x00005268
		// (set) Token: 0x06000021 RID: 33 RVA: 0x00002451 File Offset: 0x00000651
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

		// Token: 0x17000008 RID: 8
		// (get) Token: 0x06000022 RID: 34 RVA: 0x0000707C File Offset: 0x0000527C
		// (set) Token: 0x06000023 RID: 35 RVA: 0x00007090 File Offset: 0x00005290
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

		// Token: 0x17000009 RID: 9
		// (get) Token: 0x06000024 RID: 36 RVA: 0x000070DC File Offset: 0x000052DC
		// (set) Token: 0x06000025 RID: 37 RVA: 0x000070F0 File Offset: 0x000052F0
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

		// Token: 0x1700000A RID: 10
		// (get) Token: 0x06000026 RID: 38 RVA: 0x0000713C File Offset: 0x0000533C
		// (set) Token: 0x06000027 RID: 39 RVA: 0x00007150 File Offset: 0x00005350
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

		// Token: 0x1700000B RID: 11
		// (get) Token: 0x06000028 RID: 40 RVA: 0x0000719C File Offset: 0x0000539C
		// (set) Token: 0x06000029 RID: 41 RVA: 0x000071B0 File Offset: 0x000053B0
		internal virtual ToolStripMenuItem DonateToolStripMenuItem
		{
			get
			{
				return this._DonateToolStripMenuItem;
			}
			[MethodImpl(MethodImplOptions.Synchronized)]
			set
			{
				EventHandler value2 = new EventHandler(this.DonateToolStripMenuItem_Click);
				if (this._DonateToolStripMenuItem != null)
				{
					this._DonateToolStripMenuItem.Click -= value2;
				}
				this._DonateToolStripMenuItem = value;
				if (this._DonateToolStripMenuItem != null)
				{
					this._DonateToolStripMenuItem.Click += value2;
				}
			}
		}

		// Token: 0x1700000C RID: 12
		// (get) Token: 0x0600002A RID: 42 RVA: 0x000071FC File Offset: 0x000053FC
		// (set) Token: 0x0600002B RID: 43 RVA: 0x0000245A File Offset: 0x0000065A
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
		// (get) Token: 0x0600002C RID: 44 RVA: 0x00007210 File Offset: 0x00005410
		// (set) Token: 0x0600002D RID: 45 RVA: 0x00007224 File Offset: 0x00005424
		internal virtual ToolStripMenuItem HowToUseToolStripMenuItem
		{
			get
			{
				return this._HowToUseToolStripMenuItem;
			}
			[MethodImpl(MethodImplOptions.Synchronized)]
			set
			{
				EventHandler value2 = new EventHandler(this.HowToUseToolStripMenuItem_Click);
				if (this._HowToUseToolStripMenuItem != null)
				{
					this._HowToUseToolStripMenuItem.Click -= value2;
				}
				this._HowToUseToolStripMenuItem = value;
				if (this._HowToUseToolStripMenuItem != null)
				{
					this._HowToUseToolStripMenuItem.Click += value2;
				}
			}
		}

		// Token: 0x1700000E RID: 14
		// (get) Token: 0x0600002E RID: 46 RVA: 0x00007270 File Offset: 0x00005470
		// (set) Token: 0x0600002F RID: 47 RVA: 0x00002463 File Offset: 0x00000663
		internal virtual Label Label1
		{
			get
			{
				return this._Label1;
			}
			[MethodImpl(MethodImplOptions.Synchronized)]
			set
			{
				this._Label1 = value;
			}
		}

		// Token: 0x1700000F RID: 15
		// (get) Token: 0x06000030 RID: 48 RVA: 0x00007284 File Offset: 0x00005484
		// (set) Token: 0x06000031 RID: 49 RVA: 0x0000246C File Offset: 0x0000066C
		internal virtual NumericUpDown NumericUpDown1
		{
			get
			{
				return this._NumericUpDown1;
			}
			[MethodImpl(MethodImplOptions.Synchronized)]
			set
			{
				this._NumericUpDown1 = value;
			}
		}

		// Token: 0x06000032 RID: 50 RVA: 0x00002475 File Offset: 0x00000675
		private void Form1_Load(object sender, EventArgs e)
		{
			this.Button1.Enabled = false;
		}

		// Token: 0x06000033 RID: 51 RVA: 0x00007298 File Offset: 0x00005498
		private void Button1_Click(object sender, EventArgs e)
		{
			this.writefile();
			Reader reader = new Reader(this.string_0, Endian.Big, 0L, FileAccess.Read, FileShare.ReadWrite);
			Writer writer = new Writer(this.string_0, Endian.Big, 0L, FileMode.Open, FileAccess.Write, FileShare.ReadWrite);
			this.calc_tab();
			uint num = uint.MaxValue;
			reader.Position = 0L;
			int num2 = 0;
			uint num5;
			do
			{
				uint num3 = num >> 24;
				uint num4 = (uint)reader.method_32();
				num <<= 8;
				num5 = (uint)((ulong)num5 - 1UL);
				num3 ^= num4;
				num3 <<= 2;
				num3 = (uint)this.long_0[(int)num3];
				num ^= num3;
				num2++;
			}
			while (num2 <= 7);
			reader.Position = 12L;
			int num6 = 12;
			do
			{
				uint num3 = num >> 24;
				uint num4 = (uint)reader.method_32();
				num <<= 8;
				num5 = (uint)((ulong)num5 - 1UL);
				num3 ^= num4;
				num3 <<= 2;
				num3 = (uint)this.long_0[(int)num3];
				num ^= num3;
				num6++;
			}
			while (num6 <= 69567);
			num = ~num;
			num = Microsoft.VisualBasic.CompilerServices.Conversions.ToUInteger(this.reversebit(num));
			writer.Position = 8L;
			writer.method_12(num);
			reader.Close(true, false);
			writer.Close(true, false);
			Interaction.MsgBox("Checksum has been fixed,please encrypt via BSD later", MsgBoxStyle.OkOnly, null);
		}

		// Token: 0x06000034 RID: 52 RVA: 0x000073E8 File Offset: 0x000055E8
		private void OpenToolStripMenuItem_Click(object sender, EventArgs e)
		{
			OpenFileDialog openFileDialog = new OpenFileDialog();
			openFileDialog.Title = "Open a decrypted KH 2.5 HD Save";
			openFileDialog.Filter = "USR-*|USR-*";
			if (openFileDialog.ShowDialog() == DialogResult.OK)
			{
				this.string_0 = openFileDialog.FileName;
				this.method_0();
				return;
			}
		}

		// Token: 0x06000035 RID: 53 RVA: 0x00007430 File Offset: 0x00005630
		private void method_0()
		{
			Reader reader = new Reader(this.string_0, Endian.Big, 0L, FileAccess.Read, FileShare.ReadWrite);
			reader.Position = 0L;
			uint num = reader.method_43();
			reader.Close(true, false);
			if ((ulong)num == 1263022666UL)
			{
				Interaction.MsgBox("Loaded Successfully", MsgBoxStyle.OkOnly, null);
				this.Button1.Enabled = true;
				this.readfile();
			}
			else
			{
				this.Button1.Enabled = false;
				Interaction.MsgBox("Please choose a decrypted KH 2.5 HD save", MsgBoxStyle.OkOnly, null);
			}
		}

		// Token: 0x06000036 RID: 54 RVA: 0x000074BC File Offset: 0x000056BC
		public void readfile()
		{
			Reader reader = new Reader(this.string_0, Endian.Big, 0L, FileAccess.Read, FileShare.ReadWrite);
			reader.Position = 9280L;
			uint num = reader.method_43();
			num = Microsoft.VisualBasic.CompilerServices.Conversions.ToUInteger(this.reversebit(num));
			this.NumericUpDown1.Value = new decimal(num);
			reader.Close(true, false);
		}

		// Token: 0x06000037 RID: 55 RVA: 0x00007524 File Offset: 0x00005724
		public void writefile()
		{
			Writer writer = new Writer(this.string_0, Endian.Big, 0L, FileMode.Open, FileAccess.Write, FileShare.ReadWrite);
			Reader reader = new Reader(this.string_0, Endian.Big, 0L, FileAccess.Read, FileShare.ReadWrite);
			reader.Position = 9280L;
			uint num = Convert.ToUInt32(this.NumericUpDown1.Value);
			num = Microsoft.VisualBasic.CompilerServices.Conversions.ToUInteger(this.reversebit(num));
			writer.Position = 9280L;
			writer.method_12(num);
			reader.Close(true, false);
			writer.Close(true, false);
		}

		// Token: 0x06000038 RID: 56 RVA: 0x000075BC File Offset: 0x000057BC
		public object reversebit(object value)
		{
			uint num = Microsoft.VisualBasic.CompilerServices.Conversions.ToUInteger(Operators.AndObject(Operators.LeftShiftObject(value, 24), 4278190080U));
			uint num2 = Microsoft.VisualBasic.CompilerServices.Conversions.ToUInteger(Operators.AndObject(Operators.LeftShiftObject(value, 8), 16711680U));
			uint num3 = Microsoft.VisualBasic.CompilerServices.Conversions.ToUInteger(Operators.AndObject(Operators.RightShiftObject(value, 8), 65280U));
			uint num4 = Microsoft.VisualBasic.CompilerServices.Conversions.ToUInteger(Operators.AndObject(Operators.RightShiftObject(value, 24), 255U));
			uint num5 = num + num2 + num3 + num4;
			return num5;
		}

		// Token: 0x06000039 RID: 57 RVA: 0x00007660 File Offset: 0x00005860
		public void calc_tab()
		{
			uint num = 0U;
			uint num2 = 0U;
			uint num3 = 0U;
			uint num4 = 0U;
			do
			{
				uint num5 = num2;
				uint num6 = num3;
				do
				{
					uint num7 = num6 & 2147483648U;
					num6 <<= 1;
					if ((ulong)num7 != 0UL)
					{
						num6 ^= 79764919U;
					}
					num5 = (uint)((ulong)num5 + 1UL);
				}
				while ((ulong)num5 < 255UL);
				num5 = num4;
				num = (uint)((ulong)num + 1UL);
				num4 = (uint)((ulong)num4 + 4UL);
				num3 += 16777216U;
				this.long_0[(int)num5] = (long)((ulong)num6);
			}
			while ((ulong)num <= 255UL);
		}

		// Token: 0x0600003A RID: 58 RVA: 0x00002483 File Offset: 0x00000683
		private void AboutToolStripMenuItem_Click(object sender, EventArgs e)
		{
			Interaction.MsgBox("Reversed,Coded,Designed by chaoszage\r\nSpecial Thanks:\r\nchaoszage's mother for everything\r\nDark_Nacho_XD for VB tutorials\r\nRed-EyeX32 for reverse engineering tips and guides\r\nfeuldenate for PackageIO dll", MsgBoxStyle.OkOnly, null);
		}

		// Token: 0x0600003B RID: 59 RVA: 0x00002492 File Offset: 0x00000692
		private void DonateToolStripMenuItem_Click(object sender, EventArgs e)
		{
			Process.Start("https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=2VF66NZ2ZEYWS");
		}

		// Token: 0x0600003C RID: 60 RVA: 0x0000249F File Offset: 0x0000069F
		private void HowToUseToolStripMenuItem_Click(object sender, EventArgs e)
		{
			Interaction.MsgBox("How to Use:\r\n1.Use BSD to Decrypt PFD(All Files)\r\n2.Cheats - Apply Cheats\r\n3.Message pops saying x cheats applied\r\n4.Use this tool to fix checksum and close the program once checksum applied\r\n5.Switch to BSD and update PFD(Partial)(All Files)\r\n6.Encrypt PFD(All Files)\r\n7.Verify PFD", MsgBoxStyle.OkOnly, null);
		}

		// Token: 0x04000009 RID: 9
		private IContainer icontainer_0;

		// Token: 0x0400000A RID: 10
		[AccessedThroughProperty("MenuStrip1")]
		private MenuStrip _MenuStrip1;

		// Token: 0x0400000B RID: 11
		[AccessedThroughProperty("OpenToolStripMenuItem")]
		private ToolStripMenuItem _OpenToolStripMenuItem;

		// Token: 0x0400000C RID: 12
		[AccessedThroughProperty("Button1")]
		private Button _Button1;

		// Token: 0x0400000D RID: 13
		[AccessedThroughProperty("AboutToolStripMenuItem")]
		private ToolStripMenuItem _AboutToolStripMenuItem;

		// Token: 0x0400000E RID: 14
		[AccessedThroughProperty("DonateToolStripMenuItem")]
		private ToolStripMenuItem _DonateToolStripMenuItem;

		// Token: 0x0400000F RID: 15
		[AccessedThroughProperty("PictureBox1")]
		private PictureBox _PictureBox1;

		// Token: 0x04000010 RID: 16
		[AccessedThroughProperty("HowToUseToolStripMenuItem")]
		private ToolStripMenuItem _HowToUseToolStripMenuItem;

		// Token: 0x04000011 RID: 17
		[AccessedThroughProperty("Label1")]
		private Label _Label1;

		// Token: 0x04000012 RID: 18
		[AccessedThroughProperty("NumericUpDown1")]
		private NumericUpDown _NumericUpDown1;

		// Token: 0x04000013 RID: 19
		private string string_0;

		// Token: 0x04000014 RID: 20
		private long[] long_0;
	}
}
