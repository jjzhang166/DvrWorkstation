// App.cpp : Defines the entry point for the application.
//
#include "stdafx.h"

#ifdef DEBUG
#include "MiniDumper.h"
CMiniDumper g_miniDumper( true );
#endif

#include "CMainFrameWnd.h"
#include "ShareData.h"
#include "RegisterWnd.h"
#include "chmzc.h"
#include "ftp/FtpHelper.h"
#include "network/SockInit.h"

static void ParseCmdLine(LPSTR lpCmdLine);

CString GetAppPath()
{
	CString path;   
	GetModuleFileName(NULL,path.GetBufferSetLength(MAX_PATH),MAX_PATH);   
	int pos = path.ReverseFind('\\');
	path = path.Mid(0, pos);
	return path;
}

//���ֻ��һ��ʵ������
BOOL CheckOneInstance()
{
	HANDLE hMutex = CreateMutex(NULL, TRUE, DVR_INSTANCE);
	if(hMutex != NULL)
	{
		if(GetLastError() == ERROR_ALREADY_EXISTS)
		{
			//�رս��̻�����
			CloseHandle(hMutex);
			hMutex = NULL;
			return FALSE;
		}
	}
	return TRUE;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if(!CheckOneInstance())
		return 0;

    CPaintManagerUI::SetInstance(hInstance);
    CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("skin"));
    CPaintManagerUI::SetResourceZip(_T("skin.zip"));

	LOGS(_T("************���ϵͳע����Ϣ****************"));
	//���ע��
	while(!IsRegister())
	{
		CRegisterWnd* pWnd = new CRegisterWnd;
		pWnd->Create(NULL, _T(""), UI_WNDSTYLE_DIALOG, 0, 0, 0, 0, 0, NULL);
		pWnd->CenterWindow();
		pWnd->ShowModal();

		if(!pWnd->IsRegister())
		{
			LOGS(_T("ϵͳδ�Ϸ�ע��"));
			return 0;
		}
	}
	LOGS(_T("ϵͳ�Ϸ�ע�ᣬ����ϵͳ..."));

	SOCKSUPPORT;

    HRESULT Hr = ::CoInitialize(NULL);
    if( FAILED(Hr) ) return 0;

	ParseCmdLine(lpCmdLine);

    CMainFrameWnd* pFrame = new CMainFrameWnd();
    if( pFrame == NULL ) return 0;

	pFrame->SetInstance(hInstance);

    pFrame->Create(NULL, _T(""), UI_WNDSTYLE_FRAME, 0L, 0, 0, 1024, 738);
    pFrame->CenterWindow();

    ::ShowWindow(*pFrame, SW_SHOWMAXIMIZED);

    CPaintManagerUI::MessageLoop();

    ::CoUninitialize();
    return 0;
}

//���������������-d�ͽ��붨λģʽ
static void ParseCmdLine(LPSTR lpCmdLine)
{
	SHAREDATA.g_dbgLevel = WORKING;
	if(lpCmdLine == NULL)
	{
		LOGS(_T("���빤��ģʽ"));
		return;
	}

	// Process command line
	int cmdLen  = strlen(lpCmdLine);
	if( cmdLen > 0 && cmdLen < 3 )
	{
		if( lpCmdLine[0] == _T('/') || lpCmdLine[0] == _T('-') )
		{
			if( lpCmdLine[1] == _T('d') || lpCmdLine[1] == _T('D') )
			{
				SHAREDATA.g_dbgLevel = LOCATION;
				LOGS(_T("���붨λģʽ"));
				//ɾ��PORT��Ϣ
				WritePrivateProfileString(_T("PORT"),NULL,NULL,GetAppPath() + gc_Config);
				return;
			}
		}
	}
	LOGS(_T("���빤��ģʽ"));
}