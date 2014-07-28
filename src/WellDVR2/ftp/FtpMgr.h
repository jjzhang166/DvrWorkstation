//////////////////////////////////////////////////////////////////////////
//Describe:	FTP������
//			1. ͨ��ע��ص��ķ�ʽ�������ļ����ϴ�
//				1.1 �ж��ص���Ƶ��������Ƶ��
//Date:		created by yjt 2014-06-13
//History:
//
//////////////////////////////////////////////////////////////////////////
#pragma once
#include "../DataDef.h"
#include "FtpHelper.h"
#include <string>
#include <vector>
#include <queue>
using namespace std;

#define FTP_CLIENT CFtpMgr::Instance()

//////////////////////////////////////////////////////////////////////////
//�ϴ��ļ��ص�����
//	�����ڴ˹����ص���Ƶ���ֱ��ʵ�
int __stdcall FtpOneTransFileCB(const wstring& );
int __stdcall FtpFirstTransFileCB(const wstring& );
int __stdcall FtpSecondTransFileCB(const wstring& );


int __stdcall FtpTransMsgCB(EM_MSG_TYPE );
int __stdcall Ftp1TransMsgCB(EM_MSG_TYPE );
int __stdcall Ftp2TransMsgCB(EM_MSG_TYPE );

//////////////////////////////////////////////////////////////////////////

//������Ϣ�ص�
typedef void (* TransInfoCB)(const wstring& file, const wstring& info, int nPrecent);


#define FTP_TRANS_UNKONW -1
#define FTP_TRANS_BEGIN 0
#define FTP_TRANS_END 1


typedef queue<FTPInfo*> QueueLevel;

class CFtpMgr
{
	friend int __stdcall FtpOneTransFileCB(const wstring& );
	friend int __stdcall FtpFirstTransFileCB(const wstring& );
	friend int __stdcall FtpSecondTransFileCB(const wstring& );
	friend int __stdcall FtpTransMsgCB(EM_MSG_TYPE );
	friend int __stdcall Ftp1TransMsgCB(EM_MSG_TYPE );
	friend int __stdcall Ftp2TransMsgCB(EM_MSG_TYPE );
private:
	CFtpMgr(void);
	~CFtpMgr(void);

public:
	static CFtpMgr& Instance();

	//����
	bool Run();

	void InitLevelEx();


	//�Ƿ���ftp����
	bool IsFtpValid(){ return m_bFtpValid; }

	
	void Register(TransInfoCB tif){ m_transInfo = tif; }

	//////////////////////////////////////////////////////////////////////////
	//���ܣ� ��ȡ������Ϣ, �ļ�, �ٶȺͰٷֱȣ�����һ�λ�ص�
	//////////////////////////////////////////////////////////////////////////
	void GetTransInfo();

private:
	//��ȡ��ǰ������FTP������
	CFtpHelper& GetCurWorkFtp();

	int FtpOneTransFileFunc(const wstring& );
	int FtpFirstTransFileFunc(const wstring& );
	int FtpSecondTransFileFunc(const wstring& );

	int FtpTransMsgFunc(EM_MSG_TYPE );
	int Ftp1TransMsgFunc(EM_MSG_TYPE );
	int Ftp2TransMsgFunc(EM_MSG_TYPE );

	//�Ƿ�����ϴ�
	int IsContinueTrans(const wstring& strFile, const FTPInfo& ftpIf);

	//�Ƿ����ص���Ƶ
	bool IsIMPVideo(const wstring& strFile);
	//�Ƿ�����Ƶ�ļ�
	bool IsVideoFile(const wstring& strFile);
	//�Ƿ�Ϊ������Ƶ
	bool IsHDVideo(const wstring& strFile);

	//�Ƿ��Ǹ��壨ͨ�����嶨���ʶ���ж�)
	bool IsHD(int width, int height);

	//�������ȼ��ж��Ƿ�Ҫ�ȴ�
	bool IsWait(const wstring& level);
private:
	//����ftp
	CFtpHelper m_ftpOne;
	FTPInfo m_fiOne;
	//һ��FTP
	CFtpHelper m_ftpFirst;
	FTPInfo m_fiFirst;
	//����FTP
	CFtpHelper m_ftpSecond;
	FTPInfo m_fiSecond;

	//�Ƿ���ftp����
	bool m_bFtpValid;

	//��Ҫ��Ƶ��־
	wstring m_strFlagIMP;
	//���嶨��
	int m_nHDDef;


	CCriticalSection m_cs;
	//���ȼ�
	vector<wstring> m_vecLevel;
	QueueLevel m_qLevel;

	TransInfoCB m_transInfo;
};

