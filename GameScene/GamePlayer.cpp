#include "../GameScene/GamePlayer.h"
#include <math.h>
#include "../GameScene/GameResourceManager.h"
#include "../GameScene/GameMonster.h"
#include "../GameDialog/GameInfoBoardDlg.h"
#include "../GameScene/GameMapManager.h"
#include "../GameDialog/GameDlgBase.h"
#include "../GameDialog/GameInteractiveDlg.h"
#include "../GameScene/GameScene.h"
#include "../Common/SKnl3Helper.h"
#include "../Common/view.h"
#include "../../CommonModule/SaveFile.h"
#include "../../CommonModule/ByteBuffer.h"
#include "../GameDialog/GameChatDlg.h"
#include "../GameScene/GameNPC.h"
#include "../GameDialog/GameStoreDlg2.h"
#include "../GameDialog/GameShopDlg2.h"
#include "../GameDialog/GameIdentifyDlg.h"
#include "../BackMir/BackMir.h"
#include "../GameDialog/GameRoleInfoDlg.h"
#include "../../CommonModule/ItemGrowUtils.h"
#include "../../CommonModule/HideAttribHelper.h"
#include "../../CommonModule/PotentialAttribHelper.h"
#include "../../CommonModule/StoveManager.h"
#include <io.h>
//#define ZLIB_WINAPI
#include <zlib.h>

#ifdef _ZIPARCHIVE_
#include <ZipArchive.h>
#endif
//////////////////////////////////////////////////////////////////////////
//	bag sort
bool PlayerBag::PlayerBagSort(const ItemAttrib& _refL, const ItemAttrib& _refR)
{
	if(_refL.type > _refR.type)
	{
		return true;
	}
	else if(_refL.type == _refR.type)
	{
		if(_refL.id > _refR.id)
		{
			return true;
		}
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////
extern ByteBuffer g_xBuffer;
//////////////////////////////////////////////////////////////////////////

GamePlayer::GamePlayer()
{
	Reset();
	m_pSprHair = NULL;
	m_pSprWeapon = NULL;
	m_pSprState = NULL;
	m_pSprHum = NULL;
	m_pSprMgc = NULL;
}

void GamePlayer::Reset()
{
	SetType(OBJ_PLAYER);
	ZeroMemory(&m_equipAttrib, sizeof(m_equipAttrib));
	ZeroMemory(m_equip, sizeof(m_equip));
	ZeroMemory(&m_indexs, sizeof(m_indexs));
	ZeroMemory(m_stMagicDetail, sizeof(m_stMagicDetail));
	ZeroMemory(m_szSaveName, sizeof(m_szSaveName));
	m_atkTarget = NULL;
	m_nMapID = 0;
	m_bIsLocked = false;
	m_bIsWalkLocked = false;
	m_pMgcTarget = NULL;
	m_dwLockType = PST_NONE;
	m_dwLastUseMagicTime = 0;
	m_nSaveIndex = 0;
	m_dwLastAttackTime = 0;
	m_nAccMagicID = 0;
	m_bUsingPreLock = true;
	m_nDonateLeft = 0;
	m_bRequestSmallQuit = false;

	//	初始化矩形
	RECT rc = {385, 125, 445, 255};
	SetRect(rc);

	ZeroMemory(m_szNameCopy, sizeof(m_szNameCopy));
	m_uUID = 0;

	m_nWaitServerRspType = 0;
	m_dwWaitServerRspTimeoutTime = 0;

	m_dwPreCalcNormalAttackEffMask = 0;
	m_bEnableLieHuo = false;
	m_bEnableSuperLieHuo = false;
	m_bEnableBanYue = false;
	m_bEnableCiSha = false;
	m_bEnableKtSword = false;
	m_bUsingPreLock = false;

	m_fDetect = 0.0f;
	m_fMoveOffsetXTotal = 0.0f;
	m_fMoveOffsetYTotal = 0.0f;
	m_fLastUpdateSkill = 0.0f;
	m_fLastUpdateAttack = 0.0f;
	m_fLastUpdateDeath = 0.0f;
	m_fLastUpdateAttackNoWeapon = 0.0f;
	m_fLastUpdateAttackStop = 0.0f;
	m_fLastUpdateAttacked = 0.0f;
	m_fLastUpdateMove = 0.0f;

	m_dwLastAttackTime = 0;
	m_dwLastAttackStopTime = 0;

	m_dwLastAttackMode = 0;

	for(int i = 0; i < MAGICEFFECT_TOTAL; ++i)
	{
		m_eEffect[i] = ME_NONE;
	}

	ZeroMemory(m_dwHumEffectTime, sizeof(m_dwHumEffectTime));
	m_dwHumEffectFlag = 0;
	m_pMagicEffect = NULL;

	m_bPlayOneStep = m_bPlayTwoStep = false;

	m_dwJinGangExpireTime = 0;
	m_nLieHuoSkillLevel = 0;
	m_nVipLevel = 0;
	m_dwLastNameColorChangeTime = 0;
	m_nLastNameColorIndex = 0;

	ZeroMemory(&m_stExtAttrib, sizeof(m_stExtAttrib));

	m_xInputMap.clear();
	m_xAccInputMap.clear();
	m_xMgcHistory.clear();
	m_xDrugCoolDown.Clear();
	m_xQuest.Clear();
	m_bag.Clear();
}

GamePlayer::~GamePlayer()
{
	delete m_pSprHum;
	delete m_pSprHair;
	delete m_pSprState;
	delete m_pSprWeapon;
	delete m_pSprMgc;
}

int GamePlayer::GetScreenOffsetX()
{
	return (int)m_fPosx - VIEW_WIDTH / 2;
}

int GamePlayer::GetScreenOffsetY()
{
	return (int)m_fPosy - VIEW_HEIGHT / 2;
}

bool GamePlayer::CanAttack()
{
	if(m_dwLastAttackMode == 0)
	{
		DWORD dwNextAttackTime = m_dwLastAttackTime + 80 * 6 + 300;
		int nAttackInterval = 300 - m_attrib.atkSpeed * 37;
		//	??
		if(m_bUsingPreLock)
		{
			nAttackInterval -= (int)GameScene::sThis->GetNetDelayMillSeconds();
		}
		//	??
		if(nAttackInterval < 0)
		{
			nAttackInterval = 0;
		}
#ifdef _DEBUG
		//nAttackInterval = 0;
#endif
		dwNextAttackTime += nAttackInterval;
		if(GetTickCount() > dwNextAttackTime)
		{
			return true;
		}
		return false;
	}
	else if(m_dwLastAttackMode == 1)
	{
		DWORD dwNextAttackTime = m_dwLastAttackTime + 80 * 6 + 300;
		if(GetTickCount() > dwNextAttackTime)
		{
			return true;
		}
		return false;
	}
	return false;
}

void GamePlayer::Render()
{
	static const int nRenderX = CENTRAL_X;
	static const int nRenderY = CENTRAL_Y;

	__super::Render();
	return;

	HGE* hge = AfxGetHge();

	GameTextureManager* ttexs = NULL;

	//assert(_CrtCheckMemory() && "BEGIN");

	switch (GetDirection())
	{
	case PDT_LEFT:
	case PDT_LEFTDOWN:
	case PDT_LEFTUP:
		{
			RenderWeapon();
			RenderHum();
			RenderHair();
			RenderWing();
		}break;
	default:
		{
			//	反着
			bool bWingRendered = false;
			if(GetDirection() == PDT_DOWN ||
				GetDirection() == PDT_RIGHTDOWN ||
				GetDirection() == PDT_RIGHT ||
				GetDirection() == PDT_LEFTUP)
			{
				//	画翅膀
				RenderWing();
				bWingRendered = true;
			}
			RenderHum();
			RenderHair();
			RenderWeapon();

			if(!bWingRendered)
			{
				//	画翅膀
				RenderWing();
			}
			
			/*bool bWingRendered = false;
			if(GetDirection() == PDT_DOWN ||
				GetDirection() == PDT_RIGHTDOWN ||
				GetDirection() == PDT_RIGHT ||
				GetDirection() == PDT_LEFTUP)
			{
				//	画翅膀
				ttexs = GameResourceManager::GetInstance()->GetTexs(RES_HUMEFFECT);
				if(ttexs &&
					GetRenderMode() == ORM_TRANSPARENT
					)
				{
					int nTextIndex = CalWingIndex();
					if(nTextIndex != -1)
					{
						nTextIndex += m_bCurFrame;
						HTEXTURE tex = ttexs->GetTexture(nTextIndex);
						short sOffsetX = 0;
						short sOffsetY = 0;
						if(tex)
						{
							MagicElement::pEffectRender->SetTexture(tex);
							MagicElement::pEffectRender->SetTextureRect(0, 0, ttexs->GetTextureWidth(nTextIndex), 
								ttexs->GetTextureHeight(nTextIndex));
							sOffsetX = ttexs->GetTextureOffsetX(nTextIndex);
							sOffsetY = ttexs->GetTextureOffsetY(nTextIndex);
							MagicElement::pEffectRender->Render(nRenderX + sOffsetX, nRenderY + sOffsetY);
						}	
					}
				}
				bWingRendered = true;
			}

			{
				//	画人物
				int nResIndex = 0;
				m_nCurrentTextureIndex = CalTextureIndex(NULL, &nResIndex) + m_bCurFrame;

				ttexs = GameResourceManager::GetInstance()->GetTexs(RES_HUM + nResIndex);
				if(!ttexs)
				{
					hge->System_Log("没有载入人物资源");
				}

				HTEXTURE tex = ttexs->GetTexture(m_nCurrentTextureIndex);
				short sOffsetX = 0;
				short sOffsetY = 0;
				if(tex)
				{
					if(!m_pRender)
					{
						m_pRender = new hgeSprite(tex, 0, 0, ttexs->GetTextureWidth(m_nCurrentTextureIndex), 
							ttexs->GetTextureHeight(m_nCurrentTextureIndex));
					}
					else
					{
						m_pRender->SetTexture(tex);
						m_pRender->SetTextureRect(0, 0, ttexs->GetTextureWidth(m_nCurrentTextureIndex), 
							ttexs->GetTextureHeight(m_nCurrentTextureIndex));
					}
					sOffsetX = ttexs->GetTextureOffsetX(m_nCurrentTextureIndex);
					sOffsetY = ttexs->GetTextureOffsetY(m_nCurrentTextureIndex);
					m_pRender->Render(nRenderX + sOffsetX, nRenderY + sOffsetY);
				}
			}

			{
				//	画发型
				ttexs = GameResourceManager::GetInstance()->GetTexs(RES_HAIR);
				if(!ttexs)
				{
					return;
				}

				HTEXTURE tex = ttexs->GetTexture(CalHairTexIndex() + m_bCurFrame);
				short sOffsetX = 0;
				short sOffsetY = 0;
				if(tex)
				{
					if(!m_pRender)
					{
						m_pRender = new hgeSprite(tex, 0, 0, ttexs->GetTextureWidth(CalHairTexIndex() + m_bCurFrame), 
							ttexs->GetTextureHeight(CalHairTexIndex() + m_bCurFrame));
					}
					else
					{
						m_pRender->SetTexture(tex);
						m_pRender->SetTextureRect(0, 0, ttexs->GetTextureWidth(CalHairTexIndex() + m_bCurFrame), 
							ttexs->GetTextureHeight(CalHairTexIndex() + m_bCurFrame));
					}
					sOffsetX = ttexs->GetTextureOffsetX(CalHairTexIndex() + m_bCurFrame);
					sOffsetY = ttexs->GetTextureOffsetY(CalHairTexIndex() + m_bCurFrame);
					m_pRender->Render(nRenderX + sOffsetX, nRenderY + sOffsetY);
				}
			}

			{
				//	画手上武器
				ttexs = GameResourceManager::GetInstance()->GetTexs(RES_WEAPON);
				//	画手上武器
				//if(!m_texs[PLAYER_RES_WEAPON])
				if(!ttexs)
				{
					return;
				}

				HTEXTURE tex = ttexs->GetTexture(CalWeaponTexIndex() + m_bCurFrame);
				short sOffsetX = 0;
				short sOffsetY = 0;
				if(tex)
				{
					if(!m_pRender)
					{
						m_pRender = new hgeSprite(tex, 0, 0, ttexs->GetTextureWidth(CalWeaponTexIndex() + m_bCurFrame), 
							ttexs->GetTextureHeight(CalWeaponTexIndex() + m_bCurFrame));
					}
					else
					{
						m_pRender->SetTexture(tex);
						m_pRender->SetTextureRect(0, 0, ttexs->GetTextureWidth(CalWeaponTexIndex() + m_bCurFrame), 
							ttexs->GetTextureHeight(CalWeaponTexIndex() + m_bCurFrame));
					}
					sOffsetX = ttexs->GetTextureOffsetX(CalWeaponTexIndex() + m_bCurFrame);
					sOffsetY = ttexs->GetTextureOffsetY(CalWeaponTexIndex() + m_bCurFrame);
					m_pRender->Render(nRenderX + sOffsetX, nRenderY + sOffsetY);
				}

				//RenderHitEffect();
			}

			if(!bWingRendered)
			{
				//	画翅膀
				ttexs = GameResourceManager::GetInstance()->GetTexs(RES_HUMEFFECT);
				if(ttexs &&
					GetRenderMode() == ORM_TRANSPARENT
					)
				{
					int nTextIndex = CalWingIndex();
					if(nTextIndex != -1)
					{
						nTextIndex += m_bCurFrame;
						HTEXTURE tex = ttexs->GetTexture(nTextIndex);
						short sOffsetX = 0;
						short sOffsetY = 0;
						if(tex)
						{
							MagicElement::pEffectRender->SetTexture(tex);
							MagicElement::pEffectRender->SetTextureRect(0, 0, ttexs->GetTextureWidth(nTextIndex), 
								ttexs->GetTextureHeight(nTextIndex));
							sOffsetX = ttexs->GetTextureOffsetX(nTextIndex);
							sOffsetY = ttexs->GetTextureOffsetY(nTextIndex);
							MagicElement::pEffectRender->Render(nRenderX + sOffsetX, nRenderY + sOffsetY);
						}	
					}
				}
			}*/
		}break;
	}	
}

void GamePlayer::RenderHP()
{
	static const int nRenderX = VIEW_WIDTH / 2 - 16 - 5;
	static const int nRenderY = VIEW_HEIGHT / 2 - 12;

	//	画血条
	{
		float fScale = 0.0f;
		GameTextureManager* ttex = GameResourceManager::GetInstance()->GetTexs(RES_OPUI);
		//if(m_texs[PLAER_RES_PRGUSE2])
		if(ttex)
		{
			//HTEXTURE tex = ttex->GetTexture(0);
			HTEXTURE tex = ttex->GetTexture(TEX_HUM_HPTUBEBLANK);
			short sOffsetX = 0;
			short sOffsetY = 0;
			if(tex)
			{
				if(!m_pSprState)
				{
					m_pSprState = new hgeSprite(tex, 0, 0, ttex->GetTextureWidth(TEX_HUM_HPTUBEBLANK), 
						ttex->GetTextureHeight(TEX_HUM_HPTUBEBLANK));
				}
				else
				{
					m_pSprState->SetTexture(tex);
					m_pSprState->SetTextureRect(0, 0, ttex->GetTextureWidth(TEX_HUM_HPTUBEBLANK), 
						ttex->GetTextureHeight(TEX_HUM_HPTUBEBLANK));
				}
				sOffsetX = ttex->GetTextureOffsetX(TEX_HUM_HPTUBEBLANK);
				sOffsetY = ttex->GetTextureOffsetY(TEX_HUM_HPTUBEBLANK);
				m_pSprState->Render(nRenderX + sOffsetX, nRenderY + sOffsetY - 15);
			}
			fScale = (float)m_attrib.HP / (float)m_attrib.maxHP;
			//tex = ttex->GetTexture(1);
			tex = ttex->GetTexture(TEX_HUM_HPTUBEFULL);
			if(tex)
			{
				if(!m_pSprState)
				{
					m_pSprState = new hgeSprite(tex, 0, 0, ttex->GetTextureWidth(TEX_HUM_HPTUBEFULL), 
						ttex->GetTextureHeight(TEX_HUM_HPTUBEFULL));
				}
				else
				{
					m_pSprState->SetTexture(tex);
					m_pSprState->SetTextureRect(0, 0, ttex->GetTextureWidth(TEX_HUM_HPTUBEFULL) * fScale, 
						ttex->GetTextureHeight(TEX_HUM_HPTUBEFULL));
				}
				sOffsetX = ttex->GetTextureOffsetX(TEX_HUM_HPTUBEFULL);
				sOffsetY = ttex->GetTextureOffsetY(TEX_HUM_HPTUBEFULL);
				m_pSprState->Render(nRenderX + sOffsetX, nRenderY + sOffsetY - 15);
			}
		}
	}
}

bool GamePlayer::LoadSaveData_SaveFile(const char* _pszFile)
{
	SaveFile xSave;
	bool bRet = true;

	if(!xSave.Load(_pszFile, MODE_READ))
	{
		return false;
	}

	extern ByteBuffer g_xBuffer;
	g_xBuffer.Reset();

	unsigned int dwDataSize = 0;
	unsigned int dwHeaderSize = 0;
	xSave.GetDataSize(&dwDataSize);
	xSave.GetHeaderSize(&dwHeaderSize);

	if(dwHeaderSize == 0)
	{
		xSave.Close();
		AfxGetHge()->System_Log("无效的文件头");
		return false;
	}

	HeroHeader header;
	bRet = xSave.ReadHeader(&header);
	if(bRet == false)
	{
		xSave.Close();
		return false;
	}

	PkgUserLoginReq req;
	memcpy(&req.stHeader, &header, sizeof(HeroHeader));
	//m_attrib.price = header.bJob;
	SetHeroJob(header.bJob);
	if(header.bJob > 3)
	{
		xSave.Close();
		return false;
	}
	if(header.bSex == 0 ||
		header.bSex > 2)
	{
		xSave.Close();
		return false;
	}
	m_attrib.sex = header.bSex;

	if(dwDataSize != 0)
	{
		char* pSavData = new char[dwDataSize];
		bRet = xSave.ReadData(pSavData, dwDataSize);

		if(bRet == false)
		{
			xSave.Close();
			delete[] pSavData;
			AfxGetHge()->System_Log("无法读取存档数据");
			return false;
		}

		//PkgUserLoginReq req;
		g_xBuffer.Write(pSavData, dwDataSize);
		delete[] pSavData;
		g_xBuffer >> req.xData;
		xSave.Close();
	}

	g_xBuffer.Reset();
	g_xBuffer << req;
	SendBufferToGS(&g_xBuffer);
	return true;
}

bool GamePlayer::LoadSaveData_ZipArchive(const char* _pszFile)
{
	bool bRet = true;
	CZipArchive xArh;
	char* pBuf = NULL;
	CZipFileHeader* pHeader = NULL;
	PkgUserLoginReq req;

	if(!xArh.Open(_pszFile))
	{
		bRet = false;
		return bRet;
	}

	extern ByteBuffer g_xBuffer;
	g_xBuffer.Reset();

	ZIP_INDEX_TYPE zIndex = xArh.FindFile("head");
	if(zIndex == ZIP_FILE_INDEX_NOT_FOUND)
	{
		AfxGetHge()->System_Log("无法读取文件头");
		xArh.Close();
		return false;
	}

	xArh.SetPassword(SaveFile::CalcInternalPassword());
	pHeader = xArh.GetFileInfo(zIndex);
	if(NULL == pHeader)
	{
		xArh.Close();
		return false;
	}

	xArh.OpenFile(zIndex);
	pBuf = new char[pHeader->m_uUncomprSize];
	xArh.ReadFile(pBuf, pHeader->m_uUncomprSize);
	g_xBuffer.Write(pBuf, pHeader->m_uUncomprSize);
	SAFE_DELETE_ARRAY(pBuf);

	HeroHeader hHeader;
	g_xBuffer >> hHeader;

	//m_attrib.price = hHeader.bJob;
	SetHeroJob(hHeader.bJob);
	if(hHeader.bJob > 3)
	{
		xArh.Close();
		return false;
	}
	if(hHeader.bSex == 0 ||
		hHeader.bSex > 2)
	{
		xArh.Close();
		return false;
	}
	m_attrib.sex = hHeader.bSex;

	xArh.CloseFile();
	memcpy(&req.stHeader, &hHeader, sizeof(HeroHeader));

	zIndex = xArh.FindFile("data");
	if(zIndex == ZIP_FILE_INDEX_NOT_FOUND)
	{
		xArh.Close();
		//return false;
	}

	if(!xArh.IsClosed())
	{
		pHeader = xArh.GetFileInfo(zIndex);
		if(NULL == pHeader)
		{
			xArh.Close();
			return false;
		}

		xArh.OpenFile(zIndex);
		pBuf = new char[pHeader->m_uUncomprSize];
		xArh.ReadFile(pBuf, pHeader->m_uUncomprSize);
		g_xBuffer.Reset();
		g_xBuffer.Write(pBuf, pHeader->m_uUncomprSize);
		SAFE_DELETE_ARRAY(pBuf);
		g_xBuffer >> req.xData;

		xArh.Close();
	}

	g_xBuffer.Reset();
	g_xBuffer << req;
	SendBufferToGS(&g_xBuffer);
	
	return true;
}

bool GamePlayer::LoadProperty(int _id)
{
	//assert(strlen(m_attrib.name) > 0);
	const char* pszHeroName = (const char*)_id;

	char szFile[MAX_PATH];
#ifdef _DEBUG
	sprintf(szFile, "%s\\Save\\Debug\\%s", GetRootPath(), pszHeroName);
#else
	sprintf(szFile, "%s\\Save\\%s", GetRootPath(), pszHeroName);
#endif
	if(_access(szFile, 0) != 0)
	{
		return false;
	}

#ifdef _ZIPARCHIVE_
	/*SaveFile xSave;
	bool bRet = true;

	if(!xSave.Load(szFile, MODE_READ))
	{
		return false;
	}

	extern ByteBuffer g_xBuffer;
	g_xBuffer.Reset();

	DWORD dwDataSize = 0;
	DWORD dwHeaderSize = 0;
	xSave.GetDataSize(&dwDataSize);
	xSave.GetHeaderSize(&dwHeaderSize);

	if(dwHeaderSize == 0)
	{
		xSave.Close();
		AfxGetHge()->System_Log("无效的文件头");
		return false;
	}
	
	HeroHeader header;
	bRet = xSave.ReadHeader(&header);
	if(bRet == false)
	{
		xSave.Close();
		return false;
	}

	PkgUserLoginReq req;
	memcpy(&req.stHeader, &header, sizeof(HeroHeader));
	m_attrib.price = header.bJob;
	if(header.bJob > 3)
	{
		xSave.Close();
		return false;
	}
	if(header.bSex == 0 ||
		header.bSex > 2)
	{
		xSave.Close();
		return false;
	}
	m_attrib.sex = header.bSex;

	if(dwDataSize != 0)
	{
		char* pSavData = new char[dwDataSize];
		bRet = xSave.ReadData(pSavData, dwDataSize);

		if(bRet == false)
		{
			xSave.Close();
			delete[] pSavData;
			AfxGetHge()->System_Log("无法读取存档数据");
			return false;
		}

		//PkgUserLoginReq req;
		g_xBuffer.Write(pSavData, dwDataSize);
		delete[] pSavData;
		g_xBuffer >> req.xData;
		xSave.Close();
	}

	g_xBuffer.Reset();
	g_xBuffer << req;
	SendBuffer(&g_xBuffer);
	return true;*/
	return LoadSaveData_ZipArchive(szFile);
#else
	return LoadSaveData_SaveFile(szFile);
#endif

	//return bRet;
}

bool GamePlayer::NPCClick(GameObject* _pObj)
{
	if(_pObj == NULL)
	{
		return false;
	}
	if(_pObj->GetType() == OBJ_NPC)
	{
#ifdef _NET_GAME_
		if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
		{
			GameNPC* pNPC = (GameNPC*)_pObj;
			PkgPlayerClickNPCReq req;
			req.dwButtonID = 0;
			req.uUserId = GetHandlerID();
			req.uTargetId = pNPC->GetHandlerID();
			g_xBuffer.Reset();
			g_xBuffer << req;
			SendBufferToGS(&g_xBuffer);
		}
#else
		GameMainOptUI::GetInstance()->GetActiveDlg()->ScriptCreateDlg(_pObj);
#endif
		return true;
	}
	return false;
}

bool GamePlayer::OtherPlayerClick(GameObject* _pObj)
{
	if(_pObj == NULL)
	{
		return false;
	}

	if(_pObj->GetType() == OBJ_OTHERPLAYER)
	{
		bool bAltState = AfxGetHge()->Input_GetKeyState(HGEK_ALT);
		bool bCtrlState = AfxGetHge()->Input_GetKeyState(HGEK_CTRL);
		bool bShiftState = AfxGetHge()->Input_GetKeyState(HGEK_SHIFT);

		if(bAltState &&
			AfxGetHge()->Input_KeyUp(HGEK_N))
		{
			GameMainOptUI::GetInstance()->GetChatDlg()->GetEdit()->SetText(_pObj->GetAttrib()->name);
			GameMainOptUI::GetInstance()->GetChatDlg()->GetEdit()->GainFocus();
			GameMainOptUI::GetInstance()->GetChatDlg()->GetEdit()->SetVisible(true);
		}

		//	quick add teammate
		if(bAltState &&
			AfxGetHge()->Input_KeyUp(HGEK_A))
		{
			PkgPlayerSpeOperateReq req;
			req.uUserId = GETSELFPLAYERHANDLERID;
			req.dwOp = CMD_OP_ADDTEAMMATE;
			req.dwParam = _pObj->GetHandlerID();
			g_xBuffer.Reset();
			g_xBuffer << req;
			SendBufferToGS(&g_xBuffer);
		}

		//	quick kick teammate
		else if(bAltState &&
			AfxGetHge()->Input_KeyUp(HGEK_K))
		{
			PkgPlayerSpeOperateReq req;
			req.uUserId = GETSELFPLAYERHANDLERID;
			req.dwOp = CMD_OP_KICKTEAMMATE;
			req.dwParam = _pObj->GetHandlerID();
			g_xBuffer.Reset();
			g_xBuffer << req;
			SendBufferToGS(&g_xBuffer);
		}

		//	View player's equipments
		else if(bAltState &&
			AfxGetHge()->Input_KeyUp(HGEK_V))
		{
			PkgPlayerSpeOperateReq req;
			req.uUserId = GETSELFPLAYERHANDLERID;
			req.dwOp = CMD_OP_VIEWEQUIP;
			req.dwParam = _pObj->GetHandlerID();
			g_xBuffer.Reset();
			g_xBuffer << req;
			SendBufferToGS(&g_xBuffer);
		}

		if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON) &&
			!bCtrlState)
		{
			GameMainOptUI::GetInstance()->GetRoleInfoDlg()->ShowWithPlayer(_pObj);
		}
	}

	return true;
}

