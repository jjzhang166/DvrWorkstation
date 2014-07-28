///********************************************************************
//	created:	2013/06/04
//	filename: 	EncryptHelper.h
//	author:		yjt
//	
//	purpose:	���ܸ�����
//*********************************************************************/
#pragma once
#include <openssl/des.h> 
#include <openssl/aes.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/evp.h>

#include <string>
using namespace std;

#pragma comment(lib, "libeay32.lib")
#pragma comment(lib, "ssleay32.lib")


#define IVS_COMM_EXPORTS

#ifdef IVS_COMM_EXPORTS
#define IVS_COMM_API __declspec(dllexport)
#else 
#define IVS_COMM_API __declspec(dllimport)
#endif

class IVS_COMM_API EncryptHelper
{
private:
	EncryptHelper(void);
	~EncryptHelper(void);

public:
	//aes����
	static int AES_Encrypt(const unsigned char* strIn, unsigned char* strOut, int& nlen);
	//aes����
	static int AES_Decrypt(const unsigned char* strIn, int inLen, unsigned char* strOut, int& nlen);

protected:
	//�����16������
	static int Encode2Hex(const unsigned char *str,int str_len, unsigned char *encode,int& encode_len);
	//����16������
	static int DecodeHex(const unsigned char *str,int str_len, unsigned char *decode,int& decode_buffer_len);
};

