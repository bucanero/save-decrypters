# https://www.tapatalk.com/groups/koeiwarriors/dw8e-modding-efforts-t17446-s10.html
# https://pastebin.com/D16WYiRX

def decrypt():
    filedata = get_data(sys.argv[2])
    newfiledata = ''
    xor = struct.unpack('<H',filedata[0x40a:0x40c])[0]
    i = 0x40c
    while i < len(filedata):
        xor = ((xor * 0x5B1A7851) + 0xCE4E) & 0xFFFFFFFF
        xor = ((xor * 0x5B1A7851) + 0xCE4E) & 0xFFFFFFFF
        xor = ((xor * 0x5B1A7851) + 0xCE4E) & 0xFFFFFFFF
        newfiledata += struct.pack('<I',struct.unpack('<I',filedata[i:i+4])[0] ^ xor)
        i += 0x4

    i = 0
    word1 = 0
    word2 = 0
    while i < len(newfiledata):
        word1 = (word1 + struct.unpack('<H',newfiledata[i:i+2])[0]) & 0xFFFF
        word2 = (word2 + struct.unpack('<H',newfiledata[i+2:i+4])[0]) & 0xFFFF
        i += 4

    final = word1 + word2 # final & 0xFFFF should equal first word of the savedata, hash check I suppose
    
    xor = 0x14082801
    byte = 0
    byte2 = 0
    newfiledata = bytearray(newfiledata)
    i = 0
    while i < len(newfiledata)-2:
        xor = ((xor * 0x41C64E6D) + 0x3039) & 0xFFFFFFFF
        xor2 = xor
        xor = (xor * 0x41C64E6D) & 0xFFFFFFFF
        xor2 = xor2 >> 16
        newfiledata[i] = newfiledata[i] ^ (xor2 & 0xFF)
        xor = (xor + 0x3039) & 0xFFFFFFFF
        byte = (byte + newfiledata[i]) & 0xFF
        xor2 = xor >> 16
        newfiledata[i+1] = newfiledata[i+1] ^ (xor2 & 0xFF)
        byte2 = (byte2 + newfiledata[i+1]) & 0xFF
        i += 2

    if i < len(newfiledata):
        xor = (xor * 0x41C64E6D) & 0xFFFFFFFF
        xor2 = (xor + 0x3039) & 0xFFFFFFFF
        xor2 = xor2 >> 16
        newfiledata[i] = newfiledata[i] ^ (xor2 & 0xFF)

    byte2 = (byte2 + byte + newfiledata[i]) & 0xFF # another hash, should equal the last byte of the file

    outfile = open('save_decrypted.bin','wb')
    outfile.write(newfiledata[:-1])
    outfile.close()
