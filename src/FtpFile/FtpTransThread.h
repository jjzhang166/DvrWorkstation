#pragma once
#include "threadbase.h"
#include <afxmt.h>
#include <afxinet.h>

class CFtpHelper;

//Ftp���ļ�������
class CFtpTransThread :
	public CThreadBase
{
public:
	CFtpTransThread(void);
public:
	~CFtpTransThread(void);
public:
	//���͵���Ϣ
	LONGLONG m_nFileSize; //�ļ���С
	LONGLONG m_nFileTransSize;  //�ļ��Ѿ����͵Ĵ�С
	CString m_strSourceFile; //Դ�ļ�
	CString m_strDestFile;   //Ŀ���ļ�
	CString m_ftpIp,m_ftpUser,m_ftpPwd,m_ftpPath; //ftp��Ϣ
	CString m_strNewDir,m_strDate,m_strSharePath; //sharepath: ftpĿ¼�Ĺ���·��
	UINT m_ftpPort;    //ftp�˿�
	bool m_bFailed; //�Ƿ���ʧ��
	CCriticalSection m_cs;  //CriticalSection,����ͬ��faled��transSize
	virtual UINT DoWork();
	CEvent *m_pCompleteEvent;
	CFtpHelper *m_ctrl;   //�ؼ�ָ��
	bool m_bTransOver ; //ָʾ�����Ƿ����
	bool IsTransOver(void);
	void SetTransOverFlag(bool value);
public:
	bool FtpTransProc(void);
	
public:
	void SetFtpInfo(CString ip, UINT port,CString user, CString pwd,CString path);
public:
	void TransFile(CString srcFile, CString destFile,CString newDir,CString date,CString sharepath,CEvent *pCompleteEvent);
	void TransFileContiue(CString srcFile, CString remoteFile,CEvent *pCompleteEvent,BOOL isContiue);
public:
	bool IsFailed(void);
protected:
	char *buffer;
	int nBufSize;
	 LONGLONG GetFtpFileSize(CFtpConnection* pFtpCon, CString strFtpFile);

	 //�����༶Ŀ¼
	 BOOL CreateFTPDirectory(CFtpConnection* pFtpCon, const CString& strPath);
};
