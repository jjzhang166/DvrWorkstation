#include "stdafx.h"
#include "FtpMgr.h"
#include "../config/Config.h"
#include "../ShareData.h"
#include "../WellCommon/AviHelper.h"
#include "../WellCommon/StringHelper.h"

typedef struct tagResolution
{
	int w;
	int h;
}Resolution,*PResolution;

Resolution g_resolution[] ={
	{176,144},
	{352,288},
	{720,576},
	{1280,720},
	{1280,960},
	{1920,1080}
};




CFtpMgr::CFtpMgr(void)
	: m_bFtpValid(false)
	, m_nHDDef(3)
{

}


CFtpMgr::~CFtpMgr(void)
{

}

CFtpMgr& CFtpMgr::Instance()
{
	static CFtpMgr ftp;
	return ftp;
}


bool CFtpMgr::Run()
{
	bool bRlt = false;

	CString strImp;
	//��ȡ��������
	CONF.GetFtpSetting(strImp, m_nHDDef);
	m_strFlagIMP = strImp.GetString();

	m_vecLevel = CONF.GetFtpLevel();

	wstring strIp, strUser, strPwd;
	wstring strDir(SHAREDATA.g_strFilePath.GetBuffer());

	m_fiOne = CONF.GetFTPInfo(SEC_FTP_ONE);
	m_fiFirst = CONF.GetFTPInfo(SEC_FTP_FIRST);
	m_fiSecond = CONF.GetFTPInfo(SEC_FTP_SECOND);

	InitLevelEx();

#ifdef ONE_DVR
	//���е������FTP����
	if(m_fiOne.bEnable)
	{
		strIp = m_fiOne.strServ.GetString();
		strUser = m_fiOne.strUser.GetString();
		strPwd = m_fiOne.strPwd.GetString();

		bRlt = m_ftpOne.Start(strIp, strUser, strPwd, m_fiOne.nPort, strDir);
		if(bRlt)
		{
			RegCB cb;
			cb.tfCB = FtpOneTransFileCB;
			cb.ttNotify = FtpTransMsgCB;

			m_ftpOne.Register(cb);
			m_ftpOne.ServiceRun();
		}

		m_bFtpValid |= bRlt;
	}
#else
	//һ��FTP����
	if(m_fiFirst.bEnable)
	{
		strIp = m_fiFirst.strServ.GetString();
		strUser = m_fiFirst.strUser.GetString();
		strPwd = m_fiFirst.strPwd.GetString();
		bRlt = m_ftpFirst.Start(strIp, strUser, strPwd, m_fiFirst.nPort, strDir);
		if(bRlt)
		{
			RegCB cb;
			cb.tfCB = FtpFirstTransFileCB;
			cb.ttNotify = Ftp1TransMsgCB;

			m_ftpFirst.Register(cb);
			m_ftpFirst.ServiceRun();
		}
		m_bFtpValid |= bRlt; 
	}

	//����FTP����
	if(m_fiSecond.bEnable)
	{
		strIp = m_fiSecond.strServ.GetString();
		strUser = m_fiSecond.strUser.GetString();
		strPwd = m_fiSecond.strPwd.GetString();
		bRlt = m_ftpSecond.Start(strIp, strUser, strPwd, m_fiSecond.nPort, strDir);
		if(bRlt)
		{
			RegCB cb;
			cb.tfCB = FtpSecondTransFileCB;
			cb.ttNotify = Ftp2TransMsgCB;

			m_ftpSecond.Register(cb);
			m_ftpSecond.ServiceRun();
		}
		m_bFtpValid |= bRlt;
	}
#endif

	return m_bFtpValid;
}

void CFtpMgr::GetTransInfo()
{
	if(!m_bFtpValid)
	{
		m_transInfo(_T(""), _T("��Ϣ��ʾ"), 0);
		return;
	}

	CFtpHelper& ftp = GetCurWorkFtp();

	//���FTP����ٷֱ�
	int nPrecent = ftp.GetTransPrecent() * 100;
	wstring strPutFile = ftp.GetPutFile();
	CString strInfo;
	
	if(!strPutFile.empty())
	{
		if(ftp == m_ftpOne)
			strPutFile = _T("[����FTP]�ϴ��ļ���") + strPutFile;
		else if(ftp == m_ftpFirst)
			strPutFile = _T("[һ��FTP]�ϴ��ļ���") + strPutFile;
		else if(ftp == m_ftpSecond)
			strPutFile = _T("[����FTP]�ϴ��ļ���") + strPutFile;
		strInfo.Format(_T("���:%d%% %s"), nPrecent, ftp.GetTransSpeed().c_str());
	}
	else
	{
		strInfo = _T("��Ϣ��ʾ");
		nPrecent = 0;
	}
	m_transInfo(strPutFile, strInfo.GetString(), nPrecent);
}

