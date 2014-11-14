#include "stdafx.h"
#include "zcjm.h"
#include "chmzc.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <windef.h>

#include <iostream>
#include <sstream>
using namespace std;

#include <winsock2.h>
#include <iphlpapi.h>

#pragma comment(lib,"Iphlpapi.lib")

string GetAppPath()
{
	CString path;   
	GetModuleFileName(NULL,path.GetBufferSetLength(MAX_PATH),MAX_PATH);   
	int pos = path.ReverseFind('\\');
	path = path.Mid(0, pos);
	return ws2s(path.GetBuffer(0));
}

#pragma comment(lib, "netapi32.lib")

#define  ENCRYPTER_DECRYPTER_NUM  18

int getMAC(char * mac);

/**
* ��		����	 ���ܣ����ؼ��ܽ��
* ��		����	szSource [IN]	--	Դ
* 			szPassWord [IN]	--	����
*
* ʱ�����ߣ� 	created by yjt 2012-08-29
*/
string zcjm::jiami(const string& szSource, const string& szPassWord)
{
	if(szSource.empty() || szPassWord.empty())
		return NULL;

	unsigned char* ret = new unsigned char[szSource.length()];
	int ret_len = 0;
	if(RC4((const unsigned char*)szSource.c_str(), szSource.length(), (const unsigned char*)szPassWord.c_str(), 
		szPassWord.length(), ret, ret_len) == NULL)
		return NULL;
	string strRlt = ByteToHex(ret, ret_len);
	delete[] ret;
	return strRlt;
}


/**
* ��		����	 ���ܣ����ؽ��ܽ��
* ��		����	szSource [IN]	--	Դ
* 			szPassWord [IN]	--	����
*
* ʱ�����ߣ� 	created by yjt 2012-08-29
*/
string zcjm::jiemi(const string& szSource, const string& szPassWord)
{
	if(szSource.empty() || (szSource.size()%2 != 0) || szPassWord.empty())
		return NULL;

	unsigned char* pSrc = new unsigned char[szSource.length()];
	int len = HexToByte(szSource, &pSrc);

	int ret_len = 0;
	unsigned char* ret = new unsigned char[szSource.length() /2 + 1];
	if(RC4((const unsigned char*)pSrc, len / 2,  (const unsigned char*)szPassWord.c_str(), szPassWord.length(), ret, ret_len) == NULL)
		return NULL;
	ret[ret_len] = '\0';
	string strRlt((const char*)ret);
	delete[] ret;
	delete[] pSrc;
	return strRlt;
}

int zcjm::RC4(const unsigned char* data, int data_len, const unsigned char* key,
	int key_len,unsigned char* out, int& out_len)
{
	if (data == NULL || key == NULL || out == NULL)
		return NULL;

	unsigned char* mBox = new unsigned char[BOX_LEN];

	if(GetKey(key, key_len, mBox) == NULL)
		return NULL;

	int i=0;
	int x=0;
	int y=0;

	for(int k = 0; k < data_len; ++k)
	{
		x = (x + 1) % BOX_LEN;
		y = (mBox[x] + y) % BOX_LEN;
		swap_byte((unsigned char*)&mBox[x], (unsigned char*)&mBox[y]);
		out[k] = data[k] ^ mBox[(mBox[x] + mBox[y]) % BOX_LEN];
	}
	out_len = data_len;
	delete[] mBox;
	return -1;
}


int zcjm::GetKey(const unsigned char* pass, int pass_len, unsigned char* out)
{
	if(pass == NULL || out == NULL)
		return NULL;

	int i;
	for(i = 0; i < BOX_LEN; i++)
		out[i] = i;

	int j = 0;
	for(i = 0; i < BOX_LEN; i++)
	{
		j = (pass[i % pass_len] + out[i] + j) % BOX_LEN;
		swap_byte(&out[i], &out[j]);
	}
	return -1;
}

