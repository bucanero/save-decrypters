// https://www.xpgamesaves.com/threads/gtav-by-team-avalon.83585/
namespace GTA5Hash
{
	public class GTAV
	{
		public long Fix(byte[] r3)
		{
			uint num2;
			long num3;
			long num4 = 0x3FAC7125;
			long length = r3.Length;
			for (num2 = 0; num2 < length; num2++)
			{
				num3 = (sbyte)r3[num2];
				num3 += num4;
				uint num = ((uint)num3) << 10;
				num3 = num + num3;
				num = ((uint)num3) >> 6;
				num4 = num ^ num3;
			}
			num2 = (uint)(num4 << 3);
			num2 = ((uint)num4) + num2;
			num3 = num2 >> 11;
			num2 = ((uint)num3) ^ num2;
			num3 = num2 << 15;
			return (num3 + num2);
		}
	}
}