//////////////////////////////////////////////////////////////////////////
int __stdcall FtpOneTransFileCB(const wstring& strCurTransFile)
{
	return FTP_CLIENT.FtpOneTransFileFunc(strCurTransFile);
}

int __stdcall FtpFirstTransFileCB(const wstring& strCurTransFile)
{
	return FTP_CLIENT.FtpFirstTransFileFunc(strCurTransFile);
}

int __stdcall FtpSecondTransFileCB(const wstring& strCurTransFile)
{
	return FTP_CLIENT.FtpSecondTransFileFunc(strCurTransFile);
}

int __stdcall FtpTransMsgCB(EM_MSG_TYPE type)
{
	return FTP_CLIENT.FtpTransMsgFunc(type);
}

int __stdcall Ftp1TransMsgCB(EM_MSG_TYPE type)
{
	return FTP_CLIENT.Ftp1TransMsgFunc(type);
}

int __stdcall Ftp2TransMsgCB(EM_MSG_TYPE type)
{
	return FTP_CLIENT.Ftp2TransMsgFunc(type);
}

//////////////////////////////////////////////////////////////////////////

int CFtpMgr::FtpOneTransFileFunc(const wstring& strCurTransFile)
{
	int nRlt = IsContinueTrans(strCurTransFile, m_fiOne);
	if(nRlt == TRANS_CONTUNE)
	{
		//�����ϴ���ɺ�ɾ���ļ�
		return TRANS_DELETE;
	}
	return nRlt;
}

int CFtpMgr::FtpFirstTransFileFunc(const wstring& strCurTransFile)
{	
	return IsContinueTrans(strCurTransFile, m_fiFirst);
}

int CFtpMgr::FtpSecondTransFileFunc(const wstring& strCurTransFile)
{
	return IsContinueTrans(strCurTransFile, m_fiSecond);
}

int CFtpMgr::IsContinueTrans(const wstring& strFile, const FTPInfo& ftpIf)
{
	int nRlt = TRANS_SKIP;

	//��Ƶ�ļ�����
	if(IsVideoFile(strFile))
	{
		//�����ص���Ƶ
		if(ftpIf.bTransIMP && IsIMPVideo(strFile))
			nRlt = TRANS_CONTUNE;

		//���͸�����Ƶ
		if(ftpIf.bTransHD && IsHDVideo(strFile))
			nRlt = TRANS_CONTUNE;

		//���ͱ�����Ƶ
		if(ftpIf.bTransSD)
			nRlt = TRANS_CONTUNE;
	}
	
	if(nRlt == TRANS_CONTUNE)
		GetTransInfo();

	return nRlt;
}

bool CFtpMgr::IsVideoFile(const wstring& strFile)
{
	return ( strFile.rfind(_T(".AVI")) != wstring::npos || strFile.rfind(_T(".avi")) != wstring::npos );
}

bool CFtpMgr::IsIMPVideo(const wstring& strFile)
{
	return ( strFile.rfind(m_strFlagIMP) != wstring::npos );
}

bool CFtpMgr::IsHDVideo(const wstring& strFile)
{
	int width = 0;
	int height = 0;
	if( !CAviHelper::AVI_resolution(strFile, width, height) )
		return false;

	return IsHD(width, height);
}

bool CFtpMgr::IsHD(int width, int height)
{
	Resolution &rl = g_resolution[m_nHDDef];
	return (width >= rl.w && height >= rl.h);
}

int CFtpMgr::FtpTransMsgFunc(EM_MSG_TYPE type)
{
	int nRlt;
	switch(type)
	{
	case TRANS_BEGIN:

		while(IsWait(SEC_FTP_ONE))
		{
			Sleep(10);
		}

		m_transInfo(_T(""), _T("[����FTP]��ʼ����!"), 0);
		Sleep(1000);

		nRlt = TRANS_CONTUNE;
		break;
	case TRANS_END:

		m_transInfo( _T(""), _T("[����FTP]�������!"),0);
		
		m_cs.Lock();
		m_qLevel.pop();
		m_qLevel.push(&m_fiOne);
		m_cs.Unlock();

		//����ɾ�����ļ���
		nRlt = TRANS_DELETE;
		break;
	default:
		nRlt = TRANS_CONTUNE;
	}

	return nRlt;
}

