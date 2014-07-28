//////////////////////////////////////////////////////////////////////////
//Describe:	Avi To Flv
//Date: created by yjt 2014-7-6
//History:	
//////////////////////////////////////////////////////////////////////////
#pragma once

#include <queue>
using namespace std;

typedef queue<wstring> QFlvVideo;

#define AVI2FLV CAvi2Flv::Instance()

UINT ConvertProc(LPVOID param);

class CAvi2Flv
{
	friend UINT ConvertProc(LPVOID param);
private:
	CAvi2Flv(void);
	~CAvi2Flv(void);

public:
	static CAvi2Flv& Instance(); 

	//���Ҫת������Ƶ��
	void PutVideo(const wstring& strAviPath);
	
	//����
	void Run(const wstring& strPath);
	void Stop() { m_bStop = true; }

private:
	void ConvertService();

	void InsertMediaInfo(const wstring &strIn, const wstring& strOut);

	wstring GetStartTime( const wstring& strIn );
	wstring AddTime(const wstring& strStart, const wstring& millSecond);

	wstring GetOutPutPath(const wstring& strInPath);

private:
	//Ҫ�ϳ�FLV�Ķ���
	QFlvVideo m_qFlv;

	//�����·��
	wstring m_strPath;

	//�Ƿ�ֹͣ
	bool m_bStop;
};

