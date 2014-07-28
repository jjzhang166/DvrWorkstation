
#include <windows.h>
#include <winioctl.h>
#include <stdio.h>
#include <initguid.h>
#include <string.h>

#include <setupapi.h>
#pragma comment(lib,"setupapi.lib")

int gTag = 0;
#define MAX_DEVICE 256
wchar_t USBSerial[5][100] = {0};
//#define USBSERIAL_PATH C:\\Windows\\system32\\USBSerial.dat
DEFINE_GUID(UsbClassGuid, 0xa5dcbf10L, 0x6530, 0x11d2, 0x90, 0x1f, 0x00, 0xc0, 0x4f, 0xb9, 0x51, 0xed);

// SetupDiGetInterfaceDeviceDetail����Ҫ��������ȣ������㹻��
#define INTERFACE_DETAIL_SIZE    (1024)

// IOCTL������
#define IOCTL_STORAGE_QUERY_PROPERTY   CTL_CODE(IOCTL_STORAGE_BASE, 0x0500, METHOD_BUFFERED, FILE_ANY_ACCESS)



// ��ѯ�洢�豸���Ե�����
/*typedef enum _STORAGE_QUERY_TYPE {
PropertyStandardQuery = 0,          // ��ȡ����
PropertyExistsQuery,                // �����Ƿ�֧��
PropertyMaskQuery,                  // ��ȡָ��������
PropertyQueryMaxDefined             // ��֤����
} STORAGE_QUERY_TYPE, *PSTORAGE_QUERY_TYPE;
*/


// ��ѯ�洢�豸��������������
//typedef enum _STORAGE_PROPERTY_ID {
//    StorageDeviceProperty = 0,          // ��ѯ�豸����
//    StorageAdapterProperty              // ��ѯ����������
//} STORAGE_PROPERTY_ID, *PSTORAGE_PROPERTY_ID;
// 
// ��ѯ������������ݽṹ
//typedef struct _STORAGE_PROPERTY_QUERY {
//    STORAGE_PROPERTY_ID PropertyId;     // �豸/������
//    STORAGE_QUERY_TYPE QueryType;       // ��ѯ���� 
//    UCHAR AdditionalParameters[1];      // ���������(�������������Ե�1���ֽ�)
//} STORAGE_PROPERTY_QUERY, *PSTORAGE_PROPERTY_QUERY;
// 
//// ��ѯ������������ݽṹ
//typedef struct _STORAGE_DEVICE_DESCRIPTOR {
//    ULONG Version;                    // �汾
//    ULONG Size;                       // �ṹ��С
//    UCHAR DeviceType;                 // �豸����
//    UCHAR DeviceTypeModifier;         // SCSI-2������豸����
//    BOOLEAN RemovableMedia;           // �Ƿ���ƶ�
//    BOOLEAN CommandQueueing;          // �Ƿ�֧���������
//    ULONG VendorIdOffset;             // �����趨ֵ��ƫ��
//    ULONG ProductIdOffset;            // ��ƷID��ƫ��
//    ULONG ProductRevisionOffset;      // ��Ʒ�汾��ƫ��
//    ULONG SerialNumberOffset;         // ���кŵ�ƫ��
//    STORAGE_BUS_TYPE BusType;         // ��������
//    ULONG RawPropertiesLength;        // ������������ݳ���
//    UCHAR RawDeviceProperties[1];     // �������������(�������������Ե�1���ֽ�)
//} STORAGE_DEVICE_DESCRIPTOR, *PSTORAGE_DEVICE_DESCRIPTOR;