/**
* ������ �����ֽ�
*/
void zcjm::swap_byte(unsigned char* a, unsigned char* b)
{
	unsigned char swapByte;
	swapByte = *a;
	*a = *b;
	*b = swapByte;
}

/**
* ��	����	���ֽ���תΪʮ�������룬һ���ֽ�����ʮ�����ƣ��ڲ�Ϊ�ַ�������ռ�
* ��	��:	vByte [IN]	--	Ҫת�����ֽ�����
* ����ֵ��	16�������д�
* ʱ�����ߣ� created by yjt 2012-08-28
*/
string zcjm::ByteToHex(unsigned char* vByte, int vLen)
{
	if(vByte == NULL)
		return NULL;

	char* tmp = new char[vLen * 2 + 1];
	string strRlt;
	int tmp2;
	for (int i = 0; i < vLen; ++i)
	{
		tmp2 = (int)(vByte[i])/16;
		tmp[i*2] = (char)(tmp2+((tmp2>9)?'A'-10:'0'));
		tmp2 = (int)(vByte[i])%16;
		tmp[i*2 + 1] = (char)(tmp2+((tmp2>9)?'A'-10:'0'));
	}
	tmp[vLen*2] = '\0';
	strRlt = tmp;
	delete[] tmp;
	return strRlt;
}

/**
* ��	����	��ʮ�������ַ�����תΪ�ֽ��룬ÿ����ʮ�������ַ���Ϊһ���ֽ�
* 		1. ȡ��λ16������
* 		2. �ϳ��ֽ�
* ��	����	szHex [IN]		--	16���ƴ�
* ����ֵ���ֽڴ� 
* ʱ�����ߣ�	created by yjt 2012-08-29
*/
int zcjm::HexToByte(const string& szHex, unsigned char** vByte)
//string zcjm::HexToByte(const string& szHex)
{
	if(szHex.empty())
		return NULL;

	int iLen = szHex.size();
	if ( iLen <= 0 || 0 !=  iLen % 2 )
		return NULL;

	unsigned char* pbBuf = *vByte;
//	unsigned char* pbBuf = new unsigned char[iLen/2];
	int tmp1, tmp2;
	for (int i=0; i<iLen/2; i++)
	{
		tmp1 = (int)szHex[i*2] - (((int)szHex[i*2]>='A')?'A'-10:'0');

		if(tmp1 >= 16)
			return NULL;

		tmp2 = (int)szHex[i*2+1] - (((int)szHex[i*2+1]>='A')?'A'-10:'0');

		if(tmp2 >= 16)
			return NULL;

//		pbBuf[i] = (tmp1*16+tmp2);
// 	}
// 	string strRlt((const char*)pbBuf);
// 	delete[] pbBuf;
// 	return strRlt;

		*(pbBuf + i) = (tmp1*16 + tmp2);
	}

	return iLen;
}

string zcjm::GetCpuID()
{
	BYTE szSystemInfo[256]; // �ڳ���ִ����Ϻ󣬴˴��洢ȡ�õ�ϵͳ������
	UINT uSystemInfoLen = 0;

	// CPU ID
	BOOL bException = FALSE;
	BYTE szCpu[16]  = { 0 };
	UINT uCpuID     = 0U; 

	try
	{
		_asm 
		{
			mov eax, 0
			cpuid
			mov dword ptr szCpu[0], ebx
			mov dword ptr szCpu[4], edx
			mov dword ptr szCpu[8], ecx
			mov eax, 1
			cpuid
			mov uCpuID, edx
		}
	}
	catch(...)
	{
		bException = TRUE;
	}

	if( !bException )
	{
		CopyMemory( szSystemInfo + uSystemInfoLen, &uCpuID, sizeof( UINT ) );
		uSystemInfoLen += sizeof( UINT );
		uCpuID = strlen( ( char* )szCpu );
		CopyMemory( szSystemInfo + uSystemInfoLen, szCpu, uCpuID );
		uSystemInfoLen += uCpuID;

	}

	return (char*)szSystemInfo;
}

