#include "stdafx.h"
#include "ConfigBase.h"
#include "FileBackUp.h"

ConfigBase::ConfigBase(void)
{
}


ConfigBase::~ConfigBase(void)
{
}

CString ConfigBase::GetAppPath()
{
	CString path;   
	GetModuleFileName(NULL,path.GetBufferSetLength(MAX_PATH),MAX_PATH);   
	int pos = path.ReverseFind('\\');
	path = path.Mid(0, pos);
	return path;
}

CString ConfigBase::GetConfigPath()
{
	return _T("");
}

CString ConfigBase::GetConfigBackUpPath()
{
	return _T("");
}

void ConfigBase::Init()
{

}

void ConfigBase::InitFromFile()
{

}

void ConfigBase::SaveToFile()
{

}


//////////////////////////////////////////////////////////////////////////
//�ڶ�λ��ɺ󣬱������������ļ�
//�ر����ʱ����һ�£����ú󲻻���Ч��
//////////////////////////////////////////////////////////////////////////
bool ConfigBase::BackUp()
{
	CString strFile(GetConfigPath());
	CString strFileBack(GetConfigBackUpPath());
	SetFileAttributes(strFileBack,FILE_ATTRIBUTE_NORMAL);
	bool bRlt = CFileBackUp::BackUp(strFile.GetString(), strFileBack.GetString());
	DWORD dw = 0;
	if(bRlt)
		//���ñ����ļ�����
		SetFileAttributes(strFileBack,FILE_ATTRIBUTE_HIDDEN);
	else
		dw = GetLastError();
	return bRlt;
}

//////////////////////////////////////////////////////////////////////////
//ÿ��������ɺ�ָ������ļ�
//////////////////////////////////////////////////////////////////////////
bool ConfigBase::Restore()
{
	CString strFile(GetConfigPath());
	CString strFileBack(GetConfigBackUpPath());
	SetFileAttributes(strFileBack,FILE_ATTRIBUTE_NORMAL);
	wstring sf = strFile.GetBuffer();
	wstring sfb = strFileBack.GetBuffer();
	bool bRtl = CFileBackUp::BackUp( sfb, sf);
	SetFileAttributes(strFileBack,FILE_ATTRIBUTE_HIDDEN);
	return bRtl;
}