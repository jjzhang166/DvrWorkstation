#include "stdafx.h"
#include "WriteLog.h"


CWriteLog::CWriteLog(void)
{
}


CWriteLog::~CWriteLog(void)
{
}

CWriteLog& CWriteLog::Instance()
{
	static CWriteLog log;
	return log;
}

// 
// int main(int argc, char* argv[])
// {
// 	Write_Log(LOG_TYPE_SYSTEM, "Program begin.");
// 	TestLogCase_One();
// 	Write_Log(LOG_TYPE_SYSTEM, "Program end.");
// 
// 	return 0;
// }

/*********************************************************************
* ��������:void TestLogCase_One()
* ˵��:�򵥵Ĳ��Ժ��������ļ�
* �����ߣ�main
* �������:
* ��
* ���������
* ��
* ����ֵ:
* void --
*********************************************************************/
void CWriteLog::TestLogCase_One()
{
	FILE *pFile = NULL;
	TCHAR *pFieldContent = NULL;
	TCHAR szFileName[] = _T("test_case.txt");

	pFieldContent = (TCHAR *)malloc(TEST_CASE_MAX_FILE_LEN);
	if(NULL == pFieldContent)
	{
		Write_Log(LOG_TYPE_ERROR, _T("malloc memory failed,program exit!"));
		return;
	}

	memset(pFieldContent, 0, TEST_CASE_MAX_FILE_LEN);

	Write_Log(LOG_TYPE_INFO, _T("malloc memory for pFiled successful,memory size is: %ld"),
		TEST_CASE_MAX_FILE_LEN);

	Write_Log(LOG_TYPE_INFO, _T("��������д��"),
		TEST_CASE_MAX_FILE_LEN);

	pFile = _tfopen(szFileName, _T("r"));
	if(NULL == pFile)
	{
		fprintf(stderr, "open file failed.");
		Write_Log(LOG_TYPE_ERROR, _T("Open file %s failed. program exit!"), szFileName);
		return;
	}

	Write_Log(LOG_TYPE_INFO, _T("Open file %s successful."), szFileName);

	fread(pFieldContent, 1, TEST_CASE_MAX_FILE_LEN, pFile);
	pFieldContent[TEST_CASE_MAX_FILE_LEN -1] = '\0';

	fclose(pFile);
	printf("The file %s content is: \n%s\n", szFileName, pFieldContent);
	Write_Log(LOG_TYPE_INFO, _T("The file %s content is: \n%s\n"), szFileName, pFieldContent);

}

/*********************************************************************
* ��������:void Write_Log(unsigned int uiLogType, char *pstrFmt, ...)
* ˵��:��־д������֧�ֱ䳤����
* �����ߣ��κ���Ҫд��־�ĵط�
* �������:
* unsigned iType -- ��־���
* char *pstrFmt -- ��־����
* ...            -- �䳤����
* ���������
* ��
* ����ֵ:
* void --
*********************************************************************/
void CWriteLog::Write_Log(unsigned int uiLogType, TCHAR *pstrFmt, ...)
{
#if _LOG_WRITE_STATE_   /* д��־���ı��뿪��*/

	LOG_DATA data;

	time_t curTime;
	struct tm *mt;
	va_list v1;
	memset(&data, 0, sizeof(LOG_DATA));
	va_start(v1, pstrFmt);
	_vsntprintf(data.strText, MAX_LOGTEXT_LEN, pstrFmt, v1);
	va_end(v1);
	data.iType = uiLogType;
	curTime = time(NULL);
	mt = localtime(&curTime);
	_tcsftime(data.strDate, sizeof(data.strDate), _T("%Y-%m-%d"), mt);
	_tcsftime(data.strTime, sizeof(data.strTime), _T("%H:%M:%S"), mt);
	Write_Log_Text(&data);
#endif _LOG_WRITE_STATE_
}


