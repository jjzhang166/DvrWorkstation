/************************************************************************/
/* 
* Describe: FTP������
*	1. ���ftp������Ϣ
*		1.1 �������������
*		1.2 �����
*			1.2.1 �����˰���������߼���
*	2. �ȿ���������
*	3. ��̨�����ϴ���ftp��������
*		3.1 �ϵ�����
* Date: created by yjt 2014-4-17
* History:
*		add by yjt 2014-6-12 ��Ӷ�ʱ������, ͬʱ��ftp�����ɵ�����
*		add by yjt 2014-6-13 ����ϴ��ص�������Ҫ�ϴ����ļ��ص��������ÿͻ��˴���Ҫ��Ҫ�ϴ�
*/
/************************************************************************/
#pragma once
#include <afxinet.h>
#include <afxmt.h>
#include <map>
using namespace std;

#define TRANS_CONTUNE 0		//������
#define TRANS_SKIP 1		//����
#define TRANS_DELETE 2		//�������������ɾ��

enum EM_MSG_TYPE
{
	TRANS_BEGIN = 0,
	TRANS_END
};

//////////////////////////////////////////////////////////////////////////
//���ܣ��ϴ��ļ��ص�����
//������
//		strCurTransFile [IN]	--		ָʾ��ǰҪ�ϴ����ļ�
//����:
//		0: �ϴ� 
//		1: ���ϴ�,����
//////////////////////////////////////////////////////////////////////////
typedef int (__stdcall *TransFileCallBack)(const string& strCurTransFile);

//////////////////////////////////////////////////////////////////////////
//����: ������Ϣ�ص�
//		1. ��ʼ����ͽ�������ʱ���ص�һ��
//���أ�
//		0: ����
//		1: ����		--  ���Կ����Ƿ�ɾ���ļ�
//////////////////////////////////////////////////////////////////////////
typedef int (__stdcall *TransMsgCB)(EM_MSG_TYPE type);


typedef struct tagRegCB
{
	TransFileCallBack tfCB;
	TransMsgCB	ttNotify;
}RegCB, *PRegCB;
//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
class __declspec(dllexport) CFtpHelper;

typedef map<UINT_PTR, CFtpHelper*> MapTimer;
typedef MapTimer::iterator MapTimerItor;



UINT PutFileProc(LPVOID pFtp);
DWORD WINAPI FtpThrProc(LPVOID param);

class __declspec(dllexport) CFtpHelper
{
	friend UINT PutFileProc(LPVOID pFtp);
	friend DWORD WINAPI FtpThrProc(LPVOID param);
	
	enum EM_FTP_PUT_STATUS{
		EM_PUT_OK,
		EM_PUT_FAIL,
		EM_PUT_UNKNOW
	};

public:
	CFtpHelper(void);
	~CFtpHelper(void);

	//////////////////////////////////////////////////////////////////////////
	//���ܣ�����ftp�ͻ���--����ftp������
	//������
	//	addr [IN]		--	������IP��ַ
	//	strUser [IN]	--	�û���
	//	strPwd [IN]		--	����
	//	nPort [IN]		--	�˿�
	//	strDir	[IN]	--	����Ŀ¼���Ӵ�Ŀ¼�Զ��ϴ���ftp������)
	//����ֵ��
	//	true : �ɹ������Ϸ�����
	//////////////////////////////////////////////////////////////////////////
	bool Start(const string& addr, const string& strUser, const string& strPwd, UINT nPort, const string& strDir);
	bool Start(const wstring& addr, const wstring& strUser, const wstring& strPwd, UINT nPort, const wstring& strDir);
	
	//////////////////////////////////////////////////////////////////////////
	//���ܣ�ע��ص�
	//		1. ��tfFunc����Ϊ��ʱ�������лص���ֱ���ϴ�
	//////////////////////////////////////////////////////////////////////////
	void Register(const RegCB& callBacks);

	//////////////////////////////////////////////////////////////////////////
	//���ܣ���̨�����Զ��ϴ�������)
	//////////////////////////////////////////////////////////////////////////
	void ServiceRun();

