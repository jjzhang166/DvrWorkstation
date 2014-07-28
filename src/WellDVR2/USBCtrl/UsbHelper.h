//////////////////////////////////////////////////////////////////////////
//Describe: USB������
//
//Date:	created by yjt 2014-07-02
//History:
//
//////////////////////////////////////////////////////////////////////////
#pragma once

#include <string>
using namespace std;

#include "UsbEnum.h"

#define USBHUB CUsbHelper::Instance()

#define IVS_COMM_EXPORTS

#ifdef IVS_COMM_EXPORTS
#define IVS_COMM_API __declspec(dllexport)
#else 
#define IVS_COMM_API __declspec(dllimport)
#endif


class IVS_COMM_API CUsbHelper
{
private:
	CUsbHelper(void);
	~CUsbHelper(void);

public:
	static CUsbHelper& Instance();

	//��ȡ�²����USB ��Ӧ��hub��,��[Port1] ~ [Portn]
	wstring GetNewInsertUsbPort();

	wstring GetRemoveUsbPort();

	//��ȡUSB hub�ں��̷��Ķ�Ӧ��ϵ
	MapUsb& GetUsbPort_DiskName();

	//��ȡUSB_HUB�ڶ�Ӧ�����豸����
	MapUsb& GetMapPortName();

	bool UnInstallUsb(char szDriveName) ;

private:
	bool ClearUsbRegKey(char szDriveName);
	DWORD SetRegKeyValue(const wstring& strSubKey, const wstring& strKey, DWORD keyType, LPBYTE strValue, DWORD dwLen);
	bool DelRegKey(const wstring& strSubKey, const wstring& strKey);

private:
	CUsbEnum m_usb;
};

