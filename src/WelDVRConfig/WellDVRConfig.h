
// WelDVRConfig.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CWelDVRConfigApp:
// �йش����ʵ�֣������ WelDVRConfig.cpp
//

class CWelDVRConfigApp : public CWinApp
{
public:
	CWelDVRConfigApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CWelDVRConfigApp theApp;