//////////////////////////////////////////////////////////////////////////
//�����ļ�����
//////////////////////////////////////////////////////////////////////////
#pragma once

class ConfigBase
{
public:
	ConfigBase(void);
	virtual ~ConfigBase(void);

	virtual void Init();
	virtual void InitFromFile();

	virtual void SaveToFile();

	//����
	bool BackUp();
	//�ָ�
	bool Restore();

protected:
	virtual CString GetConfigPath();
	virtual CString GetConfigBackUpPath();

protected:
	CString GetAppPath();
};

