#include "StdAfx.h"
#include "FtpTransThread.h"
#include <afxinet.h>

#include "FtpHelper.h"

CFtpTransThread::CFtpTransThread(void)
{
	nBufSize = 2000000;
	buffer = new char[nBufSize];
}

CFtpTransThread::~CFtpTransThread(void)
{
	delete buffer;
}


UINT CFtpTransThread::DoWork()
{
	SetTransOverFlag(false);
	bool ret = FtpTransProc();
	SetTransOverFlag(true);
	m_cs.Lock();
	m_bFailed = !ret;
	m_cs.Unlock();

	/*if(m_pCompleteEvent!=NULL)
	m_pCompleteEvent->SetEvent();*/
	m_ctrl->CheckComplete();
	return 0;
}

//����Ftp�ļ���ʧ�ܷ���false,�ɹ�����true
bool CFtpTransThread::FtpTransProc(void)
{
	//��ʼ����
	bool ret = false;
	CInternetSession session;
	CFtpConnection *pFtpCon = NULL;
	CInternetFile *pInetFile = NULL;
	m_nFileTransSize = 0;
	m_nFileSize = 0;
	int len=0;
	try
	{
		CFile localFile;
		if(!localFile.Open(m_strSourceFile,CFile::modeRead|CFile::shareDenyNone))
			return false;

		//�����ļ���С
		CFileStatus fileStatus;
		localFile.GetStatus(fileStatus);
		m_nFileSize = fileStatus.m_size;
		pFtpCon = session.GetFtpConnection(m_ftpIp,m_ftpUser,
			m_ftpPwd,m_ftpPort,true);
		//����newDir��date����Ŀ¼
		CString new_dir;
		new_dir = m_strNewDir;
		if(m_strNewDir != _T(""))
		{
			if(!CreateFTPDirectory(pFtpCon, m_strNewDir))
			//if(!pFtpCon->CreateDirectory(m_ftpPath+m_strNewDir))
			{
				bool bError = false;
				DWORD err = GetLastError();
				if(err == ERROR_INTERNET_EXTENDED_ERROR)
				{
					DWORD dwError;
					TCHAR buf[200];
					DWORD size=200;	
					InternetGetLastResponseInfo(&dwError,buf,&size);
					CString strError(buf);
					if(strError.Left(3) == _T("550") )
						bError = false;  //Ŀ¼����Ѿ����ڣ�����Ϊû�д���
					else
						bError = true;
				}
				if(bError)
				{
					if(pFtpCon != NULL ) delete pFtpCon;
					if(pInetFile != NULL) delete pInetFile;
					return false;
				}
			}
			new_dir += _T("/");
		}
		if(m_strDate != _T(""))
		{
			if(!pFtpCon->CreateDirectory(m_ftpPath+new_dir+m_strDate))
			{
				bool bError = false;
				DWORD err = GetLastError();
				if(err == ERROR_INTERNET_EXTENDED_ERROR)
				{
					DWORD dwError;
					TCHAR buf[200];
					DWORD size=200;	
					InternetGetLastResponseInfo(&dwError,buf,&size);
					CString strError(buf);
					if(strError.Left(3) == _T("550") )
						bError = false;  //Ŀ¼����Ѿ����ڣ�����Ϊû�д���
					else
						bError = true;
				}
				if(bError)
				{
					if(pFtpCon != NULL ) delete pFtpCon;
					if(pInetFile != NULL) delete pInetFile;
					return false;
				}
			}
		}


		//�ж��Ƿ�����,ȡ���Ѿ�����ĳ��ȣ����Է��������ļ���СΪ׼��������log.xml�е�sentΪ׼)
		if(m_isContiue)
		{
			m_strDestFile=m_ftpPath+m_strDestFile;
			try
			{
				m_nFileTransSize+= GetFtpFileSize(pFtpCon,m_strDestFile);
				localFile.Seek(m_nFileTransSize,CFile::begin);
				pInetFile=pFtpCon->Command(_T("APPE ") + m_strDestFile,CFtpConnection::CmdRespWrite);
			}
			catch (CInternetException* e)
			{
				pInetFile = pFtpCon->OpenFile(m_strDestFile,GENERIC_WRITE);	
			}			
		}
		else
		{
			pInetFile = pFtpCon->OpenFile(m_strDestFile,GENERIC_WRITE);	
		}

		while( len=localFile.Read(buffer,nBufSize))
		{
			pInetFile->Write(buffer,len);
			m_cs.Lock();
			m_nFileTransSize += len;
			m_cs.Unlock();
			if(m_bForceStop)
				break;
		}
		//messageDocument.DeleteNode(messageDocument.currentNode);
			ret=true;
	}
	catch(CInternetException *e)
	{
// 		CString str;
// 		str.Format(_T("error code = %d"), GetLastError());
// 		AfxMessageBox(str);

		e->Delete();
		ret = false;
	}
	catch(CFileException *e)
	{
// 		CString str;
// 		str.Format(_T("error code = %d"), GetLastError());
// 		AfxMessageBox(str);

		e->Delete();
		ret = false;
	}
	if(pFtpCon != NULL ) delete pFtpCon;
	if(pInetFile != NULL) delete pInetFile;
	return ret;
}

