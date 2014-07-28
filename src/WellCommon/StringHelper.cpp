#include "StdAfx.h"
#include "StringHelper.h"

CStringHelper::CStringHelper(void)
{
}

CStringHelper::~CStringHelper(void)
{
}

/***************************************************************************************
*��    ��: �ָ��ַ���
*
*�����б�:	strSrc [IN]		--		Ҫ�ָ��ԭʼ��
*				splitString [IN]	--		�ָ��
*�� �� ֵ:�ַ�������--�洢�ָ�ļ���
*
*ʱ������: created by yjt [2012/10/25 17:05]
*
*�޸ļ�¼:
*
***************************************************************************************/
vector<CString> CStringHelper::split(const CString& strSrc, const CString& splitString)
{
	vector<CString> vecRlt;

	int nStart = 0;
	int nIndex = 0;
	CString strTmp;
	nIndex = strSrc.Find(splitString, nStart);
	while(nIndex > 0)
	{
		strTmp = strSrc.Mid(nStart, nIndex - nStart);
		vecRlt.push_back(strTmp);
		nStart = nIndex + splitString.GetLength();
		nIndex = strSrc.Find(splitString, nStart);
	}
	if(strSrc.GetLength() > nStart)
	{
		strTmp = strSrc.Mid(nStart, strSrc.GetLength() - nStart);
		vecRlt.push_back(strTmp);
	}
	return vecRlt;
}


vector<string> CStringHelper::split(const string& strSrc, const string& splitStr)
{
	vector<string> vecRlt;
	string strSrcT(strSrc);
	string::size_type nPosb = 0;
	string::size_type nPosF = strSrcT.find(splitStr, nPosb);
	string strTmp;
	while( nPosF != string::npos)
	{
		strTmp = strSrcT.substr(nPosb, nPosF - nPosb);
		vecRlt.push_back(strTmp);
		nPosb = nPosF + splitStr.length();
		nPosF = strSrcT.find(splitStr, nPosb);
	}
	if(nPosb < strSrcT.length())
	{
		strTmp = strSrcT.substr(nPosb, strSrcT.length() - nPosb);
		vecRlt.push_back(strTmp);
	}

	return vecRlt;
}


vector<wstring> CStringHelper::split(const wstring& strSrc, const wstring& splitStr)
{
	vector<wstring> vecRlt;
	wstring strSrcT(strSrc);
	wstring::size_type nPosb = 0;
	wstring::size_type nPosF = strSrcT.find(splitStr, nPosb);
	wstring strTmp;
	while( nPosF != wstring::npos)
	{
		strTmp = strSrcT.substr(nPosb, nPosF - nPosb);
		vecRlt.push_back(strTmp);
		nPosb = nPosF + splitStr.length();
		nPosF = strSrcT.find(splitStr, nPosb);
	}
	if(strSrcT.length() > nPosb)
	{
		strTmp = strSrcT.substr(nPosb, strSrcT.length() - nPosb);
		vecRlt.push_back(strTmp);
	}

	return vecRlt;
}

/***************************************************************************************
*��    ��:	�滻�ַ���
*
*�����б�:	strSrc [IN]	[OUT]		--		Դ�ַ�����ͬʱҲ������ַ���
*				strFind [IN]				--		Ҫ���滻�Ĵ�
*				strReplace [IN]			--		�滻�ַ���
*�� �� ֵ:
*
*ʱ������: created by yjt [2012/12/4 14:15]
*
*�޸ļ�¼:
*
***************************************************************************************/
void CStringHelper::Replace( wstring &strSrc, const wstring &strFind, const wstring &strReplace )
{
	if(strSrc.empty() || strFind.empty() || strReplace.empty())
		return;

	wstring::size_type nFindPos = 0;
	nFindPos = strSrc.find(strFind, nFindPos);
	while (nFindPos != wstring::npos) 
	{
		strSrc.replace(nFindPos, strFind.length(), strReplace);
		nFindPos += strReplace.length();
		nFindPos = strSrc.find(strFind, nFindPos);
	}
}
