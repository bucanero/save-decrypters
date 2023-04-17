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
			Class14.UlES5juzO3XxY();
			base..ctor();
			base.Load += this.Form1_Load;
			this.InitializeComponent();
		}

		// Token: 0x06000020 RID: 32 RVA: 0x00006B6C File Offset: 0x00004D6C
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

		// Token: 0x06000021 RID: 33 RVA: 0x00006BAC File Offset: 0x00004DAC
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
			this.Button2 = new Button();
			this.CheckedListBox1 = new CheckedListBox();
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
			Size size = new Size(377, 24);
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
			this.PictureBox1.BorderStyle = BorderStyle.FixedSingle;
			this.PictureBox1.Image = (Image)componentResourceManager.GetObject("PictureBox1.Image");
			Control pictureBox = this.PictureBox1;
			location = new Point(12, 27);
			pictureBox.Location = location;
			this.PictureBox1.Name = "PictureBox1";
			Control pictureBox2 = this.PictureBox1;
			size = new Size(352, 116);
			pictureBox2.Size = size;
			this.PictureBox1.TabIndex = 1;
			this.PictureBox1.TabStop = false;
			this.Button1.Font = new Font("Microsoft Sans Serif", 13f, FontStyle.Regular, GraphicsUnit.Point, 0);
			Control button = this.Button1;
			location = new Point(12, 158);
			button.Location = location;
			this.Button1.Name = "Button1";
			Control button2 = this.Button1;
			size = new Size(127, 34);
			button2.Size = size;
			this.Button1.TabIndex = 4;
			this.Button1.Text = "Decrypt";
			this.Button1.UseVisualStyleBackColor = true;
			this.Button2.Font = new Font("Microsoft Sans Serif", 13f, FontStyle.Regular, GraphicsUnit.Point, 0);
			Control button3 = this.Button2;
			location = new Point(237, 158);
			button3.Location = location;
			this.Button2.Name = "Button2";
			Control button4 = this.Button2;
			size = new Size(127, 34);
			button4.Size = size;
			this.Button2.TabIndex = 5;
			this.Button2.Text = "Encrypt";
			this.Button2.UseVisualStyleBackColor = true;
			this.CheckedListBox1.Font = new Font("Microsoft Sans Serif", 12f, FontStyle.Regular, GraphicsUnit.Point, 0);
			this.CheckedListBox1.FormattingEnabled = true;
			this.CheckedListBox1.Items.AddRange(new object[]
			{
				"Max Ampoules",
				"Max First Aid Kits",
				"Max Health Drinks",
				"Unlock Beam Saber",
				"Unlock Flamethrower",
				"Unlock Unlimited Submachine Gun",
				"Unlock Handgun with Fully Loaded",
				"Unlock Shotgun with Fully Loaded",
				"Unlock Stun Gun with Fully Loaded",
				"Unlock Submachine Gun with Fully Loaded",
				"Max Handgun Bullets(if never owned)",
				"Max Shotgun Shells(if never owned)",
				"Max Stun Gun Batteries(if never owned)",
				"Max Submachine Gun Bullets(if never owned)",
				"Max Handgun Bullets(if already owned)",
				"Max Shotgun Shells(if already owned)",
				"Max Stun Gun Batteries(if already owned)",
				"Max Submachine Gun Bullets(if already owned)"
			});
			Control checkedListBox = this.CheckedListBox1;
			location = new Point(12, 214);
			checkedListBox.Location = location;
			this.CheckedListBox1.Name = "CheckedListBox1";
			Control checkedListBox2 = this.CheckedListBox1;
			size = new Size(352, 172);
			checkedListBox2.Size = size;
			this.CheckedListBox1.TabIndex = 6;
			SizeF autoScaleDimensions = new SizeF(6f, 13f);
			this.AutoScaleDimensions = autoScaleDimensions;
			this.AutoScaleMode = AutoScaleMode.Font;
			size = new Size(377, 412);
			this.ClientSize = size;
			this.Controls.Add(this.CheckedListBox1);
			this.Controls.Add(this.Button2);
			this.Controls.Add(this.Button1);
			this.Controls.Add(this.PictureBox1);
			this.Controls.Add(this.MenuStrip1);
			this.Icon = (Icon)componentResourceManager.GetObject("$this.Icon");
			this.MainMenuStrip = this.MenuStrip1;
			size = new Size(393, 450);
			this.MaximumSize = size;
			size = new Size(393, 450);
			this.MinimumSize = size;
			this.Name = "Form1";
			this.StartPosition = FormStartPosition.CenterScreen;
			this.Text = "Silent Hill 3 HD Save Editor 2.2 by chaoszage";
			this.MenuStrip1.ResumeLayout(false);
			this.MenuStrip1.PerformLayout();
			((ISupportInitialize)this.PictureBox1).EndInit();
			this.ResumeLayout(false);
			this.PerformLayout();
		}

		// Token: 0x17000008 RID: 8
		// (get) Token: 0x06000022 RID: 34 RVA: 0x00007170 File Offset: 0x00005370
		// (set) Token: 0x06000023 RID: 35 RVA: 0x00002480 File Offset: 0x00000680
		internal virtual MenuStrip MenuStrip1
		{
			get
			{
				return this.ksTtgqSin;
			}
			[MethodImpl(MethodImplOptions.Synchronized)]
			set
			{
				this.ksTtgqSin = value;
			}
		}

		// Token: 0x17000009 RID: 9
		// (get) Token: 0x06000024 RID: 36 RVA: 0x00007184 File Offset: 0x00005384
		// (set) Token: 0x06000025 RID: 37 RVA: 0x00007198 File Offset: 0x00005398
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
		// (get) Token: 0x06000026 RID: 38 RVA: 0x000071E4 File Offset: 0x000053E4
		// (set) Token: 0x06000027 RID: 39 RVA: 0x000071F8 File Offset: 0x000053F8
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
		// (get) Token: 0x06000028 RID: 40 RVA: 0x00007244 File Offset: 0x00005444
		// (set) Token: 0x06000029 RID: 41 RVA: 0x00007258 File Offset: 0x00005458
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
		// (get) Token: 0x0600002A RID: 42 RVA: 0x000072A4 File Offset: 0x000054A4
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
		// (get) Token: 0x0600002C RID: 44 RVA: 0x000072B8 File Offset: 0x000054B8
		// (set) Token: 0x0600002D RID: 45 RVA: 0x000072CC File Offset: 0x000054CC
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
		// (get) Token: 0x0600002E RID: 46 RVA: 0x00007318 File Offset: 0x00005518
		// (set) Token: 0x0600002F RID: 47 RVA: 0x0000732C File Offset: 0x0000552C
		internal virtual Button Button2
		{
			get
			{
				return this._Button2;
			}
			[MethodImpl(MethodImplOptions.Synchronized)]
			set
			{
				EventHandler value2 = new EventHandler(this.Button2_Click);
				if (this._Button2 != null)
				{
					this._Button2.Click -= value2;
				}
				this._Button2 = value;
				if (this._Button2 != null)
				{
					this._Button2.Click += value2;
				}
			}
		}

		// Token: 0x1700000F RID: 15
		// (get) Token: 0x06000030 RID: 48 RVA: 0x00007378 File Offset: 0x00005578
		// (set) Token: 0x06000031 RID: 49 RVA: 0x00002492 File Offset: 0x00000692
		internal virtual CheckedListBox CheckedListBox1
		{
			get
			{
				return this._CheckedListBox1;
			}
			[MethodImpl(MethodImplOptions.Synchronized)]
			set
			{
				this._CheckedListBox1 = value;
			}
		}

		// Token: 0x06000032 RID: 50 RVA: 0x0000249B File Offset: 0x0000069B
		private void Form1_Load(object sender, EventArgs e)
		{
			this.Button1.Enabled = false;
			this.Button2.Enabled = false;
			this.CheckedListBox1.Enabled = false;
		}

		// Token: 0x06000033 RID: 51 RVA: 0x0000738C File Offset: 0x0000558C
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

		// Token: 0x06000034 RID: 52 RVA: 0x000073D4 File Offset: 0x000055D4
		private void method_0()
		{
			Reader reader = new Reader(this.string_0, Endian.Big, 0L, FileAccess.Read, FileShare.ReadWrite);
			Writer writer = new Writer(this.string_0, Endian.Big, 0L, FileMode.Open, FileAccess.Write, FileShare.ReadWrite);
			reader.Position = 64L;
			uint num = reader.method_43();
			if ((ulong)num == 2693972695UL)
			{
				this.Button1.Enabled = true;
				this.Button2.Enabled = false;
				this.CheckedListBox1.Enabled = false;
				Interaction.MsgBox("Save is opened successfully", MsgBoxStyle.OkOnly, null);
			}
			else if ((ulong)num == 67176193UL)
			{
				this.Button1.Enabled = false;
				this.Button2.Enabled = true;
				this.CheckedListBox1.Enabled = true;
				Interaction.MsgBox("Save is opened successfully", MsgBoxStyle.OkOnly, null);
			}
			else
			{
				Interaction.MsgBox("Please choose a decrypted Silent Hill 3 save", MsgBoxStyle.OkOnly, null);
			}
			reader.Close(true, false);
			writer.Close(true, false);
		}

		// Token: 0x06000035 RID: 53 RVA: 0x000024C1 File Offset: 0x000006C1
		private void AboutToolStripMenuItem_Click(object sender, EventArgs e)
		{
			Class2.Class3_0.Form2.Show();
		}

		// Token: 0x06000036 RID: 54 RVA: 0x000024D2 File Offset: 0x000006D2
		private void DonateToolStripMenuItem_Click(object sender, EventArgs e)
		{
			Process.Start("https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=2VF66NZ2ZEYWS");
		}

		// Token: 0x06000037 RID: 55 RVA: 0x000024DF File Offset: 0x000006DF
		private void EqahlijFG(object sender, LinkLabelLinkClickedEventArgs e)
		{
			Process.Start("http://bruteforcesavedata.forumms.net");
		}

		// Token: 0x06000038 RID: 56 RVA: 0x000074C8 File Offset: 0x000056C8
		private void Button1_Click(object sender, EventArgs e)
		{
			Reader reader = new Reader(this.string_0, Endian.Big, 0L, FileAccess.Read, FileShare.ReadWrite);
			Writer writer = new Writer(this.string_0, Endian.Big, 0L, FileMode.Open, FileAccess.Write, FileShare.ReadWrite);
			uint maxValue = uint.MaxValue;
			ulong num = 1533819434UL;
			ulong num2 = 4294967296UL;
			writer.Position = 64L;
			reader.Position = 64L;
			int num3 = 0;
			do
			{
				writer.Position = writer.Position;
				ulong num4 = (ulong)reader.method_43();
				ulong num5 = num2 - num;
				ulong num6 = num4 ^ num5;
				num6 &= (ulong)maxValue;
				uint value = Convert.ToUInt32(num6);
				writer.method_12(value);
				num2 = (num4 << 5 | num4 >> 27);
				num2 = Convert.ToUInt64(decimal.Add(new decimal(num2), 4294967296m));
				num3 += 4;
			}
			while (num3 <= 5404);
			writer.Position = 16L;
			writer.method_12(0U);
			reader.Close(true, false);
			writer.Close(true, false);
			Interaction.MsgBox("Decrypted 2nd layer encryption Successfully", MsgBoxStyle.OkOnly, null);
			this.Button1.Enabled = false;
			this.Button2.Enabled = true;
			this.CheckedListBox1.Enabled = true;
			uint num7 = 0U;
			do
			{
				this.CheckedListBox1.SetItemChecked((int)num7, false);
				num7 += 1U;
			}
			while (num7 <= 17U);
		}

		// Token: 0x06000039 RID: 57 RVA: 0x00007664 File Offset: 0x00005864
		private void Button2_Click(object sender, EventArgs e)
		{
			Reader reader = new Reader(this.string_0, Endian.Big, 0L, FileAccess.Read, FileShare.ReadWrite);
			Writer writer = new Writer(this.string_0, Endian.Big, 0L, FileMode.Open, FileAccess.Write, FileShare.ReadWrite);
			ulong num = 0UL;
			uint maxValue = uint.MaxValue;
			ulong num2 = 1533819434UL;
			ulong num3 = 4294967296UL;
			int num4 = 0;
			do
			{
				if (this.CheckedListBox1.GetItemChecked(num4))
				{
					switch (num4)
					{
					case 0:
					{
						writer.Position = 5398L;
						writer.method_4(999);
						reader.Position = 5345L;
						uint num5 = (uint)reader.method_32();
						num5 = (uint)((ulong)num5 + 16UL);
						if ((ulong)num5 >= 254UL)
						{
							num5 = 254U;
						}
						writer.Position = 5345L;
						writer.method_1((byte)num5);
						break;
					}
					case 1:
					{
						writer.Position = 5396L;
						writer.method_4(999);
						reader.Position = 5345L;
						uint num5 = (uint)reader.method_32();
						num5 = (uint)((ulong)num5 + 8UL);
						if ((ulong)num5 >= 254UL)
						{
							num5 = 254U;
						}
						writer.Position = 5345L;
						writer.method_1((byte)num5);
						break;
					}
					case 2:
					{
						writer.Position = 5394L;
						writer.method_4(999);
						reader.Position = 5345L;
						uint num5 = (uint)reader.method_32();
						num5 = (uint)((ulong)num5 + 4UL);
						if ((ulong)num5 >= 254UL)
						{
							num5 = 254U;
						}
						writer.Position = 5345L;
						writer.method_1((byte)num5);
						break;
					}
					case 3:
					{
						reader.Position = 5347L;
						uint num5 = (uint)reader.method_32();
						num5 = (uint)((ulong)num5 + 32UL);
						if ((ulong)num5 >= 254UL)
						{
							num5 = 254U;
						}
						writer.Position = 5347L;
						writer.method_1((byte)num5);
						break;
					}
					case 4:
					{
						reader.Position = 5347L;
						uint num5 = (uint)reader.method_32();
						num5 = (uint)((ulong)num5 + 64UL);
						if ((ulong)num5 >= 254UL)
						{
							num5 = 254U;
						}
						writer.Position = 5347L;
						writer.method_1((byte)num5);
						break;
					}
					case 5:
					{
						writer.Position = 5384L;
						writer.method_4(1);
						reader.Position = 5346L;
						uint num5 = (uint)reader.method_32();
						num5 = (uint)((ulong)num5 + 32UL);
						if ((ulong)num5 >= 254UL)
						{
							num5 = 254U;
						}
						writer.Position = 5346L;
						writer.method_1((byte)num5);
						break;
					}
					case 6:
					{
						writer.Position = 5378L;
						writer.method_4(10);
						reader.Position = 5346L;
						uint num5 = (uint)reader.method_32();
						num5 = (uint)((ulong)num5 + 4UL);
						if ((ulong)num5 >= 254UL)
						{
							num5 = 254U;
						}
						writer.Position = 5346L;
						writer.method_1((byte)num5);
						break;
					}
					case 7:
					{
						writer.Position = 5380L;
						writer.method_4(6);
						reader.Position = 5346L;
						uint num5 = (uint)reader.method_32();
						num5 = (uint)((ulong)num5 + 8UL);
						if ((ulong)num5 >= 254UL)
						{
							num5 = 254U;
						}
						writer.Position = 5346L;
						writer.method_1((byte)num5);
						break;
					}
					case 8:
					{
						writer.Position = 5376L;
						writer.method_4(4);
						reader.Position = 5346L;
						uint num5 = (uint)reader.method_32();
						num5 = (uint)((ulong)num5 + 2UL);
						if ((ulong)num5 >= 254UL)
						{
							num5 = 254U;
						}
						writer.Position = 5346L;
						writer.method_1((byte)num5);
						break;
					}
					case 9:
					{
						writer.Position = 5382L;
						writer.method_4(32);
						reader.Position = 5346L;
						uint num5 = (uint)reader.method_32();
						num5 = (uint)((ulong)num5 + 16UL);
						if ((ulong)num5 >= 254UL)
						{
							num5 = 254U;
						}
						writer.Position = 5346L;
						writer.method_1((byte)num5);
						break;
					}
					case 10:
					{
						writer.Position = 5388L;
						writer.method_4(999);
						reader.Position = 5346L;
						uint num5 = (uint)reader.method_32();
						num5 = (uint)((ulong)num5 + 128UL);
						if ((ulong)num5 >= 254UL)
						{
							num5 = 254U;
						}
						writer.Position = 5346L;
						writer.method_1((byte)num5);
						break;
					}
					case 11:
					{
						writer.Position = 5390L;
						writer.method_4(999);
						reader.Position = 5345L;
						uint num5 = (uint)reader.method_32();
						num5 = (uint)((ulong)num5 + 1UL);
						if ((ulong)num5 >= 254UL)
						{
							num5 = 254U;
						}
						writer.Position = 5345L;
						writer.method_1((byte)num5);
						break;
					}
					case 12:
					{
						writer.Position = 5386L;
						writer.method_4(999);
						reader.Position = 5346L;
						uint num5 = (uint)reader.method_32();
						num5 = (uint)((ulong)num5 + 64UL);
						if ((ulong)num5 >= 254UL)
						{
							num5 = 254U;
						}
						writer.Position = 5346L;
						writer.method_1((byte)num5);
						break;
					}
					case 13:
					{
						writer.Position = 5392L;
						writer.method_4(999);
						reader.Position = 5345L;
						uint num5 = (uint)reader.method_32();
						num5 = (uint)((ulong)num5 + 2UL);
						if ((ulong)num5 >= 254UL)
						{
							num5 = 254U;
						}
						writer.Position = 5345L;
						writer.method_1((byte)num5);
						break;
					}
					case 14:
						writer.Position = 5388L;
						writer.method_4(999);
						break;
					case 15:
						writer.Position = 5390L;
						writer.method_4(999);
						break;
					case 16:
						writer.Position = 5386L;
						writer.method_4(999);
						break;
					case 17:
						writer.Position = 5392L;
						writer.method_4(999);
						break;
					}
				}
				num4++;
			}
			while (num4 <= 17);
			reader.Position = 0L;
			int num6 = 0;
			do
			{
				num += (ulong)reader.method_43();
				num6 += 4;
			}
			while (num6 <= 5471);
			num &= (ulong)maxValue;
			num = Convert.ToUInt64(decimal.Multiply(new decimal(num), 2m));
			num &= (ulong)maxValue;
			uint value = Convert.ToUInt32(decimal.Add(new decimal(num), 1m));
			writer.Position = 16L;
			writer.method_12(value);
			writer.Position = 64L;
			reader.Position = 64L;
			int num7 = 0;
			do
			{
				writer.Position = writer.Position;
				num = (ulong)reader.method_43();
				ulong num8 = num3 - num2;
				ulong num9 = num ^ num8;
				num9 &= (ulong)maxValue;
				uint num10 = Convert.ToUInt32(num9);
				writer.method_12(num10);
				num3 = (ulong)(num10 << 5 | num10 >> 27);
				num3 = Convert.ToUInt64(decimal.Add(new decimal(num3), 4294967296m));
				num7 += 4;
			}
			while (num7 <= 5404);
			reader.Close(true, false);
			writer.Close(true, false);
			Interaction.MsgBox("Cheats applied(if any) and encrypted 2nd layer encryption Successfully", MsgBoxStyle.OkOnly, null);
			this.Button1.Enabled = false;
			this.Button2.Enabled = false;
			this.OpenToolStripMenuItem.Enabled = true;
			this.CheckedListBox1.Enabled = false;
		}

		// Token: 0x0400000A RID: 10
		private IContainer icontainer_0;

		// Token: 0x0400000B RID: 11
		[AccessedThroughProperty("MenuStrip1")]
		private MenuStrip ksTtgqSin;

		// Token: 0x0400000C RID: 12
		[AccessedThroughProperty("OpenToolStripMenuItem")]
		private ToolStripMenuItem _OpenToolStripMenuItem;

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
		[AccessedThroughProperty("Button2")]
		private Button _Button2;

		// Token: 0x04000012 RID: 18
		[AccessedThroughProperty("CheckedListBox1")]
		private CheckedListBox _CheckedListBox1;

		// Token: 0x04000013 RID: 19
		private string string_0;
	}
}
