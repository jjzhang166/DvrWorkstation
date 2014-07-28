
// DvrToolDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DvrTool.h"
#include "DvrToolDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma comment(lib, "../../lib/Dvr.lib")

#include "../Dvr/WellDvr34.h"
#include "../Dvr/HADvr.h"


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDvrToolDlg �Ի���




CDvrToolDlg::CDvrToolDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDvrToolDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_nType = DVR_WELL;
	m_pDvr = NULL;
}

void CDvrToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edtId);
	DDX_Control(pDX, IDC_EDIT2, m_edtPID);
	DDX_Control(pDX, IDC_EDIT3, m_edtUnit);
	DDX_Control(pDX, IDC_EDIT4, m_edtTime);
	DDX_Control(pDX, IDC_BUTTON1, m_btnTypeWell);
}

BEGIN_MESSAGE_MAP(CDvrToolDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CDvrToolDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDvrToolDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CDvrToolDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CDvrToolDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CDvrToolDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CDvrToolDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CDvrToolDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CDvrToolDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, &CDvrToolDlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_RADIO1, &CDvrToolDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CDvrToolDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &CDvrToolDlg::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO4, &CDvrToolDlg::OnBnClickedRadio4)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CDvrToolDlg ��Ϣ�������

BOOL CDvrToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_btnTypeWell.SetCheck(TRUE);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CDvrToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CDvrToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CDvrToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//��ȡ�豸iD
void CDvrToolDlg::OnBnClickedButton1()
{
	CDevBase* pDvr = GetDevObj();
	if(pDvr == NULL)
		return;
	CString strId;
	if( pDvr->GetIDEx(strId) )
	{
		m_edtId.SetWindowText(strId);
	}
}

//��ID
void CDvrToolDlg::OnBnClickedButton2()
{
	CString strId;
	m_edtId.GetWindowText(strId);
	if(strId.IsEmpty())
	{
		m_edtId.SetFocus();
		return;
	}

	CDevBase* pDvr = GetDevObj();
	if(pDvr == NULL)
		return;
	pDvr->SetIDEx(strId);
}

//��ȡ��ԱID
void CDvrToolDlg::OnBnClickedButton3()
{
	CDevBase* pDvr = GetDevObj();
	if(pDvr == NULL)
		return;

	CString strT;
	pDvr->GetMachinID(strT);
	m_edtPID.SetWindowText(strT);
}


void CDvrToolDlg::OnBnClickedButton4()
{
	CDevBase* pDvr = GetDevObj();
	if(pDvr == NULL)
		return;
	CString strT;
	m_edtPID.GetWindowText(strT);
	pDvr->SetMachinID(strT);
}

//��ȡ��λ
void CDvrToolDlg::OnBnClickedButton5()
{
	
}


void CDvrToolDlg::OnBnClickedButton6()
{
	
}

//��ȡʱ��
void CDvrToolDlg::OnBnClickedButton7()
{
	CDevBase* pDvr = GetDevObj();
	if(pDvr == NULL)
		return;
	SYSTEMTIME t;
	pDvr->GetTimeEx(&t);
	CString strT;
	strT.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"), t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond);

	m_edtTime.SetWindowText(strT);
}


void CDvrToolDlg::OnBnClickedButton8()
{
	
}
#include "../WellCommon/XmlHelper.h"
#pragma comment(lib, "WellCommon.lib")

void TestXml()
{
	CXmlHelper xml;
	xml.Load("D:\\test.xml", true);

	xml.SetNodeValue("root\\ftp\\serv", "192.168.1.1.");
	xml.SetNodeAttr("root\\ftp\\serv", "port", "21");

	string str = xml.GetNodeValue("root\\ftp\\serv");
	str = xml.GetNodeAttr("root\\ftp\\serv", "port");

	xml.Save("D:\\test.xml");
}

//U��
void CDvrToolDlg::OnBnClickedButton9()
{
	CDevBase* pDvr = GetDevObj();
	if(pDvr == NULL)
		return;

	pDvr->SetMassEx();
	//TestXml();
}


void CDvrToolDlg::OnBnClickedRadio1()
{
	m_nType = DVR_WELL;
}


void CDvrToolDlg::OnBnClickedRadio2()
{
	m_nType = DVR_A4;
}


void CDvrToolDlg::OnBnClickedRadio3()
{
	m_nType = DVR_X6;
}


void CDvrToolDlg::OnBnClickedRadio4()
{
	m_nType = DVR_D6;
}


CDevBase* CDvrToolDlg::GetDevObj()
{
	if(m_pDvr)
	{
		delete m_pDvr;
		m_pDvr == NULL;
	}

	if(m_nType == DVR_WELL)
	{
		m_pDvr = new CWellDvr34;
	}
	else if(m_nType == DVR_A4)
	{
		m_pDvr = new CA4Dvr;
	}
	else if(m_nType == DVR_X6)
	{
		m_pDvr = new CHADvr;
	}
	else if(m_nType == DVR_D6)
	{
		m_pDvr = new CHADvr;
	}
	return m_pDvr;
}

void CDvrToolDlg::OnClose()
{
	
	if(m_pDvr)
	{
		delete m_pDvr;
		m_pDvr = NULL;
	}

	CDialogEx::OnClose();
}
