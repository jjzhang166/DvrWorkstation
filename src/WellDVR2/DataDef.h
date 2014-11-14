#pragma once

#include <vector>
#include <map>
using namespace std;

#include "mmsystem.h"				//��������ͷ�ļ�
#pragma comment(lib,"winmm.lib")	//��������ͷ�ļ���

/////////////////////////////////////////////////////////////////////////////////////
//�궨��
/////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////
//���嵥��ģʽ
//#define ONE_DVR
#ifdef ONE_DVR

#define ROWS	1
#define COLS	1
#define DVR_TOTAL_NUM	ROWS*COLS

#define DVR_INSTANCE _T("WellDvrOne")
#else

#define ROWS	4
#define COLS	4
#define DVR_TOTAL_NUM	ROWS*COLS

#define DVR_INSTANCE _T("WellDvr")
#endif
////////////////////////////////////////////////////////
//�Ƿ�����ָ��
#define FINGER_ENABLE  1


#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }

//V1.00.10 �Ǿɽ���汾
//V1.01.00 ���½���汾���½������м�汾������
//V1.01.04 ���̿ռ䲻�����������
//V1.01.05 �����A4,X6,D6��֧��
//V1.02.01 �������µ�Hub��ɨ�跽ʽ
//V1.02.02 ������һЩСbug
//V1.02.03 ��������ָ�����У����豸�ͺ��ᵽ�����ļ���
//				 ע:����Ҳ�������ý��(Win32)
//V1.02.05 �������ý��(MFC)��monitor
//V1.02.06 �޸����ڴ�й¶�;��������������
//V1.03.01 �����ָ���Ǻ�����ͷ
#define SOFT_VERSION	_T("�汾:V1.02.06")

#define COMPANY _T("�����������������޹�˾")
#define PHONE _T("�ͷ�:4000602856")

#define BK_COLOR RGB(255, 50, 50)

#define DVR_BK_ACTIVE	RGB(255, 255, 0)
#define DVR_BK_NORMAL	RGB(0, 255, 255)
#define DVR_BK_DISABLE	RGB(128, 128, 128)

#define TEXT_COLOR		RGB(206, 34, 176)
//#define TEXT_COLOR		RGB(255, 0, 0)

#define DISP_MSG_CNT	30

#define WM_NOTIFYDATA           WM_USER + 3000
#define IDM_SHOW                WM_NOTIFYDATA + 1
#define IDM_EXIT                WM_NOTIFYDATA + 2
#define IDM_ABOUT             WM_NOTIFYDATA + 3

#define WM_NOTIFYID             WM_NOTIFYDATA + 20

#define CMD_VID_PID _T("VID_0595&PID_4343")
#define MASS_VID_PID _T("VID_0595&PID_2002")

#define APP_NAME _T("WellDVR2.exe") 
/////////////////////////////////////////////////////////////////////////////////////
//��������
/////////////////////////////////////////////////////////////////////////////////////
#ifdef ONE_DVR
const CString gc_AppName(_T("ִ�����ݲɼ��ն� - ���˰�"));
#else
const CString gc_AppName(_T("ִ�����ݲɼ��ն�"));
#endif

const CString gc_Config(_T("\\dvr_cfg.ini"));
const CString gc_LocationConfig(_T("\\location.xml"));
const CString gc_LocationConfig_BackUp(_T("\\location.back"));
const CString gc_Config_BackUp(_T("\\..\\dvr_cfg.back"));
const CString gc_DefaultSavePath(_T("d:\\well-ftp"));

const CString gc_configApp(_T("\\WellDVRConfig.exe"));
const CString gc_GetMachineApp(_T("\\GetMachineCode.exe"));
const CString gc_RegisterApp(_T("\\RegisterWnd.exe"));

//��С�Ŀռ�����
const UINT gc_MinSpaceLimit = 500;

//������ʱ��
const UINT gc_TimerId_CopySpeed = 0;
const UINT gc_TimerSpace_CopySpeed = 1000;

/////////////////////////////////////////////////////////////////////////////////////
//ö�ٶ���
/////////////////////////////////////////////////////////////////////////////////////
enum{ 
	COPY_OK = 0, 
	COPY_STOP, 
	COPY_SPACE_LIMIT,
	COPY_UNKNOWN 
};

enum EM_DEV_COLOR{
	//��ɫ����
	CLR_DISABLE_BK = 0x00c0DCC0,
	CLR_DISABLE_BORDER = 0x000000FF,

	CLR_NOMAL_BK = 0x0DC01330,
	CLR_NOMAL_BORDER = 0x0000FFFF,

	CLR_ACTIVR_BK = 0x72C02CC0,
	CLR_ACTIVR_BORDER = 0xFF000000,

	CLR_DERICT_BORDER = 0x00FFFF00,
	CLR_DERICT_BK = 0x7E67787F,

	CLR_COPY_BORDER = 0x00FFFF00,
	CLR_COPY_BK = 0x7E67787F,

	CLR_COPY_OK_BORDER = 0x72C02CC0,
	CLR_COPY_OK_BK = 0x68CC3CE2,

	CLR_STOP_BORDER = 0x00FFFF00,
	CLR_STOP_BK = 0x7E67787F,

	CLR_MSG_ADD_DEV = 0x000000FF,
	CLR_MSG_DEL_DEV	= 0x00FF0000
};

//�������� 
enum EM_WORK_LEVEL{
	LOCATION = 1,				//��λģʽ
	WORKING						//����ģʽ
};

enum EM_DVR_TYPE
{
	DVR_TYPE_WELL_34 = 0,					//WELL 3�� 4��DVR
	DVR_TYPE_HA,									//������WELL 2��)
	DVR_TYPE_SHLR_A34,						//�Ϻ����� A3/A4
	DVR_TYPE_SHLR_D6,						//�Ϻ����� D6
	DVR_TYPE_SHLR_X6							//�Ϻ����� X6
};