bool GamePlayer::HandleNoneSpellMagic(const POINT& _mp)
{
	PkgUserActionReq req;
	const MagicDetail* pDetail = NULL;

	for(int i = HGEK_F1; i <= HGEK_F9; ++i)
	{
		int nMappedKey = pTheGame->GetAssistPaneWnd()->CheckMappedKey(i);
		int nCheckedKey = i;
		pDetail = NULL;

		if(0 != nMappedKey)
		{
			nCheckedKey = nMappedKey;
		}

		if(i == HGEK_F9)
		{
			nCheckedKey = HGEK_MBUTTON;
		}

		if(AfxGetHge()->Input_KeyUp(nCheckedKey))
		{
			if(i == HGEK_F9)
			{
				if(m_nAccMagicID != 0)
				{
					pDetail = GetMagicByID(m_nAccMagicID);
				}
			}
			else
			{
				pDetail = GetMagicByKey(i);
			}
			
			if(pDetail == NULL)
			{
				continue;
			}

			if(pDetail->wID == MEFF_CISHA ||
				pDetail->wID == MEFF_GONGSHA ||
				pDetail->wID == MEFF_BANYUE ||
				pDetail->wID == MEFF_LIEHUO ||
				pDetail->wID == MEFF_SLIEHUO)
			{
				PLAYER_DIRECTION pd = GetDirection();

				req.uAction = ACTION_SPELL;
				req.uUserId = GetHandlerID();
				req.uParam0 = MAKELONG(GetCoordX(), GetCoordY());
				req.uParam1 = MAKELONG(pDetail->wID, pd);

				g_xBuffer.Reset();
				g_xBuffer << req;
				SendBufferToGS(&g_xBuffer);

				return true;
			}
		}
	}
	return false;
}

bool GamePlayer::HandleSpellMagic(const POINT& _mp)
{
	GameObject* pSelectObj = GameScene::sThis->GetTarget();
	const MagicDetail* pDetail = NULL;
	PkgUserActionReq req;

	bool bCanProcess = false;
	if(GetStatus() == PST_STAND)
	{
		bCanProcess = true;
	}
	if(GetStatus() == PST_ATTACKSTOP/* &&
		CanAttack()*/)
	{
		bCanProcess = true;
	}
	if(!CanAttack())
	{
		bCanProcess = false;
	}
	if(!bCanProcess)
	{
		return false;
	}

	if(GetTickCount() - m_dwLastUseMagicTime < 2000)
	{
		return false;
	}
	/*else
	{
		m_dwLastUseMagicTime = GetTickCount();
	}*/

	for(int i = HGEK_F1; i <= HGEK_F9; ++i)
	{
		int nMappedKey = pTheGame->GetAssistPaneWnd()->CheckMappedKey(i);
		int nCheckedKey = i;
		pDetail = NULL;

		if(0 != nMappedKey)
		{
			nCheckedKey = nMappedKey;
		}

		if(i == HGEK_F9)
		{
			nCheckedKey = HGEK_GRAVE;
		}

		if(AfxGetHge()->Input_GetKeyState(nCheckedKey))
		{
			if(i == HGEK_F9)
			{
				if(0 != m_nAccMagicID)
				{
					pDetail = GetMagicByID(m_nAccMagicID);
				}
			}
			else
			{
				pDetail = GetMagicByKey(i);
			}
			
			if(pDetail == NULL)
			{
				continue;
			}
			//if(GetStatus() != PST_STAND)
			{
				if(pDetail->wID >= MEFF_CISHA &&
					pDetail->wID <= MEFF_LIEHUO)
				{
					continue;
				}
				if(pDetail->wID == MEFF_SLIEHUO)
				{
					continue;
				}
			}

			int nCost = GetMagicCost(pDetail);
#ifdef _DEBUG
			//nCost = 0;
#endif
			if(m_attrib.MP < nCost)
			{
				//GameInfoBoardDlg::GetInstance()->InsertBoardMsg(ARGB_RED, "您的MP不足");
				return true;
			}

			MGCHISTORYMAP::iterator fnditer = m_xMgcHistory.find(pDetail->wID);
			if(fnditer == m_xMgcHistory.end())
			{
				m_xMgcHistory.insert(std::make_pair(pDetail->wID, GetTickCount()));
			}
			else
			{
				DWORD dwLastUseTime = fnditer->second;
				DWORD dwDelayTime = GetMagicDelay(pDetail);
				if(GetTickCount() < dwLastUseTime + 2000 + dwDelayTime)
				{
					return true;
				}
				else
				{
					fnditer->second = GetTickCount();
				}
			}
	
			POINT pt1 = {VIEW_WIDTH / 2, VIEW_HEIGHT / 2};
			PLAYER_DIRECTION pd = GetAngle(pt1, _mp);
			PLAYER_DIRECTION pdpre = GetDirection();
			//SetDirection(pd);
			if(pd != pdpre)
			{
				/*
				g_xBuffer.Reset();
								req.uAction = ACTION_TURN;
								req.uUserId = GetHandlerID();
								req.uParam0 = MAKELONG(GetCoordX(), GetCoordY());
								req.uParam1 = (unsigned int)GetDirection();
								g_xBuffer << req;
								SendBuffer(&g_xBuffer);*/
				
			}
			POINT ptDest;
			GameScene::sThis->MousePtToScreenPt(_mp, ptDest);
			//PlayMagic(pDetail->wID, pSelectObj, ptDest.x, ptDest.y, 7500);

			req.uAction = ACTION_SPELL;
			req.uUserId = GetHandlerID();
			req.uParam0 = MAKELONG(GetCoordX(), GetCoordY());
			req.uParam1 = MAKELONG(pDetail->wID, pd);

			if(pSelectObj != NULL)
			{
				//	选中了别的目标了
				if(pDetail->wID != MEFF_HEAL &&
					pDetail->wID != MEFF_CHARMAC &&
					pDetail->wID != MEFF_HEAL &&
					pDetail->wID != MEFF_SUMMON &&
					pDetail->wID != MEFF_FIREWALL &&
					pDetail->wID != MEFF_ICEROAR &&
					pDetail->wID != MEFF_LIONROAR &&
					pDetail->wID != MEFF_FIRESHOWER &&
					pDetail->wID != MEFF_SUPERHEAL /*&&
					pDetail->wID != MEFF_BIGPOISON*/)
				{
					SetMgcTarget(pSelectObj);
				}
				req.uTargetId = pSelectObj->GetHandlerID();
				req.uParam2 = MAKELONG(ptDest.x, ptDest.y);
			}
			else
			{
				//	没有目标 先检测是否有锁定目标
				if(pDetail->wID == MEFF_CHARMAC ||
					pDetail->wID == MEFF_FIREWALL ||
					pDetail->wID == MEFF_ICEROAR ||
					pDetail->wID == MEFF_LIONROAR ||
					pDetail->wID == MEFF_FIRESHOWER ||
					pDetail->wID == MEFF_SUPERHEAL/*||
					pDetail->wID == MEFF_BIGPOISON*/)
				{
					//	战甲术 不需要target
					req.uParam2 = MAKELONG(ptDest.x, ptDest.y);
				}
				else if(pDetail->wID == MEFF_HEAL)
				{
					//	治愈术不需要target
					if(pSelectObj)
					{
						req.uTargetId = pSelectObj->GetHandlerID();
					}
					else
					{

					}
				}
				else
				{
					if(GetMgcTarget() != NULL)
					{
						//	检测 是否超出范围
						int nOutBoundWidth = (VIEW_WIDTH / UNIT_WIDTH - 1) / 2;
						int nOutBoundHeight = (VIEW_HEIGHT / UNIT_HEIGHT - 1) / 2;
						if(abs(GetCoordX() - GetMgcTarget()->GetCoordX()) > nOutBoundWidth ||
							abs(GetCoordY() - GetMgcTarget()->GetCoordY()) > nOutBoundHeight)
						{
							//	超出范围 无效
							req.uParam2 = MAKELONG(ptDest.x, ptDest.y);
						}
						else
						{
							//	没有超出范围
							req.uTargetId = GetMgcTarget()->GetHandlerID();
							req.uParam2 = MAKELONG(ptDest.x, ptDest.y);
						}
					}
					else
					{
						//	没有目标
						req.uParam2 = MAKELONG(ptDest.x, ptDest.y);
					}
				}
				//req.uParam2 = MAKELONG(ptDest.x, ptDest.y);
			}
			
			g_xBuffer.Reset();
			g_xBuffer << req;
			SendBufferToGS(&g_xBuffer);
			//	锁定
			Lock(PST_SK_LEIDIANSHU);
			m_dwLastUseMagicTime = GetTickCount();

			return true;
		}
	}
	return false;
}

bool GamePlayer::HandleMagicInput(const POINT& _mp)
{
	GameObject* pSelectObj = GameScene::sThis->GetTarget();
	const MagicDetail* pDetail = NULL;
	PkgUserActionReq req;

	for(int i = HGEK_F1; i <= HGEK_F8; ++i)
	{
		int nMappedKey = pTheGame->GetAssistPaneWnd()->CheckMappedKey(i);
		int nCheckedKey = i;

		if(0 != nMappedKey)
		{
			nCheckedKey = nMappedKey;
		}

		if(AfxGetHge()->Input_KeyUp(nCheckedKey))
		{
			pDetail = GetMagicByKey(i);
			if(pDetail == NULL)
			{
				continue;
			}
			if(GetStatus() != PST_STAND)
			{
				if(pDetail->wID < MEFF_CISHA ||
					pDetail->wID > MEFF_LIEHUO)
				{
					continue;
				}
			}
			POINT pt1 = {VIEW_WIDTH / 2, VIEW_HEIGHT / 2};
			PLAYER_DIRECTION pd = GetAngle(pt1, _mp);
			PLAYER_DIRECTION pdpre = GetDirection();
			//SetDirection(pd);
			if(pd != pdpre)
			{
				/*
				g_xBuffer.Reset();
								req.uAction = ACTION_TURN;
								req.uUserId = GetHandlerID();
								req.uParam0 = MAKELONG(GetCoordX(), GetCoordY());
								req.uParam1 = (unsigned int)GetDirection();
								g_xBuffer << req;
								SendBuffer(&g_xBuffer);*/
				
			}
			POINT ptDest;
			GameScene::sThis->MousePtToScreenPt(_mp, ptDest);
			//PlayMagic(pDetail->wID, pSelectObj, ptDest.x, ptDest.y, 7500);

			req.uAction = ACTION_SPELL;
			req.uUserId = GetHandlerID();
			req.uParam0 = MAKELONG(GetCoordX(), GetCoordY());
			req.uParam1 = MAKELONG(pDetail->wID, pd);
			if(pSelectObj != NULL)
			{
				req.uTargetId = pSelectObj->GetHandlerID();
			}
			else
			{
				req.uParam2 = MAKELONG(ptDest.x, ptDest.y);
			}
			
			g_xBuffer.Reset();
			g_xBuffer << req;
			SendBufferToGS(&g_xBuffer);
			

			return true;
		}
	}
	return false;
}

bool GamePlayer::ProcUserCmd(const POINT& _mp)
{
	//	处理用户输入
	//GameObject* pSelectObj = GameScene::sThis->FindGameMonsterByArea(_mp.x, _mp.y);
	GameObject* pSelectObj = GameScene::sThis->GetTarget();

	if(TEST_FLAG_BOOL(m_dwHumEffectFlag, MMASK_STONE))
	{
		return true;
	}

	if(m_bIsWalkLocked)
	{
		if(GetTickCount() - m_dwLastLockedWalkTime > 1000)
		{
			m_bIsWalkLocked = false;
			m_dwLastLockedWalkTime = GetTickCount();
		}
		else
		{
			return true;
		}
	}

	if(!m_bUsingPreLock)
	{
		if(IsWaitServerResponse())
		{
			return true;
		}
	}

	/*static int s_nWingIndex = 0;
	static int s_nWingStyle[] = {0,1,2,3,4,5,6,7,8,9,10,13,14,15,17};

	if(AfxGetHge()->Input_KeyUp(HGEK_ADD))
	{
		++s_nWingIndex;
		if(s_nWingIndex >= sizeof(s_nWingStyle) / sizeof(s_nWingStyle[0]))
		{
			s_nWingIndex = 0;
		}
		m_stExtAttrib.uWing = s_nWingStyle[s_nWingIndex];
	}*/

	if(HandleSpellMagic(_mp))
	{
		return true;
	}
	/*if(m_bIsLocked)
	{
		if(GetTickCount() - m_dwLastLockedTime > 1000)
		{
			m_bIsLocked = false;
			m_dwLastLockedTime = GetTickCount();
		}
		else
		{
			return true;
		}
	}*/

	/*
	if(GetStatus() == PST_STAND)
		{
			if(HandleMagicInput(_mp))
			{
				return true;
			}
		}*/
	
	
	if(!IsMoving() &&
		CanAttack())
	{
		//	不在移动状态下才能处理输入
		POINT pt1 = {VIEW_WIDTH / 2, VIEW_HEIGHT / 2};
		PLAYER_DIRECTION pd = GetAngle(pt1, _mp);

		//	NPC 点击处理
		NPCClick(pSelectObj);
		//	其它玩家操作
		OtherPlayerClick(pSelectObj);

		if(GameScene::sThis->GetMouseCoordX() == GetCoordX() &&
			GameScene::sThis->GetMouseCoordY() == GetCoordY())
		{
			if(AfxGetHge()->Input_GetKeyState(HGEK_RBUTTON))
			{
				if(GetDirection() != pd)
				{
					SetDirection(pd);
					PkgUserActionReq req;
					g_xBuffer.Reset();
					req.uAction = ACTION_TURN;
					req.uUserId = GetHandlerID();
					req.uParam0 = MAKELONG(GetCoordX(), GetCoordY());
					req.uParam1 = (unsigned int)GetDirection();
					g_xBuffer << req;
					SendBufferToGS(&g_xBuffer);
				}
			}
			else if(AfxGetHge()->Input_GetKeyState(HGEK_LBUTTON))
			{
				if(pSelectObj)
				{
					AttackMonster(pSelectObj);
				}
				//	pick up item
				if(pTheGame->Config_MousePickUp() &&
					!IsMoving())
				{
					WORD wPosX = GetCoordX();
					WORD wPosY = GetCoordY();
					GROUNDITEMS& refGroundItems = GameScene::sThis->GetGroundItems();
					for(std::list<GroundItem*>::const_iterator iter = refGroundItems.begin();
						iter != refGroundItems.end();
						++iter)
					{
						if(wPosX == (*iter)->wPosX &&
							wPosY == (*iter)->wPosY)
						{
							PkgPlayerPickUpItemReq req;
							req.dwTag = (*iter)->stAttrib.tag;
							req.uUserId = GetHandlerID();
							g_xBuffer.Reset();
							g_xBuffer << req;
							SendBufferToGS(&g_xBuffer);
							break;
						}
					}
				}
			}
		}
		else
		{
			if(AfxGetHge()->Input_GetKeyState(HGEK_RBUTTON))
			{
				SetAtkMonster(NULL);

				PLAYER_DIRECTION pdpre = GetDirection();
				bool bWalked = Run(pd);

				if(!bWalked &&
					pdpre != GetDirection())
				{
#ifdef _NET_GAME_
					PkgUserActionReq req;
					g_xBuffer.Reset();
					req.uAction = ACTION_TURN;
					req.uUserId = GetHandlerID();
					req.uParam0 = MAKELONG(GetCoordX(), GetCoordY());
					req.uParam1 = (unsigned int)GetDirection();
					g_xBuffer << req;
					SendBufferToGS(&g_xBuffer);
#endif
				}
				if(bWalked)
				{
					//GameScene::sThis->GetMainOpt()->GetShopDlg()->SetVisible(false);
					//GameScene::sThis->GetMainOpt()->GetStoreDlg()->SetVisible(false);
					//GameScene::sThis->GetMainOpt()->GetShopDlg2()->CloseDialog();
					//GameScene::sThis->GetMainOpt()->GetStoreDlg2()->CloseDialog();
					//GameScene::sThis->GetMainOpt()->GetIdentifyDlg()->CloseDialog();
					//m_fLastUpdateAttackStop = 0.0f;
					GameMainOptUI::GetInstance()->CloseAllDialog();
				}
			}
			else if(AfxGetHge()->Input_GetKeyState(HGEK_LBUTTON))
			{
				bool bShiftDown = AfxGetHge()->Input_GetKeyState(HGEK_SHIFT);

				if(bShiftDown)
				{
					if((GetStatus() == PST_ATTACKSTOP || GetStatus() == PST_STAND) &&
						!m_bIsLocked &&
						CanAttack())
					{
						if(m_bUsingPreLock)
						{
							SetDirection((PLAYER_DIRECTION)pd);
							PkgUserActionReq req;
							req.uAction = ACTION_ATTACK;
							req.uUserId = GetHandlerID();
							req.uParam0 = MAKELONG(GetCoordX(), GetCoordY());
							req.uParam1 = pd;
							g_xBuffer.Reset();
							g_xBuffer << req;
							SendBufferToGS(&g_xBuffer);
							Lock(PST_ATTACKWEAPON);
						}
						else
						{
							SetDirection((PLAYER_DIRECTION)pd);
							PreNormalAttackTarget(NULL);
						}
					}
				}
				else
				{
					if(pSelectObj)
					{
						//	选中了NPC
						if(!AttackMonster(pSelectObj))
						{
							//	点了之后没有处理 就能走动
							PLAYER_DIRECTION pdpre = GetDirection();
							bool bWalked = Walk(pd);

							if(!bWalked &&
								pdpre != GetDirection())
							{
#ifdef _NET_GAME_
								PkgUserActionReq req;
								g_xBuffer.Reset();
								req.uAction = ACTION_TURN;
								req.uUserId = GetHandlerID();
								req.uParam0 = MAKELONG(GetCoordX(), GetCoordY());
								req.uParam1 = (unsigned int)GetDirection();
								g_xBuffer << req;
								SendBufferToGS(&g_xBuffer);
#endif
							}
						}
					}
					else
					{
						//	点了之后没有处理 就能走动
						PLAYER_DIRECTION pdpre = GetDirection();
						bool bWalked = Walk(pd);

						if(!bWalked &&
							pdpre != GetDirection())
						{
#ifdef _NET_GAME_
							PkgUserActionReq req;
							g_xBuffer.Reset();
							req.uAction = ACTION_TURN;
							req.uUserId = GetHandlerID();
							req.uParam0 = MAKELONG(GetCoordX(), GetCoordY());
							req.uParam1 = (unsigned int)GetDirection();
							g_xBuffer << req;
							SendBufferToGS(&g_xBuffer);
#endif
						}

						if(bWalked)
						{
							//GameScene::sThis->GetMainOpt()->GetShopDlg()->SetVisible(false);
							//GameScene::sThis->GetMainOpt()->GetStoreDlg()->SetVisible(false);
							/*if(GameScene::sThis->GetMainOpt()->GetShopDlg2()->IsVisible())
							{
								GameScene::sThis->GetMainOpt()->GetShopDlg2()->CloseDialog();
							}
							if(GameScene::sThis->GetMainOpt()->GetStoreDlg2()->IsVisible())
							{
								GameScene::sThis->GetMainOpt()->GetStoreDlg2()->CloseDialog();
							}
							GameScene::sThis->GetMainOpt()->GetIdentifyDlg()->CloseDialog();*/
							GameMainOptUI::GetInstance()->CloseAllDialog();
							
							//m_fLastUpdateAttackStop = 0.0f;
						}
					}
				}
			}
		}
	}
	return true;
}

