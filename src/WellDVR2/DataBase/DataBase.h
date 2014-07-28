/************************************************************************/
/* ���ݿ������                                                                     */
/************************************************************************/
#pragma once

#include <vector>
using namespace std;

#include "DBHelper.h"

#include "../DataDef.h"
#include <map>
using namespace std;

typedef map<CString, CString> MapDvrIdName;
typedef MapDvrIdName::iterator MapDvrIdNameItor;

#define DB CDataBase::Instance()

class CDataBase
{
private:
	CDataBase(void);
	~CDataBase(void);

public:
	static CDataBase& Instance();

	//ͨ����ԱID��ȡ����
	CString GetNameFormID(const CString& id);
	//ͨ��idȥ������(ʵʱ����)
	CString QueryNameFormID(const CString& id);

	//����ý������
	BOOL InsertMediaInfo(const DVRMEDIA_INFO& info);

	BOOL InsertDvrLog(const DVR_DEVICE_LOG& ddlog);

	BOOL InsertLog(const TERMINAL_DEV_LOG& telog);

	BOOL InsertStatus(const TERMINAL_STATE& ts);

	BOOL QueryIpPortByDevId(const wstring& devId,  wstring& ftpip, wstring& ftpport, wstring& iisIp, wstring& iisPort);

private:
	BOOL Init();
	void UnInit();
	
	//�û��б�
	void InitUserRec();
	void FillUserVector(_RecordsetPtr record);



private:
	CDBHelper m_db;

	MapDvrIdName mapDvr;
};

