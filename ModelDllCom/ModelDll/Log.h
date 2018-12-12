

#pragma once

#include <Afxmt.h>

//////////////////////////////////////////////////////////////////////////
//ʹ�÷���:
//ʹ��ʱ,ֻ��Ҫ������Ӧ��write ��writeformat����,�������ɵ���־��������
//20100608.log;
//֧���Զ�����־��,�����SetCustomName(XXX),�������ɵ���־��ΪXXX20100608.log.
//֧���Զ����ļ�����·��SetLogPath
//�Զ����һ��ǰ����־��
//֧����־�ļ�����.����ֻ�����µ���һ����ܺ���
//�������ANSCII��UNICODE����,������WINDOWSƽ̨��
//Author: maxw
///////////////////////////////////////////////////////////////////////////

class  CLog  
{
public :

	CLog(void);

	virtual ~CLog(void);

public :
	
	CString GetLogFileName() const;

	CString GetAppPath() const;


	void SetLogFileName();


	CString GetLogPath() const;

	void SetLogPath(CString pszPath = "");


	void SetCustomName(CString strCustomName){m_strCustomName = strCustomName;};

	BOOL IsHaveCustomName(){return m_strCustomName != "";};


	UINT GetLogOption() const;

	void SetLogOption(UINT newVal);


	bool GetWithThreadId() const;

	void SetWithThreadId(bool newVal);

	
	UINT GetMaxLogFileNumber() const;

	void SetMaxLogFileNumber(UINT newVal);


	bool GetAutoDelLogFile() const;

	void SetAutoDelLogFile(bool newVal);


	bool IsNeedEncrypt();

	void SetEncrypt(CString strPSK);


	void Encrypt(BYTE *buffer,DWORD bufsize);

public :

	bool Write(LPCTSTR lpszMsg);

	bool Write(LPCTSTR lpszMsg, UINT eLogType);

	bool Write(const void* lpBuf, UINT nCount);

	bool Write(const void* lpBuf, UINT nCount, UINT eLogType);

	bool WriteFormat(LPCTSTR lpszMsg, ...);

	bool WriteFormat(UINT eLogType, LPCTSTR lpszMsg, ...);

protected :
	
	BOOL IsHaveFileName(){return m_LogFileName != "";}

	void MakeLogInfoHead(CString& strMsg);

	void ChangeLogFileName();

	long CleanLog(void);

	void DelLogFiles();

	void OpenLogFile();
	
	bool CheckLogOption(UINT eLogType);

	bool LogInfo(CString strInfo);

	bool LogBuf(const void* lpBuf, UINT nCount);

private :

	CString m_LogFileName;

	CString m_strCustomName;
	
	CString m_LogPath;
	
	int  m_LastDay;

	UINT m_LogOption;

	bool m_WithThreadId;

	bool m_bEncrypt;
	
	bool m_AutoDelLogFile;

	UINT m_MaxLogFileNumber;

	CCriticalSection m_cs;

	CFile m_LogFile;

	CHAR *m_pPSK;

	DWORD  m_iPSKLen;//��������β��

};
