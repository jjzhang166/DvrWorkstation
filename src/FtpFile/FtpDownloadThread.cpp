#include "StdAfx.h"
#include "FtpDownloadThread.h"

#include "FtpHelper.h"

CFtpDownloadThread::CFtpDownloadThread(void)
{
	nBufSize = 2000000;
	buffer = new char[nBufSize];
	m_ctrl = NULL;
}

CFtpDownloadThread::~CFtpDownloadThread(void)
{
	delete []buffer;
}

void CFtpDownloadThread::SetFtpInfo(CString ip, UINT port, CString user, CString pwd)
{
	m_ftpIp = ip;
	m_ftpPort = port;
	m_ftpUser = user;
	m_ftpPwd = pwd;
}

UINT CFtpDownloadThread::DoWork(void)
{
	SetTransOverFlag(false);
	if(DownloadProc())
		m_bFailed = false;
	else m_bFailed = true;
	SetTransOverFlag(true);
	/*if(m_pEvent != NULL)
	m_pEvent->SetEvent();*/
	m_ctrl->CheckComplete();
	return 0;
}

bool CFtpDownloadThread::DownloadProc(void)
{
	/*
	//����ִ������
	//��ʼ����
	bool ret = false;
	CInternetSession session;
	CFtpConnection *pFtpCon = NULL;
	CInternetFile *pInetFile = NULL;
	m_cs.Lock();
	m_nFileTransSize = 0;
	m_nFileSize = 0;
	m_cs.Unlock();
	try
	{
		CFile localFile;
		pFtpCon = session.GetFtpConnection(m_ftpIp,m_ftpUser,
			m_ftpPwd,m_ftpPort,true);
		CString strAudioName,strAudioFileName;
		LONGLONG audioFileSize = 0;
		strAudioName = GetAudioFileName(pFtpCon,m_strFtpFile);
		int index;
		index = strAudioName.ReverseFind(_T('/'));
		strAudioFileName = strAudioName.Right(strAudioName.GetLength() - index -1);
			ULONGLONG fileSize = GetFtpFileSize(pFtpCon,m_strFtpFile);
		if(strAudioName != _T(""))
		{
		audioFileSize = GetFtpFileSize(pFtpCon,strAudioName);
		}

		//�����ļ���С
		m_cs.Lock();
		m_nFileSize = fileSize + audioFileSize;
		m_nFileTransSize = 0;
		m_cs.Unlock();

		//ȥ��guid,ȡ���10λ 
		m_strFileName = GetPureName(m_strFileName);
		strAudioFileName = GetPureName(strAudioFileName);



		CString vedioFileName;
		if(m_strRename == _T(""))
			vedioFileName = m_strFileName;
		else
		{
			//������Ƶ������
			int index = m_strFileName.ReverseFind(_T('.'));
			if(index != -1)
			{
				CString extName = m_strFileName.Right(m_strFileName.GetLength() - index - 1);
				vedioFileName = m_strRename + _T('.') + extName;
			}
			else
				vedioFileName = m_strFileName;

			//������Ƶ������
			index = strAudioFileName.ReverseFind(_T('.'));
			if(index != -1)
			{
				CString extName = strAudioFileName.Right(strAudioFileName.GetLength() - index -1);
				strAudioFileName = m_strRename + _T('.') + extName;
			}
		}

		//������Ƶ�ļ�

		if(!localFile.Open(m_strLocalFolder + vedioFileName ,CFile::modeWrite|CFile::modeNoTruncate|CFile::modeCreate|CFile::shareDenyNone))
			return false;

		//�ж��Ƿ�����,ȡ���Ѿ�����ĳ��ȣ����Ա������ļ���СΪ׼��������log.xml�е�sentΪ׼)
		if(m_isContiue)
		{
				//�����ļ���С

				CFileStatus fileStatus;
				localFile.GetStatus(fileStatus);
				m_nFileTransSize=fileStatus.m_size;
				localFile.SeekToEnd();
				CString pszOffset;
				pszOffset.Format(L"%d",m_nFileTransSize);
				HINTERNET hResponse;
				CString	strSeekOffsetCMD = L"RETR ";
			
				DWORD dwError;
				TCHAR databuf1[200];
						TCHAR databuf2[200];
								TCHAR databuf3[200];
								TCHAR databuf4[200];
									TCHAR databuf5[200];
				DWORD size=200;	


			/ *	pFtpCon->Command("cwd ",CFtpConnection::CmdRespNone); 
				InternetGetLastResponseInfo(&dwError,databuf1,&size);

				pFtpCon->Command("pwd",CFtpConnection::CmdRespNone);
				InternetGetLastResponseInfo(&dwError,databuf1,&size);* /


				pFtpCon->Command(L"TYPE I",CFtpConnection::CmdRespNone);
				InternetGetLastResponseInfo(&dwError,databuf1,&size);

				pFtpCon->Command(L"SIZE /"+m_strFtpFile,CFtpConnection::CmdRespNone);
				InternetGetLastResponseInfo(&dwError,databuf1,&size);

				pFtpCon->Command(L"PASV",CFtpConnection::CmdRespNone);
				InternetGetLastResponseInfo(&dwError,databuf1,&size);
			int dataPort=ParsePasv(databuf1);

			pFtpCon->Command(L"PASV",CFtpConnection::CmdRespNone);
			InternetGetLastResponseInfo(&dwError,databuf1,&size);
			 dataPort=ParsePasv(databuf1);
		
			SOCKET recSocket;
			sockaddr_in saServer;
			hostent* remoteHost;
			char* remoteIP;

			// Create a listening socket
			recSocket = socket(AF_INET, SOCK_STREAM, 0);

			// Get the local host information
			remoteHost = gethostbyname(m_ftpIp);
			remoteIP = inet_ntoa (*(struct in_addr *)*remoteHost->h_addr_list);

			// Set up the sockaddr structure
			saServer.sin_family = AF_INET;
			saServer.sin_addr.s_addr = inet_addr(remoteIP);
			saServer.sin_port = htons(dataPort);

			// Bind the listening socket using the
			// information in the sockaddr structure
			InternetGetLastResponseInfo(&dwError,databuf4,&size);
			int ret = connect( recSocket, (SOCKADDR*)&saServer, sizeof(SOCKADDR) );
			InternetGetLastResponseInfo(&dwError,databuf5,&size);
			if( ret == SOCKET_ERROR )  
			{
				InternetGetLastResponseInfo(&dwError,databuf1,&size);
				DWORD ERRORRRR;
				ERRORRRR =GetLastError();
				return FALSE;
			}


			pFtpCon->Command(L"REST "+pszOffset,CFtpConnection::CmdRespNone);
			InternetGetLastResponseInfo(&dwError,databuf2,&size);

			strSeekOffsetCMD=L"RETR /"+m_strFtpFile;
			pFtpCon->Command(strSeekOffsetCMD,CFtpConnection::CmdRespNone);

			InternetGetLastResponseInfo(&dwError,databuf3,&size);

			//�����ļ�
			int len;
			do
			{
				len = recv(recSocket,buffer,nBufSize,0);
				if(len<0)
					break;
				localFile.Write(buffer,len);
				m_cs.Lock();
				m_nFileTransSize += len;
				m_cs.Unlock();
				if(m_bForceStop)
					break;
			}while( len > 0 );
			localFile.Close();						
		}
		else
		{
			pInetFile = pFtpCon->OpenFile(m_strFtpFile,GENERIC_READ);
			int len;
			while( len=pInetFile->Read(buffer,nBufSize))
			{
				localFile.Write(buffer,len);
				m_cs.Lock();
				m_nFileTransSize += len;
				m_cs.Unlock();
				if(m_bForceStop)
					break;
			}
			localFile.Close();
			if(pInetFile != NULL) 
			{
				delete pInetFile;
				pInetFile = NULL;
			}
		}


		//������Ƶ�ļ�
		if(strAudioName != _T(""))
		{
			pInetFile = pFtpCon->OpenFile(strAudioName,GENERIC_READ);
			if(!localFile.Open(m_strLocalFolder + strAudioFileName ,CFile::modeWrite|CFile::modeCreate|CFile::shareDenyNone))
				return false;
			int len=0;
			while( len=pInetFile->Read(buffer,nBufSize))
			{
				localFile.Write(buffer,len);
				m_cs.Lock();
				m_nFileTransSize += len;
				m_cs.Unlock();
				if(m_bForceStop)
					break;
			}
			localFile.Close();
		}
		ret = true;
	}
	catch(CInternetException *e)
	{
		e->Delete();
		ret = false;

		//test error
		DWORD dwError;
		TCHAR buf[200];
		DWORD size=200;	
		InternetGetLastResponseInfo(&dwError,buf,&size);
		DWORD strError;
		strError= GetLastError();
	}
	catch(CFileException *e)
	{
		e->Delete();
		ret = false;
	}
	if(pFtpCon != NULL ) delete pFtpCon;
	if(pInetFile != NULL) delete pInetFile;
	return ret;
	
	*/
	
	return true;
}


