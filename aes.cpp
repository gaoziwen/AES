/**
 * 该文件是AES算法的源代码
 */
#include <stdio.h>

static unsigned char s_box[256] = {
    // 0     1     2     3     4     5     6     7     8     9     a     b     c     d     e     f
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76, // 0
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0, // 1
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15, // 2
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75, // 3
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84, // 4
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf, // 5
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8, // 6
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2, // 7
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73, // 8
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb, // 9
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79, // a
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08, // b
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a, // c
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e, // d
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf, // e
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16};// f

static unsigned char inv_s_box[256] = {
    // 0     1     2     3     4     5     6     7     8     9     a     b     c     d     e     f
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb, // 0
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb, // 1
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e, // 2
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25, // 3
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92, // 4
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84, // 5
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06, // 6
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b, // 7
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73, // 8
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e, // 9
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b, // a
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4, // b
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f, // c
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef, // d
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61, // e
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d};// f


//SubBytes及其逆操作函数
void subBytes(unsigned char *state)
{
    for (int i = 0; i < 16; i++)
    {
        state[i] = s_box[state[i]];
    }
}

void invSubBytes(unsigned char *state)
{
    for (int i = 0; i < 16; i++)
    {
        state[i] = inv_s_box[state[i]];
    }
}

//ShiftRows及其逆操作函数
void shiftRows(unsigned char *state)
{
    unsigned char temp;
    temp = state[4];
    state[4] = state[5];
    state[5] = state[6];
    state[6] = state[7];
    state[7] = temp;

    temp = state[8];
    state[8] = state[10];
    state[10] = temp;
    temp = state[9];
    state[9] = state[11];
    state[11] = temp;

    temp = state[15];
    state[15] = state[14];
    state[14] = state[13];
    state[13] = state[12];
    state[12] = temp;
}

void invShiftRows(unsigned char *state)
{
    unsigned char temp;

    temp = state[7];
    state[7] = state[6];
    state[6] = state[5];
    state[5] = state[4];
    state[4] = temp;

    temp = state[11];
    state[11] = state[9];
    state[9] = temp;
    temp = state[10];
    state[10] = state[8];
    state[8] = temp;

    temp = state[12];
    state[12] = state[13];
    state[13] = state[14];
    state[14] = state[15];
    state[15] = temp;
}

//GF(2^8)下的相乘函数，函数会返回两个字节相乘的结果
unsigned char mul(unsigned char in1, unsigned char in2)
{
    unsigned short temp;
    temp = 0;
    for (int i = 0; i < 8; i++)
    {
        if((in2 >> i) & 0x01)
        {
            temp ^= in1 << i;
        }
    }

    for (int i = 14; i >= 8; i--)
    {
        if ((temp >> i) & 0x01)
        {
            temp ^= 0x011b << (i - 8);
        }
    }

    return (unsigned char)temp;
}

//MixColumns相乘矩阵
static unsigned char mix_mat[16] = {
    0x02, 0x03, 0x01, 0x01,
    0x01, 0x02, 0x03, 0x01,
    0x01, 0x01, 0x02, 0x03,
    0x03, 0x01, 0x01, 0x02,
};

//InvMixColumns相乘矩阵
static unsigned char inv_mix_mat[16] = {
    0x0e, 0x0b, 0x0d, 0x09,
    0x09, 0x0e, 0x0b, 0x0d,
    0x0d, 0x09, 0x0e, 0x0b,
    0x0b, 0x0d, 0x09, 0x0e,
};

//MixColumns及其逆操作函数
void mixColumns(unsigned char *state)
{
    unsigned char temp;
    unsigned char state_c[16];
    for (int i = 0; i < 16; i++)
    {
        state_c[i] = state[i];
    }

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            temp = 0;
            for (int k = 0; k < 4; k++)
            {
                temp ^= mul(mix_mat[i * 4 + k], state_c[k * 4 + j]);
            }
            state[i * 4 + j] = temp;
        }
    }
}

void invMixColumns(unsigned char *state)
{
    unsigned char temp;
    unsigned char state_c[16];
    for (int i = 0; i < 16; i++)
    {
        state_c[i] = state[i];
    }

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            temp = 0;
            for (int k = 0; k < 4; k++)
            {
                temp ^= mul(inv_mix_mat[i * 4 + k], state_c[k * 4 + j]);
            }
            state[i * 4 + j] = temp;
        }
    }
}

