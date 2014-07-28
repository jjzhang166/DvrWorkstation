#include "stdafx.h"
#include "FtpHelper.h"
#include "../WellCommon/CharSetHelper.h"

MapTimer CFtpHelper::m_mapTimerObjs;

CFtpHelper::CFtpHelper()
	: m_pInetSession(NULL)
	, m_pFtpConnection(NULL)
	, m_nPort(21)
	, m_hEvent(NULL)
	, m_hThread(NULL)
	, m_bExit(false)
	, m_nPutStatus(EM_PUT_UNKNOW)
	, m_bForceStop(false)
{
	Init();
}


CFtpHelper::~CFtpHelper()
{
	Unit();
}

bool CFtpHelper::Init()
{
	m_regCB.tfCB = NULL;
	m_regCB.ttNotify = NULL;

	m_nBufSize = 2000000;
	m_pBuffer = new char[m_nBufSize];
	ReSetTrans();

	//������ʱ��
	//�ڶ�������Ϊ0�����Զ�����һ��id��
	m_putTimerId = ::SetTimer(NULL, 0, 1000, (TIMERPROC)CFtpHelper::PutTimerProc);

	//�洢
	m_mapTimerObjs[m_putTimerId] = this;

	return true;
}

void CFtpHelper::Unit()
{
	ReSetTrans();
	::KillTimer(NULL, m_putTimerId);
	//ɾ����
	m_mapTimerObjs.erase(m_mapTimerObjs.find(m_putTimerId));

	if( m_hThread )
	{
		WaitForSingleObject(m_hThread, 2000);
		m_hThread = NULL;
	}

	if( m_hEvent )
	{
		CloseHandle(m_hEvent);
		m_hEvent = NULL;
	}

	if(m_pFtpConnection)
	{
		m_pFtpConnection->Close();
		delete m_pFtpConnection;
		m_pFtpConnection = NULL;
	}

	if(m_pInetSession)
	{
		m_pInetSession->Close();
		delete m_pInetSession;
		m_pInetSession = NULL;
	}

	if(m_pBuffer)
	{
		delete[] m_pBuffer;
		m_pBuffer = NULL;
	}
}

bool CFtpHelper::_Start( const wstring& addr, const wstring& strUser, const wstring& strPwd, UINT nPort)
{
	m_strServAddr = addr;
	m_strUser = strUser;
	m_strPwd = strPwd;
	m_nPort = nPort;

	if(Start())
	{
		//m_hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		//m_hThread = AfxBeginThread(PutFileProc, this);
		return true;
	}

	return false;
}

bool CFtpHelper::Start()
{
	if(m_pInetSession)
	{
		m_pInetSession->Close();
		delete m_pInetSession;
	}

	m_pInetSession = new CInternetSession(_T("welldvr"), 1, PRE_CONFIG_INTERNET_ACCESS);
	try
	{
		if(m_pFtpConnection)
		{
			m_pFtpConnection->Close();
			delete m_pFtpConnection;
			m_pFtpConnection = NULL;
		}
		//���һ������ҪΪTRUE, ��Ȼ��ִ�е�FindFile��ʱ��,����Ϳ�ס�ˣ�CFtpHelperFindfile)
		m_pFtpConnection = m_pInetSession-> GetFtpConnection(m_strServAddr.c_str(), m_strUser.c_str(), m_strPwd.c_str(), m_nPort,TRUE);
	}
	catch(CInternetException *pEx)
	{
		//����½���ɹ����׳��쳣������������쳣�Ĵ���
		TCHAR szError[1024] = {0};
		if(pEx->GetErrorMessage(szError,1024))
		{
			CString strError;
			strError.Format(_T("����FTP����������%s\r\n���������ļ��е�ftp��������Ϣ!"), szError);
			AfxMessageBox(strError);
		}
		else
			AfxMessageBox(_T("There was an exception"));
		pEx->Delete();
		m_pFtpConnection = NULL;
		return false;
	}
	return true;
}

void CFtpHelper::Stop()
{
	if(m_pFtpConnection)
	{
		m_pFtpConnection->Close();
		delete m_pFtpConnection;
		m_pFtpConnection = NULL;
	}

	if(m_pInetSession)
	{
		m_pInetSession->Close();
		delete m_pInetSession;
		m_pInetSession = NULL;
	}
}

UINT PutFileProc(LPVOID pFtp)
{
	CFtpHelper* p = static_cast<CFtpHelper*>(pFtp);
	if(p)
	{
		return p->PutFile();
	}
	return 0;
}

