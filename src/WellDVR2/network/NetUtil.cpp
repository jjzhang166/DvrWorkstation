#include "stdafx.h"
#include "NetUtil.h"
#include <Afxsock.h>
#include "../WellCommon/CharSetHelper.h"


CNetUtil::CNetUtil(void)
{
}


CNetUtil::~CNetUtil(void)
{

}

wstring CNetUtil::GetLocalIpAddr()
{
	if(!AfxSocketInit()) 
	{ 
		return _T(""); 
	}

	char szHostName[MAX_PATH + 1];
	gethostname(szHostName, MAX_PATH);			//�õ��������
	hostent *p = gethostbyname(szHostName);		//�Ӽ�������õ�������Ϣ
	if(p == NULL)
	{
		return _T("");
	}

	//��32λIPת��Ϊ�ַ���IP
	char *pIP = inet_ntoa(*(in_addr *)p->h_addr_list[0]);
	wstring wcsIp = S2WS(pIP);
	return wcsIp;
}
