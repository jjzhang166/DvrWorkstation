
// GetMachineCodeDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// CGetMachineCodeDlg �Ի���
class CGetMachineCodeDlg : public CDialogEx
{
// ����
public:
	CGetMachineCodeDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_GETMACHINECODE_DIALOG };

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
	CEdit m_edtCode;
	afx_msg void OnBnClickedOk();
};