/**
* ��	����	��ȡ������Ϣ
*/
string zcjm::GetMACInfo(void)
{
#if _LINUX
	//��ȡ����16������ʽ��MAC���
	unsigned char macaddr[6];
	int i;
	int sock;
	struct sockaddr_in sin;
	struct ifreq ifr[16];			//�����������
	struct ifconf ifc;
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock < 0)
		return ""; 	//printf("get the socket file descriptor faild!\n");


	ifc.ifc_len = sizeof(ifr);
	ifc.ifc_buf = (caddr_t)ifr;
	if(!ioctl(sock, SIOCGIFCONF, (char*)&ifc) == -1)
	{
		cout << "get network card interface info is failed!" << endl;
		return "";
	}
	//�õ��ӿ�����
	int intrface = ifc.ifc_len / sizeof(struct ifreq);
	int retn = 0;
	//ѭ����ȡ�豸��IP��MAC
	while(intrface-- > 0)
	{
		//�豸����
		//	cout << "network card name:" << ifr[intrface].ifr_name << endl;

		//�ж���������
		if(!(ioctl(sock, SIOCGIFFLAGS, (char*)&ifr[intrface])))
		{
			if(ifr[intrface].ifr_flags & IFF_PROMISC)
			{
				puts("the interface is PROMISC");
				retn ++;
			}
		}
		else
		{
			char str[256];
			sprintf(str, "cpm: ioctl device %s", ifr[intrface].ifr_name);
			perror(str);
		}
		/*	
		//�ж�����״̬
		if(ifr[intrface].ifr_flags & IFF_UP)
		puts("the interface is UP");
		else
		puts("the interface status is DOWN");

		//��ȡ��ǰ������IP��ַ
		if(!(ioctl(sock, SIOCGIFADDR, (char*)&ifr[intrface])))
		{
		puts("IP address is:");
		puts(inet_ntoa(((struct sockaddr_in*)(&ifr[intrface].ifr_addr))->sin_addr));
		puts("");
		}
		else
		{
		char str[256];
		sprintf(str, "cpm: ioctl device %s", ifr[intrface].ifr_name);
		puts(str);
		}
		*/	
		if((ioctl(sock, SIOCGIFHWADDR, &ifr[intrface])) < 0)
		{
			cout << "get the network card addr faild!"<< endl;
			return "";
		}
		memcpy(macaddr, ifr[intrface].ifr_hwaddr.sa_data, 6);
		char macHex[15] = {0};
		sprintf(macHex, "%.2x%.2x%.2x%.2x%.2x%.2x", macaddr[0], macaddr[1],
			macaddr[2],macaddr[3],macaddr[4],macaddr[5]);
		close(sock);
		return macHex;
	}
