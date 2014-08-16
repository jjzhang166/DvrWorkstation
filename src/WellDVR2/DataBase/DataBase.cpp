#include "stdafx.h"
#include "DataBase.h"
#include "../IniFile.h"
#include "../DataDef.h"
#include "../ShareData.h"
#include "../Config.h"


CDataBase::CDataBase(void)
	: m_bInit(false)
{
	if(!Init())
	{
		SHAREDATA.g_pMainFrame->Alert(m_db.GetLastError().c_str());
	}
}


CDataBase::~CDataBase(void)
{
	UnInit();
}

BOOL CDataBase::Init()
{
	DBInfo dbi = CONF.GetDBInfo();
	//add by yjt 2014-06-13 ������ݿ�����Ե�����
	if(!dbi.bEnable)
	{
		m_bInit = false;
		return TRUE;
	}

	CString strConnect;
	strConnect.Format(_T("Provider=OraOLEDB.Oracle.1;Password=%s;Persist Security Info=True;User ID=%s;\
					  Data Source=\"(DESCRIPTION =(ADDRESS_LIST =(ADDRESS = (PROTOCOL = TCP)(HOST = %s)(PORT = %d)) )(CONNECT_DATA = (SID = %s)))\""),
					  dbi.strPwd, dbi.strUser, dbi.strHost, dbi.nPort, dbi.strSid);

	BOOL bRlt = m_db.Init(strConnect.GetBuffer());
	strConnect.ReleaseBuffer();

	m_bInit = true;

	return bRlt;
}

void CDataBase::UnInit()
{
	m_db.UnInit();
	m_bInit = false;
}

CDataBase& CDataBase::Instance()
{
	static CDataBase db;
	return db;
}

//�ӻ����л�ȡ���ݣ����id�����ڣ���ˢ�»���
//�����̨�ı��DVRNUM��Ӧ�����ƣ���û�취ˢ��
CString CDataBase::GetNameFormID(const CString& id)
{
	CString str(_T(""));
	if( !m_db.IsConnect() )
		return str;

	MapDvrIdNameItor it = mapDvr.find(id);
	if(it != mapDvr.end())
	{
		return (*it).second;
	}

	InitUserRec();
	
	it = mapDvr.find(id);
	if(it != mapDvr.end())
	{
		return (*it).second;
	}
	return _T("");
}

//modified by yjt 2014-4-17 ��DVR ��Ϊ mycar.DVR
void CDataBase::InitUserRec()
{
	_RecordsetPtr record = m_db.Query(_T("select DVRNUM,REALNAME from mycar.DVR"));
	if(record)
	{
		FillUserVector(record);
	}
}


void CDataBase::FillUserVector(_RecordsetPtr record)
{
	_variant_t var;
	CString strID, strName;

	mapDvr.clear();

	try
	{
		if( !record->BOF )
			record->MoveFirst();

		while( !record->adoEOF )
		{
			var = record->GetCollect("DVRNUM");
			if( var.vt != VT_NULL )
			{
				strID = (LPCSTR)_bstr_t(var);
				strID.TrimRight();
				strID.TrimLeft();
			}			

			var = record->GetCollect("REALNAME");
			if( var.vt != VT_NULL )
			{
				strName = (LPCSTR)_bstr_t(var);
				strName.TrimRight();
				strName.TrimLeft();
			}			

			mapDvr[strID] = strName;

			record->MoveNext();
		}
	}
	catch(_com_error *e)
	{
		SHAREDATA.g_pMainFrame->Alert(e->ErrorMessage());
	}
}

//����IDʵʱ��ѯ����
CString CDataBase::QueryNameFormID(const CString& id)
{
	if( !IsValid())
		return _T("");

	CString strSql;
	strSql.Format(_T("select DVRNUM,REALNAME from mycar.DVR where DVRNUM='%s'"), id);
	_RecordsetPtr record = m_db.Query(strSql.GetString());
	if(record == NULL)
	{
		return _T("");
	}

	CString strName(_T(""));
	try
	{
		if( !record->BOF )
			record->MoveFirst();

		_variant_t var;
		while( !record->adoEOF )
		{
			var = record->GetCollect("REALNAME");
			if( var.vt != VT_NULL )
			{
				strName = (LPCSTR)_bstr_t(var);
				strName.TrimRight();
				strName.TrimLeft();
			}			
			record->MoveNext();
		}
	}
	catch(_com_error *e)
	{
		SHAREDATA.g_pMainFrame->Alert(e->ErrorMessage());
	}

	mapDvr[id] = strName;

	return strName;
}

