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

/*

PPC:

_XOR:
li %r11, 0 # Load Immediate
cmplwi cr6, %r4, 0 # Compare Logical Word Immediate
beq cr6, $$$$ # Branch if equal

mtctr %r4 # Move to count register

[Loop]
$loop: # Load Byte and Zero Indexed
lbzx %r10, %r11, %r3
addi %r11, %r11, 1 # Add Immediate
extsb %r10, %r10 # Extend Sign Byte
add %r10, %r10, %r5 # Add
slwi %r9, %r10, 10 # Shift Left Immediate
add %r10, %r9, %r10 # Add
srwi %r9, %r10, 6 # Shift Right Immediate
xor %r5, %r9, %r10 # XOR
bdnz $loop # CTR--; branch if CTR non-zero

[End]
$$$$: # Shift Left Immediate
slwi %r11, %r5, 3
add %r11, %r5, %r11 # Add
srwi %r10, %r11, 11 # Shift Right Immediate
xor %r11, %r10, %r11 # XOR
slwi %r10, %r11, 15 # Shift Left Immediate
add %r3, %r10, %r11 # Add
blr # Branch unconditionally
# End of function _XOR

Credit to Team Avalon
PPC Analysis: Homura
Reversed by Team Avalon
Members include: Chaos/Homura, XPGOBYTO/Raiden x, Dippidip/Vanquish.

*/
