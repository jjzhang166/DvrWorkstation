#include "stdafx.h"
#include "Avi2Flv.h"
#include "../WellCommon/AviHelper.h"
#include "ConvertVideo.h"
#include "../WellCommon/DirHelper.h"
#include "../ShareData.h"
#include "../DataBase/DataBase.h"
#include "../WellCommon/StringHelper.h"

CAvi2Flv::CAvi2Flv(void)
{
	m_bStop = false;
}


CAvi2Flv::~CAvi2Flv(void)
{
}

CAvi2Flv& CAvi2Flv::Instance()
{
	static CAvi2Flv aviflv;
	return aviflv;
}

//���������ȡ����·��
wstring CAvi2Flv::GetOutPutPath(const wstring& strInPath)
{
	wstring strExt = strInPath.substr( SHAREDATA.g_strFilePath.GetLength() + 1);
	wstring str = strExt.substr(0, strExt.length() - 4);
	return m_strPath + str + _T(".flv");
}

void CAvi2Flv::PutVideo(const wstring& strAviPath)
{
	wstring strExt = strAviPath.substr(strAviPath.length() - 4);
	if(strExt == _T(".AVI") || strExt == _T(".avi"))
	{
//		strExt = strAviPath.substr( SHAREDATA.g_strFilePath.GetLength() + 1);
		m_qFlv.push(strAviPath);
	}
}

void CAvi2Flv::Run(const wstring& strPath)
{
	m_strPath = strPath;
	//����ת���߳�
	AfxBeginThread((AFX_THREADPROC)ConvertProc, this);
}

UINT ConvertProc(LPVOID param)
{
	CAvi2Flv* aviflv = (CAvi2Flv*)param;
	if(aviflv)
	{
		aviflv->ConvertService();
	}
	return 0;
}


void CAvi2Flv::ConvertService()
{
	while(!m_bStop)
	{
		if(m_qFlv.empty())
		{
			Sleep(200);
			continue;
		}
		wstring strIn = m_qFlv.front();
		if(strIn.empty())
		{
			Sleep(200);
			continue;
		}

		wstring strOut = GetOutPutPath(strIn);
		wstring strOutPath = strOut.substr(0, strOut.rfind(_T("\\")));
		CDirHelper::CreateDir(strOutPath.c_str());

		
		int width = 0;
		int height = 0;
		CAviHelper::AVI_resolution(strIn, width, height); 

		CConvertVideo::Avi2Flv(strIn, strOut, width, height);

		//ת��ɹ�����뵽���ݿ�
		InsertMediaInfo(strIn, strOut);

		m_qFlv.pop();
		Sleep(10);
	}
}


void CAvi2Flv::InsertMediaInfo(const wstring &strIn, const wstring& strOut)
{
	AVIFILEINFO aviInfo = CAviHelper::AVI_GetFileInfo(strIn);
	//ת��ɹ�������ݿ�
	DVRMEDIA_INFO mi;
	CString strT;
	strT.Format(_T("%d"), aviInfo.dwWidth);
	mi.strViewWidth = strT.GetString();
	strT.Format(_T("%d"), aviInfo.dwHeight);
	mi.strViewHeight = strT.GetString();
	wstring strPath = strIn.substr( SHAREDATA.g_strFilePath.GetLength() + 1);
	wstring str = strPath.substr(0, strPath.find(_T("\\")) );
	mi.strDvrNumber = str;

	CTime tm = CTime::GetCurrentTime();
	mi.strDataUploadTime = tm.Format(_T("%Y-%m-%d %H:%M:%S"));
	wstring strIp;
	mi.strDataIp = SHAREDATA.g_pMainFrame->GetIpAddr(strIp), strIp.c_str();
	mi.strFileName = strIn.substr(strIn.rfind(_T("\\")) + 1);

	CFile localFile;
	if(!localFile.Open(strIn.c_str(), CFile::modeRead|CFile::shareDenyNone))
	{
		strT.Format(_T("%s"), 0);
	}
	else
	{
		strT.Format(_T("%d"), localFile.GetLength());
		localFile.Close();
	}
	mi.strFileSize = strT;

	mi.strFileType = _T("AVI");
	strT.Format(_T("%d"), aviInfo.dwRate / aviInfo.dwScale);
	mi.strFrameRate = strT;

	//wstring sFtpip, sFtpport, sIISip, sIISport;
	//bool bRlt = DB.QueryIpPortByDevId(SHAREDATA.g_strDeviceId.GetString(), sFtpip, sFtpport, sIISip, sIISport);

	strPath = strOut.substr(SHAREDATA.g_strFilePath.GetLength() + 1);
	mi.strFtpFileUrl = strPath; //_T("ftp://") + sFtpip + _T(":") + sFtpport  + _T("//") +  strPath;
	mi.strIISFileUrl = strPath; //_T("http://") + sIISip + _T(":") + sIISport + _T("//") + strPath;
	mi.strLocateFilePath = strIn.c_str();
	strT.Format(_T("%d"), aviInfo.dwRate / aviInfo.dwScale * aviInfo.dwLength);
	mi.strMediaDuration = strT;	//����

	mi.strMediaStartTime = GetStartTime(strIn);
	mi.strMediaEndTime = AddTime(mi.strMediaStartTime, mi.strMediaDuration);
	mi.strRecordTime = mi.strMediaStartTime;

	mi.strRemark = _T(" ");
	mi.strVideoType = _T("AVI");

	DB.InsertMediaInfo(mi);
}

