
// GetMachineCode.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CGetMachineCodeApp:
// �йش����ʵ�֣������ GetMachineCode.cpp
//

class CGetMachineCodeApp : public CWinApp
{
public:
	CGetMachineCodeApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CGetMachineCodeApp theApp;