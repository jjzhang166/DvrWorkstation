// FtpFile.h : FtpFile DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CFtpFileApp
// �йش���ʵ�ֵ���Ϣ������� FtpFile.cpp
//

class CFtpFileApp : public CWinApp
{
public:
	CFtpFileApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
