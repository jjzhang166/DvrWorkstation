
// DvrTool.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CDvrToolApp:
// �йش����ʵ�֣������ DvrTool.cpp
//

class CDvrToolApp : public CWinApp
{
public:
	CDvrToolApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CDvrToolApp theApp;