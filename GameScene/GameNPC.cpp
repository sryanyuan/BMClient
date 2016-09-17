#include "GameNPC.h"
#include "GameInfoManager.h"
#include "GameResourceManager.h"
#include "GamePlayer.h"
#include "GameScene.h"
#include "../GameDialog/GameInteractiveDlg.h"
#include "../GameDialog/GameShopDlg2.h"
#include "../GameDialog/GameDonateDlg.h"
#include "../GameDialog/GameIdentifyDlg.h"
#include "../BackMir/BackMir.h"
//////////////////////////////////////////////////////////////////////////
GameNPC::GameNPC()
{
	SetType(OBJ_NPC);
	m_drt = PDT_RIGHTDOWN;
	m_stat = PST_STAND;
	m_fUpdateTime = 0.0f;
	m_pSpr = NULL;
	m_pRenderInfo = NULL;
}

GameNPC::~GameNPC()
{
	delete m_pSpr;
}

void GameNPC::Render()
{
	HGE* hge = AfxGetHge();
	GamePlayer* pPlayer = GamePlayer::GetInstance();
	GameTextureManager* ttex = GameResourceManager::GetInstance()->GetTexs(RES_NPC);

	if(NULL == m_pRenderInfo)
	{
		return;
	}

	if(ttex)
	{
		GameTextureManager* res = ttex;
		//	画怪物
		int nTexIndex = CalRenderIndex() + m_bCurFrame;
		HTEXTURE tex = res->GetTexture(nTexIndex);
		short sOffsetX = 0;
		short sOffsetY = 0;
		int nDrawOffsetX = -1;
		int nDrawOffsetY = -1;
		if(tex)
		{
			if(GameScene::sThis->GetTarget() == this)
			{
				HTEXTURE texSelected = SelectedTextureManager::GetInstance()->GetSelectedTextureMonster(tex, GetAttrib()->id, nTexIndex);
				if(texSelected != NULL)
				{
					tex = texSelected;
				}
			}

			if(!m_pSpr)
			{
				m_pSpr = new hgeSprite(tex, 0, 0, res->GetTextureWidth(nTexIndex),
					res->GetTextureHeight(nTexIndex));
			}
			else
			{
				m_pSpr->SetTexture(tex);
				m_pSpr->SetTextureRect(0, 0, res->GetTextureWidth(nTexIndex), 
					res->GetTextureHeight(nTexIndex));
			}
			sOffsetX = res->GetTextureOffsetX(nTexIndex);
			sOffsetY = res->GetTextureOffsetY(nTexIndex);
			//	根据玩家位置 得先算出来正常矩形的位置
			nDrawOffsetX = CENTRAL_X + (int)m_fPosx - (int)pPlayer->GetObjx();
			nDrawOffsetY = CENTRAL_Y + (int)m_fPosy - (int)pPlayer->GetObjy();
			m_pSpr->Render(nDrawOffsetX + sOffsetX,
				nDrawOffsetY + sOffsetY);
			//	记录当前纹理索引
			m_nCurrentTextureIndex = nTexIndex;
		}

		//	名字
		/*if(m_bRenderName)
		{
			AfxGetFont()->Print(nDrawOffsetX + strlen(m_attrib.name) / 4 * FONT_WIDTH_WORD,
				nDrawOffsetY -16, m_attrib.name);
			m_bRenderName = false;
		}*/
	}
}

void GameNPC::RenderName()
{
	if(!IsVisible())
	{
		return;
	}
	//	计算中值坐标
	int nDrawX = 0;
	int nDrawY = 0;
	GamePlayer* pPlayer = GamePlayer::GetInstance();

	nDrawX = CENTRAL_X + (int)m_fPosx - (int)pPlayer->GetObjx();
	nDrawY = CENTRAL_Y + (int)m_fPosy - (int)pPlayer->GetObjy();
	//int nTextWidth = 12 * strlen(m_attrib.name);
	int nTextWidth = GetTextWidth(m_attrib.name, 12);

	if(0 == nTextWidth)
	{
		return;
	}
	nDrawY -= UNIT_HEIGHT / 2;
	nDrawX += UNIT_WIDTH / 2;
	nDrawX -= nTextWidth / 2;

	AfxGetPrinter()->SetColor(ARGB(255, 84, 131, 241));
	AfxGetPrinter()->Print(nDrawX, nDrawY, m_attrib.name);
}

