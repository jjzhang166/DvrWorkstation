//////////////////////////////////////////////////////////////////////////
//Describe: ���ݿ����������
//			1. ��װADO�����ݿ�Ĳ���
//			ע��
//				a. �ֶ�Ϊ�յ�" "(ע���ֵ����Ϊ��)���������֡�������һ�»�����"
//				b. ������˳�����һ�£���������map���Զ�����)
//Date: created by yjt 2014-07-03
//History:
//////////////////////////////////////////////////////////////////////////
#pragma once

//�� msado15.dll ��Ϊ  msado60.tlb
//1. ���������ݿⴴ��connectʱ ���� E_NOINTERFACE������

//2. ע�⣺
//		��װ·��������Ӣ�ĵ����ţ������oracle�г�ͻ�������������ݿ�
// #import "c:\program files\common files\system\ado\msado60.tlb" \
// 	no_namespace \
// 	rename ("EOF", "adoEOF")

#ifdef _WIN64
#define TLB_FILE_NAME "..\..\..\bin\msado60_x64.tlb"
#else
#define TLB_FILE_NAME "..\..\..\bin\msado60_i386.tlb"
#endif
#import TLB_FILE_NAME no_namespace rename("EOF", "adoEOF")

#include <string>
#include <map>
#include <vector>
using namespace std;

typedef struct tagFieldInfo{
	wstring strName;
	DataTypeEnum emType;
	ParameterDirectionEnum pType;
	int nLen;
	wstring strValue;
}FieldInfo, * PFieldInfo;

//�洢���̲���
typedef vector<FieldInfo> VecParams;
typedef VecParams::const_iterator VecParamsItor;

class CDBHelper
{
public:
	CDBHelper(void);
	~CDBHelper(void);

	//��ʼ��
	BOOL Init(const wstring& strConnString);
	void UnInit();

	//�Ƿ�����
	BOOL IsConnect();
	
	//��ȡ������Ϣ
	wstring GetLastError() const { return m_strErrMsg; }

	//////////////////////////////////////////////////////////////////////////
	//��ѯsql����
	_RecordsetPtr Query(const wstring& strSql);

	//ִ�д洢���� -- �����û�н������
	bool ExecProc(const wstring& procName, const VecParams& params);

	//ִ�д洢���� -- ������н������
	_RecordsetPtr ExecProcEx(const wstring& procName, const VecParams& params);

	//////////////////////////////////////////////////////////////////////////
private:
	_ConnectionPtr	m_pConnection;
	_RecordsetPtr m_pUserRecSet;
	_CommandPtr m_pCmd;

	wstring m_strConn;			//���Ӵ�

	BOOL m_bConnectDB;			//�Ƿ�����

	wstring m_strErrMsg;		//������Ϣ
};

