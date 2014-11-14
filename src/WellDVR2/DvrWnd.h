/************************************************************************/
/* ������DVR�豸��
   ʱ�䣺created by yjt 2014-4-12
   ��ʷ��
*/
/************************************************************************/
#pragma once

#include "DataDef.h"
#include <map>
#include <afxmt.h>
using namespace std;

UINT CopyProc(LPVOID param);

class CDevBase;
class CDvrWnd;
typedef map<int, CDvrWnd*> MapDvrTimer;
typedef MapDvrTimer::iterator MapDvrTimerItor;

class CDvrWnd
{
	friend UINT CopyProc(LPVOID param);
public:
	CDvrWnd(void);
	CDvrWnd(bool empty);
	~CDvrWnd(void);

	CDvrWnd& operator=(const CDvrWnd& dvr);

	void SetStatus(DVRSTATUS ds) { m_status = ds; }
	DVRSTATUS GetStatus() const { return m_status; }

	void SetPos(int x, int y) { m_x = x; m_y = y; }
	int GetRow() const { return m_x; }
	int GetColumn() const { return m_y; }

	void SetId(const CString& strId) { m_strId = strId; }
	CString GetId() const { return m_strId; }
	
	void SetName(const CString& strName) { m_strName = strName; }
	CString GetName() const { return m_strName; }

	void SetFile(const CString& strFile) { m_strFile = strFile; }
	CString GetFile() const { return m_strFile; }

	void SetDisk(const CString& strDisk) { m_strDisk = strDisk; }
	CString GetDisk() const { return m_strDisk; }

	int GetProgressMax() const { return m_proRange; }
	int GetProgressValue() const { return m_proValue; }

	void SetStatusStr(const CString& strInfo) { m_strStatus = strInfo; }
	CString& GetStatusStr() { return m_strStatus; }

	CString& GetSpeedStr() { return m_strSpeed; }

	void SetCopyStatus(int nStatus);

	//���������Ϣ
	void ClearInfo();

	//����Ŀ¼
	void CopyDir(const CString& strDrive);

	//���úͻ�ȡ ָ���豸�Ķ���
	CDevBase* GetDevPtr() const { return m_pDev; }
	void SetDevPtr(CDevBase* pDev);
	void RemoveDevPtr();

	void SyncTime();
	//ö���豸
	void MassDev();
	BOOL IsCopying() { return m_bIsCopying; }
	void SetCopying(bool isCopying) { m_bIsCopying = isCopying; }

protected:

	bool IsLogFile(const CString& strFileName);
	//������־�ļ���׷�ӿ���)
	bool CopyLogFile(const TCHAR* strFileSrc, const TCHAR* strDst);

private:
	void Init(bool empty);
	void UnInit();

	//CopyFileEx�Ļص��������ڴ˺�����ʵ���ļ����ƹ��̵Ŀ��ơ�
	static DWORD CALLBACK CopyProgressRoutine(
		LARGE_INTEGER TotalFileSize,
		LARGE_INTEGER TotalBytesTransferred,
		LARGE_INTEGER StreamSize,
		LARGE_INTEGER StreamBytesTransferred,
		DWORD dwStreamNumber,
		DWORD dwCallbackReason,
		HANDLE hSourceFile,
		HANDLE hDestinationFile,
		LPVOID lpData
		);

private:
	UINT CopyDirProc();
	void CopyDir(const TCHAR *src, const TCHAR *dst, LPBOOL bCancelCopy);

	void RecursiveFindFile(const CString& strPath);

	BOOL CreateDeepDirectory(const CString& strPath,        //directory name
		LPSECURITY_ATTRIBUTES lpSecurityAttributes  // SD
		);

	void InitSpeed();
	void UnInitSpeed();
	void ClearSpeed();

	void ResetTransSize() { 
		m_nTotalTanstime = 0;
		m_llTransSize = 0;
	}

	void SpaceLessAlarm();

	//��ʱ���ص��� ������ͷ�ļ��ж���(ȫ�ֺ���Ҳ�ǣ�ͬʱҪ��static)
	static void CALLBACK CopyTimerProc(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime);

private:
	CString m_strId;			//ID
	CString m_strName;			//NAME
	CString m_strDisk;			//�̷�
	CString m_intPocess;		//����
	DVRSTATUS m_status;			//״̬
	CString m_strFile;

	////////////////////////////////////////////////////
	//�����ٶ�
	CString m_strSpeed;			//�����ٶ�

	//�����ٶȶ�ʱ��
	UINT_PTR m_cpTimerId;
	int m_nTotalTanstime;		//�ܴ���ʱ��(��)
	LONGLONG m_llTransSize;		//����Ĵ�С(ÿ���ļ�)
	static MapDvrTimer m_mapTimer;
	////////////////////////////////////////////////////


	//��Ӧ���ڵ�λ��
	int m_x;
	int m_y;	

	int m_proRange;				//�������ķ�Χ
	int m_proValue;				//��������ǰֵ
	
	//״̬��ʾ�ַ���
	CString m_strStatus;

private:

	HANDLE m_hCopyEvent;
	HANDLE m_hCopyThread;

	BOOL m_bExit;
	volatile int m_CopyStatus;
	//�˱�����m_CopyStatus���Ӧ
	//m_bStopCopy = trueʱ����ֹ����  
	BOOL m_bStopCopy;			

	CString m_strDstPath;
	CString m_strSrcPath;
	volatile int m_filecnt;

	//��Ӧ��DVR����
	CDevBase* m_pDev;
	CCriticalSection m_cs;
	BOOL m_bIsCopying;		//�Ƿ����ڿ���

	//��ȡ�ļ��Ĵ���, U�̶���֮�󣬸տ�ʼ���ܶ��������ļ���һ��׼����֮����Զ����ļ�
	short m_nReadFileCount;		
};