UINT CFtpHelper::PutFile()
{
	while( ::WaitForSingleObject(m_hEvent, INFINITE) == WAIT_OBJECT_0 )
	{
		if(m_bExit )
			break;

		if(m_strPutFile.empty() || m_strSaveFile.empty())
			continue;
		
		//��һ������Ϊ�����ļ��ĵ�ַ���ڶ���Ϊ���������ļ��ĵ�ַ
		if(m_pFtpConnection->PutFile(m_strPutFile.c_str(), m_strSaveFile.c_str()))
		{
			m_strPutFile.clear();
			m_strSaveFile.clear();
			m_nPutStatus = EM_PUT_OK;
		}
		else
		{			
			m_nPutStatus = EM_PUT_FAIL;
			DWORD dw = GetLastError();
			CString strErr;
			strErr.Format(_T("FTP�ϴ�ʧ�ܣ�[%d]\r\n�ļ���%s"), dw, m_strSaveFile);
			//SHAREDATA.g_pMainFrame->Alert(strErr);
		}

		ResetEvent(m_hEvent);
	}

	return 1;
}

UINT CFtpHelper::_PutFile(const wstring& strFileName, const wstring& saveFile)
{
	if(strFileName.empty() || saveFile.empty())
	{
		return 0;
	}

	m_nPutStatus = EM_PUT_UNKNOW;

	m_strSaveFile = saveFile;
	m_strPutFile = strFileName;

	SetEvent(m_hEvent);	
	while(m_nPutStatus == EM_PUT_UNKNOW)
	{
		Sleep(10);
	}

	return m_nPutStatus == EM_PUT_OK ? 1 : 0;
}

bool CFtpHelper::_CreateDirctory(const wstring& strPath)
{
	if(strPath.empty())
		return false;

	bool bError = false;

	if( !m_pFtpConnection->CreateDirectory(strPath.c_str()) )
	{
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
	}

	return !bError;
}

void CFtpHelper::ServiceRun()
{
	//FTP��̨�߳�
	AfxBeginThread((AFX_THREADPROC)FtpThrProc, this);
}