void GameNPC::Update(float _dt)
{
	m_fUpdateTime += _dt;
	//SetRenderName();

	if(NULL == m_pRenderInfo)
	{
		return;
	}
	switch(m_stat)
	{
	case PST_WALK:
		{
			if(!IsLastFrame())
			{
				//	增加帧
				if(m_fUpdateTime > 0.8f)
				{
					++m_bCurFrame;
					m_fUpdateTime = 0.0f;
				}
			}
			else
			{
				//	最后一帧 帧设为0
				if(m_fUpdateTime > 0.8f)
				{
					m_bCurFrame = 0;
					m_fUpdateTime = 0.0f;
				}
			}
		}break;
	case PST_STAND:
		{
			if(!IsLastFrame())
			{
				//	增加
				if(m_fUpdateTime > 0.8f)
				{
					++m_bCurFrame;
					m_fUpdateTime = 0.0f;
				}
			}
			else
			{
				//	最后一帧 帧设为0
				if(m_fUpdateTime > 0.8f)
				{
					m_bCurFrame = 0;
					m_fUpdateTime = 0.0f;
				}
			}
		}
	}
}

bool GameNPC::LoadProperty(int _id)
{
	bool bSuc = true;
	bSuc &= GameInfoManager::GetInstance()->GetMonsterAttrib(_id, &m_attrib);
	bSuc &= GameInfoManager::GetInstance()->GetMonsterRenderInfo(_id, &m_pRenderInfo);
	if(bSuc)
	{
		SetDirection((PLAYER_DIRECTION)m_pRenderInfo->drts);
	}
	return bSuc;
}

int GameNPC::CalRenderIndex()
{
	int nIndex = 0;
	int nDirIdx = m_drt - PDT_RIGHTDOWN;

	switch(m_stat)
	{
	case PST_WALK:
		{
			//	NPC的walk状态为动作
			nIndex =m_pRenderInfo->walkbegin/* + nDirIdx * m_renderInfo.walktotal*/;
		}break;
	case PST_STAND:
		{
			nIndex =m_pRenderInfo->standbegin/* + nDirIdx * m_renderInfo.standtotal*/;
		}break;
	}

	return nIndex;
}

bool GameNPC::IsLastFrame()
{
	switch(m_stat)
	{
	case PST_STAND:
		{
			return (m_pRenderInfo->standframe - 1) == m_bCurFrame;
		}break;
	case PST_WALK:
		{
			return (m_pRenderInfo->walkframe - 1) == m_bCurFrame;
		}break;
	}
	return false;
}

int GameNPC::OnLButtonDown()
{
	return 0;
}

HTEXTURE GameNPC::GetCurTexture()
{
	GameTextureManager* ttex = GameResourceManager::GetInstance()->GetTexs(RES_NPC);
	if(ttex)
	{
		HTEXTURE tex = ttex->GetTexture(CalRenderIndex());
		return tex;
	}

	return 0;
}

