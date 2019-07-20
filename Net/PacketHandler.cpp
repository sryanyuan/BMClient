/************************************************************************/
/* class PacketHandler
/* 包处理基类 注册给SocketDataCenter用以处理数据包
/*
/* class SocketDataCenter
/* 数据包解析中心 调用注册过的物件的OnPacket函数
/************************************************************************/
#include "../Net/PacketHandler.h"
#include "../GameScene/GamePlayer.h"
#include "../GameScene/GameScene.h"
#include "../GameDialog/GameQuitDialog.h"
#include "../BackMir/BackMir.h"
#include "../../CommonModule/SaveFile.h"
#include "../GameDialog/GameChatDlg.h"
#include "../GameDialog/GameStoreDlg2.h"
#include "../GameDialog/GameBigStoreDlg.h"
#include "../../CommonModule/DataEncryptor.h"
#include "../../CommonModule/version.h"
#include <ZipArchive.h>
#include <direct.h>
#include <Windows.h>
#include "../../CommonModule/base64.h"
extern ByteBuffer g_xBuffer;

//////////////////////////////////////////////////////////////////////////
/*
 *	SocketDataCenter
 */
//////////////////////////////////////////////////////////////////////////
SocketDataCenter::SocketDataCenter()
{
	SetHandlePacket(&SocketDataCenter::ProcessData);
}

SocketDataCenter::~SocketDataCenter()
{
	//
}
//////////////////////////////////////////////////////////////////////////
/************************************************************************/
/* void PushData(void* _pData, int _nLen)
/************************************************************************/
void SocketDataCenter::PushData(const void* _pData, unsigned int _nLen)
{
	try
	{
		OnSocketData(_pData, _nLen);
	}
	//BUFFER_EXCEPTION_CATCH_RETURN_VOID;
	catch(std::exception& e)
	{
		AfxGetHge()->System_Log("Buffer overflow...error:", e.what());
	}
}

/************************************************************************/
/* PacketHandler* GetHandler(int _nId)
/************************************************************************/
PacketHandler* SocketDataCenter::GetHandler(int _nId)
{
	HandlerMap::iterator iterFind = m_xHandlers.find(_nId);
	if(iterFind == m_xHandlers.end())
	{
		return NULL;
	}
	else
	{
		return iterFind->second;
	}
}

/************************************************************************/
/* virtual void ProcessSystemPacket(const PacketHeader* _pPkt)
/************************************************************************/
void SocketDataCenter::ProcessSystemPacket(const PacketHeader* _pPkt)
{
	//
	//static ByteBuffer buffer(2048);
	//buffer.Reset();
	//buffer.Write(_pPkt, _pPkt->uLen);
	g_xBuffer.Reset();
	g_xBuffer.Write(_pPkt, _pPkt->uLen);

	switch (_pPkt->uOp)
	{
	case PKG_SYSTEM_USERLOGIN_ACK:
		{
			PkgUserLoginAck ack;
			g_xBuffer >> ack;
			DoPacket_SystemUserLoginAck(&ack);
		}break;
	case PKG_SYSTEM_FORCE_ACTION_ACK:
		{
			PkgForceActionAck ack;
			g_xBuffer >> ack;
			DoPacket_SystemForceActionAck(&ack);
		}break;
	case PKG_SYSTEM_NEWPLAYER_NOT:
		{
			PkgNewPlayerNot not;
			g_xBuffer >> not;
			DoPacket_SystemNewPlayerNot(&not);
		}break;
	case PKG_SYSTEM_NEWNPC_NOT:
		{
			PkgNewNPCNot not;
			g_xBuffer >> not;
			DoPacket_SystemNewNPCNot(&not);
		}break;
	case PKG_SYSTEM_USERDATA_ACK:
		{
			PkgSystemUserDataAck ack;
			g_xBuffer >> ack;
			DoPacket_SystemUserDataAck(&ack);
		}break;

		DO_SYSTEM_PACKET(PKG_SYSTEM_NEWITEM_NOT, PkgSystemNewItemNot);
		DO_SYSTEM_PACKET(PKG_SYSTEM_DELITEM_NOT, PkgSystemDelItemNot);
		DO_SYSTEM_PACKET(PKG_SYSTEM_DELNPC_NOT, PkgDelNPCNot);
		DO_SYSTEM_PACKET(PKG_SYSTEM_CLEARITEM_NTF, PkgSystemClearGroundItemNtf);
		DO_SYSTEM_PACKET(PKG_GAME_GM_NOTIFICATION_NOT, PkgGmNotificationNot);
		DO_SYSTEM_PACKET(PKG_SYSTEM_NOTIFY_NOT, PkgSystemNotifyNot);
		DO_SYSTEM_PACKET(PKG_SYSTEM_CLIENTVERSIONERR_NTF, PkgSystemClientVersionErrNtf);
		DO_SYSTEM_PACKET(PKG_SYSTEM_HIDEDOOR_NTF, PkgSystemHideDoorNtf);
		DO_SYSTEM_PACKET(PKG_SYSTEM_WORLD_SAY_NOT, PkgSystemWorldSayNot);
		DO_SYSTEM_PACKET(PKG_SYSTEM_EXT_USERDATA_ACK, PkgSystemExtUserDataAck);

	default:
		{
			AfxGetHge()->System_Log("未处理的系统数据包[%d]",
				_pPkt->uOp);
		}break;
	}
}

