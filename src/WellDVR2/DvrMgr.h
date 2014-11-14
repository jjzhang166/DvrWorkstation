/************************************************************************/
/* 
* ������ DVR������
*		1. �Ե����ķ�ʽ�����ṩ�ӿ�
* ʱ�䣺 created by yjt 2014-4-12
* ��ʷ��
*/
/************************************************************************/
#pragma once

#include "DvrWnd.h"
#include <map>
#include <vector>
#include <list>
#include <algorithm>
#include <afxmt.h>
#include "WellCommon/CriLock.h"
#include "DataDef.h"

using namespace std;

class CDevBase;

//<���ڶ���>
typedef vector<CDvrWnd*> VecDvrWnd;
typedef VecDvrWnd::iterator VecDvrWndItor;

//<�������������ڶ���>
typedef map<int, DvrWndInfo*> MapDvrWnd;
typedef MapDvrWnd::iterator MapDvrWndItor;

class CDvrMgrBase{
public:
	CDvrMgrBase();
	~CDvrMgrBase();
	
public:
	CDvrWnd* GetDvr(int wndIndex);
	CDvrWnd* GetDvr(int row, int col);
	CDvrWnd* GetDvr(const wstring& strPort);
		
	virtual int GetWndIndex(const wstring& strPort);

protected:
	void Init();
	void UnInit();

	virtual bool GetRowCol(const wstring& strPort, int& row, int& col);
	virtual void GetRowCol( int nWndIndex, int& row, int& col );

	//�������л�ȡ���ڵ�����
	int GetWndIndexByRowCol(int row, int col);

protected:
	//<Dvr���ڶ���>
	VecDvrWnd m_vecDvrWnd;
	DECLARE_LOCK
};


UINT ClearProc(LPVOID param);

class CDvrMgr : public CDvrMgrBase
{
	friend UINT ClearProc(LPVOID param);
public:
	CDvrMgr(void);
	virtual ~CDvrMgr();

public:
	static CDvrMgr& Instance();

	virtual CDvrWnd* PutDvr(const wstring& strPort);
	virtual void RemoveDvr(const wstring& strPort);

	void CopyDvrFile(const wstring& strPort, const wstring& diskName);

protected:
	//��ȡDVRId
	bool GetDvrId(CString& strId, CDevBase** pDvr);

	void InsertDvrDevice(const CString& strID, const CString& strLog);

	//��������õ�dvr
	void ClearInValidDvr();

protected:
	CCriticalSection m_cs;

	//������ô�����
	vector<int> m_vecClear;

	//////////////////////////////////////////////////////////////////////////
	//���ڼ�¼ÿ�����ڵĶ���״̬
	//��Щ������Ӧ�������ܾò���ö�ٳ�U��
	//��ö�ٳ�U��ʱ������������գ�������
	//��Щ����Ķ���ָ����ڵ�״̬
	//  1. ���벢��ȡIDʱ������
	//	2. ���U��ʱ�����û��ID����ָ�
	//	3. һ���ֺ����
	//////////////////////////////////////////////////////////////////////////
	MapDvrWnd m_mapDvrWnd;

	//�]�ж�λ�ӿ�--Alert��B�惦
	//�ѽ�ʾ�ľͲ�Ҫ�؏���ʾ
	map<wstring, int> m_mapAlertPort;
};

