/********************************************************************
	created:	2012/10/26 8:46  
	filename: 	DirHelper.h
	author:		yjt
	purpose:		Ŀ¼������
*********************************************************************/
#pragma once

#define IVS_COMM_EXPORTS

#ifdef IVS_COMM_EXPORTS
#define IVS_COMM_API __declspec(dllexport)
#else 
#define IVS_COMM_API __declspec(dllimport)
#endif

#include <vector>
#include <string>
using namespace std;

class IVS_COMM_API CDirHelper
{
public:
	CDirHelper(void);
	~CDirHelper(void);

	//��ȡ��ǰĿ¼--Ӧ�ó���
	static CString GetCurrentPath();

	//��ȡӦ�ó���·��
	static string GetAppPath();

	//����Ŀ¼
	static bool CreateDir(const CString& strPath);

	//�ж�Ŀ¼�Ƿ����
	static bool DirExits(const CString& strDir);

	//��ȡĿ¼�µ�ָ�����͵��ļ����б�
	static vector<string> GetPathFilesName(const CString& strPath, const CString& strFileType);
};
