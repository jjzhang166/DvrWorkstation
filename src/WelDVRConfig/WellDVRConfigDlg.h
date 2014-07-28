
// WelDVRConfigDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "FtpObj.h"
#include "IniFile.h"
#include <map>
using namespace std;

typedef map<int, CString> MapLevel;
typedef MapLevel::iterator MapLevelItor;

const CString gc_Config(_T("\\dvr_cfg.ini"));

// CWelDVRConfigDlg �Ի���
class CWelDVRConfigDlg : public CDialogEx
{
// ����
public:
	CWelDVRConfigDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_WELDVRCONFIG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
private:
	void Init();

	void RefreshList();

	CString GetAppPath();

	CFtpObj LoadFtpObj(const CString& Sec);
	void SaveFtpObj(const CString& sec, const CFtpObj& obj);

public:
	void SetOneDvr(){ m_bOne = true; }

private:
	CIniFile m_IniFile;

	CFtpObj m_ftpOneDvr;			//����
	CFtpObj m_ftpFirst;				//һ��
	CFtpObj m_ftpSecond;			//����
		
	//�ϴ����ȼ�
	MapLevel m_mapLevel;

	//�Ƿ�Ϊ������
	bool m_bOne;

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
	afx_msg void OnBnClickedButton1();
	CEdit m_edtPath;

	//���ݿ�
	CEdit m_edtDBIp;
	CEdit m_edtDBPort;
	CEdit m_edtDBName;
	CEdit m_edtDBUser;
	CEdit m_edtDBPwd;
	CEdit m_edtDBTime;

	//������ftp
	CEdit m_edtFTPIp;
	CEdit m_edtFTPPort;
	CEdit m_edtFTPUser;
	CEdit m_edtFTPPwd;
	afx_msg void OnBnClickedOk();
	
	//һ��FTP
	CEdit m_edtFtpIp1;
	CEdit m_edtFtpPort1;
	CEdit m_edtFtpUser1;
	CEdit m_edtFtpPwd1;
	CButton m_ckTransIMP1;
	CButton m_ckTransHD1;
	CButton m_ckTransSD1;

	//����FTP
	CEdit m_edtFtpIp2;
	CEdit m_edtFtpPort2;
	CEdit m_edtFtpUser2;
	CEdit m_edtFtpPwd2;
	CButton m_ckTransIMP2;
	CButton m_ckTransHD2;
	CButton m_ckTransSD2;

	//�ϴ�����
	CComboBox m_cbxHDDef;
	CEdit m_edtVideoIMP;
	CButton m_ckEnableFtp1;
	CButton m_ckEnableFtp2;
	CButton m_ckEnableFtp;
	afx_msg void OnBnClickedCheck1();

	void EnableFtp1( bool bEnable );

	afx_msg void OnBnClickedCheck2();

	void EnableFtp2( bool bEnable );

	afx_msg void OnBnClickedCheck3();

	void EnableFtp( bool bEnable );

	CButton m_ckTransIMP;
	CButton m_ckTransHD;
	CButton m_ckTransSD;
	CButton m_ckDB;
	afx_msg void OnBnClickedCheck14();

	void EnableDB( bool bEnable );

	CListBox m_listTransLevel;
	CButton m_btnUp;
	CButton m_btnDown;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnLbnSelchangeList1();
	CEdit m_edtDeviceId;
	afx_msg void OnBnClickedButton4();
	CEdit m_edtWebIp;
	CEdit m_edtWebPort;
};