bool GamePlayer::Run(PLAYER_DIRECTION _pd)
{
	bool bWalked = false;
	int nOftX = 0;
	int nOftY = 0;
	//if(AfxGetHge()->Input_GetKeyState(HGEK_RBUTTON))
	{
		//	右键 跑步
		switch (_pd)
		{
		case PDT_UP:
			{
				if(GameMapManager::GetInstance()->CanThrough(GetCoordX(), GetCoordY() - 1))
				{
					//	能走一个 就走
					//SetOffsetCoordY(-1);
					nOftY = -1;
					SetStatus(PST_WALK);
					bWalked = true;
					//	能走的情况下 再判断下一格
					//	注意GetCoord的时候因为上面已经-1了 所以只要偏移一位
					//if(GameMapManager::GetInstance()->CanThrough(GetCoordX(), GetCoordY() - 1))
					if(GameMapManager::GetInstance()->CanThrough(GetCoordX(), GetCoordY() - 2))
					{
						//	假如前一格还能走 再加一 变为跑状态
						//SetOffsetCoordY(-1);
						nOftY = -2;
						SetStatus(PST_RUN);
					}
				}
				SetDirection(PDT_UP);
				//m_pPlayer->SetPosyOffset(- dt * m_pPlayer->GetSpeed() * 0.7);
			}break;
		case PDT_DOWN:
			{
				if(GameMapManager::GetInstance()->CanThrough(GetCoordX(), GetCoordY() + 1))
				{
					//SetOffsetCoordY(1);
					nOftY = 1;
					SetStatus(PST_WALK);
					bWalked = true;
					//if(GameMapManager::GetInstance()->CanThrough(GetCoordX(), GetCoordY() + 1))
					if(GameMapManager::GetInstance()->CanThrough(GetCoordX(), GetCoordY() + 2))
					{
						//SetOffsetCoordY(1);
						nOftY = 2;
						SetStatus(PST_RUN);
					}
				}
				SetDirection(PDT_DOWN);

				//m_pPlayer->SetPosyOffset(dt * m_pPlayer->GetSpeed() * 0.7);
			}break;
		case PDT_LEFT:
			{
				//	先判断阻挡
				if(GameMapManager::GetInstance()->CanThrough(GetCoordX() - 1, GetCoordY()))
				{
					//SetOffsetCoordX(-1);
					nOftX = -1;
					SetStatus(PST_WALK);
					bWalked = true;
					//if(GameMapManager::GetInstance()->CanThrough(GetCoordX() - 1, GetCoordY()))
					if(GameMapManager::GetInstance()->CanThrough(GetCoordX() - 2, GetCoordY()))
					{
						//SetOffsetCoordX(-1);
						nOftX = -2;
						SetStatus(PST_RUN);
					}
				}
				SetDirection(PDT_LEFT);

				//m_pPlayer->SetPosxOffset(- dt * m_pPlayer->GetSpeed());
			}break;
		case PDT_RIGHT:
			{
				if(GameMapManager::GetInstance()->CanThrough(GetCoordX() + 1, GetCoordY()))
				{
					//SetOffsetCoordX(1);
					nOftX = 1;
					SetStatus(PST_WALK);
					bWalked = true;
					//if(GameMapManager::GetInstance()->CanThrough(GetCoordX() + 1, GetCoordY()))
					if(GameMapManager::GetInstance()->CanThrough(GetCoordX() + 2, GetCoordY()))
					{
						//SetOffsetCoordX(1);
						nOftX = 2;
						SetStatus(PST_RUN);
					}
				}
				SetDirection(PDT_RIGHT);

				//m_pPlayer->SetPosxOffset(dt * m_pPlayer->GetSpeed());
			}break;
		case PDT_RIGHTUP:
			{
				if(GameMapManager::GetInstance()->CanThrough(GetCoordX() + 1, GetCoordY() - 1)/*
				 &&
									(GameMapManager::GetInstance()->CanThrough(GetCoordX(), GetCoordY() - 1) ||
									GameMapManager::GetInstance()->CanThrough(GetCoordX() + 1, GetCoordY()))*/
				)
				{
					//	斜向 先判断斜向走一格 再两格
					//SetOffsetCoordX(1);
					//SetOffsetCoordY(-1);
					nOftX = 1;
					nOftY = -1;
					SetStatus(PST_WALK);
					bWalked = true;
					// 					if(GameMapManager::GetInstance()->CanThrough(GetCoordX() + 1, GetCoordY() - 1) &&
					// 						(GameMapManager::GetInstance()->CanThrough(GetCoordX(), GetCoordY() - 1) ||
					// 						GameMapManager::GetInstance()->CanThrough(GetCoordX() + 1, GetCoordY())))
					if(GameMapManager::GetInstance()->CanThrough(GetCoordX() + 2, GetCoordY() - 2)/*
						 &&
												(GameMapManager::GetInstance()->CanThrough(GetCoordX(), GetCoordY() - 2) ||
												GameMapManager::GetInstance()->CanThrough(GetCoordX() + 2, GetCoordY()))*/
						)
					{
						//SetOffsetCoordX(1);
						//SetOffsetCoordY(-1);
						nOftX = 2;
						nOftY = -2;
						SetStatus(PST_RUN);
					}
				}
				SetDirection(PDT_RIGHTUP);
			}break;
		case PDT_RIGHTDOWN:
			{
				if(GameMapManager::GetInstance()->CanThrough(GetCoordX() + 1, GetCoordY() + 1)/*
					 &&
										(GameMapManager::GetInstance()->CanThrough(GetCoordX() + 1, GetCoordY()) ||
										GameMapManager::GetInstance()->CanThrough(GetCoordX(), GetCoordY() + 1))*/
					)

				{
					//SetOffsetCoordX(1);
					//SetOffsetCoordY(1);
					nOftX = 1;
					nOftY = 1;
					SetStatus(PST_WALK);
					bWalked = true;
					// 					if(GameMapManager::GetInstance()->CanThrough(GetCoordX() + 1, GetCoordY() + 1) &&
					// 						(GameMapManager::GetInstance()->CanThrough(GetCoordX() + 1, GetCoordY()) ||
					// 						GameMapManager::GetInstance()->CanThrough(GetCoordX(), GetCoordY() + 1)))
					if(GameMapManager::GetInstance()->CanThrough(GetCoordX() + 2, GetCoordY() + 2)/*
						 &&
												(GameMapManager::GetInstance()->CanThrough(GetCoordX() + 2, GetCoordY()) ||
												GameMapManager::GetInstance()->CanThrough(GetCoordX(), GetCoordY() + 2))*/
						)
					{
						//SetOffsetCoordX(1);
						//SetOffsetCoordY(1);
						nOftX = 2;
						nOftY = 2;
						SetStatus(PST_RUN);
					}
				}
				SetDirection(PDT_RIGHTDOWN);
			}break;
		case PDT_LEFTUP:
			{
				if(GameMapManager::GetInstance()->CanThrough(GetCoordX() -1, GetCoordY() - 1)/*
					 &&
										(GameMapManager::GetInstance()->CanThrough(GetCoordX() - 1, GetCoordY()) ||
										GameMapManager::GetInstance()->CanThrough(GetCoordX(), GetCoordY() - 1))*/
					)
				{
					//SetOffsetCoordX(-1);
					//SetOffsetCoordY(-1);
					nOftX = -1;
					nOftY = -1;
					SetStatus(PST_WALK);
					bWalked = true;
					// 					if(GameMapManager::GetInstance()->CanThrough(GetCoordX() -1, GetCoordY() - 1) &&
					// 						(GameMapManager::GetInstance()->CanThrough(GetCoordX() - 1, GetCoordY()) ||
					// 						GameMapManager::GetInstance()->CanThrough(GetCoordX(), GetCoordY() - 1)))
					if(GameMapManager::GetInstance()->CanThrough(GetCoordX() -2, GetCoordY() - 2)/* &&
						(GameMapManager::GetInstance()->CanThrough(GetCoordX() - 2, GetCoordY()) ||
						GameMapManager::GetInstance()->CanThrough(GetCoordX(), GetCoordY() - 2))*/)
					{
						//SetOffsetCoordX(-1);
						//SetOffsetCoordY(-1);
						nOftX = -2;
						nOftY = -2;
						SetStatus(PST_RUN);
					}		
				}
				SetDirection(PDT_LEFTUP);
			}break;
		case PDT_LEFTDOWN:
			{
				if(GameMapManager::GetInstance()->CanThrough(GetCoordX() - 1, GetCoordY() + 1)/*
					 &&
										(GameMapManager::GetInstance()->CanThrough(GetCoordX() - 1, GetCoordY()) ||
										GameMapManager::GetInstance()->CanThrough(GetCoordX(), GetCoordY() + 1))*/
					)
				{
					//SetOffsetCoordX(-1);
					//SetOffsetCoordY(1);
					nOftX = -1;
					nOftY = 1;
					SetStatus(PST_WALK);
					bWalked = true;
					// 					if(GameMapManager::GetInstance()->CanThrough(GetCoordX() - 1, GetCoordY() + 1) &&
					// 						(GameMapManager::GetInstance()->CanThrough(GetCoordX() - 1, GetCoordY()) ||
					// 						GameMapManager::GetInstance()->CanThrough(GetCoordX(), GetCoordY() + 1)))
					if(GameMapManager::GetInstance()->CanThrough(GetCoordX() - 2, GetCoordY() + 2)/*
						 &&
												(GameMapManager::GetInstance()->CanThrough(GetCoordX() - 2, GetCoordY()) ||
												GameMapManager::GetInstance()->CanThrough(GetCoordX(), GetCoordY() + 2))*/
						)
					{
						//SetOffsetCoordX(-1);
						//SetOffsetCoordY(1);
						nOftX = -2;
						nOftY = 2;
						SetStatus(PST_RUN);
					}
				}
				SetDirection(PDT_LEFTDOWN);
			}break;
		}
	}

	if(bWalked)
	{
		GameMainOptUI::GetInstance()->GetActiveDlg()->SetVisible(false);
		POINT ptPre = m_ptPos;
		SetOffsetCoordX(nOftX);
		SetOffsetCoordY(nOftY);

#ifdef _NET_GAME_
		PkgUserActionReq req;
		if(abs(nOftX) == 2 ||
			abs(nOftY) == 2)
		{
			req.uAction = ACTION_RUN;
		}
		else
		{
			req.uAction = ACTION_WALK;
		}
		req.uParam0 = MAKELONG(ptPre.x, ptPre.y);
		req.uParam1 = MAKELONG(m_ptPos.x, m_ptPos.y);
		req.uUserId = GetHandlerID();
		g_xBuffer.Reset();
		g_xBuffer << req;
		m_dwLastMoveTime = GetTickCount();
		if(0 == SendBufferToGS(&g_xBuffer))
		{
			//
		}
#endif
	}
	return bWalked;
}

bool GamePlayer::Walk(PLAYER_DIRECTION _pd)
{
	bool bWalked = false;
	int nOftX = 0;
	int nOftY = 0;

	{
		//	可以走动的话
		SetAtkMonster(NULL);

		switch (_pd)
		{
		case PDT_UP:
			{
				if(GameMapManager::GetInstance()->CanThrough(GetCoordX(), GetCoordY() - 1))
				{
					//SetOffsetCoordY(-1);
					nOftY = -1;
					SetStatus(PST_WALK);
					bWalked = true;
				}
				SetDirection(PDT_UP);
			}break;
		case PDT_DOWN:
			{
				if(GameMapManager::GetInstance()->CanThrough(GetCoordX(), GetCoordY() + 1))
				{
					//SetOffsetCoordY(1);
					nOftY = 1;
					SetStatus(PST_WALK);
					bWalked = true;
				}
				SetDirection(PDT_DOWN);
			}break;
		case PDT_LEFT:
			{
				//	判断阻挡
				if(GameMapManager::GetInstance()->CanThrough(GetCoordX() - 1, GetCoordY()))
				{
					SetStatus(PST_WALK);
					//SetOffsetCoordX(-1);
					nOftX = -1;
					bWalked = true;
				}
				SetDirection(PDT_LEFT);
			}break;
		case PDT_RIGHT:
			{
				//	阻挡
				if(GameMapManager::GetInstance()->CanThrough(GetCoordX() + 1, GetCoordY()))
				{
					//SetOffsetCoordX(1);
					nOftX = 1;
					SetStatus(PST_WALK);
					bWalked = true;
				}
				SetDirection(PDT_RIGHT);
			}break;
			/*
			*	斜着走 假若前进方向两边阻挡 则判断为不能走动
			*/
		case PDT_RIGHTUP:
			{
				if(GameMapManager::GetInstance()->CanThrough(GetCoordX() + 1, GetCoordY() - 1)/*
					 &&
										(GameMapManager::GetInstance()->CanThrough(GetCoordX(), GetCoordY() - 1) ||
										GameMapManager::GetInstance()->CanThrough(GetCoordX() + 1, GetCoordY()))*/
					)
				{
					//SetOffsetCoordX(1);
					//SetOffsetCoordY(-1);
					nOftX = 1;
					nOftY = -1;
					SetStatus(PST_WALK);
					bWalked = true;
				}
				SetDirection(PDT_RIGHTUP);
			}break;
		case PDT_RIGHTDOWN:
			{
				if(GameMapManager::GetInstance()->CanThrough(GetCoordX() + 1, GetCoordY() + 1)/*
					 &&
										(GameMapManager::GetInstance()->CanThrough(GetCoordX() + 1, GetCoordY()) ||
										GameMapManager::GetInstance()->CanThrough(GetCoordX(), GetCoordY() + 1))*/
					)
				{
					//SetOffsetCoordX(1);
					//SetOffsetCoordY(1);
					nOftX = 1;
					nOftY = 1;
					SetStatus(PST_WALK);
					bWalked = true;
				}
				SetDirection(PDT_RIGHTDOWN);
			}break;
		case PDT_LEFTUP:
			{
				if(GameMapManager::GetInstance()->CanThrough(GetCoordX() -1, GetCoordY() - 1)/*
					 &&
										(GameMapManager::GetInstance()->CanThrough(GetCoordX() - 1, GetCoordY()) ||
										GameMapManager::GetInstance()->CanThrough(GetCoordX(), GetCoordY() - 1))*/
					)
				{
					//SetOffsetCoordX(-1);
					//SetOffsetCoordY(-1);
					nOftX = -1;
					nOftY = -1;
					SetStatus(PST_WALK);
					bWalked = true;
				}
				SetDirection(PDT_LEFTUP);
			}break;
		case PDT_LEFTDOWN:
			{
				if(GameMapManager::GetInstance()->CanThrough(GetCoordX() - 1, GetCoordY() + 1)/*
					 &&
										(GameMapManager::GetInstance()->CanThrough(GetCoordX() - 1, GetCoordY()) ||
										GameMapManager::GetInstance()->CanThrough(GetCoordX(), GetCoordY() + 1))*/
					)
				{
					SetStatus(PST_WALK);
					//SetOffsetCoordX(-1);
					//SetOffsetCoordY(1);
					nOftX = -1;
					nOftY = 1;
					bWalked = true;
				}
				SetDirection(PDT_LEFTDOWN);
			}break;
		}
	}

	if(bWalked)
	{
		GameMainOptUI::GetInstance()->GetActiveDlg()->SetVisible(false);
		POINT ptPre = m_ptPos;
		SetOffsetCoordX(nOftX);
		SetOffsetCoordY(nOftY);

#ifdef _NET_GAME_
		PkgUserActionReq req;
		req.uAction = ACTION_WALK;
		req.uUserId = GetHandlerID();
		req.uParam0 = MAKELONG(ptPre.x, ptPre.y);
		req.uParam1 = MAKELONG(m_ptPos.x, m_ptPos.y);
		g_xBuffer.Reset();
		g_xBuffer << req;

		m_dwLastMoveTime = GetTickCount();
		if(0 == SendBufferToGS(&g_xBuffer))
		{
			//
		}
#endif
	}
	return bWalked;
}

