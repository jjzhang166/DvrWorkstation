
// AviTestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AviTest.h"
#include "AviTestDlg.h"
#include "afxdialogex.h"
#include "AviHelper.h"

#pragma comment(lib, "WellCommon.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CAviTestDlg �Ի���




CAviTestDlg::CAviTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAviTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAviTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edtAviFile);
	DDX_Control(pDX, IDC_EDIT2, m_edtBmpDir);
}

BEGIN_MESSAGE_MAP(CAviTestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CAviTestDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CAviTestDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON4, &CAviTestDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON3, &CAviTestDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CAviTestDlg ��Ϣ�������

BOOL CAviTestDlg::OnInitDialog()
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

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CAviTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CAviTestDlg::OnPaint()
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
HCURSOR CAviTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//ѡ��Avi�ļ�
void CAviTestDlg::OnBnClickedButton1()
{
	TCHAR szFilters[]= _T("AVI Files (*.avi)|*.avi|All Files (*.*)|*.*||");
	CFileDialog fileDlg(TRUE, _T("avi"), _T("*.avi"),
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters);
	if(fileDlg.DoModal() == IDOK)
	{
		CString pathName = fileDlg.GetPathName();
		m_edtAviFile.SetWindowText(pathName);
	}
}

//bmpĿ¼
void CAviTestDlg::OnBnClickedButton2()
{
	// ��ȡ�ض��ļ��е�LPITEMIDLIST�����Խ�֮���ΪHANDLE
	// ��ν���ض��ļ���,�������CSIDL_XXX������֮��
	LPITEMIDLIST rootLoation;
	SHGetSpecialFolderLocation( NULL, CSIDL_DESKTOP, &rootLoation );
	if ( rootLoation == NULL ) {
		// unkown error
		return;
	}

	// ���öԻ���
	BROWSEINFO bi;
	ZeroMemory( &bi, sizeof( bi ) );
	bi.pidlRoot = rootLoation;			// �ļ��жԻ���֮��Ŀ¼����ָ���Ļ���Ϊ�ҵĵ���
	bi.lpszTitle = _T( "BMP�洢Ŀ¼" ); // ���Բ�ָ��
	// bi.ulFlags = BIF_EDITBOX | BIF_RETURNONLYFSDIRS;

	// �򿪶Ի���, �е���DoModal
	LPITEMIDLIST targetLocation = SHBrowseForFolder( &bi );
	if ( targetLocation != NULL ) {
		TCHAR targetPath[ MAX_PATH ];
		SHGetPathFromIDList( targetLocation, targetPath );
		// MessageBox( targetPath );
		m_edtBmpDir.SetWindowText(targetPath);
	}

}


//��ȡ
void CAviTestDlg::OnBnClickedButton4()
{

}

//ת��
void CAviTestDlg::OnBnClickedButton3()
{
	CString aviFile;
	m_edtAviFile.GetWindowText(aviFile);
	CString bmpDir;
	m_edtBmpDir.GetWindowText(bmpDir);

	CAviHelper::AVItoBmp(aviFile, bmpDir);
}