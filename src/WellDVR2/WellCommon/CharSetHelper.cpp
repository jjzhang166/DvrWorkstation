#include "StdAfx.h"
#include "CharSetHelper.h"


CCharSetHelper::CCharSetHelper(void)
{
}


CCharSetHelper::~CCharSetHelper(void)
{
}

/***************************************************************************************
*��    ��: ��wchar_t* ת��char*
*
*�����б�: *pcstr [OUT]		--		���ڽ���ת���������
*		   *pwstr [IN]		--		Ҫת���Ŀ��ֽ�ָ��
*		   len [IN]			--		���ջ������ĳ���
*								���len = 0�������Զ�����: ���ֽڳ���*2 + 1								
*
*�� �� ֵ: 
*
*ʱ������: created by yjt [2012/5/25 13:12]
*
*�޸ļ�¼:
//modified by yjt [2012/10/29 11:31] �޸���buff[nbytes + 1] = 0; �±�nbytes��Ϊnbytes+1λ��
***************************************************************************************/
char* CCharSetHelper::w2c(char *pcstr,wchar_t *pwstr, size_t len)
{
	if(!pwstr)
	{
		pcstr = NULL;
		return 0;
	}

	int nlength=wcslen(pwstr);
	
	//�Զ����㳤��
	if(len == 0)
		len = nlength*2 + 1;

	//��ȡ�����㣩ת����ĳ���
	int nbytes = WideCharToMultiByte( 0, // specify the code page used to perform the conversion
		0,         // no special flags to handle unmapped characters
		pwstr,     // wide character string to convert
		nlength,   // the number of wide characters in that string
		NULL,      // no output buffer given, we just want to know how long it needs to be
		0,
		NULL,      // no replacement character given
		NULL );    // we don't want to know if a character didn't make it through the translation
	
	// make sure the buffer is big enough for this, making it larger if necessary
	if((size_t)nbytes >= len)   
		nbytes = len - 1;
	
	char* buff = new char[nbytes + 1];

	//ͨ�����ϵõ��Ľ����ת��unicode �ַ�Ϊascii �ַ�
	WideCharToMultiByte( 0, // specify the code page used to perform the conversion
		0,			// no special flags to handle unmapped characters
		pwstr,		// wide character string to convert
		nlength,	// the number of wide characters in that string
		buff,		// put the output ascii characters at the end of the buffer
		nbytes,		// there is at least this much space there
		NULL,		// no replacement character given
		NULL );
	
	//������
	buff[nbytes] = 0;
	pcstr = buff;

	return pcstr ;
}

/***************************************************************************************
*��    ��: ��wchar_t* ת��char*
*
*�����б�: *pcstr [OUT]		--		���ڽ���ת���������
*		   *pwstr [IN]		--		Ҫת���Ŀ��ֽ�ָ��							
*
*�� �� ֵ: ���ؽ��ջ�����(ת��������)�ĳ���
*
*ʱ������: created by yjt [2012/5/25 13:12]
*
*�޸ļ�¼:
//modified by yjt [2012/10/29 11:31] �޸���buff[nbytes + 1] = 0; �±�nbytes��Ϊnbytes+1λ��
***************************************************************************************/
size_t CCharSetHelper::w2c(char *pcstr, wchar_t *pwstr)
{
	if(!pwstr)
	{
		pcstr = NULL;
		return 0;
	}

	int nlength=wcslen(pwstr);

	//��ȡ�����㣩ת����ĳ���
	int nbytes = WideCharToMultiByte( 0, // specify the code page used to perform the conversion
		0,         // no special flags to handle unmapped characters
		pwstr,     // wide character string to convert
		nlength,   // the number of wide characters in that string
		NULL,      // no output buffer given, we just want to know how long it needs to be
		0,
		NULL,      // no replacement character given
		NULL );    // we don't want to know if a character didn't make it through the translation

	char* buff = new char[nbytes + 1];

	//ͨ�����ϵõ��Ľ����ת��unicode �ַ�Ϊascii �ַ�
	WideCharToMultiByte( 0, // specify the code page used to perform the conversion
		0,			// no special flags to handle unmapped characters
		pwstr,		// wide character string to convert
		nlength,	// the number of wide characters in that string
		buff,		// put the output ascii characters at the end of the buffer
		nbytes,		// there is at least this much space there
		NULL,		// no replacement character given
		NULL );

	//������
	buff[nbytes] = 0;
	pcstr = buff;

	return nbytes;
}