#else

	char chMac[64] = {0};
	//PIP_ADAPTER_INFO�ṹ��ָ��洢����������Ϣ
	PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();
	//�õ��ṹ���С,����GetAdaptersInfo����
	unsigned long stSize = sizeof(IP_ADAPTER_INFO);
	//����GetAdaptersInfo����,���pIpAdapterInfoָ�����;����stSize��������һ��������Ҳ��һ�������
	int nRel = GetAdaptersInfo(pIpAdapterInfo,&stSize);
	//��¼��������
	int netCardNum = 0;
	//��¼ÿ�������ϵ�IP��ַ����
	int IPnumPerNetCard = 0;
	if (ERROR_BUFFER_OVERFLOW == nRel)
	{
		//����������ص���ERROR_BUFFER_OVERFLOW
		//��˵��GetAdaptersInfo�������ݵ��ڴ�ռ䲻��,ͬʱ�䴫��stSize,��ʾ��Ҫ�Ŀռ��С
		//��Ҳ��˵��ΪʲôstSize����һ��������Ҳ��һ�������
		//�ͷ�ԭ�����ڴ�ռ�
		delete pIpAdapterInfo;
		//���������ڴ�ռ������洢����������Ϣ
		pIpAdapterInfo = (PIP_ADAPTER_INFO)new BYTE[stSize];
		//�ٴε���GetAdaptersInfo����,���pIpAdapterInfoָ�����
		nRel=GetAdaptersInfo(pIpAdapterInfo,&stSize);    
	}
	if (ERROR_SUCCESS == nRel)
	{
		//���������Ϣ
		//�����ж�����,���ͨ��ѭ��ȥ�ж�
		while (pIpAdapterInfo)
		{
// 			cout<<"����������"<<++netCardNum<<endl;
// 			cout<<"�������ƣ�"<<pIpAdapterInfo->AdapterName<<endl;
// 			cout<<"����������"<<pIpAdapterInfo->Description<<endl;
// 			switch(pIpAdapterInfo->Type)
// 			{
// 			case MIB_IF_TYPE_OTHER:
// 				cout<<"�������ͣ�"<<"OTHER"<<endl;
// 				break;
// 			case MIB_IF_TYPE_ETHERNET:
// 				cout<<"�������ͣ�"<<"ETHERNET"<<endl;
// 				break;
// 			case MIB_IF_TYPE_TOKENRING:
// 				cout<<"�������ͣ�"<<"TOKENRING"<<endl;
// 				break;
// 			case MIB_IF_TYPE_FDDI:
// 				cout<<"�������ͣ�"<<"FDDI"<<endl;
// 				break;
// 			case MIB_IF_TYPE_PPP:
// 				printf("PP\n");
// 				cout<<"�������ͣ�"<<"PPP"<<endl;
// 				break;
// 			case MIB_IF_TYPE_LOOPBACK:
// 				cout<<"�������ͣ�"<<"LOOPBACK"<<endl;
// 				break;
// 			case MIB_IF_TYPE_SLIP:
// 				cout<<"�������ͣ�"<<"SLIP"<<endl;
// 				break;
// 			default:
// 
// 				break;
// 			}
// 			cout<<"����MAC��ַ��";
			for (DWORD i = 0; i < pIpAdapterInfo->AddressLength; i++)
			{
				sprintf(chMac + i * 2, "%02X", pIpAdapterInfo->Address[i]);
			
// 				if (i < pIpAdapterInfo->AddressLength-1)
// 				{
// 					printf("%02X-", pIpAdapterInfo->Address[i]);
// 				}
// 				else
// 				{
// 					printf("%02X\n", pIpAdapterInfo->Address[i]);
// 				}
// 				cout<<"����IP��ַ���£�"<<endl;
// 				//���������ж�IP,���ͨ��ѭ��ȥ�ж�
// 				IP_ADDR_STRING *pIpAddrString =&(pIpAdapterInfo->IpAddressList);
// 				do 
// 				{
// 					cout<<"�������ϵ�IP������"<<++IPnumPerNetCard<<endl;
// 					cout<<"IP ��ַ��"<<pIpAddrString->IpAddress.String<<endl;
// 					cout<<"������ַ��"<<pIpAddrString->IpMask.String<<endl;
// 					cout<<"���ص�ַ��"<<pIpAdapterInfo->GatewayList.IpAddress.String<<endl;
// 					pIpAddrString=pIpAddrString->Next;
// 				} while (pIpAddrString);
// 				pIpAdapterInfo = pIpAdapterInfo->Next;
// 				cout<<"--------------------------------------------------------------------"<<endl;
			}

			break;
		}

	}
	//�ͷ��ڴ�ռ�
	if (pIpAdapterInfo)
	{
		delete pIpAdapterInfo;
	}

	return chMac;
#endif
}