//////////////////////////////////////////////////////////////////////////
//������������Ƶ��ý����Ϣ)
//ʱ�䣺created by yjt 2014-07-03
//�洢�������£�
// 			P_INSERT_DVRMEDIA_INFO (
// 				iDvrNumber in varchar2, -- DVR���
// 				iFileName in varchar2, --�ļ���
// 				iFileSize in number, --�ļ���С
// 				iViewWidth in number, -- ��
// 				iViewHeight in number, -- ��
// 				iMediaDuration in number, --ʱ��
// 				iFrameRate in varchar2, --֡��
// 				iDataUploadTime in date, --�ϴ�ʱ��
// 				iMediaStartTime in date, --��ʼʱ��
// 				iMediaEndTime in date, --����ʱ��
// 				iRecordTime in date, -- ¼��ʱ��
// 				iFileType in varchar2, -- �ļ�����
// 				iVideoType in varchar2, -- �ص�/����/����
// 				iLocateFilePath in varchar2, --�����ļ�·��
// 				iFtpFileUrl in varchar2, --ftp�ļ�·��
// 				iIISFileUrl in varchar2, --iis�ļ�·��
// 				iDataIp in varchar2, --���ݴ洢ip
// 				iRemark in varchar2 --��ע
// 				)
//////////////////////////////////////////////////////////////////////////
BOOL CDataBase::InsertMediaInfo(const DVRMEDIA_INFO& info)
{
	if(!IsValid())
	{
		return FALSE;
	}

	wstring strProcName(_T("P_INSERT_DVRMEDIA_INFO"));

	_ParameterPtr param;
	//����
	VecParams mapParams;
	FieldInfo fi;
	fi.strValue = info.strDvrNumber; //_T("000000000000010001");
	fi.nLen = fi.strValue.length();
	fi.emType = adVarChar;
	fi.strName = _T("iDvrNumber");
	mapParams.push_back(fi);

	fi.strValue = info.strFileName; //_T("00000001_20140706.AVI");
	fi.nLen = fi.strValue.length();
	fi.emType = adVarChar;
	fi.strName = _T("iFileName");
	mapParams.push_back(fi);

	fi.strValue = info.strFileSize; //_T("2343243");
	fi.nLen = fi.strValue.length();
	fi.emType = adVarChar;
	fi.strName = _T("iFileSize");
	mapParams.push_back(fi);

	fi.strValue = info.strViewWidth; //_T("1080");
	fi.nLen = fi.strValue.length();
	fi.emType = adVarChar;
	fi.strName = _T("iViewWidth");
	mapParams.push_back(fi);

	fi.strValue = info.strViewHeight;	//_T("720");
	fi.nLen = fi.strValue.length();
	fi.emType = adVarChar;
	fi.strName = _T("iViewHeight");
	mapParams.push_back(fi);

	fi.strValue = info.strMediaDuration; //_T("69");
	fi.nLen = fi.strValue.length();
	fi.emType = adInteger;
	fi.strName = _T("iMediaDuration");
	mapParams.push_back(fi);

	fi.strValue = info.strFrameRate; //_T("30");
	fi.nLen = fi.strValue.length();
	fi.emType = adVarChar;
	fi.strName = _T("iFrameRate");
	mapParams.push_back(fi);

	fi.strValue = info.strDataUploadTime; //_T("2014-07-03 09:10:10");
	fi.nLen = fi.strValue.length();
	fi.emType = adVarChar;
	fi.strName = _T("iDataUploadTime");
	mapParams.push_back(fi);

	fi.strValue = info.strMediaStartTime; //_T("2014-07-03 09:10:10");
	fi.nLen = fi.strValue.length();
	fi.emType = adVarChar;
	fi.strName = _T("iMediaStartTime");
	mapParams.push_back(fi);

	fi.strValue = info.strMediaEndTime; //_T("2014-07-03 09:10:10");
	fi.nLen = fi.strValue.length();
	fi.emType = adVarChar;
	fi.strName = _T("iMediaEndTime");
	mapParams.push_back(fi);
	
	fi.strValue = info.strRecordTime; //_T("2014-07-03 09:10:10");
	fi.nLen = fi.strValue.length();
	fi.emType = adVarChar;
	fi.strName = _T("iRecordTime");
	mapParams.push_back(fi);

	fi.strValue = info.strFileType; //_T("AVI");
	fi.nLen = fi.strValue.length();
	fi.emType = adVarChar;
	fi.strName = _T("iFileType");
	mapParams.push_back(fi);

	fi.strValue = info.strVideoType; //_T("IPM");
	fi.nLen = fi.strValue.length();
	fi.emType = adVarChar;
	fi.strName = _T("iVideoType");
	mapParams.push_back(fi);

	fi.strValue = info.strLocateFilePath; //_T("0000001/DCIM/RECORD/00000001_20140706.AVI");
	fi.nLen = fi.strValue.length();
	fi.emType = adVarChar;
	fi.strName = _T("iLocateFilePath");
	mapParams.push_back(fi);

	fi.strValue = info.strFtpFileUrl; //_T("ftp://192.168.1.1:21/0000001/DCIM/RECORD/00000001_20140706.AVI");
	fi.nLen = fi.strValue.length();
	fi.emType = adVarChar;
	fi.strName = _T("iFtpFileUrl");
	mapParams.push_back(fi);

	fi.strValue = info.strIISFileUrl; //_T("http://192.168.1.1:80/0000001/DCIM/RECORD/00000001_20140706.AVI");
	fi.nLen = fi.strValue.length();
	fi.emType = adVarChar;
	fi.strName = _T("iIISFileUrl");
	mapParams.push_back(fi);

	fi.strValue = info.strDataIp;	//_T("192.168.1.1");
	fi.nLen = fi.strValue.length();
	fi.emType = adVarChar;
	fi.strName = _T("iDataIp");
	mapParams.push_back(fi);	

	fi.strValue = info.strRemark;	//_T(" ");
	fi.nLen = fi.strValue.length();
	fi.emType = adVarChar;
	fi.strName = _T("iRemark");
	mapParams.push_back(fi);

	return m_db.ExecProc(strProcName, mapParams);
}