int CFtpMgr::Ftp1TransMsgFunc(EM_MSG_TYPE type)
{
	int nRlt;
	switch(type)
	{
	case TRANS_BEGIN:
		while(IsWait(SEC_FTP_FIRST))
		{
			Sleep(10);
		}
		m_transInfo(_T(""), _T("[һ��FTP]��ʼ����!"), 0);
		Sleep(1000);

		nRlt = TRANS_CONTUNE;
		break;
	case TRANS_END:
		m_transInfo(_T(""), _T("[һ��FTP]�������!"), 0);

		m_cs.Lock();
		m_qLevel.pop();
		m_qLevel.push(&m_fiFirst);
		m_cs.Unlock();

		nRlt = TRANS_DELETE;
		break;
	default:
		nRlt = TRANS_CONTUNE;
	}

	return nRlt;
}

int CFtpMgr::Ftp2TransMsgFunc(EM_MSG_TYPE type)
{
	int nRlt;
	switch(type)
	{
	case TRANS_BEGIN:
		while(IsWait(SEC_FTP_SECOND))
		{
			Sleep(10);
		}

		m_transInfo(_T(""), _T("[����FTP]��ʼ����!"), 0);
		Sleep(1000);

		nRlt = TRANS_CONTUNE;
		break;
	case TRANS_END:
		m_transInfo(_T(""), _T("[����FTP]�������!"), 0);

		m_cs.Lock();
		m_qLevel.pop();
		m_qLevel.push(&m_fiSecond);
		m_cs.Unlock();

		nRlt = TRANS_DELETE;
		break;
	default:
		nRlt = TRANS_CONTUNE;
	}

	return nRlt;
}


//////////////////////////////////////////////////////////////////////////
//��ǰ������FTP�������ȼ��й�ϵ��
//
//////////////////////////////////////////////////////////////////////////
CFtpHelper& CFtpMgr::GetCurWorkFtp()
{
	FTPInfo* p = m_qLevel.front();
	if(p == &m_fiOne)
		return m_ftpOne;
	else if( p == &m_fiFirst )
		return m_ftpFirst;
	else if( p == &m_fiSecond )
		return m_ftpSecond;
}

//////////////////////////////////////////////////////////////////////////
//���ܣ��ж�ָ���ļ����Ƿ���Ҫ�ȴ�
//		1. ��ǰ�����Ƿ��ڶ��е�ͷ��
//			������Ǿ͵ȴ��������������ȴ�
//		2. ������һ�κ���У������
//������
//		level [IN]	--	����["FTP","FTP1","FTP2"]
//���أ�
//		true: ��Ҫ�ȴ�
//		false: ����ȴ�
//////////////////////////////////////////////////////////////////////////
bool CFtpMgr::IsWait(const wstring& level)
{
	m_cs.Lock();
	bool bRlt = false;
	if(level == SEC_FTP_ONE)
	{
		if(m_qLevel.front() != &m_fiOne)
			bRlt = true;
	}
	else if(level == SEC_FTP_FIRST)
	{
		if(m_qLevel.front() != &m_fiFirst)
			bRlt = true;
	}
	else if(level == SEC_FTP_SECOND)
	{
		if(m_qLevel.front() != &m_fiSecond)
			bRlt = true;
	}
	m_cs.Unlock();
	return bRlt;
}

//��ʼ��������չ����
void CFtpMgr::InitLevelEx()
{
	m_cs.Lock();
	for (int i = 0; i < m_vecLevel.size(); ++i)
	{
#ifdef ONE_DVR
		if(m_vecLevel[i] == SEC_FTP_ONE)
		{
			if(m_fiOne.bEnable)
				m_qLevel.push(&m_fiOne);
		}
#else
		if(m_vecLevel[i] == SEC_FTP_FIRST)
		{
			if(m_fiFirst.bEnable)
				m_qLevel.push(&m_fiFirst);
		}
		else 
		{
			if(m_fiSecond.bEnable)
				m_qLevel.push(&m_fiSecond);
		}
#endif

	}
	m_cs.Unlock();
}
