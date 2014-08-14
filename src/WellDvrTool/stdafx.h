// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define WIN32_LEAN_AND_MEAN	
#define _CRT_SECURE_NO_DEPRECATE

// Required for VS 2008 (fails on XP and Win2000 without this fix)
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#endif

#include <stdio.h>
#include <tchar.h>
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            //  �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ
#endif

#include <afx.h>
#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ
#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <iostream>

#include <windows.h>
#include <objbase.h>

// Windows Header Files:
//#include <windows.h>

// C RunTime Header Files
//#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


#include <UIlib.h>

//using namespace DuiLib;

#ifdef _DEBUG
#   ifdef _UNICODE
#       pragma comment(lib, "..\\..\\lib\\Duilib\\DuiLib_ud.lib")
#   else
#       pragma comment(lib, "..\\..\\lib\\Duilib\\DuiLib_d.lib")
#   endif
#else
#   ifdef _UNICODE
#       pragma comment(lib, "..\\..\\lib\\Duilib\\DuiLib_u.lib")
#   else
#       pragma comment(lib, "..\\..\\lib\\Duilib\\DuiLib.lib")
#   endif
#endif

#pragma comment(lib, "UI43API.LIB")

// TODO: reference additional headers your program requires here