/************************************************************************/
/* System packet processor
/************************************************************************/
void SocketDataCenter::DoPacket_SystemUserLoginAck(const PkgUserLoginAck* _pPkt)
{
	//	login process
	if(_pPkt->bOk == false)
	{
		::MessageBox(NULL, "服务器验证失败", "Error", MB_ICONERROR | MB_TASKMODAL);
		::PostQuitMessage(0);
		return;
	}
	else
	{
		if(_pPkt->uVersion != BACKMIR_VERSION)
		{
#ifdef _DEBUG
#else
			::MessageBox(NULL, "客户端服务端版本不匹配", "Error", MB_ICONERROR | MB_TASKMODAL);
			::PostQuitMessage(0);
			return;
#endif
		}
#define MAX_DATA_SIZE 10240
		GamePlayer* pPlayer = GamePlayer::GetInstance();
		const char* pData = &_pPkt->xMsg[0];
		DWORD dwDataLen = _pPkt->xMsg.size();

		char* pBuf = new char[MAX_DATA_SIZE];
		uLongf buflen = MAX_DATA_SIZE;
		uLongf srclen = dwDataLen;
		int nRet = uncompress((Bytef*)pBuf, &buflen, (const Bytef*)pData, srclen);
		if(nRet == Z_OK)
		{
			GamePlayer::GetInstance()->SetHandlerID(_pPkt->nId);
			RegisterObject(GamePlayer::GetInstance());

			g_xBuffer.Reset();
			g_xBuffer.Write(pBuf, buflen);
			delete[] pBuf;

			char szName[20];
			g_xBuffer >> szName;
			strcpy(pPlayer->GetAttrib()->name, szName);
			g_xBuffer >> pPlayer->GetAttrib()->level;
			USHORT uMapID = 0;
			g_xBuffer >> uMapID;
			pPlayer->SetMapID(uMapID);
			WORD wPosX = 0;
			WORD wPosY = 0;
			g_xBuffer >> wPosX;
			g_xBuffer >> wPosY;
			pPlayer->SetReallyCoord(wPosX, wPosY);
			//	HP MP EXP MONEY
			DWORD dwHME = 0;
			g_xBuffer >> dwHME;
			pPlayer->GetAttrib()->HP = LOWORD(dwHME);
			pPlayer->GetAttrib()->maxHP = HIWORD(dwHME);
			g_xBuffer >> dwHME;
			pPlayer->GetAttrib()->MP = LOWORD(dwHME);
			pPlayer->GetAttrib()->maxMP = HIWORD(dwHME);
			g_xBuffer >> dwHME;
			pPlayer->GetAttrib()->EXPR = dwHME;
			g_xBuffer >> dwHME;
			pPlayer->GetAttrib()->maxEXPR = dwHME;
			int nMoney = 0;
			g_xBuffer >> nMoney;
			pPlayer->GetPlayerBag()->SetMoney(nMoney);

			BYTE bBagSum = 0;
			BYTE bBodySum = 0;
			g_xBuffer >> bBagSum;

			ItemAttrib item;
			if(bBagSum)
			{
				if(bBagSum <= HERO_MAINBAG_SIZE_CUR)
				{
					for(int i = 0; i < bBagSum; ++i)
					{
						g_xBuffer >> item;
						pPlayer->GetPlayerBag()->AddItem(&item);
						if(!pTheGame->CheckItemValid(&item))
						{
							PostQuitMessage(0);
						}
					}
				}
				else
				{
					//	Move to assist
					int nMoveCount = 0;
					for(int i = 0; i < bBagSum; ++i)
					{
						g_xBuffer >> item;

						if(nMoveCount < 6 &&
							item.type == ITEM_COST)
						{
							pPlayer->GetPlayerBag()->GetAssistItemList()[nMoveCount] = item;
							++nMoveCount;
						}
						else
						{
							pPlayer->GetPlayerBag()->AddItem(&item);
							if(!pTheGame->CheckItemValid(&item))
							{
								PostQuitMessage(0);
							}
						}
					}
				}
			}

			g_xBuffer >> bBodySum;
			BYTE bPos = 0;
			if(bBodySum)
			{
				for(int i = 0; i < bBodySum; ++i)
				{
					g_xBuffer >> bPos;
					g_xBuffer >> item;
					memcpy(pPlayer->GetPlayerItem((PLAYER_ITEM_TYPE)bPos), &item, sizeof(ItemAttrib));
					if(!pTheGame->CheckItemValid(&item))
					{
						PostQuitMessage(0);
					}
				}
			}
			pPlayer->RefleashHumState();

			BYTE bMagic = 0;
			WORD wID = 0;
			BYTE bLevel = 0;

			if(g_xBuffer.GetLength() >= 1)
			{
				g_xBuffer >> bMagic;
			}
			if(bMagic > 0)
			{
				for(int i = 0; i < bMagic; ++i)
				{
					g_xBuffer >> wID >> bLevel;
					GamePlayer::GetInstance()->AddToMagicTree(wID);
					GamePlayer::GetInstance()->SetMagicLevel(wID, bLevel);
				}
			}

			g_xBuffer.Reset();
			char szSavFile[MAX_PATH];
			string strName = pPlayer->GetAttrib()->name;
			string strFile;
			Base64::Encode(strName, &strFile);
			sprintf(szSavFile, "%s\\Save\\%s.cfg",
				GetRootPath(), strFile.c_str());

			HANDLE hCfgFile = ::CreateFile(szSavFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
			DWORD dwRead = 0;
			DWORD dwFileSize = 0;
			int nAssistID[6] = {0};
			if(hCfgFile != INVALID_HANDLE_VALUE)
			{
				dwFileSize = GetFileSize(hCfgFile, NULL);
				ItemAttrib* pAssistItem = NULL;
				GamePlayer* pPlayer = GamePlayer::GetInstance();

				if(dwFileSize >= 4 * 6)
				{
					ReadFile(hCfgFile, nAssistID, 4 * 6, &dwRead, NULL);
					if(dwRead == 4 * 6)
					{
						for(int i = 0; i < HERO_CLIENT_ASSIST_SIZE; ++i)
						{
							if(nAssistID[i] != 0)
							{
								for(int j = 0; j < HERO_BAG_SIZE_CUR; ++j)
								{
									if(GamePlayer::GetInstance()->GetPlayerBag()->GetItemList()[j].id == nAssistID[i])
									{
										memcpy(&GamePlayer::GetInstance()->GetPlayerBag()->GetAssistItemList()[i],
											&GamePlayer::GetInstance()->GetPlayerBag()->GetItemList()[j],
											sizeof(ItemAttrib));
										GamePlayer::GetInstance()->UpdateAccelerate(i);
										break;
									}
								}
							}
							/*if(nAssistID[i] != 0)
							{
								pAssistItem = &pPlayer->GetPlayerBag()->GetAssistItemList()[i];
								if(pAssistItem->type == ITEM_NO)
								{
									//	直接放上去
									pPlayer->GetPlayerBag()->MoveBagItemToAssistItem(nAssistID[i]);
								}
								else
								{
									//	要交换了
									ItemAttrib* pBagItem = NULL;
									ItemList& items = pPlayer->GetPlayerBag()->GetItemList();
									for(int i = 0; i < HERO_MAINBAG_SIZE_CUR; ++i)
									{
										if(items[i].id == nAssistID[i] &&
											items[i].type != ITEM_NO)
										{
											pBagItem = &items[i];
											break;
										}
									}
									if(pBagItem)
									{
										ItemAttrib item;
										memcpy(&item, pAssistItem, sizeof(ItemAttrib));
										memcpy(pAssistItem, pBagItem, sizeof(ItemAttrib));
										memcpy(pBagItem, &item, sizeof(ItemAttrib));
									}
								}
							}*/
						}
					}
					dwFileSize -= 4 * 6;

					if(dwFileSize > 0)
					{
						char szBuf[1024];
						ReadFile(hCfgFile, szBuf, dwFileSize, &dwRead, NULL);

						if(dwRead == dwFileSize)
						{
							char* pKey = NULL;
							WORD* pMgc = NULL;

							if(dwFileSize % 3 == 0)
							{
								int nMgcSum = dwFileSize / 3;
								for(int i = 0; i < nMgcSum; ++i)
								{
									pKey = &szBuf[3 * i + 0];
									pMgc = (WORD*)&szBuf[3 * i + 1];

									if(*pKey >= HGEK_F1 &&
										*pKey <= HGEK_F8)
									{
										pPlayer->SetMagicKey(*pMgc, *pKey);
									}
								}
							}
						}
					}
				}
				CloseHandle(hCfgFile);
			}
			pPlayer->ReadBagItemOrder();
			pPlayer->UpdatePlayerAttrib();
			pPlayer->LoadAccMagicKeyCfg();

#ifdef _DEBUG
			PkgPlayerSpeOperateReq req;
			req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
			req.dwOp = CMD_OP_GMHIDE;
			// Get code from environment variable
			const char* pszGMCode = getenv("gmcode");
			if (NULL != pszGMCode)
			{
				int nGMCode = atoi(pszGMCode);
				if (0 != nGMCode)
				{
					req.dwParam = nGMCode;
					req.dwParam = DataEncryptor::EncryptGMCode(req.dwParam);
					g_xBuffer.Reset();
					g_xBuffer << req;
					SendBufferToGS(&g_xBuffer);
				}
			}
#endif
		}
		else
		{
			ALERT_MSGBOX("解压服务器数据失败!");
			delete[] pBuf;
			return;
		}

		if(pTheGame->GetLoginScene())
		{
			pTheGame->GetLoginScene()->EnterLoadingPage();
		}
	}
}

void SocketDataCenter::DoPacket_SystemForceActionAck(const PkgForceActionAck* _pPkt)
{
	//	Force action
	switch(_pPkt->nType)
	{
	case FORCE_KICK_OUT:
		{
			::MessageBox(NULL, "服务器验证失败", "ERROR", MB_ICONERROR | MB_TASKMODAL);
			PostQuitMessage(0);
		}break;
	}
}

void SocketDataCenter::DoPacket_SystemNewPlayerNot(const PkgNewPlayerNot* _pPkt)
{
	//	new player or delete player
	GameScene::sThis->InsertNewObject(*_pPkt);
}

void SocketDataCenter::DoPacket_SystemNewNPCNot(const PkgNewNPCNot* _pPkt)
{
	GameScene::sThis->InsertNewObject(*_pPkt);
}

bool SocketDataCenter::SaveHumData_ZipArchive(const char* _pszFile, const PkgSystemUserDataAck* _pPkt)
{
	char szExpr[100];
	GamePlayer* pPlayer = GamePlayer::GetInstance();

	if(_pPkt->xData.empty())
	{
		strcpy(szExpr, "存档数据为空!");
	}
	else
	{
		if(PathFileExists(_pszFile))
		{
			CZipArchive xArh;
			if(xArh.Open(_pszFile))
			{
				xArh.SetPassword(SaveFile::CalcInternalPassword());

				HeroHeader header;
				//header.bJob = pPlayer->GetAttrib()->price;
				header.bJob = pPlayer->GetHeroJob();
				header.bSex = pPlayer->GetAttrib()->sex;
				strcpy(header.szName, pPlayer->GetAttrib()->name);
				header.uLevel = pPlayer->GetAttrib()->level;

				ZIP_INDEX_TYPE zIndex = xArh.FindFile("head");
				//ZIP_INDEX_TYPE zIndex2;

				if(zIndex != ZIP_FILE_INDEX_NOT_FOUND)
				{
					xArh.RemoveFile(zIndex);

					CZipFileHeader zHeader;
					zHeader.SetFileName("head");
					xArh.OpenNewFile(zHeader);

					g_xBuffer.Reset();
					g_xBuffer << header;

					xArh.WriteNewFile(g_xBuffer.GetBuffer(), g_xBuffer.GetLength());
					xArh.CloseNewFile();

					zIndex = xArh.FindFile("data");
					//zIndex2 = xArh.FindFile("head");
					if(zIndex != ZIP_FILE_INDEX_NOT_FOUND)
					{
						if(!_pPkt->xData.empty())
						{
							xArh.RemoveFile(zIndex);

							CZipFileHeader zHeader;
							zHeader.SetFileName("data");
							xArh.OpenNewFile(zHeader);

							g_xBuffer.Reset();
							g_xBuffer << _pPkt->xData;

							xArh.WriteNewFile(g_xBuffer.GetBuffer(), g_xBuffer.GetLength());
							xArh.CloseNewFile();

							strcpy(szExpr, "存档成功!");
						}
						else
						{
							strcpy(szExpr, "存档数据为空!");
						}
					}
					else
					{
						//	新存档
						if(!_pPkt->xData.empty())
						{
							CZipFileHeader zHeader;
							zHeader.SetFileName("data");
							xArh.OpenNewFile(zHeader);

							g_xBuffer.Reset();
							g_xBuffer << _pPkt->xData;

							xArh.WriteNewFile(g_xBuffer.GetBuffer(), g_xBuffer.GetLength());
							xArh.CloseNewFile();

							strcpy(szExpr, "存档成功!");
						}
						else
						{
							strcpy(szExpr, "存档数据为空!");
						}
					}

					xArh.Close();

					//	修改所有物品属性为绑定
					ItemList& bagitems = GamePlayer::GetInstance()->GetPlayerBag()->GetItemList();
					ItemList& astitems = GamePlayer::GetInstance()->GetPlayerBag()->GetAssistItemList();

					for(int i = 0; i < HERO_MAINBAG_SIZE_CUR; ++i)
					{
						if(bagitems[i].type != ITEM_NO)
						{
							//bagitems[i].atkPois = 1;
							SET_FLAG(bagitems[i].atkPois, POIS_MASK_BIND);
						}
					}
					for(int i = 0; i < HERO_ASSISTBAG_SIZE_CUR; ++i)
					{
						if(astitems[i].type != ITEM_NO)
						{
							//astitems[i].atkPois = 1;
							SET_FLAG(astitems[i].atkPois, POIS_MASK_BIND);
						}
					}
					for(int i = 0; i < PLAYER_ITEM_TOTAL; ++i)
					{
						if(GamePlayer::GetInstance()->GetPlayerItem((PLAYER_ITEM_TYPE)i)->type != ITEM_NO)
						{
							//GamePlayer::GetInstance()->GetPlayerItem((PLAYER_ITEM_TYPE)i)->atkPois = 1;
							SET_FLAG(GamePlayer::GetInstance()->GetPlayerItem((PLAYER_ITEM_TYPE)i)->atkPois, POIS_MASK_BIND);
						}
					}
				}
				else
				{
					strcpy(szExpr, "无法定位文件头");
					xArh.Close();
				}
			}
			else
			{
				strcpy(szExpr, "无法打开存档文件");
			}
		}
		else
		{
			strcpy(szExpr, "无法定位存档文件");
		}
	}

	//GameScene::sThis->GetMainOpt()->GetStaticDlg()->Create(szExpr, 500);
	GameScene::sThis->GetMainOpt()->GetQuitDlg()->CreateMsgDlg(szExpr);
	if(GamePlayer::GetInstance()->IsRequestSmallQuit())
	{
		GameScene::sThis->GetMainOpt()->GetQuitDlg()->CloseDialog();
	}
	return true;
}

bool SocketDataCenter::SaveHumData_ZipArchiveMultiLogin(const PkgSystemUserDataAck* _pPkt)
{
	char szPath[MAX_PATH];
	sprintf(szPath, "%s\\NetSave\\", GetRootPath());
	if(!PathFileExists(szPath))
	{
		mkdir(szPath);
	}
	sprintf(szPath, "%s\\NetSave\\hum.sav", GetRootPath());
	if(!PathFileExists(szPath))
	{
		CZipArchive xSave;
		xSave.Open(szPath, CZipArchive::zipCreate);
		xSave.SetPassword(SaveFile::CalcInternalPassword());
		xSave.Close();
	}

	char szExpr[100];
	GamePlayer* pPlayer = GamePlayer::GetInstance();

	static const char* s_szHeader[] =
	{
		"head",
		"head1",
		"head2"
	};
	static const char* s_szData[] =
	{
		"data",
		"data1",
		"data2"
	};

	const char* pszUserHeader = s_szHeader[pPlayer->GetSaveIndex()];
	const char* pszUserData = s_szData[pPlayer->GetSaveIndex()];

	int nSaveIndex = -1;

	if(_pPkt->xData.empty())
	{
		strcpy(szExpr, "存档数据为空!");
	}
	else
	{
		if(PathFileExists(szPath))
		{
			do 
			{
				CZipArchive xArh;
				if(xArh.Open(szPath))
				{
					xArh.SetPassword(SaveFile::CalcInternalPassword());

					HeroHeader header;
					//header.bJob = pPlayer->GetAttrib()->price;
					header.bJob = pPlayer->GetHeroJob();
					header.bSex = pPlayer->GetAttrib()->sex;
					//strcpy(header.szName, pPlayer->GetAttrib()->name);
					pPlayer->GetHeroNameInMask(header.szName);
					header.uLevel = pPlayer->GetAttrib()->level;

					ZIP_INDEX_TYPE zIndex = ZIP_FILE_INDEX_NOT_FOUND;
					int nEmptyPlace = -1;

					for(int i = 0; i < 3; ++i)
					{
						zIndex = xArh.FindFile(s_szHeader[i]);
						HeroHeader heroheader;

						if(zIndex != ZIP_FILE_INDEX_NOT_FOUND)
						{
							CZipFileHeader* pHeader = NULL;
							pHeader = xArh.GetFileInfo(zIndex);

							if(pHeader)
							{
								xArh.OpenFile(zIndex);
								char* pBuf = new char[pHeader->m_uUncomprSize];
								xArh.ReadFile(pBuf, pHeader->m_uUncomprSize);
								g_xBuffer.Reset();
								g_xBuffer.Write(pBuf, pHeader->m_uUncomprSize);
								SAFE_DELETE_ARRAY(pBuf);
								xArh.CloseFile();

								g_xBuffer >> heroheader;
								if(0 == strcmp(heroheader.szName, header.szName))
								{
									nSaveIndex = i;
									pszUserHeader = s_szHeader[i];
									pszUserData = s_szData[i];
									break;
								}
							}
						}
						else
						{
							if(nEmptyPlace == -1)
							{
								nEmptyPlace = i;
							}
						}
					}

					if(header.szName[0] == 0 ||
						nSaveIndex == -1)
					{
						//	没有相同的 自己创一个
						if(nEmptyPlace != -1)
						{
							nSaveIndex = nEmptyPlace;
							pszUserHeader = s_szHeader[nSaveIndex];
							pszUserData = s_szData[nSaveIndex];
						}
						else
						{
							xArh.Close();
							return false;
						}
					}

					zIndex = xArh.FindFile(pszUserHeader);
					if(zIndex == ZIP_FILE_INDEX_NOT_FOUND)
					{
						CZipFileHeader zHeader;
						zHeader.SetFileName(pszUserHeader);
						xArh.OpenNewFile(zHeader);

						g_xBuffer.Reset();
						g_xBuffer << header;
						xArh.WriteNewFile(g_xBuffer.GetBuffer(), g_xBuffer.GetLength());
						xArh.CloseNewFile();
						zIndex = xArh.FindFile(pszUserHeader);
					}
					//ZIP_INDEX_TYPE zIndex2;

					if(zIndex != ZIP_FILE_INDEX_NOT_FOUND)
					{
						xArh.RemoveFile(zIndex);

						CZipFileHeader zHeader;
						zHeader.SetFileName(pszUserHeader);
						xArh.OpenNewFile(zHeader);

						g_xBuffer.Reset();
						g_xBuffer << header;

						xArh.WriteNewFile(g_xBuffer.GetBuffer(), g_xBuffer.GetLength());
						xArh.CloseNewFile();

						zIndex = xArh.FindFile(pszUserData);
						//zIndex2 = xArh.FindFile("head");
						if(zIndex != ZIP_FILE_INDEX_NOT_FOUND)
						{
							if(!_pPkt->xData.empty())
							{
								xArh.RemoveFile(zIndex);

								CZipFileHeader zHeader;
								zHeader.SetFileName(pszUserData);
								xArh.OpenNewFile(zHeader);

								g_xBuffer.Reset();
								g_xBuffer << _pPkt->xData;

								xArh.WriteNewFile(g_xBuffer.GetBuffer(), g_xBuffer.GetLength());
								xArh.CloseNewFile();

								strcpy(szExpr, "存档成功!");
							}
							else
							{
								strcpy(szExpr, "存档数据为空!");
							}
						}
						else
						{
							//	新存档
							if(!_pPkt->xData.empty())
							{
								CZipFileHeader zHeader;
								zHeader.SetFileName(pszUserData);
								xArh.OpenNewFile(zHeader);

								g_xBuffer.Reset();
								g_xBuffer << _pPkt->xData;

								xArh.WriteNewFile(g_xBuffer.GetBuffer(), g_xBuffer.GetLength());
								xArh.CloseNewFile();

								strcpy(szExpr, "存档成功!");
							}
							else
							{
								strcpy(szExpr, "存档数据为空!");
							}
						}

						xArh.Close();

						//	修改所有物品属性为绑定
						ItemList& bagitems = GamePlayer::GetInstance()->GetPlayerBag()->GetItemList();
						ItemList& astitems = GamePlayer::GetInstance()->GetPlayerBag()->GetAssistItemList();

						for(int i = 0; i < HERO_MAINBAG_SIZE_CUR; ++i)
						{
							if(bagitems[i].type != ITEM_NO)
							{
								//bagitems[i].atkPois = 1;
								SET_FLAG(bagitems[i].atkPois, POIS_MASK_BIND);
								if(TEST_FLAG_BOOL(bagitems[i].EXPR, EXPR_MASK_NOSAVE))
								{
									ZeroMemory(&bagitems[i], sizeof(ItemAttrib));
								}
							}
						}
						for(int i = 0; i < HERO_ASSISTBAG_SIZE_CUR; ++i)
						{
							if(astitems[i].type != ITEM_NO)
							{
								//astitems[i].atkPois = 1;
								SET_FLAG(astitems[i].atkPois, POIS_MASK_BIND);
							}
						}
						for(int i = 0; i < PLAYER_ITEM_TOTAL; ++i)
						{
							if(GamePlayer::GetInstance()->GetPlayerItem((PLAYER_ITEM_TYPE)i)->type != ITEM_NO)
							{
								//GamePlayer::GetInstance()->GetPlayerItem((PLAYER_ITEM_TYPE)i)->atkPois = 1;
								SET_FLAG(GamePlayer::GetInstance()->GetPlayerItem((PLAYER_ITEM_TYPE)i)->atkPois, POIS_MASK_BIND);
							}
						}
					}
					else
					{
						strcpy(szExpr, "无法定位文件头");
						xArh.Close();
					}
				}
				else
				{
					strcpy(szExpr, "无法打开存档文件");
				}
			} while (0);
		}
		else
		{
			strcpy(szExpr, "无法定位存档文件");
		}
	}

	//GameScene::sThis->GetMainOpt()->GetStaticDlg()->Create(szExpr, 500);
	GameScene::sThis->GetMainOpt()->GetQuitDlg()->CreateMsgDlg(szExpr);
	if(GamePlayer::GetInstance()->IsRequestSmallQuit())
	{
		GameScene::sThis->GetMainOpt()->GetQuitDlg()->CloseDialog();
	}
	return true;
}

bool SocketDataCenter::SaveExtHumData_ZipArchiveMulti(const PkgSystemExtUserDataAck& ack)
{
	static const char* s_szHeader[] =
	{
		"head",
		"head1",
		"head2"
	};
	static const char* s_szData[] =
	{
		"data",
		"data1",
		"data2"
	};

	char szPath[MAX_PATH];
	if(pTheGame->GetGameMode() == GM_LOGIN)
	{
		sprintf(szPath, "%s\\NetSave\\hum.sav",
			GetRootPath());
	}
	else
	{
		sprintf(szPath, "%s\\Save\\hum.sav",
			GetRootPath());
	}

	char szExpr[100];
	GamePlayer* pPlayer = GamePlayer::GetInstance();

	char szExtFile[10] = {0};
	sprintf(szExtFile, "ext%d_%d", ack.nExtIndex, pPlayer->GetSaveIndex());
	int nSaveIndex = -1;

	char szHeroName[20] = {0};
	pPlayer->GetHeroNameInMask(szHeroName);

	do 
	{
		if(ack.xData.empty())
		{
			strcpy(szExpr, "无效的扩展数据");
			break;
		}

		if(!PathFileExists(szPath))
		{
			strcpy(szExpr, "找不到存档文件");
			break;
		}

		//	can open save file...
		bool bOpened = false;
		CZipArchive xArh;

		do 
		{
			if(!xArh.Open(szPath))
			{
				strcpy(szExpr, "无法打开存档文件");
				break;
			}
			bOpened = true;

			xArh.SetPassword(SaveFile::CalcInternalPassword());

			ZIP_INDEX_TYPE zIndex = ZIP_FILE_INDEX_NOT_FOUND;

			for(int i = 0; i < 3; ++i)
			{
				zIndex = xArh.FindFile(s_szHeader[i]);
				HeroHeader heroheader;

				if(zIndex != ZIP_FILE_INDEX_NOT_FOUND)
				{
					CZipFileHeader* pHeader = NULL;
					pHeader = xArh.GetFileInfo(zIndex);

					if(pHeader)
					{
						xArh.OpenFile(zIndex);
						char* pBuf = new char[pHeader->m_uUncomprSize];
						xArh.ReadFile(pBuf, pHeader->m_uUncomprSize);
						g_xBuffer.Reset();
						g_xBuffer.Write(pBuf, pHeader->m_uUncomprSize);
						SAFE_DELETE_ARRAY(pBuf);
						xArh.CloseFile();

						g_xBuffer >> heroheader;
						if(0 == strcmp(heroheader.szName, szHeroName))
						{
							nSaveIndex = i;
							break;
						}
					}
				}
			}

			if(-1 == nSaveIndex)
			{
				strcpy(szExpr, "无法定位存档人物");
				break;
			}

			sprintf(szExtFile, "ext%d_%d", ack.nExtIndex, nSaveIndex);

			zIndex = xArh.FindFile(szExtFile);
			if(zIndex != ZIP_FILE_INDEX_NOT_FOUND)
			{
				xArh.RemoveFile(zIndex);
			}
			
			CZipFileHeader zHeader;
			zHeader.SetFileName(szExtFile);
			xArh.OpenNewFile(zHeader);

			g_xBuffer.Reset();
			g_xBuffer << ack.xData;
			xArh.WriteNewFile(g_xBuffer.GetBuffer(), g_xBuffer.GetLength());
			xArh.CloseNewFile();

			GameMainOptUI::GetInstance()->GetBigStoreDlg()->BindAllItem();
			strcpy(szExpr, "保存人物扩展数据成功");
		} while (false);

		if(bOpened)
		{
			xArh.Close();
		}
	} while (false);

	GameScene::sThis->GetMainOpt()->GetQuitDlg()->CreateMsgDlg(szExpr);
	if(GamePlayer::GetInstance()->IsRequestSmallQuit())
	{
		GameScene::sThis->GetMainOpt()->GetQuitDlg()->CloseDialog();
	}
	return true;
}

bool SocketDataCenter::SaveHumData_ZipArchiveMulti(const PkgSystemUserDataAck* _pPkt)
{
	char szPath[MAX_PATH];
	sprintf(szPath, "%s\\Save\\hum.sav",
		GetRootPath());

	char szExpr[100];
	GamePlayer* pPlayer = GamePlayer::GetInstance();

	static const char* s_szHeader[] =
	{
		"head",
		"head1",
		"head2"
	};
	static const char* s_szData[] =
	{
		"data",
		"data1",
		"data2"
	};

	const char* pszUserHeader = s_szHeader[pPlayer->GetSaveIndex()];
	const char* pszUserData = s_szData[pPlayer->GetSaveIndex()];

	int nSaveIndex = -1;

	if(_pPkt->xData.empty())
	{
		strcpy(szExpr, "存档数据为空!");
	}
	else
	{
		if(PathFileExists(szPath))
		{
			do 
			{
				CZipArchive xArh;
				if(xArh.Open(szPath))
				{
					xArh.SetPassword(SaveFile::CalcInternalPassword());

					HeroHeader header;
					//header.bJob = pPlayer->GetAttrib()->price;
					header.bJob = pPlayer->GetHeroJob();
					header.bSex = pPlayer->GetAttrib()->sex;
					//strcpy(header.szName, pPlayer->GetAttrib()->name);
					pPlayer->GetHeroNameInMask(header.szName);
					header.uLevel = pPlayer->GetAttrib()->level;

					ZIP_INDEX_TYPE zIndex = ZIP_FILE_INDEX_NOT_FOUND;

					for(int i = 0; i < 3; ++i)
					{
						zIndex = xArh.FindFile(s_szHeader[i]);
						HeroHeader heroheader;

						if(zIndex != ZIP_FILE_INDEX_NOT_FOUND)
						{
							CZipFileHeader* pHeader = NULL;
							pHeader = xArh.GetFileInfo(zIndex);

							if(pHeader)
							{
								xArh.OpenFile(zIndex);
								char* pBuf = new char[pHeader->m_uUncomprSize];
								xArh.ReadFile(pBuf, pHeader->m_uUncomprSize);
								g_xBuffer.Reset();
								g_xBuffer.Write(pBuf, pHeader->m_uUncomprSize);
								SAFE_DELETE_ARRAY(pBuf);
								xArh.CloseFile();

								g_xBuffer >> heroheader;
								if(0 == strcmp(heroheader.szName, header.szName))
								{
									nSaveIndex = i;
									pszUserHeader = s_szHeader[i];
									pszUserData = s_szData[i];
									break;
								}
							}
						}
					}

					if(header.szName[0] == 0 ||
						nSaveIndex == -1)
					{
						strcpy(szExpr, "存档数据出现错误!");
						xArh.Close();
						break;
					}

					zIndex = xArh.FindFile(pszUserHeader);
					//ZIP_INDEX_TYPE zIndex2;

					if(zIndex != ZIP_FILE_INDEX_NOT_FOUND)
					{
						xArh.RemoveFile(zIndex);

						CZipFileHeader zHeader;
						zHeader.SetFileName(pszUserHeader);
						xArh.OpenNewFile(zHeader);

						g_xBuffer.Reset();
						g_xBuffer << header;

						xArh.WriteNewFile(g_xBuffer.GetBuffer(), g_xBuffer.GetLength());
						xArh.CloseNewFile();

						zIndex = xArh.FindFile(pszUserData);
						//zIndex2 = xArh.FindFile("head");
						if(zIndex != ZIP_FILE_INDEX_NOT_FOUND)
						{
							if(!_pPkt->xData.empty())
							{
								xArh.RemoveFile(zIndex);

								CZipFileHeader zHeader;
								zHeader.SetFileName(pszUserData);
								xArh.OpenNewFile(zHeader);

								g_xBuffer.Reset();
								g_xBuffer << _pPkt->xData;

								xArh.WriteNewFile(g_xBuffer.GetBuffer(), g_xBuffer.GetLength());
								xArh.CloseNewFile();

								strcpy(szExpr, "保存人物数据成功!");
							}
							else
							{
								strcpy(szExpr, "存档数据为空!");
							}
						}
						else
						{
							//	新存档
							if(!_pPkt->xData.empty())
							{
								CZipFileHeader zHeader;
								zHeader.SetFileName(pszUserData);
								xArh.OpenNewFile(zHeader);

								g_xBuffer.Reset();
								g_xBuffer << _pPkt->xData;

								xArh.WriteNewFile(g_xBuffer.GetBuffer(), g_xBuffer.GetLength());
								xArh.CloseNewFile();

								strcpy(szExpr, "保存人物数据成功!");
							}
							else
							{
								strcpy(szExpr, "存档数据为空!");
							}
						}

						xArh.Close();

						//	修改所有物品属性为绑定
						ItemList& bagitems = GamePlayer::GetInstance()->GetPlayerBag()->GetItemList();
						ItemList& astitems = GamePlayer::GetInstance()->GetPlayerBag()->GetAssistItemList();

						for(int i = 0; i < HERO_MAINBAG_SIZE_CUR; ++i)
						{
							if(bagitems[i].type != ITEM_NO)
							{
								//bagitems[i].atkPois = 1;
								SET_FLAG(bagitems[i].atkPois, POIS_MASK_BIND);
							}
						}
						for(int i = 0; i < HERO_ASSISTBAG_SIZE_CUR; ++i)
						{
							if(astitems[i].type != ITEM_NO)
							{
								//astitems[i].atkPois = 1;
								SET_FLAG(astitems[i].atkPois, POIS_MASK_BIND);
							}
						}
						for(int i = 0; i < PLAYER_ITEM_TOTAL; ++i)
						{
							if(GamePlayer::GetInstance()->GetPlayerItem((PLAYER_ITEM_TYPE)i)->type != ITEM_NO)
							{
								//GamePlayer::GetInstance()->GetPlayerItem((PLAYER_ITEM_TYPE)i)->atkPois = 1;
								SET_FLAG(GamePlayer::GetInstance()->GetPlayerItem((PLAYER_ITEM_TYPE)i)->atkPois, POIS_MASK_BIND);
							}
						}
						GameMainOptUI::GetInstance()->GetStoreDlg2()->BindAllItem();
					}
					else
					{
						strcpy(szExpr, "无法定位文件头");
						xArh.Close();
					}
				}
				else
				{
					strcpy(szExpr, "无法打开存档文件");
				}
			} while (0);
		}
		else
		{
			strcpy(szExpr, "无法定位存档文件");
		}
	}

	//GameScene::sThis->GetMainOpt()->GetStaticDlg()->Create(szExpr, 500);
	GameScene::sThis->GetMainOpt()->GetQuitDlg()->CreateMsgDlg(szExpr);
	if(GamePlayer::GetInstance()->IsRequestSmallQuit())
	{
		GameScene::sThis->GetMainOpt()->GetQuitDlg()->CloseDialog();
	}
	return true;
}

bool SocketDataCenter::SaveHumData_SaveFile(const char* _pszFile, const PkgSystemUserDataAck* _pPkt)
{
	BOOL bDeleteOk = true;
	BOOL bSaveOK = true;
	char szExpr[100];
	GamePlayer* pPlayer = GamePlayer::GetInstance();

	if(PathFileExists(_pszFile))
	{
		bDeleteOk = DeleteFile(_pszFile);
	}
	if(bDeleteOk)
	{
		//	First write the header!
		HeroHeader header;
		//header.bJob = pPlayer->GetAttrib()->price;
		header.bJob = pPlayer->GetHeroJob();
		header.bSex = pPlayer->GetAttrib()->sex;
		strcpy(header.szName, pPlayer->GetAttrib()->name);
		header.uLevel = pPlayer->GetAttrib()->level;

		SaveFile xSav;
		bSaveOK = xSav.Load(_pszFile);
		if(!bSaveOK)
		{
			strcpy(szExpr, "无法创建存档文件!");
		}
		else
		{
			bSaveOK = xSav.WriteHeader(&header);
			if(!bSaveOK)
			{
				strcpy(szExpr, "无法创建文件头!");
			}
			else
			{
				g_xBuffer.Reset();
				g_xBuffer << _pPkt->xData;
				bSaveOK = xSav.WriteData((const char*)g_xBuffer.GetBuffer(), g_xBuffer.GetLength());
				if(!bSaveOK)
				{
					strcpy(szExpr, "无法写入存档数据!");
				}
				else
				{
					strcpy(szExpr, "存档成功!");

					//	修改所有物品属性为绑定
					ItemList& bagitems = GamePlayer::GetInstance()->GetPlayerBag()->GetItemList();
					ItemList& astitems = GamePlayer::GetInstance()->GetPlayerBag()->GetAssistItemList();

					for(int i = 0; i < HERO_MAINBAG_SIZE_CUR; ++i)
					{
						if(bagitems[i].type != ITEM_NO)
						{
							//bagitems[i].atkPois = 1;
							SET_FLAG(bagitems[i].atkPois, POIS_MASK_BIND);
						}
					}
					for(int i = 0; i < HERO_ASSISTBAG_SIZE_CUR; ++i)
					{
						if(astitems[i].type != ITEM_NO)
						{
							//astitems[i].atkPois = 1;
							SET_FLAG(astitems[i].atkPois, POIS_MASK_BIND);
						}
					}
					for(int i = 0; i < PLAYER_ITEM_TOTAL; ++i)
					{
						if(GamePlayer::GetInstance()->GetPlayerItem((PLAYER_ITEM_TYPE)i)->type != ITEM_NO)
						{
							//GamePlayer::GetInstance()->GetPlayerItem((PLAYER_ITEM_TYPE)i)->atkPois = 1;
							SET_FLAG(GamePlayer::GetInstance()->GetPlayerItem((PLAYER_ITEM_TYPE)i)->atkPois, POIS_MASK_BIND);
						}
					}
				}
			}

			xSav.Close();
		}

		GameScene::sThis->GetMainOpt()->GetStaticDlg()->Create(szExpr, 500);
	}
	else
	{
		//GameScene::sThis->GetMainOpt()->GetStaticDlg()->Create("删除存档文件失败!请检查是否占用!", 3000);
		GameScene::sThis->GetMainOpt()->GetQuitDlg()->CreateMsgDlg("删除存档文件失败!请检查是否占用!");
	}
	return true;
}

void SocketDataCenter::DoPacket(const PkgSystemExtUserDataAck& ack)
{
	//	Get the data
	GamePlayer* pPlayer = GamePlayer::GetInstance();
	SaveExtHumData_ZipArchiveMulti(ack);
}

void SocketDataCenter::DoPacket_SystemUserDataAck(const PkgSystemUserDataAck* _pPkt)
{
	//	Get the data
	GamePlayer* pPlayer = GamePlayer::GetInstance();
	//char szExpr[100];

	if(_pPkt->xData.empty())
	{
		//GameScene::sThis->GetMainOpt()->GetStaticDlg()->Create("请求服务器数据失败!数据为空!", 3000);
		GameScene::sThis->GetMainOpt()->GetQuitDlg()->CreateMsgDlg("背包内含有无法存档物品,请丢弃后存档!");
	}
	else
	{
		char szSavFile[MAX_PATH];
#ifdef _DEBUG
		sprintf(szSavFile, "%s\\Save\\Debug\\%s.sav",
			GetRootPath(), pPlayer->GetAttrib()->name);
#else
		sprintf(szSavFile, "%s\\Save\\%s.sav",
			GetRootPath(), pPlayer->GetAttrib()->name);
#endif

#ifdef _ZIPARCHIVE_
#ifdef _SELCHR_
		if(pTheGame->GetGameMode() == GM_NORMAL)
		{
			SaveHumData_ZipArchiveMulti(_pPkt);
		}
		else if(pTheGame->GetGameMode() == GM_LOGIN)
		{
			SaveHumData_ZipArchiveMultiLogin(_pPkt);
		}
#else
		SaveHumData_ZipArchive(szSavFile, _pPkt);
#endif
#else
		SaveHumData_SaveFile(szSavFile, _pPkt);
#endif
		string strName = pPlayer->GetAttrib()->name;
		string strFile;
		Base64::Encode(strName, &strFile);
		sprintf(szSavFile, "%s\\Save\\%s.cfg",
			GetRootPath(), strFile.c_str());
		SaveHumConfig(szSavFile);
		pPlayer->WriteAccMagicKeyCfg();
		pPlayer->WriteBagItemOrder();
	}
}

bool SocketDataCenter::SaveHumConfig(const char* _pszFile)
{
	int nAttribID = 0;
	g_xBuffer.Reset();

	for(int i = 0; i < HERO_CLIENT_ASSIST_SIZE; ++i)
	{
		if(GamePlayer::GetInstance()->GetPlayerBag()->GetAssistItemList()[i].type != ITEM_NO)
		{
			nAttribID = GamePlayer::GetInstance()->GetPlayerBag()->GetAssistItemList()[i].id;
		}
		else
		{
			nAttribID = 0;
		}
		g_xBuffer << nAttribID;
	}
	MagicDetail* pDetail = NULL;
	GamePlayer::GetInstance()->WriteMagicKeyCfg(&g_xBuffer);

	HANDLE hCfgFile = ::CreateFile(_pszFile, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, NULL, NULL);
	DWORD dwWrite = 0;
	if(hCfgFile != INVALID_HANDLE_VALUE)
	{
		WriteFile(hCfgFile, g_xBuffer.GetBuffer(), g_xBuffer.GetLength(), &dwWrite, NULL);
		if(dwWrite == g_xBuffer.GetLength())
		{
			AfxGetHge()->System_Log("人物操作配置保存成功");
		}
	}
	else
	{
		AfxGetHge()->System_Log("无法保存人物配置");
	}
	CloseHandle(hCfgFile);
	return true;
}

//////////////////////////////////////////////////////////////////////////
void SocketDataCenter::DoPacket(const PkgSystemNewItemNot& not)
{
	if(pTheGame->GetCurStage() != SCENE_GAME)
	{
		//return;
	}
	GroundItem item;
	item.bVisible = true;
	item.stAttrib.id = not.dwID;
	GameInfoManager::GetInstance()->GetItemAttrib(not.dwID, &item.stAttrib);
	item.stAttrib.tag = not.dwTag;
	item.wPosX = LOWORD(not.dwPos);
	item.wPosY = HIWORD(not.dwPos);
	item.stAttrib.level = not.uUserId;
	item.stAttrib.maxMP = not.dwItemHideAttrib;
	if(!pTheGame->GetAssistPaneWnd()->CheckItemVisible(item.stAttrib.name) &&
		0 == GetItemUpgrade(item.stAttrib.level))
	{
		item.bVisible = false;
	}
	GameScene::sThis->InsertNewGroundItem(&item);
}
//////////////////////////////////////////////////////////////////////////
void SocketDataCenter::DoPacket(const PkgSystemDelItemNot& not)
{
	if(pTheGame->GetCurStage() != SCENE_GAME)
	{
		//return;
	}
	GameScene::sThis->RemoveGroundItem(not.dwTag);
}
//////////////////////////////////////////////////////////////////////////
void SocketDataCenter::DoPacket(const PkgDelNPCNot& not)
{
	if(pTheGame->GetCurStage() != SCENE_GAME)
	{
		//return;
	}
	if(GameScene::sThis->RemoveNPC(not.uTargetId))
	{
		AfxGetHge()->System_Log("清除怪物[%d]",
			not.uTargetId);
	}
	else
	{
		AfxGetHge()->System_Log("无法清除怪物[%d]",
			not.uTargetId);
	}
}
//////////////////////////////////////////////////////////////////////////
void SocketDataCenter::DoPacket(const PkgSystemClearGroundItemNtf& ntf)
{
	if(pTheGame->GetCurStage() != SCENE_GAME)
	{
		//return;
	}
	GameScene::sThis->RemoveGroundItem(ntf.dwTag);
}
//////////////////////////////////////////////////////////////////////////
void SocketDataCenter::DoPacket(const PkgGmNotificationNot& not)
{
	if(pTheGame->GetCurStage() != SCENE_GAME)
	{
		//return;
	}
	GameScene::sThis->ShowGmNotification(not.xMsg);
}
//////////////////////////////////////////////////////////////////////////
void SocketDataCenter::DoPacket(const PkgSystemNotifyNot& not)
{
	if(pTheGame->GetCurStage() != SCENE_GAME)
	{
		//return;
	}
	DWORD dwColor = not.dwColor;
	if(0 == dwColor)
	{
		dwColor = ARGB_WHITE;
	}
	GameScene::sThis->ShowSystemNotify(not.xMsg, not.dwColor, not.dwTimes);
}
//////////////////////////////////////////////////////////////////////////
void SocketDataCenter::DoPacket(const PkgSystemClientVersionErrNtf& not)
{
	std::string xMsg = "您的客户端版本与服务端版本不符，服务端版本号：";
	xMsg += not.xServerVersion;
	ALERT_MSGBOX(xMsg.c_str());
}
//////////////////////////////////////////////////////////////////////////
void SocketDataCenter::DoPacket(const PkgSystemHideDoorNtf& not)
{

}
//////////////////////////////////////////////////////////////////////////
void SocketDataCenter::DoPacket(const PkgSystemWorldSayNot& not)
{
	if(pTheGame->GetCurStage() != SCENE_GAME)
	{
		return;
	}

	std::string xSay;
	xSay = "[世界]";
	xSay += not.xName;
	xSay += "说:";
	xSay += not.xMsg;

	GameScene::sThis->GetMainOpt()->GetChatDlg()->GetHistoryDlg()->InsertPublicChatMessage(not.xName.c_str(), xSay.c_str(), 0xffFF00FF);
}
/************************************************************************/
/* virtual void ProcessGamePacket(const PacketHeader* _pPkt)
/************************************************************************/
void SocketDataCenter::ProcessGamePacket(const PacketHeader* _pPkt)
{
	if(pTheGame->GetCurStage() != SCENE_GAME)
	{
		//return;
	}
	//
	PacketHandler* pHandler = NULL;
	HandlerMap::const_iterator iterFind = m_xHandlers.find(_pPkt->uTargetId);

	if(iterFind == m_xHandlers.end() ||
		iterFind->second == NULL)
	{
		//??
		AfxGetHge()->System_Log("处理游戏数据包[%d]时，由于寻找不到目标[%d]，数据包忽略",
			_pPkt->uOp, _pPkt->uTargetId);
		PkgPlayerMonsInfoReq req;
		req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
		req.uTargetId = _pPkt->uTargetId;
		g_xBuffer.Reset();
		g_xBuffer << req;
		SendBufferToGS(&g_xBuffer);
		return;
	}
	pHandler = iterFind->second;
	pHandler->OnPacket(_pPkt);
}

/************************************************************************/
/* Game packet processor
/************************************************************************/
void SocketDataCenter::DoPacket_GameObjectActionNot(const PkgObjectActionNot* _pPkt)
{
	//
}

/************************************************************************/
/* void RegisterObject(PacketHandler* _pHandler)
/************************************************************************/
bool SocketDataCenter::RegisterObject(PacketHandler* _pHandler)
{
	HandlerMap::iterator iterFind = m_xHandlers.find(_pHandler->GetHandlerID());
	if(iterFind != m_xHandlers.end())
	{
		//??
		return false;
	}
	if(_pHandler->GetHandlerID() == UNDEFINE_OBJECT_ID)
	{
		//??
		return false;
	}
	m_xHandlers.insert(std::make_pair(_pHandler->GetHandlerID(), _pHandler));
	return true;
}

/************************************************************************/
/* void UnregisterObject(int _nId)
/************************************************************************/
void SocketDataCenter::UnregisterObject(int _nId)
{
	HandlerMap::iterator iterFind = m_xHandlers.find(_nId);
	if(iterFind != m_xHandlers.end())
	{
		m_xHandlers.erase(iterFind);
	}
}

//////////////////////////////////////////////////////////////////////////
/************************************************************************/
/* static void ProcessData(void* _pData, int _nLen)
/* Param:	_pData: the received data
/*			_nLen:	the received data length
/************************************************************************/
void SocketDataCenter::ProcessData(const void* _pData, unsigned int _nLen)
{
	const PacketHeader* pPacket = (const PacketHeader*)_pData;
	PacketHeader* pReadPacket = const_cast<PacketHeader*>(pPacket);
	pReadPacket->uLen = _nLen;

	unsigned int uOpCode = pPacket->uOp;
	//AfxGetHge()->System_Log("处理数据包[%d]", uOpCode);

	if(uOpCode >= GAME_PKG_HEADER &&
		uOpCode < GAME_PKG_END)
	{
		if(uOpCode > PKG_SYSTEM_START &&
			uOpCode < PKG_SYSTEM_END)
		{
			GetInstancePtr()->ProcessSystemPacket(pPacket);
		}
		else if(uOpCode > PKG_GAME_START &&
			uOpCode < PKG_GAME_END)
		{
			GetInstancePtr()->ProcessGamePacket(pPacket);
		}
		else
		{
			//	undefine code??
			AfxGetHge()->System_Log("未处理的数据包[%d]",
				uOpCode);
		}
	}
	else
	{
		//	undefine code??
		AfxGetHge()->System_Log("未定义的数据包[%d]",
			uOpCode);
	}
	//AfxGetHge()->System_Log("处理数据包完毕[%d]", uOpCode);
}











//////////////////////////////////////////////////////////////////////////
/*
 *	PacketHandler
 */
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
PacketHandler::PacketHandler()
{
	m_nUniqueID = UNDEFINE_OBJECT_ID;
}

PacketHandler::~PacketHandler()
{
	//
}

//////////////////////////////////////////////////////////////////////////
/************************************************************************/
/* virtual void OnPacket(const PacketHeader* _pPkt)
/************************************************************************/
void PacketHandler::OnPacket(const PacketHeader* _pPkt)
{
	//	Entry
}