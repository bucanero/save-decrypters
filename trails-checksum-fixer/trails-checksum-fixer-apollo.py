import apollo
import uhashlib

def reverseBits(n: int) -> int:
    """
    Reverse the bits of a 32-bit unsigned integer.
    """
    result = 0
  
    for i in range(32):
        result |= (n & 1) << (31 - i)
        n >>= 1
      
    return result

def loh_trails_crc32(savedata, start_off):
    """Returns the custom CRC32 for LoH Trails games"""
    crc_init = reverseBits(len(savedata) - start_off)
    print("CRC Init: 0x{:08X}".format(crc_init))

    trails_crc = uhashlib.crc(savedata[start_off:], uhashlib.CRC_32_BITS, 0x04C11DB7, crc_init, 0, 1, 1)
    apollo.endian_swap(trails_crc)

    return trails_crc
