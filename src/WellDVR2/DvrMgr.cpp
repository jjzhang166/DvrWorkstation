#include "stdafx.h"
#include "DvrMgr.h"
#include "dvr/interface.h"
#include <comutil.h>
#include "DataBase/DataBase.h"
#include "IniFile.h"
#include "Config.h"
#include "dvr/DvrFactory.h"
#include "USBCtrl/UsbHelper.h"
#include <sstream>
using namespace std;
#include "LocationConfig.h"
#include "dvr/interface.h"
#include "ShareData.h"

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
CDvrMgrBase::CDvrMgrBase()
{
	Init();
}

CDvrMgrBase::~CDvrMgrBase()
{
	UnInit();
}

void CDvrMgrBase::Init()
{
	//m_vecDvrWnd.resize(SHAREDATA.g_dvrProp.numDvr);
	CDvrWnd * pDvrWnd;
	for ( int i = 0; i < SHAREDATA.g_dvrProp.numRow; ++i)
	{
		for(int j = 0; j < SHAREDATA.g_dvrProp.numCol; ++j)
		{
			pDvrWnd = new CDvrWnd();
			pDvrWnd->SetPos(i, j);
			m_vecDvrWnd.push_back(pDvrWnd);
		}
	}

	INIT_LOCK;
}

void CDvrMgrBase::UnInit()
{
	CDvrWnd * pDvrWnd = NULL;
	VecDvrWndItor it = m_vecDvrWnd.begin();
	for ( ; it != m_vecDvrWnd.end(); ++it)
	{
		pDvrWnd = (*it);
		if(pDvrWnd)
		{
			delete pDvrWnd;
			pDvrWnd = NULL;
		}
	}
	m_vecDvrWnd.clear();
	UNINIT_LOCK;
}

int CDvrMgrBase::GetWndIndexByRowCol(int row, int col)
{
	return (row * SHAREDATA.g_dvrProp.numCol + col);
}

bool CDvrMgrBase::GetRowCol(const wstring& strPort, int& row, int& col)
{
	int index = LOCATIONCONF.GetWndIndex(strPort);
	if(index == -1)
		return false;

	GetRowCol(index, row, col);
	if(row == -1 || col == -1)
		return false;

	return true;
}

void CDvrMgrBase::GetRowCol( int nWndIndex, int& row, int& col )
{
	row = (nWndIndex / 10) - 1;
	col = nWndIndex - (row + 1)*10 - 1;
}


int CDvrMgrBase::GetWndIndex(const wstring& strPort)
{
	int index = LOCATIONCONF.GetWndIndex(strPort);
	if(index == -1)
		return -1;

	int row = -1;
	int col = -1;
	GetRowCol(index, row, col);

	index = (row * SHAREDATA.g_dvrProp.numCol + col);
	return index >= SHAREDATA.g_dvrProp.numDvr ? -1 : index;
}

CDvrWnd* CDvrMgrBase::GetDvr(const wstring& strPort)
{
	int nIndex = GetWndIndex(strPort);
	if(nIndex == -1)
	{
		return NULL;
	}
	return GetDvr(nIndex);
}

CDvrWnd* CDvrMgrBase::GetDvr(int wndIndex)
{
	if(wndIndex < 0 || wndIndex > SHAREDATA.g_dvrProp.numDvr)
		return NULL;
	return m_vecDvrWnd[wndIndex];
}

