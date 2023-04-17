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
			Class14.hu04NYZz38M2i();
			base..ctor();
			base.Load += this.Form1_Load;
			this.InitializeComponent();
		}

		// Token: 0x06000020 RID: 32 RVA: 0x00006B3C File Offset: 0x00004D3C
		[DebuggerNonUserCode]
		protected override void Dispose(bool disposing)
		{
			try
			{
				if (disposing && this.RipEqOhUB != null)
				{
					this.RipEqOhUB.Dispose();
				}
			}
			finally
			{
				base.Dispose(disposing);
			}
		}

		// Token: 0x06000021 RID: 33 RVA: 0x00006B7C File Offset: 0x00004D7C
		[DebuggerStepThrough]
		private void InitializeComponent()
		{
			ComponentResourceManager componentResourceManager = new ComponentResourceManager(typeof(Form1));
			this.MenuStrip1 = new MenuStrip();
			this.OpenToolStripMenuItem = new ToolStripMenuItem();
			this.AboutToolStripMenuItem = new ToolStripMenuItem();
			this.DonateToolStripMenuItem = new ToolStripMenuItem();
			this.Button1 = new Button();
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
			Size size = new Size(392, 24);
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
			location = new Point(116, 195);
			button.Location = location;
			this.Button1.Name = "Button1";
			Control button2 = this.Button1;
			size = new Size(150, 40);
			button2.Size = size;
			this.Button1.TabIndex = 1;
			this.Button1.Text = "Fix Checksum";
			this.Button1.UseVisualStyleBackColor = true;
			this.PictureBox1.BorderStyle = BorderStyle.FixedSingle;
			this.PictureBox1.Image = Class6.smethod_0();
			Control pictureBox = this.PictureBox1;
			location = new Point(43, 27);
			pictureBox.Location = location;
			this.PictureBox1.Name = "PictureBox1";
			Control pictureBox2 = this.PictureBox1;
			size = new Size(306, 149);
			pictureBox2.Size = size;
			this.PictureBox1.TabIndex = 3;
			this.PictureBox1.TabStop = false;
			SizeF autoScaleDimensions = new SizeF(6f, 13f);
			this.AutoScaleDimensions = autoScaleDimensions;
			this.AutoScaleMode = AutoScaleMode.Font;
			size = new Size(392, 247);
			this.ClientSize = size;
			this.Controls.Add(this.PictureBox1);
			this.Controls.Add(this.Button1);
			this.Controls.Add(this.MenuStrip1);
			this.Icon = (Icon)componentResourceManager.GetObject("$this.Icon");
			this.MainMenuStrip = this.MenuStrip1;
			size = new Size(408, 285);
			this.MaximumSize = size;
			size = new Size(408, 285);
			this.MinimumSize = size;
			this.Name = "Form1";
			this.StartPosition = FormStartPosition.CenterScreen;
			this.Text = "Tears to Tiara 2 checksum fixer 1.1 by chaoszage";
			this.MenuStrip1.ResumeLayout(false);
			this.MenuStrip1.PerformLayout();
			((ISupportInitialize)this.PictureBox1).EndInit();
			this.ResumeLayout(false);
			this.PerformLayout();
		}

		// Token: 0x17000008 RID: 8
		// (get) Token: 0x06000022 RID: 34 RVA: 0x00006F18 File Offset: 0x00005118
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
		// (get) Token: 0x06000024 RID: 36 RVA: 0x00006F2C File Offset: 0x0000512C
		// (set) Token: 0x06000025 RID: 37 RVA: 0x00006F40 File Offset: 0x00005140
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
		// (get) Token: 0x06000026 RID: 38 RVA: 0x00006F8C File Offset: 0x0000518C
		// (set) Token: 0x06000027 RID: 39 RVA: 0x00006FA0 File Offset: 0x000051A0
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
		// (get) Token: 0x06000028 RID: 40 RVA: 0x00006FEC File Offset: 0x000051EC
		// (set) Token: 0x06000029 RID: 41 RVA: 0x00007000 File Offset: 0x00005200
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

		// Token: 0x1700000C RID: 12
		// (get) Token: 0x0600002A RID: 42 RVA: 0x0000704C File Offset: 0x0000524C
		// (set) Token: 0x0600002B RID: 43 RVA: 0x00007060 File Offset: 0x00005260
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

		// Token: 0x1700000D RID: 13
		// (get) Token: 0x0600002C RID: 44 RVA: 0x000070AC File Offset: 0x000052AC
		// (set) Token: 0x0600002D RID: 45 RVA: 0x00002489 File Offset: 0x00000689
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

		// Token: 0x0600002E RID: 46 RVA: 0x000070C0 File Offset: 0x000052C0
		private void OpenToolStripMenuItem_Click(object sender, EventArgs e)
		{
			OpenFileDialog openFileDialog = new OpenFileDialog();
			openFileDialog.Title = "Open your Save";
			openFileDialog.Filter = "DAT|*.DAT*";
			if (openFileDialog.ShowDialog() == DialogResult.OK)
			{
				this.string_0 = openFileDialog.FileName;
				this.method_0();
				return;
			}
		}

		// Token: 0x0600002F RID: 47 RVA: 0x00007108 File Offset: 0x00005308
		private void method_0()
		{
			Reader reader = new Reader(this.string_0, Endian.Big, 0L, FileAccess.Read, FileShare.ReadWrite);
			reader.Position = 16L;
			uint num = reader.method_43();
			if ((ulong)num == 1195461957UL)
			{
				Interaction.MsgBox("Loaded Successfully", MsgBoxStyle.OkOnly, null);
				this.Button1.Enabled = true;
			}
			else
			{
				Interaction.MsgBox("Please choose a decrypted Tears to Tiara 2 save", MsgBoxStyle.OkOnly, null);
			}
			reader.Close(true, false);
		}

		// Token: 0x06000030 RID: 48 RVA: 0x00007180 File Offset: 0x00005380
		private void Button1_Click(object sender, EventArgs e)
		{
			Reader reader = new Reader(this.string_0, Endian.Big, 0L, FileAccess.Read, FileShare.ReadWrite);
			Writer writer = new Writer(this.string_0, Endian.Big, 0L, FileMode.Open, FileAccess.Write, FileShare.ReadWrite);
			ulong num = 13352UL;
			ulong num2 = 1UL;
			uint maxValue = uint.MaxValue;
			reader.Position = 8L;
			int num3 = 0;
			do
			{
				ulong num4 = (ulong)reader.method_32();
				num += num4;
				num4 = num2 * num;
				num4 += num;
				num2 = num4;
				num3++;
			}
			while (num3 <= 13351);
			num2 &= (ulong)maxValue;
			writer.Position = 0L;
			writer.method_12((uint)num);
			writer.Position = 4L;
			writer.method_12((uint)num2);
			reader.Close(true, false);
			writer.Close(true, false);
			Interaction.MsgBox("Checksum has been fixed,please encrypt via BSD later", MsgBoxStyle.OkOnly, null);
		}

		// Token: 0x06000031 RID: 49 RVA: 0x00002492 File Offset: 0x00000692
		private void AboutToolStripMenuItem_Click(object sender, EventArgs e)
		{
			Class2.Class3_0.Form2.Show();
		}

		// Token: 0x06000032 RID: 50 RVA: 0x000024A3 File Offset: 0x000006A3
		private void DonateToolStripMenuItem_Click(object sender, EventArgs e)
		{
			Process.Start("https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=2VF66NZ2ZEYWS");
		}

		// Token: 0x06000033 RID: 51 RVA: 0x000024B0 File Offset: 0x000006B0
		private void Form1_Load(object sender, EventArgs e)
		{
			this.Button1.Enabled = false;
		}

		// Token: 0x0400000A RID: 10
		private IContainer RipEqOhUB;

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
		[AccessedThroughProperty("AboutToolStripMenuItem")]
		private ToolStripMenuItem _AboutToolStripMenuItem;

		// Token: 0x0400000F RID: 15
		[AccessedThroughProperty("DonateToolStripMenuItem")]
		private ToolStripMenuItem _DonateToolStripMenuItem;

		// Token: 0x04000010 RID: 16
		[AccessedThroughProperty("PictureBox1")]
		private PictureBox _PictureBox1;

		// Token: 0x04000011 RID: 17
		private string string_0;
	}
}
