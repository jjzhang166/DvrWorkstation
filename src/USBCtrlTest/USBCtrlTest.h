
// USBCtrlTest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CUSBCtrlTestApp:
// �йش����ʵ�֣������ USBCtrlTest.cpp
//

class CUSBCtrlTestApp : public CWinApp
{
public:
	CUSBCtrlTestApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CUSBCtrlTestApp theApp;