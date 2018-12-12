/********************************************************************
	created:	2004/05/07
	created:	7:5:2004   14:38
	filename: 	e:\sourcecode\mfc\newopcserver\comm\common.h
	file path:	e:\sourcecode\mfc\newopcserver\comm
	file base:	common
	file ext:	h
	author:		Hotyoung
	
	purpose:	�������������DLLͨ�õĶ���
*********************************************************************/
#ifndef COMMON_H
#define COMMON_H


/////////////////////////////////////////////////////////
//�궨��

#define DEVICE_API extern "C"  __declspec(dllexport)
//Dll�˵�ID�Ŀ�ʼID
#define DLL_MENU_ID		20500
#define DLL_BEGIN_UPDATE		DLL_MENU_ID + 1
#define DLL_END_UPDATE			DLL_MENU_ID + 2

/////////////////////////////////////////////////////////
#define GET_DLL_PROC(DllName,APIVAR,APITYPE,APINAME)			\
		HINSTANCE hDll = AfxLoadLibrary(DllName);				\
		ASSERT(hDll);											\
		APITYPE APIVAR = (APITYPE)GetProcAddress(hDll,APINAME); \
		ASSERT(APIVAR)	

////////////////////////////////////////////////////////////
#define FREE_DLL(hDll) AfxFreeLibrary(hDll)
///////////////////////////////////////////////////////////

#define MAX_BUFF_LEN	2048

//������
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
//
#if defined(_INTERIM_RELEASE) || defined(_DEBUG)
#define OPC_TRY
#define OPC_CATCH
#define OPC_CATCH2
#else
#define OPC_TRY try{
#define OPC_CATCH }\
catch(CString &str){ try{LogError(GetResString(IDS_EXCEPTION_MSG1),__FUNCTION__,__FILE__, __LINE__,str);}catch(...){} }\
catch(CException *err){ try{  TCHAR  szCause[255];   err->GetErrorMessage(szCause, 255);\
LogError(GetResString(IDS_EXCEPTION_MSG1),__FUNCTION__,__FILE__, __LINE__,szCause);err->Delete();}catch(...){} }\
catch(...){ try{LogError(GetResString(IDS_EXCEPTION_MSG1),__FUNCTION__,__FILE__, __LINE__,_T(""));}catch(...){} }
#define OPC_CATCH2 } catch(...){}
#endif

//��������Ϣ����

//������и���
#define CW_UPDATEITEMVALUE WM_USER+1011 
/*
 ���� WPARAM wParam ��������ĸ��±��
	  LPARAM lParam ���CBaseItem���͵�ָ��
	  �û���Ҫ����Ϊ��ָ������ڴ�
	  ����ɹ�����0
*/

//һ�����ݽ��и���
#define CW_UPDATEGROUPITEM (CW_UPDATEITEMVALUE + 1) 
/*
 ���� WPARAM wParam ��������ĸ��±��
	  LPARAM lParam ��Ű���CBaseItem����CPtrArray��ָ��
	  �û���Ҫ����Ϊ��ָ������ڴ�
	  ����ɹ�����0
*/

//���±�Ƕ��� ��ΪWPARAM����
#define DIC_ITEM_UPDATE  0xFF01 //�豸��ֵ�ñ仯
#define DIC_ITEM_DELEATE 0xFF02	//�豸�ɾ��
#define DIC_ITEM_INVALID 0xFF03 //Щ��ΪЧ��

#define MY_TIMER 2013

#ifdef _MAIN_PROGRAMER

//�������ж����ʶ
#define HINT_LOAD_PROJECT		1060
#define HINT_CLOSE_PROJECT		1061
#define HINT_REFRESH_GROUPVIEW	1062
#define HINT_REFRESH_ITEMVIEW	1063
//DLL�Ĳ˵���Χ
#define MIN_DLL_MENU_ID			DLL_MENU_ID
#define MAX_DLL_MENU_ID			DLL_MENU_ID + 400
#else
//////////////////////////////////////////////////////////////////
//ȡ������������
#define GET_EXE_HWND(hWnd)										\
		{														\
			typedef HWND (*GETTHEWND)();						\
			GETTHEWND GetTheWnd = (GETTHEWND)GetProcAddress(GetModuleHandle(""),"mn_GetMainWnd");\
			ASSERT(GetTheWnd != NULL);							\
			(hWnd) = GetTheWnd();								\
		}														\
//////////////////////////////////////////////////////////////////

bool GetAppPath(CString& strPath);

#endif
#include <atlbase.h>
#endif