//////////////////////////////////////////////////////////////////////////
//����������DVR��־
//ʱ�䣺created by yjt 2014-07-03
//�洢�������£�
// 			P_INSERT_DVR_DEVICE_LOG (iDvrNumber in varchar2, --dvr���
// 				iDeviceInsertTime in date, --�豸����ʱ��
// 				iDeviceOperTime in date, --�豸����ʱ��
// 				iDeviceOperUser in varchar2, --�豸�����û�
// 				iLogContent in varchar2, --��־����
// 				iRemark in varchar2 --��ע
// 				)
//////////////////////////////////////////////////////////////////////////
BOOL CDataBase::InsertDvrLog(const DVR_DEVICE_LOG& ddLog)
{
	if(!IsValid())
	{
		return FALSE;
	}

	wstring strProcName(_T("P_INSERT_DVR_DEVICE_LOG"));
	//����
	VecParams mapParams;
	FieldInfo fi;
	fi.strValue = ddLog.strDvrNumber;	//_T("000000000000010001");
	fi.nLen = fi.strValue.length();
	fi.emType = adVarChar;
	fi.strName = _T("iDvrNumber");
	mapParams.push_back(fi);

	fi.strValue = ddLog.strDeviceInsertTime;	//_T("2014-07-03 09:10:10");
	fi.nLen = fi.strValue.length();
	fi.emType = adVarChar;
	fi.strName = _T("iDeviceInsertTime");
	mapParams.push_back(fi);

	fi.strValue = ddLog.strDeviceOperTime;	//_T("2014-07-03 09:10:10");
	fi.nLen = fi.strValue.length();
	fi.emType = adVarChar;
	fi.strName = _T("iDeviceOperTime");
	mapParams.push_back(fi);

	fi.strValue = ddLog.strDeviceOperUser;	//_T("Administrator");
	fi.nLen = fi.strValue.length();
	fi.emType = adVarChar;
	fi.strName = _T("iDeviceOperUser");
	mapParams.push_back(fi);

	fi.strValue = ddLog.strLogContent;	//_T("�ɼ��ն˻�����");
	fi.nLen = fi.strValue.length();
	fi.emType = adVarChar;
	fi.strName = _T("iLogContent");
	mapParams.push_back(fi);

	fi.strValue = ddLog.strRemark;	//_T(" ");
	fi.nLen = fi.strValue.length();
	fi.emType = adVarChar;
	fi.strName = _T("iRemark");
	mapParams.push_back(fi);


	return m_db.ExecProc(strProcName, mapParams);
}