void CFtpDownloadThread::DownloadFile(CString ftpPath, CString localFolder,CString rename,CEvent *pEvent)
{
	m_cs.Lock();
	m_strFtpFile = ftpPath;
	m_strLocalFolder = localFolder;
	m_strRename = rename;
	m_pEvent = pEvent;
	m_cs.Unlock();
	//��ȡ�ļ�������׺��
	int index;
	index=ftpPath.ReverseFind(_T('/'));
	m_strFileName = ftpPath.Right(ftpPath.GetLength() - index -1);
	index=m_strFileName.ReverseFind(_T('.'));
	if(index != -1)
	{
		m_strFileTitle = m_strFileName.Left(index);
	}
	else
		m_strFileTitle = m_strFileName;
	Begin();
}

LONGLONG CFtpDownloadThread::GetFtpFileSize(CFtpConnection* pFtpCon, CString strFtpFile)
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

bool CFtpDownloadThread::IsFailed(void)
{
	bool ret;
	m_cs.Lock();
	ret = m_bFailed;
	m_cs.Unlock();
	return ret;
}

bool CFtpDownloadThread::IsTransOver(void)
{
	bool ret;
	m_cs.Lock();
	ret = m_bTransOver;
	m_cs.Unlock();
	return ret;;
}

void CFtpDownloadThread::SetTransOverFlag(bool value) //���ô�����ɱ���λ
{
	m_cs.Lock();
	m_bTransOver = value;
	m_cs.Unlock();
}

