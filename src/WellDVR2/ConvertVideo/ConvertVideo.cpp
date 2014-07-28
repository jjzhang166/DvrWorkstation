#include "stdafx.h"
#include "ConvertVideo.h"
#include <sstream>


CConvertVideo::CConvertVideo(void)
{
}


CConvertVideo::~CConvertVideo(void)
{

}

bool CConvertVideo::Convert(const wstring& strCmd)
{
	SECURITY_ATTRIBUTES sa;//����һ����ȫ���Եı���
	HANDLE hRead,hWrite; //�ܵ��Ķ�д�������
	sa.nLength = sizeof(SECURITY_ATTRIBUTES); //��ȡ��ȫ���Եĳ���
	sa.lpSecurityDescriptor = NULL;  //ʹ��ϵͳĬ�ϵİ�ȫ������ 
	sa.bInheritHandle = TRUE;  //�����Ľ�������̳о��

	if (!CreatePipe(&hRead,&hWrite,&sa,0))  //���������ܵ�
	{  
		//MessageBox(_T("CreatePipe Failed!"), _T("��ʾ"), MB_OK | MB_ICONWARNING);  
		return false;
	}
	STARTUPINFO si;				//������Ϣ�ṹ�����
	PROCESS_INFORMATION pi;		//��Ҫ����Ľ�����Ϣ�ı���

	ZeroMemory(&si,sizeof(STARTUPINFO)); //��ΪҪ�����������������ոñ���
	si.cb = sizeof(STARTUPINFO); //�ṹ��ĳ���
	GetStartupInfo(&si); 
	si.hStdError = hWrite; 
	si.hStdOutput = hWrite;  //�´������̵ı�׼�������д�ܵ�һ��
	si.wShowWindow = SW_HIDE;  //���ش��� 
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

	TCHAR cmdline[512] = {0};
	CString tmp,stredit2;
	//GetDlgItemText(IDC_EDIT2,stredit2);  //��ȡ�༭���������������
	//tmp.Format(_T("cmd /C %s"),stredit2);
	tmp.Format(_T("cmd /C %s"), strCmd.c_str());
	swprintf(cmdline,_T("%s"),tmp);

	if (!CreateProcess(_T("C://Windows//System32//cmd.exe"),cmdline,NULL,NULL,TRUE,NULL,NULL,NULL,&si,&pi))  //�����ӽ���
	{		
		DWORD dwErr = GetLastError();
		//MessageBox(_T("CreateProcess Failed!"), _T("��ʾ"),MB_OK | MB_ICONWARNING);
		return false;
	}
	CloseHandle(hWrite);  //�رչܵ����

	char buffer[4096] = {0};
	CString strOutput;
	DWORD bytesRead;

	while (true)
	{
		if (ReadFile(hRead,buffer,4095,&bytesRead,NULL) == NULL)  //��ȡ�ܵ����ݵ�buffer��  
			break;

		strOutput += buffer;
		//m_edtMsg.SetWindowText(strOutput);
		//SetDlgItemText(IDC_EDIT2,strOutput);  //��ʾ�����Ϣ���༭��,��ˢ�´���
		//UpdateWindow();
		//Sleep(100);
	}
	CloseHandle(hRead); //�رն�ȡ���

	return true;
}

//������: ffmpeg -i 20140625_111916.avi -ab 128 -acodec libmp3lame -ac 1 -ar 22050 -r 29.97 -qscale 6 -y 2.flv
//������: ffmpeg -i 20140625_111916.avi -ab 128 -acodec libmp3lame -ac 1 -ar 22050 -r 29.97 -b 360 -y 3.flv
bool CConvertVideo::Avi2Flv(const wstring& sAviFile, const wstring& sFlvFile, int width, int Height)
{
	wstringstream os;
	os << GetAppPath().GetString() << _T("\\ffmpeg.exe -i ")
		<< sAviFile
		<< _T(" -ab 128 -acodec libmp3lame -ac 1 -ar 22050 -r 29.97 -b 360 -s ")
		<< width  << _T("x") << Height
		<< _T(" ")
		<< sFlvFile 
		<< _T("");

	return Convert(os.str());
}