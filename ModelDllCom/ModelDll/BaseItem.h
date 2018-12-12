#ifndef BASEITEM_H
#define BASEITEM_H

class CBaseItem 
{
public:
	CBaseItem(void);
	virtual ~CBaseItem(void);
public:
	void SetDeviceName(CString strName)
	{
		m_strDeviceName = strName;
	}
	CString GetDeviceName()
	{
		return m_strDeviceName;
	}
	int GetShortValue();
	void SetVarValue(CString strValue);
	void FormatVariant(CString &Str);

	CString GetTypeString(int *pnSel)const;
	CString SetVarType(int nType);
	void Copy(CBaseItem* pItem);
public:
	//���豸�������
	DWORD   m_dwItemID;
	//����
	CString m_strItemName;
	//��ֵ(�������ͺ�ֵ)
	COleVariant	 m_vItemValue;
	//�������
	CString m_strItemDescription;
	//�Ƿ�Ϊֻ��
	BOOL	m_bReadOnly;
	//virtual void ItemUpdate(void) = 0;
protected:
	CString m_strDeviceName;
};
#endif