#pragma once
#include "NetClient.h"

class CNetUdp :
	public CNetClient
{
public:
	CNetUdp(void);
	virtual ~CNetUdp(void);

	//��������
	virtual int Send(const char* buf, int nLen);

	//��������
	virtual int Recv(char* buf, int nLen);
};

