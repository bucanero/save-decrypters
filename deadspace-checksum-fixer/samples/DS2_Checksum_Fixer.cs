using ns0;
using ns2;
using System;
using System.ComponentModel;
using System.Drawing;
using System.IO;
using System.Resources;
using System.Windows.Forms;

namespace ns1
{
    internal sealed class MainForm : Form
    {
        private IContainer icontainer_0 = null;

        private Button FixChecksumButton;

        private Label label1;

        public MainForm()
        {
            this.InitializeComponent();
        }

        private unsafe void FixChecksumButton_Click(object sender, EventArgs e)
        {
            void* voidPointer = stackalloc void[17];
            FolderBrowserDialog folderBrowserDialog = new FolderBrowserDialog();
            *(voidPointer + 16) = folderBrowserDialog.ShowDialog() != DialogResult.OK;
            if ((sbyte)(*(voidPointer + 16)) == 0)
            {
                try
                {
                    Class0 class0 = new Class0(string.Concat(folderBrowserDialog.SelectedPath, "\\HED-DATA"), Enum0.const_0, true);
                    class0.vmethod_7((long)0);
                    *voidPointer = (uint)class0.vmethod_4();
                    class0.vmethod_7((long)4);
                    class0.method_4().Write(0);
                    class0.vmethod_7((long)108);
                    *(voidPointer + 4) = class0.method_2().ReadUInt32();
                    class0.vmethod_7((long)0);
                    byte[] numArray = class0.method_2().method_0((int)((uint)(*voidPointer)));
                    *(voidPointer + 8) = MainForm.smethod_0(numArray, 0);
                    Class0 class01 = new Class0(string.Concat(folderBrowserDialog.SelectedPath, "\\USR-DATA"), Enum0.const_0, true);
                    class01.vmethod_7((long)0);
                    byte[] numArray1 = class01.method_2().method_0((int)((uint)(*(voidPointer + 4))));
                    *(voidPointer + 12) = MainForm.smethod_0(numArray1, (uint)(*(voidPointer + 8)));
                    MessageBox.Show(string.Concat("0x", (*(voidPointer + 12)).ToString("X")));
                    class01.vmethod_6();
                    class0.vmethod_7((long)4);
                    class0.method_4().Write((uint)(*(voidPointer + 12)));
                    class0.vmethod_6();
                    MessageBox.Show("Calculated The Checksum Successfully!", "Success", MessageBoxButtons.OK, MessageBoxIcon.Asterisk);
                }
                catch (Exception exception1)
                {
                    Exception exception = exception1;
                    MessageBox.Show(string.Concat("Error: ", exception.Message));
                }
            }
        }

        private void InitializeComponent()
        {
            ComponentResourceManager componentResourceManager = new ComponentResourceManager(typeof(MainForm));
            this.FixChecksumButton = new Button();
            this.label1 = new Label();
            base.SuspendLayout();
            this.FixChecksumButton.Location = new Point(12, 12);
            this.FixChecksumButton.Name = "FixChecksumButton";
            this.FixChecksumButton.Size = new Size(0x104, 49);
            this.FixChecksumButton.TabIndex = 0;
            this.FixChecksumButton.Text = "Fix Checksum";
            this.FixChecksumButton.UseVisualStyleBackColor = true;
            this.FixChecksumButton.Click += new EventHandler(this.FixChecksumButton_Click);
            this.label1.AutoSize = true;
            this.label1.Location = new Point(12, 64);
            this.label1.Name = "label1";
            this.label1.Size = new Size(121, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "Made By: Red-EyeX32";
            base.AutoScaleDimensions = new SizeF(6f, 13f);
            base.AutoScaleMode = AutoScaleMode.Font;
            base.ClientSize = new Size(0x11c, 86);
            base.Controls.Add(this.label1);
            base.Controls.Add(this.FixChecksumButton);
            this.Font = new Font("Consolas", 8.25f, FontStyle.Regular, GraphicsUnit.Point, 0);
            base.Icon = (Icon)componentResourceManager.GetObject("$this.Icon");
            base.MaximizeBox = false;
            base.MinimizeBox = false;
            base.Name = "MainForm";
            base.StartPosition = FormStartPosition.CenterScreen;
            this.Text = "Dead Space 2 Checksum Fixer";
            base.ResumeLayout(false);
            base.PerformLayout();
        }

        public static unsafe uint smethod_0(byte[] byte_0, uint uint_0 = 0)
        {
            void* uint0 = stackalloc void[25];
            *uint0 = (long)0;
            *(uint0 + 8) = (ulong)uint_0;
            *(uint0 + 16) = null;
            while (true)
            {
                *(uint0 + 24) = (int)(*(uint0 + 16)) <= (int)byte_0.Length - 1;
                if ((sbyte)(*(uint0 + 24)) == 0)
                {
                    break;
                }
                *uint0 = (ulong)byte_0[(int)(*(uint0 + 16))];
                *(uint0 + 8) = Convert.ToInt64((long)(*(uint0 + 8))) * (long)0x1003f;
                *(uint0 + 8) = (long)(*(uint0 + 8)) + (long)(*uint0);
                *(uint0 + 8) = (long)BitConverter.ToInt32(BitConverter.GetBytes((long)(*(uint0 + 8))), 0);
                *(uint0 + 16) = (int)(*(uint0 + 16)) + 1;
            }
            *(uint0 + 20) = BitConverter.ToUInt32(BitConverter.GetBytes((long)(*(uint0 + 8))), 0);
            return (uint)(*(uint0 + 20));
        }

        protected override void System.Windows.Forms.Form.Dispose(bool disposing)
        {
            if ((!disposing ? false : this.icontainer_0 != null))
            {
                this.icontainer_0.Dispose();
            }
            base.Dispose(disposing);
        }
    }
}
