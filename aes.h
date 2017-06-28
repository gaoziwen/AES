#ifndef _MY_AES
#define _MY_AES

void subBytes(unsigned char *state);
void invSubBytes(unsigned char *state);
void shiftRows(unsigned char *state);
void invShiftRows(unsigned char *state);
void mixColumns(unsigned char *state);
void invMixColumns(unsigned char *state);
void addRoundKey(unsigned char *state, unsigned char *w, int round);
void keyExpansion(unsigned char *key, unsigned char *w);
void Cipher(unsigned char *in, unsigned char *out, unsigned char *w, int prt);
void InvCipher(unsigned char *in, unsigned char *out, unsigned char *w, int prt);

#endif