/**
* ��	����	��ȡϵͳ��Ϣ
* 	linux��ϵͳ���� + linux��Ӳ����Ϣ + linux������Ϣ + ��������MAC��Ϣ
* 	  linux                   x86_64              2.6.40.4-5.fc15.x86_64
*/
string zcjm::GetSysInfo(void)
{
#if _LINUX
	struct utsname un;
	if(uname(&un)!=0)
		return NULL;

	string strRlt;
	strRlt += un.sysname;
	strRlt += un.machine;
	strRlt += un.release;
	strRlt += GetMACInfo();
	return strRlt;
#endif
	SYSTEM_INFO sInfo;
	GetSystemInfo(&sInfo);

	ostringstream os;
	os << sInfo.dwActiveProcessorMask;
	os << sInfo.dwAllocationGranularity;
	os << sInfo.dwNumberOfProcessors;
	os << sInfo.dwOemId;
	os << sInfo.dwProcessorType;
	os << sInfo.dwNumberOfProcessors;
	os << GetMACInfo();
	return os.str();
}

/**
* ��	��:	����
*/
void zcjm::decrypter(char *a)
{
	int i;
	int len = strlen(a);
	for (i=0; i<len-1; i=i+2)
	{
		char c;
		c = a[i];
		a[i] = a[i+1];
		a[i+1] = c;
	}
	for (i = 0; a[i]; i++)
	{
		if(a[i]>='0' && a[i]<='9')
		{
			a[i]+=ENCRYPTER_DECRYPTER_NUM;
		}
		else if(a[i]>='B' && a[i]<='K')
		{
			a[i]-=ENCRYPTER_DECRYPTER_NUM;
		}
	}
}

void zcjm::encrypter(char* a)
{
	int i;
	for( i = 0; a[i]; ++i)
	{
		if(a[i] > '0' && a[i] <= '9')
			a[i] += ENCRYPTER_DECRYPTER_NUM;
		else if(a[i] >= 'B' && a[i] <= 'K')
			a[i] -= ENCRYPTER_DECRYPTER_NUM;
	}
	int len = strlen(a);
	for( i = 0; i < len -1; i=i+2){
		char c;
		c = a[i];
		a[i] = a[i+1];
		a[i+1] = c;
	}
}

/**
* ��	����	��ȡ������
* 		1. ��ȡMAC
* 		2. ��ȡȡϵͳ��Ϣ
* 		3. �����ֺϳɻ�����
* �� �� ֵ�� ��ȡ�ɹ����ػ����룬ʧ�ܷ���""
* ʱ�����ߣ� created by yjt 2012-08-29
*/
string zcjm::GetMachineCode()
{
	string strInfo = GetSysInfo();
	encrypter((char*)strInfo.c_str());
	return strInfo;
}

/**
* ��	����	��ȡע����
* ��	����	strMichineCode [IN]	--	�����루GetMichineCode���ص�)
* 			strPass [IN]			--	������,���Ϊ����ϵͳĬ�ϵļ�����
* �� �� ֵ��  ����ע����, �������ʧ�ܷ���""
* ʱ�����ߣ� created by yjt 2012-08-29
*/
string zcjm::GetRegisterCode(const string& strMichineCode, const string& strPass)
{
	string pass("@abc231@");
	if(strPass != "" && !strPass.empty())
		pass = strPass;
	return jiami(strMichineCode, pass);
}

