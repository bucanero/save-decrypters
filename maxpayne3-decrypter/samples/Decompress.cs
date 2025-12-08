/*
var ms = new MemoryStream();

Mp3_DecompressData(cData, cData.Length, 0x000000000000c7bc, ms); // the target buffer should never be longer than 0xC800
*/

private static int Mp3_DecompressData(byte[] data, int len, int maxlen, MemoryStream outstream)
{
    var op = new EndianIO(outstream, EndianType.BigEndian, true);
    var ip = new EndianReader(data, EndianType.BigEndian);
    
    int ip_end = len, a = ip.ReadByte(), b = ip.ReadByte(), err = 0;
    int c = Global.ROTL32(b, 8), e, f, m_pos;
    
    a += c + 2;
    if (a == len)
    {
        while (ip.BaseStream.Position < ip_end)
        {
            a = ip.ReadByte();
            m_pos = (int)ip.BaseStream.Position + 1;
            
            for (int d = 0; d < 8 && ip.BaseStream.Position < ip_end; d++)
            {
                c = a & 0x80;
                e = a & 0xFF;
                
                if (c == 0x00 && m_pos < ip_end)
                {
                    a = ip.ReadByte();
                    f = (a >> 4) | Global.ROTL32(ip.SeekNPeekByte(m_pos), 4);
                    c = (int)(op.Stream.Position - f) - 1;
                    
                    if (c >= 0)
                    {
                        a = (a & 0xF) + 3;
                        ip.BaseStream.Position++;
                        
                        while (a-- > 0 && a <= maxlen)
                            op.Out.WriteByte(op.In.SeekNPeekByte(c++));
                    }
                }
                else if (op.Stream.Position < maxlen)
                {
                    op.Out.WriteByte(ip.ReadByte());
                    m_pos++;
                }
                
                a = (e << 1) & 0xFE;
            }
        }
    }
    
    err = (int)op.Stream.Position;
    ip.Close();
    op.Stream.Flush();
    
    return err;
}
