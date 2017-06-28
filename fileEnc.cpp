/**
 * 文件加密程序，其第一个参数为输入文件名，第二个参数为输出文件名，第三个参数为密钥，第四个参事为加解密选项，值为e时是加密，为d时是解密
 */
#include <stdio.h>
#include "aes.h"

//加密文件函数
void file_encrypt(const char *inPath, const char *outPath, unsigned char *key, int *error);
//解密文件函数
void file_decrypt(const char *inPath, const char *outPath, unsigned char *key, int *error);
//32位字符串转为16字节数组函数
void str32ToByte16(char *inStr, unsigned char *in,int *error);

int main(int argc, char **argv)
{
	if (argc != 5)
	{
		return -1;
	}

	char *inFilename = argv[1];
	char *outFilename = argv[2];
	char *keyStr = argv[3];
	char *method = argv[4];

	unsigned char key[16];
	int error = 0;

	str32ToByte16(keyStr, key, &error);

	if (method[0] == 'e')
	{
		file_encrypt(inFilename, outFilename, key, &error);
		if (error == 1)
		{
			printf("File not found");
			return -1;
		}
	} 
	else if (method[0] == 'd')
	{
        file_decrypt(inFilename, outFilename, key, &error);
		if (error == 1)
		{
			printf("File not found!");
			return -1;
		}
	}

	return 0;
}

void file_encrypt(const char *inPath, const char *outPath, unsigned char *key, int *error)
{
    *error = 0;

    FILE *in_file = fopen(inPath, "rb+");

    //未找到输入文件
    if (in_file == NULL)
    {
        *error = 1;
        return;
    }

    FILE *out_file = fopen(outPath, "wb+");
    int ch;

    //get lenth 计算文件长度 保存在两个64位数len0,len1中
    unsigned long long len0 = 0ll;
    unsigned long long len1 = 0ll;
    while ((ch = fgetc(in_file)) != EOF)
    {
        len1 += 8ll;
        if (len1 == 0)
        {
            len0++;
        }
    }

    //convert len0,len1 to char[16] 将文件长度转为16字节数组
    unsigned char len[16];
    for (int i = 0; i < 8; i++)
    {
        len[i] = (len0 >> ((7 - i) * 8ll)) & 0x0ff;
        len[i + 8] = (len1 >> ((7 - i) * 8ll)) & 0x0ff;
    }

    //write lenth to out file 将文件长度连续三次写入输出文件中
    for (int i = 0; i < 16; i++)
    {
        fputc(len[i], out_file);
    }
    for (int i = 0; i < 16; i++)
    {
        fputc(len[i], out_file);
    }
    for (int i = 0; i < 16; i++)
    {
        fputc(len[i], out_file);
    }


    unsigned char tmpin[16];//输入块数组
    unsigned char tmpout[16];//输出块数组

    //密钥扩展
    unsigned char w[176];
    keyExpansion(key, w);

    //文件加密
    int i = 0;
    rewind(in_file);
    while ((ch = fgetc(in_file)) != EOF)
    {
        tmpin[i] = (unsigned char)ch;
        if (i == 15)
        {
            i = 0;
            Cipher(tmpin, tmpout, w, 0);

            for (int j = 0; j < 16; j++)
            {
                fputc(tmpout[j], out_file);
            }
        }
        else
        {
            i++;
        }
    }

    //add tail 文件末尾补全并加密
    while (i > 0)
    {
        tmpin[i] = 0x00;
        if (i == 15)
        {
            i = 0;
            Cipher(tmpin, tmpout, w, 0);

            for (int j = 0; j < 16; j++)
            {
                fputc(tmpout[j], out_file);
            }
        }
        else
        {
            i++;
        }
    }

    fclose(in_file);
    fclose(out_file);
}

void file_decrypt(const char *inPath, const char *outPath, unsigned char *key, int *error)
{
    FILE *in_file = fopen(inPath, "rb+");

    //未找到输入文件
    if (in_file == NULL)
    {
        *error = 1;
        return;
    }

    FILE *out_file = fopen(outPath, "wb+");

    unsigned char len0[16];
    unsigned char len1[16];
    unsigned char len2[16];

    //get 3 length 连续三次读取文件长度
    int ch;
    for (int i = 0; i < 16; i++)
    {
        ch = fgetc(in_file);
        len0[i] = (unsigned char)ch;
    }
    for (int i = 0; i < 16; i++)
    {
        ch = fgetc(in_file);
        len1[i] = (unsigned char)ch;
    }
    for (int i = 0; i < 16; i++)
    {
        ch = fgetc(in_file);
        len2[i] = (unsigned char)ch;
    }


    //length check 检查文件长度
    int p0 = 0;
    int p1 = 0;
    int p2 = 0;
    for (int i = 0; i < 16; i++)
    {
        if(len0[i] != len1[i])
        {
            p0 = 1;
            break;
        }
    }

    for (int i = 0; i < 16; i++)
    {
        if(len0[i] != len2[i])
        {
            p1 = 1;
            break;
        }
    }

    for (int i = 0; i < 16; i++)
    {
        if(len1[i] != len2[i])
        {
            p2 = 1;
            break;
        }
    }

    if ((p0 == 1)&&(p1 == 1)&&(p2 == 1))
    {
        //input wrong 三个文件长度均不相同
        *error = 2;
        return;
    }


    //将读取到的文件长度写入两个64位变量llen0,llen1中
    unsigned long long llen0 = 0ll;
    unsigned long long llen1 = 0ll;
    if (p0 == 0 || p1 == 0)
    {
        for (int i = 0; i < 8; i++)
        {
            llen0 <<= 8;
            llen0 |= len0[i] & 0x0ff;
            llen1 <<= 8;
            llen1 |= len0[i + 8] & 0x0ff;
        }
    }
    else
    {
        for (int i = 0; i < 8; i++)
        {
            llen0 <<= 8;
            llen0 |= len2[i] & 0x0ff;
            llen1 <<= 8;
            llen1 |= len2[i + 8] & 0x0ff;
        }
    }

    //密钥扩展
    unsigned char w[176];
    keyExpansion(key, w);

    unsigned char tmpin[16];//输入块数组
    unsigned char tmpout[16];//输出块数组

    //文件解密
    int j = 0;
    int end = 0;
    while((ch = fgetc(in_file)) != EOF)
    {
        tmpin[j] = (unsigned char)ch;

        if (j == 15)
        {
            j = 0;
            InvCipher(tmpin, tmpout, w, 0);

            for(int i = 0; i < 16; i++)
            {
                fputc(tmpout[i], out_file);

                llen1 -= 8;
                if (llen1 == 0)
                {
                    if (llen0 == 0)
                    {
                        //end of file
                        end = 1;
                        break;
                    }
                    else
                    {
                        llen0--;
                    }
                }
            }
        }
        else
        {
            j++;
        }

        if (end == 1)
        {
            break;
        }
    }

    fclose(in_file);
    fclose(out_file);

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