string CCharSetHelper::w2c(const wstring& wstr)
{
	if(wstr.size() == 0)
	{
		return "";
	}

	int nlength=wstr.size();

	//��ȡ�����㣩ת����ĳ���
	int nbytes = WideCharToMultiByte( 0, // specify the code page used to perform the conversion
		0,         // no special flags to handle unmapped characters
		wstr.c_str(),     // wide character string to convert
		nlength,   // the number of wide characters in that string
		NULL,      // no output buffer given, we just want to know how long it needs to be
		0,
		NULL,      // no replacement character given
		NULL );    // we don't want to know if a character didn't make it through the translation

	char* buff = new char[nbytes + 1];

	//ͨ�����ϵõ��Ľ����ת��unicode �ַ�Ϊascii �ַ�
	WideCharToMultiByte( 0, // specify the code page used to perform the conversion
		0,			// no special flags to handle unmapped characters
		wstr.c_str(),		// wide character string to convert
		nlength,	// the number of wide characters in that string
		buff,		// put the output ascii characters at the end of the buffer
		nbytes,		// there is at least this much space there
		NULL,		// no replacement character given
		NULL );
	buff[nbytes] = 0;
	//������
	string strRlt = buff;
	delete[] buff;
	return strRlt;
}

size_t CCharSetHelper::w2c(const wstring& wstrIn, string& strOut)
{
	if(wstrIn.empty())
	{
		strOut = "";
		return 0;
	}

	int nlength=wstrIn.size();

	//��ȡ�����㣩ת����ĳ���
	int nbytes = WideCharToMultiByte( 0, // specify the code page used to perform the conversion
		0,         // no special flags to handle unmapped characters
		wstrIn.c_str(),     // wide character string to convert
		nlength,   // the number of wide characters in that string
		NULL,      // no output buffer given, we just want to know how long it needs to be
		0,
		NULL,      // no replacement character given
		NULL );    // we don't want to know if a character didn't make it through the translation

	char* buff = new char[nbytes + 1];

	//ͨ�����ϵõ��Ľ����ת��unicode �ַ�Ϊascii �ַ�
	WideCharToMultiByte( 0, // specify the code page used to perform the conversion
		0,			// no special flags to handle unmapped characters
		wstrIn.c_str(),		// wide character string to convert
		nlength,	// the number of wide characters in that string
		buff,		// put the output ascii characters at the end of the buffer
		nbytes,		// there is at least this much space there
		NULL,		// no replacement character given
		NULL );
	buff[nbytes] = 0;
	//������
	strOut = buff;

	//delete[] buff;
	return nbytes;
}

/***************************************************************************************
*��    ��: ��char* ת��wchar_t*
*
*�����б�: *pwstr [OUT]		--		����ת����Ŀ��ֽ�����
*		   *pstr [IN]		--		ָ��Դ����
*		   len [IN]			--		���ջ������ĳ���
*
*�� �� ֵ:
*
*ʱ������: created by yjt [2012/5/25 13:16]
*
*�޸ļ�¼:
//modified by yjt [2012/10/29 11:31] �޸���buff[nbytes + 1] = 0; �±�nbytes��Ϊnbytes+1λ��
***************************************************************************************/
void CCharSetHelper::c2w(wchar_t *pwstr, size_t len, char *pstr)
{
	if(pstr)
	{
		size_t nu = strlen(pstr);
		
		//���������
		size_t n = (size_t)MultiByteToWideChar(CP_ACP,0,(const char *)pstr,(int)nu,NULL,0);
		
		//��������ĳ��ȴ��ڻ�������ֻȡ�������Ĵ�С
		if(n >= len)
			n = len - 1;
		
		MultiByteToWideChar(CP_ACP,0,(const char *)pstr,(int)nu, pwstr,(int)n);
		
		//���һ���ַ�����
		pwstr[n]=0;
	}
}

