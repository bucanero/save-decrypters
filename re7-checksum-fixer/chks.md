```C
__int64 __fastcall checksum(__int64 in, int size)
{
  int v2; // r9d
  int v3; // r8d
  int v4; // r10d
  int *v5; // rdx
  int v6; // eax
  int v7; // edx
  int v8; // ecx
  int v9; // eax
  unsigned int v10; // ecx

  v2 = -1;
  v3 = size / 4;
  if ( (unsigned int)(size + 3) >= 7 )
  {
    v2 = -1;
    v4 = size / 4;
    v5 = (int *)(in + 4 * (size / 4) + 4LL * (size / -4));
    do
    {
      v6 = *v5++;
      --v4;
      v2 = 5 * __ROL4__(v2 ^ (461845907 * ((380141568 * v6) | ((unsigned int)(-862048943 * v6) >> 17))), 13) - 430675100;
    }
    while ( v4 );
  }
  v7 = 0;
  v8 = size & 3;
  switch ( v8 )
  {
    case 2:
      goto LABEL_8;
    case 1:
LABEL_9:
      v9 = v7 ^ *(unsigned __int8 *)(in + 4LL * v3);
      v2 ^= 461845907 * ((380141568 * v9) | ((unsigned int)(-862048943 * v9) >> 17));
      break;
    case 3:
      v7 = *(unsigned __int8 *)(in + 4LL * v3 + 2) << 16;
LABEL_8:
      v7 |= *(unsigned __int8 *)(in + 4LL * v3 + 1) << 8;
      goto LABEL_9;
  }
  v10 = -1028477387
      * ((-2048144789 * (size ^ v2 ^ ((size ^ (unsigned int)v2) >> 16))) ^ ((-2048144789
                                                                           * (size ^ v2 ^ ((size ^ (unsigned int)v2) >> 16))) >> 13));
  return v10 ^ HIWORD(v10);
}
```