void CWriteLog::Write_Log(unsigned int uiLogType, TCHAR* pStrModel, TCHAR* pStrFile, unsigned int iLine, TCHAR *pstrFmt, ...)
{
#if _LOG_WRITE_STATE_   /* д��־���ı��뿪��*/

	LOG_DATA data;

	time_t curTime;
	struct tm *mt;
	va_list v1;
	memset(&data, 0, sizeof(LOG_DATA));
	va_start(v1, pstrFmt);
	_vsntprintf(data.strText, MAX_LOGTEXT_LEN, pstrFmt, v1);
	va_end(v1);
	data.iType = uiLogType;
	curTime = time(NULL);
	mt = localtime(&curTime);
	_tcsftime(data.strDate, sizeof(data.strDate), _T("%Y-%m-%d"), mt);
	_tcsftime(data.strTime, sizeof(data.strTime), _T("%H:%M:%S"), mt);

	_tcscat(data.strModel, pStrModel);
	_tcscat(data.strFile, pStrFile);
	data.iLine = iLine;
	
	Write_Log_Text(&data);
#endif _LOG_WRITE_STATE_
}


void CWriteLog::Write_Log(unsigned int uiLogType, TCHAR* pStrModel, char* pStrFile, unsigned int iLine, TCHAR *pstrFmt, ...)
{
#if _LOG_WRITE_STATE_   /* д��־���ı��뿪��*/

	LOG_DATA data;

	time_t curTime;
	struct tm *mt;
	va_list v1;
	memset(&data, 0, sizeof(LOG_DATA));
	va_start(v1, pstrFmt);
	_vsntprintf(data.strText, MAX_LOGTEXT_LEN, pstrFmt, v1);
	va_end(v1);
	data.iType = uiLogType;
	curTime = time(NULL);
	mt = localtime(&curTime);
	_tcsftime(data.strDate, sizeof(data.strDate), _T("%Y-%m-%d"), mt);
	_tcsftime(data.strTime, sizeof(data.strTime), _T("%H:%M:%S"), mt);

	_tcscat(data.strModel, pStrModel);

	//ת��TCHAR
	int len = strlen(pStrFile);
	wchar_t * pUnicode = new  wchar_t[ len + 1];  
	memset(pUnicode,0,(len +1)*sizeof(wchar_t));  
	::MultiByteToWideChar( CP_ACP, 0, pStrFile, -1, (LPWSTR)pUnicode, len );  
	_tcscat(data.strFile, pUnicode);
	delete[] pUnicode;
	pUnicode = NULL;

	data.iLine = iLine;

	Write_Log_Text(&data);
#endif _LOG_WRITE_STATE_
}

/*********************************************************************
* ��������:int GetLogPath(char *pStrPath)
* ˵��:��ȡ��־�ļ�·��
* �����ߣ�Write_Log_Text
* �������:
* ��
* ���������
* char *pStrPath
* ����ֵ:
* int -- LOG_FAILED: ʧ��
*      -- LOG_SUCCESS: �ɹ�
*********************************************************************/
int CWriteLog::GetLogPath(TCHAR *pStrPath)
{
	if(NULL == pStrPath)
	{
		return LOG_FAILED;
	}
	int iRet = 0;
	time_t curTime = time(NULL);
	struct tm *mt = localtime(&curTime);
	/* ������������ļ�������*/
	_stprintf(pStrPath, _T("%s\\%s\\%d%02d%02d"), GetAppPath(), g_LogRootPath, mt->tm_year + 1900, mt->tm_mon + 1, mt->tm_mday);
	iRet = Create_LogDir(pStrPath);

	return iRet;
}

