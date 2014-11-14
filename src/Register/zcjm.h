/**
 * ��	������������
 * 	1. û���õ�����
 * 	2. ���ܹ���
 * 		2.1������mac��ϵͳ��Ϣ�õ� ������
 * 		2.2	���ݻ���������ע����
 *		2.3 	��ע����д���ļ�
 *	3. �ж��Ƿ����
 *		3.1 ��̬����ע���� 
 * 		3.2 ��ȡע����
 * 		3.3 �Ա�����ע���룬һ��˵���м���
 * ʱ�����ߣ���created by yjt 2012-08-29
 */

#ifndef _ENCRYPT_RC4_
#define _ENCRYPT_RC4_

#include <string>
#include <sys/stat.h>
using namespace std;

#define BOX_LEN 256

/**
 * ��	����	���ܲ�����
 * 	1. ��ֲwindow�Ľ��ܴ���
 * ʱ�����ߣ� created by yjt 2012-08-29
 */
class zcjm {
private:
    zcjm();

public:
    /**
     * ��	����	��ȡ������
     * 		1. ��ȡMAC
     * 		2. ��ȡȡϵͳ��Ϣ
     * 		3. �����ֺϳɻ�����
     * �� �� ֵ�� ��ȡ�ɹ����ػ����룬ʧ�ܷ���""
     * ʱ�����ߣ� created by yjt 2012-08-29
     */
    static string GetMachineCode();
    
    /**
     * ��	����	��ȡע����
     * ��	����	strMichineCode [IN]	--	�����루GetMichineCode���ص�)
     * 			strPass [IN]			--	������,���Ϊ����ϵͳĬ�ϵļ�����
     * �� �� ֵ��  ����ע����, �������ʧ�ܷ���""
     * ʱ�����ߣ� created by yjt 2012-08-29
     */
    static string GetRegisterCode(const string& strMichineCode, const string& strPass="");
    
    /**
     * ��	����	ע��
     * 		1. ��ע����д���ļ�
     * 		2. ��ע�����뵱ǰ����Ӳ����Ϣ���ɵ�ע����Ϣ���бȽ�
     * 			2.1 �ѱȽϽ�����
     * ��	����	strRegisterCode [IN]		--		ע����
     * �� �� ֵ��	ע��ɹ�����true, ʧ�ܷ���false
     * ʱ�����ߣ� created by yjt 2012-08-29
     */
    static bool Register(const string& strRegisterCode);
    
    /**
     * ��	����	�����Ƿ���ע��
     * 		1. ���ļ��ж�ȡע����
     * 		2. ��ע�����뵱ǰ����Ӳ����Ϣ���ɵ�ע����Ϣ���бȽ�
     * 			2.1 �ѱȽϽ�����
     * �� �� ֵ��	�Ѿ�ע�᷵��true, û�з���false
     * ʱ�����ߣ� created by yjt 2012-08-29
     */
    static bool IsRegister();

protected:
    /**
     * ����
     */
    static string jiami(const string& szSource, const string& szPassWord);    
    
     /**
     * ���ܣ����ؽ��ܽ��
     */
    static string jiemi(const string& szSource, const string& szPassWord);

    /**
     * ���� -- ���Ӳ����Ϣ
     */
    static void decrypter(char *a);

    /**
     * ���� -- ���Ӳ����Ϣ
     */
    static void encrypter(char* a);
    
private:
    /**
     * �õ���ֵ--���������
     */
    static int GetKey(const unsigned char* pass, int pass_len, unsigned char* out);

    /**
     * 
     */
    static int RC4(const unsigned char* data, int data_len, const unsigned char* key, int key_len,
		   unsigned char* out, int& out_len);

    /**
     * �����ֽ�
     */
    static void swap_byte(unsigned char* a, unsigned char* b);

    /**
     * ���ֽ���pbBufferתΪʮ�������ַ��������㴫��
     */
    static string ByteToHex(unsigned char* vByte, int vLen);

    /**
     * ��ʮ�������ַ���תΪ�ֽ���pbBuffer������
     */
    static int HexToByte(const string& szHex, unsigned char** vByte);

    /**
     *	��ȡϵͳ��Ϣ
     */
    static string GetSysInfo(void);
    
    /**
     *	��ȡ����MAC��Ϣ
     */
    static string GetMACInfo(void);

	static string GetCpuID(void);

	static CTime zcjm::String2Time(string&  strS);
};

#endif // #ifndef _ENCRYPT_RC4_