//////////////////////////////////////////////////////////////////////////
//�����������ն���־
//ʱ�䣺created by yjt 2014-07-03
//�洢�������£�
// 			//�ն���־
// 			P_INSERT_TERMINAL_DEV_LOG (
// 				iDeviceNumber IN VARCHAR2, --�豸��
// 				iHarewareVersion IN VARCHAR2, --Ӳ���汾
// 				iSoftwareVersion IN VARCHAR2, --����汾
// 				iDiskSpace IN NUMBER, --���̿ռ�
// 				iTerminalIp IN VARCHAR2, --IP
// 				iIsusing IN VARCHAR2, --�Ƿ����� -Y�� -N��
// 				iDetachment IN VARCHAR2, --֧��
// 				iBattalion IN VARCHAR2, --���
// 				iSquadron IN VARCHAR2, --�ж�
// 				iDescribe IN VARCHAR2, --����
// 				iRemark IN VARCHAR2 --��ע
// 				)
//////////////////////////////////////////////////////////////////////////
BOOL CDataBase::InsertLog(const TERMINAL_DEV_LOG& tdLog)
{
	if(!IsValid())
	{
		return FALSE;
	}

	wstring strProcName(_T("P_INSERT_TERMINAL_DEV_LOG"));
	//����
	VecParams mapParams;

	FieldInfo fi;
	fi.strValue = tdLog.strDeviceNumber;	//_T("000000000000020001");
	fi.nLen = fi.strValue.length();
	fi.emType = adVarChar;
	fi.strName = _T("iDeviceNumber");
	mapParams.push_back(fi);

	fi.strValue = tdLog.strHarewareVersion;	//_T("V1.0.0");
	fi.nLen = fi.strValue.length();
	fi.emType = adVarChar;
	fi.strName = _T("iHarewareVersion");
	mapParams.push_back(fi);

	fi.strValue = tdLog.strSoftwareVersion;	//_T("V1.0.0.5");
	fi.nLen = fi.strValue.length();
	fi.emType = adVarChar;
	fi.strName = _T("iSoftwareVersion");
	mapParams.push_back(fi);

	fi.strValue = tdLog.strDiskSpace;	//_T("2000000");
	fi.nLen = fi.strValue.length();
	fi.emType = adVarChar;
	fi.strName = _T("iDiskSpace");
	mapParams.push_back(fi);

	fi.strValue = tdLog.strTerminalIp;	//_T("192.168.1.2");
	fi.nLen = fi.strValue.length();
	fi.emType = adVarChar;
	fi.strName = _T("iTerminalIp");
	mapParams.push_back(fi);

	fi.strValue = tdLog.strIsusing;		//_T("1");
	fi.nLen = fi.strValue.length();
	fi.emType = adVarChar;
	fi.strName = _T("iIsusing");
	mapParams.push_back(fi);

	fi.strValue = tdLog.strDetachment;	//_T("֧��");
	fi.nLen = fi.strValue.length();
	fi.emType = adVarChar;
	fi.strName = _T("iDetachment");
	mapParams.push_back(fi);

	fi.strValue = tdLog.strBattalion;	//_T("���");
	fi.nLen = fi.strValue.length();
	fi.emType = adVarChar;
	fi.strName = _T("iBattalion");
	mapParams.push_back(fi);

	fi.strValue = tdLog.strSquadron;	//_T("�ж�");
	fi.nLen = fi.strValue.length();
	fi.emType = adVarChar;
	fi.strName = _T("iSquadron");
	mapParams.push_back(fi);

	fi.strValue = tdLog.strDescribe;	//_T("����������");
	fi.nLen = fi.strValue.length();
	fi.emType = adVarChar;
	fi.strName = _T("iDescribe");
	mapParams.push_back(fi);

	fi.strValue = tdLog.strRemark;		//_T(" ");
	fi.nLen = fi.strValue.length();
	fi.emType = adVarChar;
	fi.strName = _T("iRemark");
	mapParams.push_back(fi);

	return m_db.ExecProc(strProcName, mapParams);
}

