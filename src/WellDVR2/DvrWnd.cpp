#include "stdafx.h"
#include "DvrWnd.h"
#include "ShareData.h"
#include "ftp/FtpHelper.h"

#include "resource.h"

#include "dvr/interface.h"
#include "ConvertVideo/Avi2Flv.h"
#include "WellCommon/DirHelper.h"

MapDvrTimer CDvrWnd::m_mapTimer;
CDvrWnd::CDvrWnd(void)
{
	Init(false);
}

CDvrWnd::CDvrWnd(bool empty)
{
	Init(empty);
}

void CDvrWnd::Init(bool empty)
{
	m_status = DVR_DISABLE;
	m_x = -1;
	m_y = -1;

	m_hCopyEvent = NULL;
	m_hCopyThread = NULL;
	m_bExit = FALSE;

	m_CopyStatus = COPY_OK;
	m_bStopCopy = FALSE;

	m_filecnt = 0;
	m_nReadFileCount = 0;

	m_proRange = 100;
	m_proValue = 0;

	if(empty)
	{
		m_hCopyThread = NULL;
		m_hCopyEvent = NULL;
	}
	else
	{
		m_hCopyEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		m_hCopyThread = AfxBeginThread(CopyProc, this);

		InitSpeed();
	}

	m_pDev = NULL;
	m_bIsCopying = FALSE;
}

void CDvrWnd::UnInit()
{
	UnInitSpeed();

	m_status = DVR_DISABLE;
	m_x = -1;
	m_y = -1;
	if( m_hCopyThread )
	{
		WaitForSingleObject(m_hCopyThread, 2000);
		m_hCopyThread = NULL;
	}

	if( m_hCopyEvent )
	{
		CloseHandle(m_hCopyEvent);
		m_hCopyEvent = NULL;
	}


	m_hCopyEvent = NULL;
	m_hCopyThread = NULL;
	m_bExit = FALSE;

	m_CopyStatus = COPY_OK;
	m_bStopCopy = FALSE;

	m_filecnt = 0;
	m_nReadFileCount = 0;

	if(m_pDev)
	{
		delete m_pDev;
		m_pDev = NULL;
	}
	m_bIsCopying = FALSE;
}

CDvrWnd::~CDvrWnd(void)
{
	UnInit();
}

void CDvrWnd::InitSpeed()
{
	m_nTotalTanstime = 0;

	m_llTransSize = 0;

	//�ڶ�������Ϊ0�����Զ�����һ��id��
	m_cpTimerId = ::SetTimer(NULL, gc_TimerId_CopySpeed, gc_TimerSpace_CopySpeed, (TIMERPROC)CDvrWnd::CopyTimerProc);
	TRACE("input id=%d, outputid=%d\r\n", gc_TimerId_CopySpeed, m_cpTimerId);

	//��ӵ�������ȥ
	m_mapTimer[m_cpTimerId] = this;
}

void CDvrWnd::UnInitSpeed()
{
	m_nTotalTanstime = 0;

	m_llTransSize = 0;
	::KillTimer(NULL, m_cpTimerId);
	TRACE("kill timer = %d\n", m_cpTimerId);

	//�Ӷ�����ɾ��
	MapDvrTimerItor itF = m_mapTimer.find(m_cpTimerId);
	if(itF != m_mapTimer.end())
	{
		m_mapTimer.erase(itF);
	}
}

//modified by yjt 2014-8-22 �޸����°����һ�wDCIM�ļ�
void CDvrWnd::CopyDir(const CString& strDrive)
{
	if( strDrive.IsEmpty() )
	{
		m_strStatus = _T("Ŀ¼Ϊ��, ���԰�ȫ�ε�DVR�豸");
		return ;
	}
	//��ȫֹͣ���ڿ�ؐ
	if(m_bStopCopy == TRUE)
	{
		return;
	}

	m_filecnt = 0;
	RecursiveFindFile(strDrive);

	//�豸������
	if( m_filecnt > 0 )
	{
		SetStatus(DVR_NORMAL);
		m_bIsCopying = TRUE;

		m_proRange = m_filecnt;
		m_proValue = 0;
		m_nReadFileCount = 0;

		if(CDirHelper::DirExits(strDrive + _T("\\DCIM")))
		{
			m_strSrcPath = strDrive + _T("\\DCIM");
		}
		else
		{
			m_strSrcPath = strDrive;
		}

	
		if( SHAREDATA.g_strFileTarget == _T("localhost") )
			m_strDstPath.Format(_T("%s\\%s"), SHAREDATA.g_strFilePath, m_strId);	

		SetEvent(m_hCopyEvent);	
		LOG(_T("DVR�豸[%s], ö�ٳɹ����̷�[%s], �������ݿ�ʼ"), m_strId, m_strDisk);
	}	
	else
	{
		m_bIsCopying = FALSE;
		if(m_proValue == m_proRange)
		{
			m_nReadFileCount = 0;
			m_strStatus = _T("�������, ���԰�ȫ�ε�DVR�豸");
			LOG(_T("%s[%s]"), m_strStatus, m_strId);
		}
		else
		{
			if(m_nReadFileCount++ < 10)
			{
				m_strStatus = _T("���ڼ��[") + strDrive + _T("]�� ���Ժ�...");	
			}
			else
			{
				m_nReadFileCount = 0;
				m_strStatus = _T("û���ļ�, ���԰�ȫ�ε�DVR�豸");	
			}
		}
		m_status = DVR_COPY_OK;
	}

}