/**
* ��	����	ע��
* 		1. ��ע����д���ļ�
* 		2. ��ע�����뵱ǰ����Ӳ����Ϣ���ɵ�ע����Ϣ���бȽ�
* 			2.1 �ѱȽϽ�����
* ��	����	strRegisterCode [IN]		--		ע����
* �� �� ֵ��	ע��ɹ�����true, ʧ�ܷ���false
* ʱ�����ߣ� created by yjt 2012-08-29
*/
//modified by yjt 2014-10-16 �����Ч��
bool zcjm::Register(const string& strRegisterCode)
{
	//��ȡ������
	string strCode = GetMachineCode();
	if(strCode == "" || strCode.empty())
		return false;
	//��ȡע����
	strCode = GetRegisterCode(strCode);
	if(strRegisterCode.length() > strCode.length())
	{
		string strTmp = strRegisterCode.substr(0, strCode.length());
		if(strTmp != strCode)
			return false;
	}
	else if(strCode != strRegisterCode)
		return false;

	//д�������ļ�
	string strAppPath;
	strAppPath = GetAppPath();
	//CDirHelper::GetAppPath(strAppPath);
	strAppPath += "/info.dll";

	//д��
	FILE *pFile = fopen(strAppPath.c_str(), "w+");
	if(pFile == NULL)
		return false;
	//д��ע����
	fwrite(strRegisterCode.c_str(), strRegisterCode.length(), 1, pFile);
	fclose(pFile);

	return true;
}

/**
* ��	����	�����Ƿ���ע��ɹ�--����ע�ᵫ�벻��
* 		1. ���ļ��ж�ȡע����
* 		2. ��ע�����뵱ǰ����Ӳ����Ϣ���ɵ�ע����Ϣ���бȽ�
* 			2.1 �ѱȽϽ�����
* �� �� ֵ��	�Ѿ�ע�᷵��true, û�з���false
* ʱ�����ߣ� created by yjt 2012-08-29
*/
bool zcjm::IsRegister()
{
	//���������ļ�
	string strAppPath;
	strAppPath = GetAppPath();
//	CDirHelper::GetAppPath(strAppPath);
	strAppPath += "/info.dll";

	//��
	FILE *pFile = fopen(strAppPath.c_str(), "r");
	if(pFile == NULL)
		return false;

	//��ȡ�ļ��ĳ���
	struct stat fs;
	stat(strAppPath.c_str(), &fs);
	int len = fs.st_size;
	if(len < 2)
		return false;

	//��ʼ��ȡע����
	char* pBuff = new char[len + 1];
	fread(pBuff, len, 1, pFile);
	pBuff[len] = '\0';
	fclose(pFile);

	string strCodeR= pBuff;
	delete[] pBuff;
	pBuff = NULL;

	//��ȡ������
	string strCode = GetMachineCode();
	if(strCode == "" || strCode.empty())
		return false;
	//��ȡע����
	strCode = GetRegisterCode(strCode);
	if(strCodeR.length() > strCode.length())
	{
		string strTmp = strCodeR.substr(0, strCode.length());
		if(strTmp != strCode)
			return false;

		//����ʱ��
		string pass("@abc231@");
		string strDt = jiemi(strCodeR, pass);
		string strS = strDt.substr(strDt.find('@') + 1, 8);
		string strE = strDt.substr(strDt.find('-') + 1, 8);
		CTime tm = CTime::GetCurrentTime();
		CTime tmS = String2Time(strS);
		CTime tmE = String2Time(strE);
		
		CTimeSpan tS = tm - tmS;
		CTimeSpan tE = tmE - tm;

		if(tS.GetDays() >= 0 && tE.GetDays() >= 0)
		{
			return true;
		}
		else
		{
			return false;
		}

	}
	else if(strCode != strCodeR)
	{
		return false;
	}

	return true;
}

CTime zcjm::String2Time(string&  strS)
{
	int a, b, c;   
	a = atoi(strS.substr(0, 4).c_str());
	b = atoi(strS.substr(4, 2).c_str());
	c = atoi(strS.substr(6, 2).c_str());  
	return CTime(a,b,c,0,0,0); 
}

/*
* ����--������ʾ��
*
int main()
{
string strCode = zcjm::GetMichineCode();
cout << "MichineCode : " << strCode << endl;
strCode = zcjm::GetRegisterCode(strCode);
cout << "register code: " << strCode << endl;

if(zcjm::Register(strCode))
cout << "register successful" << endl;
else
cout << "register code is not invalide! " << endl;

if(zcjm::IsRegister())
cout << "is has register" << endl;
else
cout << "no register " << endl;

return 0;
}
*/