bool GamePlayer::AttackMonster(GameObject* _mons)
{
	if(_mons->GetType() == OBJ_NPC)
	{
		return true;
	}
	else if(_mons->GetType() == OBJ_MONS)
	{
		//	点击怪物
		GameMonster* pMons = (GameMonster*)_mons;

		if(_mons->GetStatus() != PST_DEAD)
		{
			SetAtkMonster(_mons);
			return true;
		}
	}
	else if(_mons->GetType() == OBJ_OTHERPLAYER)
	{
		//	不动
		GameOtherPlayer* pPlayer = (GameOtherPlayer*)_mons;

		//	shift情况下才会跟踪攻击
		if(_mons->GetStatus() != PST_DEAD &&
			AfxGetHge()->Input_GetKeyState(HGEK_CTRL))
		{
			SetAtkMonster(_mons);
		}
		return true;
	}

	return false;
}
//////////////////////////////////////////////////////////////////////////
int GamePlayer::GetMagicCost(const MagicDetail* _pDetail)
{
	if(_pDetail->wID >= MEFF_USERTOTAL)
	{
		return 0;
	}
	int nBaseCost = GameScene::sThis->m_dwMgcCostTable[_pDetail->wID];
	int nHalfCost = nBaseCost / 2;

	nBaseCost += (nHalfCost) * (_pDetail->cLevel - 1);
	return nBaseCost;
}
//////////////////////////////////////////////////////////////////////////
int GamePlayer::GetMagicDelay(const MagicDetail* _pDetail)
{
	if(_pDetail->wID >= MEFF_USERTOTAL)
	{
		return 0;
	}
	int nBaseCost = GameScene::sThis->m_dwMgcDelayTable[_pDetail->wID];
	return nBaseCost;
}
//////////////////////////////////////////////////////////////////////////
void GamePlayer::AutoAttackTarget()
{
	if(GetStatus() == PST_DEAD)
	{
		SetAtkMonster(NULL);
		SetMgcTarget(NULL);
		return;
	}
	if(TEST_FLAG_BOOL(m_dwHumEffectFlag, MMASK_STONE))
	{
		return;
	}

	if(m_atkTarget)
	{
		//if(!m_bIsLocked)
		if(!IsLocked())
		{
			if(!IsMoving())
			{
				DWORD dwCurTick = GetTickCount();
				POINT p1 = {GetCoordX(), GetCoordY()};
				POINT p2 = {m_atkTarget->GetCoordX(), m_atkTarget->GetCoordY()};
				int pd = GetDirectionOf2(p1, p2);
				if(pd >= 0 &&
					pd <= 7)
				{
					//	Auto attack target
					if((GetStatus() == PST_STAND ||
						GetStatus() == PST_ATTACKSTOP) &&
						CanAttack())
					{
						if(m_bUsingPreLock)
						{
							//	预加锁模式 等待服务器回应 锁住人物动作
							SetDirection((PLAYER_DIRECTION)pd);
							PkgUserActionReq req;
							req.uAction = ACTION_ATTACK;
							req.uUserId = GetHandlerID();
							req.uTargetId = m_atkTarget->GetHandlerID();
							req.uParam0 = MAKELONG(GetCoordX(), GetCoordY());
							req.uParam1 = pd;
							g_xBuffer.Reset();
							g_xBuffer << req;
							SendBufferToGS(&g_xBuffer);
							//m_bIsLocked = true;
							//m_dwLastLockedTime = dwCurTick;

							Lock(PST_ATTACKWEAPON);
						}
						else
						{
							SetDirection((PLAYER_DIRECTION)pd);
							PreNormalAttackTarget(m_atkTarget);
						}
					}
				}
				else
				{
					//	Can't attack the target , then try to move to the destinate pos
					int nOftX = abs(GetCoordX() - m_atkTarget->GetCoordX());
					int nOftY = abs(GetCoordY() - m_atkTarget->GetCoordY());

					BYTE bWalkMode = MOVE_WALK;
					if(nOftX > 2 ||
						nOftY > 2)
					{
						bWalkMode = MOVE_RUN;
					}
					if(nOftX == 0 &&
						nOftY == 0)
					{
						return;
					}
					int nWalkDrt = GetDirection8(GetCoordX(), GetCoordY(), m_atkTarget->GetCoordX(), m_atkTarget->GetCoordY());
					if(nWalkDrt >= 0 &&
						nWalkDrt <= 7)
					{
						if(bWalkMode == MOVE_WALK)
						{
							nOftX = GetCoordX();
							nOftX += g_nMoveOftTable[nWalkDrt * 2];
							nOftY = GetCoordY();
							nOftY += g_nMoveOftTable[nWalkDrt * 2 + 1];
						}
						else
						{
							nOftX = GetCoordX();
							nOftX += g_nMoveOftTable[nWalkDrt * 2] * 2;
							nOftY = GetCoordY();
							nOftY += g_nMoveOftTable[nWalkDrt * 2 + 1] * 2;
						}
						POINT ptPre = GetPosition();
						//	Test the point
						bool bCanWalk = true;
						if(bWalkMode == MOVE_WALK)
						{
							bCanWalk = GameMapManager::GetInstance()->CanThrough(nOftX, nOftY);
						}
						else
						{
							bCanWalk = GameMapManager::GetInstance()->CanThrough(nOftX, nOftY);
							bCanWalk = (bCanWalk && GameMapManager::GetInstance()->CanThrough(nOftX - g_nMoveOftTable[nWalkDrt * 2],
								nOftY - g_nMoveOftTable[nWalkDrt * 2 + 1]));
						}
						if(!CanAttack())
						{
							bCanWalk = false;
						}
						if(bCanWalk &&
							(GetStatus() == PST_WALK || GetStatus() == PST_RUN || GetStatus() == PST_STAND || GetStatus() == PST_ATTACKSTOP))
						{
							//	Note ,this is to void player running or walking when they are attacking monster
							//	Send to server
							if(MoveToDestination(nOftX, nOftY, bWalkMode))
							{
								PkgUserActionReq req;
								if(bWalkMode == MOVE_WALK)
								{
									req.uAction = ACTION_WALK;
								}
								else
								{
									req.uAction = ACTION_RUN;
								}
								req.uParam0 = MAKELONG(ptPre.x, ptPre.y);
								req.uParam1 = MAKELONG(m_ptPos.x, m_ptPos.y);
								req.uUserId = GetHandlerID();
								g_xBuffer.Reset();
								g_xBuffer << req;
								if(0 == SendBufferToGS(&g_xBuffer))
								{
									//
								}
							}
						}
					}
				}
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////
void GamePlayer::Update(float _dt)
{
	m_xDrugCoolDown.Update();
	UpdateWaitServerResponse();

#define UPDATE_ATTACK_INTERVAL	0.08f
#define UPDATE_SKILL_INTERVAL	0.08f
	//	正在移动中 自动跑路
	GamePlayer * m_pPlayer = this;
	PLAYER_DIRECTION pd;
	float dt = _dt;
	int nTotalFrame = 0;
	int nMoveOffset = 0;
	static float fMoveOffsetXTotal = 0.0f;
	static float fMoveOffsetYTotal = 0.0f;
	float fMoveOffsetX = 0.0f;
	float fMoveOffsetY = 0.0f;

	static float fLastLockTime = 0.0f;
	static bool bIsLocked = false;
	int nPreFrame = 0;

	SelfEffectUpdate();

	if(GetStatus() == PST_DEAD &&
		IsLastFrame())
	{
		return;
	}

	DWORD dwCurTick = GetTickCount();

	if(m_bIsLocked)
	{
		if(dwCurTick - m_dwLastLockedTime > 9000)
		{
			m_bIsLocked = false;
		}
	}

	if(GetStatus() != PST_WALK &&
		GetStatus() != PST_RUN)
	{
		m_bPlayOneStep = m_bPlayTwoStep = false;
	}

	AutoAttackTarget();

	//	技能update
	static float fLastUpdateSk = 0.0f;
	if(GetStatus() == PST_SK_LEIDIANSHU &&
		!IsLastFrame())
	{
		fLastUpdateSk += _dt;
		if(fLastUpdateSk > UPDATE_SKILL_INTERVAL)
		{
			m_pPlayer->SetCurFrame(m_pPlayer->GetCurFrame() + 1);
			fLastUpdateSk = 0.0f;
		}
	}
	else if(GetStatus() == PST_SK_LEIDIANSHU &&
		IsLastFrame())
	{
		fLastUpdateSk += _dt;
		if(fLastUpdateSk > UPDATE_SKILL_INTERVAL)
		{
			//SetStatus(PST_STAND);
			SetStatus(PST_ATTACKSTOP);
			fLastUpdateSk = 0.0f;
		}
	}

	//	攻击update
	static float fLastUpdateAttackTime = 0.0f;
	//float fAtkInterval = GetAttackInterval();
	if(m_pPlayer->GetStatus() == PST_ATTACKWEAPON &&
		!IsLastFrame())
	{
		fLastUpdateAttackTime += _dt;
		if(fLastUpdateAttackTime > /*(1.0f / ((float)m_attrib.atkSpeed + 8.0f))*//*fAtkInterval*/UPDATE_ATTACK_INTERVAL)
		{
			nPreFrame = GetCurFrame();
			m_pPlayer->SetCurFrame(m_pPlayer->GetCurFrame() + 1);
			fLastUpdateAttackTime = 0.0f;
			if(nPreFrame == 3)
			{
				PlayAttackSound();
			}
		}
	}
	else if(m_pPlayer->GetStatus() == PST_ATTACKWEAPON &&
		IsLastFrame())
	{
		fLastUpdateAttackTime += _dt;
		if(fLastUpdateAttackTime > UPDATE_ATTACK_INTERVAL)
		{
			if(m_bUsingPreLock)
			{
				fLastUpdateAttackTime = 0.0f;
				SetStatus(PST_ATTACKSTOP);
				SetCurFrame(0);
				if(TEST_FLAG_BOOL(m_dwHumEffectFlag, MMASK_SLIEHUO))
				{
					//
					MagicElement* pMagic = new MagicSuperFire;
					if(pMagic->Create(MEFF_SLIEHUO, this, this, GetObjx(), GetObjy(), GetDirection()))
					{
						pMagic->SetActive();
						GameScene::sThis->InsertNewMagic(pMagic);
					}
					else
					{
						SAFE_DELETE(pMagic);
					}
				}
				if(TEST_FLAG_BOOL(m_dwHumEffectFlag, MMASK_KTSWORD))
				{
					{
						//
						MagicElement* pMagic = new MagicSuperFire;
						if(pMagic->Create(MEFF_KTSWORD, this, this, GetObjx(), GetObjy(), GetDirection()))
						{
							pMagic->SetActive();
							GameScene::sThis->InsertNewMagic(pMagic);
						}
						else
						{
							SAFE_DELETE(pMagic);
						}
					}
				}
				ClearHitEffect();
			}
			else
			{
				//	假设还在等待服务器回应 则锁住
				if(IsWaitServerResponse() &&
					WAITSERVERRSP_NORMALATK == GetWaitServerResponseType())
				{
					//	nothing
				}
				else
				{
					fLastUpdateAttackTime = 0.0f;
					SetStatus(PST_ATTACKSTOP);
					SetCurFrame(0);
					if(TEST_FLAG_BOOL(m_dwHumEffectFlag, MMASK_SLIEHUO))
					{
						//
						MagicElement* pMagic = new MagicSuperFire;
						if(pMagic->Create(MEFF_SLIEHUO, this, this, GetObjx(), GetObjy(), GetDirection()))
						{
							pMagic->SetActive();
							GameScene::sThis->InsertNewMagic(pMagic);
						}
						else
						{
							SAFE_DELETE(pMagic);
						}
					}
					if(TEST_FLAG_BOOL(m_dwHumEffectFlag, MMASK_KTSWORD))
					{
						{
							//
							MagicElement* pMagic = new MagicSuperFire;
							if(pMagic->Create(MEFF_KTSWORD, this, this, GetObjx(), GetObjy(), GetDirection()))
							{
								pMagic->SetActive();
								GameScene::sThis->InsertNewMagic(pMagic);
							}
							else
							{
								SAFE_DELETE(pMagic);
							}
						}
					}
					ClearHitEffect();
				}
			}
		}
	}

	//	死亡update
	if(m_pPlayer->GetStatus() == PST_DEAD &&
		!IsLastFrame())
	{
		static float fLastUpdateDead = 0.0f;
		fLastUpdateDead += _dt;
		if(fLastUpdateDead > 0.2f)
		{
			m_pPlayer->SetCurFrame(m_pPlayer->GetCurFrame() + 1);
			fLastUpdateDead = 0.0f;
		}
	}
	else if(m_pPlayer->GetStatus() == PST_DEAD &&
		IsLastFrame())
	{
		//	死亡处理
	}

	//	空手攻击update
	static float fLastUpdateNoWeapon = 0.0f;
	if(m_pPlayer->GetStatus() == PST_ATTACKNOWEAPON &&
		!IsLastFrame())
	{
		fLastUpdateNoWeapon += _dt;
		if(fLastUpdateNoWeapon > /*(1.0f / ((float)m_attrib.atkSpeed + 10.0f))*//*fAtkInterval*/UPDATE_ATTACK_INTERVAL)
		{
			nPreFrame = GetCurFrame();
			m_pPlayer->SetCurFrame(m_pPlayer->GetCurFrame() + 1);
			fLastUpdateNoWeapon = 0.0f;
			if(nPreFrame == 3)
			{
				PlayAttackSound();
			}

			AfxGetHge()->System_Log("Swtich at %d", GetTickCount());
		}
	}
	else if(m_pPlayer->GetStatus() == PST_ATTACKNOWEAPON &&
		IsLastFrame())
	{
		fLastUpdateNoWeapon += _dt;
		if(fLastUpdateNoWeapon > /*(1.0f / ((float)m_attrib.atkSpeed + 10.0f)) + 0.2f*//*fAtkInterval*/UPDATE_ATTACK_INTERVAL)
		{
			if(m_bUsingPreLock)
			{
				SetCurFrame(0);
				SetStatus(PST_ATTACKSTOP);
				fLastUpdateNoWeapon = 0.0f;

				if(TEST_FLAG_BOOL(m_dwHumEffectFlag, MMASK_SLIEHUO))
				{
					//
					MagicElement* pMagic = new MagicSuperFire;
					if(pMagic->Create(MEFF_SLIEHUO, this, this, GetObjx(), GetObjy(), GetDirection()))
					{
						pMagic->SetActive();
						GameScene::sThis->InsertNewMagic(pMagic);
					}
					else
					{
						SAFE_DELETE(pMagic);
					}
				}
				if(TEST_FLAG_BOOL(m_dwHumEffectFlag, MMASK_KTSWORD))
				{
					{
						//
						MagicElement* pMagic = new MagicSuperFire;
						if(pMagic->Create(MEFF_KTSWORD, this, this, GetObjx(), GetObjy(), GetDirection()))
						{
							pMagic->SetActive();
							GameScene::sThis->InsertNewMagic(pMagic);
						}
						else
						{
							SAFE_DELETE(pMagic);
						}
					}
				}
				ClearHitEffect();
			}
			else
			{
				//	假设还在等待服务器回应 则锁住
				if(IsWaitServerResponse() &&
					WAITSERVERRSP_NORMALATK == GetWaitServerResponseType())
				{
					//	nothing
				}
				else
				{
					fLastUpdateNoWeapon = 0.0f;
					SetStatus(PST_ATTACKSTOP);
					SetCurFrame(0);
					if(TEST_FLAG_BOOL(m_dwHumEffectFlag, MMASK_SLIEHUO))
					{
						//
						MagicElement* pMagic = new MagicSuperFire;
						if(pMagic->Create(MEFF_SLIEHUO, this, this, GetObjx(), GetObjy(), GetDirection()))
						{
							pMagic->SetActive();
							GameScene::sThis->InsertNewMagic(pMagic);
						}
						else
						{
							SAFE_DELETE(pMagic);
						}
					}
					if(TEST_FLAG_BOOL(m_dwHumEffectFlag, MMASK_KTSWORD))
					{
						{
							//
							MagicElement* pMagic = new MagicSuperFire;
							if(pMagic->Create(MEFF_KTSWORD, this, this, GetObjx(), GetObjy(), GetDirection()))
							{
								pMagic->SetActive();
								GameScene::sThis->InsertNewMagic(pMagic);
							}
							else
							{
								SAFE_DELETE(pMagic);
							}
						}
					}
					ClearHitEffect();
				}
			}
		}
	}

	//	攻击停止状态update
	/*static float fLastUpdateStop = 0.0f;
	if(GetStatus() == PST_ATTACKSTOP &&
		!IsLastFrame())
	{
		//fLastUpdateStop += dt;
		m_fLastUpdateAttackStop += dt;
		if(m_fLastUpdateAttackStop > / *0.2f* /3.0f)
		{
			SetCurFrame(GetCurFrame() + 1);
			m_fLastUpdateAttackStop = 0.0f;
		}
	}
	else if(GetStatus() == PST_ATTACKSTOP &&
		IsLastFrame())
	{
		m_fLastUpdateAttackStop += _dt;
		if(m_fLastUpdateAttackStop > / *0.5f* /3.0f)
		{
			SetStatus(PST_STAND);
			m_fLastUpdateAttackStop = 0.0f;
		}
	}*/
	if(GetStatus() == PST_ATTACKSTOP)
	{
		if(GetTickCount() - m_dwLastAttackStopTime > 3000)
		{
			SetStatus(PST_STAND);
		}
	}


	//	受伤update
	static float fLastUpdateAtked = 0.0f;
	if(GetStatus() == PST_ATTACKED &&
		!IsLastFrame())
	{
		fLastUpdateAtked += dt;
		if(fLastUpdateAtked > 0.08f)
		{
			SetCurFrame(GetCurFrame() + 1);
			fLastUpdateAtked = 0.0f;
		}
	}
	else if(GetStatus() == PST_ATTACKED &&
		IsLastFrame())
	{
		fLastUpdateAtked += _dt;
		if(fLastUpdateAtked > 0.08f)
		{
			/*if(m_fLastUpdateAttackStop < 3.0f)
			{
				SetStatus(PST_ATTACKSTOP);
			}
			else*/
			{
				SetStatus(PST_STAND);
			}
			fLastUpdateAtked = 0.0f;
		}
	}

	/*
	if(m_pPlayer->GetObjx() != m_pPlayer->GetCoordX() * UNIT_WIDTH ||
	m_pPlayer->GetObjy() != m_pPlayer->GetCoordY() * UNIT_HEIGHT)*/

#define _NORMAL_MOVE_
#ifdef _NORMAL_MOVE_
	const float fMvParam = 1.15f;
	if(IsMoving())	
	{
		pd = m_pPlayer->GetDirection();
		float fMovePerSecond = GetMovePerSecond();

		switch (pd)
		{
		case PDT_RIGHT:
			{
				if(m_pPlayer->GetStatus() == PST_RUN)
				{
					fMoveOffsetX = /*dt * m_pPlayer->GetSpeed();*/dt * fMovePerSecond;
					m_pPlayer->SetPosxOffset(fMoveOffsetX);
					fMoveOffsetXTotal += fMoveOffsetX; 
					//	切换跑动帧
					m_pPlayer->CalTextureIndex(&nTotalFrame);
					m_pPlayer->SetCurFrame(fMoveOffsetXTotal / (96 /nTotalFrame));

					if(m_pPlayer->GetObjx() >= m_pPlayer->GetCoordX() * UNIT_WIDTH)
					{
						m_pPlayer->SetObjx(m_pPlayer->GetCoordX() * UNIT_WIDTH);
						m_pPlayer->SetCurFrame(nTotalFrame - 1);
						fMoveOffsetXTotal = fMoveOffsetYTotal = 0.0f;
					}
				}
				else if(m_pPlayer->GetStatus() == PST_WALK)
				{
					fMoveOffsetX = /*dt * m_pPlayer->GetSpeed() * 0.5f;*/dt * fMovePerSecond * 0.5;
					m_pPlayer->SetPosxOffset(fMoveOffsetX);
					fMoveOffsetXTotal += fMoveOffsetX; 
					//	切换跑动帧
					m_pPlayer->CalTextureIndex(&nTotalFrame);
					m_pPlayer->SetCurFrame(fMoveOffsetXTotal / (48 /nTotalFrame));

					if(m_pPlayer->GetObjx() >= m_pPlayer->GetCoordX() * UNIT_WIDTH)
					{
						m_pPlayer->SetObjx(m_pPlayer->GetCoordX() * UNIT_WIDTH);
						m_pPlayer->SetCurFrame(nTotalFrame - 1);
						fMoveOffsetXTotal = fMoveOffsetYTotal = 0.0f;
					}
				}
			}break;
		case PDT_LEFT:
			{
				if(m_pPlayer->GetStatus() == PST_RUN)
				{
					fMoveOffsetX = /*- dt * m_pPlayer->GetSpeed()*/- dt * fMovePerSecond;
					m_pPlayer->SetPosxOffset(fMoveOffsetX);
					fMoveOffsetXTotal += fMoveOffsetX; 
					//	切换跑动帧
					m_pPlayer->CalTextureIndex(&nTotalFrame);
					m_pPlayer->SetCurFrame(abs(fMoveOffsetXTotal / (96 /nTotalFrame)));

					if(m_pPlayer->GetObjx() <= m_pPlayer->GetCoordX() * UNIT_WIDTH)
					{
						m_pPlayer->SetObjx(m_pPlayer->GetCoordX() * UNIT_WIDTH);
						m_pPlayer->SetCurFrame(nTotalFrame - 1);
						fMoveOffsetXTotal = fMoveOffsetYTotal = 0.0f;
					}
				}
				else if(m_pPlayer->GetStatus() == PST_WALK)
				{
					fMoveOffsetX = /*-dt * m_pPlayer->GetSpeed() * 0.5f*/-dt * fMovePerSecond * 0.5f;
					m_pPlayer->SetPosxOffset(fMoveOffsetX);
					fMoveOffsetXTotal += fMoveOffsetX; 
					//	切换跑动帧
					m_pPlayer->CalTextureIndex(&nTotalFrame);
					m_pPlayer->SetCurFrame(abs(fMoveOffsetXTotal / (48 /nTotalFrame)));

					if(m_pPlayer->GetObjx() <= m_pPlayer->GetCoordX() * UNIT_WIDTH)
					{
						m_pPlayer->SetObjx(m_pPlayer->GetCoordX() * UNIT_WIDTH);
						m_pPlayer->SetCurFrame(nTotalFrame - 1);
						fMoveOffsetXTotal = fMoveOffsetYTotal = 0.0f;
					}
				}
			}break;
		case PDT_DOWN:
			{
				if(m_pPlayer->GetStatus() == PST_RUN)
				{
					/*fMoveOffsetY = dt * m_pPlayer->GetSpeed() * 0.7f;*/
					fMoveOffsetY = dt * fMovePerSecond * 0.7f;
					m_pPlayer->SetPosyOffset(fMoveOffsetY);
					fMoveOffsetYTotal += fMoveOffsetY;
					//	切换跑动帧
					m_pPlayer->CalTextureIndex(&nTotalFrame);
					m_pPlayer->SetCurFrame((abs(fMoveOffsetYTotal) / (64 /nTotalFrame)) > 5 ? 5 : (abs(fMoveOffsetYTotal) / (64 /nTotalFrame)));

					if(m_pPlayer->GetObjy() >= m_pPlayer->GetCoordY() * UNIT_HEIGHT)
					{
						m_pPlayer->SetObjy(m_pPlayer->GetCoordY() * UNIT_HEIGHT);
						m_pPlayer->SetCurFrame(nTotalFrame - 1);
						fMoveOffsetXTotal = fMoveOffsetYTotal = 0.0f;
					}
				}
				else if(m_pPlayer->GetStatus() == PST_WALK)
				{
					/*fMoveOffsetY = dt * m_pPlayer->GetSpeed() * 0.5f * 0.7f;*/
					fMoveOffsetY = dt *fMovePerSecond * 0.5f * 0.7f;
					m_pPlayer->SetPosyOffset(fMoveOffsetY);
					fMoveOffsetYTotal += fMoveOffsetY;
					//	切换跑动帧
					m_pPlayer->CalTextureIndex(&nTotalFrame);
					m_pPlayer->SetCurFrame((abs(fMoveOffsetYTotal) / (32 /nTotalFrame)) > 5 ? 5 :(abs(fMoveOffsetYTotal) / (32 /nTotalFrame)) );

					if(m_pPlayer->GetObjy() >= m_pPlayer->GetCoordY() * UNIT_HEIGHT)
					{
						m_pPlayer->SetObjy(m_pPlayer->GetCoordY() * UNIT_HEIGHT);
						m_pPlayer->SetCurFrame(nTotalFrame - 1);
						fMoveOffsetXTotal = fMoveOffsetYTotal = 0.0f;
					}
				}
			}break;
		case PDT_UP:
			{
				if(m_pPlayer->GetStatus() == PST_RUN)
				{
					/*fMoveOffsetY = - dt * m_pPlayer->GetSpeed() * 0.7f;*/
					fMoveOffsetY = - dt * fMovePerSecond * 0.7f;
					m_pPlayer->SetPosyOffset(fMoveOffsetY);
					fMoveOffsetYTotal += fMoveOffsetY;
					//	切换跑动帧
					m_pPlayer->CalTextureIndex(&nTotalFrame);
					m_pPlayer->SetCurFrame((abs(fMoveOffsetYTotal) / (64 /nTotalFrame)) > 5 ? 5 : (abs(fMoveOffsetYTotal) / (64 /nTotalFrame)));

					if(m_pPlayer->GetObjy() <= m_pPlayer->GetCoordY() * UNIT_HEIGHT)
					{
						m_pPlayer->SetObjy(m_pPlayer->GetCoordY() * UNIT_HEIGHT);
						m_pPlayer->SetCurFrame(nTotalFrame - 1);
						fMoveOffsetXTotal = fMoveOffsetYTotal = 0.0f;
					}
				}
				else if(m_pPlayer->GetStatus() == PST_WALK)
				{
					/*fMoveOffsetY = -dt * m_pPlayer->GetSpeed() * 0.5f * 0.7f;*/
					fMoveOffsetY = -dt * fMovePerSecond * 0.5f * 0.7f;
					m_pPlayer->SetPosyOffset(fMoveOffsetY);
					fMoveOffsetYTotal += fMoveOffsetY;
					//	切换跑动帧
					m_pPlayer->CalTextureIndex(&nTotalFrame);
					m_pPlayer->SetCurFrame((abs(fMoveOffsetYTotal) / (32 /nTotalFrame)) > 5 ? 5 : (abs(fMoveOffsetYTotal) / (32 /nTotalFrame)));

					if(m_pPlayer->GetObjy() <= m_pPlayer->GetCoordY() * UNIT_HEIGHT)
					{
						m_pPlayer->SetObjy(m_pPlayer->GetCoordY() * UNIT_HEIGHT);
						m_pPlayer->SetCurFrame(nTotalFrame - 1);
						fMoveOffsetXTotal = fMoveOffsetYTotal = 0.0f;
					}
				}
			}break;
		case PDT_RIGHTUP:
			{
				if(m_pPlayer->GetStatus() == PST_RUN)
				{
					/*
					fMoveOffsetX = dt * m_pPlayer->GetSpeed() * 0.6f * 1.4f;
										fMoveOffsetY = - dt * m_pPlayer->GetSpeed() * 0.4f * 1.4f;*/
					fMoveOffsetX = dt * fMovePerSecond * 0.6f * 1.4f * fMvParam;
					fMoveOffsetY = - dt *fMovePerSecond * 0.4f * 1.4f * fMvParam;
					m_pPlayer->SetPosxOffset(fMoveOffsetX);
					m_pPlayer->SetPosyOffset(fMoveOffsetY);
					fMoveOffsetXTotal += fMoveOffsetX;
					fMoveOffsetYTotal += fMoveOffsetY;
					//	切换跑动帧
					m_pPlayer->CalTextureIndex(&nTotalFrame);
					nMoveOffset = sqrt(pow((float)fMoveOffsetXTotal, 2) + pow((float)fMoveOffsetYTotal, 2));
					m_pPlayer->SetCurFrame((nMoveOffset / (RUN_HYPO_DISTANCE /nTotalFrame)) > (nTotalFrame - 1) ? (nTotalFrame - 1) : (nMoveOffset / (RUN_HYPO_DISTANCE /nTotalFrame)));

					if(m_pPlayer->GetObjy() <= m_pPlayer->GetCoordY() * UNIT_HEIGHT ||
						m_pPlayer->GetObjx() >= m_pPlayer->GetCoordX() * UNIT_WIDTH)
					{
						m_pPlayer->SetReallyCoord(m_pPlayer->GetCoordX(), m_pPlayer->GetCoordY());
						m_pPlayer->SetCurFrame(nTotalFrame - 1);
						fMoveOffsetXTotal = fMoveOffsetYTotal = 0.0f;
					}
				}
				else if(m_pPlayer->GetStatus() == PST_WALK)
				{
					/*
					fMoveOffsetX = dt * m_pPlayer->GetSpeed() * 0.6f * 0.7f;
										fMoveOffsetY = - dt * m_pPlayer->GetSpeed() * 0.4f * 0.7f;*/
					fMoveOffsetX = dt * fMovePerSecond * 0.6f * 0.7f * fMvParam;
					fMoveOffsetY = - dt * fMovePerSecond * 0.4f * 0.7f * fMvParam;
					m_pPlayer->SetPosxOffset(fMoveOffsetX);
					m_pPlayer->SetPosyOffset(fMoveOffsetY);
					fMoveOffsetXTotal += fMoveOffsetX;
					fMoveOffsetYTotal += fMoveOffsetY;
					//	切换跑动帧
					m_pPlayer->CalTextureIndex(&nTotalFrame);
					nMoveOffset = sqrt(pow((float)fMoveOffsetXTotal, 2) + pow((float)fMoveOffsetYTotal, 2));
					m_pPlayer->SetCurFrame((nMoveOffset / (RUN_HYPO_DISTANCE / 2 /nTotalFrame)) > (nTotalFrame - 1) ? (nTotalFrame - 1) : (nMoveOffset / (RUN_HYPO_DISTANCE / 2 /nTotalFrame)));

					if(m_pPlayer->GetObjy() <= m_pPlayer->GetCoordY() * UNIT_HEIGHT ||
						m_pPlayer->GetObjx() >= m_pPlayer->GetCoordX() * UNIT_WIDTH)
					{
						m_pPlayer->SetReallyCoord(m_pPlayer->GetCoordX(), m_pPlayer->GetCoordY());
						m_pPlayer->SetCurFrame(nTotalFrame - 1);
						fMoveOffsetXTotal = fMoveOffsetYTotal = 0.0f;
					}
				}
			}break;
		case PDT_RIGHTDOWN:
			{
				if(m_pPlayer->GetStatus() == PST_RUN)
				{
					/*
					fMoveOffsetX = dt * m_pPlayer->GetSpeed() * 0.6f * 1.4f;
										fMoveOffsetY = dt * m_pPlayer->GetSpeed() * 0.4f * 1.4f;*/
					fMoveOffsetX = dt * fMovePerSecond * 0.6f * 1.4f * fMvParam;
					fMoveOffsetY = dt * fMovePerSecond * 0.4f * 1.4f * fMvParam;
					m_pPlayer->SetPosxOffset(fMoveOffsetX);
					m_pPlayer->SetPosyOffset(fMoveOffsetY);
					fMoveOffsetXTotal += fMoveOffsetX;
					fMoveOffsetYTotal += fMoveOffsetY;
					//	切换跑动帧
					m_pPlayer->CalTextureIndex(&nTotalFrame);
					nMoveOffset = sqrt(pow((float)fMoveOffsetXTotal, 2) + pow((float)fMoveOffsetYTotal, 2));
					m_pPlayer->SetCurFrame((nMoveOffset / (RUN_HYPO_DISTANCE /nTotalFrame)) > (nTotalFrame - 1) ? (nTotalFrame - 1) : (nMoveOffset / (RUN_HYPO_DISTANCE /nTotalFrame)));

					if(m_pPlayer->GetObjy() >= m_pPlayer->GetCoordY() * UNIT_HEIGHT ||
						m_pPlayer->GetObjx() >= m_pPlayer->GetCoordX() * UNIT_WIDTH)
					{
						m_pPlayer->SetReallyCoord(m_pPlayer->GetCoordX(), m_pPlayer->GetCoordY());
						m_pPlayer->SetCurFrame(nTotalFrame - 1);
						fMoveOffsetXTotal = fMoveOffsetYTotal = 0.0f;
					}
				}
				else if(m_pPlayer->GetStatus() == PST_WALK)
				{
					/*
					fMoveOffsetX = dt * m_pPlayer->GetSpeed() * 0.6f * 0.7f;
										fMoveOffsetY = dt * m_pPlayer->GetSpeed() * 0.4f * 0.7f;*/
					fMoveOffsetX = dt * fMovePerSecond * 0.6f * 0.7f * fMvParam;
					fMoveOffsetY = dt * fMovePerSecond * 0.4f * 0.7f * fMvParam;
					m_pPlayer->SetPosxOffset(fMoveOffsetX);
					m_pPlayer->SetPosyOffset(fMoveOffsetY);
					fMoveOffsetXTotal += fMoveOffsetX;
					fMoveOffsetYTotal += fMoveOffsetY;
					//	切换跑动帧
					m_pPlayer->CalTextureIndex(&nTotalFrame);
					nMoveOffset = sqrt(pow((float)fMoveOffsetXTotal, 2) + pow((float)fMoveOffsetYTotal, 2));
					m_pPlayer->SetCurFrame((nMoveOffset / (RUN_HYPO_DISTANCE / 2 /nTotalFrame)) > (nTotalFrame - 1) ? (nTotalFrame - 1) : (nMoveOffset / (RUN_HYPO_DISTANCE / 2 /nTotalFrame)));

					if(m_pPlayer->GetObjy() >= m_pPlayer->GetCoordY() * UNIT_HEIGHT ||
						m_pPlayer->GetObjx() >= m_pPlayer->GetCoordX() * UNIT_WIDTH)
					{
						m_pPlayer->SetReallyCoord(m_pPlayer->GetCoordX(), m_pPlayer->GetCoordY());
						m_pPlayer->SetCurFrame(nTotalFrame - 1);
						fMoveOffsetXTotal = fMoveOffsetYTotal = 0.0f;
					}
				}
			}break;
		case PDT_LEFTUP:
			{
				if(m_pPlayer->GetStatus() == PST_RUN)
				{
					/*
					fMoveOffsetX = -dt * m_pPlayer->GetSpeed() * 0.6f * 1.4f;
										fMoveOffsetY = -dt * m_pPlayer->GetSpeed() * 0.4f * 1.4f;*/
					fMoveOffsetX = -dt * fMovePerSecond * 0.6f * 1.4f * fMvParam;
					fMoveOffsetY = -dt * fMovePerSecond * 0.4f * 1.4f * fMvParam;
					m_pPlayer->SetPosxOffset(fMoveOffsetX);
					m_pPlayer->SetPosyOffset(fMoveOffsetY);
					fMoveOffsetXTotal += fMoveOffsetX;
					fMoveOffsetYTotal += fMoveOffsetY;
					//	切换跑动帧
					m_pPlayer->CalTextureIndex(&nTotalFrame);
					nMoveOffset = sqrt(pow((float)fMoveOffsetXTotal, 2) + pow((float)fMoveOffsetYTotal, 2));
					m_pPlayer->SetCurFrame((nMoveOffset / (RUN_HYPO_DISTANCE /nTotalFrame)) > (nTotalFrame - 1) ? (nTotalFrame - 1) : (nMoveOffset / (RUN_HYPO_DISTANCE /nTotalFrame)));

					if(m_pPlayer->GetObjy() <= m_pPlayer->GetCoordY() * UNIT_HEIGHT ||
						m_pPlayer->GetObjx() <= m_pPlayer->GetCoordX() * UNIT_WIDTH)
					{
						m_pPlayer->SetReallyCoord(m_pPlayer->GetCoordX(), m_pPlayer->GetCoordY());
						m_pPlayer->SetCurFrame(nTotalFrame - 1);
						fMoveOffsetXTotal = fMoveOffsetYTotal = 0.0f;
					}
				}
				else if(m_pPlayer->GetStatus() == PST_WALK)
				{
					/*
					fMoveOffsetX = -dt * m_pPlayer->GetSpeed() * 0.6f * 0.7f;
										fMoveOffsetY = -dt * m_pPlayer->GetSpeed() * 0.4f * 0.7f;*/
					fMoveOffsetX = -dt * fMovePerSecond * 0.6f * 0.7f * fMvParam;
					fMoveOffsetY = -dt * fMovePerSecond * 0.4f * 0.7f * fMvParam;
					m_pPlayer->SetPosxOffset(fMoveOffsetX);
					m_pPlayer->SetPosyOffset(fMoveOffsetY);
					fMoveOffsetXTotal += fMoveOffsetX;
					fMoveOffsetYTotal += fMoveOffsetY;
					//	切换跑动帧
					m_pPlayer->CalTextureIndex(&nTotalFrame);
					nMoveOffset = sqrt(pow((float)fMoveOffsetXTotal, 2) + pow((float)fMoveOffsetYTotal, 2));
					m_pPlayer->SetCurFrame((nMoveOffset / (RUN_HYPO_DISTANCE / 2 /nTotalFrame)) > (nTotalFrame - 1) ? (nTotalFrame - 1) : (nMoveOffset / (RUN_HYPO_DISTANCE / 2 /nTotalFrame)));

					if(m_pPlayer->GetObjy() <= m_pPlayer->GetCoordY() * UNIT_HEIGHT ||
						m_pPlayer->GetObjx() <= m_pPlayer->GetCoordX() * UNIT_WIDTH)
					{
						m_pPlayer->SetReallyCoord(m_pPlayer->GetCoordX(), m_pPlayer->GetCoordY());
						m_pPlayer->SetCurFrame(nTotalFrame - 1);
						fMoveOffsetXTotal = fMoveOffsetYTotal = 0.0f;
					}
				}
			}break;
		case PDT_LEFTDOWN:
			{
				if(m_pPlayer->GetStatus() == PST_RUN)
				{
					/*
					fMoveOffsetX = -dt * m_pPlayer->GetSpeed() * 0.6f * 1.4f;
										fMoveOffsetY = dt * m_pPlayer->GetSpeed() * 0.4f * 1.4f;*/
					fMoveOffsetX = -dt * fMovePerSecond * 0.6f * 1.4f * fMvParam;
					fMoveOffsetY = dt * fMovePerSecond * 0.4f * 1.4f * fMvParam;
					m_pPlayer->SetPosxOffset(fMoveOffsetX);
					m_pPlayer->SetPosyOffset(fMoveOffsetY);
					fMoveOffsetXTotal += fMoveOffsetX;
					fMoveOffsetYTotal += fMoveOffsetY;
					//	切换跑动帧
					m_pPlayer->CalTextureIndex(&nTotalFrame);
					nMoveOffset = sqrt(pow((float)fMoveOffsetXTotal, 2) + pow((float)fMoveOffsetYTotal, 2));
					m_pPlayer->SetCurFrame((nMoveOffset / (RUN_HYPO_DISTANCE /nTotalFrame)) > (nTotalFrame - 1) ? (nTotalFrame - 1) : (nMoveOffset / (RUN_HYPO_DISTANCE /nTotalFrame)));

					if(m_pPlayer->GetObjy() >= m_pPlayer->GetCoordY() * UNIT_HEIGHT ||
						m_pPlayer->GetObjx() <= m_pPlayer->GetCoordX() * UNIT_WIDTH)
					{
						m_pPlayer->SetReallyCoord(m_pPlayer->GetCoordX(), m_pPlayer->GetCoordY());
						m_pPlayer->SetCurFrame(nTotalFrame - 1);
						fMoveOffsetXTotal = fMoveOffsetYTotal = 0.0f;
					}
				}
				else if(m_pPlayer->GetStatus() == PST_WALK)
				{
					/*
					fMoveOffsetX = -dt * m_pPlayer->GetSpeed() * 0.6f * 0.7f;
										fMoveOffsetY = dt * m_pPlayer->GetSpeed() * 0.4f * 0.7f;*/
					fMoveOffsetX = -dt * fMovePerSecond * 0.6f * 0.7f * fMvParam;
					fMoveOffsetY = dt * fMovePerSecond * 0.4f * 0.7f * fMvParam;
					m_pPlayer->SetPosxOffset(fMoveOffsetX);
					m_pPlayer->SetPosyOffset(fMoveOffsetY);
					fMoveOffsetXTotal += fMoveOffsetX;
					fMoveOffsetYTotal += fMoveOffsetY;
					//	切换跑动帧
					m_pPlayer->CalTextureIndex(&nTotalFrame);
					nMoveOffset = sqrt(pow((float)fMoveOffsetXTotal, 2) + pow((float)fMoveOffsetYTotal, 2));
					m_pPlayer->SetCurFrame((nMoveOffset / (RUN_HYPO_DISTANCE / 2 /nTotalFrame)) > (nTotalFrame - 1) ? (nTotalFrame - 1) : (nMoveOffset / (RUN_HYPO_DISTANCE / 2 /nTotalFrame)));

					if(m_pPlayer->GetObjy() >= m_pPlayer->GetCoordY() * UNIT_HEIGHT ||
						m_pPlayer->GetObjx() <= m_pPlayer->GetCoordX() * UNIT_WIDTH)
					{
						m_pPlayer->SetReallyCoord(m_pPlayer->GetCoordX(), m_pPlayer->GetCoordY());
						m_pPlayer->SetCurFrame(nTotalFrame - 1);
						fMoveOffsetXTotal = fMoveOffsetYTotal = 0.0f;
					}
				}
			}break;
		}

		if(GetCurFrame() == 1)
		{
			if(!m_bPlayOneStep)
			{
				m_bPlayOneStep = true;
				m_bPlayTwoStep = false;
				GameSoundManager::GetInstancePtr()->PlayHumSound(SDHUM_WALK);
			}
		}
		else if(GetCurFrame() == 4)
		{
			if(!m_bPlayTwoStep)
			{
				m_bPlayTwoStep = true;
				m_bPlayOneStep = false;
				GameSoundManager::GetInstancePtr()->PlayHumSound(SDHUM_WALK);
			}
		}
	}
#else
	if(IsMoving())
	{
		UpdateMove();
	}
#endif

	if(m_bHurt &&
		IsMoving() &&
		GetStatus() == PST_RUN)
	{
		int nOftX = abs(m_ptPos.x * UNIT_WIDTH - m_fPosx);
		int nOftY = abs(m_ptPos.y * UNIT_HEIGHT - m_fPosy);

// 		switch(GetDirection())
// 		{
// 		case PDT_UP:
// 			{
// 				if(nOftY > UNIT_HEIGHT)
// 				{
// 					m_ptPos.y += 1;
// 				}
// 			}break;
// 		case PDT_RIGHTUP:
// 			{
// 				if(nOftY > UNIT_HEIGHT)
// 				{
// 					m_ptPos.y += 1;
// 					m_ptPos.x -= 1;
// 				}
// 			}break;
// 		case PDT_RIGHT:
// 			{
// 				if(nOftX > UNIT_WIDTH)
// 				{
// 					m_ptPos.x -= 1;
// 				}
// 			}break;
// 		case PDT_RIGHTDOWN:
// 			{
// 				if(nOftX > UNIT_WIDTH)
// 				{
// 					m_ptPos.x -= 1;
// 					m_ptPos.y -= 1;
// 				}
// 			}break;
// 		case PDT_DOWN:
// 			{
// 				if(nOftY > UNIT_HEIGHT)
// 				{
// 					m_ptPos.y -= 1;
// 				}
// 			}break;
// 		case PDT_LEFTDOWN:
// 			{
// 				if(nOftY > UNIT_HEIGHT)
// 				{
// 					m_ptPos.x += 1;
// 					m_ptPos.y -= 1;
// 				}
// 			}break;
// 		case PDT_LEFT:
// 			{
// 				if(nOftX > UNIT_WIDTH)
// 				{
// 					m_ptPos.x += 1;
// 				}
// 			}break;
// 		case PDT_LEFTUP:
// 			{
// 				if(nOftX > UNIT_WIDTH)
// 				{
// 					m_ptPos.x += 1;
// 					m_ptPos.y += 1;
// 				}
// 			}break;
// 		}
	}

	if(m_bHurt &&
		!IsMoving() &&
		(GetStatus() != PST_ATTACKNOWEAPON && 
		GetStatus() != PST_ATTACKWEAPON && 
		GetStatus() != PST_SK_LEIDIANSHU))
	{
		if(GetStatus() == PST_DEAD)
		{
			return;
		}
		SetStatus(PST_ATTACKED);
		SetCurFrame(0);
		m_bHurt = false;

		if(GetAttrib()->sex == 2)
		{
			GameSoundManager::GetInstancePtr()->PlayHumSound(SDHUM_FEMALESTRUCK);
		}
		else
		{
			GameSoundManager::GetInstancePtr()->PlayHumSound(SDHUM_MALESTRUCK);
		}

#ifdef _NET_GAME_
		PkgUserActionReq req;
		req.uAction = ACTION_STRUCK;
		req.uUserId = GetHandlerID();
		req.uParam0 = MAKELONG(GetCoordX(), GetCoordY());
		req.uParam1 = GetDirection();
		g_xBuffer.Reset();
		g_xBuffer << req;
		SendBufferToGS(&g_xBuffer);
#endif
	}

	if(GetStatus() == PST_STAND)
	{
		/*if(m_fLastUpdateAttackStop < 3.0f &&
			m_fLastUpdateAttackStop != 0.0f)
		{
			m_stat = PST_ATTACKSTOP;
		}*/
		if(GetTickCount() - m_dwLastAttackStopTime < 3000)
		{
			//SetStatus(PST_ATTACKSTOP);
			m_stat = PST_ATTACKSTOP;
		}
	}


	static float tDetected = 0;
	if((m_pPlayer->GetStatus() == PST_WALK ||
		m_pPlayer->GetStatus() == PST_RUN) &&
		!m_pPlayer->IsMoving() &&
		IsLastFrame())
	{
		//	走动状态
		if(abs(tDetected - 0.0f) < 0.00001f)
		{
			tDetected = AfxGetHge()->Timer_GetTime();
		}
		else
		{
			if(abs(tDetected - AfxGetHge()->Timer_GetTime()) > 0.1f)
			{
				m_pPlayer->SetStatus(PST_STAND);
				m_pPlayer->SetCurFrame(0);
				tDetected = 0.0f;
			}
		}
	}
	// 	else if(m_pPlayer->GetStatus() == PST_ATTACKWEAPON &&
	// 		!m_pPlayer->IsMoving() &&
	// 		IsLastFrame())
	// 	{
	// 		//	攻击状态
	// 		if(abs(tDetected - 0.0f) < 0.00001f)
	// 		{
	// 			tDetected = AfxGetHge()->Timer_GetTime();
	// 		}
	// 		else
	// 		{
	// 			if(abs(tDetected - AfxGetHge()->Timer_GetTime()) > 0.2f)
	// 			{
	// 				m_pPlayer->SetStatus(PST_STAND);
	// 				m_pPlayer->SetCurFrame(0);
	// 				tDetected = 0.0f;
	// 			}
	// 		}
	// 	}
	// 	else if(m_pPlayer->GetStatus() == PST_ATTACKSTOP &&
	// 		!m_pPlayer->IsMoving() &&
	// 		IsLastFrame())
	// 	{
	// 		//	攻击状态
	// 		if(abs(tDetected - 0.0f) < 0.00001f)
	// 		{
	// 			tDetected = AfxGetHge()->Timer_GetTime();
	// 		}
	// 		else
	// 		{
	// 			if(abs(tDetected - AfxGetHge()->Timer_GetTime()) > 0.2f)
	// 			{
	// 				m_pPlayer->SetStatus(PST_STAND);
	// 				m_pPlayer->SetCurFrame(0);
	// 				tDetected = 0.0f;
	// 			}
	// 		}
	// 	}
	// 	else if(m_pPlayer->GetStatus() == PST_ATTACKED &&
	// 		IsLastFrame())
	// 	{
	// 		//	攻击状态
	// 		if(abs(tDetected - 0.0f) < 0.00001f)
	// 		{
	// 			tDetected = AfxGetHge()->Timer_GetTime();
	// 		}
	// 		else
	// 		{
	// 			if(abs(tDetected - AfxGetHge()->Timer_GetTime()) > 0.05f)
	// 			{
	// 				m_pPlayer->SetStatus(PST_STAND);
	// 				//m_pPlayer->SetStatus(m_pPlayer->GetPreStatus());
	// 				m_pPlayer->SetCurFrame(0);
	// 				tDetected = 0.0f;
	// 			}
	// 		}
	// 	}
	// 	else if(m_pPlayer->GetStatus() == PST_SK_LEIDIANSHU &&
	// 		IsLastFrame())
	// 	{
	// 		//	攻击状态
	// 		if(abs(tDetected - 0.0f) < 0.00001f)
	// 		{
	// 			tDetected = AfxGetHge()->Timer_GetTime();
	// 		}
	// 		else
	// 		{
	// 			if(abs(tDetected - AfxGetHge()->Timer_GetTime()) > 0.05f)
	// 			{
	// 				m_pPlayer->SetStatus(PST_STAND);
	// 				//m_pPlayer->SetStatus(m_pPlayer->GetPreStatus());
	// 				m_pPlayer->SetCurFrame(0);
	// 				tDetected = 0.0f;
	// 			}
	// 		}
	// 	}
	else
	{
		tDetected = 0.0f;
	}

	//	动态
	if(m_pPlayer->GetStatus() == PST_STAND)
	{
		if(TEST_FLAG_BOOL(m_dwHumEffectFlag, MMASK_STONE))
		{
			SetCurFrame(0);
		}
		else
		{
			int nFrm = ((int)((AfxGetHge()->Timer_GetTime() - m_pPlayer->GetLastStandTime()) / 0.5)) % 4;
			m_pPlayer->SetCurFrame(nFrm);
		}
	}

	//	更新人物装备效果
	UpdateHumState();

	//UpdatePlayerTexIndex();
}

// int GamePlayer::CalTextureIndex(int* _ptexsum /* = NULL */)
// {
// 	static const int indexTable[] = {60,80,61,81,62,82,63,83,64,84,85,88,86,89,87,90};
// 	int tex = m_equip[PLAYER_ITEM_CLOTH].tex;
// 	//int nIndex = m_equip[PLAYER_ITEM_CLOTH].id;
// 	bool bFind = false;
// 	for(int i = 0; i < sizeof(indexTable) / 4; ++i)
// 	{
// 		if(indexTable[i] == tex)
// 		{
// 			tex = i;
// 			bFind = true;
// 			break;
// 		}
// 	}
// 	int nIndex = 0;
// 	if(bFind)
// 	{
// 		nIndex = tex / 2 + 1;
// 	}
// 	else
// 	{
// 		nIndex = 0;
// 	}
// 	nIndex *= 1200;
// 	if(m_sex == PSX_WOMAN)
// 	{
// 		nIndex += 600;
// 	}
// 
// 	switch(m_stat)
// 	{
// 	case PST_STAND:
// 		{
// 			if(_ptexsum)
// 			{
// 				*_ptexsum = 4;
// 			}
// 			nIndex += 8 * m_drt;
// 		}break;
// 	case PST_WALK:
// 		{
// 			nIndex += 64;
// 			if(_ptexsum)
// 			{
// 				*_ptexsum = 6;
// 			}
// 			nIndex += 8 * m_drt;
// 		}break;
// 	case PST_RUN:
// 		{
// 			nIndex += 128;
// 			if(_ptexsum)
// 			{
// 				*_ptexsum = 6;
// 			}
// 			nIndex += 8 * m_drt;
// 		}break;
// 	case PST_ATTACKSTOP:
// 		{
// 			nIndex += 192;
// 			if(_ptexsum)
// 			{
// 				*_ptexsum = 1;
// 			}
// 			nIndex += m_drt;
// 		}break;
// 	case PST_ATTACKNOWEAPON:
// 		{
// 			nIndex += 200;
// 			if(_ptexsum)
// 			{
// 				*_ptexsum = 6;
// 			}
// 			nIndex += 8 * m_drt;
// 		}break;
// 	case PST_ATTACKWEAPON:
// 		{
// 			nIndex += 264;
// 			if(_ptexsum)
// 			{
// 				*_ptexsum = 6;
// 			}
// 			nIndex += 8 * m_drt;
// 		}break;
// 	case PST_FORCEATTACK:
// 		{
// 			nIndex += 328;
// 			if(_ptexsum)
// 			{
// 				*_ptexsum = 8;
// 			}
// 			nIndex += 8 * m_drt;
// 		}break;
// 	case PST_SK_LEIDIANSHU:
// 		{
// 			nIndex += 392;
// 			if(_ptexsum)
// 			{
// 				*_ptexsum = 6;
// 			}
// 			nIndex += 8 * m_drt;
// 		}break;
// 	case PST_WAROU:
// 		{
// 			nIndex += 456;
// 			if(_ptexsum)
// 			{
// 				*_ptexsum = 2;
// 			}
// 			nIndex += 2 * m_drt;
// 		}break;
// 	case PST_ATTACKED:
// 		{
// 			nIndex += 472;
// 			if(_ptexsum)
// 			{
// 				*_ptexsum = 3;
// 			}
// 			nIndex += 8 * m_drt;
// 		}break;
// 	case PST_DEAD:
// 		{
// 			nIndex += 536;
// 			if(_ptexsum)
// 			{
// 				*_ptexsum = 4;
// 			}
// 			nIndex += 8 * m_drt;
// 		}break;
// 	}
// 	return nIndex;
// }
// 
// int GamePlayer::CalHairTexIndex(int* _ptexsum /* = NULL */)
// {
// 	int nIndex = 1200;
// 	//nIndex += (m_equipAttrib.nHairStyle * 600);
// 	nIndex += (m_attrib.id * 600);
// 
// 	switch(m_stat)
// 	{
// 	case PST_STAND:
// 		{
// 			if(_ptexsum)
// 			{
// 				*_ptexsum = 4;
// 			}
// 			nIndex += 8 * m_drt;
// 		}break;
// 	case PST_WALK:
// 		{
// 			nIndex += 64;
// 			if(_ptexsum)
// 			{
// 				*_ptexsum = 6;
// 			}
// 			nIndex += 8 * m_drt;
// 		}break;
// 	case PST_RUN:
// 		{
// 			nIndex += 128;
// 			if(_ptexsum)
// 			{
// 				*_ptexsum = 6;
// 			}
// 			nIndex += 8 * m_drt;
// 		}break;
// 	case PST_ATTACKSTOP:
// 		{
// 			nIndex += 192;
// 			if(_ptexsum)
// 			{
// 				*_ptexsum = 1;
// 			}
// 			nIndex += m_drt;
// 		}break;
// 	case PST_ATTACKNOWEAPON:
// 		{
// 			nIndex += 200;
// 			if(_ptexsum)
// 			{
// 				*_ptexsum = 6;
// 			}
// 			nIndex += 8 * m_drt;
// 		}break;
// 	case PST_ATTACKWEAPON:
// 		{
// 			nIndex += 264;
// 			if(_ptexsum)
// 			{
// 				*_ptexsum = 6;
// 			}
// 			nIndex += 8 * m_drt;
// 		}break;
// 	case PST_FORCEATTACK:
// 		{
// 			nIndex += 328;
// 			if(_ptexsum)
// 			{
// 				*_ptexsum = 8;
// 			}
// 			nIndex += 8 * m_drt;
// 		}break;
// 	case PST_SK_LEIDIANSHU:
// 		{
// 			nIndex += 392;
// 			if(_ptexsum)
// 			{
// 				*_ptexsum = 8;
// 			}
// 			nIndex += 8 * m_drt;
// 		}break;
// 	case PST_WAROU:
// 		{
// 			nIndex += 456;
// 			if(_ptexsum)
// 			{
// 				*_ptexsum = 2;
// 			}
// 			nIndex += 2 * m_drt;
// 		}break;
// 	case PST_ATTACKED:
// 		{
// 			nIndex += 472;
// 			if(_ptexsum)
// 			{
// 				*_ptexsum = 3;
// 			}
// 			nIndex += 8 * m_drt;
// 		}break;
// 	case PST_DEAD:
// 		{
// 			nIndex += 536;
// 			if(_ptexsum)
// 			{
// 				*_ptexsum = 4;
// 			}
// 			nIndex += 8 * m_drt;
// 		}break;
// 	}
// 	return nIndex;
// }
// 
// int GamePlayer::CalWeaponTexIndex(int* _ptexsum /* = NULL */)
// {
// 	static const int indexTable[] = {30,43,31,36,32,67,33,33,34,34,35,35,40,40,39,39,
// 	38,38,37,37,41,41,42,42,45,45,47,47,44,44,46,46,48,48,49,49,50,50,51,51,52,52,53,53,54,54,
// 	55,55,56,5,57,57,58,58,59,59,65,65,66,66,69,69,70,70,71,71};
// 	bool bFind = false;
// 
// 	int nIndex = 1200;
// 	int nOffset = 0;
// 	if(m_equip[PLAYER_ITEM_WEAPON].type == ITEM_NO)
// 	{
// 		nOffset = 0;
// 		nIndex = 0;
// 		bFind = true;
// 	}
// 	else
// 	{
// 		for(int i = 0; i < sizeof(indexTable) / 4; ++i)
// 		{
// 			if(indexTable[i] == m_equip[PLAYER_ITEM_WEAPON].tex)
// 			{
// 				nOffset = i;
// 				bFind = true;
// 				break;
// 			}
// 		}
// 	}
// 	
// 	nIndex += (nOffset * 600);
// 
// 	switch(m_stat)
// 	{
// 	case PST_STAND:
// 		{
// 			if(_ptexsum)
// 			{
// 				*_ptexsum = 4;
// 			}
// 			nIndex += 8 * m_drt;
// 		}break;
// 	case PST_WALK:
// 		{
// 			nIndex += 64;
// 			if(_ptexsum)
// 			{
// 				*_ptexsum = 6;
// 			}
// 			nIndex += 8 * m_drt;
// 		}break;
// 	case PST_RUN:
// 		{
// 			nIndex += 128;
// 			if(_ptexsum)
// 			{
// 				*_ptexsum = 6;
// 			}
// 			nIndex += 8 * m_drt;
// 		}break;
// 	case PST_ATTACKSTOP:
// 		{
// 			nIndex += 192;
// 			if(_ptexsum)
// 			{
// 				*_ptexsum = 1;
// 			}
// 			nIndex += m_drt;
// 		}break;
// 	case PST_ATTACKNOWEAPON:
// 		{
// 			nIndex += 200;
// 			if(_ptexsum)
// 			{
// 				*_ptexsum = 6;
// 			}
// 			nIndex += 8 * m_drt;
// 		}break;
// 	case PST_ATTACKWEAPON:
// 		{
// 			nIndex += 264;
// 			if(_ptexsum)
// 			{
// 				*_ptexsum = 6;
// 			}
// 			nIndex += 8 * m_drt;
// 		}break;
// 	case PST_FORCEATTACK:
// 		{
// 			nIndex += 328;
// 			if(_ptexsum)
// 			{
// 				*_ptexsum = 8;
// 			}
// 			nIndex += 8 * m_drt;
// 		}break;
// 	case PST_SK_LEIDIANSHU:
// 		{
// 			nIndex += 392;
// 			if(_ptexsum)
// 			{
// 				*_ptexsum = 8;
// 			}
// 			nIndex += 8 * m_drt;
// 		}break;
// 	case PST_WAROU:
// 		{
// 			nIndex += 456;
// 			if(_ptexsum)
// 			{
// 				*_ptexsum = 2;
// 			}
// 			nIndex += 2 * m_drt;
// 		}break;
// 	case PST_ATTACKED:
// 		{
// 			nIndex += 472;
// 			if(_ptexsum)
// 			{
// 				*_ptexsum = 3;
// 			}
// 			nIndex += 8 * m_drt;
// 		}break;
// 	case PST_DEAD:
// 		{
// 			nIndex += 536;
// 			if(_ptexsum)
// 			{
// 				*_ptexsum = 4;
// 			}
// 			nIndex += 8 * m_drt;
// 		}break;
// 	}
// 	return nIndex;
// }

void GamePlayer::UpdatePlayerAttrib()
{
	ItemAttrib item;
	ZeroMemory(&item, sizeof(ItemAttrib));

	for(int i = PLAYER_ITEM_WEAPON; i < PLAYER_ITEM_TOTAL; ++i)
	{
		ItemAttrib& refItem = m_equip[i];

		if(m_equip[i].type == ITEM_NO)
		{
			continue;
		}

		int nPotentialIndex = PotentialAttribHelper::GetPotentialIndex(refItem.MP);
		const StoveAttribInfo* pStoveAttribInfo = StoveManager::GetInstance()->GetStoveAttrib(nPotentialIndex);

		float fDefenceMulti = 1.0f;
		float fAttackMulti = 1.0f;

		if(NULL != pStoveAttribInfo)
		{
			//	find enhance defence and attack attrib
			for(int i = 0; i < pStoveAttribInfo->nAttribCount; ++i)
			{
				int nAttribId = LOWORD(pStoveAttribInfo->dwAttribs[i]);
				int nAttribValue = HIWORD(pStoveAttribInfo->dwAttribs[i]);

				if(nAttribId == kStoveAttrib_EnhanceDefence)
				{
					fDefenceMulti += float(nAttribValue) / 100;
				}
				else if(nAttribId == kStoveAttrib_EnhanceAttack)
				{
					fAttackMulti += float(nAttribValue) / 100;
				}
			}
		}
		
		if(m_equip[i].type == ITEM_WEAPON)
		{
			//	武器不加防御 魔御 移动速度 麻痹 中毒 攻速 躲避
			//item.AC += m_equip[i].AC;
			//item.maxAC += m_equip[i].maxAC;
			item.DC += m_equip[i].DC;
			item.maxDC += m_equip[i].maxDC * fAttackMulti;
			item.SC += m_equip[i].SC;
			item.maxSC += m_equip[i].maxSC * fAttackMulti;
			item.MC += m_equip[i].MC;
			item.maxMC += m_equip[i].maxMC * fAttackMulti;
			//item.MAC += m_equip[i].MAC;
			//item.maxMAC += m_equip[i].maxMAC;
			//item.moveSpeed += m_equip[i].moveSpeed;
			//item.atkPalsy += m_equip[i].atkPalsy;
			//item.atkPois += m_equip[i].atkPois;
			//item.atkSpeed += m_equip[i].atkSpeed;
			item.lucky += m_equip[i].lucky;
			item.accuracy += m_equip[i].accuracy;
			//item.hide += m_equip[i].hide;
		}
		else if(m_equip[i].type == ITEM_HELMET)
		{
			//	头盔不加
			item.AC += m_equip[i].AC;
			item.maxAC += m_equip[i].maxAC * fDefenceMulti;
			item.DC += m_equip[i].DC;
			item.maxDC += m_equip[i].maxDC * fAttackMulti;
			item.SC += m_equip[i].SC;
			item.maxSC += m_equip[i].maxSC * fAttackMulti;
			item.MC += m_equip[i].MC;
			item.maxMC += m_equip[i].maxMC * fAttackMulti;
			item.MAC += m_equip[i].MAC;
			item.maxMAC += m_equip[i].maxMAC * fDefenceMulti;
			//item.moveSpeed += m_equip[i].moveSpeed;
			//item.atkPalsy += m_equip[i].atkPalsy;
			//item.atkPois += m_equip[i].atkPois;
			//item.atkSpeed += m_equip[i].atkSpeed;
			//item.lucky += m_equip[i].lucky;
			//item.accuracy += m_equip[i].accuracy;
			//item.hide += m_equip[i].hide;
		}
		else if(m_equip[i].type == ITEM_NECKLACE)
		{
			//item.AC += m_equip[i].AC;
			//item.maxAC += m_equip[i].maxAC;
			item.DC += m_equip[i].DC;
			item.maxDC += m_equip[i].maxDC * fAttackMulti;
			item.SC += m_equip[i].SC;
			item.maxSC += m_equip[i].maxSC * fAttackMulti;
			item.MC += m_equip[i].MC;
			item.maxMC += m_equip[i].maxMC * fAttackMulti;
			//item.MAC += m_equip[i].MAC;
			//item.maxMAC += m_equip[i].maxMAC;
			//item.moveSpeed += m_equip[i].moveSpeed;
			//item.atkPalsy += m_equip[i].atkPalsy;
			//item.atkPois += m_equip[i].atkPois;
			item.atkSpeed += m_equip[i].atkSpeed;
			item.lucky += m_equip[i].lucky;
			item.accuracy += m_equip[i].accuracy;
			item.hide += m_equip[i].hide;
		}
		else if(m_equip[i].type == ITEM_CLOTH)
		{
			item.AC += m_equip[i].AC;
			item.maxAC += m_equip[i].maxAC * fDefenceMulti;
			item.DC += m_equip[i].DC;
			item.maxDC += m_equip[i].maxDC * fAttackMulti;
			item.SC += m_equip[i].SC;
			item.maxSC += m_equip[i].maxSC * fAttackMulti;
			item.MC += m_equip[i].MC;
			item.maxMC += m_equip[i].maxMC * fAttackMulti;
			item.MAC += m_equip[i].MAC;
			item.maxMAC += m_equip[i].maxMAC * fDefenceMulti;
			//item.moveSpeed += m_equip[i].moveSpeed;
			//item.atkPalsy += m_equip[i].atkPalsy;
			//item.atkPois += m_equip[i].atkPois;
			//item.atkSpeed += m_equip[i].atkSpeed;
			//item.lucky += m_equip[i].lucky;
			//item.accuracy += m_equip[i].accuracy;
			//item.hide += m_equip[i].hide;
		}
		else if(m_equip[i].type == ITEM_MEDAL)
		{
			item.AC += m_equip[i].AC;
			item.maxAC += m_equip[i].maxAC * fDefenceMulti;
			item.DC += m_equip[i].DC;
			item.maxDC += m_equip[i].maxDC * fAttackMulti;
			item.SC += m_equip[i].SC;
			item.maxSC += m_equip[i].maxSC * fAttackMulti;
			item.MC += m_equip[i].MC;
			item.maxMC += m_equip[i].maxMC * fAttackMulti;
			item.MAC += m_equip[i].MAC;
			item.maxMAC += m_equip[i].maxMAC * fDefenceMulti;
			//item.moveSpeed += m_equip[i].moveSpeed;
			//item.atkPalsy += m_equip[i].atkPalsy;
			//item.atkPois += m_equip[i].atkPois;
			//item.atkSpeed += m_equip[i].atkSpeed;
			//item.lucky += m_equip[i].lucky;
			//item.accuracy += m_equip[i].accuracy;
			//item.hide += m_equip[i].hide;
		}
		else if(m_equip[i].type == ITEM_BRACELAT)
		{
			item.AC += m_equip[i].AC;
			item.maxAC += m_equip[i].maxAC * fDefenceMulti;
			item.DC += m_equip[i].DC;
			item.maxDC += m_equip[i].maxDC * fAttackMulti;
			item.SC += m_equip[i].SC;
			item.maxSC += m_equip[i].maxSC * fAttackMulti;
			item.MC += m_equip[i].MC;
			item.maxMC += m_equip[i].maxMC * fAttackMulti;
			item.MAC += m_equip[i].MAC;
			item.maxMAC += m_equip[i].maxMAC * fDefenceMulti;
			//item.moveSpeed += m_equip[i].moveSpeed;
			//item.atkPalsy += m_equip[i].atkPalsy;
			//item.atkPois += m_equip[i].atkPois;
			//item.atkSpeed += m_equip[i].atkSpeed;
			//item.lucky += m_equip[i].lucky;
			item.accuracy += m_equip[i].accuracy;
			item.hide += m_equip[i].hide;
		}
		else if(m_equip[i].type == ITEM_RING)
		{
			item.AC += m_equip[i].AC;
			item.maxAC += m_equip[i].maxAC * fDefenceMulti;
			item.DC += m_equip[i].DC;
			item.maxDC += m_equip[i].maxDC * fAttackMulti;
			item.SC += m_equip[i].SC;
			item.maxSC += m_equip[i].maxSC * fAttackMulti;
			item.MC += m_equip[i].MC;
			item.maxMC += m_equip[i].maxMC * fAttackMulti;
			item.MAC += m_equip[i].MAC;
			item.maxMAC += m_equip[i].maxMAC * fDefenceMulti;
			item.moveSpeed += m_equip[i].moveSpeed;
			item.atkPalsy += m_equip[i].atkPalsy;
			//item.atkPois += m_equip[i].atkPois;
			item.atkSpeed += m_equip[i].atkSpeed;
			//item.lucky += m_equip[i].lucky;
			//item.accuracy += m_equip[i].accuracy;
			//item.hide += m_equip[i].hide;
		}
		else if(m_equip[i].type == ITEM_SHOE)
		{
			item.moveSpeed += m_equip[i].moveSpeed;
			item.AC += m_equip[i].AC;
			item.maxAC += m_equip[i].maxAC * fDefenceMulti;
			item.DC += m_equip[i].DC;
			item.maxDC += m_equip[i].maxDC * fAttackMulti;
			item.SC += m_equip[i].SC;
			item.maxSC += m_equip[i].maxSC * fAttackMulti;
			item.MC += m_equip[i].MC;
			item.maxMC += m_equip[i].maxMC * fAttackMulti;
			item.MAC += m_equip[i].MAC;
			item.maxMAC += m_equip[i].maxMAC * fDefenceMulti;
		}
		else if(m_equip[i].type == ITEM_BELT)
		{
			item.AC += m_equip[i].AC;
			item.maxAC += m_equip[i].maxAC * fDefenceMulti;
			item.DC += m_equip[i].DC;
			item.maxDC += m_equip[i].maxDC * fAttackMulti;
			item.SC += m_equip[i].SC;
			item.maxSC += m_equip[i].maxSC * fAttackMulti;
			item.MC += m_equip[i].MC;
			item.maxMC += m_equip[i].maxMC * fAttackMulti;
			item.MAC += m_equip[i].MAC;
			item.maxMAC += m_equip[i].maxMAC * fDefenceMulti;
		}
		else if(m_equip[i].type == ITEM_GEM)
		{
			item.AC += m_equip[i].AC;
			item.maxAC += m_equip[i].maxAC * fDefenceMulti;
			item.DC += m_equip[i].DC;
			item.maxDC += m_equip[i].maxDC * fAttackMulti;
			item.SC += m_equip[i].SC;
			item.maxSC += m_equip[i].maxSC * fAttackMulti;
			item.MC += m_equip[i].MC;
			item.maxMC += m_equip[i].maxMC * fAttackMulti;
			item.MAC += m_equip[i].MAC;
			item.maxMAC += m_equip[i].maxMAC * fDefenceMulti;
		}
		else if(m_equip[i].type == ITEM_CHARM)
		{
			item.AC += m_equip[i].AC;
			item.maxAC += m_equip[i].maxAC * fDefenceMulti;
			item.DC += m_equip[i].DC;
			item.maxDC += m_equip[i].maxDC * fAttackMulti;
			item.SC += m_equip[i].SC;
			item.maxSC += m_equip[i].maxSC * fAttackMulti;
			item.MC += m_equip[i].MC;
			item.maxMC += m_equip[i].maxMC * fAttackMulti;
			item.MAC += m_equip[i].MAC;
			item.maxMAC += m_equip[i].maxMAC * fDefenceMulti;
		}
		/*item.AC += m_equip[i].AC;
		item.maxAC += m_equip[i].maxAC;
		item.DC += m_equip[i].DC;
		item.maxDC += m_equip[i].maxDC;
		item.SC += m_equip[i].SC;
		item.maxSC += m_equip[i].maxSC;
		item.MC += m_equip[i].MC;
		item.maxMC += m_equip[i].maxMC;
		item.MAC += m_equip[i].MAC;
		item.maxMAC += m_equip[i].maxMAC;
		item.moveSpeed += m_equip[i].moveSpeed;
		item.atkPalsy += m_equip[i].atkPalsy;
		item.atkPois += m_equip[i].atkPois;
		item.atkSpeed += m_equip[i].atkSpeed;
		item.lucky += m_equip[i].lucky;
		item.accuracy += m_equip[i].accuracy;
		item.hide += m_equip[i].hide;*/
		//	隐藏属性
		if(refItem.maxMP != 0)
		{
			int nActiveAttribSum = HideAttribHelper::GetActiveAttribCount(refItem.maxMP);

			for(int i = 0; i < nActiveAttribSum; ++i)
			{
				int nActiveAttribType = HideAttribHelper::GetActiveAttribType(i, refItem.maxMP);
				int nActiveAttribValue = HideAttribHelper::GetActiveAttribValue(i, refItem.maxMP);

				/*if(HideAttrib_AC == nActiveAttribType)
				{
					item.maxAC += nActiveAttribValue;
				}
				else if(HideAttrib_MAC == nActiveAttribType)
				{
					item.maxMAC += nActiveAttribValue;
				}
				else if(HideAttrib_DC == nActiveAttribType)
				{
					item.maxDC += nActiveAttribValue;
				}
				else if(HideAttrib_MC == nActiveAttribType)
				{
					item.maxMC += nActiveAttribValue;
				}
				else if(HideAttrib_SC == nActiveAttribType)
				{
					item.maxSC += nActiveAttribValue;
				}
				else if(HideAttrib_DcHide == nActiveAttribType)
				{
					item.hide += nActiveAttribValue;
				}
				else if(HideAttrib_MagicHide == nActiveAttribType)
				{
					item.accuracy += nActiveAttribValue;
				}
				else */if(HideAttrib_Lucky == nActiveAttribType)
				{
					item.lucky += nActiveAttribValue;
				}
			}
		}

		//	stove attrib
		if(NULL != pStoveAttribInfo)
		{
			for(int i = 0; i < pStoveAttribInfo->nAttribCount; ++i)
			{
				int nAttribId = LOWORD(pStoveAttribInfo->dwAttribs[i]);
				int nAttribValue = HIWORD(pStoveAttribInfo->dwAttribs[i]);

				if(0 == nAttribId ||
					0 == nAttribValue)
				{
					continue;
				}

				switch(nAttribId)
				{
				case kStoveAttrib_NormalAttackSpeed:
					{
						item.atkSpeed += nAttribValue;
					}break;
				}
			}
		}
	}

	item.HP = m_attrib.HP;
	item.maxHP = m_attrib.maxHP;
	item.MP = m_attrib.MP;
	item.maxMP = m_attrib.maxMP;
	item.EXPR = m_attrib.EXPR;
	item.maxEXPR = m_attrib.maxEXPR;
	item.level = m_attrib.level;
	item.sex = m_attrib.sex;
	item.price = m_attrib.price;
	//item.moveSpeed = item.moveSpeed + m_attrib.moveSpeed;
	item.id = m_attrib.id;
	strcpy(item.name, m_attrib.name);

	int n7MinAdd = m_attrib.level / 5;
	int n7MaxAdd = n7MinAdd + 1;
	int n14MinAdd = m_attrib.level / 10;
	int n14MaxAdd = n14MinAdd + 1;

	if(GetHeroJob() == 0)
	{
		item.DC += n7MinAdd;
		item.maxDC += n7MaxAdd;
		item.AC += n7MinAdd;
		item.maxAC += n7MaxAdd;
	}
	else if(GetHeroJob() == 1)
	{
		item.MC += n7MinAdd;
		item.maxMC += n7MaxAdd;
		item.MAC += n7MinAdd;
		item.maxMAC += n7MaxAdd;
	}
	else if(GetHeroJob() == 2)
	{
		item.SC += n7MinAdd;
		item.maxSC += n7MaxAdd;
		item.DC += n14MinAdd;
		item.maxDC += n14MaxAdd;
		item.AC += n14MinAdd;
		item.maxAC += n14MaxAdd;
		item.MAC += n14MinAdd;
		item.maxMAC += n14MaxAdd;
	}

	m_attrib = item;

	if(GetHeroJob() == 0)
	{
		const MagicDetail* pDetail = GetMagicByID(MEFF_BASESWORD);
		if(NULL != pDetail)
		{
			if(pDetail->cLevel == 1)
			{
				m_attrib.accuracy += 1;
			}
			else if(pDetail->cLevel == 2)
			{
				m_attrib.accuracy += 3;
			}
			else if(pDetail->cLevel == 3)
			{
				m_attrib.accuracy += 6;
			}
		}

		pDetail = GetMagicByID(MEFF_BERSERKER);
		if(NULL != pDetail)
		{
			if(pDetail->cLevel == 1)
			{
				m_attrib.atkSpeed += 1;
				m_attrib.DC *= 1.1f;
				m_attrib.maxDC *= 1.3f;
				m_attrib.maxAC += 4;
				m_attrib.accuracy += 2;
			}
			else if(pDetail->cLevel == 2)
			{
				m_attrib.atkSpeed += 2;
				m_attrib.DC *= 1.2f;
				m_attrib.maxDC *= 1.4f;
				m_attrib.maxAC += 7;
				m_attrib.accuracy += 3;
			}
			else if(pDetail->cLevel == 3)
			{
				m_attrib.atkSpeed += 3;
				m_attrib.DC *= 1.3f;
				m_attrib.maxDC *= 1.5f;
				m_attrib.maxAC += 10;
				m_attrib.accuracy += 4;
			}
		}

		pDetail = GetMagicByID(MEFF_JINGANG);
		if(NULL != pDetail)
		{
			m_attrib.AC += pDetail->cLevel * 10;
			m_attrib.maxAC += pDetail->cLevel * 10;
			m_attrib.MAC += pDetail->cLevel * 10;
			m_attrib.maxMAC += pDetail->cLevel * 10;
		}

		//m_attrib.atkSpeed += 4;
		//m_attrib.atkSpeed += (m_attrib.level / 10);
		//m_attrib.atkSpeed += 1;
	}
	else if(GetHeroJob() == 2)
	{
		const MagicDetail* pDetail = GetMagicByID(MEFF_SPIRIT);
		if(NULL != pDetail)
		{
			if(pDetail->cLevel == 1)
			{
				m_attrib.accuracy += 1;
			}
			else if(pDetail->cLevel == 2)
			{
				m_attrib.accuracy += 2;
				m_attrib.hide += 1;
			}
			else if(pDetail->cLevel == 3)
			{
				m_attrib.accuracy += 3;
				m_attrib.hide += 2;
			}
		}
	}

	ItemAttrib* pItem = GetPlayerItem(PLAYER_ITEM_WEAPON);
	if(pItem &&
		pItem->type == ITEM_WEAPON)
	{
		int nLevel = GetGrowLevelFromDWORD(pItem->HP);

		if(nLevel != 0)
		{
			/*if(GetHeroJob() == 0)
			{
				m_attrib.maxDC += nLevel * 2;
			}
			else if(GetHeroJob() == 1)
			{
				m_attrib.maxMC += nLevel * 2;
			}
			else if(GetHeroJob() == 2)
			{
				m_attrib.maxSC += nLevel * 2;
			}*/

			m_attrib.lucky += nLevel / 3;
		}
	}

	UpdateSuitAttrib();
	UpdateSuitSameLevel();
	//	同时更新纹理索引
	//UpdatePlayerTexIndex();
}

//////////////////////////////////////////////////////////////////////////
void GamePlayer::UpdateSuitAttrib()
{
	//	计算套装附加属性
	ItemAttrib suitAttrib;
	ZeroMemory(&suitAttrib, sizeof(ItemAttrib));

	std::list<const ItemExtraAttribList*> xSuitAttribInfoList;
	std::list<const ItemExtraAttribList*>::const_iterator begIter;
	std::list<const ItemExtraAttribList*>::const_iterator endIter;
	bool bExist = false;
	const ItemExtraAttribList* pCurAttribList = NULL;

	//	Get all suit attrib information
	for(int i = 0; i < PLAYER_ITEM_TOTAL; ++i)
	{
		if(m_equip[i].type != ITEM_NO)
		{
			if(m_equip[i].atkPalsy != 0)
			{
				pCurAttribList = GameInfoManager::GetInstance()->GetItemExtraSuitAttribList(m_equip[i].atkPalsy);

				if(pCurAttribList != NULL)
				{
					begIter = xSuitAttribInfoList.begin();
					endIter = xSuitAttribInfoList.end();
					bExist = false;

					for(begIter;
						begIter != endIter;
						++begIter)
					{
						if(*begIter == pCurAttribList)
						{
							bExist = true;
							break;
						}
					}

					if(!bExist)
					{
						xSuitAttribInfoList.push_back(GameInfoManager::GetInstance()->GetItemExtraSuitAttribList(m_equip[i].atkPalsy));
					}
				}
			}
		}
	}

	//	Calculate every suit list on every item on body
	bool bActivePos[10] = {false};
	int nActiveNumber = 0;

	if(!xSuitAttribInfoList.empty())
	{
		begIter = xSuitAttribInfoList.begin();
		endIter = xSuitAttribInfoList.end();

		for(begIter;
			begIter != endIter;
			++begIter)
		{
			pCurAttribList = *begIter;
			ZeroMemory(bActivePos, sizeof(bActivePos));
			nActiveNumber = 0;

			if(NULL != pCurAttribList)
			{
				for(int i = 0; i < PLAYER_ITEM_TOTAL; ++i)
				{
					const ItemAttrib* pItem = &m_equip[i];

					if(pItem->type != ITEM_NO &&
						pItem->atkPalsy == pCurAttribList->nSuitID)
					{
						for(int j = 0; j < 10; ++j)
						{
							if(pCurAttribList->nSuitEquipID[j] == pItem->id &&
								!bActivePos[j] &&
								pCurAttribList->nSuitEquipID[j] != 0)
							{
								bActivePos[j] = true;
								++nActiveNumber;
								break;
							}
						}
					}
				}

				int nActiveAttribNumber = 0;
				int nActiveMax = 0;

				if(nActiveNumber != 0)
				{
					//	Activate some suit attrib,then update the player's attrib
					ZeroMemory(&suitAttrib, sizeof(ItemAttrib));

					for(int i = 0; i < MAX_EXTRAATTIRB; ++i)
					{
						if(nActiveNumber >= pCurAttribList->nActiveSum[i] &&
							pCurAttribList->nActiveSum[i] > nActiveMax)
						{
							nActiveMax = pCurAttribList->nActiveSum[i];
							nActiveAttribNumber = pCurAttribList->nActiveAttribSum[i];
						}
					}

					//	Now we can calculate the attrib that the suit bring to us
					for(int i = 0; i < nActiveAttribNumber; ++i)
					{
						int nCurValue = pCurAttribList->stExtraAttrib[i].nAttribValue;

						switch(pCurAttribList->stExtraAttrib[i].nAttribID)
						{
						case EAID_DC:
							{
								suitAttrib.DC += HIWORD(pCurAttribList->stExtraAttrib[i].nAttribValue);
								suitAttrib.maxDC += LOWORD(pCurAttribList->stExtraAttrib[i].nAttribValue);

								m_attrib.DC += HIWORD(pCurAttribList->stExtraAttrib[i].nAttribValue);
								m_attrib.maxDC += LOWORD(pCurAttribList->stExtraAttrib[i].nAttribValue);
							}break;
						case EAID_AC:
							{
								suitAttrib.AC += HIWORD(pCurAttribList->stExtraAttrib[i].nAttribValue);
								suitAttrib.maxAC += LOWORD(pCurAttribList->stExtraAttrib[i].nAttribValue);

								m_attrib.AC += HIWORD(pCurAttribList->stExtraAttrib[i].nAttribValue);
								m_attrib.maxAC += LOWORD(pCurAttribList->stExtraAttrib[i].nAttribValue);
							}break;
						case EAID_MAC:
							{
								suitAttrib.MAC += HIWORD(pCurAttribList->stExtraAttrib[i].nAttribValue);
								suitAttrib.maxMAC += LOWORD(pCurAttribList->stExtraAttrib[i].nAttribValue);

								m_attrib.MAC += HIWORD(pCurAttribList->stExtraAttrib[i].nAttribValue);
								m_attrib.maxMAC += LOWORD(pCurAttribList->stExtraAttrib[i].nAttribValue);
							}break;
						case EAID_MC:
							{
								suitAttrib.MC += HIWORD(pCurAttribList->stExtraAttrib[i].nAttribValue);
								suitAttrib.maxMC += LOWORD(pCurAttribList->stExtraAttrib[i].nAttribValue);

								m_attrib.MC += HIWORD(pCurAttribList->stExtraAttrib[i].nAttribValue);
								m_attrib.maxMC += LOWORD(pCurAttribList->stExtraAttrib[i].nAttribValue);
							}break;
						case EAID_SC:
							{
								suitAttrib.SC += HIWORD(pCurAttribList->stExtraAttrib[i].nAttribValue);
								suitAttrib.maxSC += LOWORD(pCurAttribList->stExtraAttrib[i].nAttribValue);

								m_attrib.SC += HIWORD(pCurAttribList->stExtraAttrib[i].nAttribValue);
								m_attrib.maxSC += LOWORD(pCurAttribList->stExtraAttrib[i].nAttribValue);
							}break;
						case EAID_LUCKY:
							{
								suitAttrib.lucky += pCurAttribList->stExtraAttrib[i].nAttribValue;

								m_attrib.lucky += pCurAttribList->stExtraAttrib[i].nAttribValue;
							}break;
						case EAID_ATKSPEED:
							{
								suitAttrib.atkSpeed += pCurAttribList->stExtraAttrib[i].nAttribValue;

								m_attrib.atkSpeed += pCurAttribList->stExtraAttrib[i].nAttribValue;
							}break;
						case EAID_ACCURACY:
							{
								suitAttrib.accuracy += nCurValue;

								m_attrib.accuracy += nCurValue;
							}break;
						case EAID_MOVESPEED:
							{
								suitAttrib.moveSpeed += nCurValue;

								m_attrib.moveSpeed += nCurValue;
							}break;
						}
					}

#ifdef _DEBUG
					AfxGetHge()->System_Log("Suit[%d] AC:%d MAXAC:%d MAC:%d MAXMAC: %d DC:%d MAXDC:%d MC:%d MAXMC: %d SC:%d MAXSC:%d LUCKY:%d ATKSPEED:%d ACCURACY:%d MOVESPEED:%d",
						pCurAttribList->nSuitID, suitAttrib.AC, suitAttrib.maxAC, suitAttrib.MAC, suitAttrib.maxMAC, suitAttrib.DC, suitAttrib.maxDC, suitAttrib.MC, suitAttrib.maxMC, suitAttrib.SC, suitAttrib.maxSC, suitAttrib.lucky, suitAttrib.atkSpeed, suitAttrib.accuracy, suitAttrib.moveSpeed);
#endif
				}
			}
		}
	}
}

void GamePlayer::UpdateSuitSameLevel()
{
	
	//	计算套装附加属性
	int nSuitExtraType = 0;
	int nSuitCount[9] = {0};

	//	Get all suit attrib information
	for(int i = 0; i < PLAYER_ITEM_TOTAL; ++i)
	{
		if(m_equip[i].type != ITEM_NO)
		{
			int nQualityIndex = GetItemUpgrade(m_equip[i].level);

			if (0 < nQualityIndex &&
				nQualityIndex < 9)
			{
				nSuitCount[nQualityIndex]++;
			}
		}
	}

	//	check active
	for (int i = 0; i < 9; ++i)
	{
		if (nSuitCount[i] >= 5)
		{
			nSuitExtraType = i;
			break;
		}
	}

	GameStatusDlg* pDlg = GameScene::sThis->GetStatusDlg();

	if (NULL == pDlg)
	{
		return;
	}

	pDlg->RemoveStatus(GSTATUS_SUITSAMELEVEL);

	if (0 != nSuitExtraType)
	{
		//	add status
		pDlg->AddStatus(GSTATUS_SUITSAMELEVEL, MAKELONG(nSuitExtraType, 0xffff));
	}
}


// void GamePlayer::UpdatePlayerTexIndex()
// {
// 	m_indexs.wHum = CalTextureIndex();
// 	m_indexs.wHair = CalHairTexIndex();
// 	m_indexs.wWeapon = CalWeaponTexIndex();
// }
//////////////////////////////////////////////////////////////////////////
const MagicDetail* GamePlayer::GetMagicByID(DWORD _dwMgcID)
{
	for(int i = 0; i < USER_MAGIC_NUM; ++i)
	{
		if(m_stMagicDetail[i].wID == _dwMgcID)
		{
			return &m_stMagicDetail[i];
		}
	}
	return NULL;
}
//////////////////////////////////////////////////////////////////////////
void GamePlayer::SetPlayerItem(PLAYER_ITEM_TYPE _type, const ItemAttrib* _item)
{
	if(_item == NULL)
	{
		ZeroMemory(&m_equip[_type], sizeof(ItemAttrib));
		return;
	}
	memcpy(&m_equip[_type], _item, sizeof(ItemAttrib));
	UpdatePlayerAttrib();
}

bool GamePlayer::CanEquip(const ItemAttrib* _item, DWORD* _err)
{
	if(!IsEquipItem(*_item) &&
		_item->type != ITEM_BOOK)
	{
		if(_err)
		{
			*_err = REQ_ERR_ISTEP;
		}
		return false;
	}

	switch(_item->reqType)
	{
	case 0:
		{
			//	通用
			return true;
		}break;
	case REQ_LEVEL:
		{
			//	等级
			if(m_attrib.level < _item->reqValue)
			{
				if(_err)
				{
					*_err = REQ_ERR_LEVEL;
				}
				return false;
			}
			else
			{
				//return true;
			}
		}break;
	case REQ_DC:
		{
			//	攻击
			if(m_attrib.maxDC < _item->reqValue)
			{
				if(_err)
				{
					*_err = REQ_ERR_DC;
				}
				return false;
			}
			else
			{
				//return true;
			}
		}break;
	case REQ_MC:
		{
			//	魔法力
			if(m_attrib.maxMC < _item->reqValue)
			{
				if(_err)
				{
					*_err = REQ_ERR_MC;
				}
				return false;
			}
			else
			{
				//return true;
			}
		}break;
	case REQ_SC:
		{
			//	道术
			if(m_attrib.maxSC < _item->reqValue)
			{
				if(_err)
				{
					*_err = REQ_ERR_SC;
				}
				return false;
			}
			else
			{
				//return true;
			}
		}break;
		// 	case REQ_SEX:
		// 		{
		// 			//	性别
		// 			if(m_sex != _item->reqValue)
		// 			{
		// 				if(_err)
		// 				{
		// 					*_err = REQ_ERR_SEX;
		// 				}
		// 				return false;
		// 			}
		// 			else
		// 			{
		// 				return true;
		// 			}
		// 		}break;
	}

	if(_item->sex == 0)
	{
		return true;
	}

	if(m_attrib.sex != _item->sex)
	{
		if(_err)
		{
			*_err = REQ_SEX;
		}
		return false;
	}
	else
	{
		return true;
	}

	if(_err)
	{
		*_err = REQ_ERR_UNKNOWREQ;
	}
	return false;
}

bool GamePlayer::DressEquip(int _bagidx, PLAYER_ITEM_TYPE _type, DWORD* _err /* = NULL */)
{
	ItemAttrib& bagItem = GetPlayerBag()->GetItemList()[_bagidx];
	if(bagItem.type == ITEM_NO ||
		bagItem.id == -1)
	{
		//	背包物品不存在
		if(_err)
		{
			*_err = REQ_ERR_NOTHING;
		}
		goto lbl_showerr;
	}

	//	判断是否是同类型物品
	BYTE type = PlayerItem2Item(_type);
	if(type != bagItem.type)
	{
		return false;
	}

	if(!CanEquip(&bagItem, _err))
	{
		//	不满足要求
		goto lbl_showerr;
	}

	ItemAttrib& dressItem = m_equip[_type];
	//	穿戴装备
	if(dressItem.type != ITEM_NO)
	{
		//	身上装备不为空 需要换装
		ItemAttrib item;
		memcpy(&item, &dressItem, sizeof(ItemAttrib));
		memcpy(&dressItem, &bagItem, sizeof(ItemAttrib));
		ZeroMemory(&bagItem, sizeof(ItemAttrib));
		m_bag.AddItem(&item);
		GameInfoBoardDlg::GetInstance()->ShowItemMsg(&item);
	}
	else
	{
		//	直接穿上
		memcpy(&dressItem, &bagItem, sizeof(ItemAttrib));
		ZeroMemory(&bagItem, sizeof(ItemAttrib));
	}

	UpdatePlayerAttrib();
#ifdef _NET_GAME_
// 	PkgPlayerChangeEquipReq req;
// 	if(_type == PLAYER_ITEM_WEAPON)
// 	{
// 		req.uUserId = GetHandlerID();
// 		memcpy(&req.stItem, GetPlayerItem(PLAYER_ITEM_WEAPON), sizeof(ItemAttrib));
// 		g_xBuffer.Reset();
// 		g_xBuffer << req;
// 		SendBuffer(&g_xBuffer);
// 	}
// 	else if(_type == PLAYER_ITEM_CLOTH)
// 	{
// 		req.uUserId = GetHandlerID();
// 		memcpy(&req.stItem, GetPlayerItem(PLAYER_ITEM_CLOTH), sizeof(ItemAttrib));
// 		g_xBuffer.Reset();
// 		g_xBuffer << req;
// 		SendBuffer(&g_xBuffer);
// 	}
#endif
	return true;

lbl_showerr:
	if(_err)
	{
		GameInfoBoardDlg::GetInstance()->InsertEquipErrorMsg(*_err);
	}
	return false;
}

bool GamePlayer::DressEquip(int _bagidx, DWORD* _err /* = NULL */)
{
	ItemAttrib& bagItem = GetPlayerBag()->GetItemList()[_bagidx];
	if(bagItem.type == ITEM_NO ||
		bagItem.id == -1)
	{
		//	背包物品不存在
		if(_err)
		{
			*_err = REQ_ERR_NOTHING;
		}
		goto lbl_showerr;
	}

	if(!CanEquip(&bagItem, _err))
	{
		//	不满足要求
		goto lbl_showerr;
	}

	PLAYER_ITEM_TYPE itemtype = Item2PlayerItem(&bagItem);
	if(itemtype == PLAYER_ITEM_TOTAL)
	{
		return false;
	}
	else if(itemtype == PLAYER_ITEM_BRACELAT1)
	{
		//	检测手镯1位置是否为空
		if(m_equip[PLAYER_ITEM_BRACELAT2].type == ITEM_NO &&
			m_equip[PLAYER_ITEM_BRACELAT1].type != ITEM_NO)
		{
			itemtype = PLAYER_ITEM_BRACELAT2;
		}
	}
	else if(itemtype == PLAYER_ITEM_RING1)
	{
		if(m_equip[PLAYER_ITEM_RING2].type == ITEM_NO &&
			m_equip[PLAYER_ITEM_RING1].type != ITEM_NO)
		{
			itemtype = PLAYER_ITEM_RING2;
		}
	}

	ItemAttrib& dressItem = m_equip[itemtype];
	//	穿戴装备
	if(dressItem.type != ITEM_NO)
	{
		//	身上装备不为空 需要换装
		ItemAttrib item;
		memcpy(&item, &dressItem, sizeof(ItemAttrib));
		memcpy(&dressItem, &bagItem, sizeof(ItemAttrib));
		ZeroMemory(&bagItem, sizeof(ItemAttrib));
		m_bag.AddItem(&item);
		GameInfoBoardDlg::GetInstance()->ShowItemMsg(&item);
	}
	else
	{
		//	直接穿上
		memcpy(&dressItem, &bagItem, sizeof(ItemAttrib));
		ZeroMemory(&bagItem, sizeof(ItemAttrib));
	}

	UpdatePlayerAttrib();
// #ifdef _NET_GAME_
// 	PkgPlayerChangeEquipReq req;
// 	if(itemtype == PLAYER_ITEM_WEAPON)
// 	{
// 		req.uUserId = GetHandlerID();
// 		memcpy(&req.stItem, GetPlayerItem(PLAYER_ITEM_WEAPON), sizeof(ItemAttrib));
// 		g_xBuffer.Reset();
// 		g_xBuffer << req;
// 		SendBuffer(&g_xBuffer);
// 	}
// 	else if(itemtype == PLAYER_ITEM_CLOTH)
// 	{
// 		req.uUserId = GetHandlerID();
// 		memcpy(&req.stItem, GetPlayerItem(PLAYER_ITEM_CLOTH), sizeof(ItemAttrib));
// 		g_xBuffer.Reset();
// 		g_xBuffer << req;
// 		SendBuffer(&g_xBuffer);
// 	}
// #endif
	return true;

lbl_showerr:
	if(_err)
	{
		GameInfoBoardDlg::GetInstance()->InsertEquipErrorMsg(*_err);
	}
	return false;
}

PLAYER_ITEM_TYPE GamePlayer::Item2PlayerItem(ItemAttrib* _item)
{
	BYTE bType = _item->type;

	switch (bType)
	{
	case ITEM_WEAPON:
		{
			return PLAYER_ITEM_WEAPON;
		}break;
	case ITEM_CLOTH:
		{
			return PLAYER_ITEM_CLOTH;
		}break;
	case ITEM_NECKLACE:
		{
			return PLAYER_ITEM_NECKLACE;
		}break;
	case ITEM_HELMET:
		{
			return PLAYER_ITEM_HELMET;
		}break;
	case ITEM_MEDAL:
		{
			return PLAYER_ITEM_MEDAL;
		}break;
	case ITEM_RING:
		{
			return PLAYER_ITEM_RING1;
		}break;
	case ITEM_BRACELAT:
		{
			return PLAYER_ITEM_BRACELAT1;
		}break;
	}

	return PLAYER_ITEM_TOTAL;
}

BYTE GamePlayer::PlayerItem2Item(PLAYER_ITEM_TYPE _type)
{
	switch (_type)
	{
	case PLAYER_ITEM_WEAPON:
		{
			return ITEM_WEAPON;
		}break;
	case PLAYER_ITEM_CLOTH:
		{
			return ITEM_CLOTH;
		}break;
	case PLAYER_ITEM_HELMET:
		{
			return ITEM_HELMET;
		}break;
	case PLAYER_ITEM_MEDAL:
		{
			return ITEM_MEDAL;
		}break;
	case PLAYER_ITEM_NECKLACE:
		{
			return ITEM_NECKLACE;
		}break;
	case PLAYER_ITEM_RING1:
	case PLAYER_ITEM_RING2:
		{
			return ITEM_RING;
		}break;
	case PLAYER_ITEM_BRACELAT1:
	case PLAYER_ITEM_BRACELAT2:
		{
			return ITEM_BRACELAT;
		}break;
	}

	return ITEM_NO;
}

bool GamePlayer::UnDressEquip(PLAYER_ITEM_TYPE _type)
{
	if(m_equip[_type].type == ITEM_NO)
	{
		return false;
	}

	bool bPut = GetPlayerBag()->AddItem(&m_equip[_type]);
	if(!bPut)
	{
		return false;
	}
	else
	{
		GameInfoBoardDlg::GetInstance()->ShowItemMsg(&m_equip[_type]);
		ZeroMemory(&m_equip[_type], sizeof(ItemAttrib));
		UpdatePlayerAttrib();
	}

	return true;
}

void GamePlayer::UpdateAccelerate(int _idx /* = -1 */)
{
	ItemList& assistItems = m_bag.GetAssistItemList();
	ItemList& bagItems = m_bag.GetItemList();

	if(_idx < 0 ||
		_idx >= HERO_CLIENT_ASSIST_SIZE)
	{
		return;
	}

	if(-1 == _idx)
	{
		for(int i = 0; i < HERO_CLIENT_ASSIST_SIZE; ++i)
		{
			if(assistItems[i].type == ITEM_COST)
			{
				int nAttribID = assistItems[i].id;
				int nCounter = 0;

				for(int j = 0; j < HERO_BAG_SIZE_CUR; ++j)
				{
					if(bagItems[j].id == nAttribID)
					{
						nCounter += bagItems[j].atkSpeed;
					}
				}

				assistItems[i].atkSpeed = nCounter;

				if(nCounter == 0)
				{
					ZeroMemory(&assistItems[i], sizeof(ItemAttrib));
				}
			}
		}
	}
	else
	{
		if(assistItems[_idx].type == ITEM_COST)
		{
			int nAttribID = assistItems[_idx].id;
			int nCounter = 0;

			for(int i = 0; i < HERO_BAG_SIZE_CUR; ++i)
			{
				if(bagItems[i].id == nAttribID)
				{
					nCounter += bagItems[i].atkSpeed;
				}
			}

			assistItems[_idx].atkSpeed = nCounter;

			if(nCounter == 0)
			{
				ZeroMemory(&assistItems[_idx], sizeof(ItemAttrib));
			}
		}
	}
}

bool GamePlayer::MoveBagItemToAssistItem(int _bagidx, int _astidx)
{
	if(_bagidx < 0 ||
		_bagidx >= HERO_MAINBAG_SIZE_CUR ||
		_astidx < 0 ||
		_astidx >= HERO_CLIENT_ASSIST_SIZE)
	{
		return false;
	}

	ItemAttrib& bagitem = m_bag.GetItemList()[_bagidx];
	if(!IsCostItem(bagitem))
	{
		return false;
	}

	ItemAttrib& astitem = m_bag.GetAssistItemList()[_astidx];
	/*if(astitem.type != ITEM_NO)
	{
		//	背包有东西
		ItemAttrib item;
		memcpy(&item, &astitem, sizeof(ItemAttrib));
		memcpy(&astitem, &bagitem, sizeof(ItemAttrib));
		memcpy(&bagitem, &item, sizeof(ItemAttrib));
	}
	else
	{
		//	背包没东西
		memcpy(&astitem, &bagitem, sizeof(ItemAttrib));
		ZeroMemory(&bagitem, sizeof(ItemAttrib));
	}*/
	memcpy(&astitem, &bagitem, sizeof(ItemAttrib));
	UpdateAccelerate(_astidx);

	//PkgPlayerSyncAssistReq req;
	//req.bNum = GetAssistItemsEmptySum();
	//req.uUserId = GetHandlerID();
	//g_xBuffer << req;
	//SendBuffer(&g_xBuffer);

	return true;
}

bool GamePlayer::IsCostItem(const ItemAttrib& _item)
{
	/*if(_item.type == ITEM_YAO_SPE ||
		_item.type == ITEM_YAP ||
		_item.type == ITEM_SCROLL)*/
	if(_item.type == ITEM_COST)
	{
		return true;
	}

	return false;
}

bool GamePlayer::IsEquipItem(const ItemAttrib& _item)
{
	if(_item.type == ITEM_WEAPON ||
		_item.type == ITEM_CLOTH ||
		_item.type == ITEM_NECKLACE ||
		_item.type == ITEM_RING ||
		_item.type == ITEM_BRACELAT ||
		_item.type == ITEM_HELMET ||
		_item.type == ITEM_MEDAL ||
		_item.type == ITEM_SHOE ||
		_item.type == ITEM_BELT ||
		_item.type == ITEM_GEM ||
		_item.type == ITEM_CHARM)
	{
		return true;
	}

	return false;
}

bool GamePlayer::UseItemFromBag(int _bagidx)
{
	if(_bagidx < 0 ||
		_bagidx >= HERO_MAINBAG_SIZE_CUR)
	{
		return false;
	}

	ItemAttrib& item = m_bag.GetItemList()[_bagidx];
	if(!IsCostItem(item))
	{
		return false;
	}

	if(UseItem(&item))
	{
		ZeroMemory(&item, sizeof(ItemAttrib));
		return true;
	}
	return false;
}

bool GamePlayer::UseItemFromAssist(int _astidx)
{
	if(_astidx < 0 ||
		_astidx >= HERO_CLIENT_ASSIST_SIZE)
	{
		return false;
	}

	ItemAttrib& item = m_bag.GetAssistItemList()[_astidx];
	if(!IsCostItem(item))
	{
		return false;
	}

	int nUseID = item.id;
	int nUseTag = -1;

	for(int i = 0; i < HERO_BAG_SIZE_CUR; ++i)
	{
		if(m_bag.GetItemList()[i].id == nUseID)
		{
			nUseTag = m_bag.GetItemList()[i].tag;
			break;
		}
	}

#ifdef _NET_GAME_
	if(-1 != nUseTag)
	{
		if(m_xDrugCoolDown.CanUse(item.id, item.hide * 1000))
		{
			PkgPlayerUseItemReq req;
			req.dwTag = nUseTag;
			req.uUserId = GetHandlerID();
			g_xBuffer.Reset();
			g_xBuffer << req;
			SendBufferToGS(&g_xBuffer);
		}
	}
#else
	if(UseItem(&item))
	{
		//	同时找到包裹是否可以补进辅助栏的
		ZeroMemory(&item, sizeof(ItemAttrib));
		m_bag.MoveBagItemToAssistItem(nUseID);
		return true;
	}
#endif

	return false;
}

bool GamePlayer::UseItem(ItemAttrib* _item)
{
	/*switch(_item->type)
	{
	case ITEM_YAP:
		{
			//	普通药品
			AddHP(_item->HP, ADD_SPECL, _item->HP / 5);
			AddMP(_item->MP, ADD_SPECL, _item->MP / 5);
		}break;
	case ITEM_YAO_SPE:
		{
			//	特殊药品
			AddHP(_item->HP);
			AddMP(_item->MP);
		}break;*/
	/*case ITEM_BOOK:
		{
			//	书籍
		}break;
	case ITEM_SCROLL:
		{
			//	卷轴
			if(_item->id == ITID_RANDOMSCROLL)
			{
				//	随机卷
				RandomPos();
			}
		}break;
	default:
		{
			//
			return false;
		}
	}*/

	return true;
}
//////////////////////////////////////////////////////////////////////////
int GamePlayer::GetAssistItemsEmptySum()
{
	ItemList& xItems = m_bag.GetAssistItemList();
	int nCounter = 0;

	ItemList::const_iterator begiter = xItems.begin();
	ItemList::const_iterator enditer = xItems.end();
	for(begiter; begiter != enditer; ++begiter)
	{
		if(begiter->type == ITEM_NO)
		{
			++nCounter;
		}
	}
	return nCounter;
}

int GamePlayer::GetBagItemsEmptySum()
{
	ItemList& xItems = m_bag.GetItemList();
	int nCounter = 0;

	ItemList::const_iterator begiter = xItems.begin();
	ItemList::const_iterator enditer = xItems.end();
	for(begiter; begiter != enditer; ++begiter)
	{
		if(begiter->type == ITEM_NO)
		{
			++nCounter;
		}
	}
	return nCounter;
}

void GamePlayer::RandomPos()
{
	//	随机移动位置
	GameMapManager* pMap = GameMapManager::GetInstance();
	MapInfo info;
	pMap->GetMapData()->GetMapInfo(info);

	int nRandomX = AfxGetHge()->Random_Int(0, info.nCol);
	int nRandomY = AfxGetHge()->Random_Int(0, info.nRow);
	int nCounter = 0;
	bool bCanMove = true;

	while(!pMap->CanThrough(nRandomX, nRandomY))
	{
		nRandomX = AfxGetHge()->Random_Int(0, info.nCol);
		nRandomY = AfxGetHge()->Random_Int(0, info.nRow);
		++nCounter;
		if(nCounter > 999)
		{
			bCanMove = false;
			break;
		}
	}

	if(bCanMove)
	{
		//	随机移动
		SetReallyCoord(nRandomX, nRandomY);
	}
}

bool GamePlayer::AddHP(int _hp, int _way /* = ADD_NORMAL */, int _step /* = 10 */)
{
	if(_way == ADD_NORMAL)
	{
		m_attrib.HP += _hp;
		if(m_attrib.HP > m_attrib.maxHP)
		{
			m_attrib.HP = m_attrib.maxHP;
			return false;
		}
	}
	else
	{
		if(_hp > 0)
		{
			//m_statusControl.AddStatusItem(STATUS_HP, _hp, _step);
		}
	}
	return true;
}

bool GamePlayer::AddMP(int _mp, int _way /* = ADD_NORMAL */, int _step /* = 10 */)
{
	if(_way == ADD_NORMAL)
	{
		m_attrib.MP += _mp;
		if(m_attrib.MP > m_attrib.maxMP)
		{
			m_attrib.MP = m_attrib.maxMP;
			return false;
		}
	}
	else
	{
		if(_mp > 0)
		{
			//m_statusControl.AddStatusItem(STATUS_MP, _mp, _step);
		}
	}

	return true;
}

void GamePlayer::DecHP(int _hp)
{
	if(_hp >= m_attrib.HP)
	{
		m_attrib.HP = 0;
		SetStatus(PST_DEAD);
		SetCurFrame(0);
	}
	else
	{
		m_attrib.HP -= _hp;
	}
}

void GamePlayer::DecMP(int _mp)
{
	if(_mp >= m_attrib.MP)
	{
		m_attrib.MP = 0;
	}
	else
	{
		m_attrib.MP -= _mp;
	}
}

void GamePlayer::AddExp(int _exp)
{
	if(_exp + m_attrib.EXPR >= m_attrib.maxEXPR)
	{
		LevelUp();
	}
	else
	{
		m_attrib.EXPR += _exp;
	}
}

void GamePlayer::LevelUp()
{

}

/************************************************************************/
/* 保存
/************************************************************************/
bool GamePlayer::Save()
{
	char szFile[MAX_PATH];
#ifdef _DEBUG
	sprintf(szFile, "%s\\Save\\Debug\\%s.sav", GetRootPath(), m_attrib.name);
#else
	sprintf(szFile, "%s\\Save\\%s.sav", GetRootPath(), m_attrib.name);
#endif
	HANDLE hFile = ::CreateFile(szFile, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, NULL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		AfxGetHge()->System_Log("保存人物数据失败,打开文件[%s]失败", szFile);
		return false;
	}

	bool bRet = true;
	::SetFilePointer(hFile, 0, 0, FILE_BEGIN);
	DWORD dwWrite = 0;
	//	20字节的人物名字
	do 
	{
		if(!::WriteFile(hFile, m_attrib.name, 20, &dwWrite, NULL))
		{
			//
			bRet = false;
		}
	} while (false);

	return bRet;
}
//////////////////////////////////////////////////////////////////////////
int GamePlayer::GetMagicCount()
{
	int nCount = 0;

	for(int i = 0; i < USER_MAGIC_NUM; ++i)
	{
		if(m_stMagicDetail[i].wID != MEFF_NONE)
		{
			++nCount;
		}
		else
		{
			return nCount;
		}
	}

	return nCount;
}
//////////////////////////////////////////////////////////////////////////
bool GamePlayer::AddToMagicTree(DWORD _dwMgcID)
{
	if(_dwMgcID >= MEFF_TOTAL)
	{
		return false;
	}
	for(int i = 0; i < USER_MAGIC_NUM; ++i)
	{
		if(m_stMagicDetail[i].wID == MEFF_NONE)
		{
			m_stMagicDetail[i].wID = (WORD)_dwMgcID;
			m_stMagicDetail[i].szName = g_szMagicName[_dwMgcID];
			return true;
		}
	}
	return false;
}
//////////////////////////////////////////////////////////////////////////
bool GamePlayer::IsMagicExist(DWORD _dwMgcID)
{
	if(_dwMgcID >= MEFF_TOTAL)
	{
		return false;
	}
	for(int i = 0; i < USER_MAGIC_NUM; ++i)
	{
		if(m_stMagicDetail[i].wID == _dwMgcID)
		{
			return true;
		}
	}
	return false;
}
//////////////////////////////////////////////////////////////////////////
bool GamePlayer::UpgradeMagicLevel(DWORD _dwMgcID, char _c /* = 1 */)
{
	for(int i = 0; i < USER_MAGIC_NUM; ++i)
	{
		if(m_stMagicDetail[i].wID == _dwMgcID)
		{
			m_stMagicDetail->cLevel += _c;
			return true;
		}
	}
	return false;
}
//////////////////////////////////////////////////////////////////////////
bool GamePlayer::SetMagicLevel(DWORD _dwMgcID, char _level)
{
	for(int i = 0; i < USER_MAGIC_NUM; ++i)
	{
		if(m_stMagicDetail[i].wID == _dwMgcID)
		{
			m_stMagicDetail[i].cLevel = _level;
			return true;
		}
	}
	return false;
}
//////////////////////////////////////////////////////////////////////////
bool GamePlayer::SetMagicKey(DWORD _dwMgcID, char _cKey)
{
	if(_cKey == 0)
	{
		for(int i = 0; i < USER_MAGIC_NUM; ++i)
		{
			if(m_stMagicDetail[i].wID == _dwMgcID)
			{
				if(m_stMagicDetail[i].cKey != 0)
				{
					std::map<char, MagicDetail*>::iterator fnditer = m_xInputMap.find(m_stMagicDetail[i].cKey);
					if(fnditer != m_xInputMap.end())
					{
						m_xInputMap.erase(fnditer);
					}
				}
				m_stMagicDetail[i].cKey = 0;
				return true;
			}
		}
		return false;
	}

	std::map<char, MagicDetail*>::iterator iter = m_xInputMap.find(_cKey);
	if(iter != m_xInputMap.end())
	{
		// already has key
		iter->second->cKey = 0;
		m_xInputMap.erase(iter);

		for(int i = 0; i < USER_MAGIC_NUM; ++i)
		{
			if(m_stMagicDetail[i].wID == _dwMgcID)
			{
				m_stMagicDetail[i].cKey = _cKey;
				m_xInputMap.insert(std::make_pair(_cKey, &m_stMagicDetail[i]));
				return true;
			}
		}
	}
	else
	{
		for(int i = 0; i < USER_MAGIC_NUM; ++i)
		{
			if(m_stMagicDetail[i].wID == _dwMgcID)
			{
				m_stMagicDetail[i].cKey = _cKey;
				m_xInputMap.insert(std::make_pair(_cKey, &m_stMagicDetail[i]));
				return true;
			}
		}
	}

	return false;
}
//////////////////////////////////////////////////////////////////////////
const MagicDetail* GamePlayer::GetMagicByKey(char _cKey)
{
	std::map<char, MagicDetail*>::iterator iter = m_xInputMap.find(_cKey);
	if(iter != m_xInputMap.end())
	{
		return iter->second;
	}
	return NULL;
}
//////////////////////////////////////////////////////////////////////////
bool GamePlayer::SetMagicAccKey(DWORD _dwMgcID, char _cKey)
{
	if(_cKey == 0)
	{
		for(int i = 0; i < USER_MAGIC_NUM; ++i)
		{
			if(m_stMagicDetail[i].wID == _dwMgcID)
			{
				if(m_stMagicDetail[i].cAccKey != 0)
				{
					std::map<char, MagicDetail*>::iterator fnditer = m_xAccInputMap.find(m_stMagicDetail[i].cAccKey);
					if(fnditer != m_xAccInputMap.end())
					{
						m_xAccInputMap.erase(fnditer);
					}
				}
				m_stMagicDetail[i].cAccKey = 0;
				return true;
			}
		}
		return false;
	}

	std::map<char, MagicDetail*>::iterator iter = m_xAccInputMap.find(_cKey);
	if(iter != m_xAccInputMap.end())
	{
		// already has key
		iter->second->cAccKey = 0;
		m_xAccInputMap.erase(iter);

		for(int i = 0; i < USER_MAGIC_NUM; ++i)
		{
			if(m_stMagicDetail[i].wID == _dwMgcID)
			{
				m_stMagicDetail[i].cAccKey = _cKey;
				m_xAccInputMap.insert(std::make_pair(_cKey, &m_stMagicDetail[i]));
				return true;
			}
		}
	}
	else
	{
		for(int i = 0; i < USER_MAGIC_NUM; ++i)
		{
			if(m_stMagicDetail[i].wID == _dwMgcID)
			{
				m_stMagicDetail[i].cAccKey = _cKey;
				m_xAccInputMap.insert(std::make_pair(_cKey, &m_stMagicDetail[i]));
				return true;
			}
		}
	}

	return false;
}
//////////////////////////////////////////////////////////////////////////
const MagicDetail* GamePlayer::GetMagicByAccKey(char _cKey)
{
	std::map<char, MagicDetail*>::iterator iter = m_xAccInputMap.find(_cKey);
	if(iter != m_xAccInputMap.end())
	{
		return iter->second;
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
void GamePlayer::WriteAccMagicKeyCfg()
{
	g_xBuffer.Reset();

	std::map<char, MagicDetail*>::const_iterator begiter = m_xAccInputMap.begin();

	for(begiter; begiter != m_xAccInputMap.end();
		++begiter)
	{
		g_xBuffer << begiter->first << begiter->second->wID;
	}

	char szCfgFile[MAX_PATH];
	sprintf(szCfgFile, "%s\\Save\\%s.acc",
		GetRootPath(), GetAttrib()->name);

	HANDLE hCfgFile = ::CreateFile(szCfgFile, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, NULL, NULL);
	DWORD dwWrite = 0;
	if(hCfgFile != INVALID_HANDLE_VALUE)
	{
		WriteFile(hCfgFile, g_xBuffer.GetBuffer(), g_xBuffer.GetLength(), &dwWrite, NULL);
		if(dwWrite == g_xBuffer.GetLength())
		{
			AfxGetHge()->System_Log("人物快捷技能保存成功");
		}
	}
	else
	{
		AfxGetHge()->System_Log("无法保存人物快捷技能");
	}
	CloseHandle(hCfgFile);
}

void GamePlayer::LoadAccMagicKeyCfg()
{
	char szCfgFile[MAX_PATH];
	sprintf(szCfgFile, "%s\\Save\\%s.acc",
		GetRootPath(), GetAttrib()->name);

	HANDLE hCfgFile = ::CreateFile(szCfgFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
	DWORD dwRead = 0;
	DWORD dwFileSize = 0;
	if(hCfgFile != INVALID_HANDLE_VALUE)
	{
		dwFileSize = GetFileSize(hCfgFile, NULL);

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

						if(*pKey >= HGEK_A &&
							*pKey <= HGEK_Z)
						{
							SetMagicAccKey(*pMgc, *pKey);
						}
					}
				}
			}
		}
		CloseHandle(hCfgFile);
	}
}

int GamePlayer::GetSkillLevel(DWORD _dwMgcID)
{
	const MagicDetail* pDetail = GetMagicByID(_dwMgcID);
	if(NULL == pDetail)
	{
		return 0;
	}
	return pDetail->cLevel;
}

void GamePlayer::SetWaitServerResponse(int _nWaitType, int _nTimeout)
{
	if(0 == _nTimeout)
	{
		m_dwWaitServerRspTimeoutTime = 0;
		m_nWaitServerRspType = 0;
	}
	else
	{
		m_dwWaitServerRspTimeoutTime = GetTickCount() + _nTimeout;
		m_nWaitServerRspType = _nWaitType;
	}
}

bool GamePlayer::IsWaitServerResponse()
{
	if(0 == m_dwWaitServerRspTimeoutTime)
	{
		return false;
	}
	if(WAITSERVERRSP_NONE == m_nWaitServerRspType)
	{
		return false;
	}
	if(GetTickCount() > m_dwWaitServerRspTimeoutTime)
	{
		return false;
	}

	return true;
}

void GamePlayer::UpdateWaitServerResponse()
{
	if(m_bUsingPreLock)
	{
		return;
	}

	if(GetTickCount() > m_dwWaitServerRspTimeoutTime)
	{
		/*if(GetStatus() == PST_ATTACKWEAPON ||
			GetStatus() == PST_ATTACKNOWEAPON)
		{
			SetStatus(PST_ATTACKSTOP);
			SetCurFrame(0);
			m_fLastUpdateAttack = 0;
		}*/

		m_dwWaitServerRspTimeoutTime = 0;
		m_nWaitServerRspType = 0;
	}
}

void GamePlayer::UpdateEnableSkill(int _nSkillType, bool _bEnabled)
{
	if(_nSkillType == MEFF_LIEHUO)
	{
		m_bEnableLieHuo = _bEnabled;
	}
	else if(_nSkillType == MEFF_SLIEHUO)
	{
		m_bEnableSuperLieHuo = _bEnabled;
	}
	else if(_nSkillType == MEFF_CISHA)
	{
		m_bEnableCiSha = _bEnabled;
	}
	else if(_nSkillType == MEFF_BANYUE)
	{
		m_bEnableBanYue = _bEnabled;
	}
}

void GamePlayer::PreNormalAttackTarget(GameObject* _pTarget)
{
	//	预先判断模式
	m_dwPreCalcNormalAttackEffMask = 0;
	bool bProc = false;

	if(m_bEnableLieHuo)
	{
		SET_FLAG(m_dwHumEffectFlag, MMASK_LIEHUO);
		bProc = true;
	}
	else if(m_bEnableSuperLieHuo)
	{
		SET_FLAG(m_dwHumEffectFlag, MMASK_SLIEHUO);
		bProc = true;
	}
	else if(m_bEnableBanYue)
	{
		//	魔法值够的话 就半月的攻击效果
		const MagicDetail* pDetail = GetMagicByID(MEFF_BANYUE);

		int nCost = GetMagicCost(pDetail);
		if(m_attrib.MP >= nCost)
		{
			SET_FLAG(m_dwHumEffectFlag, MMASK_BANYEU);
			bProc = true;
		}
	}

	int nAtkRand = m_xNormalAtkRand.Rand(0, 99);
	int nGongShaCrashNumber = 20;
	int nKtSwordCrashNumber = 0;

	const MagicDetail* pGongShaMgc = GetMagicByID(MEFF_GONGSHA);
	const MagicDetail* pKtSwordMgc = GetMagicByID(MEFF_KTSWORD);
	bool bGongShaEff = false;
	bool bKtSwordEff = false;

	if(pGongShaMgc != NULL &&
		pGongShaMgc->cLevel > 0 &&
		NULL != _pTarget)
	{
		if(nAtkRand < nGongShaCrashNumber)
		{
			bGongShaEff = true;
		}
	}

	if(!bGongShaEff &&
		pKtSwordMgc != NULL &&
		pKtSwordMgc->cLevel > 0)
	{
		if(pKtSwordMgc->cLevel == 1)
		{
			nKtSwordCrashNumber = 8;
		}
		else if(pKtSwordMgc->cLevel == 2)
		{
			nKtSwordCrashNumber = 10;
		}
		else if(pKtSwordMgc->cLevel == 3)
		{
			nKtSwordCrashNumber = 12;
		}

		if(nAtkRand < nGongShaCrashNumber + nKtSwordCrashNumber)
		{
			bKtSwordEff = true;
		}
	}

	if(!bProc)
	{
		if(bGongShaEff)
		{
			SET_FLAG(m_dwHumEffectFlag, MMASK_GONGSHA);
			bProc = true;
		}
	}

	if(!bProc)
	{
		if(bKtSwordEff)
		{
			SET_FLAG(m_dwHumEffectFlag, MMASK_KTSWORD);
			bProc = true;
		}
	}

	if(!bProc)
	{
		if(m_bEnableCiSha)
		{
			SET_FLAG(m_dwHumEffectFlag, MMASK_CISHA);
			bProc = true;
		}
	}

	//	normal attack

	//	play normal attack animation and wait for unlocking
	SetCurFrame(0);
	SetStatus(PST_ATTACKNOWEAPON);
	m_dwLastAttackTime = GetTickCount();
	m_dwLastAttackMode = 0;

	//	send attack request
	PkgUserActionReq req;
	req.uAction = ACTION_ATTACK;
	req.uUserId = GetHandlerID();
	if(NULL != _pTarget)
	{
		req.uTargetId = _pTarget->GetHandlerID();
	}
	req.uParam0 = MAKELONG(GetCoordX(), GetCoordY());
	req.uParam1 = (unsigned int)(GetDirection());
	req.uParam2 = nAtkRand;
	g_xBuffer.Reset();
	g_xBuffer << req;
	SendBufferToGS(&g_xBuffer);

	//	lock for server response
	SetWaitServerResponse(WAITSERVERRSP_NORMALATK, 5000);
}

void GamePlayer::WriteBagItemOrder()
{
	char szPath[MAX_PATH] = {0};
	sprintf(szPath, "%s/save/%s.ord", GetRootPath(), m_attrib.name);

	g_xBuffer.Reset();

	ItemList& refItems = GetPlayerBag()->GetItemList();
	ItemList::const_iterator begIter = refItems.begin();
	for(begIter;
		begIter != refItems.end();
		++begIter)
	{
		const ItemAttrib& refItem = *begIter;
		if(refItem.type == ITEM_NO)
		{
			g_xBuffer << UINT(0);
		}
		else
		{
			g_xBuffer << refItem.id;
		}
	}

	HANDLE hCfgFile = ::CreateFile(szPath, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, NULL, NULL);
	DWORD dwWrite = 0;
	if(hCfgFile != INVALID_HANDLE_VALUE)
	{
		WriteFile(hCfgFile, g_xBuffer.GetBuffer(), g_xBuffer.GetLength(), &dwWrite, NULL);
	}

	CloseHandle(hCfgFile);
}

void GamePlayer::ReadBagItemOrder()
{
	char szPath[MAX_PATH] = {0};
	sprintf(szPath, "%s/save/%s.ord", GetRootPath(), m_attrib.name);

	if(!PathFileExists(szPath))
	{
		return;
	}

	HANDLE hCfgFile = ::CreateFile(szPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
	DWORD dwRead = 0;
	DWORD dwFileSize = 0;
	int nAssistID[6] = {0};
	UINT uBagItemIds[HERO_MAINBAG_SIZE_CUR] = {0};

	if(hCfgFile != INVALID_HANDLE_VALUE)
	{
		dwFileSize = GetFileSize(hCfgFile, NULL);
		ItemAttrib* pAssistItem = NULL;
		GamePlayer* pPlayer = GamePlayer::GetInstance();

		do 
		{
			if(HERO_MAINBAG_SIZE_CUR * sizeof(UINT) != pPlayer->GetPlayerBag()->GetItemList().size() * sizeof(UINT))
			{
				break;
			}
			ReadFile(hCfgFile, uBagItemIds, HERO_MAINBAG_SIZE_CUR * sizeof(UINT), &dwRead, NULL);
			if(dwRead != HERO_MAINBAG_SIZE_CUR * sizeof(UINT))
			{
				break;
			}

			//	change bag item order
			ItemList& refItems = pPlayer->GetPlayerBag()->GetItemList();
			for(int i = 0; i < sizeof(uBagItemIds) / sizeof(uBagItemIds[0]); ++i)
			{
				int nId = uBagItemIds[i];

				if(nId == refItems[i].id)
				{
					continue;
				}

				for(int j = i; j < sizeof(uBagItemIds) / sizeof(uBagItemIds[0]); ++j)
				{
					if(refItems[j].id == nId)
					{
						//	swap
						if(j == i)
						{
							//	already the same item
						}
						else
						{
							ItemAttrib item = refItems[j];
							refItems[j] = refItems[i];
							refItems[i] = item;
						}
						break;
					}
				}
			}
		} while (false);
		CloseHandle(hCfgFile);
		hCfgFile = NULL;
	}
}