UINT CopyProc(LPVOID param)
{
	CDvrWnd* pDvr = (CDvrWnd*)param;
	if(pDvr)
	{
		return pDvr->CopyDirProc();
	}
	return 0;
}

UINT CDvrWnd::CopyDirProc()
{
	while( ::WaitForSingleObject(m_hCopyEvent, INFINITE) == WAIT_OBJECT_0 )
	{
		if(m_bExit )
			break;

		if(m_strSrcPath.IsEmpty() || m_strDstPath.IsEmpty() )
		{
			ResetEvent(m_hCopyEvent);
			continue;
		}

		m_bIsCopying = TRUE;

		CopyDir(m_strSrcPath.GetString(), m_strDstPath.GetString(), &m_bStopCopy);

		if( m_CopyStatus == COPY_STOP )
		{
			m_status = DVR_COPY_OK;
			m_strStatus = _T("ֹͣ����, ���԰�ȫ�ε�DVR�豸");
		}
		else if( m_CopyStatus == COPY_UNKNOWN )
		{
			m_status = DVR_STOP;
			m_strStatus = _T("ֹͣ����, �ļ������쳣");
		}
		else if( m_CopyStatus == COPY_SPACE_LIMIT)
		{
			m_status = DVR_STOP;
			m_strStatus = _T("�ռ䲻��, ֹͣ���䣬���԰�ȫ�γ�!");
		}
		else
		{
			m_status = DVR_COPY_OK;
			m_strStatus = _T("�������, ���԰�ȫ�ε�DVR�豸");
		}

		ResetTransSize();
		m_CopyStatus = COPY_OK;

		ResetEvent(m_hCopyEvent);

		m_bIsCopying = FALSE;
		m_nReadFileCount = 0;
	}

	return 0;
}

#define  BUF_LEN	(1024*1024)
char tmp_buf[BUF_LEN] = {0};
#define AUTO_DELETE		1

