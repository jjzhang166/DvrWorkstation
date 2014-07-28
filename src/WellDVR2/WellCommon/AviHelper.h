//////////////////////////////////////////////////////////////////////////
//������
//		AVI��Ƶ������װ
//�ο���
//		1. AVIʱ������: http://blog.sina.com.cn/s/blog_54d63a8b0100t5b6.html
//History:
//
//////////////////////////////////////////////////////////////////////////
#pragma once

#include <string>
using namespace std;

#include <Vfw.h>
#pragma comment(lib, "Vfw32.lib")


#define IVS_COMM_EXPORTS

#ifdef IVS_COMM_EXPORTS
#define IVS_COMM_API __declspec(dllexport)
#else 
#define IVS_COMM_API __declspec(dllimport)
#endif

class IVS_COMM_API CAviHelper
{
public:
	CAviHelper(void);
	~CAviHelper(void);

	static AVIFILEINFO AVI_GetFileInfo(const wstring& strFileName);
	static AVIFILEINFO AVI_GetFileInfo(const string& strFileName);

	static bool AVI_GetStreamInfo(const wstring& strFileName, PAVISTREAM pVideoStream, PAVISTREAM pAudioStream);

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

