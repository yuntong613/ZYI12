//////////////////////////////////////////////////////////////////////
// OPCIniFile.h   :  ��ȡ��׼INI�����ļ�, ���ṩ������������߶�ȡ�ٶ�
// Comment     :  ������Open���ļ�, Ȼ����ReadIniString����ReadSection
//             :  ��ȡ��Ҫ������,ȫ���������Close�ر��ļ�.
//             :  ����Ŀ���˵���˽������ļ���ʽ��
// Author      :  �޳�Ⱥ
// Date        :  2002.11.04
//                2003.05.30  ������������const CString&��ΪLPCSTR
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INIFILE_H__A7E1E6C3_2787_43A2_B887_6BD009D7EA47__INCLUDED_)
#define AFX_INIFILE_H__A7E1E6C3_2787_43A2_B887_6BD009D7EA47__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class COPCIniFile : public CStdioFile  
{
public:
	COPCIniFile();
	virtual ~COPCIniFile();
  
  // ˳���ȡ�����ļ���һ�����ݣ����밴���ļ��е�˳�����ζ�ȡ
  //�Էֺţ�';'����ʼ����Ϊע����
  // ���������ǿ��У��������� Name=Value
  // Ӧָ��pszName=Name��sValue�������ս��
  // ���� Name������'['��ʼ����������[Section]
  BOOL ReadIniString(LPCSTR pszName, CString& sValue);
  
  // ˳���ȡ�����ļ��е�һ�нںţ����밴���ļ��е�˳�����ζ�ȡ
  // �Զ��������к�ע���У��������� [Section]
  // Ӧָ��pszSection=[Section]�������˶������ļ��е������Ƿ���ȷ
  BOOL ReadSection(LPCSTR pszSection);
  
	// ��ȡ�����ļ���һ��������ڽ������밴���ļ��е�˳�����ζ�ȡ
	// Ҫ����Ŀ�����ڽ���Ψһ
	BOOL ReadIniUniItem(LPCSTR pszSection,LPCSTR pszName, CString& sValue);
	
	// ˳���ȡ�����ļ���һ����������ԷǷ���
	BOOL ReadIniItem(LPCSTR pszName, CString& sValue);
	// ��ȡ�����ļ��е�һ�нںţ����밴���ļ��е�˳�����ζ�ȡ
	BOOL ReadNoSeqSection(LPCSTR pszSection);

	//��ȡ�ֶ�ֵ���ֶ�֮���Զ��ţ�','���ָ�
	BOOL ExtractSubValue(LPCSTR pszSrc,CString& strValue,int nSplit)const;
	
	// ���������Ч��ITEM������
	int GetItemsCount(LPCSTR pszSection,LPCSTR pszItem);

	BOOL Endof(){ return feof(m_pStream) ? TRUE : FALSE; }

};

#endif // !defined(AFX_INIFILE_H__A7E1E6C3_2787_43A2_B887_6BD009D7EA47__INCLUDED_)

