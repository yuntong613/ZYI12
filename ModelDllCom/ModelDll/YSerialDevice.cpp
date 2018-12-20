#include "StdAfx.h"
#include "common.h"
#include "YSerialDevice.h"
#include "IniFile.h"
#include "ItemBrowseDlg.h"
#include "YSerialItem.h"
#include <cstringt.h>
#include "ModelDll.h"
#include "OPCIniFile.h"
#include "ModbusCRC.h"

extern CModelDllApp theApp;

DWORD CALLBACK QuertyThread(LPVOID pParam)
{
	YSerialDevice* pDevice = (YSerialDevice*)pParam;
	while (!pDevice->m_bStop)
	{
		Sleep(1000);
		pDevice->QueryOnce();
	}
	return 0;
}


YSerialDevice::YSerialDevice(LPCSTR pszAppPath)
	: m_nBaudRate(0)
	, m_bStop(true)
{
	m_nParity = 0;
	m_hQueryThread = INVALID_HANDLE_VALUE;
	y_lUpdateTimer = 0;
	m_nUseLog = 0;
	CString strConfigFile(pszAppPath);
	strConfigFile += _T("\\ComFile.ini");
	if (!InitConfig(strConfigFile))
	{
		return;
	}

	// 	CString strListItemsFile(pszAppPath);
	// 	strListItemsFile += _T("\\ListItems.ini");
	// 	COPCIniFile opcFile;
	// 	if (!opcFile.Open(strListItemsFile,CFile::modeRead|CFile::typeText))
	// 	{
	// 		AfxMessageBox("Can't open INI file!");
	// 		return;
	// 	}
	// 	CArchive ar(&opcFile,CArchive::load);
	// 	Serialize(ar);
	// 	opcFile.Close();
}

YSerialDevice::~YSerialDevice(void)
{
	m_Com.Close();
	POSITION pos = m_ItemsArray.GetStartPosition();
	YSerialItem* pItem = NULL;
	CString strItemName;
	while (pos) {
		m_ItemsArray.GetNextAssoc(pos, strItemName, (CObject*&)pItem);
		if (pItem)
		{
			delete pItem;
			pItem = NULL;
		}
	}
	m_ItemsArray.RemoveAll();
}

void YSerialDevice::Serialize(CArchive& ar)
{
	if (ar.IsStoring()) {
	}
	else {
		Load(ar);
	}
}

BOOL YSerialDevice::InitConfig(CString strFilePath)
{
	if (!PathFileExists(strFilePath))
		return FALSE;
	m_strConfigFile = strFilePath;

	CIniFile iniFile(m_strConfigFile);
	m_lRate = iniFile.GetUInt("param", "UpdateRate", 3000);
	m_nUseLog = iniFile.GetUInt("param", "Log", 0);

	m_nBaudRate = iniFile.GetUInt("ComInfo", "BaudRate", 9600);
	m_nParity = iniFile.GetUInt("ComInfo", "Parity", 0);

	MakeItems();
	return TRUE;
}

