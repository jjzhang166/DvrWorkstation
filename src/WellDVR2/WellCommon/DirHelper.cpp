#include "StdAfx.h"
#include "DirHelper.h"
#include "CharSetHelper.h"
#include <io.h>

#include <shlwapi.h>//ͷ�ļ�

#include <list>
using namespace std;

CDirHelper::CDirHelper(void)
{
}

CDirHelper::~CDirHelper(void)
{
}

/***************************************************************************************
*��    ��: ��ȡӦ�ó���ĵ�ǰĿ¼
*
*�����б�:
*
*�� �� ֵ:
*
*ʱ������: created by yjt [2012/10/26 8:48]
*
*�޸ļ�¼:
*
***************************************************************************************/
CString CDirHelper::GetCurrentPath()
{
	TCHAR szFilePath[MAX_PATH] = {0};
	GetModuleFileName(NULL,szFilePath,MAX_PATH);
	(_tcsrchr(szFilePath, _T('\\')))[1] = 0;
	return szFilePath;
}

/***************************************************************************************
*��    ��: ��ȡӦ�ó���Ŀ¼
*
*�����б�:
*
*�� �� ֵ:
*
*ʱ������: created by yjt [2012/10/26 8:48]
*
*�޸ļ�¼:
*
***************************************************************************************/
string CDirHelper::GetAppPath()
{
	wchar_t appPath[MAX_PATH] = {0};
	GetModuleFileName(NULL, appPath, MAX_PATH);
	string strAp = CCharSetHelper::w2c(appPath).c_str();
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];

	_splitpath(strAp.c_str(), drive, dir, NULL,NULL);
	strAp = drive;
	strAp += dir;
	return strAp;
}

/***************************************************************************************
*��    ��:	����Ŀ¼
*				1. �Ѿ����ڷ���true
*				2. �����ڣ��ݹ鴴��
*
*�����б�:
*
*�� �� ֵ:
*
*ʱ������: created by yjt [2012/10/26 9:06]
*
*�޸ļ�¼:
*
***************************************************************************************/
bool CDirHelper::CreateDir(const CString& strPath)
{	
	CString strTmp(strPath);

	list<CString> dirs;
	dirs.push_back(strTmp);

	int i = strTmp.ReverseFind('\\');

	//����ÿһ��Ŀ¼
	while(i > 0)
	{
		if(DirExits(strTmp))
			break;
		strTmp = strTmp.Mid(0, i);
		dirs.push_back(strTmp);
		i = strTmp.ReverseFind('\\');
	}

	try{
		//����Ŀ¼
		list<CString>::reverse_iterator itor = dirs.rbegin();
		for ( ; itor != dirs.rend(); ++itor)
		{
			::CreateDirectory((*itor), NULL);
		}
	}
	catch(...)
	{
		return false;
	}
	return true;	
}

/***************************************************************************************
*��    ��: �ж�Ŀ¼�Ƿ����
*
*�����б�:
*
*�� �� ֵ:
*
*ʱ������: created by yjt [2012/10/26 9:52]
*
*�޸ļ�¼:
*
***************************************************************************************/
// bool CDirHelper::DirExits(const CString& strDir)
// {
// 	bool brlt = true;
// 	WIN32_FIND_DATA fd;
// 	HANDLE hd = FindFirstFile(strDir, &fd);
// 	if(hd == INVALID_HANDLE_VALUE && (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
// 		brlt = false;
// 
// 	FindClose(hd);
// 	return brlt;
// }

bool CDirHelper::DirExits(const CString& strDir)
{
	if (PathFileExists(strDir))
	{
		return true;
	}
	return false;
}



/***************************************************************************************
*��    ��:	��ȡָ��·���µ������ļ������б�
*				1. �ļ��к���
*�����б�: strPath [IN]		--		ָ��·�� [d:\\test]
*				strFileType [IN]	--		�ļ������� [xml/jpg/ini ....]
*
*�� �� ֵ:	vector<string>	--	�ļ������б�
*
*ʱ������: created by yjt [2012/11/28 16:49]
*
*�޸ļ�¼:
*
***************************************************************************************/
vector<string> CDirHelper::GetPathFilesName(const CString& strPath, const CString& strFileType)
{
	CString strPathWid;
	strPathWid.Format(_T("%s\\*.%s"), strPath, strFileType);
	string strPW = W2C(strPathWid.GetBuffer(strPathWid.GetLength()));
	strPathWid.ReleaseBuffer();

	vector<string> vecFilesName;

	struct _finddata_t c_file;
	long hFile;
	if((hFile = _findfirst(strPW.c_str(), &c_file)) == -1L)
		return vecFilesName;
	
	do 
	{
		vecFilesName.push_back(c_file.name);
	} while (_findnext(hFile, &c_file) == 0);

	return vecFilesName;
}