//ͨ����Ƶ���õ���Ƶ��
CString CFtpDownloadThread::GetAudioFileName(CFtpConnection* pFtpCon,CString strVideo)
{
	//ȥ����׺��
	int index;
	CString strVideoExt;
	index = strVideo.ReverseFind(_T('.'));
	if(index !=-1)
	{
		strVideoExt = strVideo.Right(strVideo.GetLength() - index - 1);
		strVideo = strVideo.Left(index);
	}
	if(strVideoExt == _T(""))
		return _T("");
	CFtpFileFind   ftpFind(pFtpCon); 
	CString retName = _T("");
	BOOL bWorking = ftpFind.FindFile(strVideo+_T(".*"));
	while(bWorking)
	{
		bWorking = ftpFind.FindNextFile();
		CString filename = ftpFind.GetFilePath();
		int index = filename.ReverseFind(_T('.'));
		if(index != -1)
		{
			CString ext = filename.Right(filename.GetLength() - index -1);
			if(ext.CompareNoCase(strVideoExt) != 0) //�����
			{
				retName = filename;
				break;
			}
		}
	}

	ftpFind.Close(); 
	return retName;
}

//��ȡȥ��guid��·��
CString CFtpDownloadThread::GetPureName(CString fullname)
{
	int index = fullname.ReverseFind(_T('.'));
	CString extname;
	if(index != -1)
		extname = fullname.Right(fullname.GetLength() - index);
	CString ret;
	ret = fullname.Left(10).Trim();
	ret = ret + extname;
	return ret;
}


int CFtpDownloadThread::ParsePasv( char *buf )
{
	char *p = strchr( buf, '(' ); 
	p++; //����������
	char *q = strchr( buf, ')' );
	if( !p || !q ) return -1;
	*( p + ( q - p ) ) = '\0';
	int port = -1;
	p = strchr( p, ',' );
	p++;
	p = strchr( p, ',' );
	p++;
	p = strchr(p,',');
	p++;
	p = strchr( p, ',' );
	p++;
	port = atol( p ) * 256 ;
	p = strchr( p, ',' );
	p++;
	port += atol( p );
	return port;
}