// bool GameNPC::IsSelected(const POINT& _pt)
// {
// 	GamePlayer* pPlayer = GamePlayer::GetInstance();
// 	int nDrawX = CENTRAL_X + (int)m_fPosx - (int)pPlayer->GetObjx();
// 	int nDrawY = CENTRAL_Y + (int)m_fPosy - (int)pPlayer->GetObjy();
// 	GameTextureManager* ttex = GameResourceManager::GetInstance()->GetTexs(RES_NPC);
// 
// 	if(!ttex)
// 	{
// 		return false;
// 	}
// 
// 	HTEXTURE tex = GetCurTexture();
// 	if(tex)
// 	{
// 		short sOffsetX = ttex->GetTextureOffsetXEx(CalRenderIndex());
// 		short sOffsetY = ttex->GetTextureOffsetYEx(CalRenderIndex());
// 		short sWidth = ttex->GetTextureWidthEx(CalRenderIndex());
// 		short sHeight = ttex->GetTextureHeightEx(CalRenderIndex());
// 
// 		//	缩小4像素 生成检测矩形
// 		RECT rcMons;
// 		rcMons.left = nDrawX + sOffsetX + 2;
// 		rcMons.right = rcMons.left + sWidth - 2;
// 		rcMons.top = nDrawY + sOffsetY + 2;
// 		rcMons.bottom = rcMons.top + sHeight - 2;
// 		if(!::PtInRect(&rcMons, _pt))
// 		{
// 			return false;
// 		}
// 
// 		if(rcMons.left > rcMons.right ||
// 			rcMons.top > rcMons.bottom)
// 		{
// 			return false;
// 		}
// 
// 		//	进行像素检测
// 		//	还原相对矩形
// 		POINT ptDetect = _pt;
// 		ptDetect.x = _pt.x - (nDrawX + sOffsetX);
// 		ptDetect.y = _pt.y - (nDrawY + sOffsetY);
// 		if(ptDetect.x < 0 ||
// 			ptDetect.y < 0)
// 		{
// 			return false;
// 		}
// 		DWORD dwActlWidth = AfxGetHge()->Texture_GetWidth(tex);
// 		DWORD dwActlHeight = AfxGetHge()->Texture_GetHeight(tex);
// 		DWORD* pColValue = AfxGetHge()->Texture_Lock(tex);
// 
// 		if((pColValue[ptDetect.y * dwActlWidth + ptDetect.x] & 0xFF000000) != 0 &&
// 			(pColValue[ptDetect.y * dwActlWidth + ptDetect.x + 1] & 0xFF000000) != 0 &&
// 			(pColValue[ptDetect.y * dwActlWidth + ptDetect.x - 1] & 0xFF000000) != 0 &&
// 			(pColValue[(ptDetect.y + 1) * dwActlWidth + ptDetect.x] & 0xFF000000) != 0 &&
// 			(pColValue[(ptDetect.y - 1) * dwActlWidth + ptDetect.x] & 0xFF000000) != 0)
// 		{
// 			AfxGetHge()->Texture_Unlock(tex);
// 			return true;
// 		}
// 		AfxGetHge()->Texture_Unlock(tex);
// 	}
// 
// 	return false;
// }





//////////////////////////////////////////////////////////////////////////
//	数据包
void GameNPC::OnPacket(const PacketHeader *_pPkt)
{
	g_xBuffer.Reset();
	unsigned int uWriteLen = g_xBuffer.Write(_pPkt, _pPkt->uLen);
	if(uWriteLen != _pPkt->uLen)
	{
		ALERT_MSGBOX("写入全局Buffer失败");
		::PostQuitMessage(0);
		return;
	}

	BEGIN_HANDLE_PACKET(_pPkt)
		HANDLE_PACKET(PKG_GAME_OBJECT_ACTION_NOT,			PkgObjectActionNot)
		HANDLE_PACKET(PKG_GAME_PLAYER_CLICKNPC_ACK,			PkgPlayerClickNPCAck)
		HANDLE_PACKET(PKG_GAME_PLAYER_SHOWSHOP_ACK,			PkgPlayerShowShopAck)
		HANDLE_PACKET(PKG_GAME_PLAYER_INTERACTIVEDIALOGCONTENT_NOT, PkgPlayerInteractiveDialogContentNot)
		HANDLE_DEFAULT(_pPkt)
	END_HANDLE_PACKET(_pPkt)
}

