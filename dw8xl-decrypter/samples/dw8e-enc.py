# https://www.tapatalk.com/groups/koeiwarriors/dw8e-modding-efforts-t17446-s10.html
# http://pastebin.com/gMMGfRvd

def encrypt():
    filedata = get_data(sys.argv[2])
    if sys.argv[2].find('SystemSave') != -1:
        xor = 0x14082801
        byte = 0
        byte2 = 0
        filedata = bytearray(filedata)
        i = 0
        while i < len(filedata)-2:
            byte = (byte + filedata[i]) & 0xFF
            byte2 = (byte2 + filedata[i+1]) & 0xFF
            xor = ((xor * 0x41C64E6D) + 0x3039) & 0xFFFFFFFF
            xor2 = xor
            xor = (xor * 0x41C64E6D) & 0xFFFFFFFF
            xor2 = xor2 >> 16
            filedata[i] = filedata[i] ^ (xor2 & 0xFF)
            xor = (xor + 0x3039) & 0xFFFFFFFF
            xor2 = xor >> 16
            filedata[i+1] = filedata[i+1] ^ (xor2 & 0xFF)
            i += 2
            
        byte2 = (byte2 + byte + filedata[i]) & 0xFF

        if i < len(filedata):
            xor = (xor * 0x41C64E6D) & 0xFFFFFFFF
            xor2 = (xor + 0x3039) & 0xFFFFFFFF
            xor2 = xor2 >> 16
            filedata[i] = filedata[i] ^ (xor2 & 0xFF)

        filedata = ''.join(map(chr,filedata)) + chr(byte2)

    newfiledata = ''
    
    word1 = 0
    word2 = 0
    i = 0
    pos = 0
    while i < (len(filedata) >> 2):
        word1 = (word1 + struct.unpack('<H',filedata[pos:pos+2])[0]) & 0xFFFF
        word2 = (word2 + struct.unpack('<H',filedata[pos+2:pos+4])[0]) & 0xFFFF
        i += 1
        pos += 4

    final = word1 + word2
        
    xor = 0x6d87
    i = 0
    pos = 0
    while i < (len(filedata) >> 2):
        xor = ((xor * 0x5B1A7851) + 0xCE4E) & 0xFFFFFFFF
        xor = ((xor * 0x5B1A7851) + 0xCE4E) & 0xFFFFFFFF
        xor = ((xor * 0x5B1A7851) + 0xCE4E) & 0xFFFFFFFF
        newfiledata += struct.pack('<I',struct.unpack('<I',filedata[pos:pos+4])[0] ^ xor)
        i += 1
        pos += 4

    newfiledata = struct.pack('<I',0x0) + struct.pack('<I',0x0) + (struct.pack('<I',0x0)*0x100) + \
                  struct.pack('<H',final & 0xFFFF) + struct.pack('<H',0x6d87) + newfiledata

    filename = sys.argv[2]
    if filename.find('.') != -1:
        filename = filename.split('.',1)[0]
    if filename.find('_') != -1:
        filename = filename.split('_',1)[0]
    outfile = open(filename + '_encrypted.bin','wb')
    outfile.write(newfiledata)
    outfile.close()
    
    filedata = None
    newfiledata = None
    
    return
