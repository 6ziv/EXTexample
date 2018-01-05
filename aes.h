#ifndef AES_H
#define AES_H
#include <string.h>
class AES
{
public:
  AES();
  virtual ~AES();

  void Cipher(const unsigned char *input, unsigned char *output);
  void init(const unsigned char* key);
private:
  unsigned char Sbox[256];
  unsigned char w[11][4][4];

  void KeyExpansion(const unsigned char* key, unsigned char w[][4][4]);
  unsigned char FFmul(unsigned char a, unsigned char b);

  void SubBytes(unsigned char state[][4]);
  void ShiftRows(unsigned char state[][4]);
  void MixColumns(unsigned char state[][4]);
  void AddRoundKey(unsigned char state[][4], unsigned char k[][4]);
};
#endif // AES_H