/////////////////////////////////////////////////////////////////////////////////////
//�ṹ����
/////////////////////////////////////////////////////////////////////////////////////
typedef struct tsDVRPROP
{
	int numDvr;
	int numRow;
	int numCol;
} DVRPROP, *PDVRPROP; 

typedef enum teDVRSTATUS
{
	DVR_ACTIVE = 0,			//����״̬�������ʶ��ɹ���
	DVR_NORMAL,				//����״̬
	DVR_DISABLE,			//�����ã��γ�״̬��
	DVR_COPY,				//���ڿ���
	DVR_COPY_OK,			//�������
	DVR_STOP,				//ֹͣ����
	DVR_DERICT				//��λģʽ
} DVRSTATUS, *PDVRSTATUS;

typedef struct tsDVRITEMPROP
{
	CRect rcPos;
	DVRSTATUS status;
} DVRITEMPROP, *PDVRITEMPROP;

typedef struct tsVIDPID
{
	USHORT idVendor;
	USHORT idProduct;
} VIDPID, *PVIDPID;

typedef struct tsDVRIDNAME
{
	CString strID;
	CString strName;
} DVRIDNAME, *PDVRIDNAME;

//////////////////////////////////////////////////////////////////////////
//�����ļ�
//FTP��Ϣ
typedef struct tagFTPInfo{
	bool bEnable;
	CString strServ;
	UINT nPort;
	CString strUser;
	CString strPwd;
	bool bTransIMP;
	bool bTransHD;
	bool bTransSD;
	bool bDelAfterTrans;
}FTPInfo, *PFTPInfo;

//���ݿ���Ϣ
typedef struct tagDBInfo{
	bool bEnable;
	CString strHost;
	UINT nPort;
	CString strSid;
	CString strUser;
	CString strPwd;
	long lConnTime;
}DBInfo, *PDBInfo;

//�ɼ��ն��豸��Ϣ
typedef struct tagDeviceInfo
{
	CString strId;
}DeviceInfo, *PDeviceInfo;

typedef struct tagWebInfo
{
	CString strIp;
	int nPort;
}WebInfo, *PWebInfo;

typedef struct tagDvrWndInfo{
	CString m_strId;			//ID
	CString m_strName;			//NAME
	CString m_strDisk;			//DiskName
	DVRSTATUS m_status;			//״̬
	//״̬��ʾ�ַ���
	CString m_strStatus;
	int nFileCount;						//���ļ���
	int nTransFileCount;				//�т�ݔ�ļ���
}DvrWndInfo, *PDvrWndInfo;

typedef struct tagSoftWareInfo
{
	CString strPhone;
	CString strCpy;
	CString strVersion;
}SoftWareInfo, *PSoftWareInfo;

typedef struct tagSettingInfo{
	DVRPROP dvrProp;
}SettingInfo, *PSettingInfo;
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//���ݿ���Ӧ��ʵ����
//////////////////////////////////////////////////////////////////////////
typedef struct tagDVRMEDIA_INFO
{
	wstring strDvrNumber;		//-- DVR���
	wstring strFileName;		//--�ļ���
	wstring strFileSize;		//--�ļ���С
	wstring strViewWidth;		//-- ��
	wstring strViewHeight;		//-- ��
	wstring strMediaDuration;	//--ʱ��
	wstring strFrameRate;		//--֡��
	wstring strDataUploadTime;	//--�ϴ�ʱ��
	wstring strMediaStartTime;	//--��ʼʱ��
	wstring strMediaEndTime;	//--����ʱ��
	wstring strRecordTime;		//-- ¼��ʱ��
	wstring strFileType;		//-- �ļ�����
	wstring strVideoType;		//-- �ص�/����/����
	wstring strLocateFilePath;	//--�����ļ�·��
	wstring strFtpFileUrl;		//--ftp�ļ�·��
	wstring strIISFileUrl;		//--iis�ļ�·��
	wstring strDataIp;			//--���ݴ洢ip
	wstring strRemark;			//--��ע
}DVRMEDIA_INFO, *PDVRMEDIA_INFO;

typedef struct tagDVR_DEVICE_LOG{
	wstring strDvrNumber;			//--dvr���
	wstring strDeviceInsertTime;	//--�豸����ʱ��
	wstring strDeviceOperTime;		//--�豸����ʱ��
	wstring strDeviceOperUser;		//--�豸�����û�
	wstring strLogContent;			//--��־����
	wstring strRemark;				//--��ע
}DVR_DEVICE_LOG,*PDVR_DEVICE_LOG;

typedef struct tagTERMINAL_DEV_LOG{
	wstring strDeviceNumber;		//--�豸��
	wstring strHarewareVersion;		//--Ӳ���汾
	wstring strSoftwareVersion;		//-����汾
	wstring strDiskSpace;			//--���̿ռ�
	wstring strTerminalIp;			//--IP
	wstring strIsusing;				//--�Ƿ����� -Y�� -N��
	wstring strDetachment;			//--֧��
	wstring strBattalion;			//--���
	wstring strSquadron;			//--�ж�
	wstring strDescribe;			//--����
	wstring strRemark;				//--��ע
}TERMINAL_DEV_LOG, *PTERMINAL_DEV_LOG;

typedef struct tagTERMINAL_STATE{
	wstring strDeviceNumber;		//--�豸��
	wstring strReportTime;			//--����ʱ��
	wstring strDeviceState;			//--�豸״̬
	wstring strRemark;				//--��ע
}TERMINAL_STATE, *PTERMINAL_STATE;