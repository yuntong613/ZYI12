// ModelDll.h : ModelDll DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "DllMainFrame.h"
#include "BaseItem.h"
#include "BaseDevice.h"
// CModelDllApp
// �йش���ʵ�ֵ���Ϣ������� ModelDll.cpp
//

class CModelDllApp : public CWinApp
{
public:
	CModelDllApp();
	CDllMainFrame* pFrame;
// ��д
public:
	virtual BOOL InitInstance();
	CDllMainFrame* GetMainWindow(){return pFrame;}
	void* NewDevice(LPCTSTR pszAppPath);
	void DeleteDevice(void *pDevice);

	bool RunDevice();
	bool StopDevice();

	bool ShowItemBrowseDlg(CObArray* pDevItemArray);
	bool GetArrayItemByName(CObArray* pItemOutArray,const CStringArray* pStringArray = NULL);

	bool AddDllMenu(CMenu* pMenu);
	void OnCommand(UINT nID);
	bool SetDeviceItemValue(CBaseItem* pAppItem);
	bool GetAppPath(CString& strPath)const;


	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};