void CFtpTransThread::SetFtpInfo(CString ip,UINT port, CString user, CString pwd,CString path)
{
	m_ftpIp = ip;
	m_ftpPwd = pwd;
	m_ftpUser = user;
	m_ftpPort = port;
	m_ftpPath = path+_T("/");
}


void CFtpTransThread::TransFile(CString srcFile, CString destFile,CString newDir,CString date,CString sharepath,CEvent *pCompleteEvent)
{
	m_cs.Lock();
	m_strSourceFile = srcFile;
	m_strDestFile = destFile;
	m_pCompleteEvent = pCompleteEvent;
	m_strNewDir = newDir;
	m_strDate = date;
	m_strSharePath = sharepath;
    m_isContiue=false;
	m_cs.Unlock();
	Begin();
}
void CFtpTransThread::TransFileContiue(CString srcFile, CString remoteFile,CEvent *pCompleteEvent,BOOL isContiue)
{
	m_cs.Lock();
	m_strSourceFile = srcFile;
		m_strDestFile = remoteFile;
	m_pCompleteEvent = pCompleteEvent;
	m_isContiue=isContiue;
	m_cs.Unlock();
	Begin();
}
bool CFtpTransThread::IsFailed(void)
{
	bool ret;
	m_cs.Lock();
	ret = m_bFailed;
	m_cs.Unlock();
	return ret;
}

bool CFtpTransThread::IsTransOver(void)
{
	bool ret;
	m_cs.Lock();
	ret = m_bTransOver;
	m_cs.Unlock();
	return ret;
}

void CFtpTransThread::SetTransOverFlag(bool value) //���ô�����ɱ���λ
{
	m_cs.Lock();
	m_bTransOver = value;
	m_cs.Unlock();
}

LONGLONG CFtpTransThread::GetFtpFileSize(CFtpConnection* pFtpCon, CString strFtpFile)
{
	CFtpFileFind   ftpFind(pFtpCon); 
	LONGLONG filelen = 0;
	if(ftpFind.FindFile(strFtpFile)) 
	{ 
		ftpFind.FindNextFile(); 
		filelen =   ftpFind.GetLength(); 
	} 
	ftpFind.Close(); 
	return filelen;
}

//����Ŀ¼
//�Ӹ�Ŀ¼����һ����ʼ����
//	����һ���󣬰��´���������Ϊ��ǰĿ¼
//	ȫ��������ɺ�ص�����
BOOL CFtpTransThread::CreateFTPDirectory(CFtpConnection* pFtpCon, const CString& strPath1)
{
	if(!pFtpCon)
		return false;

	CString strPath = strPath1;
	size_t pos = strPath.Find("\\");
	CString strT;

	BOOL bRlt = TRUE;
	while(pos >= 0 && pos < strPath.GetLength())
	{
		strT = strPath.Mid(0, pos);
		bRlt |= pFtpCon->CreateDirectory(strT);
		pFtpCon->SetCurrentDirectory(strT);
		strPath = strPath.Right(strPath.GetLength() - pos - 1);
		pos = strPath.Find("\\");
	}
	if(strPath.GetLength() > 0)
	{
		//�������һ��Ŀ¼
		pFtpCon->CreateDirectory(strPath);
	}
	pFtpCon->SetCurrentDirectory(m_ftpPath);
	return bRlt;
}