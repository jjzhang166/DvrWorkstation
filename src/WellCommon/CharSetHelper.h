#pragma once
/********************************************************************
	created:	2012/05/25 12:53  
	filename: 	CharSetHelper.h
	author:		yjt
	purpose:	�ַ���������

	����:
		1.string ת CString
		CString.format(��%s��, string.c_str());

		2.char ת CString
		CString.format(��%s��, char*);

		3.char to string
		string s(char *);

		4.string ת char *
		char *p = string.c_str();

		5.CString ת string
		string s(CString.GetBuffer());

		6.string -> CString
		CString.format(��%s��, string.c_str());
		��c_str()ȷʵ��data()Ҫ��.
		7.CString -> string
		string s(CString.GetBuffer());
		GetBuffer()��һ��ҪReleaseBuffer(),�����û���ͷŻ�������ռ�Ŀռ�.
		8.�������������Խ��ַ���������ת��Ϊ�ַ������C��string
		1.data(),����û�С�\0�����ַ�������
		2,c_str()�������С�\0�����ַ�������
		3��copy()

		9.���ַ�ת��Ϊ����������ʹ��atoi��_atoi64��atol��
		10.������ת��ΪCString����������ʹ��CString��Format����

		CString s;
		int i = 64;
		s.Format(��%d��, i)
		Format�����Ĺ��ܺ�ǿ��ֵ�����о�һ�¡�

		11.cstring TO char *
		charpoint=strtest.GetBuffer(strtest.GetLength());

		12.CStringת�� char[100]

		char a[100];
		CString str(��aaaaaa��);
		strncpy(a,(LPCTSTR)str,sizeof(a));
*********************************************************************/
#include <string>
using namespace std;

#define IVS_COMM_EXPORTS

#ifdef IVS_COMM_EXPORTS
#define IVS_COMM_API __declspec(dllexport)
#else 
#define IVS_COMM_API __declspec(dllimport)
#endif

#define C2W(str) CCharSetHelper::c2w((str))
#define W2C(str) CCharSetHelper::w2c((str))

class IVS_COMM_API CCharSetHelper
{
public:
	CCharSetHelper(void);
	~CCharSetHelper(void);

	//char to wchar_t
	static size_t c2w(wchar_t *pwstr, char *str);
	static void c2w(wchar_t *pwstr,size_t len,char *str);
	static wstring c2w(const string& str);
	static size_t c2w(const string& strIn, wstring& wstrOut);

	//wchar_t change to char
	static char* w2c(char *pcstr,wchar_t *pwstr, size_t len);
	static size_t w2c(char *pcstr,wchar_t *pwstr);
	static string w2c(const wstring& wstr);
	static size_t w2c(const wstring& wstrIn, string& strOut);
};

