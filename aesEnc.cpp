/**
 * 数据串加密程序，其第一个参数为输入串，第二个参数为密钥，第三个参数加解密选项，值为e时是加密，为d时是解密
 */
#include<stdio.h>
#include"aes.h"

//将32位字符串转化为16字节数组
void str32ToByte16(char *inStr, unsigned char *in,int *error);
//将16字节数组转化为32位字符串
void byte16ToStr32(unsigned char *in, char *out);

int main(int argc, char **argv)
{
	if (argc != 4)
	{
		return -1;
	}

	char *inStr = argv[1];
	char *keyStr = argv[2];

	char *method = argv[3];

	char outStr[40];

	unsigned char in[16];
	unsigned char key[16];
	unsigned char out[16];
	unsigned char w[176];
    
	int error = 0;
    str32ToByte16(inStr, in, &error);

	str32ToByte16(keyStr, key, &error);

    //密钥扩展
	keyExpansion(key, w);
	
    //打印扩展后的密钥
	for (int i = 0; i < 44; i++)
	{
		printf("w[%d]: ", i);
		for (int j = 0; j < 4; j++)
		{
			printf("%02x ",w[i * 4 + j]);
		}
		printf("<br>");
	}

    printf("\n");

    //加密或解密
	if (method[0] == 'e')
	{
	    Cipher(in, out, w, 1);
	}
	else if (method[0] == 'd')
	{
		InvCipher(in, out, w, 1);
	}

    //将输出转化为32位字符串
	byte16ToStr32(out, outStr);

	printf("%s", outStr);

	return 0;
}


void str32ToByte16(char *inStr, unsigned char *in,int *error)
{
    unsigned char tmpCh = 0;
    *error = 0;

    for (int i = 0; i < 32; i++)
    {
        char ch = inStr[i];
        if (ch >= '0' && ch <= '9')
        {
            tmpCh <<= 4ll;
            tmpCh |= ch - '0';
        }
        else if (ch >= 'a' && ch <= 'f')
        {
            tmpCh <<= 4ll;
            tmpCh |= ch - 'a' + 10;
        }
        else if (ch >= 'A' && ch <= 'F')
        {
            tmpCh <<= 4ll;
            tmpCh |= ch - 'A' + 10;
        }
        else
        {
            //wrong input
            *error = 1;
            return;
        }

        if (i % 2 == 1)
        {
            in[i / 2] = tmpCh;
            tmpCh = 0;
        }
    }
}

void byte16ToStr32(unsigned char *in, char *out)
{
    unsigned char ch0;
    unsigned char ch1;
    for (int i = 0; i < 16; i++)
    {
        ch0 = (in[i] >> 4) & 0x0f;
        ch1 = in[i] & 0x0f;

        if (ch0 < 10)
        {
            out[i * 2] = '0' + ch0;
        }
        else
        {
            out[i * 2] =  'a' + ch0 - 10;
        }

        if (ch1 < 10)
        {
            out[i * 2 + 1] = '0' + ch1;
        }
        else
        {
            out[i * 2 + 1] = 'a' + ch1 - 10;
        }
    }

	out[32] = 0;
}
