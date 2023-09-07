// Token: 0x06000078 RID: 120 RVA: 0x00004FA0 File Offset: 0x000031A0
public static byte[] ComputeHash(byte[] Bytes)
{
	int num = 0;
	int num2 = 0;
	for (int i = 0; i < Bytes.Length; i += 2)
	{
		num += ((int)Bytes[i] ^ (i & 255));
		num2 += ((int)Bytes[i + 1] ^ (i + 1 & 255));
	}
	int value = num + num2;
	return BitConverter.GetBytes(value);
}

// Token: 0x06000079 RID: 121 RVA: 0x00004FF0 File Offset: 0x000031F0
public static bool VerifyFile(string filePath)
{
	return File.Exists(filePath) && FileSystem.FileLen(filePath) == 294916L;
}

// Token: 0x0600007A RID: 122 RVA: 0x0000502C File Offset: 0x0000322C
public static bool VerifyHash(string filePath)
{
	bool result;
	if (!SaveGame.VerifyFile(filePath))
	{
		result = false;
	}
	else
	{
		byte[] array = new byte[262144];
		byte[] array2 = new byte[32768];
		byte[] array3 = new byte[4];
		using (FileStream fileStream = new FileStream(filePath, FileMode.Open, FileAccess.Read))
		{
			fileStream.Read(array, 0, array.Length);
			fileStream.Read(array2, 0, array2.Length);
			fileStream.Read(array3, 0, array3.Length);
			fileStream.Close();
		}
		byte[] bytes = Functions.CombineArrays(array, array2);
		result = Functions.CompareBytes(array3, SaveGame.ComputeHash(bytes));
	}
	return result;
}
