
// GetRegisterCodeDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// CGetRegisterCodeDlg �Ի���
class CGetRegisterCodeDlg : public CDialogEx
{
// ����
public:
	CGetRegisterCodeDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_GETREGISTERCODE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_edtMC;
	CEdit m_edtRC;
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangeEdit1();
};
