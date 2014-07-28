#pragma once
#include "NetTcpClient.h"

class CNetHttpClient :
	public CNetTcpClient
{
public:
	CNetHttpClient(void);
	virtual ~CNetHttpClient(void);

	//��������
	virtual int Send(const char* buf, int nLen);

	//��������
	virtual int Recv(char* buf, int nLen);

private:
	string POSTString(const char* hostName, const char* api, const char* parameters);
	string HttpString(const char* parames, int type);
};

