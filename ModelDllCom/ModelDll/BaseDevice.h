#pragma once
#ifndef BASEDEVICE_H
#define BASEDEVICE_H

// CBaseDevice ����Ŀ��

class CBaseDevice : public CObject
{
//public:
	//DECLARE_SERIAL(CBaseDevice)
public:
	CBaseDevice();
	virtual ~CBaseDevice();
protected:
	CString m_strDeviceName;//�豸����
	LONG	m_lRate;		//�����ʣ�ms��
	BOOL	m_bUpdate;      //�Ƿ���и��¡������Ƿ������߳�
	CString m_strDeviceDescription;
public:
	//�����豸����
	void SetDeviceName(CString strDeviceName){ m_strDeviceName = strDeviceName; }
	void SetDeviceDesc(CString strDeviceDesc){ m_strDeviceDescription = strDeviceDesc; }
	//ȡ���豸����
	CString GetDeviceName()	{ return m_strDeviceName;}
	CString GetDeviceDesc()	{ return m_strDeviceDescription;}
	//ˢ������
	void SetUpdateRate(LONG lRate){ m_lRate = lRate;}
	LONG GetUpdateRate(){ return m_lRate;}
	
	virtual void Serialize(CArchive& ar);
};

#endif

