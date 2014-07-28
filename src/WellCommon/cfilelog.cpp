#include "stdafx.h"
#include "cfilelog.h"
#include "DirHelper.h"
#include "CharSetHelper.h"

/**
 * ��	������������־����
 * ˵	����
 * 		1. �Զ����Ե���ʱ��Ϊ�ļ�����������־
 * 		2. ʹ�þ�̬�ķ��������죬���Ч��
 * 			��������������رգ����죩������д����������һ���У�������ǲ����û��ǿ���Ч�ʺ���
 * 			2.2 �Ժ��ٸ�Ϊÿ�λ�ȡʱ�� -- ��ȡ��һ����ʱ��
 * ʹ��ʾ��: log() << "string" << 100 << 100.00 << true << el/tl;
 * ʱ�����ߣ���created by yjt 2012-09-07
 */
CFileLog log() {
    //������־����
    return CFileLog(getLogPath());
}

/**
 * ��	���� ��ȡ��־·��
 *	1. ��������Ŀ¼ + /log/������.txt
 * ʱ�����ߣ�created by yjt 2012-09-07
 */
string getLogPath()
{
	//Ӧ�ó���·��
	string strPath = CDirHelper::GetAppPath();
    strPath += "log\\";

	CString strTmp;
	strTmp.Format(_T("%s"), CCharSetHelper::c2w(strPath).c_str());
	static bool bCreate = false;
	if(!bCreate)
	{
		if(CDirHelper::CreateDir(strTmp))
			bCreate = true;
	}
	
	//����ʱ��.txt
	time_t t;
	time(&t);
	char time_str[20]= {0};
	struct tm timeinfo;
	localtime_s (&timeinfo, &t);
	strftime(time_str, 100, "%Y%m%d.txt", &timeinfo);

	strPath += time_str;
    return strPath;
}


/*
int main()
{
    / *
	//����1
    CLog log("log\\log.txt");
    log << "sssssa;" << tl;
    log << "��ӡ����Ϣ" << el;
    * /
	
	//����2
    log() << "��ӡ�պ�" << 100 << tl;

    return 0;
}
*/