	//////////////////////////////////////////////////////////////////////////
	//���ܣ���ȡ���Ͱٷֱ�
	//////////////////////////////////////////////////////////////////////////
	float GetTransPrecent();

	//////////////////////////////////////////////////////////////////////////
	//����: ��ȡ�����ٶ�
	//////////////////////////////////////////////////////////////////////////
	void GetTransSpeed(wstring& strSpd);
	void GetTransSpeed(string& strSpd);
	
	//////////////////////////////////////////////////////////////////////////
	//���ܣ���ȡ���ڴ��͵��ļ���
	//////////////////////////////////////////////////////////////////////////
	void GetPutFile(wstring& strPF);
	void GetPutFile(string& strPF);

protected:
	//����Ŀ¼
	bool CreateDirctory(const wstring& strPath);
	bool CreateDirctory(const string& strPath);

	//�ϴ��ļ�
	UINT PutFile(const wstring& strFile, const wstring& saveFile);
	UINT PutFile(const string& strFile, const string& saveFile);

	//�ϴ��ļ���
	bool TransFolder(const wstring& strFoldername, const wstring& strDst);
	bool TransFolder(const string& strFoldername, const string& strDst);

	//ֹͣ����
	void StopTrans() { m_bForceStop = true; }
private:
	//����ftp�ͻ���
	bool _Start( const wstring& addr, const wstring& strUser, const wstring& strPwd, UINT nPort);
	
	//����Ŀ¼
	bool _CreateDirctory(const wstring& strPath);

	//�ϴ��ļ�
	UINT _PutFile(const wstring& strFile, const wstring& saveFile);

	//�ϴ��ļ���
	bool _TransFolder(const wstring& strFoldername, const wstring& strDst);

protected:
	UINT PutFile();

	//�жϷ��������ļ��Ƿ����
	LONGLONG IsFileExit(const wstring& strFile);
	int DelEmptyFolder(const wstring& strFolder);

	bool Init();
	void Unit();

	void Service();

	bool Start();
	void Stop();

	bool ReStart();
	void ReSetTrans();
	//��ʱ���ص��� ������ͷ�ļ��ж���(ȫ�ֺ���Ҳ�ǣ�ͬʱҪ��static)
	static void CALLBACK PutTimerProc(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime);
	static MapTimer m_mapTimerObjs;
private:

	CInternetSession* m_pInetSession;
	CFtpConnection*   m_pFtpConnection;

	wstring m_strServAddr;		//ftp�������ĵ�ַ  LPCTSTR
	wstring m_strUser;			//��½�û���       LPCTSTR 
	wstring m_strPwd;			//����            LPCTSTR
	UINT m_nPort;				//�˿�            UINT

	HANDLE m_hEvent;
	HANDLE m_hThread;

	//FTP����Ŀ¼
	wstring m_strLocalDir;

	wstring m_wstrPutFile;
	string m_strPutFile;
	wstring m_strSaveFile;
	bool m_bExit;

	int m_nPutStatus;				//�ϴ����


	/////////////////////////////////////////////////////////////
	//��̨����
	CCriticalSection m_cs;
	//�����Ĵ�С
	char *m_pBuffer;
	int m_nBufSize;
	////////////////////////////////////////////////////////////

	//�Ƿ�ֹͣ����
	bool m_bForceStop;

	////////////////////////////////////////////////////

	//�����ٶ�
	wstring m_wstrSpeed;			//�����ٶ�
	string m_strSpeed;

	//�����ٶȶ�ʱ��
	UINT_PTR m_putTimerId;
	int m_nTotalTanstime;		//�ܴ���ʱ��(��)
	LONGLONG m_llTransSize;		//����Ĵ�С(ÿ���ļ�)
	LONGLONG m_llFileSize;		//ÿ���ļ����ܴ�С
	////////////////////////////////////////////////////

	//�ϴ��ص�����
	RegCB m_regCB;
};