void YSerialDevice::Load(CArchive& ar)
{
	/*	LoadItems(ar);*/


}
void YSerialDevice::MakeItems()
{
	YOPCItem* pItemStatus = NULL;
	YOPCItem* pItemCtrl = NULL;
	DWORD dwItemPId = 0L;
	CString strItemName, strItemDesc;

	CIniFile iniFile(m_strConfigFile);
	m_lRate = iniFile.GetUInt("param", "UpdateRate", 3000);
	m_nUseLog = iniFile.GetUInt("param", "Log", 0);

	CStringArray _ComPortArray, _SlaveAddr;
	iniFile.GetArray("ComInfo", "ComPort", &_ComPortArray);

	CString strSec;

	for (int j = 0; j < _ComPortArray.GetCount(); j++)
	{
		int nPort = atoi(_ComPortArray.GetAt(j));
		strSec.Format("COM%d",nPort);
		iniFile.GetArray(strSec, "Addr", &_SlaveAddr);

		for (int i = 0; i < _SlaveAddr.GetCount(); i++)
		{
			BYTE bAddr = atoi(_SlaveAddr.GetAt(i));
			for (int k = 1; k <= 12; k++)
			{
				strItemName.Format("S_%d_%d_%d", nPort, bAddr, k);
				strItemDesc.Format("%d回路状态", k);
				pItemStatus = new YShortItem(dwItemPId++, strItemName, strItemDesc);

				m_ItemsArray.SetAt(pItemStatus->GetName(), (CObject*)pItemStatus);

				strItemName.Format("CS_%d_%d_%d", nPort, bAddr, k);
				strItemDesc.Format("%d回路控制", k);
				pItemCtrl = new YShortItem(dwItemPId++, strItemName, strItemDesc);

				m_ItemsArray.SetAt(pItemCtrl->GetName(), (CObject*)pItemCtrl);
			}

			strItemName.Format("CA_%d_%d_ALL", nPort, bAddr);
			strItemDesc.Format("%d站全部控制", bAddr);
			pItemCtrl = new YShortItem(dwItemPId++, strItemName, strItemDesc);
			m_ItemsArray.SetAt(pItemCtrl->GetName(), (CObject*)pItemCtrl);
		}
	}
}

void YSerialDevice::LoadItems(CArchive& ar)
{
	COPCIniFile* pIniFile = static_cast<COPCIniFile*>(ar.GetFile());
	YOPCItem* pItem = NULL;
	int nItems = 0;
	CString strTmp("Item");
	CString strItemName;
	CString strItemDesc;
	CString strValue;
	DWORD dwItemPId = 0L;
	strTmp += CString(_T("List"));
	if (pIniFile->ReadNoSeqSection(strTmp)) {
		nItems = pIniFile->GetItemsCount(strTmp, "Item");
		for (int i = 0; i < nItems && !pIniFile->Endof(); i++)
		{
			try {
				if (pIniFile->ReadIniItem("Item", strTmp))
				{
					if (!pIniFile->ExtractSubValue(strTmp, strValue, 1))
						throw new CItemException(CItemException::invalidId, pIniFile->GetFileName());
					dwItemPId = atoi(strValue);
					if (!pIniFile->ExtractSubValue(strTmp, strItemName, 2))strItemName = _T("Unknown");
					if (!pIniFile->ExtractSubValue(strTmp, strItemDesc, 3)) strItemDesc = _T("Unknown");
					pItem = new YShortItem(dwItemPId, strItemName, strItemDesc);
					if (GetItemByName(strItemName))
						delete pItem;
					else
						m_ItemsArray.SetAt(pItem->GetName(), (CObject*)pItem);
				}
			}
			catch (CItemException* e) {
				if (pItem) delete pItem;
				e->Delete();
			}
		}
	}
}

void YSerialDevice::OnUpdate()
{

}

