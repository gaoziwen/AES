/**
 * 加密细节运算程序，
 * 当有2个参数时，第一个参数为输入串，第二个参数为运算方式，值为0时是SubBytes，值为1时是ShiftRows，值为2时是MixColumns，
 * 当有3个参数时，第一个参数为输入串，第二个参数为密钥，第三个参数为运算方式，其值只能为4，表明是AddRoundKey运算
 */
#include<stdio.h>
#include"aes.h"

void str32ToByte16(char *inStr, unsigned char *in,int *error);
void byte16ToStr32(unsigned char *in, char *out);

int main(int argc, char **argv)
{
	char *inStr;
	char *keyStr;
	char *method;
	if (argc == 3)
	{
		inStr = argv[1];
		method = argv[2];
	}
	else if (argc == 4)
	{
		inStr = argv[1];
		keyStr = argv[2];
		method = argv[3];
	}

	char outStr[40];

	unsigned char in[16];
	unsigned char out[16];
	unsigned char key[16];
    
	int error = 0;
    str32ToByte16(inStr, in, &error);
	str32ToByte16(keyStr, key, &error);

	if (method[0] == '0')//SubBytes
	{
		subBytes(in);
	}
	else if (method[0] == '1')//ShiftRows
	{
		for (int i = 0; i < 4; i++)
			for(int j = 0; j < 4; j++)
			{
				out[i * 4 + j] = in[j * 4 + i];
			}
		shiftRows(out);
		for (int i = 0; i < 4; i++)
			for(int j = 0; j < 4; j++)
			{
				in[i * 4 + j] = out[j * 4 + i];
			}
	}
	else if (method[0] == '2')//MixColumns
	{
		for (int i = 0; i < 4; i++)
			for(int j = 0; j < 4; j++)
			{
				out[i * 4 + j] = in[j * 4 + i];
			}
		mixColumns(out);
		for (int i = 0; i < 4; i++)
			for(int j = 0; j < 4; j++)
			{
				in[i * 4 + j] = out[j * 4 + i];
			}
	}
	else if (method[0] == '4')//AddRoundKey
	{
		for (int i = 0; i < 16; i++)
		{
			in[i] ^= key[i];
		}
	}

	byte16ToStr32(in, outStr);

    //打印输出结果
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