void GameNPC::DoPacket(const PkgObjectActionNot& not)
{
	if(not.uAction == ACTION_APPEAR ||
		not.uAction == ACTION_GROUND)
	{
		if(not.uAction == ACTION_APPEAR)
		{
			SetStatus(PST_APPEAR);
			//m_pObject->GetAttrib()->curse = 1;
			SetCurFrame(0);
			SetVisible(true);
		}
		else
		{
			SetStatus(PST_GROUND);
			SetCurFrame(0);
		}
	}
	else
	{
		SetVisible(true);
		__super::DoPacket(not);
	}
}
//////////////////////////////////////////////////////////////////////////
void GameNPC::DoPacket(const PkgPlayerClickNPCAck& not)
{
	int nQuestID = LOWORD(not.dwParam);
	int nStepID = HIWORD(not.dwParam);
	int nNPCID = (int)not.dwNPCID;

	//if(nQuestID == 0)
	//{
		//	SellDlg or Other dlg
	//}
	//else
	{
		//pTheGame->GetGameScene()->GetMainOpt()->GetActiveDlg()->Create(nNPCID, nQuestID, nStepID);
		GameScene::sThis->GetMainOpt()->GetActiveDlg()->Create(this, nQuestID, nStepID);
	}
}
//////////////////////////////////////////////////////////////////////////
void GameNPC::DoPacket(const PkgPlayerShowShopAck& ack)
{
	//GameShopDlg* pShopDlg = GameScene::sThis->GetMainOpt()->GetShopDlg();
	GameShopDlg2* pShopDlg = GameScene::sThis->GetMainOpt()->GetShopDlg2();

	if(ack.bType == SHOP_SELLSHOP)
	{
		pShopDlg->ClearItem();
		std::list<USHORT>::const_iterator begiter = ack.xItems.begin();
		std::list<USHORT>::const_iterator enditer = ack.xItems.end();
		for(begiter; begiter != enditer; ++begiter)
		{
			pShopDlg->AddItem(*begiter);
		}
		GameScene::sThis->GetMainOpt()->GetActiveDlg()->SetVisible(false);
		pShopDlg->CreateByID(this);

		//	Show bag dialog and move to top right corner
		int nPosX = WINDOW_WIDTH - GameScene::sThis->GetMainOpt()->GetBagDlg2()->GetWidth();
		int nPosY = 0;
		GameScene::sThis->GetMainOpt()->GetBagDlg2()->SetVisible(true);
		GameScene::sThis->GetMainOpt()->GetBagDlg2()->MoveTo(nPosX, nPosY);
	}
	else if(ack.bType == SHOP_DONATE)
	{
		GameScene::sThis->GetMainOpt()->GetDonateDlg()->CreateDonateDlg(2);
	}
	else if(ack.bType == SHOP_IDENTIFY ||
		ack.bType == SHOP_IDENTIFY_LOW ||
		ack.bType == SHOP_UNBIND)
	{
		GameIdentifyDlg* pDlg = GameScene::sThis->GetMainOpt()->GetIdentifyDlg();

		if(ack.bType == SHOP_IDENTIFY)
		{
			pDlg->SetDlgMode(GameIdentifyDlgMode_Identify);
		}
		else
		{
			pDlg->SetDlgMode(GameIdentifyDlgMode_Unbind);
		}

		pDlg->ShowDialog();
	}
}

void GameNPC::DoPacket(const PkgPlayerInteractiveDialogContentNot& not)
{
	int nNPCID = (int)not.nNpcId;
	GameScene::sThis->GetMainOpt()->GetActiveDlg()->Create(this, not.xItems);
}


//////////////////////////////////////////////////////////////////////////
MagicDoorNPC::MagicDoorNPC()
{

}

void MagicDoorNPC::Update(float _dt)
{
	m_fUpdateTime += _dt;
	//SetRenderName();

	if(NULL == m_pRenderInfo)
	{
		return;
	}
	switch(m_stat)
	{
	case PST_WALK:
		{
			if(!IsLastFrame())
			{
				//	增加帧
				if(m_fUpdateTime > 0.1f)
				{
					++m_bCurFrame;
					m_fUpdateTime = 0.0f;
				}
			}
			else
			{
				//	最后一帧 帧设为0
				if(m_fUpdateTime > 0.1f)
				{
					m_bCurFrame = 0;
					m_fUpdateTime = 0.0f;
				}
			}
		}break;
	case PST_STAND:
		{
			if(!IsLastFrame())
			{
				//	增加
				if(m_fUpdateTime > 0.1f)
				{
					++m_bCurFrame;
					m_fUpdateTime = 0.0f;
				}
			}
			else
			{
				//	最后一帧 帧设为0
				if(m_fUpdateTime > 0.1f)
				{
					m_bCurFrame = 0;
					m_fUpdateTime = 0.0f;
				}
			}
		}
	}
}