int YSerialDevice::QueryOnce()
{
	y_lUpdateTimer += 1000;
	if (y_lUpdateTimer < m_lRate)
		return 0; 
	y_lUpdateTimer = 0;

	CIniFile iniFile(m_strConfigFile);

	CStringArray _ComPortArray, _SlaveAddr;
	iniFile.GetArray("ComInfo", "ComPort", &_ComPortArray);
	int nTimeOut = iniFile.GetInt("ComInfo", "TimeOut", 5000);
	CString strSec;
	for (int j = 0; j < _ComPortArray.GetCount(); j++)
	{
		m_Lock.Lock();
		int nPort = atoi(_ComPortArray.GetAt(j));
		m_Com.Open(nPort, m_nBaudRate, m_nParity);

		strSec.Format("COM%d", nPort);
		iniFile.GetArray(strSec, "Addr", &_SlaveAddr);
		for (int k = 0; k < _SlaveAddr.GetCount(); k++)
		{
			BYTE bAddr = atoi(_SlaveAddr.GetAt(k));

			BYTE bSend[8] = { 0 };
			bSend[0] = bAddr;
			bSend[1] = 0x03;
			bSend[2] = 0x00;
			bSend[3] = 0x03;
			bSend[4] = 0x00;
			bSend[5] = 0x0c;
			WORD wCrc = 0;
			CheckCRCModBus(bSend, 6, &wCrc);
			bSend[6] = HIBYTE(wCrc);
			bSend[7] = LOBYTE(wCrc);

			m_Com.Write(bSend, 8);

			CString strHex = Bin2HexStr(bSend, 8);
			OutPutLog("Send：" + strHex);

			BYTE recvBuf[60] = { 0 };
			DWORD dwRead = m_Com.Read(recvBuf, 29, nTimeOut);
			if (dwRead > 0)
			{
				strHex = Bin2HexStr(recvBuf, dwRead);
				OutPutLog("Recv：" + strHex);
			}

			if (dwRead == 17)
			{
				CheckCRCModBus(recvBuf, dwRead - 2, &wCrc);
				if (recvBuf[dwRead - 1] == LOBYTE(wCrc) && recvBuf[dwRead - 2] == HIBYTE(wCrc))
				{
					if (bAddr != recvBuf[0])
						return -1;
					if (recvBuf[1] != 0x03)
						return -1;
					if (recvBuf[2] != 0x18)
						return -1;

					CString strHex = Bin2HexStr(recvBuf, dwRead);
					strHex.Remove(' ');
					strHex.Delete(0, 6);

					CString strItemName, strValue;
					for (int i = 1; i <= 12; i++)
					{
						strValue = strHex.Left(2);
						strItemName.Format("S_%d_%d_%d",nPort, bAddr, i);
						YOPCItem* pItem = GetItemByName(strItemName);
						if (pItem)
						{
							pItem->OnUpdate(strValue);
						}
						strHex.Delete(0, 4);
					}
				}
			}
		}

		m_Com.Close();
		m_Lock.Unlock();
	}

	return 0;
}

void YSerialDevice::BeginUpdateThread()
{
	if (m_hQueryThread == INVALID_HANDLE_VALUE)
	{
		if (m_bStop == true)
		{
			m_bStop = false;
			m_hQueryThread = CreateThread(NULL, 0, QuertyThread, this, 0, NULL);
		}
	}
}

void YSerialDevice::EndUpdateThread()
{
	if (!m_bStop)
	{
		m_bStop = true;
		DWORD dwRet = WaitForSingleObject(m_hQueryThread, 3000);
		if (dwRet == WAIT_TIMEOUT)
			TerminateThread(m_hQueryThread, 0);

		m_hQueryThread = INVALID_HANDLE_VALUE;
	}
}

BYTE YSerialDevice::Hex2Bin(CString strHex)
{
	int iDec = 0;
	if (strHex.GetLength() == 2) {
		char cCh = strHex[0];
		if ((cCh >= '0') && (cCh <= '9'))iDec = cCh - '0';
		else if ((cCh >= 'A') && (cCh <= 'F'))iDec = cCh - 'A' + 10;
		else if ((cCh >= 'a') && (cCh <= 'f'))iDec = cCh - 'a' + 10;
		else return 0;
		iDec *= 16;
		cCh = strHex[1];
		if ((cCh >= '0') && (cCh <= '9'))iDec += cCh - '0';
		else if ((cCh >= 'A') && (cCh <= 'F'))iDec += cCh - 'A' + 10;
		else if ((cCh >= 'a') && (cCh <= 'f'))iDec += cCh - 'a' + 10;
		else return 0;
	}
	return iDec & 0xff;
}

int YSerialDevice::HexStr2Bin(BYTE * cpData, CString strData)
{
	CString strByte;
	for (int i = 0; i < strData.GetLength(); i += 2) {
		strByte = strData.Mid(i, 2);
		cpData[i / 2] = Hex2Bin(strByte);
	}
	return strData.GetLength() / 2;
}

