/*
   ������
	    ��־������
   ʵ�֣�
	1. �����ķ�ʽʵ��
	    ��.1 �����󣭣�����д��--���Ч��
	    1.2 ������������ʱ���Զ�д��
	    1.3 ����<<������,����д��
   ʱ�����ߣ���
	    created by yjt 2012-09-06
*/

#ifndef CLOGHELPER_H
#define CLOGHELPER_H
#include <time.h>

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#include "CharSetHelper.h"

class CFileLog;

//ȫ����־����
CFileLog IVS_COMM_API log();
//��ȡ��־·��
string getLogPath();

//��־��������
enum LogEndType {
    el = 0,			//�н���-end line��\r\n
    tl,				//�н��� - end line, �ڶ�����ʱ�俪ͷ
    lst				//�п�ʼ��������ʱ��
};

class CFileLog
{
public:
    enum LogType {
        Debug_Log = 0,		//������־  --�����ǵ���״̬����ӡ
        System_Log,		//ϵͳ��־	 --����ʱ��
        Run_Log			//������־	 --   ��ӡ�û�ָ����Ϣ
    };


    struct Stream {
        Stream(const string& strFileName) : ref(1), space(true), message_output(false), open(false) {
			ts.open(strFileName.c_str(), ios::app);
            if(ts.is_open())
                open = true;
        }
        ofstream ts;					//�ļ�������
        int ref;							//���ü�����
        bool space;						//�ո�
        bool message_output;		//�Ѿ����
        bool open;						//�Ѿ���
    } *stream;

public:
    inline CFileLog(const string& strFileName) : stream(new Stream(strFileName)) {
        (*this)<< lst;
    }

    inline CFileLog(const CFileLog &o):stream(o.stream) {
        ++stream->ref;
    }
    inline CFileLog& operator=(const CFileLog &other);
    inline ~CFileLog() {
        if(stream->ref == 1)
            stream->message_output = true;
        if (!--stream->ref) {
            if(stream->message_output) {
                try {
                    (*this) << el;			//��һ�����з�
                    stream->ts.flush();
                    stream->ts.close();
                } catch(std::bad_alloc&) {
                    /* We're out of memory - give up. */
                }
            }
            delete stream;
        }
    }
    inline CFileLog& space() {
        stream->space = true;
        stream->ts << ' ';
        return *this;
    }
    inline CFileLog &nospace() {
        stream->space = false;
        return *this;
    }
    inline CFileLog &maybeSpace() {
        if (stream->space) stream->ts << ' ';
        return *this;
    }

    inline CFileLog &operator<<(bool t) {
        stream->ts << (t ? "true" : "false");
        return maybeSpace();
    }
    inline CFileLog &operator<<(char t) {
        stream->ts << t;
        return maybeSpace();
    }
    inline CFileLog &operator<<(signed short t) {
        stream->ts << t;
        return maybeSpace();
    }
    inline CFileLog &operator<<(unsigned short t) {
        stream->ts << t;
        return maybeSpace();
    }
    inline CFileLog &operator<<(signed int t) {
        stream->ts << t;
        return maybeSpace();
    }
    inline CFileLog &operator<<(unsigned int t) {
        stream->ts << t;
        return maybeSpace();
    }
    inline CFileLog &operator<<(signed long t) {
        stream->ts << t;
        return maybeSpace();
    }
    inline CFileLog &operator<<(unsigned long t) {
        stream->ts << t;
        return maybeSpace();
    }
    inline CFileLog &operator<<(float t) {
        stream->ts << t;
        return maybeSpace();
    }
    inline CFileLog &operator<<(double t) {
        stream->ts << t;
        return maybeSpace();
    }
    inline CFileLog &operator<<(const char* t) {
        stream->ts << t;
        return maybeSpace();
    }
    inline CFileLog &operator<<(const string& t) {
        stream->ts << '\"' << t  << '\"';
        return maybeSpace();
    }
	inline CFileLog &operator<<(const wstring& t){
		stream->ts <<'\"' << W2C(t) << '\"';
		return maybeSpace();
	}
	inline CFileLog &operator<<(const void * t) {
        stream->ts << t;
        return maybeSpace();
    }
    inline CFileLog&operator<<(LogEndType emET) {
        time_t t;
        time(&t);
        char time_str[23]= {0};
        struct tm timeinfo;
		localtime_s ( &timeinfo,&t);
        strftime(time_str, 100, "[%Y-%m-%d %H:%M:%S] ", &timeinfo);

        if(emET == el)
            stream->ts << "\r\n";
        else if(emET == tl)
            stream->ts << "\r\n" << time_str;
        else if(emET == lst)
            stream->ts << time_str;
	return *this;
    }
};

#endif // CLOGHELPER_H