/*********************************************************************
* ��������:int GetLogFileName(int iLogType, const char *pStrPath, char *pStrName)
* ˵��:��ȡ��־�ļ���
* �����ߣ�Write_Log_Text
* �������:
* int iLogType         -- ��־���� 3��:INFO(0)/ERROR(1)/SYSTEM(2)
* const char *pStrPath -- ��־·�� ��GetLogPath�õ�
* ���������
* char *pStrName       -- ��־�ļ���
* ����ֵ:
* int -- LOG_FAILED: ʧ��
*      -- LOG_SUCCESS: �ɹ�
*********************************************************************/
int CWriteLog::GetLogFileName(int iLogType, const TCHAR *pStrPath, TCHAR *pStrName)
{
	if(NULL == pStrPath)
	{
		return LOG_FAILED;
	}
	TCHAR szLogName[MAX_FILE_PATH];
	FILE *pFile = NULL;
	memset(szLogName, 0, MAX_FILE_PATH);
	switch (iLogType)
	{
	case LOG_TYPE_INFO:
		_stprintf(szLogName, _T("%s\\app_info"), pStrPath);
		break;
	case LOG_TYPE_ERROR:
		_stprintf(szLogName, _T("%s\\app_error"), pStrPath);
		break;
	case LOG_TYPE_SYSTEM:
		_stprintf(szLogName, _T("%s\\app_system"), pStrPath);
		break;
	default:
		return LOG_FAILED;
		break;
	}
	_tcscat(szLogName, _T(".log"));
	if(IsFileExist(szLogName))
	{
		/* ����ļ����ȴ���ָ������󳤶ȣ����´���һ�ļ�������ԭ�ļ�*/
		if((int)GetFileLenth(szLogName) + 256 >= MAX_LOG_FILE_SIZE)
		{
			Create_LogFile(szLogName, 0);
		}
	}
	else
	{
		Create_LogFile(szLogName, 0);
	}
	_stprintf(pStrName, _T("%s"), szLogName);
	return LOG_SUCCESS;
}

/*********************************************************************
* ��������:int Create_LogDir(const char *pStrPath)
* ˵��:������־���·��
* �����ߣ�GetLogPath
* �������:
* const char *pStrPath --�û�ָ���ĸ�·��
* ���������
* ��
* ����ֵ:
* int -- LOG_FAILED: ʧ��
*      -- LOG_SUCCESS: �ɹ�
*********************************************************************/
int CWriteLog::Create_LogDir(const TCHAR *pStrPath)
{
	if(NULL == pStrPath)
	{
		return LOG_FAILED;
	}
	int iRet = 0;
	TCHAR szSub[MAX_FILE_PATH];
	TCHAR *pSub = NULL;
	int iIndex = 0;
	int iLen = 0;
	int bFind = 0;
	memset(szSub, 0, sizeof(MAX_FILE_PATH));
	/* ��㴴��Ŀ¼*/
	while(1)
	{
		pSub = (TCHAR*)_tcschr(pStrPath + iLen, _T('\\'));
		if(NULL == pSub)
		{
			if(iLen == 0)
			{
				return LOG_FAILED;
			}
			iRet = CreateDirectory(pStrPath, NULL);
			if(0 == iRet)
			{
				iRet = GetLastError();
				if(ERROR_ALREADY_EXISTS == iRet)
				{
					return LOG_SUCCESS;
				}
				return LOG_FAILED;
			}
			return LOG_SUCCESS;
		}
		else
		{
			if (!bFind)
			{
				bFind = 1;
			}
			else
			{
				memset(szSub, 0, sizeof(szSub));
				_tcsncpy(szSub, pStrPath, pSub - pStrPath);
				CreateDirectory(szSub, NULL);
			}
			iLen = pSub - pStrPath + 1;
		}
	}
	return LOG_SUCCESS;
}

/*********************************************************************
* ��������:int Create_LogFile(const char *pStrFile, int iPos)
* ˵��:������־�ļ�
* �����ߣ�GetLogFileName
* �������:
* const char *pStrFile --�ļ���
* int iPos             --�ļ�ָ��λ��
* ���������
* ��
* ����ֵ:
* int -- LOG_FAILED: ʧ��
*      -- LOG_SUCCESS: �ɹ�
*********************************************************************/
int CWriteLog::Create_LogFile(const TCHAR *pStrFile, int iPos)
{
	HANDLE hd = 0;
	int iRet = 0;
	if(NULL == pStrFile)
	{
		return LOG_FAILED;
	}
	hd = CreateFile(pStrFile,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
		);
	if(INVALID_HANDLE_VALUE == hd)
	{
		return LOG_FAILED;
	}
	if(DWORD_NULL == SetFilePointer(hd, iPos, NULL, FILE_BEGIN))
	{
		return LOG_FAILED;
	}
	iRet = SetEndOfFile(hd);
	CloseHandle(hd);
	return iRet;
}

