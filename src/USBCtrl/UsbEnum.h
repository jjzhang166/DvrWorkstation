//////////////////////////////////////////////////////////////////////////
//Describe: USBö����
//			1. ��DDK��USBView����ض��������
//			2. ���϶�ȡ�̷�����
//			�ο���
//				http://bbs.csdn.net/topics/350036814
//				http://www.laomaotao.net/new/2012/0218/499.html ����������̶�U���̷�ԭ��
//			ʵ�֣�
//				1. ��������USB�豸����ȡ����ID
//					1.1 CM_Get_Child()��CM_Get_Device_ID()������Ӧ��USBSTOR���豸ID������USB\VID_0595&PID_2002\ZORAN01234567��
//					1.2 ͨ�� USB\\VID_0595&PID_2002\\ ���˵�����ص��豸
//				2. ����SYSTEM\\MountedDevices�´�\\DosDevices\\X:�Ľ�����������ֵ�����(�̷���ֵ��Ӧ���
//						//����A-Z
// 						for (n = 0; n < 26; ++n)
// 						{
// 							sprintf(strTmp, "\\DosDevices\\%c:", 'A' + n);
// 							dwLen = sizeof(strValue);
// 							if( (dwLen = GetRegKeyValue(strSKey1, strTmp, REG_BINARY, strValue, dwLen)) != 0)
// 							{
// 								j = 0;
// 								for( i = 0; i < dwLen; ++i)
// 								{
// 									if(strValue[i] != 0)
// 									{
//										//ȥ��ÿ���ַ��м�00
// 										*(strValue1[n] + j++) =strValue[i];
// 									}
// 								}
// 							}
// 						}
//
//				3. ��SYSTEM\\CurrentControlSet\\Enum\\ + USBSTOR���豸ID �ļ�
//					�� SYSTEM\\CurrentControlSet\\Enum\\USB\\VID_0595&PID_2002\\ZORAN01234567
//					2.1 ��ȡParentIdPrefix��ֵ(),�����ã�ת��4, ���û��ParentIdPrefixת��5
//				4. �ѻ�ȡ�õ���ParentIdPrefix��ֵ��8&16a2684b&0��,�͵ڶ����õ���ֵ�Աȣ��ҵ���˵���̷��ҵ�
//					//��ParentIdPrefix�ģ�ֱ�ӿ����ҵ��̷�
//					//_??_USBSTOR#Disk&Ven_ZOKOD_DV&Prod_R&Rev_1.10#8&16a2684b&0&ZORAN01234567&0#{53f56307-b6bf-11d0-94f2-00a0c91efb8b}
//					4.1 ƥ�䵽ParentIdPrefix���ҵ��̷�
//
//				5. û��ParentIdPrefix(ÿһ�������ö�ٵ��̲������ParentIdPrefix)
//					//��һ��ö�ٳ�U�̵ģ�û��ParentIdPrefix��
//					//_??_USBSTOR#Disk&Ven_ZOKOD&Prod___DVR&Rev_1.10#ZORAN01234567&0#{53f56307-b6bf-11d0-94f2-00a0c91efb8b}
//					5.1 ��ȡ���Ҵ�( # + USBSTOR���豸ID�����һ����),��#ZORAN01234567
//					5.2 �͵ڶ����õ���ֵ�Աȣ����ң����õ��̷�
//
//Date: created by yjt 2014-06-26
//History:
//
//////////////////////////////////////////////////////////////////////////
#pragma once

#include "usbioctl.h"

#include <string>
#include <map>
#include <vector>
using namespace std;

wstring ConnectionStatuses[] =
{
	_T("NoDeviceConnected"),
	_T("DeviceConnected"),
	_T("DeviceFailedEnumeration"),
	_T("DeviceGeneralFailure"),
	_T("DeviceCausedOvercurrent"),
	_T("DeviceNotEnoughPower")
};

#define NUM_HCS_TO_CHECK 20

#ifdef  DEBUG
#undef  DBG
#define DBG 1
#endif

//test
#define DBG 0

#if DBG
#define OOPS() Oops(__FILE__, __LINE__)
#else
#define OOPS()
#endif

#if DBG

#define ALLOC(dwBytes) MyAlloc(__FILE__, __LINE__, (dwBytes))

#define REALLOC(hMem, dwBytes) MyReAlloc((hMem), (dwBytes))

#define FREE(hMem)  MyFree((hMem))

#define CHECKFORLEAKS() MyCheckForLeaks()

#else

#define ALLOC(dwBytes) GlobalAlloc(GPTR,(dwBytes))

#define REALLOC(hMem, dwBytes) GlobalReAlloc((hMem), (dwBytes), (GMEM_MOVEABLE|GMEM_ZEROINIT))

#define FREE(hMem)  GlobalFree((hMem))

#define CHECKFORLEAKS()

#endif

//<port1-n, usb string>
typedef map<wstring, wstring> MapUsb;
typedef MapUsb::iterator MapUsbItor;

class AFX_EXT_CLASS CUsbEnum
{
public:
	CUsbEnum(void);

	void Init();

	~CUsbEnum(void);
	
	//ö�ٿ�����
	MapUsb& EnumerateHostControllers();

protected:
	VOID EnumerateHubPorts(HANDLE hHubDevice, ULONG NumPorts);

	VOID EnumerateHub (const wstring& HubName, PUSB_NODE_CONNECTION_INFORMATION	ConnectionInfo, const wstring& DeviceDesc);

private:
	wstring GetDriverKeyName ( HANDLE Hub, ULONG ConnectionIndex );
	wstring GetHCDDriverKeyName (HANDLE  HCD);
	wstring GetRootHubName( HANDLE HostController );
	wstring GetExternalHubName (HANDLE Hub, ULONG ConnectionIndex);
	wstring DriverNameToDeviceDesc (const wstring& DriverName);

	DWORD GetRegKeyValue(const wstring& strSubKey, const wstring& strKey, DWORD keyType, LPBYTE strValue, DWORD dwLen);

	void AddLeaf(int level, const wstring& text);

	//��ע����ж�ȡ�̷���Ӧ�ı�ʶ��
	vector<wstring> ReadDeviceStrings();
	//��ȡ�̷�
	wstring GetDiskName(const wstring& deviceDesc, const vector<wstring>& vecDiskStrings);
private:
	ULONG TotalDevicesConnected;
	ULONG TotalHubs;

	//DriverNameToDeviceDesc�õ�
	TCHAR buf[512];
	
	//����--��ӡ��
	int m_nLevel;

	//�˿�--�����еĶ˿ڶ����б��
	int m_nPort;

	MapUsb m_mapUsb;
};

