#pragma once
class CFtpObj
{
public:
	CFtpObj(void);
	~CFtpObj(void);

public:
	//�Ƿ�����
	bool m_bEnable;
	//IP
	CString m_strIp;
	//�˿�
	CString m_strPort;
	//�û�
	CString m_strUser;
	//����
	CString m_strPwd;
	//������Ҫ��Ƶ
	bool m_bTransImp;
	//���������Ƶ
	bool m_bTransHd;
	//���������Ƶ
	bool m_bTransSd;

	//������Ƿ�ɾ��
	bool m_bDelAfterTrans;
};