DWORD WINAPI FtpThrProc(LPVOID param)
{
	CFtpHelper* p = (CFtpHelper*)param;
	if(p)
	{
		p->Service();
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
//���ܣ��ݹ�ɾ�����ļ���
//������
//		strFolde [IN]	--	�����ļ���·������E:\\test-ftp
//���أ��ļ��е��ļ�����
//
//////////////////////////////////////////////////////////////////////////
int CFtpHelper::DelEmptyFolder(const wstring& strFolder)
{
	CFileFind tempFind; 
	BOOL bFound; //�ж��Ƿ�ɹ��ҵ��ļ�
	bFound = tempFind.FindFile(wstring(strFolder + _T("\\*.*")).c_str());

	int fileCount = 0;

	CString strTmp;   //����ҵ������ļ��� ����ļ���·��
	while(bFound)     //���������ļ�
	{ 
		bFound = tempFind.FindNextFile(); //��һ��ִ��FindNextFile��ѡ�񵽵�һ���ļ����Ժ�ִ��Ϊѡ��

		if(tempFind.IsDots()) 
			continue;
		if(tempFind.IsDirectory())   //�ҵ������ļ��У���������ļ����µ��ļ�
		{ 
			strTmp = tempFind.GetFilePath();
			CString strF = tempFind.GetFileName();
				
			int fileCount = DelEmptyFolder(strTmp.GetString()); 

			if(fileCount == 0)
			{	
				//ɾ�����ļ���
				RemoveDirectory(strTmp); 
			} 
		}
		else 
		{ 
			fileCount++;
		}
	} 
	tempFind.Close();
	return fileCount;
}


void CFtpHelper::Service()
{
	//��ͣ�Ĵӱ���Ŀ¼���ϴ���ftp��������
	if(m_strLocalDir.empty())
		return;

	int nRlt = TRANS_CONTUNE;
	while(true)
	{
		nRlt = TRANS_CONTUNE;
		//��ʼ֮ǰ֪ͨ�ͻ�
		if(m_regCB.ttNotify)
			nRlt = m_regCB.ttNotify(TRANS_BEGIN);

		if(nRlt == TRANS_CONTUNE)
		{
			_TransFolder(m_strLocalDir, _T(""));
		}

		//ɾ���ļ�������ҪһС��ʱ�䣬˯һ���ٵ�ɾ�����ļ���		
		Sleep(5000);
		

		nRlt = TRANS_SKIP;
		//���������֪ͨ�ͻ�
		if(m_regCB.ttNotify)
			nRlt = m_regCB.ttNotify(TRANS_END);
		if(nRlt == TRANS_DELETE)
		{
			//ɾ�����ļ���
			DelEmptyFolder(m_strLocalDir);
		}

		Sleep(2000);
	}
}

//�ж�ftp�������Ƿ���ڴ��ļ�
//������ڷ����ļ��Ĵ�С
LONGLONG CFtpHelper::IsFileExit(const wstring& strFile)
{
	CFtpFileFind ftpff(m_pFtpConnection);
	LONGLONG filelen = 0;
	if(ftpff.FindFile(strFile.c_str())) 
	{ 
		ftpff.FindNextFile(); 
		filelen = ftpff.GetLength(); 
	} 
	ftpff.Close();

	return filelen;
}

bool CFtpHelper::_TransFolder(const wstring& strFoldername, const wstring& strDst)
{
	CInternetFile *pInetFile = NULL;
	try
	{
		CFileFind tempFind; 
		BOOL bFound; //�ж��Ƿ�ɹ��ҵ��ļ�
		bFound = tempFind.FindFile(wstring(strFoldername + _T("\\*.*")).c_str());

		if(!strDst.empty()  && !_CreateDirctory(strDst))
		{
			return false;
		}

		CString strTmp;
		while(bFound)
		{ 
			//��һ��ִ��FindNextFile��ѡ�񵽵�һ���ļ����Ժ�ִ��Ϊѡ��
			bFound = tempFind.FindNextFile(); 

			if(tempFind.IsDots()) 
				continue;
			if(tempFind.IsDirectory())
			{ 
				//�ҵ������ļ��У���������ļ����µ��ļ�
				strTmp = tempFind.GetFilePath();
				CString strF = tempFind.GetFileName();

				_TransFolder(strTmp.GetString(), strDst + _T("\\") + strF.GetString());
			} 
			else 
			{ 
				m_bForceStop = false;

				CFile localFile;
				strTmp = tempFind.GetFilePath();//�����ļ�����������׺��

				int nRlt = TRANS_CONTUNE;
				//add by yjt 2014-06-13	�ص��ϴ��ļ����ɿͻ������Ƿ��ϴ�
				if(m_regCB.tfCB != NULL && TRANS_SKIP == (nRlt = m_regCB.tfCB(strTmp.GetString())) )
				{
					continue;
				}

				if(!localFile.Open(strTmp, CFile::modeRead|CFile::shareDenyNone))
				{
					m_llFileSize = 0;
					m_llTransSize = 0;
					Sleep(10);
					continue;
				}

				//Զ��Ŀ¼Ҫ�Ӹ�Ŀ¼��ʼ
				CString strReoveFie;
				strReoveFie.Format(_T("%s\\%s"), strDst.c_str() ,tempFind.GetFileName());
				
				m_llFileSize = localFile.GetLength();
				m_llTransSize = IsFileExit(strReoveFie.GetString());
				//����ļ��Ѵ��ڣ��Ҵ�Сһ��, ˵���Ѵ���
				if(m_llTransSize == m_llFileSize)
				{
					localFile.Close();
					continue;
				}

				if(m_llTransSize > 0)
				{
					//�ϵ�����
					try
					{
						localFile.Seek(m_llTransSize, CFile::begin);
						pInetFile = m_pFtpConnection->Command(_T("APPE ") + strReoveFie, CFtpConnection::CmdRespWrite);
						TRACE("begin %lld/%lld!!!\n", m_llTransSize, m_llFileSize);
						//����ʱ,�����С��0�������ٶȲ���
						m_llTransSize = 0;
					}
					catch (CInternetException* e)
					{
						pInetFile = m_pFtpConnection->OpenFile(strReoveFie, GENERIC_WRITE);	
					}
				}
				else
				{
					pInetFile = m_pFtpConnection->OpenFile(strReoveFie, GENERIC_WRITE);
				}

				m_strPutFile = tempFind.GetFileName(); 
				int len = 0;
				while( len = localFile.Read(m_pBuffer,m_nBufSize))
				{
					pInetFile->Write(m_pBuffer,len);
					m_cs.Lock();
					m_llTransSize += len;
					m_cs.Unlock();

					TRACE("trans %lld/%lld!!!!\n" , m_llTransSize, m_llFileSize);
					//�Ƿ�ֹͣ����
					if(m_bForceStop)
					{
						localFile.Close();
						if(pInetFile)
						{
							pInetFile->Close();
							delete pInetFile;
						}
						break;
					}
				}
				TRACE("end %lld/%lld!!!!\n" , m_llTransSize, m_llFileSize);
				localFile.Close();
				pInetFile->Close();
				ReSetTrans();

				if(pInetFile)
				{
					pInetFile->Close();
					delete pInetFile;
					pInetFile = NULL;
				}

				if(!m_bForceStop)
				{	
					if(nRlt == TRANS_DELETE)
					{
						//ɾ���ļ�
						DeleteFile(strTmp);
					}
				}
			} 
		} 
		tempFind.Close();
		ReSetTrans();
		return true;
	}
	catch(CInternetException *e)
	{
		if(pInetFile)
		{
			pInetFile->Close();
		}
		e->Delete();
	}
	catch(CFileException *e)
	{
		e->Delete();
		DWORD error = GetLastError();
	}
	if(pInetFile)
	{
		delete pInetFile;
		pInetFile = NULL;
	}

	//����
	ReStart();

	return false;
}

bool CFtpHelper::ReStart()
{
	Stop();
	Sleep(1000);

	ReSetTrans();

	return Start();
}

void CFtpHelper::ReSetTrans()
{	
	m_cs.Lock();
	m_llFileSize = 0;
	m_llTransSize = 0;
	m_nTotalTanstime = 0;
	m_strPutFile.clear();
	m_strSpeed.clear();
	m_cs.Unlock();
}

//��ȡ���Ͱٷֱ�
float CFtpHelper::GetTransPrecent()
{ 
	float fPrecent = 0;
	if(m_llFileSize > 0)
	{
		m_cs.Lock();
		fPrecent = (float)m_llTransSize / m_llFileSize; 
		if(fPrecent > 1)
			fPrecent = 1;
		if(fPrecent < 0)
			fPrecent = 0;
		m_cs.Unlock();

	}
	return fPrecent;
}

//��ȡ�����ٶ�
wstring CFtpHelper::GetTransSpeed()
{ 
	return m_strSpeed;
}

void CFtpHelper::GetTransSpeed(wstring& strspd)
{ 
	strspd = m_strSpeed;
}

void CFtpHelper::GetPutFile(wstring& strpf)
{
	strpf = m_strPutFile;
}

wstring CFtpHelper::GetPutFile()
{
	return m_strPutFile;
}

//��ʱ���㴫���ٶ�
void CALLBACK CFtpHelper::PutTimerProc(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{
	if( m_mapTimerObjs.find(idEvent) == m_mapTimerObjs.end())
		return;

	CFtpHelper* pFtp = m_mapTimerObjs[idEvent];
	if(pFtp == NULL)
	{
		return;
	}

	if(pFtp->m_llTransSize > 100)
	{
		pFtp->m_nTotalTanstime += 1;
		float fSpeed = pFtp->m_llTransSize / pFtp->m_nTotalTanstime;   // B/s
		fSpeed = fSpeed / 1024/1024;
		CString strSd;
		strSd.Format(_T("�����ٶ�:%.2fMB/s"), fSpeed);
		pFtp->m_strSpeed = strSd.GetString();
	}
}

//����ftp�ͻ���
bool CFtpHelper::Start(const string& addr, const string& strUser, const string& strPwd, UINT nPort, const string& strDir)
{
	return Start(S2WS(addr), S2WS(strUser), S2WS(strPwd), nPort, S2WS(strDir));
}

bool CFtpHelper::Start(const wstring& addr, const wstring& strUser, const wstring& strPwd, UINT nPort, const wstring& strDir)
{
	m_strLocalDir = strDir;
	return _Start(addr.c_str(), strUser.c_str(), strPwd.c_str(), nPort);
}


//����Ŀ¼
bool CFtpHelper::CreateDirctory(const wstring& strPath)
{
	return _CreateDirctory(strPath);
}

bool CFtpHelper::CreateDirctory(const string& strPath)
{
	return CreateDirctory(S2WS(strPath));
}

//�ϴ��ļ�
UINT CFtpHelper::PutFile(const wstring& strFile, const wstring& saveFile)
{
	return _PutFile(strFile, saveFile);
}

UINT CFtpHelper::PutFile(const string& strFile, const string& saveFile)
{
	return PutFile(S2WS(strFile), S2WS(saveFile));
}

//�ϴ��ļ���
bool CFtpHelper::TransFolder(const wstring& strFoldername, const wstring& strDst)
{
	return  _TransFolder(strFoldername, strDst);
}

bool CFtpHelper::TransFolder(const string& strFoldername, const string& strDst)
{
	return TransFolder(S2WS(strFoldername), S2WS(strDst));
}

void CFtpHelper::Register(const RegCB& cb)
{
	m_regCB = cb;
}

bool CFtpHelper::operator==(const CFtpHelper& ftp)
{
	return (this == &ftp);
}