CString YSerialDevice::Bin2HexStr(BYTE* cpData, int nLen)
{
	CString strResult, strTemp;
	for (int i = 0; i < nLen; i++)
	{
		strTemp.Format("%02X ", cpData[i]);
		strResult += strTemp;
	}
	return strResult;
}

void YSerialDevice::HandleData()
{

	return;
}

bool YSerialDevice::SetDeviceItemValue(CBaseItem* pAppItem)
{
	CString strItemName = pAppItem->m_strItemName;
	int nValue = pAppItem->GetShortValue();

	YOPCItem* pItem = GetItemByName(strItemName);
	if (!pItem)
	{
		return false;
	}
	int nPort = 0;
	int nAddr = 0;
	int nLoop = 0;

	CIniFile iniFile(m_strConfigFile);
	int nTimeOut = iniFile.GetInt("ComInfo", "TimeOut", 5000);
	if (strItemName.Left(2) == "CA")
	{
		sscanf(strItemName, "CA_%d_%d", &nPort,&nAddr);
		m_Lock.Lock();
		if (m_Com.Open(nPort,m_nBaudRate,m_nParity))
		{
			BYTE bSend[8] = { 0 };
			bSend[0] = nAddr;
			bSend[1] = 0x06;
			bSend[2] = 0x00;
			bSend[3] = (nValue == 0) ? 1 : 2;
			bSend[4] = 0x00;
			bSend[5] = (nValue == 0) ? 0 : 1;
			WORD wCrc = 0;
			CheckCRCModBus(bSend, 6, &wCrc);
			bSend[6] = HIBYTE(wCrc);
			bSend[7] = LOBYTE(wCrc);

			m_Com.Write(bSend, 8);

			CString strSendHex = Bin2HexStr(bSend, 8);
			OutPutLog("Send：" + strSendHex);

			BYTE recvBuf[60] = { 0 };
			DWORD dwRead = m_Com.Read(recvBuf, 29, nTimeOut);
			if (dwRead > 0)
			{
				CString strRecvHex = Bin2HexStr(recvBuf, dwRead);
				OutPutLog("Recv：" + strRecvHex);
				if (strSendHex == strRecvHex)
				{
					return true;
				}
			}
			m_Com.Close();
		}
		m_Lock.Unlock();
		
	}
	else if (strItemName.Left(2) == "CS")
	{
		sscanf(strItemName, "CS_%d_%d_%d",&nPort, &nAddr, &nLoop);
		m_Lock.Lock();
		if (m_Com.Open(nPort, m_nBaudRate, m_nParity))
		{
			BYTE bSend[8] = { 0 };
			bSend[0] = nAddr;
			bSend[1] = 0x06;
			bSend[2] = 0x00;
			bSend[3] = nLoop+2;
			bSend[4] = 0x00;
			bSend[5] = (nValue?1:0);
			WORD wCrc = 0;
			CheckCRCModBus(bSend, 6, &wCrc);
			bSend[6] = HIBYTE(wCrc);
			bSend[7] = LOBYTE(wCrc);

			m_Com.Write(bSend, 8);

			CString strSendHex = Bin2HexStr(bSend, 8);
			OutPutLog("Send：" + strSendHex);

			BYTE recvBuf[60] = { 0 };
			DWORD dwRead = m_Com.Read(recvBuf, 29, nTimeOut);

			if (dwRead > 0)
			{
				CString strRecvHex = Bin2HexStr(recvBuf, dwRead);
				OutPutLog("Recv：" + strRecvHex);
				if (strSendHex == strRecvHex )
				{
					return true;
				}
			}
			m_Com.Close();
		}
		m_Lock.Unlock();
	}
	return false;
}

void YSerialDevice::OutPutLog(CString strMsg)
{
	if (m_nUseLog)
		m_Log.Write(strMsg);
}
