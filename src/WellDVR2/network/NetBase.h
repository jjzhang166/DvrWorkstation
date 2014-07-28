//////////////////////////////////////////////////////////////////////////
//Describe:	��������
//Date: created by yjt 2014-07-07
//History:
//////////////////////////////////////////////////////////////////////////
#pragma once


class CNetBase
{
public:
	CNetBase(void);
	virtual ~CNetBase(void);

public:
	virtual bool Init() = 0;
	virtual void UnInit() = 0;

	//��������
	virtual int Send(const char* buf, int nLen);

	//��������
	virtual int Recv(char* buf, int nLen);

protected:
	SOCKET m_sock;
};

