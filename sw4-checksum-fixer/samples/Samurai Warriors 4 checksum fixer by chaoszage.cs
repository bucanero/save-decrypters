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
			Class14.ybVLygozAZwCZ();
			base..ctor();
			base.Load += this.Form1_Load;
			this.InitializeComponent();
		}

		// Token: 0x06000020 RID: 32 RVA: 0x00006B50 File Offset: 0x00004D50
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

		// Token: 0x06000021 RID: 33 RVA: 0x00006B90 File Offset: 0x00004D90
		[DebuggerStepThrough]
		private void InitializeComponent()
		{
			ComponentResourceManager componentResourceManager = new ComponentResourceManager(typeof(Form1));
			this.MenuStrip1 = new MenuStrip();
			this.OpenToolStripMenuItem = new ToolStripMenuItem();
			this.AboutToolStripMenuItem = new ToolStripMenuItem();
			this.DonateToolStripMenuItem = new ToolStripMenuItem();
			this.PictureBox1 = new PictureBox();
			this.Button1 = new Button();
			this.StatusStrip1 = new StatusStrip();
			this.ToolStripStatusLabel1 = new ToolStripStatusLabel();
			this.MenuStrip1.SuspendLayout();
			((ISupportInitialize)this.PictureBox1).BeginInit();
			this.StatusStrip1.SuspendLayout();
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
			Size size = new Size(411, 24);
			menuStrip2.Size = size;
			this.MenuStrip1.TabIndex = 1;
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
			this.PictureBox1.BorderStyle = BorderStyle.FixedSingle;
			this.PictureBox1.Image = Class6.smethod_0();
			Control pictureBox = this.PictureBox1;
			location = new Point(67, 39);
			pictureBox.Location = location;
			this.PictureBox1.Name = "PictureBox1";
			Control pictureBox2 = this.PictureBox1;
			size = new Size(268, 99);
			pictureBox2.Size = size;
			this.PictureBox1.TabIndex = 2;
			this.PictureBox1.TabStop = false;
			Control button = this.Button1;
			location = new Point(149, 156);
			button.Location = location;
			this.Button1.Name = "Button1";
			Control button2 = this.Button1;
			size = new Size(117, 26);
			button2.Size = size;
			this.Button1.TabIndex = 4;
			this.Button1.Text = "Fix Checksum";
			this.Button1.UseVisualStyleBackColor = true;
			this.StatusStrip1.Items.AddRange(new ToolStripItem[]
			{
				this.ToolStripStatusLabel1
			});
			Control statusStrip = this.StatusStrip1;
			location = new Point(0, 195);
			statusStrip.Location = location;
			this.StatusStrip1.Name = "StatusStrip1";
			Control statusStrip2 = this.StatusStrip1;
			size = new Size(411, 22);
			statusStrip2.Size = size;
			this.StatusStrip1.TabIndex = 5;
			this.StatusStrip1.Text = "StatusStrip1";
			this.ToolStripStatusLabel1.Name = "ToolStripStatusLabel1";
			ToolStripItem toolStripStatusLabel = this.ToolStripStatusLabel1;
			size = new Size(236, 17);
			toolStripStatusLabel.Size = size;
			this.ToolStripStatusLabel1.Text = "US/EU/JP save are compatible with this tool";
			SizeF autoScaleDimensions = new SizeF(6f, 13f);
			this.AutoScaleDimensions = autoScaleDimensions;
			this.AutoScaleMode = AutoScaleMode.Font;
			size = new Size(411, 217);
			this.ClientSize = size;
			this.Controls.Add(this.StatusStrip1);
			this.Controls.Add(this.Button1);
			this.Controls.Add(this.PictureBox1);
			this.Controls.Add(this.MenuStrip1);
			this.HelpButton = true;
			this.Icon = (Icon)componentResourceManager.GetObject("$this.Icon");
			this.MainMenuStrip = this.MenuStrip1;
			size = new Size(427, 255);
			this.MaximumSize = size;
			size = new Size(427, 255);
			this.MinimumSize = size;
			this.Name = "Form1";
			this.StartPosition = FormStartPosition.CenterScreen;
			this.Text = "Samurai Warriors 4 Checksum Fixer 1.0 by chaoszage";
			this.MenuStrip1.ResumeLayout(false);
			this.MenuStrip1.PerformLayout();
			((ISupportInitialize)this.PictureBox1).EndInit();
			this.StatusStrip1.ResumeLayout(false);
			this.StatusStrip1.PerformLayout();
			this.ResumeLayout(false);
			this.PerformLayout();
		}

		// Token: 0x17000008 RID: 8
		// (get) Token: 0x06000022 RID: 34 RVA: 0x00007034 File Offset: 0x00005234
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
		// (get) Token: 0x06000024 RID: 36 RVA: 0x00007048 File Offset: 0x00005248
		// (set) Token: 0x06000025 RID: 37 RVA: 0x0000705C File Offset: 0x0000525C
		internal virtual ToolStripMenuItem OpenToolStripMenuItem
		{
			get
			{
				return this.iBbIkmjc7;
			}
			[MethodImpl(MethodImplOptions.Synchronized)]
			set
			{
				EventHandler value2 = new EventHandler(this.OpenToolStripMenuItem_Click);
				if (this.iBbIkmjc7 != null)
				{
					this.iBbIkmjc7.Click -= value2;
				}
				this.iBbIkmjc7 = value;
				if (this.iBbIkmjc7 != null)
				{
					this.iBbIkmjc7.Click += value2;
				}
			}
		}

		// Token: 0x1700000A RID: 10
		// (get) Token: 0x06000026 RID: 38 RVA: 0x000070A8 File Offset: 0x000052A8
		// (set) Token: 0x06000027 RID: 39 RVA: 0x000070BC File Offset: 0x000052BC
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
		// (get) Token: 0x06000028 RID: 40 RVA: 0x00007108 File Offset: 0x00005308
		// (set) Token: 0x06000029 RID: 41 RVA: 0x0000711C File Offset: 0x0000531C
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
		// (get) Token: 0x0600002A RID: 42 RVA: 0x00007168 File Offset: 0x00005368
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
		// (get) Token: 0x0600002C RID: 44 RVA: 0x0000717C File Offset: 0x0000537C
		// (set) Token: 0x0600002D RID: 45 RVA: 0x00007190 File Offset: 0x00005390
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

		// Token: 0x1700000E RID: 14
		// (get) Token: 0x0600002E RID: 46 RVA: 0x000071DC File Offset: 0x000053DC
		// (set) Token: 0x0600002F RID: 47 RVA: 0x00002492 File Offset: 0x00000692
		internal virtual StatusStrip StatusStrip1
		{
			get
			{
				return this._StatusStrip1;
			}
			[MethodImpl(MethodImplOptions.Synchronized)]
			set
			{
				this._StatusStrip1 = value;
			}
		}

		// Token: 0x1700000F RID: 15
		// (get) Token: 0x06000030 RID: 48 RVA: 0x000071F0 File Offset: 0x000053F0
		// (set) Token: 0x06000031 RID: 49 RVA: 0x0000249B File Offset: 0x0000069B
		internal virtual ToolStripStatusLabel ToolStripStatusLabel1
		{
			get
			{
				return this._ToolStripStatusLabel1;
			}
			[MethodImpl(MethodImplOptions.Synchronized)]
			set
			{
				this._ToolStripStatusLabel1 = value;
			}
		}

		// Token: 0x06000032 RID: 50 RVA: 0x00007204 File Offset: 0x00005404
		private void OpenToolStripMenuItem_Click(object sender, EventArgs e)
		{
			OpenFileDialog openFileDialog = new OpenFileDialog();
			openFileDialog.Title = "Open a decrypted Samurai Warriors 4 Save";
			openFileDialog.Filter = "DATA.BIN|*DATA.BIN*";
			if (openFileDialog.ShowDialog() == DialogResult.OK)
			{
				this.string_1 = openFileDialog.FileName;
				this.method_0();
				return;
			}
		}

		// Token: 0x06000033 RID: 51 RVA: 0x0000724C File Offset: 0x0000544C
		private void method_0()
		{
			Reader reader = new Reader(this.string_1, Endian.Big, 0L, FileAccess.Read, FileShare.ReadWrite);
			reader.Position = 2L;
			uint num = (uint)reader.method_35();
			if ((ulong)num == 8472UL)
			{
				reader.Position = 361106L;
				uint num2 = reader.method_43();
				if ((ulong)num2 == 0UL)
				{
					this.string_0 = "US/EU";
					this.Button1.Enabled = true;
					Interaction.MsgBox("Loaded " + this.string_0 + " save Successfully", MsgBoxStyle.OkOnly, null);
					this.ToolStripStatusLabel1.Text = "You are using " + this.string_0 + " save";
				}
				else
				{
					this.string_0 = "JP";
					this.Button1.Enabled = true;
					Interaction.MsgBox("Loaded " + this.string_0 + " save Successfully", MsgBoxStyle.OkOnly, null);
					this.ToolStripStatusLabel1.Text = "You are using " + this.string_0 + " save";
				}
			}
			else
			{
				Interaction.MsgBox("Please choose a decrypted Samurai Warriors 4 save", MsgBoxStyle.OkOnly, null);
			}
			reader.Close(true, false);
		}

		// Token: 0x06000034 RID: 52 RVA: 0x000024A4 File Offset: 0x000006A4
		private void Form1_Load(object sender, EventArgs e)
		{
			this.Button1.Enabled = false;
		}

		// Token: 0x06000035 RID: 53 RVA: 0x000024B2 File Offset: 0x000006B2
		private void DonateToolStripMenuItem_Click(object sender, EventArgs e)
		{
			Process.Start("https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=2VF66NZ2ZEYWS");
		}

		// Token: 0x06000036 RID: 54 RVA: 0x000024BF File Offset: 0x000006BF
		private void AboutToolStripMenuItem_Click(object sender, EventArgs e)
		{
			Class2.Class3_0.Form2.Show();
		}

		// Token: 0x06000037 RID: 55 RVA: 0x0000737C File Offset: 0x0000557C
		private void Button1_Click(object sender, EventArgs e)
		{
			Reader reader = new Reader(this.string_1, Endian.Big, 0L, FileAccess.Read, FileShare.ReadWrite);
			Writer writer = new Writer(this.string_1, Endian.Big, 0L, FileMode.Open, FileAccess.Write, FileShare.ReadWrite);
			ulong num = 0UL;
			ulong num2 = 0UL;
			ulong num3 = 0UL;
			ulong num4 = 0UL;
			ulong num5 = 0UL;
			ulong num6 = 0UL;
			uint num7 = 2147483647U;
			if (Operators.CompareString(this.string_0, "JP", false) == 0)
			{
				writer.Position = 4L;
				writer.method_12(0U);
				writer.Position = 168L;
				writer.method_12(0U);
				writer.Position = 3094L;
				writer.method_12(0U);
				writer.Position = 361106L;
				writer.method_12(0U);
				reader.Position = 8L;
				int num8 = 8;
				do
				{
					num2 += (ulong)reader.method_32();
					num8++;
				}
				while (num8 <= 167);
				reader.Position = 3098L;
				num8 = 3098;
				do
				{
					num += (ulong)reader.method_32();
					num8++;
				}
				while (num8 <= 30293);
				reader.Position = 172L;
				num8 = 172;
				do
				{
					num3 += (ulong)reader.method_32();
					num8++;
				}
				while (num8 <= 2283);
				reader.Position = 33630L;
				num8 = 33630;
				do
				{
					num4 += (ulong)reader.method_32();
					num8++;
				}
				while (num8 <= 361105);
				reader.Position = 30294L;
				num8 = 30294;
				do
				{
					num5 += (ulong)reader.method_32();
					num8++;
				}
				while (num8 <= 33630);
				reader.Position = 8L;
				num8 = 8;
				do
				{
					num6 += (ulong)reader.method_32();
					num8++;
				}
				while (num8 <= 30293);
				ulong num9 = num + num2;
				num9 *= num3;
				num9 += num5;
				num9 &= (ulong)num7;
				ulong num10 = num + num3;
				num10 += num4;
				num4 = num2 * num10;
				num4 &= (ulong)num7;
				ulong num11 = num9 + num4 + num6;
				num11 &= (ulong)num7;
				writer.Position = 4L;
				writer.method_12((uint)num9);
				writer.Position = 168L;
				writer.method_12((uint)num4);
				writer.Position = 3094L;
				writer.method_12((uint)num6);
				writer.Position = 361106L;
				writer.method_12((uint)num11);
				reader.Close(true, false);
				writer.Close(true, false);
				Interaction.MsgBox("Checksum has been fixed,please continue encrypt via BSD", MsgBoxStyle.OkOnly, null);
			}
			else
			{
				writer.Position = 4L;
				writer.method_12(0U);
				writer.Position = 168L;
				writer.method_12(0U);
				writer.Position = 3094L;
				writer.method_12(0U);
				writer.Position = 361778L;
				writer.method_12(0U);
				reader.Position = 8L;
				int num8 = 8;
				do
				{
					num2 += (ulong)reader.method_32();
					num8++;
				}
				while (num8 <= 167);
				reader.Position = 3098L;
				num8 = 3098;
				do
				{
					num += (ulong)reader.method_32();
					num8++;
				}
				while (num8 <= 30933);
				reader.Position = 172L;
				num8 = 172;
				do
				{
					num3 += (ulong)reader.method_32();
					num8++;
				}
				while (num8 <= 2283);
				reader.Position = 34270L;
				num8 = 34270;
				do
				{
					num4 += (ulong)reader.method_32();
					num8++;
				}
				while (num8 <= 361777);
				reader.Position = 30934L;
				num8 = 30934;
				do
				{
					num5 += (ulong)reader.method_32();
					num8++;
				}
				while (num8 <= 34270);
				reader.Position = 8L;
				num8 = 8;
				do
				{
					num6 += (ulong)reader.method_32();
					num8++;
				}
				while (num8 <= 30933);
				ulong num9 = num + num2;
				num9 *= num3;
				num9 += num5;
				num9 &= (ulong)num7;
				ulong num10 = num + num3;
				num10 += num4;
				num4 = num2 * num10;
				num4 &= (ulong)num7;
				ulong num11 = num9 + num4 + num6;
				num11 &= (ulong)num7;
				writer.Position = 4L;
				writer.method_12((uint)num9);
				writer.Position = 168L;
				writer.method_12((uint)num4);
				writer.Position = 3094L;
				writer.method_12((uint)num6);
				writer.Position = 361778L;
				writer.method_12((uint)num11);
				reader.Close(true, false);
				writer.Close(true, false);
				Interaction.MsgBox("Checksum has been fixed,please continue encrypt via BSD", MsgBoxStyle.OkOnly, null);
			}
		}

		// Token: 0x0400000A RID: 10
		private IContainer icontainer_0;

		// Token: 0x0400000B RID: 11
		[AccessedThroughProperty("MenuStrip1")]
		private MenuStrip _MenuStrip1;

		// Token: 0x0400000C RID: 12
		[AccessedThroughProperty("OpenToolStripMenuItem")]
		private ToolStripMenuItem iBbIkmjc7;

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
		[AccessedThroughProperty("Button1")]
		private Button _Button1;

		// Token: 0x04000011 RID: 17
		[AccessedThroughProperty("StatusStrip1")]
		private StatusStrip _StatusStrip1;

		// Token: 0x04000012 RID: 18
		[AccessedThroughProperty("ToolStripStatusLabel1")]
		private ToolStripStatusLabel _ToolStripStatusLabel1;

		// Token: 0x04000013 RID: 19
		private string string_0;

		// Token: 0x04000014 RID: 20
		private string string_1;
	}
}
