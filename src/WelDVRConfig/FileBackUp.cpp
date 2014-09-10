#include "StdAfx.h"
#include "FileBackUp.h"
#include "CharSetHelper.h"

#define FILEMAX 100   

const CString gc_Config(_T("\\dvr_cfg.ini"));
const CString gc_Config_BackUp(_T("\\dvr_cfg.back"));
const CString gc_LocationConfig(_T("\\location.xml"));
const CString gc_LocationConfig_BackUp(_T("\\location.back"));

typedef struct _SRCFILE   
{   
	WIN32_FIND_DATA  fd;   
	bool bIsNew;   
}SRCFILE;   
typedef struct _DSTFILE   
{   
	WIN32_FIND_DATA  fd;   
	bool bIsMatch;   
}DSTFILE;   

SRCFILE SrcFiles[FILEMAX];   
DSTFILE DstFiles[FILEMAX];   

WIN32_FIND_DATA fd;   

CFileBackUp::CFileBackUp(void)
{
}


CFileBackUp::~CFileBackUp(void)
{
}


//////////////////////////////////////////////////////////////////////////
//����:
//		pSrcDir [IN]	--		Դ, ��: D:\\aaa\\*
//		pDstDir [IN]	--		���ݵ�����ȥ, ��: E:\\bbb\\*
//
//////////////////////////////////////////////////////////////////////////
bool CFileBackUp::BackUp(const wstring& src, const wstring& dst)
{
	return CopyFile(src.c_str(), dst.c_str(), FALSE);   

	bool bRet=TRUE;   
	int iSrcFile=0;   
	int iDstFile=0;   

	TCHAR* pSrcDir = const_cast<TCHAR*>(src.c_str());
	TCHAR* pDstDir = const_cast<TCHAR*>(dst.c_str());

    //��ԴĿ¼�±������е��ļ�������SrcFiles�ṹ����   
    HANDLE hFile=FindFirstFile(pSrcDir,&fd);   
    while(hFile!=INVALID_HANDLE_VALUE && bRet)   
    {   
        if(fd.dwFileAttributes==FILE_ATTRIBUTE_ARCHIVE)   
        {   
            SrcFiles[iSrcFile].fd=fd;   
            SrcFiles[iSrcFile].bIsNew=FALSE;   
            //out<<SrcFiles[iSrcFile].fd.cFileName<<endl;   
            iSrcFile++;   
        }   
        bRet=FindNextFile(hFile,&fd);   
    }   
       
    //��Ŀ��Ŀ¼�±������е��ļ�������DstFiles�ṹ����   
    bRet=TRUE;   
    hFile=FindFirstFile(pDstDir,&fd);   
    while(hFile!=INVALID_HANDLE_VALUE && bRet)   
    {   
        if(fd.dwFileAttributes==FILE_ATTRIBUTE_ARCHIVE)   
        {   
            DstFiles[iDstFile].fd=fd;   
            DstFiles[iDstFile].bIsMatch=FALSE;   
            iDstFile++;   
        }   
        bRet=FindNextFile(hFile,&fd);   
    }   
///////////////////////////////////////////////////////////////    
//  ���濪ʼ�Ƚ�ԴĿ¼��Ŀ��Ŀ¼�������ļ������뽨��ʱ��     //   
//  �ҳ�SrcFile����Щ�ļ���DstFile�ļ�ʱ���ϸ��磬           //   
//  �ͽ�bIsNew��ΪTRUE��ͬʱDstFile�ļ��д��ڣ�����SrcFile�� //   
//  �����ڣ��Ͱ�bMatch��ΪFalse                              //   
    for(int i=0;i<iSrcFile-1;i++)   
    {   
        bool bNull=TRUE;   
        for(int j=0;j<iDstFile-1;j++)   
        {   
            if(lstrcmpi(SrcFiles[i].fd.cFileName,DstFiles[j].fd.cFileName)==0)   
            {   
                DstFiles[j].bIsMatch=TRUE;   
                bNull=FALSE;   
                if(1==CompareFileTime(&SrcFiles[i].fd.ftCreationTime,&DstFiles[j].fd.ftCreationTime))   
            //  if(SrcFiles[i].fd.ftCreationTime.dwLowDateTime > DstFiles[j].fd.ftCreationTime.dwLowDateTime)   
                    SrcFiles[i].bIsNew=TRUE;   
                break;   
            }   
        }   
        if(bNull==TRUE)   
            SrcFiles[i].bIsNew=TRUE;   
    }   
   
   
    //����SrcFile��bIsNewλTRUE���ļ���DstFile��ȥ   
    for(int a=0;a<iSrcFile-1;a++)   
    {   
        if(SrcFiles[a].bIsNew)   
        {   
            CopyFile(SrcFiles[a].fd.cFileName,pDstDir,FALSE);   
        }   
    }   
       
    //ɾ��Ŀ����bMatchΪFALSE���ļ�   
    for (int b=0; b<iDstFile; b++)    
    {   
        if (!DstFiles[b].bIsMatch)   
        {   
            lstrcat(pDstDir, DstFiles[b].fd.cFileName);   
            DeleteFile(pDstDir);   
            // printf("delete %s \n", dest);   
        }   
    }   
   
    return 0;
}

bool CFileBackUp::BackUp(const string& src, const string& dst)
{
	return BackUp(S2WS(src), S2WS(dst));
}

bool CFileBackUp::FileCopyTo(CString source, CString destination, CString searchStr, BOOL cover)
{
	CString strSourcePath = source;
	CString strDesPath = destination;
	CString strFileName = searchStr;
	CFileFind filefinder;
	CString strSearchPath = strSourcePath + _T("\\") + strFileName;
	CString filename;
	BOOL bfind = filefinder.FindFile(strSearchPath);
	CString SourcePath, DisPath;

	bool bRlt = true;
	while (bfind)
	{
		bfind = filefinder.FindNextFile();
		filename = filefinder.GetFileName();
		SourcePath = strSourcePath + _T("\\") + filename;
		DisPath = strDesPath + _T("\\") + filename;
		bRlt |= CopyFile(SourcePath.GetString(), DisPath.GetString(), cover);
	}
	filefinder.Close();
	return bRlt;
}

CString GetAppPath()
{
	CString path;   
	GetModuleFileName(NULL,path.GetBufferSetLength(MAX_PATH),MAX_PATH);   
	int pos = path.ReverseFind('\\');
	path = path.Mid(0, pos);
	return path;
}

//////////////////////////////////////////////////////////////////////////
//�ڶ�λ��ɺ󣬱������������ļ�
//�ر����ʱ����һ�£����ú󲻻���Ч��
//////////////////////////////////////////////////////////////////////////
bool ConfigBackUp::BackUp()
{
	CString strFile(GetAppPath() + gc_Config);
	CString strFileBack(GetAppPath() + gc_Config_BackUp);
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
bool ConfigBackUp::Restore()
{
	CString strFile(GetAppPath() + gc_Config);
	CString strFileBack(GetAppPath() + gc_Config_BackUp);
	SetFileAttributes(strFileBack,FILE_ATTRIBUTE_NORMAL);
	wstring sf = strFile.GetBuffer();
	wstring sfb = strFileBack.GetBuffer();
	bool bRtl = CFileBackUp::BackUp( sfb, sf);
	SetFileAttributes(strFileBack,FILE_ATTRIBUTE_HIDDEN);
	return bRtl;
}