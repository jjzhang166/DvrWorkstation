//////////////////////////////////////////////////////////////////////////
//Describe: Socket��ʼ������ز���
//Date:	created by yjt 2014-07-07
//History:
//////////////////////////////////////////////////////////////////////////
#pragma once

#define SOCKSUPPORT CSockInit::Instance()

class CSockInit
{
private:
	CSockInit(void);
	~CSockInit(void);

public:
	static CSockInit& Instance();

private:
	bool Init();
	void UnInit();
};