// ����GUID����豸·��
// lpGuid: GUIDָ��
// pszDevicePath: �豸·��ָ���ָ��
// ����: �ɹ��õ����豸·�����������ܲ�ֹ1��
int GetDevicePath(LPGUID lpGuid, LPTSTR* pszDevicePath)
{
	HDEVINFO hDevInfoSet;    //�豸��Ϣ�������
	SP_DEVICE_INTERFACE_DATA ifdata;
	PSP_DEVICE_INTERFACE_DETAIL_DATA pDetail;
	int nCount;
	BOOL bResult;

	// ȡ��һ����GUID��ص��豸��Ϣ�����
	hDevInfoSet = ::SetupDiGetClassDevs((LPGUID)&UsbClassGuid,	// class GUID 
		NULL,// �޹ؼ���
		NULL,// ��ָ�������ھ��
		DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);	 // Ŀǰ���ڵ��豸

	// ʧ��...
	if (hDevInfoSet == INVALID_HANDLE_VALUE)
	{
		printf("Error!\n");
		//   break;
		return 0;
	}

	// �����豸�ӿ����ݿռ�
	pDetail = (PSP_DEVICE_INTERFACE_DETAIL_DATA)::GlobalAlloc(LMEM_ZEROINIT, INTERFACE_DETAIL_SIZE);

	pDetail->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

	nCount = 0;
	bResult = TRUE;

	// �豸���=0,1,2... ��һ�����豸�ӿڣ���ʧ��Ϊֹ
	while (bResult)
	{
		ifdata.cbSize = sizeof(ifdata);

		// ö�ٷ��ϸ�GUID���豸�ӿ�
		bResult = ::SetupDiEnumDeviceInterfaces(
			hDevInfoSet,     // �豸��Ϣ�����
			NULL,            // ���������豸����
			lpGuid,          // GUID
			(ULONG)nCount,   // �豸��Ϣ������豸���
			&ifdata);        // �豸�ӿ���Ϣ

		if (bResult)
		{
			// ȡ�ø��豸�ӿڵ�ϸ��(�豸·��)
			bResult = SetupDiGetInterfaceDeviceDetail(
				hDevInfoSet,    // �豸��Ϣ�����
				&ifdata,        // �豸�ӿ���Ϣ
				pDetail,        // �豸�ӿ�ϸ��(�豸·��)
				INTERFACE_DETAIL_SIZE,   // �����������С
				NULL,           // ������������������С(ֱ�����趨ֵ)
				NULL);          // ���������豸����
			if (bResult)
			{
				// �����豸·�������������
				//wcscpy_s(pszDevicePath[nCount],wcslen(pDetail->DevicePath)+1, pDetail->DevicePath);
				strcpy_s(pszDevicePath[nCount],strlen(pDetail->DevicePath)+1, pDetail->DevicePath);
				// ��������ֵ
				nCount++;
			}
		}
	}
	// printf("%d\n", nCount);
	// printf("%s\n",pszDevicePath[1]);
	// �ͷ��豸�ӿ����ݿռ�
	::GlobalFree(pDetail);
	// �ر��豸��Ϣ�����
	::SetupDiDestroyDeviceInfoList(hDevInfoSet);
	return nCount;
}

// OpenDevice(char* DevicePath)
// ���ؾ�� hDevice
HANDLE OpenDevice(TCHAR* DevicePath)
{
	HANDLE hDevice;
	hDevice = CreateFile(DevicePath,
		GENERIC_READ && GENERIC_WRITE,
		FILE_SHARE_READ && FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);
	return hDevice;
}