//MFC�п����ļ���
void CDvrWnd::CopyDir(const TCHAR* src, const TCHAR* dst, LPBOOL bCancel)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	TCHAR tmpsrc[256];

	_tcscpy(tmpsrc, src);
	_tcscat(tmpsrc, _T("\\*.*"));

	hFind = FindFirstFile(tmpsrc, &FindFileData);
	if(hFind == INVALID_HANDLE_VALUE)
		return;

	if(!CreateDeepDirectory(dst, NULL))
	{
		CString str;
		str.Format(_T("����Ŀ¼[%s]ʧ��"), dst);
		SHAREDATA.g_pMainFrame->Alert(str);
		return;
	}

	do
	{
		TCHAR newdst[256] = {0};
		_tcscpy(newdst, dst);
		if( newdst[_tcslen(newdst)] != '\\')
			_tcscat(newdst, _T("\\"));

		_tcscat(newdst, FindFileData.cFileName);

		TCHAR newsrc[256] = {0};
		_tcscat(newsrc, src);
		if( newsrc[_tcslen(newsrc)] != '\\' )
			_tcscat(newsrc, _T("\\"));
		_tcscat(newsrc, FindFileData.cFileName);
		if( FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
		{
			if( _tcscmp(FindFileData.cFileName, _T(".")) != 0 && \
				_tcscmp(FindFileData.cFileName, _T("..")) != 0 )
			{
				CopyDir(newsrc, newdst, bCancel);

				if(m_CopyStatus != COPY_OK )
					break;
#if AUTO_DELETE
				//ɾ���ļ���
				RemoveDirectory(newsrc);  
#endif
			}
		}
		else
		{
			if( m_CopyStatus == COPY_STOP || *bCancel )
			{
				m_CopyStatus = COPY_STOP;
				break;
			}

			m_strStatus = _T("���ڴ���");
			m_strFile.Format(_T("%s"), FindFileData.cFileName);

			//�����̿ռ�
			if(SHAREDATA.g_dwFreeSpace > 0 && SHAREDATA.g_dwFreeSpace < gc_MinSpaceLimit)
			{
				//winserver 2003Ȩ�����ⲻ�ܷ�����Ҫװ����
				//::Beep(1000, 1000);
				SpaceLessAlarm();
				m_CopyStatus = COPY_SPACE_LIMIT;
				break;
			}
			
			ResetTransSize();

			BOOL bSucceed = FALSE;

			//modified by yjt 2014-08-22 ����־�ļ����⴦��׷��д��
			if(!IsLogFile(newsrc))
			{
				//���ú���API����CopyFileEx�������ļ�
				bSucceed = CopyFileEx(newsrc,newdst,
					CopyProgressRoutine,
					this,
					bCancel,				//�����Ƿ�ȡ������s
					COPY_FILE_ALLOW_DECRYPTED_DESTINATION);
			}
			else
			{
				bSucceed = CopyLogFile(newsrc, newdst);
			}


			if(!bSucceed)
			{
				DWORD dw = GetLastError();
				if(dw == ERROR_FILE_EXISTS)
				{
					//The file exists.   
				}
				else if(dw == ERROR_REQUEST_ABORTED)
				{
					//The request was aborted
				}

				if(m_bStopCopy == TRUE)
				{
 					m_CopyStatus = COPY_STOP;
				}
 				else
				{
					//������
					if(m_proValue < m_proRange)
					{
						++m_proValue;
					}
					//�쳣�ļ�����
					continue;
				}
				break;
 			}
			else
			{
				//����ת��
				AVI2FLV.PutVideo(newdst);

#if AUTO_DELETE
				//ɾ���ļ�
				DeleteFile(newsrc);
#endif	
			}


			if( m_CopyStatus == COPY_OK )
			{
				//������
				if(m_proValue < m_proRange)
				{
					++m_proValue;
				}
			}
			else
			{
				TRACE("pWnd->m_CopyStatus: %d\n", m_CopyStatus);
				break;
			}

		}

	}while(FindNextFile(hFind,&FindFileData));

	FindClose(hFind);
}

void CDvrWnd::RecursiveFindFile(const CString& strPath)
{
	CFileFind ff;
	BOOL bFind = ff.FindFile(strPath + _T("\\*.*"));

	while( bFind )
	{
		bFind = ff.FindNextFile();
		if (ff.IsDots() || ff.IsSystem() || ff.IsHidden())
			continue;

		if (ff.IsDirectory())
		{
			RecursiveFindFile(ff.GetFilePath());
		}
		else
		{
			CString strFileName = ff.GetFilePath();
			m_filecnt += 1;
		}
	}
}

BOOL CDvrWnd::CreateDeepDirectory(const CString& strPath,        //directory name
	LPSECURITY_ATTRIBUTES lpSecurityAttributes  // SD
	)
{
	CString strSubPath;
	CString strInfo;
	int nCount = 0; 
	int nIndex = 0;

	//�����ַ�"\\"�ĸ���
	do
	{
		nIndex = strPath.Find(_T("\\"), nIndex) + 1;
		nCount++;
	}while( (nIndex-1) != -1);
	nIndex = 0;
	//��飬������Ŀ¼
	while( (nCount-1) >= 0)
	{
		nIndex = strPath.Find(_T("\\"), nIndex) + 1;
		if( (nIndex - 1) == -1)
		{
			strSubPath = strPath;
		}
		else
			strSubPath = strPath.Left(nIndex);
		if(!PathFileExists(strSubPath))
		{
			if(!::CreateDirectory(strSubPath,NULL))
			{
				strInfo = _T("Build Directory");
				strInfo += strSubPath;
				strInfo += _T(" Fail!");
				SHAREDATA.g_pMainFrame->Alert(strInfo);
				return FALSE;
			}
		}
		nCount--;
	};
	return TRUE;
}

void CDvrWnd::ClearInfo()
{
	m_strId.Empty();
	m_strName.Empty();
	m_strDisk.Empty();
	m_strStatus.Empty();
	m_proRange = 100;
	m_proValue = 0;
	m_strFile.Empty();

	m_bIsCopying = FALSE;
	m_bStopCopy = FALSE;
	if(m_pDev)
	{
		delete m_pDev;
		m_pDev = NULL;
	}

	ClearSpeed();

	m_nReadFileCount = 0;
}

void CDvrWnd::ClearSpeed()
{
	ResetTransSize();
	m_strSpeed.Empty();
}

