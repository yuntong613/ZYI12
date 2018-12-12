
#ifndef DEVICEAPI_H
#define DEVICEAPI_H
/*
* Copyright (c) 2004,�廪ͬ�����޹�˾eHome�о�Ժ
* All rights reserved.
* 
* �ļ����ƣ�DeviceApi.h
* �ļ���ʶ�������ù���ƻ���
* ժ    Ҫ���豸�����豸�е�TreeView
* 
* ��ǰ�汾��1.5
* ��    �ߣ������
* ������ڣ�2005��5��10��
*
* ȡ���汾��1.0 
* ԭ����  �������
* ������ڣ�2004��12��10��
*/

//DLLҪʵ�ֺ����Ķ���

/*''''''''''''''''''''''''''''''''''''''''''''''

//	��DLL��Ҫʵ�ִ������ʾ�������
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());

''''''''''''''''''''''''''''''''''''''''''''''''*/
//*******************
//1.5���º���
//*******************
/*
//����DLL�İ�
DEVICE_API WORD ht_GetDeviceVersion()
{
	int nMajorVer = 2;//���汾��
	int nMinorVer = 0;//���汾��
	WORD wVersion = MAKEWORD(nMajorVer,nMinorVer);
	return wVersion;
}
*/

DEVICE_API void* ht_NewDevice();

DEVICE_API void ht_DeleteDevice(void *pDevice);

DEVICE_API WORD ht_GetDeviceVersion();


//---------------------------------------
/*
�˺����ṩDLL��ʵ���豸������
��������������ָ�롣

��: *pName = "Simulate Device";
return true;
*/
DEVICE_API bool ht_GetDeviceName(CString /*[out]*/*pName);


//*******************
//1.5���º���
//*******************
/*
*/
DEVICE_API HICON ht_GetDeviceIcon();


//*******************
//1.5���º���
//*******************
//
DEVICE_API bool ht_GetArrayItemByName(CObArray *pItemOutArray, const CStringArray *pStringArray = NULL);

//---------------------------------------
/*	
ht_ShowProDlg
�˺����ṩDLL���豸�Ĳ������õĴ���
��:
AFX_MANAGE_STATE(AfxGetStaticModuleState());
//������Ҫʵ�ַ�ģʽ�Ĵ���.����ҪCreate����
//��Ϊ�˴�������ʾ�����������豸�����
//����Ҫ�Դ�������Խ�����������:
//	Border = None
//	Stytle = Child

pSettingDlg = new CSettingDlg();
ASSERT(pSettingDlg);
if(!pSettingDlg->Create(CSettingDlg::IDD))
return NULL;
pSettingDlg->ShowWindow(SW_SHOW);
pSettingDlg->UpdateWindow();

return m_pSettingDlg->m_hWnd;	

*/
DEVICE_API HWND ht_ShowProDlg(VOID  /*[in out]*/*pSetting = NULL);


//���������Ϣ�Ƿ�������ȷ
DEVICE_API bool ht_CheckSetting();

/*
�˺����ṩDLL���豸�������õı��棬����DLL�У���
��DLL��ʹ��
*/
DEVICE_API LPVOID ht_SaveDeviceSetting();

/*
�˺����ṩDLL���豸�����С��磺���ӵ���Ӧ��Ӳ���豸
�ϡ�ʹDLL�����������������豸�߳�
*/
DEVICE_API bool ht_RunDevice();

/*
�˺����ṩDLL���豸��ֹͣ����ֹͣ�豸�߳�
*/
DEVICE_API bool ht_StopDevice();

/*
�˺����ṩDLL����ʾ�豸������Ĵ��塣����������ݵ�
*/
DEVICE_API bool ht_ShowItemBrowseDlg(CObArray /*[in out]*/*pDevItemArray);

/*
ע��:DLL�˵� ID�ķ�Χ��20500��20900����400��
�˺����ṩDLL�豸��������Ĳ˵���ע����豸�Ĳ˵���
*/
DEVICE_API bool ht_AddDllMenu(CMenu /*[in out]*/*pMenu);

/*
�˺����ṩ����DLL�豸����������ע��Ĳ˵���Ĳ���
*/
DEVICE_API void ht_OnCommand(UINT /*[in]*/nID);

/*
ƽ̨��DLL����дֵ����
������CBaseItem��ָ�룬���ڱ���Ҫ���������ֵ�͸�����Ϣ
*/
DEVICE_API bool ht_SetDeviceItemValue(LPVOID /*[in]*/);

#endif