//////////////////////////////////////////////////////////////////////////
//���ļ����л�ȡʱ�䴮
// 1. �ļ����ĸ�ʽ:
//			a.  ����/ID_����_ʱ��_IPM.avi
//			b.  ����/����_ʱ��.avi
//			c.	����/ʱ��.avi
// 2. ���ɵ��ļ���:
//		 2014-07-07 22:52:12
//////////////////////////////////////////////////////////////////////////
wstring CAvi2Flv::GetStartTime( const wstring& strIn)
{
	vector<wstring> vecPath = CStringHelper::split(strIn, _T("\\"));
	if(vecPath.size() <= 2)
		return _T("");

	wstring strLast = vecPath[vecPath.size() - 1];
	wstring strDate = vecPath[vecPath.size() -2];

	//��ȡ����
	if(strDate.length() > 8)	//ID_����
	{
		strDate = strDate.substr(strDate.length() - 8);
	}

	int y, m, d, h, min, s;   
	swscanf(strDate.c_str(), _T("%4d%2d%2d"), &y, &m, &d);

	//��ȡʱ��
	strLast = strLast.substr(0, strLast.length() - 4);		//ȥ����׺��
	vecPath = CStringHelper::split(strLast, _T("_"));
	if(vecPath.size() == 1 && vecPath[0].size() == 6)
	{
		swscanf(vecPath[0].c_str(), _T("%2d%2d%2d"), &h, &min, &s);
	}
	else if(vecPath.size() >= 2)
	{
		int size = vecPath.size();
		int index = 1;
		if(vecPath[size - 1].length() == 3)			//��ʶ	
		{
			index = size - 2;
		}
		swscanf(vecPath[index].c_str(), _T("%2d%2d%2d"), &h, &min, &s);
	}

	CTime time(y,m,d,h,min,s);

	return time.Format(_T("%Y-%m-%d %H:%M:%S"));
}

//////////////////////////////////////////////////////////////////////////
//��������ַ���ʱ��
//	strStart [IN]		--		��ʼʱ��,��ʽ(2014-07-07 22:52:12)
//	millSecond [IN]		--		������
//���ؽ���ʱ��
//////////////////////////////////////////////////////////////////////////
wstring CAvi2Flv::AddTime(const wstring& strStart, const wstring& millSecond)
{
	int y, m, d, h, min, s;   
	swscanf(strStart.c_str(), _T("%d-%d-%d %d:%d:%d"), &y, &m, &d, &h, &min, &s);
	CTime   time(y,m,d,h,min,s); 

	int nSecond = _wtoi(millSecond.c_str());
	CTimeSpan delta(0, 0, 0, nSecond / 1000 + 1);
	time = time + delta;

	return time.Format(_T("%Y-%m-%d %H:%M:%S"));
}