/***************************************************************************************
*��    ��: ��ascii �ַ�ת��Ϊunicode�ַ�
*
*�����б�: *pwstr [OUT]		--		����ת����Ŀ��ֽ�����
*		   *pstr [IN]		--		ָ��Դ����
*
*�� �� ֵ: ����ת�������ݵĳ���
*
*ʱ������: created by yjt [2012/5/25 13:19]
*
*�޸ļ�¼:
//modified by yjt [2012/10/29 11:31] �޸���buff[nbytes + 1] = 0; �±�nbytes��Ϊnbytes+1λ��
***************************************************************************************/
size_t CCharSetHelper::c2w(wchar_t *pwstr, char *str)
{
	wchar_t* buffer = NULL;
	size_t n = 0;
	if(str)
	{
		size_t nu = strlen(str);
		n =(size_t)MultiByteToWideChar(CP_ACP,0,(const char *)str,int(nu),NULL,0);
		buffer=0;
		buffer = new wchar_t[n+1];
		//if(n>=len) n=len-1;
		::MultiByteToWideChar(CP_ACP,0,(const char *)str,int(nu),buffer,int(n));    
		//������
		buffer[n] = 0;
	}
	pwstr = buffer;

	//���ػ������ĳ���
	return n;
	//delete[] buffer;
}

wstring CCharSetHelper::c2w(const string& str)
{
	if(str.empty())
		return L"";

	wchar_t* buffer = NULL;
	size_t n = 0;
	if(str.size() > 0)
	{
		size_t nu = str.size();
		n = (size_t)MultiByteToWideChar(CP_ACP,0,(const char *)str.c_str(), int(nu),NULL,0);
		buffer = new wchar_t[n + 1];
		//if(n>=len) n=len-1;
		::MultiByteToWideChar(CP_ACP,0,(const char *)str.c_str(),int(nu),buffer,int(n));    
		//������
		buffer[n] = 0;
	}
	//���ػ������ĳ���
	wstring strRlt = buffer;
	delete[] buffer;

	return strRlt;
}

size_t CCharSetHelper::c2w(const string& strIn, wstring& wstrOut)
{
	if(strIn.empty())
	{
		wstrOut = L"";
		return 0;
	}

	wchar_t* buffer = NULL;
	size_t n = 0;
	if(strIn.size() > 0)
	{
		size_t nu = strIn.size();
		n =(size_t)MultiByteToWideChar(CP_ACP,0,(const char *)strIn.c_str(),int(nu),NULL,0);
		buffer=0;
		buffer = new wchar_t[n + 1];
		//if(n>=len) n=len-1;
		::MultiByteToWideChar(CP_ACP,0,(const char *)strIn.c_str(),int(nu),buffer,int(n));    
	
		//������
		buffer[n] = 0;
	}
	//���ػ������ĳ���
	wstrOut = buffer;
	//delete[] buffer;
	return n;
}

/************************************************************************/
/* 
	ʹ��ʾ��                                                                     
*/
/************************************************************************/
/*
int _tmain(int argc, _tchar* argv[])
{
	wchar_t pwstr[] =l"�����й���";
	wchar_t pwstr2[20];
	char *pcstr = (char *)malloc(sizeof(char)*(2 * wcslen(pwstr)+1));
	memset(pcstr , 0 , 2 * wcslen(pwstr)+1 );
	w2c(pcstr,pwstr,2 * wcslen(pwstr)+1) ;
	printf("%s\n",pcstr);
	c2w(pwstr2,20,pcstr);
	wprintf(l"%s",pwstr2);
	free(pcstr) ;
	return 0;
}
*/

/************************************************************************/