/*********************************************************************
* ��������:int IsFileExist(const char *pStrFile)
* ˵��:�ж�ָ�����ļ��Ƿ����
* �����ߣ�GetLogFileName
* �������:
* const char *pStrFile --�ļ���
* ���������
* ��
* ����ֵ:
* int -- LOG_BOOL_FALSE: ������
*      -- LOG_BOOL_TRUE: ����
*********************************************************************/
int CWriteLog::IsFileExist(const TCHAR *pStrFile)
{
	int iLen = 0;
	WIN32_FIND_DATA finddata;
	memset(&finddata, 0, sizeof(WIN32_FIND_DATA));
	HANDLE hd = FindFirstFile(pStrFile, &finddata);
	if(INVALID_HANDLE_VALUE == hd)
	{
		DWORD dwRet = GetLastError();
		if(ERROR_FILE_NOT_FOUND == dwRet || ERROR_PATH_NOT_FOUND == dwRet)
		{
			return LOG_BOOL_FALSE;
		}
	}
	FindClose(hd);
	return LOG_BOOL_TRUE;
}

/*********************************************************************
* ��������:DWORD GetFileLenth(const char *pFile)
* ˵��:�ж�ָ�����ļ���С
* �����ߣ�GetLogFileName
* �������:
* const char *pFile --�ļ���
* ���������
* ��
* ����ֵ:
* DWORD -- �ļ���С
*********************************************************************/
DWORD CWriteLog::GetFileLenth(const TCHAR *pFile)
{
	WIN32_FIND_DATA buff;
	HANDLE hd = NULL;
	memset(&buff, 0, sizeof(WIN32_FIND_DATA));
	hd = FindFirstFile(pFile, &buff);
	FindClose(hd);
	return (buff.nFileSizeHigh * MAXDWORD) + buff.nFileSizeLow;
}

/*********************************************************************
* ��������:int Write_Log_Text(LPLOG_DATA lpLogData)
* ˵��:д��־����
* �����ߣ�Write_Log
* �������:
* LPLOG_DATA lpLogData --��־���ݽṹ����
* ���������
* ��
* ����ֵ:
* int -- LOG_FAILED: ʧ��
*      -- LOG_SUCCESS: �ɹ�
*********************************************************************/
int CWriteLog::Write_Log_Text(LPLOG_DATA lpLogData)
{
	TCHAR szFilePath[MAX_FILE_PATH];
	TCHAR szFileName[MAX_LOG_FILE_NAME_LEN];
	FILE *pFile = NULL;
	TCHAR szLogText[MAX_LOGTEXT_LEN];
	memset(szFilePath, 0, MAX_FILE_PATH);
	memset(szFileName, 0, MAX_LOG_FILE_NAME_LEN);
	memset(szLogText, 0, MAX_LOGTEXT_LEN);
	GetLogPath(szFilePath);
	GetLogFileName(lpLogData->iType, szFilePath, szFileName);
	pFile = _tfopen(szFileName, _T("a+"));
	if(NULL == pFile)
	{
		return LOG_FAILED;
	}
	_stprintf(szLogText, _T("%s[%s %s] %s   -----  [%s][%d]\n"), lpLogData->strModel, lpLogData->strDate, lpLogData->strTime, lpLogData->strText, lpLogData->strFile, lpLogData->iLine);

	//fwrite(szLogText, 1, _tcslen(szLogText), pFile);
	//acsii �ķ�ʽ�� unicodeд������
	int len = _tcslen(szLogText);
	char* pchBuffer = new char[len * 2 + 1];
	int a = WideCharToMultiByte(CP_ACP, NULL, (LPCWCH)szLogText, -1, LPSTR(pchBuffer), len*2 + 1, NULL, FALSE);//Unicodeת��ΪANSI��a��ֵ�������ַ�������0
	fwrite(pchBuffer, 1, a - 1, pFile);
	delete[] pchBuffer;
	pchBuffer = NULL;

	fclose(pFile);
	return LOG_SUCCESS;
}