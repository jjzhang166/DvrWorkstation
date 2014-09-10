/************************************************************************/
/*
*���������������ࣨȫ�ֱ�����
*		1. �������п���ʵ�ַ��ʿ��ƣ������ȣ�
*/
/************************************************************************/
#pragma once

#include "DataDef.h"
#include "CMainFrameWnd.h"

#define SHAREDATA CShareData::Instance()

class CShareData
{
private:
	CShareData(void);

	~CShareData(void);
public:
	static CShareData& Instance();

private:
	void Init();
	void UnInit();

public:
	DVRPROP g_dvrProp;					//DVR ���й̶�����

	EM_WORK_LEVEL g_dbgLevel;						//����1:��λ������������ģʽ

	CMainFrameWnd* g_pMainFrame;

	CString g_strFileTarget;			//�洢�ļ���Ŀ���ַ, �� localhost
	CString g_strFilePath;				//�洢�ļ���Ŀ¼����d://well-ftp

	double g_dwTotalSpace;				//�ܿռ�(MB)
	double g_dwFreeSpace;				//ʣ��ռ�(MB)

	CString g_strDeviceId;				//�豸ID

	CString g_strWebIp;					//WebIP
	int g_nWebPort;						//WebPort

	int g_nDvrType;
};

