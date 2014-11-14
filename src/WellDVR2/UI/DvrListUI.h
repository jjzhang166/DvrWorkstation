#pragma once
#include "../DvrWnd.h"
#include <map>
using namespace std;

using namespace DuiLib;

//ÿһ��DVR��Ŀؼ�ָ��
typedef struct tagDvrItemCtrl{
	//��������Ԫ����
	CVerticalLayoutUI* pImg;
	CVerticalLayoutUI* pLabel;
	CVerticalLayoutUI* pValue;

	//ͼƬ�еı�ǩ�����豸��
	CLabelUI* pDev;

	//ID
	CLabelUI* pId;
	//Name
	CLabelUI* pName;
	//�̷�
	CLabelUI* pDisk;
	//������
	CProgressUI* pProgress;
	CLabelUI* pSpeed;
	//״̬
	CLabelUI* pStatus;
	CLabelUI* pFile;
	//ֹͣ����İ�ť
	CButtonUI* pStop;
}DvrItemCtrl, *PDvrItemCtrl;

//<���������� ÿ�����ָ��>
typedef map<int, DvrItemCtrl> MapDvrCtrl;
typedef MapDvrCtrl::iterator MapDvrCtrlItor;

class DvrListUI : public CTileLayoutUI
{
public:
	enum { SCROLL_TIMERID = 10 };

	DvrListUI();

	void DoEvent(TEventUI& event);

	void SetPos(RECT rc);

	void BKColor(int row, int column, DWORD boderColor, DWORD bkcolor);

	//����Dvr��Ϣ����ʾ����
	void SetInfo(int row, int col, CDvrWnd* dvr);
	void ClearInfo(int row, int col);	
	void ClearAllInfo();

	//��ȡѡ�е�λ��
	void GetSelectPos(int& row, int& col);
	void ClearSelectPos();

	void OnBtnClick(TNotifyUI& msg);
private:
	//����DVR��λ�ã������ͻ���)
	void SetDvrPos(RECT rc);

	void OnClick(const POINT& pt);

	CControlUI* HitPtInItem(const POINT& pt);

	void HitItem(CControlUI* pItem);

	//�������л�ȡ��Ӧ��
	CContainerUI* GetItem(int row, int col);

	//��ȡָ�����еĿؼ�ָ�����
	DvrItemCtrl GetDvrItemCtrl(int row, int col);

	DvrItemCtrl InitDvrItemCtrl(int row, int col);

	void SetInfo_Active(const DvrItemCtrl& dic, CDvrWnd* pDvr);
	void SetInfo_Nomal(const DvrItemCtrl& dic, CDvrWnd* pDvr);
	void SetInfo_Disable(const DvrItemCtrl& dic, CDvrWnd* pDvr);
	void SetInfo_Direct(const DvrItemCtrl& dic, CDvrWnd* pDvr);
	void SetInfo_Copy(const DvrItemCtrl& dic, CDvrWnd* pDvr);
	void SetInfo_Copy_OK(const DvrItemCtrl& dic, CDvrWnd* pDvr);
	void SetInfo_Stop(const DvrItemCtrl& dic, CDvrWnd* pDvr);
private:
	//ѡ�������,��0��ʼ
	int m_selectRow;
	int m_selectCol;

	MapDvrCtrl m_mapItem;
};