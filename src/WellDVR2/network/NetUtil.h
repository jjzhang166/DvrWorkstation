//////////////////////////////////////////////////////////////////////////
//Describe:
//		���繤����
//Date: [2014/11/16 yjt]
//History:
//////////////////////////////////////////////////////////////////////////
#pragma once
#include <string>
using namespace std;



class CNetUtil
{
public:
	CNetUtil(void);
	~CNetUtil(void);

public:
	static wstring GetLocalIpAddr();
};
