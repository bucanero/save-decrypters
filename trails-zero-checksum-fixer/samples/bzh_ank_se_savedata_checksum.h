#ifndef BZH_ANK_SE_SAVEDATA_CHECKSUM_H
#define BZH_ANK_SE_SAVEDATA_CHECKSUM_H

#include "bzh_ank_se_define.h"

bool savedata_file_head_size_verify(QString savedata_file_path,bool user_savedata_or_system_savedata)
{
    QFile savedata_checksum_file;
    savedata_checksum_file.setFileName(savedata_file_path);
    savedata_checksum_file.open(savedata_checksum_file.ReadOnly);

    QDataStream savedata_checksum_file_stream(&savedata_checksum_file);
    savedata_checksum_file_stream.setVersion(savedata_checksum_file_stream.Qt_5_0);
    savedata_checksum_file_stream.setByteOrder(savedata_checksum_file_stream.LittleEndian);

    quint64 savedata_file_size = savedata_checksum_file.size()-0x01;

    quint64 savedata_file_head1,savedata_file_head2;
    savedata_checksum_file.seek(0x00);
    savedata_checksum_file_stream >> savedata_file_head1;
    savedata_checksum_file.seek(0x08);
    savedata_checksum_file_stream >> savedata_file_head2;

    //为真认为是用户存档,为假认为是系统存档
    if(user_savedata_or_system_savedata)
    {
        if((savedata_file_size == 0x0002643b) && (savedata_file_head1 == 0xbf80000000000000) && (savedata_file_head2 == 0x00))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        if((savedata_file_size == 0x0000050b) && (savedata_file_head1 == 0x00) && (savedata_file_head2 == 0x600000000 || savedata_file_head2 == 0x100000000))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

QVector<quint32> savedata_checksum_make(QString savedata_file_path)
{
    QFile savedata_checksum_file;
    savedata_checksum_file.setFileName(savedata_file_path);
    savedata_checksum_file.open(savedata_checksum_file.ReadOnly);

    QDataStream savedata_checksum_file_stream(&savedata_checksum_file);
    savedata_checksum_file_stream.setVersion(savedata_checksum_file_stream.Qt_5_0);
    savedata_checksum_file_stream.setByteOrder(savedata_checksum_file_stream.LittleEndian);

    //计算开始
    quint64 pos = 0x00;
    quint64 file_size_max_pos = savedata_checksum_file.size()-0x01;

    quint32 file_savedata_checksum = 0;
    quint32 file_size_checksum = -1 * (((file_size_max_pos - 0x08) / 0x04) + 0x01);
    quint32 cache;

    while(pos <= (file_size_max_pos - 0x08))
    {
        savedata_checksum_file.seek(pos);
        savedata_checksum_file_stream >> cache;
        file_savedata_checksum += cache;
        pos += 0x04;
    }

    file_size_checksum -= file_savedata_checksum;
    //计算完毕

    savedata_checksum_file.close();

    //截取
    file_size_checksum -= 0xFFFFFFFF00000000;

    QVector<quint32> return_checksum(2);
    return_checksum[0] = file_size_checksum;
    return_checksum[1] = file_savedata_checksum;
    return return_checksum;
}

QVector<quint32> savedata_checksum_patch(QString savedata_file_path,bool user_savedata_or_system_savedata)
{
    QVector<quint32> checksum(2);
    checksum = savedata_checksum_make(savedata_file_path);

    quint64 offset_size_checksum,offset_savedata_checksum;
    if(user_savedata_or_system_savedata)
    {
        offset_size_checksum = offset_user_savedata_size_checksum;
        offset_savedata_checksum = offset_user_savedata_checksum;
    }
    else
    {
        offset_size_checksum = offset_system_savedata_size_checksum;
        offset_savedata_checksum = offset_system_savedata_checksum;
    }

    savedata_binary_write_quint32(savedata_file_path,offset_size_checksum,checksum[0]);
    savedata_binary_write_quint32(savedata_file_path,offset_savedata_checksum,checksum[1]);

    return checksum;
}

bool savedata_checksum_verify(QString savedata_file_path, bool user_savedata_or_system_savedata)
{
    QVector<quint32> checksum(2);
    checksum = savedata_checksum_make(savedata_file_path);

    quint64 offset_size_checksum,offset_savedata_checksum;
    if(user_savedata_or_system_savedata)
    {
        offset_size_checksum = offset_user_savedata_size_checksum;
        offset_savedata_checksum = offset_user_savedata_checksum;
    }
    else
    {
        offset_size_checksum = offset_system_savedata_size_checksum;
        offset_savedata_checksum = offset_system_savedata_checksum;
    }

    quint32 size_checksum = savedata_binary_read_quint32(savedata_file_path,offset_size_checksum);
    quint32 savedata_checksum = savedata_binary_read_quint32(savedata_file_path,offset_savedata_checksum);

    if(size_checksum == checksum[0] && savedata_checksum == checksum[1])
    {
        return true;
    }
    else
    {
        return false;
    }
}

#endif // BZH_ANK_SE_SAVEDATA_CHECKSUM_H
