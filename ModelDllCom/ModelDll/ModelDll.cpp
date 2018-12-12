// ModelDll.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "ModelDll.h"
#include "common.h"
#include "DeviceApi.h"
#include "YOPCItem.h"
#include "YOPCDevice.h"
#include "YSerialDevice.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CModelDllApp

BEGIN_MESSAGE_MAP(CModelDllApp, CWinApp)
END_MESSAGE_MAP()


// CModelDllApp ����

CModelDllApp::CModelDllApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
	pFrame = NULL;
}


// Ψһ��һ�� CModelDllApp ����

CModelDllApp theApp;


// CModelDllApp ��ʼ��

BOOL CModelDllApp::InitInstance()
{
	CWinApp::InitInstance();
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	pFrame = new CDllMainFrame;
	pFrame->Create(CDllMainFrame::IDD,NULL);
	return TRUE;
}

int CModelDllApp::ExitInstance()
{
	// TODO: �ڴ����ר�ô����/����û���
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	SendMessage(GetMainWindow()->m_hWnd,WM_CLOSE,0,0);
	if(pFrame)
		delete pFrame;
	return CWinApp::ExitInstance();
}

void* CModelDllApp::NewDevice(LPCTSTR pszAppPath)
{
	CString strPath;
	GetAppPath(strPath);
	return pFrame->NewDevice(pszAppPath);
}

void CModelDllApp::DeleteDevice(void *pDevice)
{
	return pFrame->DeleteDevice(pDevice);
}


bool CModelDllApp::RunDevice()
{
	return pFrame->RunDevice();
}

bool CModelDllApp::StopDevice()
{
	pFrame->StopDevice();
	return true;
}

bool CModelDllApp::ShowItemBrowseDlg(CObArray* pDevItemArray)
{
	return pFrame ? pFrame->ShowItemBrowseDlg(pDevItemArray) : false;
}

bool CModelDllApp::GetArrayItemByName(CObArray *pItemOutArray, const CStringArray *pStringArray)
{
	if(! pFrame) return false;
	if(pItemOutArray == NULL)
	{
		return false;
	}
	const YOPCDevice* pDevice = GetMainWindow()->GetDevice();
	if( pDevice == NULL ) return false;

	const CMapStringToOb* pItemArray = pDevice->GetItemArray();

	if(pItemArray->IsEmpty()) return false;
	if(pStringArray == NULL)
	{
		//��������ֵ
		pItemOutArray->SetSize(pItemArray->GetCount());
		POSITION pos = pItemArray->GetStartPosition();
		YOPCItem* pItem = NULL;
		CString strName;
		while(pos){
			pItemArray->GetNextAssoc(pos,strName,(CObject*&)pItem);
			if(pItem)
				pItemOutArray->Add((CObject*)pItem);
		}
	}
	else
	{
		//�����ṩ�����ӷ���
		YOPCItem* pItem = NULL;
		for(INT_PTR i=0;i<pStringArray->GetCount();i++)
		{
			if(pItem=pDevice->GetItemByName((*pStringArray)[i]))
			{
				pItemOutArray->Add(static_cast<CObject*>((void*)pItem));
			}
		}
	}
	return true;
}

bool CModelDllApp::AddDllMenu(CMenu* pMenu)
{
	return false;
}

void CModelDllApp::OnCommand(UINT nID)
{
}

bool CModelDllApp::GetAppPath(CString& strPath)const
{
	char szPath[MAX_PATH];
	CString strTmp;
	if (!GetModuleFileName(m_hInstance, szPath, MAX_PATH))return false;
	strTmp=_T(szPath);
	int nPos = strTmp.ReverseFind(_T('\\'));
	if (nPos < 1)return false;
	strPath = strTmp.Left(nPos);
	return true;
}

bool CModelDllApp::SetDeviceItemValue(CBaseItem* pItem)
{
	return pFrame ? pFrame->SetDeviceItemValue(pItem) : false;
}


DEVICE_API void* ht_NewDevice()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString strPath;
	GetAppPath(strPath);
	return theApp.NewDevice(strPath);
}

DEVICE_API void ht_DeleteDevice(void *pDevice)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	theApp.DeleteDevice(pDevice);
}

DEVICE_API WORD ht_GetDeviceVersion()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	int nMajorVer = 1;//���汾��
	int nMinorVer = 0;//���汾��
	WORD wVersion = MAKEWORD(nMajorVer,nMinorVer);
	return wVersion;
}

//
DEVICE_API bool ht_GetDeviceName(CString *pName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString strDeviceName;
	strDeviceName.LoadString(IDS_DEVICENAME);
	*pName = strDeviceName;
	return true;
}

//
DEVICE_API HICON ht_GetDeviceIcon()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	HICON hIcon = NULL;
	// 	AFX_MANAGE_STATE(   AfxGetStaticModuleState()   );   
	// 	hIcon = LoadIcon(theApp.m_hInstance,MAKEINTRESOURCE(IDI_OPCSAMPLEDLL));
	return hIcon;
}
//
DEVICE_API HWND ht_ShowProDlg(VOID *pSetting /*= NULL*/)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return NULL;
	//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	//	return theApp.ShowSettingDlg(pSetting);	
}

DEVICE_API LPVOID ht_SaveDeviceSetting()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return NULL;
}

DEVICE_API bool ht_RunDevice()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return theApp.RunDevice();
}

DEVICE_API bool ht_StopDevice() 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return theApp.StopDevice();
}


DEVICE_API bool ht_ShowItemBrowseDlg(CObArray /*[in out]*/*pDevItemArray)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return theApp.ShowItemBrowseDlg(pDevItemArray);
}

//
DEVICE_API bool ht_GetArrayItemByName(CObArray *pItemOutArray, const CStringArray *pStringArray)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return theApp.GetArrayItemByName(pItemOutArray,pStringArray);
}	


DEVICE_API bool ht_AddDllMenu(CMenu *pMenu)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	ASSERT(theApp.GetMainWindow() != NULL);
	return theApp.AddDllMenu(pMenu);
}

/*
�˺����ṩ����DLL�豸����������ע��Ĳ˵���Ĳ���
*/
DEVICE_API void ht_OnCommand(UINT nID)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	ASSERT(theApp.GetMainWindow() != NULL);
	theApp.OnCommand(nID);
}


DEVICE_API bool ht_SetDeviceItemValue(LPVOID /*[in]*/pItem)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(pItem == NULL) return false;
	return theApp.SetDeviceItemValue(static_cast<CBaseItem*>(pItem));
}

//////////////////////////////////////////////
//����API

bool GetAppPath(CString& strPath)
{
	return theApp.GetAppPath(strPath);
}