void MagicDoorNPC::Render()
{
	HGE* hge = AfxGetHge();
	GamePlayer* pPlayer = GamePlayer::GetInstance();
	GameTextureManager* ttex = GameResourceManager::GetInstance()->GetTexs(RES_NPC);

	if(NULL == m_pRenderInfo)
	{
		return;
	}

	if(ttex)
	{
		GameTextureManager* res = ttex;
		//	画怪物
		HTEXTURE tex = res->GetTexture(CalRenderIndex() + m_bCurFrame);
		short sOffsetX = 0;
		short sOffsetY = 0;
		int nDrawOffsetX = -1;
		int nDrawOffsetY = -1;
		if(tex)
		{
			//if(!m_pSpr)
			//{
				//m_pSpr = new hgeSprite(tex, 0, 0, res->GetTextureWidth(CalRenderIndex() + m_bCurFrame),
					//res->GetTextureHeight(CalRenderIndex() + m_bCurFrame));
			//}
			//else
			{
				MagicElement::pEffectRender->SetTexture(tex);
				MagicElement::pEffectRender->SetTextureRect(0, 0, res->GetTextureWidth(CalRenderIndex() + m_bCurFrame), 
					res->GetTextureHeight(CalRenderIndex() + m_bCurFrame));
			}
			sOffsetX = res->GetTextureOffsetX(CalRenderIndex() + m_bCurFrame);
			sOffsetY = res->GetTextureOffsetY(CalRenderIndex() + m_bCurFrame);
			//	根据玩家位置 得先算出来正常矩形的位置
			nDrawOffsetX = CENTRAL_X + (int)m_fPosx - (int)pPlayer->GetObjx();
			nDrawOffsetY = CENTRAL_Y + (int)m_fPosy - (int)pPlayer->GetObjy();
			MagicElement::pEffectRender->Render(nDrawOffsetX + sOffsetX,
				nDrawOffsetY + sOffsetY);
			//	记录当前纹理索引
			m_nCurrentTextureIndex = CalRenderIndex() + m_bCurFrame;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
GameNPCAltar::GameNPCAltar()
{
	m_nAlphaFrame = 0;
	m_dwLastSwitchAlphaTime = GetTickCount();
}

void GameNPCAltar::Update(float _dt)
{
	__super::Update(_dt);

	if(GetTickCount() - m_dwLastSwitchAlphaTime > 150)
	{
		m_dwLastSwitchAlphaTime = GetTickCount();

		++m_nAlphaFrame;
		if(m_nAlphaFrame >= 10)
		{
			m_nAlphaFrame = 0;
		}
	}
}

void GameNPCAltar::Render()
{
	__super::Render();

	if(NULL == m_pRenderInfo)
	{
		return;
	}

	GameTextureManager* ttex = GameResourceManager::GetInstance()->GetTexs(RES_NPC);
	int nTextIndex = m_pRenderInfo->standbegin + 30 + m_nAlphaFrame;
	HTEXTURE tex = ttex->GetTexture(nTextIndex);
	short sOffsetX = 0;
	short sOffsetY = 0;
	int nDrawOffsetX = -1;
	int nDrawOffsetY = -1;
	if(tex)
	{
		MagicElement::pEffectRender->SetTexture(tex);
		MagicElement::pEffectRender->SetTextureRect(0, 0, ttex->GetTextureWidth(nTextIndex), 
			ttex->GetTextureHeight(nTextIndex));
		sOffsetX = ttex->GetTextureOffsetX(nTextIndex);
		sOffsetY = ttex->GetTextureOffsetY(nTextIndex);
		//	根据玩家位置 得先算出来正常矩形的位置
		nDrawOffsetX = GetScreenPosX();
		nDrawOffsetY = GetScreenPosY();
		MagicElement::pEffectRender->Render(nDrawOffsetX + sOffsetX,
			nDrawOffsetY + sOffsetY);
	}
}