CDvrWnd* CDvrMgrBase::GetDvr(int row, int col)
{
	int index = GetWndIndexByRowCol(row, col);
	return GetDvr(index);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

CDvrMgr::CDvrMgr(void)
{
	m_vecClear.reserve(SHAREDATA.g_dvrProp.numDvr);
	//AfxBeginThread((AFX_THREADPROC)ClearProc, NULL);

	for (int i = 0; i < SHAREDATA.g_dvrProp.numDvr; ++i)
	{
		m_mapDvrWnd[i] = new DvrWndInfo;
	}
}

CDvrMgr::~CDvrMgr()
{
	for (int i = 0; i < SHAREDATA.g_dvrProp.numDvr; ++i)
	{
		DvrWndInfo* pDwi = m_mapDvrWnd[i];
		if(pDwi)
		{
			delete pDwi; 
			pDwi = NULL;
		}
	}
	m_mapDvrWnd.clear();
}

CDvrMgr& CDvrMgr::Instance()
{
	static CDvrMgr dm;
	return dm;
}


CDvrWnd* CDvrMgr::PutDvr(const wstring& strPort)
{
	LOCK;
	if(strPort.empty())
	{
		return NULL;
	}

	int index = -1;
#ifdef ONE_DVR
	index = 0;
#else
	index = GetWndIndex(strPort);
#endif

	if(index == -1)
	{
		map<wstring, int>::iterator itF = m_mapAlertPort.find(strPort);
		if(itF != m_mapAlertPort.end())
		{
			if(itF->second == 1)
			{
				return NULL;
			}
		}

		m_mapAlertPort[strPort] = 1;
		SHAREDATA.g_pMainFrame->Alert(_T("�˽ӿ�û�ж�λ"));
		SHAREDATA.g_pMainFrame->ShowMsgInfo(_T("�˽ӿ�û�ж�λ"));
		m_mapAlertPort[strPort] = 0;

		return NULL;
	}
	CDvrWnd* dvr = m_vecDvrWnd[index];

	if(dvr == NULL)
	{
		return NULL;
	}

	CDevBase* pDev = dvr->GetDevPtr();
	if(pDev != NULL)
	{
		if(dvr->GetId().IsEmpty())
		{
			delete pDev;
			pDev = NULL;
		}
		else
			return dvr;
	}

	CString strID;
	if( !GetDvrId(strID, &pDev) )
	{					
		LOGS(_T("��ȡ�豸IDʧ�ܻ����豸IDΪ��"));
		SHAREDATA.g_pMainFrame->ShowMsgInfo(_T("��ȡ�豸IDʧ�ܻ����豸IDΪ��"));
		delete pDev;
		pDev = NULL;
		return NULL;
	}

	CString strName = DB.QueryNameFormID(strID); //DB.GetNameFormID(strID);

	dvr->SetDevPtr(pDev);
	dvr->SetStatus(DVR_NORMAL);

	//�洢����
	dvr->SetId( strID );
	dvr->SetName( strName );
	dvr->SetStatusStr(_T("���Ժ�, DVR�豸����׼������..."));

	//ͬ��ʱ��
	pDev->SetTimeEx();

	//���벢��ȡIDʱ������
	DvrWndInfo* pDwi = m_mapDvrWnd[index];
	if(pDwi)
	{
		pDwi->m_strId = dvr->GetId();
		pDwi->m_strName = dvr->GetName();	
		pDwi->m_strDisk.Empty();
		pDwi->m_status = dvr->GetStatus();
		pDwi->m_strStatus = dvr->GetStatusStr();
	}

	InsertDvrDevice(strID, _T("DVR�豸����"));

	return dvr;
}

//modified by yjt 2014-06-12 �����a4,d6,x6������֧��
bool CDvrMgr::GetDvrId(CString& strId, CDevBase** pDvr)
{
	LOCK;
	strId.Empty();
	
	CDevBase* pDev = CREATE_DVR;
	if(pDev == NULL)
		return false;

	BOOL bret = pDev->GetIDEx(strId);
	if( !bret || strId.IsEmpty())
	{
		delete pDev;
		pDev = NULL;
		return false;
	}
	*pDvr = pDev;
	return true;
}


void CDvrMgr::RemoveDvr(const wstring& strPort)
{
	LOCK;
	if(strPort.empty())
	{
		return;
	}

	int index = GetWndIndex(strPort);
	if(index == -1)
	{
		return;
	}

	CDvrWnd *dvr = m_vecDvrWnd[index];
	if(dvr)
	{
			InsertDvrDevice(dvr->GetId(), _T("DVR�豸�γ�"));

			dvr->RemoveDevPtr();
			dvr->ClearInfo();
			dvr->SetStatus(DVR_DISABLE);
	}
}

void CDvrMgr::CopyDvrFile(const wstring& strPort, const wstring& diskName)
{
	LOCK;
	int nWndIndex = -1;
#ifdef ONE_DVR
	nWndIndex = 0;
#else
	nWndIndex = GetWndIndex(strPort);
#endif
	if(nWndIndex == -1)
	{
		return;
	}

	CDvrWnd* pDvr = GetDvr(nWndIndex);
	if(pDvr == NULL)
		return;

	DvrWndInfo* pDwi = m_mapDvrWnd[nWndIndex];

	if(pDvr->GetId().IsEmpty())
	{
		//���IDΪ��, ͬʱ�̷�û�䣬���û���Ļָ�
		if(pDwi && !pDwi->m_strId.IsEmpty() && pDwi->m_strDisk.Compare(diskName.c_str()) == 0)
		{
			pDvr->SetId(pDwi->m_strId);
			pDvr->SetName(pDwi->m_strName);
			pDvr->SetStatus(pDwi->m_status);
			pDvr->SetStatusStr(pDwi->m_strStatus);
			pDvr->SetCopying(false);
		}
		else
		{
			return;
		}
	}
	else
	{
		pDwi->m_strId = pDvr->GetId();
		pDwi->m_strDisk  = diskName.c_str();
		pDwi->m_strName = pDvr->GetName();
		pDwi->m_status = pDvr->GetStatus();
		pDwi->m_strStatus = pDvr->GetStatusStr();
		pDwi->nFileCount = 0;// pDvr->GetProgressMax();
		pDwi->nTransFileCount = 0;// pDvr->GetProgressValue();
	}

	if(!pDvr->IsCopying())
	{
		//�����̷�
		pDvr->SetDisk(diskName.c_str());
		pDvr->CopyDir(diskName.c_str());
	}
}

UINT ClearProc(LPVOID param)
{
	Sleep(1000);
	CDvrMgr *pMgr = NULL;
	while(pMgr == NULL)
	{
		pMgr = &(CDvrMgr::Instance());
		Sleep(20);
	}

	pMgr->ClearInValidDvr();

	return 0;
}

//////////////////////////////////////////////////////////////////////////
//��������������õĴ�������
//		1. ��ID�Ĵ��ڣ�5 - 20 ���û���̷������
//		2. �ն���ID�Ͱγ��豸�����Ҳ��5����������
//////////////////////////////////////////////////////////////////////////
void CDvrMgr::ClearInValidDvr()
{
	while(true)
	{
		for (int i = 0; i < SHAREDATA.g_dvrProp.numDvr; ++i)
		{
			CDvrWnd* dvr = m_vecDvrWnd[i];
			CDevBase* pDev = dvr->GetDevPtr();
			if(pDev)
			{
				if(!dvr->GetDisk().IsEmpty())
				//if(pDev->IsMassed())
				{
					//�Ѿ�ö�ٳ�U�̾Ͳ��ù�����
					m_vecClear[i] = 0;
				}
				else
				{
					//���豸�����pDev����Ч�ģ�ͬʱδö�ٳ�U��
					//�ȴ�5/10���������δ���U��(û���̷�)������Ϊ�ǲ��״̬
					int iClear = m_vecClear[i];
					if(iClear == 100)
					{
						dvr->RemoveDevPtr();
						dvr->ClearInfo();
						dvr->SetStatus(DVR_DISABLE);
						m_vecClear[i] = 0;
					}
					else
					{
						iClear++;
						m_vecClear[i] = iClear;
					}	
				}
				
			}
			else
			{
				m_vecClear[i] = 0;
			}
		}

		Sleep(100);
	}
}

//����DVR�豸��־
void CDvrMgr::InsertDvrDevice(const CString& strID, const CString& strLog)
{
	if(strID.IsEmpty())
		return;

	LOG(_T("DVR�豸[%s], %s"), strID, strLog);

	DVR_DEVICE_LOG dvrLog;
	dvrLog.strDvrNumber = strID;
	dvrLog.strLogContent = strLog;
	CTime tm = CTime::GetCurrentTime();
	dvrLog.strDeviceOperTime =  tm.Format(_T("%Y-%m-%d %H:%M:%S"));
	dvrLog.strDeviceInsertTime = dvrLog.strDeviceOperTime;

	//��ȡ�û���
	TCHAR userName[MAX_PATH];
	CString sUsername,cName;
	DWORD size = MAX_PATH;
	::GetUserName(userName,&size);
	sUsername.Format(_T("%s"),userName);
	dvrLog.strDeviceOperUser = sUsername;

	dvrLog.strRemark = _T(" ");
	DB.InsertDvrLog(dvrLog);
}
