// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� REGISTER_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// REGISTER_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef REGISTER_EXPORTS
#define REGISTER_API __declspec(dllexport)
#else
#define REGISTER_API __declspec(dllimport)
#endif

// �����Ǵ� Register.dll ������
class REGISTER_API CRegister {
public:
	CRegister(void);
	// TODO: �ڴ�������ķ�����
};

extern REGISTER_API int nRegister;

REGISTER_API int fnRegister(void);
