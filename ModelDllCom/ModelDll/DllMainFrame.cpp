// DllMainFrame.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ModelDll.h"
#include "DllMainFrame.h"
#include "afxdialogex.h"
#include "common.h"
#include "YSerialDevice.h"


// CDllMainFrame �Ի���

IMPLEMENT_DYNAMIC(CDllMainFrame, CDialogEx)

CDllMainFrame::CDllMainFrame(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDllMainFrame::IDD, pParent)
{
	y_pDevice = NULL;
}

CDllMainFrame::~CDllMainFrame()
{
}

void CDllMainFrame::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDllMainFrame, CDialogEx)
	ON_WM_CLOSE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDllMainFrame ��Ϣ�������


void CDllMainFrame::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
//	KillTimer(MY_TIMER);
	CDialogEx::OnClose();
}


void CDllMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if ((MY_TIMER == nIDEvent))
	{
		if (GetDevice()!=NULL)
		{
			GetDevice()->OnUpdate();
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}

void* CDllMainFrame::NewDevice(LPCTSTR pszAppPath)
{
	if (GetDevice()==NULL)
	{
		try{
			y_pDevice = new YSerialDevice(pszAppPath);
		}
		catch(CFileException* e){
#if defined(_DEBUG)
			e->ReportError();
#endif
			e->Delete();
			return NULL;
		}
		return (void*)y_pDevice;
	}
	return NULL;
}

void CDllMainFrame::DeleteDevice(void *pDevice)
{
	if (GetDevice()!=NULL && GetDevice()==pDevice)
	{
		delete y_pDevice;
		y_pDevice = NULL;
	}
}

bool CDllMainFrame::RunDevice()
{
	if (GetDevice() != NULL)
		GetDevice()->BeginUpdateThread();
	return true;
}

void CDllMainFrame::StopDevice()
{
	if (GetDevice() != NULL)
		GetDevice()->EndUpdateThread();
}