//////////////////////////////////////////////////////////////////////////
//�������ϱ��ն�״̬
//ʱ�䣺created by yjt 2014-07-03
//�洢�������£�
//�ն�״̬
// P_INSERT_TERMINAL_STATE (
// 	iDeviceNumber IN VARCHAR2, --�豸��
// 	iReportTime IN date, --����ʱ��
// 	iDeviceState IN VARCHAR2, --�豸״̬
// 	iRemark IN VARCHAR2 --��ע
// 	)
//////////////////////////////////////////////////////////////////////////
BOOL CDataBase::InsertStatus(const TERMINAL_STATE& ts)
{
	if(!IsValid())
	{
		return FALSE;
	}

	wstring strProcName(_T("P_INSERT_TERMINAL_STATE"));
	
	//����
	VecParams mapParams;
	FieldInfo fi;
	fi.strValue = ts.strDeviceNumber;	//_T("df");
	fi.nLen = fi.strValue.length();
	fi.emType = adVarChar;
	fi.pType = adParamInput;
	fi.strName = _T("iDeviceNumber");
	mapParams.push_back(fi);

	fi.strValue = ts.strReportTime;	//_T("2014-07-03 12:11:12");
	fi.nLen = fi.strValue.length();
	fi.emType = adVarWChar;
	fi.pType = adParamInput;
	fi.strName = _T("iReportTime");
	mapParams.push_back(fi);

	fi.strValue = ts.strDeviceState;	//_T("����������");
	fi.nLen = fi.strValue.length();
	fi.emType = adVarChar;
	fi.pType = adParamInput;
	fi.strName = _T("iDeviceState");
	mapParams.push_back(fi);

	fi.strValue = ts.strRemark;			//_T(" ");
	fi.nLen = fi.strValue.length();
	fi.emType = adVarChar;
	fi.pType = adParamInput;
	fi.strName = _T("iRemark");
	mapParams.push_back(fi);

	return m_db.ExecProc(strProcName, mapParams);
}

////�����豸�Ż�ȡip�Ͷ˿�
//select t.ftpip, t.ftpport, t.iisip, t.iisport from TERMINALIPCONF t where t.terminalsn = ?
BOOL CDataBase::QueryIpPortByDevId(const wstring& devId, wstring& ftpip, wstring& ftpport, wstring& iisIp, wstring& iisPort)
{
	if( !IsValid() )
		return FALSE;

	CString strSql;
	strSql.Format(_T("select t.ftpip, t.ftpport, t.iisip, t.iisport from TERMINALIPCONF t where t.terminalsn = '%s'"), devId.c_str());
	_RecordsetPtr record = m_db.Query(strSql.GetString());
	if(record == NULL)
	{
		return FALSE;
	}

	try
	{
		if( !record->BOF )
			record->MoveFirst();

		_variant_t var;
		while( !record->adoEOF )
		{
			var = record->GetCollect("FTPIP");
			if( var.vt != VT_NULL )
			{
				ftpip = (wchar_t*)_bstr_t(var);
			}			
			var = record->GetCollect("FTPPORT");
			if( var.vt != VT_NULL )
			{
				ftpport = (wchar_t*)_bstr_t(var);
			}
			var = record->GetCollect("IISIP");
			if( var.vt != VT_NULL )
			{
				iisIp = (wchar_t*)_bstr_t(var);
			}
			var = record->GetCollect("IISPORT");
			if( var.vt != VT_NULL )
			{
				iisPort = (wchar_t*)_bstr_t(var);
			}
			record->MoveNext();
		}
	}
	catch(_com_error *e)
	{
		//SHAREDATA.g_pMainFrame->Alert(e->ErrorMessage());
		return FALSE;
	}

	return TRUE;
}