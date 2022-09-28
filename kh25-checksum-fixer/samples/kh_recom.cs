// WindowsApplication1.Form1
// Token: 0x0600002F RID: 47 RVA: 0x000070D0 File Offset: 0x000052D0
private void Button1_Click(object sender, EventArgs e)
{
	Reader reader = new Reader(this.string_0, Endian.Big, 0L, FileAccess.Read, FileShare.ReadWrite);
	Writer writer = new Writer(this.string_0, Endian.Big, 0L, FileMode.Open, FileAccess.Write, FileShare.ReadWrite);
	uint num = 13856U;
	uint num2 = 0U;
	uint num3 = uint.MaxValue;
	uint num4 = 0U;
	uint num5 = 79764919U;
	reader.Position = 16L;
	do
	{
		uint num6 = (uint)reader.method_32();
		uint num7 = num4;
		num6 <<= 31;
		num3 ^= num6;
		do
		{
			num6 = (num3 & 2147483648U);
			num3 <<= 1;
			if ((ulong)num6 != 0UL)
			{
				num3 ^= num5;
			}
			num7 = (uint)((ulong)num7 + 1UL);
		}
		while ((ulong)num7 < 1UL);
		num2 = (uint)((ulong)num2 + 1UL);
	}
	while (num2 < num);
	num = ~num3;
	writer.Position = 4L;
	writer.method_12(num);
	reader.Close(true, false);
	writer.Close(true, false);
	Interaction.MsgBox("Checksum fixed,please close this tool and encrypt via BSD later", MsgBoxStyle.OkOnly, null);
}