//AddRoundKey操作函数
void addRoundKey(unsigned char *state, unsigned char *w, int round)
{
    for (int j = 0; j < 4; j++)
    {
        for (int i = 0; i < 4; i++)
        {
            state[i * 4 + j] ^= w[round * 16 + j * 4 + i];
        }
    }
}

//Rcon数组
static unsigned char RC_table[11] = {
    0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36
};

//密钥扩展函数
void keyExpansion(unsigned char *key, unsigned char *w)
{
    unsigned char temp[4];
    unsigned char tmp;

    for (int i = 0; i < 4; i++)
    {
        w[i * 4] = key[4 * i];
        w[i * 4 + 1] = key[4 * i + 1];
        w[i * 4 + 2] = key[4 * i + 2];
        w[i * 4 + 3] = key[4 * i + 3];
    }

    for (int i = 4; i < 44; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            temp[j] = w[(i - 1) * 4 + j];
        }

        if (i % 4 == 0)
        {
            //rotWord
            tmp = temp[0];
            temp[0] = temp[1];
            temp[1] = temp[2];
            temp[2] = temp[3];
            temp[3] = tmp;

            //subWord
            for (int j = 0; j < 4; j++)
            {
                temp[j] = s_box[temp[j]];
            }

            //xor Rcon[i / 4]
            temp[0] = temp[0] ^ RC_table[i / 4];
        }

        //w[i] = w[i - 4] xor temp
        for (int j = 0; j < 4; j++)
        {
            w[i * 4 + j] = w[(i - 4) * 4 + j] ^ temp[j];
        }
    }
}

//加密函数，in为输入串，out为输出串，w扩展的密钥，prt为过程打印选项，prt为1时打印过程
void Cipher(unsigned char *in, unsigned char *out, unsigned char *w, int prt)
{
    unsigned char state[16];

    //state = in
    for (int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
        {
            state[i * 4 + j] = in[j * 4 + i];
        }

	if (prt == 1)
	{
		printf("R[0].input: ");
		for (int j = 0; j < 4; j++)
		{
			for (int i = 0; i < 4; i++)
			{
				printf("%02x ", state[i * 4 + j]);
			}
		}
		printf("<br>");

		printf("R[0].k_sch: ");
		for (int i = 0; i < 16; i++)
		{
			printf("%02x ", w[i]);
		}
		printf("<br>");
	}

    addRoundKey(state, w, 0);

    for (int i = 1; i < 10; i++)
    {

		if (prt == 1)
		{
			printf("R[%d].start: ", i);
			for (int j = 0; j < 4; j++)
			{
				for (int k = 0; k < 4; k++)
				{
					printf("%02x ", state[k * 4 + j]);
				}
			}
			printf("<br>");
		}
        subBytes(state);
		if (prt == 1)
		{
			printf("R[%d].s_box: ", i);
			for (int j = 0; j < 4; j++)
			{
				for (int k = 0; k < 4; k++)
				{
					printf("%02x ", state[k * 4 + j]);
				}
			}
			printf("<br>");
		}
        shiftRows(state);
		if (prt == 1)
		{
			printf("R[%d].s_row: ", i);
			for (int j = 0; j < 4; j++)
			{
				for (int k = 0; k < 4; k++)
				{
					printf("%02x ", state[k * 4 + j]);
				}
			}
			printf("<br>");
		}
        mixColumns(state);
		if (prt == 1)
		{
			printf("R[%d].m_col: ", i);
			for (int j = 0; j < 4; j++)
			{
				for (int k = 0; k < 4; k++)
				{
					printf("%02x ", state[k * 4 + j]);
				}
			}
			printf("<br>");
		}
        addRoundKey(state, w, i);
		if (prt == 1)
		{
			printf("R[%d].k_sch: ", i);
			for (int k = 0; k < 16; k++)
			{
				printf("%02x ", w[i * 16 + k]);
			}
			printf("<br>");
		}
    }

	if (prt == 1)
	{
		printf("R[10].start: ");
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				printf("%02x ", state[k * 4 + j]);
			}
		}
		printf("<br>");
	}
    subBytes(state);
	if (prt == 1)
	{
		printf("R[10].s_box: ");
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				printf("%02x ", state[k * 4 + j]);
			}
		}
		printf("<br>");
	}
    shiftRows(state);
	if (prt == 1)
	{
		printf("R[10].s_row: ");
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				printf("%02x ", state[k * 4 + j]);
			}
		}
		printf("<br>");
	}
    addRoundKey(state, w, 10);
	if (prt == 1)
	{
		printf("R[10].k_sch: ");
		for (int i = 0; i < 16; i++)
		{
			printf("%02x ", w[160 + i]);
		}
		printf("<br>");

		printf("R[10].output: ");
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				printf("%02x ", state[k * 4 + j]);
			}
		}
		printf("<br>");
        printf("\n");
	}

    //out = state
    for (int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
        {
            out[i * 4 + j] = state[j * 4 + i];
        }
}