#if 0
int WINAPI
	WinMain (
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpszCmdLine,
	int       nCmdShow
	)
{
#else

int main(int argc, char *argv[])
{
#endif
	int i, nDevice; // nDevice ��ʾ�ж��ٸ�USB�豸
	int ndevice = 0; // ��ʾ���ٸ�U���豸(�ɽ��ж�д���豸)
	TCHAR* szDevicePath[MAX_DEVICE];        // �豸·��
	char pdg[1024] ;
	DWORD junk;
	HANDLE hDevice;
	BOOL bResult;

	// DWORD dwOutBytes; // IOCTL������ݳ���
	// ����һ�� PSTORAGE_DEVICE_DESCRIPTOR ����������豸����
	PSTORAGE_DEVICE_DESCRIPTOR DeviceDesc;
	// ������ʼ��
	DeviceDesc=(PSTORAGE_DEVICE_DESCRIPTOR)new BYTE[sizeof(STORAGE_DEVICE_DESCRIPTOR) + 512 - 1];
	DeviceDesc->Size = sizeof(STORAGE_DEVICE_DESCRIPTOR) + 512 - 1;

	// ������Ҫ�Ŀռ�
	for (i = 0; i < MAX_DEVICE; i++)
	{
		szDevicePath[i] = new TCHAR[256];
	}

	// ȡ�豸·��
	nDevice = ::GetDevicePath((LPGUID)&UsbClassGuid, szDevicePath);

	// printf("%d\n", nDevice);
	// for(i = 0; i< nDevice; i++)
	//   printf("%s\n",szDevicePath[i]);

	// ��һ��ȡ�豸��Ϣ
	for (i = 0; i < nDevice; i++)
	{
		// ���豸
		hDevice = OpenDevice(szDevicePath[i]);

		if (hDevice != INVALID_HANDLE_VALUE)
		{

			bResult = ::DeviceIoControl(hDevice, // device to be queried
				IOCTL_STORAGE_GET_MEDIA_TYPES, // operation to perform
				NULL, 0, // no input buffer
				pdg, 1024,     // output buffer
				&junk,                 // # bytes returned
				(LPOVERLAPPED) NULL); // synchronous I/O

			for(int j = 0; j < 4; j++)
			{
				USBSerial[ndevice][j] = szDevicePath[i][j+12]; 
			}
			for(int j = 4; j < 28; j++)
			{
				USBSerial[ndevice][j] = szDevicePath[i][j+22]; 
			}
			printf("U�����к�Ϊ��");
			wprintf(L"%ws\n", USBSerial[ndevice]);
			ndevice ++;

			::CloseHandle(hDevice);
		}

	} 

	// �ͷſռ�
	for (i = 0; i < MAX_DEVICE; i++)
	{
		delete []szDevicePath[i];
	}

	return gTag;
/*
	////////////////// �ļ����� ////////////////////////////////////////
	HANDLE hFile; 
	DWORD nBytesRead = 0, dwBytesWritten = 0;
	// ���ļ�
	hFile = CreateFile(TEXT("C:\\Windows\\system32\\USBSerial.dat"),    // file to open
		GENERIC_READ | GENERIC_WRITE,          // open for reading
		FILE_SHARE_READ | FILE_SHARE_WRITE,       // share for reading
		NULL,                  // default security
		OPEN_EXISTING ,         // existing file only
		FILE_ATTRIBUTE_NORMAL, // normal file
		NULL);                 // no attr. template
	if (hFile == INVALID_HANDLE_VALUE) 
	{

		// printf("1111111111111111111111111111\n");
		// printf("Could not open file!\n");
		hFile = CreateFile(TEXT("C:\\Windows\\system32\\USBSerial.dat"),     // file to create
			GENERIC_READ | GENERIC_WRITE,          // open for writing
			FILE_SHARE_READ | FILE_SHARE_WRITE,                      // do not share
			NULL,                   // default security
			CREATE_ALWAYS,          // overwrite existing
			FILE_ATTRIBUTE_NORMAL | // normal file
			FILE_ATTRIBUTE_NORMAL,   // asynchronous I/O
			NULL); 
		// printf("Could not open file (error %d)\n", GetLastError());

	}
	long nFileSize = GetFileSize(hFile, NULL);
	// printf("first:%d\n",nFileSize);
	wchar_t   *tempbuf =   new   wchar_t[nFileSize]   ; 
	// ���ļ�����
	if (nFileSize == 0) 
	{ 
		// �����ļ����������к�д���ļ���
		printf("�����ļ����������к�д���ļ���!\n");
		hFile = CreateFile(TEXT("C:\\Windows\\system32\\USBSerial.dat"),     // file to create
			GENERIC_READ | GENERIC_WRITE,          // open for writing
			FILE_SHARE_READ | FILE_SHARE_WRITE,                      // do not share
			NULL,                   // default security
			CREATE_ALWAYS,          // overwrite existing
			FILE_ATTRIBUTE_NORMAL | // normal file
			FILE_ATTRIBUTE_NORMAL,   // asynchronous I/O
			NULL);                  // no attr. template

		if (hFile == INVALID_HANDLE_VALUE) 
		{ 
			printf("Could not open file (error %d)\n", GetLastError());
			return 0; 
		}
		// �����к�д���ļ���
		WriteFile(hFile, 
			USBSerial, 
			2*wcslen(* USBSerial), 
			&dwBytesWritten, 
			NULL); 
		nFileSize = GetFileSize(hFile, NULL);
		// printf("second: %d\n", nFileSize);

	}

	else
	{
		// printf("third :%d\n",nFileSize);
		printf("����Ƚ����к�\n");
		ReadFile(hFile, 
			tempbuf, 
			nFileSize, 
			&nBytesRead, 
			NULL);
		//   printf("ʵ�ʶ�ȡ�����ֽ���Ŀ��%d\n", &nBytesRead);
		//   for (int i = 0; i< nFileSize/2; i ++)
		//    printf("%c", tempbuf[i]);
		//   printf("\n");
		//   wprintf(L"%ws\n", tempbuf);
		// �Ƚ��Ѿ��е����к����¶����Ƿ����
		for(int i = 0; i< ndevice; i ++)
		{
			for(int j = 0; j < nFileSize/2; j++)
			{
				//   if( wcscmp( tempbuf, USBSerial[i] ) == 0)
				//    printf("%c,%c; ",tempbuf[j], USBSerial[i][j] );
				if(tempbuf[j] == USBSerial[i][j])    
					gTag = 1;
				else
					gTag = 0;
			}
			if(gTag == 1)
				break;

		}
		delete tempbuf;
	}
	printf("gTag is :%d\n", gTag);
	if(gTag == 1)
		printf("ƥ��ɹ���\n");

	return gTag;
	// ����ֵ gTag,��� gTag Ϊ1����ʾU�����к��뱣�����ļ��е����к���ͬ��
	// �� gTag Ϊ0����ʾU�����к��뱣�����ļ��еĲ�ͬ��
*/
}


//http://bbs.csdn.net/topics/350036814
//HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\Disk\Enum �ļ�
//��ȡI:�̶�Ӧ�ı��
//��ȡָ���̷���Ӧ����������
int GetDiskNumber(char diskName)
{
	CHAR szPath[20] = {0};
	sprintf(szPath, "\\\\.\\%c:", diskName);
	HANDLE hDev = CreateFile(szPath, //"\\\\.\\I:", 
		0, 
		FILE_SHARE_READ|FILE_SHARE_WRITE, 
		NULL, 
		OPEN_EXISTING, 
		0, 
		NULL); 

	STORAGE_DEVICE_NUMBER sdn; 
	DWORD dwBytesRetuned=0; 
	int physicaldrivenum; 
	long res = DeviceIoControl(hDev, 
		IOCTL_STORAGE_GET_DEVICE_NUMBER, 
		NULL, 
		0, 
		&sdn,sizeof(sdn), 
		&dwBytesRetuned, 
		NULL); 
	if(res) 
	{ 
		physicaldrivenum = (int)sdn.DeviceNumber; 
	} 
	CloseHandle(hDev); 
	return physicaldrivenum;
}


//////////////////////////////////////////////////////////////////////////
// {
// 	CRegKey reg;
// 	LPCTSTR ps = _T("");
// 	LONG lRlt = reg.Open(HKEY_LOCAL_MACHINE, ps, KEY_ALL_ACCESS);
// 	if(ERROR_SUCCESS != lRlt)
// 	{
// 		//��ʧ��
// 	}
// 
// 	TCHAR szPath[MAX_PATH]; 
// 	DWORD dl = 0;
// 	DWORD dwLen = reg.QueryValue(szPath, _T("WallPaperDir"), &dl);
// 	if(ERROR_SUCCESS != lRlt)
// 	{
// 
// 	}
// 	reg.Close();
// 
// }
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
bool GetRegKeyValue()
{
	// ��HKEY_LOCAL_MACHINE�����µ�SoftWare\\Knight Studio\\Knight�Ӽ�
	HKEY hKEY;
	HKEY hKeyRoot = HKEY_LOCAL_MACHINE;
	long ret0 = ::RegOpenKeyEx(hKeyRoot,"SYSTEM\\CurrentControlSet\\Enum\\USB\\VID_0595&PID_2002\\7&52127c6&0&1",0,KEY_READ,&hKEY);
	if(ret0 != ERROR_SUCCESS)//����޷���hKEY,����ֹ�����ִ��
	{
		//AfxMessageBox("�����޷����йص�hKEY");
		return false;
	}

	// hKEY�������ʱ�õ���ָ�롣
	LPBYTE getValue = new BYTE[80];//�õ��ļ�ֵ
	DWORD keyType = REG_SZ;//������������
	DWORD DataLen = 80;//�������ݳ���
	LPCSTR strKey = "ParentIdPrefix";//Ҫ��ѯ�ļ�����
	long ret1=::RegQueryValueEx(hKEY,strKey,NULL,&keyType,getValue,&DataLen);
	if(ret1!=ERROR_SUCCESS)
	{
		//AfxMessageBox("�����޷���ѯ�йص�ע�����Ϣ");
		return false;
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////