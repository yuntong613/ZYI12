/*
* Copyright (c) 2004,�廪ͬ�����޹�˾eHome�о�Ժ
* All rights reserved.
* 
* �ļ����ƣ�BaseDevice.cpp
* �ļ���ʶ�������ù���ƻ���
* ժ    Ҫ���豸������
* 
* ��ǰ�汾��2.0
* ��    �ߣ������
* ������ڣ�2004��3��20��
*
* ȡ���汾��1.0 
* ԭ����  �������
* ������ڣ�2004��1��10��
*/

// BaseDevice.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "BaseDevice.h"



//IMPLEMENT_SERIAL(CBaseDevice,CObject,1)


// CBaseDevice

CBaseDevice::CBaseDevice()
{
	m_strDeviceName = "Unname";
	m_bUpdate	 = FALSE;
	m_lRate      = 5000;//ms
	m_strDeviceDescription = "unKnow";
}

CBaseDevice::~CBaseDevice()
{

}


// CBaseDevice ��Ա����

void CBaseDevice::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	
		ar << m_strDeviceName;
		ar << m_lRate;		
		ar << m_bUpdate;      
		ar << m_strDeviceDescription;

	}
	else
	{	
		ar >> m_strDeviceName;
		ar >> m_lRate;		
		ar >> m_bUpdate;      
		ar >> m_strDeviceDescription;
	}

}


