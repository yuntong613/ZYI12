#pragma once

#include "YOPCDevice.h"

// CDllMainFrame �Ի���

class CDllMainFrame : public CDialogEx
{
	DECLARE_DYNAMIC(CDllMainFrame)

public:
	CDllMainFrame(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDllMainFrame();
	YOPCDevice* y_pDevice;
// �Ի�������
	enum { IDD = IDD_DLLFRAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	void* NewDevice(LPCTSTR pszAppPath);
	void DeleteDevice(void *pDevice);
public:
	bool RunDevice();
	void StopDevice();
	YOPCDevice* GetDevice()const{ return y_pDevice;}
	bool ShowItemBrowseDlg(CObArray* pDevItemArray)
	{ return(GetDevice()!=NULL)?GetDevice()->ShowItemBrowseDlg(pDevItemArray):false;}
	bool SetDeviceItemValue(CBaseItem* pAppItem)
	{ return (GetDevice()!=NULL)?GetDevice()->SetDeviceItemValue(pAppItem):false;}

	const CMapStringToOb* GetItemArray()const{ return (GetDevice()!=NULL)?GetDevice()->GetItemArray():NULL;}

	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
