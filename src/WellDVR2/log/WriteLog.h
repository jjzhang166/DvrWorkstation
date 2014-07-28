#pragma once

#include "stdafx.h"
#include <time.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <windows.h>

#define _LOG_WRITE_STATE_ 1            /* �������뿪�أ�1��д��־��0����д��־ */

#define LOG_SUCCESS (0)
#define LOG_FAILED (-1)

#define LOG_BOOL_TRUE (1)
#define LOG_BOOL_FALSE (0)

#define DWORD_NULL (0xFFFFFFFF)

#define MAX_LOGTEXT_LEN (2048)         /* ÿ����־����󳤶�*/
#define MAX_FILE_PATH (255)            /* ��־�ļ�·������󳤶�*/
#define MAX_LOG_FILE_SIZE (512 * 1024) /* ��־�ļ����ݵ���󳤶�*/
#define MAX_LOG_FILE_NAME_LEN (256)    /* ��־�ļ�������󳤶�*/
#define LOG_TYPE_INFO    0             /* ��־����: ��Ϣ����*/

#define LOG_TYPE_ERROR   1             /* ��־����: ��������*/

#define LOG_TYPE_SYSTEM 2             /* ��־����: ϵͳ����*/

#define TEST_CASE_MAX_FILE_LEN (1024)   /* ���Ժ������ļ�������󳤶�*/

const TCHAR g_LogRootPath[] = _T(".//LOG"); /*��־�ļ���·�������û�ָ��*/

#pragma pack(push, 1)

typedef struct tagLOG_DATA             /* ��־���ݽṹ��*/
{
	TCHAR             strDate[11];   /* ����:��ʽΪ��:2009-10-11*/
	TCHAR             strTime[9];    /* ʱ��:��ʽΪ��:16:10:57*/
	unsigned int iType;         /* ��־����:3��:INFO(0)/ERROR(1)/SYSTEM(2)*/
	TCHAR             strText[MAX_LOGTEXT_LEN]; /*��־����*/
	TCHAR			  strModel[20];	/*ģ����*/			
	TCHAR			  strFile[50];		/*�ļ���*/
	unsigned int iLine;					/*�к�*/
}LOG_DATA, *LPLOG_DATA;

#pragma pack(pop)

#define LOG(fmt, ...) CWriteLog::Instance().Write_Log(LOG_TYPE_INFO, _T("[WELLDVR          ]"), __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOGE(fmt, ...) CWriteLog::Instance().Write_Log(LOG_TYPE_ERROR, _T("[WELLDVR          ]"), __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOGS(fmt, ...) CWriteLog::Instance().Write_Log(LOG_TYPE_SYSTEM, _T("[WELLDVR          ]"), __FILE__, __LINE__, fmt, ##__VA_ARGS__)

class CWriteLog
{
private:
	CWriteLog(void);
	~CWriteLog(void);

	int Create_LogDir(const TCHAR *pStrPath);
	int Create_LogFile(const TCHAR *pStrFile, int iPos);
	int IsFileExist(const TCHAR *pStrFile);
	int GetLogPath(TCHAR *pStrPath);
	DWORD GetFileLenth(const TCHAR *pFile);
	int Write_Log_Text(LPLOG_DATA lpLogData);

public:
	static CWriteLog& Instance();

	void Write_Log(unsigned int uiLogType, TCHAR *pstrFmt, ...);
	void Write_Log(unsigned int uiLogType, TCHAR* pStrModel, TCHAR* pStrFile, unsigned int iLine, TCHAR *pstrFmt, ...);
	void Write_Log(unsigned int uiLogType, TCHAR* pStrModel, char* pStrFile, unsigned int iLine, TCHAR *pstrFmt, ...);
	void TestLogCase_One();

private:
	int GetLogFileName(int iLogType, const TCHAR *pStrPath, TCHAR *pStrName);
};