DWORD CALLBACK CDvrWnd::CopyProgressRoutine(
	LARGE_INTEGER TotalFileSize,
	LARGE_INTEGER TotalBytesTransferred,
	LARGE_INTEGER StreamSize,
	LARGE_INTEGER StreamBytesTransferred,
	DWORD dwStreamNumber,
	DWORD dwCallbackReason,
	HANDLE hSourceFile,
	HANDLE hDestinationFile,
	LPVOID lpData
	)
{
	CDvrWnd* pDvr = (CDvrWnd*)lpData;
	if(pDvr)
	{
		pDvr->m_llTransSize = TotalBytesTransferred.QuadPart;
		//�����̿ռ�
		if(SHAREDATA.g_dwFreeSpace > 0  && SHAREDATA.g_dwFreeSpace + pDvr->m_llTransSize/1024/1024 < gc_MinSpaceLimit)
		{			
			pDvr->m_bStopCopy = TRUE;
			pDvr->m_bIsCopying = FALSE;
			pDvr->SpaceLessAlarm();
			return PROGRESS_CANCEL;
		}
	}

	//����0���������ƣ���ͨ��bCancel���Ƹ��ƽ������˷���ֵ��ʱδ��
	return PROGRESS_CONTINUE;
}

//�ռ䲻�㱨��
void CDvrWnd::SpaceLessAlarm()
{
	PlaySound((LPCTSTR)IDR_WAVE2, NULL, SND_RESOURCE | SND_ASYNC);
	CString strMsg;
	strMsg.Format(_T("���̿ռ䲻��%dM��Ϊ��֤�ļ������������������κβ�����"), gc_MinSpaceLimit);
	SHAREDATA.g_pMainFrame->Alert(strMsg);
}

void CDvrWnd::SetCopyStatus(int nStatus){ 
	m_CopyStatus = nStatus; 
	if(m_CopyStatus == COPY_STOP)
	{
		m_bStopCopy = TRUE;
		m_bIsCopying = FALSE;
	}
	else
	{
		m_bStopCopy = FALSE;
	}
}

void CALLBACK CDvrWnd::CopyTimerProc(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{   
	CDvrWnd * pDvr = m_mapTimer[idEvent];
	if(pDvr)
	{
		//ͨ��timeid����ȡCDvr����
		if(pDvr->m_llTransSize > 100)
		{
			pDvr->m_nTotalTanstime += 1;
			float fSpeed = pDvr->m_llTransSize / pDvr->m_nTotalTanstime;   // B/s
			fSpeed = fSpeed / 1024/1024;
			pDvr->m_strSpeed.Format(_T("�����ٶ�:%.2fMB/s"), fSpeed);
//			TRACE(pDvr->m_strSpeed);
		}
	}
} 

void CDvrWnd::MassDev()
{
	m_cs.Lock();
	
	if(m_pDev && !m_pDev->IsMassed())
	{
		m_pDev->SetMassEx();
	}

	m_cs.Unlock();
}

void CDvrWnd::SetDevPtr(CDevBase* pDev) 
{ 
	m_cs.Lock();
	m_pDev = pDev; 
	m_cs.Unlock();
}

void CDvrWnd::RemoveDevPtr()
{
	m_cs.Lock();
	if(m_pDev)
	{
		delete m_pDev;
		m_pDev = NULL;
	}
	m_cs.Unlock();
}

CDvrWnd& CDvrWnd::operator=(const CDvrWnd& dvr)
{
	if(&dvr == this)
		return *this;

	RemoveDevPtr();

	*this = dvr;

	return *this;
}

bool CDvrWnd::IsLogFile(const CString& strFileName)
{
	CString str = strFileName.Right(3);
	str.MakeUpper();
	return str == _T("TXT");
}

//������־�ļ���׷�ӿ���)
bool CDvrWnd::CopyLogFile(const TCHAR* strFileSrc, const TCHAR* strDst)
{
	CFile fileSrc;
	CFile fileDst;
	try
	{
		if(!fileSrc.Open(strFileSrc, CFile::modeRead))
		{
			return false;
		}

		if(!fileDst.Open(strDst, CFile::modeWrite | CFile::modeCreate | CFile::modeNoTruncate))
		{
			fileSrc.Close();
			return false;
		}

		fileDst.SeekToEnd();

		char buff[1024] = {0};
		DWORD dwRead = 0;
		do
		{
			dwRead = fileSrc.Read(buff, 1024);
			fileDst.Write(buff, dwRead);
		}while(dwRead > 0);

		fileSrc.Close();
		fileDst.Close();
	}
	catch(...)
	{
		return false;
	}

	return true;
}