//解密函数，in为输入串，out为输出串，w扩展的密钥， prt为过程打印选项，prt为1时打印过程
void InvCipher(unsigned char *in, unsigned char *out, unsigned char *w, int prt)
{
    unsigned char state[16];

    //state = in
    for (int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
        {
            state[i * 4 + j] = in[j * 4 + i];
        }

    if (prt == 1)
    {
        printf("R[0].iinput: ");
        for (int j = 0; j < 4; j++)
        {
            for (int i = 0; i < 4; i++)
            {
                printf("%02x ", state[i * 4 + j]);
            }
        }
        printf("<br>");

        printf("R[0].ik_sch: ");
        for (int i = 0; i < 16; i++)
        {
            printf("%02x ", w[160 + i]);
        }
        printf("<br>");
    }

    addRoundKey(state, w, 10);

    for (int i = 9; i >= 1; i--)
    {
        if (prt == 1)
        {
            printf("R[%d].istart: ", 10 - i);
            for (int j = 0; j < 4; j++)
            {
                for (int k = 0; k < 4; k++)
                {
                    printf("%02x ", state[k * 4 + j]);
                }
            }
            printf("<br>");
        }

        invShiftRows(state);

        if (prt == 1)
        {
            printf("R[%d].is_row: ", 10 - i);
            for (int j = 0; j < 4; j++)
            {
                for (int k = 0; k < 4; k++)
                {
                    printf("%02x ", state[k * 4 + j]);
                }
            }
            printf("<br>");
        }

        invSubBytes(state);

        if (prt == 1)
        {
            printf("R[%d].is_box: ", 10 - i);
            for (int j = 0; j < 4; j++)
            {
                for (int k = 0; k < 4; k++)
                {
                    printf("%02x ", state[k * 4 + j]);
                }
            }
            printf("<br>");
        }

        addRoundKey(state, w, i);

        if (prt == 1)
        {
            printf("R[%d].ik_sch: ", 10 - i);
            for (int k = 0; k < 16; k++)
            {
                printf("%02x ", w[i * 16 + k]);
            }
            printf("<br>");
        }

        if (prt == 1)
        {
            printf("R[%d].ik_add: ", 10 - i);
            for (int j = 0; j < 4; j++)
            {
                for (int k = 0; k < 4; k++)
                {
                    printf("%02x ", state[k * 4 + j]);
                }
            }
            printf("<br>");
        }

        invMixColumns(state);

    }

    if (prt == 1)
    {
        printf("R[10].istart: ");
        for (int j = 0; j < 4; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                printf("%02x ", state[k * 4 + j]);
            }
        }
        printf("<br>");
    }

    invShiftRows(state);

    if (prt == 1)
    {
        printf("R[10].is_row: ");
        for (int j = 0; j < 4; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                printf("%02x ", state[k * 4 + j]);
            }
        }
        printf("<br>");
    }

    invSubBytes(state);

    if (prt == 1)
    {
        printf("R[10].is_box: ");
        for (int j = 0; j < 4; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                printf("%02x ", state[k * 4 + j]);
            }
        }
        printf("<br>");
    }

    addRoundKey(state, w, 0);

    if (prt == 1)
    {
        printf("R[10].ik_sch: ");
        for (int k = 0; k < 16; k++)
        {
            printf("%02x ", w[k]);
        }
        printf("<br>");
    }

    if (prt == 1)
    {
        printf("R[10].ioutput: ");
        for (int j = 0; j < 4; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                printf("%02x ", state[k * 4 + j]);
            }
        }
        printf("<br>");
        printf("\n");
    }

    //out = state
    for (int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
        {
            out[i * 4 + j] = state[j * 4 + i];
        }
}
