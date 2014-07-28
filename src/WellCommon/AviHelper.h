#pragma once

#include <string>
using namespace std;

class AFX_EXT_CLASS CAviHelper
{
public:
	CAviHelper(void);
	~CAviHelper(void);

	//��ȡavi�ķֱ���
	static bool AVI_resolution(const wstring& strAviFileName, int& width, int& height);
	static bool AVI_resolution(const string& strAviFileName, int& width, int& height);

	//Aviת����bmp
	static void AVItoBmp(const wstring& strAVIFileName, const wstring& strBmpDir);
	static void AVItoBmp(const string& strAVIFileName, const string& strBmpDir);

	//bmpת����avi
	static void BMPtoAVI(const wstring& szAVIName, const wstring& strBmpDir);
	static void BMPtoAVI(const string& szAVIName, const string& strBmpDir);

};

