#include "../Common/GameRenderObject.h"
#include "../GameScene/GamePlayer.h"
#include "../GameScene/GameResourceManager.h"
#include "../GameScene/GameMonster.h"
#include "../GameDialog/GameInfoBoardDlg.h"
#include "../GameScene/GameScene.h"
#include "../BackMir/BackMir.h"
#include "../../CommonModule/SimpleActionHelper.h"
/*
 *	GameRenderObject 抽象了玩家 其它玩家 怪物 以及各种窗口绘图对象
 */
//////////////////////////////////////////////////////////////////////////
//	Global varible
const int g_nMoveOftTable[] = 
{
	0,-1,	1,-1,	1,0,
	1,1,	0,1,	-1,1,
	-1,0,	-1,-1
};

const DWORD g_dwMonsLvColor[8] =
{
	//	0
	ARGB_WHITE,
	//	1
	ARGB_WHITE,
	//	2
	ARGB(255, 205, 231, 250),
	//	3
	ARGB(255, 170, 214, 247),
	//	4
	ARGB(255, 145, 202, 244),
	//	5
	ARGB(255, 147, 141, 248),
	//	6
	ARGB(255, 113, 105, 245),
	//	7
	ARGB(255, 52, 43, 243)
};

const DWORD g_dwItemColor[9] =
{
	//	0
	ARGB(255,255,255,255),
	//	1
	//ARGB(255,191,169,162),
	0xFFC0FF3E,
	//	2
	//ARGB(255,191,169,162),
	0xFF76EE00,
	//	3
	//ARGB(255,211,145,141),
	0xFF97FFFF,
	//	4
	//ARGB(255,211,145,141),
	0xFF436EEE,
	//	5
	//ARGB(255,245,121,107),
	0xFFCD0000,
	//	6
	//ARGB(255,245,121,107),
	0xFF3A5FCD,
	//	7
	//ARGB(255,247,236,106)
	0xFFFFFF00,
	//	8
	0xFFFF00FF,
};

/************************************************************************/
/* 处理动作数据包
/************************************************************************/
/*
void GameObject::UpdateNetPacket()
{
	if(m_lstActPacket.empty())
	{
		//	没有数据包
		return;
	}

	//	处理数据包
	ActionPacket* pPacket = NULL;
	for(ACTIONLIST::iterator iter = m_lstActPacket.begin();
		iter != m_lstActPacket.end(); )
	{
		pPacket = *iter;
		if(pPacket != NULL)
		{
			switch(pPacket->wCmd)
			{
			case PC_ACTION_WALK:
				{
					OnWalk(pPacket);
				}break;
			case PC_ACTION_RUN:
				{
					OnRun(pPacket);
				}break;
			case PC_ACTION_TURN:
				{
					OnTurn(pPacket);
				}break;
			case PC_ACTION_ATTACK:
				{
					OnAttack(pPacket);
				}break;
			default:
				{
					AfxGetHge()->System_Log("未知的动作包[%d]",
						pPacket->wCmd);
				}break;
			}
		}

		SAFE_DELETE(pPacket);
		iter = m_lstActPacket.erase(iter);
	}
}*/

/************************************************************************/
/* 改变状态
/************************************************************************/
/*
void GameObject::SetObjectAction(PLAYER_STATUS _ps, PLAYER_DIRECTION _pd, DWORD _dwParam0, DWORD _dwParam1, DWORD _dwParam2)
{
	switch(_ps)
	{
	case PST_STAND:
		{

		}break;
	default:
		{
			AfxGetHge()->System_Log("未能处理的动作类型[%d]", (DWORD)_ps);
		}break;
	}
}*/

/************************************************************************/
/* 绘制名字
/************************************************************************/
void GameObject::RenderName()
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

	bool bRenderMaster = false;
	if(m_szMasterName[0] != 0)
	{
		bRenderMaster = true;
		nTextWidth += GetTextWidth(m_szMasterName, 12) + 12;
	}

	if(0 == nTextWidth)
	{
		return;
	}
	nDrawY -= UNIT_HEIGHT / 2;
	nDrawX += UNIT_WIDTH / 2;
	nDrawX -= nTextWidth / 2;

	char szBuf[40];
	if(bRenderMaster)
	{
		sprintf(szBuf, "%s(%s)",
			m_attrib.name, m_szMasterName);
	}
	

	//AfxGetFont()->Print(nDrawX, nDrawY, m_attrib.name);
	if((DWORD)this == (DWORD)GamePlayer::GetInstance())
	{
		DWORD dwPreColor = AfxGetPrinter()->GetColor();
		AfxGetPrinter()->SetColor(TEXT_ARGB_GREEN);
		AfxGetPrinter()->Print((float)nDrawX, (float)nDrawY, m_attrib.name);
		//AfxGetFont()->Print((float)nDrawX, (float)nDrawY, m_attrib.name);
		AfxGetPrinter()->SetColor(dwPreColor);
	}
	else
	{
		if(bRenderMaster)
		{
			AfxGetPrinter()->SetColor(TEXT_ARGB_WHITE);
			AfxGetPrinter()->Print(nDrawX, nDrawY, szBuf);
		}
		else
		{
			AfxGetPrinter()->SetColor(TEXT_ARGB_WHITE);
			m_attrib.name[19] = 0;
			AfxGetPrinter()->Print(nDrawX, nDrawY, m_attrib.name);
		}
	}
}

/************************************************************************/
/* 绘制血条
/************************************************************************/
void GameObject::RenderHPBar()
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
	int nTextureHeightOffset = -1;

	//	画血条
	GameTextureManager* t_TexMgr = GameResourceManager::GetInstance()->GetTexs(RES_OPUI);
	int nTextIndex = TEX_HUM_HPTUBEFULL;
	if(GetType() == OBJ_PLAYER)
	{
		nTextIndex = 52;
	}
	else if(GetType() == OBJ_OTHERPLAYER)
	{
		nTextIndex = 50;
	}
	else if(GetType() == OBJ_MONS)
	{
		GameMonster* pMonster = (GameMonster*)this;
		if(pMonster->GetMasterId() != 0)
		{
			nTextIndex = 53;
		}
	}

	if(t_TexMgr)
	{
		HTEXTURE texFull = 0;
		HTEXTURE texEmpty = 0;

		texFull = t_TexMgr->GetTexture(nTextIndex);
		texEmpty = t_TexMgr->GetTexture(TEX_HUM_HPTUBEBLANK);
		USHORT uWidth = 0;
		USHORT uHeight = 0;

		if(texFull &&
			texEmpty)
		{
			uWidth = t_TexMgr->GetTextureWidth(nTextIndex);
			uHeight = t_TexMgr->GetTextureHeight(nTextIndex);
			float fScale = (float)m_attrib.HP / (float)m_attrib.maxHP;

			if(-1 == nTextureHeightOffset)
			{
				nDrawY -= (2 * UNIT_HEIGHT);
			}
			else
			{
				nDrawY -= nTextureHeightOffset;
			}
			
			nDrawX += UNIT_WIDTH / 2;
			nDrawX -= uWidth / 2;

			if(!m_pRender)
			{
				m_pRender = new hgeSprite(texEmpty, 0, 0, uWidth, uHeight);
			}
			else
			{
				m_pRender->SetTexture(texEmpty);
				m_pRender->SetTextureRect(0, 0, uWidth, uHeight);
			}
			m_pRender->Render(nDrawX, nDrawY);

			m_pRender->SetTexture(texFull);
			m_pRender->SetTextureRect(0, 0, fScale * uWidth, uHeight);
			m_pRender->Render(nDrawX, nDrawY);
		}
	}

	if(pTheGame->Config_RenderHPBar())
	{
		//	写数字
		char szHealth[15] = {0};
		sprintf(szHealth, "%d/%d", m_attrib.HP, m_attrib.maxHP);
		int nWidth = strlen(szHealth) * 6;
		nDrawX = CENTRAL_X + (int)m_fPosx - (int)pPlayer->GetObjx();
		nDrawX += UNIT_WIDTH / 2;
		nDrawX -= nWidth / 2;
		//AfxGetFont()->Print(nDrawX, nDrawY - 12, szHealth);
		AfxGetPrinter()->SetColor(TEXT_ARGB_WHITE);
		AfxGetPrinter()->Print(nDrawX, nDrawY - 12, szHealth);;
	}
}

/************************************************************************/
/* 鼠标拾取EX
/************************************************************************/
bool GameObject::IsSelectedEx(const POINT& _pt)
{
	//	快速判定
	GamePlayer* pPlayer = GamePlayer::GetInstance();
	int nDrawX = CENTRAL_X + (int)m_fPosx - (int)pPlayer->GetObjx();
	int nDrawY = CENTRAL_Y + (int)m_fPosy - (int)pPlayer->GetObjy();

	if(m_nCurrentSelectIndex == m_nCurrentTextureIndex)
	{
		if(m_nCurrentTextureIndex == -1)
		{
			return false;
		}
		else
		{

		}
	}

	return false;
}

/************************************************************************/
/* 得到物体包围矩形
/************************************************************************/
void GameObject::GetMinAroundRect(RECT& _stOut)
{
	ZeroMemory(&_stOut, sizeof(RECT));

	if(m_nCurrentTextureIndex == -1)
	{
		return;
	}
	int _idx = m_nCurrentTextureIndex;

	//	载入资源
	int nResIndex = GetTextureMgrIndex();
	//ZeroMemory(&m_rcSelectRect, sizeof(RECT));
	GameTextureManager* pTexMgr = GameResourceManager::GetInstance()->GetTexs(nResIndex);

	if(pTexMgr)
	{
		HTEXTURE tex = pTexMgr->GetTexture(_idx);

		if(tex)
		{
			int nDrawX = 0;
			int nDrawY = 0;
			//short sOffsetX = pTexMgr->GetTextureOffsetX(_idx);
			//short sOffsetY = pTexMgr->GetTextureOffsetY(_idx);
			short sWidth = pTexMgr->GetTextureWidth(_idx);
			short sHeight = pTexMgr->GetTextureHeight(_idx);

			DWORD dwActlWidth = AfxGetHge()->Texture_GetWidth(tex);
			DWORD dwActlHeight = AfxGetHge()->Texture_GetHeight(tex);
			DWORD* pColValue = AfxGetHge()->Texture_Lock(tex);

			int nMinLeft = -1;
			int nMaxRight = -1;
			int nMinTop = -1;
			int nMaxBottom = -1;

			//nDrawX += sOffsetX;
			//nDrawY += sOffsetY;

			for(int i = 0; i < sWidth; ++i)
			{
				for(int j = 0; j < sHeight; ++j)
				{
					int nCurMaxWidth = 0;
					int nCurMaxHeight = 0;

					DWORD dwColor = pColValue[j * dwActlWidth + i];

					/*if(dwColor == 0xFF000000 ||
						dwColor == ARGB(255, 16, 8, 8))
					{
						DebugBreak();
					}*/

					if(dwColor != ARGB(255, 16, 8, 8) &&
						(dwColor & 0xFF000000) != 0)
					{
						if(nMinLeft == -1 &&
							nMaxRight == -1)
						{
							nMinLeft = nMaxRight = nDrawX + i;
						}
						else
						{
							if(nDrawX + i < nMinLeft)
							{
								nMinLeft = nDrawX + i;
							}
							else if(nDrawX + i > nMaxRight)
							{
								nMaxRight = nDrawX + i;
							}
						}

						if(nMinTop == -1 &&
							nMaxBottom == -1)
						{
							nMinTop = nMaxBottom = nDrawY + j;
						}
						else
						{
							if(nDrawY + j < nMinTop)
							{
								nMinTop = nDrawY + j;
							}
							else if(nDrawY + j > nMaxBottom)
							{
								nMaxBottom = nDrawY + j;
							}
						}
					}
				}
			}

			AfxGetHge()->Texture_Unlock(tex);

			if(nMaxBottom != -1 &&
				nMaxRight != -1 &&
				nMinLeft != -1 &&
				nMinTop != -1)
			{
				m_rcSelectRect.left = nMinLeft;
				m_rcSelectRect.right = nMaxRight;
				m_rcSelectRect.top = nMinTop;
				m_rcSelectRect.bottom = nMaxBottom;
			}
		}
	}
}

void GameObject::GetMaxAroundRect(RECT& _stOut)
{
	ZeroMemory(&_stOut, sizeof(RECT));

	if(m_nCurrentTextureIndex == -1)
	{
		return;
	}

	GamePlayer* pPlayer = GamePlayer::GetInstance();
	int nDrawX = CENTRAL_X + (int)m_fPosx - (int)pPlayer->GetObjx();
	int nDrawY = CENTRAL_Y + (int)m_fPosy - (int)pPlayer->GetObjy();

	//	载入资源
	int nResIndex = GetTextureMgrIndex();
	GameTextureManager* pTexMgr = GameResourceManager::GetInstance()->GetTexs(nResIndex);

	if(pTexMgr)
	{
		HTEXTURE tex = pTexMgr->GetTexture(m_nCurrentTextureIndex);

		if(tex)
		{
			short sOffsetX = pTexMgr->GetTextureOffsetX(m_nCurrentTextureIndex);
			short sOffsetY = pTexMgr->GetTextureOffsetY(m_nCurrentTextureIndex);
			short sWidth = pTexMgr->GetTextureWidth(m_nCurrentTextureIndex);
			short sHeight = pTexMgr->GetTextureHeight(m_nCurrentTextureIndex);

			_stOut.right = sWidth;
			_stOut.bottom = sHeight;
			_stOut.left += (nDrawX + sOffsetX);
			_stOut.right += (nDrawX + sOffsetX);
			_stOut.top += (nDrawY + sOffsetY);
			_stOut.bottom += (nDrawY + sOffsetY);
		}
	}
}

int GameObject::GetScreenPosX()
{
	GamePlayer* pPlayer = GamePlayer::GetInstance();
	if(NULL == pPlayer)
	{
		return 0;
	}

	int nPosX = CENTRAL_X + (int)m_fPosx - (int)pPlayer->GetObjx();
	return nPosX;
	int nDrawOffsetY = CENTRAL_Y + (int)m_fPosy - (int)pPlayer->GetObjy();
}

int GameObject::GetScreenPosY()
{
	GamePlayer* pPlayer = GamePlayer::GetInstance();
	if(NULL == pPlayer)
	{
		return 0;
	}

	int nPosY = CENTRAL_Y + (int)m_fPosy - (int)pPlayer->GetObjy();
	return nPosY;
}

/************************************************************************/
/* 更新选取矩形
/************************************************************************/
void GameObject::UpdateSelectRect(int _idx)
{
	if(_idx == m_nCurrentSelectIndex)
	{
		return;
	}

	GamePlayer* pPlayer = GamePlayer::GetInstance();
	int nDrawX = CENTRAL_X + (int)m_fPosx - (int)pPlayer->GetObjx();
	int nDrawY = CENTRAL_Y + (int)m_fPosy - (int)pPlayer->GetObjy();

	//	载入资源
	int nResIndex = GetTextureMgrIndex();
	ZeroMemory(&m_rcSelectRect, sizeof(RECT));
	GameTextureManager* pTexMgr = GameResourceManager::GetInstance()->GetTexs(nResIndex);

	if(pTexMgr)
	{
		HTEXTURE tex = pTexMgr->GetTexture(_idx);

		if(tex)
		{
			short sOffsetX = pTexMgr->GetTextureOffsetX(_idx);
			short sOffsetY = pTexMgr->GetTextureOffsetY(_idx);
			short sWidth = pTexMgr->GetTextureWidth(_idx);
			short sHeight = pTexMgr->GetTextureHeight(_idx);

			DWORD dwActlWidth = AfxGetHge()->Texture_GetWidth(tex);
			DWORD dwActlHeight = AfxGetHge()->Texture_GetHeight(tex);
			DWORD* pColValue = AfxGetHge()->Texture_Lock(tex);

			int nMinLeft = -1;
			int nMaxRight = -1;
			int nMinTop = -1;
			int nMaxBottom = -1;

			nDrawX += sOffsetX;
			nDrawY += sOffsetY;

			for(int i = 0; i < sWidth; ++i)
			{
				for(int j = 0; j < sHeight; ++j)
				{
					int nCurMaxWidth = 0;
					int nCurMaxHeight = 0;

					DWORD dwColor = pColValue[j * dwActlWidth + i];

					/*if(dwColor == 0xFF000000 ||
						dwColor == ARGB(255, 16, 8, 8))
					{
						DebugBreak();
					}*/

					if(dwColor != ARGB(255, 16, 8, 8) &&
						(dwColor & 0xFF000000) != 0)
					{
						if(nMinLeft == -1 &&
							nMaxRight == -1)
						{
							nMinLeft = nMaxRight = nDrawX + i;
						}
						else
						{
							if(nDrawX + i < nMinLeft)
							{
								nMinLeft = nDrawX + i;
							}
							else if(nDrawX + i > nMaxRight)
							{
								nMaxRight = nDrawX + i;
							}
						}

						if(nMinTop == -1 &&
							nMaxBottom == -1)
						{
							nMinTop = nMaxBottom = nDrawY + j;
						}
						else
						{
							if(nDrawY + j < nMinTop)
							{
								nMinTop = nDrawY + j;
							}
							else if(nDrawY + j > nMaxBottom)
							{
								nMaxBottom = nDrawY + j;
							}
						}
					}
				}
			}

			AfxGetHge()->Texture_Unlock(tex);

			if(nMaxBottom != -1 &&
				nMaxRight != -1 &&
				nMinLeft != -1 &&
				nMinTop != -1)
			{
				m_rcSelectRect.left = nMinLeft;
				m_rcSelectRect.right = nMaxRight;
				m_rcSelectRect.top = nMinTop;
				m_rcSelectRect.bottom = nMaxBottom;
				m_nCurrentSelectIndex = _idx;

				/*AfxGetHge()->System_Log("Rect of texture:l%dt%dw%dh%d Rect of select:l%dt%dw%dh%d",
					nDrawX, nDrawY, sWidth, sHeight,
					m_rcSelectRect.left, m_rcSelectRect.top,
					m_rcSelectRect.right - m_rcSelectRect.left,
					m_rcSelectRect.bottom - m_rcSelectRect.top);*/
			}
		}
	}
}

/************************************************************************/
/* 是否选中
/************************************************************************/
bool GameObject::IsSelected(const POINT& _pt)
{
	if(!IsVisible())
	{
		return false;
	}
	if(-1 == m_nCurrentTextureIndex)
	{
		return false;
	}

	if(m_type == OBJ_OTHERPLAYER)
	{
		int c = 0;
	}

	//	Find rect cache
	if(pTheGame->Config_EnhanceSel())
	{
		RECT stObjRect;
		GetMaxAroundRect(stObjRect);
		if(stObjRect.left != stObjRect.right)
		{
			return PtInRect(&stObjRect, _pt) ? true : false;
		}
	}

	/*UpdateSelectRect(m_nCurrentTextureIndex);
	if(PtInRect(&m_rcSelectRect, _pt))
	{
		return true;
	}
	else
	{
		return false;
	}*/

	GamePlayer* pPlayer = GamePlayer::GetInstance();
	int nDrawX = CENTRAL_X + (int)m_fPosx - (int)pPlayer->GetObjx();
	int nDrawY = CENTRAL_Y + (int)m_fPosy - (int)pPlayer->GetObjy();

	//	载入资源
	int nResIndex = GetTextureMgrIndex();
// 	int nResIndex = 0;
// 	if(m_type == OBJ_MONS)
// 	{
// 		//	怪物
// 		nResIndex = RES_MON1 + static_cast<GameMonster*>(this)->GetRenderInfo()->wil;
// 		if(nResIndex > RES_MON17)
// 		{
// 			return false;
// 		}
// 	}
// 	else if(m_type == OBJ_NPC)
// 	{
// 		//	NPC
// 		nResIndex = RES_NPC;
// 	}
// 	else if(m_type == OBJ_PLAYER ||
// 		m_type == OBJ_SELF)
// 	{
// 		//	其余玩家
// 		nResIndex = RES_HUM;
// 	}

	GameTextureManager* t_TexMgr = GameResourceManager::GetInstance()->GetTexs(nResIndex);
	if(!t_TexMgr)
	{
		return false;
	}
	HTEXTURE tex = t_TexMgr->GetTexture(m_nCurrentTextureIndex);
	if(!tex)
	{
		return false;
	}

	short sOffsetX = t_TexMgr->GetTextureOffsetX(m_nCurrentTextureIndex);
	short sOffsetY = t_TexMgr->GetTextureOffsetY(m_nCurrentTextureIndex);
	short sWidth = t_TexMgr->GetTextureWidth(m_nCurrentTextureIndex);
	short sHeight = t_TexMgr->GetTextureHeight(m_nCurrentTextureIndex);

	//	缩小4像素 生成检测矩形
	RECT rcMons;
	rcMons.left = nDrawX + sOffsetX + 2;
	rcMons.right = rcMons.left + sWidth - 2;
	rcMons.top = nDrawY + sOffsetY + 2;
	rcMons.bottom = rcMons.top + sHeight - 2;
	if(!::PtInRect(&rcMons, _pt))
	{
		return false;
	}

	if(rcMons.left > rcMons.right ||
		rcMons.top > rcMons.bottom)
	{
		return false;
	}

	//return true;

	//	进行像素检测
	//	还原相对矩形
	POINT ptDetect = _pt;
	ptDetect.x = _pt.x - (nDrawX + sOffsetX);
	ptDetect.y = _pt.y - (nDrawY + sOffsetY);
	if(ptDetect.x < 0 ||
		ptDetect.y < 0)
	{
		return false;
	}
	DWORD dwActlWidth = AfxGetHge()->Texture_GetWidth(tex);
	DWORD dwActlHeight = AfxGetHge()->Texture_GetHeight(tex);
	DWORD* pColValue = AfxGetHge()->Texture_Lock(tex);

	if((pColValue[ptDetect.y * dwActlWidth + ptDetect.x] & 0xFF000000) != 0 &&
		(pColValue[ptDetect.y * dwActlWidth + ptDetect.x + 1] & 0xFF000000) != 0 &&
		(pColValue[ptDetect.y * dwActlWidth + ptDetect.x - 1] & 0xFF000000) != 0 &&
		(pColValue[(ptDetect.y + 1) * dwActlWidth + ptDetect.x] & 0xFF000000) != 0 &&
		(pColValue[(ptDetect.y - 1) * dwActlWidth + ptDetect.x] & 0xFF000000) != 0)
	{
		AfxGetHge()->Texture_Unlock(tex);
		return true;
	}
	AfxGetHge()->Texture_Unlock(tex);

	return false;
}

//////////////////////////////////////////////////////////////////////////
bool GameObject::UpdateHide()
{
	if(TEST_FLAG_BOOL(m_dwHumEffectFlag, MMASK_HIDE))
	{
		//	In hide mode
		if(GetTickCount() > m_dwHumEffectTime[MMASK_C_HIDE_INDEX])
		{
			RESET_FLAG(m_dwHumEffectFlag, MMASK_HIDE);
			m_dwHumEffectTime[MMASK_C_HIDE_INDEX] = 0;
			return false;
		}
		return true;
	}
	return false;
}

/************************************************************************/
/* 获得资源索引(获取对应的GameTextureManager的索引)
/************************************************************************/
int GameObject::GetTextureMgrIndex()
{
	int nResIndex = 0;
	if(m_type == OBJ_MONS)
	{
		//	怪物
		nResIndex = RES_MON1 + static_cast<GameMonster*>(this)->GetRenderInfo()->wil;
		if(nResIndex > RES_MON33)
		{
			return false;
		}
	}
	else if(m_type == OBJ_NPC)
	{
		//	NPC
		nResIndex = RES_NPC;
	}
	else if(m_type == OBJ_PLAYER ||
		m_type == OBJ_SELF ||
		m_type == OBJ_OTHERPLAYER)
	{
		//	其余玩家
		nResIndex = RES_HUM;
	}

	return nResIndex;
}

/************************************************************************/
/* 画透明人物
/************************************************************************/
void GameObject::TransparentRender()
{
	/*if(!m_pRender)
	{
		return;
	}

	DWORD dwPreCol = m_pRender->GetColor();

	m_pRender->SetColor(ARGB(96, 255, 255, 255));
	Render();
	m_pRender->SetColor(dwPreCol);*/

	if(!m_pRender)
	{
		return;
	}
	if(!IsVisible())
	{
		return;
	}

	/*GamePlayer* pPlayer = GamePlayer::GetInstance();
	int nRenderX = CENTRAL_X + (int)m_fPosx - (int)pPlayer->GetObjx();
	int nRenderY = CENTRAL_Y + (int)m_fPosy - (int)pPlayer->GetObjy();*/
	DWORD dwPreCol = m_pRender->GetColor();
	DWORD dwCurTick = GetTickCount();
	bool bChangedColor = false;

	if(TEST_FLAG(m_dwHumEffectFlag, MMASK_HONGDU))
	{
		if(m_dwHumEffectTime[MMASK_C_HONGDU_INDEX] < dwCurTick)
		{
			m_dwHumEffectTime[MMASK_C_HONGDU_INDEX] = 0;
			RESET_FLAG(m_dwHumEffectFlag, MMASK_HONGDU);
		}
		else
		{
			m_pRender->SetColor(ARGB(96, 0xFF, 0, 0));
			bChangedColor = true;
		}
	}

	if(TEST_FLAG(m_dwHumEffectFlag, MMASK_LVDU))
	{
		if(m_dwHumEffectTime[MMASK_C_LVDU_INDEX] < dwCurTick)
		{
			m_dwHumEffectTime[MMASK_C_LVDU_INDEX] = 0;
			RESET_FLAG(m_dwHumEffectFlag, MMASK_LVDU);
		}
		else
		{
			m_pRender->SetColor(ARGB(96, 0, 0xFF, 0));
			bChangedColor = true;
		}
	}

	if(TEST_FLAG_BOOL(m_dwHumEffectFlag, MMASK_ICE))
	{
		if(m_dwHumEffectTime[MMASK_C_ICE_INDEX] < dwCurTick)
		{
			m_dwHumEffectTime[MMASK_C_ICE_INDEX] = 0;
			RESET_FLAG(m_dwHumEffectFlag, MMASK_ICE);
		}
		else
		{
			m_pRender->SetColor(COLOR_EFF_ICE);
			bChangedColor = true;
		}
	}

	bool bIsStone = false;
	if(TEST_FLAG(m_dwHumEffectFlag, MMASK_STONE))
	{
		if(m_dwHumEffectTime[MMASK_C_STONE_INDEX] < dwCurTick)
		{
			m_dwHumEffectTime[MMASK_C_STONE_INDEX] = 0;
			RESET_FLAG(m_dwHumEffectFlag, MMASK_STONE);
		}
		else
		{
			//m_pRender->SetColor(ARGB(96, 155, 155, 155));
			m_pRender->SetColor(COLOR_EFF_STONE);
			bChangedColor = true;
			bIsStone = true;
		}
	}

	/*if(bIsStone &&
		GetStatus() == PST_STAND)
	{
		SetCurFrame(0);
	}*/

	if(!bChangedColor)
	{
		m_pRender->SetColor(ARGB(96, 255, 255, 255));
	}
	/*if(bIsStone &&
		GetStatus() != PST_DEAD)
	{
		SetStatus(PST_STAND);
		SetCurFrame(0);
	}*/
	Render();
	m_pRender->SetColor(dwPreCol);
}
/************************************************************************/
/* 高亮显示人物
/************************************************************************/
void GameObject::HighlightRender()
{
	return;
	DWORD dwPreColor = m_pRender->GetColor();
	m_pRender->SetColor(ARGB(192, 255, 255, 0));
	Render();
	m_pRender->SetColor(dwPreColor);
}
/************************************************************************/
/* 绘制发言
/************************************************************************/
void GameObject::RenderSaying()
{
	if(AfxGetHge()->Timer_GetTime() - m_fSayingTime > 8.0f)
	{
		ResetSaying();
	}

	if(m_szSaying[0] == 0 ||
		m_fSayingTime == 0.0f)
	{
		return;
	}
	if(m_nCurrentTextureIndex == INVALID_TEXTURE_INDEX)
	{
		return;
	}

	int nDrawX = 0;
	int nDrawY = 0;
	GamePlayer* pPlayer = GamePlayer::GetInstance();

	nDrawX = CENTRAL_X + (int)m_fPosx - (int)pPlayer->GetObjx() + UNIT_WIDTH / 2;
	if(m_stSayingSize.cx > OBJECT_CHAT_EACHLINE)
	{
		nDrawX -= OBJECT_CHAT_EACHLINE / 2;
	}
	else
	{
		nDrawX -= m_stSayingSize.cx / 2;
	}
	nDrawY = CENTRAL_Y + (int)m_fPosy - (int)pPlayer->GetObjy() - 2 * UNIT_HEIGHT - UNIT_HEIGHT - 12 * m_dwLineNumber;

	DWORD dwColor = GetSayingColor();
	if(0 == dwColor)
	{
		dwColor = ARGB_WHITE;
	}

	AfxGetPrinter()->SetColor(dwColor);
	float fPreKerning = AfxGetPrinter()->GetKerningHeight();
	AfxGetPrinter()->SetKerningHeight(1.0f);
	AfxGetPrinter()->LineFeedRender((float)nDrawX, (float)nDrawY, m_szSaying, OBJECT_CHAT_EACHLINE);
	AfxGetPrinter()->SetKerningHeight(fPreKerning);
}
/************************************************************************/
/* 设置发言
/************************************************************************/
bool GameObject::SetSaying(const char* _pszWords)
{
	if(strlen(_pszWords) + 20 >= MAX_SAY_LENGTH)
	{
		return false;
	}
	char szBuf[MAX_PATH];
	sprintf(szBuf, "%s说:%s", m_attrib.name, _pszWords);
	
	if(-1 == mbstowcs(m_szSaying, szBuf, strlen(szBuf) + 1))
	{
		return false;
	}
	m_fSayingTime = AfxGetHge()->Timer_GetTime();
	m_stSayingSize = AfxGetPrinter()->GetTextSize(m_szSaying);
	m_dwLineNumber = m_stSayingSize.cx / OBJECT_CHAT_EACHLINE + 1;

	StringList xStrings;
	int nLines = SplitTextWithLength(szBuf, 12, OBJECT_CHAT_EACHLINE, xStrings);
	m_dwLineNumber = nLines;

	return true;
}
//////////////////////////////////////////////////////////////////////////
bool GameObject::MoveToDestination(WORD _wCoordX, WORD _wCoordY, BYTE _bWay)
{
	int nDrt = -1;
	static const int s_nWalkOft[] =
	{
		0,-1,//UP
		1,-1,//UPRIGHT
		1,0,//RIGHT
		1,1,//RIGHTDOWN
		0,1,//DOWN
		-1,1,//LEFTDOWN
		-1,0,//LEFT
		-1,-1//LEFTUP
	};
	static const int s_nRunOft[] =
	{
		0,-2,//UP
		2,-2,//UPRIGHT
		2,0,//RIGHT
		2,2,//RIGHTDOWN
		0,2,//DOWN
		-2,2,//LEFTDOWN
		-2,0,//LEFT
		-2,-2//LEFTUP
	};

	if(_bWay != MOVE_RUN &&
		_bWay != MOVE_WALK)
	{
		return false;
	}

	int nOftX = _wCoordX - m_ptPos.x;
	int nOftY = _wCoordY - m_ptPos.y;

	if(_bWay == MOVE_WALK)
	{
		for(int i = 0; i < 8; ++i)
		{
			if(nOftX == s_nWalkOft[i * 2] &&
				nOftY == s_nWalkOft[i * 2 + 1])
			{
				nDrt = i;
				break;
			}
		}
	}
	else if(_bWay == MOVE_RUN)
	{
		for(int i = 0; i < 8; ++i)
		{
			if(nOftX == s_nRunOft[i * 2] &&
				nOftY == s_nRunOft[i * 2 + 1])
			{
				nDrt = i;
				break;
			}
		}
	}

	if(nDrt == -1)
	{
		return false;
	}

	PLAYER_DIRECTION pd = (PLAYER_DIRECTION)nDrt;
	SetCurFrame(0);
	ResetTimerCounter();
	SetStatus(_bWay == MOVE_WALK ? PST_WALK : PST_RUN);
	SetDirection(pd);
	SetOffsetCoordX(nOftX);
	SetOffsetCoordY(nOftY);

	m_dwLastMoveTime = GetTickCount();

	return true;
}


//////////////////////////////////////////////////////////////////////////
void GameObject::OnPacket(const PacketHeader* _pPkt)
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
		HANDLE_PACKET(PKG_GAME_PLAYER_SAY_REQ,				PkgPlayerSayReq)
		HANDLE_PACKET(PKG_GAME_OBJECT_ACTION_TURN_NOT,		PkgObjectActionTurnNot)
		HANDLE_PACKET(PKG_GAME_OBJECT_ACTION_WALK_NOT,		PkgObjectActionWalkNot)
		HANDLE_PACKET(PKG_GAME_OBJECT_ACTION_RUN_NOT,		PkgObjectActionRunNot)
		HANDLE_PACKET(PKG_GAME_OBJECT_ACTION_ATTACK_NOT,	PkgObjectActionAttackNot)
		HANDLE_PACKET(PKG_GAME_OBJECT_ACTION_DEAD_NOT,		PkgObjectActionDeadNot)
		HANDLE_PACKET(PKG_GAME_OBJECT_ACTION_EXT_NOT,		PkgObjectActionExtNot)
		default:
		{
		//ALERT_MSGBOX("Unknown opcode!");
			AfxGetHge()->System_Log("未知的数据包[%d]",
				_pPkt->uOp);
		}break;
	END_HANDLE_PACKET(_pPkt)
}

void GameObject::DoPacket(const PkgObjectActionTurnNot& _not)
{
	PkgObjectActionNot not;
	not.uTargetId = _not.uTargetId;
	not.uAction = ACTION_TURN;

	not.uParam1 = SimpleActionHelper::GetDirection(_not);
	not.uParam0 = MAKELONG(SimpleActionHelper::GetCoordX(_not), SimpleActionHelper::GetCoordY(_not));
	DoPacket(not);
}

void GameObject::DoPacket(const PkgObjectActionWalkNot& _not)
{
	PkgObjectActionNot not;
	not.uTargetId = _not.uTargetId;
	not.uAction = ACTION_WALK;

	not.uParam1 = _not.dwPos;
	not.uParam0 = MAKELONG(SimpleActionHelper::GetCoordX(_not), SimpleActionHelper::GetCoordY(_not));
	DoPacket(not);
}

void GameObject::DoPacket(const PkgObjectActionRunNot& _not)
{
	PkgObjectActionNot not;
	not.uTargetId = _not.uTargetId;
	not.uAction = ACTION_RUN;

	not.uParam1 = _not.dwPos;
	not.uParam0 = MAKELONG(SimpleActionHelper::GetCoordX(_not), SimpleActionHelper::GetCoordY(_not));
	DoPacket(not);
}

void GameObject::DoPacket(const PkgObjectActionAttackNot& _not)
{
	PkgObjectActionNot not;
	not.uTargetId = _not.uTargetId;
	not.uAction = ACTION_ATTACK;

	not.uParam1 = SimpleActionHelper::GetDirection(_not);
	not.uParam0 = MAKELONG(SimpleActionHelper::GetCoordX(_not), SimpleActionHelper::GetCoordY(_not));
	DoPacket(not);
}

void GameObject::DoPacket(const PkgObjectActionDeadNot& _not)
{
	PkgObjectActionNot not;
	not.uTargetId = _not.uTargetId;
	not.uAction = ACTION_DEAD;

	not.uParam1 = SimpleActionHelper::GetDirection(_not);
	not.uParam0 = MAKELONG(SimpleActionHelper::GetCoordX(_not), SimpleActionHelper::GetCoordY(_not));
	DoPacket(not);
}

void GameObject::DoPacket(const PkgObjectActionExtNot& _not)
{
	PkgObjectActionNot not;
	not.uTargetId = _not.uTargetId;
	not.uAction = _not.cExtAction;

	not.uParam1 = SimpleActionHelper::GetDirection(_not);
	not.uParam0 = MAKELONG(SimpleActionHelper::GetCoordX(_not), SimpleActionHelper::GetCoordY(_not));
	DoPacket(not);
}
/************************************************************************/
/* void DoPacket(const PkgObjectActionNot& _not)
/************************************************************************/
void GameObject::DoPacket(const PkgObjectActionNot& _not)
{
	static const int s_nMoveOftTable[] = 
	{
		0,-1,	1,-1,	1,0,
		1,1,	0,1,	-1,1,
		-1,0,	-1,-1
	};
	static const int s_nRunOftTable[] = 
	{
		0,-2,	2,-2,	2,0,
		2,2,	0,2,	-2,2,
		-2,0,	-2,-2
	};

	if(_not.uAction == ACTION_CREATE)
	{
		//	Create Monster
		//	first check if the object is hero
		if(GetType() >= OBJ_PLAYER)
		{
			//	create the monster
			WORD wMonsterID = (WORD)_not.uParam0;
			WORD wCoordX = LOWORD(_not.uParam1);
			WORD wCoordY = HIWORD(_not.uParam1);
			//GameMapManager::GetInstance()->CreateMonster(wCoordX, wCoordY, wMonsterID);
		}
	}//	create end
	else if(_not.uAction == ACTION_STATUS)
	{
		//	change object's state
		int nDelta = _not.uParam1;
		switch(_not.uParam0)
		{
		case 0:
			{
				//	hp
				GetAttrib()->HP += nDelta;

				PkgServerStatusReq req;
				req.uSign = 1;
				g_xBuffer.Reset();
				g_xBuffer << req;
				SendBufferToGS(&g_xBuffer);
			}break;
		case 1:
			{
				//	mp
				GetAttrib()->MP += nDelta;
			}break;
		case 2:
			{
				//	exp
				GetAttrib()->EXPR += nDelta;
			}break;
		}
	}//	status end
	else if(_not.uAction == ACTION_TURN)
	{
		//	change the object's direction
		PLAYER_DIRECTION pd = (PLAYER_DIRECTION)_not.uParam1;
		if(pd >= PDT_UP &&
			pd <= PDT_LEFTUP)
		{
			SetDirection(pd);
		}
		SetReallyCoord(LOWORD(_not.uParam0), HIWORD(_not.uParam0));
		SetStatus(PST_STAND);
		SetCurFrame(0);
	}//	turn end
	else if(_not.uAction == ACTION_WALK)
	{
		WORD dwPreCoordX = LOWORD(_not.uParam0);
		WORD dwPreCoordY = HIWORD(_not.uParam0);
		WORD dwNowCoordX = LOWORD(_not.uParam1);
		WORD dwNowCoordY = HIWORD(_not.uParam1);

		SetReallyCoord(dwPreCoordX, dwPreCoordY);
		MoveToDestination(dwNowCoordX, dwNowCoordY, MOVE_WALK);
		SetCurFrame(0);
	}
	else if(_not.uAction == ACTION_RUN)
	{
		WORD dwPreCoordX = LOWORD(_not.uParam0);
		WORD dwPreCoordY = HIWORD(_not.uParam0);
		WORD dwNowCoordX = LOWORD(_not.uParam1);
		WORD dwNowCoordY = HIWORD(_not.uParam1);

		SetReallyCoord(dwPreCoordX, dwPreCoordY);
		MoveToDestination(dwNowCoordX, dwNowCoordY, MOVE_RUN);
		SetCurFrame(0);
	}
	else if(_not.uAction == ACTION_STRUCK)
	{
		//	struck
// 		m_pObject->GetAttrib()->HP = _not.uParam0;
// 		m_pObject->SetReallyCoord(LOWORD(_not.uParam1), HIWORD(_not.uParam1));
// 		m_pObject->SetStatus(PST_ATTACKED);
// 		m_pObject->SetCurFrame(0);
		bool bCriticalAttack = TEST_FLAG_BOOL(_not.uParam3, STRUCK_MASK_CRITICAL);

		/*if(GetType() == OBJ_MONS &&
			GetAttrib()->id == 140 ||
			GetAttrib()->id == 143)
		{
			SetReallyCoord(LOWORD(_not.uParam0), HIWORD(_not.uParam0));
			if(_not.uParam1 <= 7)
			{
				SetDirection((PLAYER_DIRECTION)_not.uParam1);
			}
			SetStatus(PST_ATTACKED);

			int nCurHP = GetAttrib()->HP;
			GetAttrib()->HP = _not.uParam2;
			int nHPChange = GetAttrib()->HP - nCurHP;
			//if(nHPChange < 0)
			{
				GameScene::sThis->InsertNewAttackNumber(abs(nHPChange), nHPChange > 0, this, bCriticalAttack);
			}

			SetCurFrame(0);
		}
		else*/
		{
			SetReallyCoord(LOWORD(_not.uParam0), HIWORD(_not.uParam0));
			if(_not.uParam1 <= 7)
			{
				SetDirection((PLAYER_DIRECTION)_not.uParam1);
			}
			SetStatus(PST_ATTACKED);
			int nCurHP = GetAttrib()->HP;
			GetAttrib()->HP = LOWORD(_not.uParam2);

			int nHPChange = GetAttrib()->HP - nCurHP;
			//if(nHPChange < 0)
			{
				GameScene::sThis->InsertNewAttackNumber(abs(nHPChange), nHPChange > 0, this, bCriticalAttack);
			}

			GetAttrib()->maxHP = HIWORD(_not.uParam2);
			SetCurFrame(0);
		}
	}//struck end
	else if(_not.uAction == ACTION_ATTACK)
	{
		if(GetType() == OBJ_OTHERPLAYER ||
			GetType() == OBJ_MONS)
		{
			SetReallyCoord(LOWORD(_not.uParam0), HIWORD(_not.uParam0));
			SetCurFrame(0);
			if(GetType() == OBJ_MONS)
			{
				SetStatus(PST_ATTACKNOWEAPON);
			}
			else
			{
				SetStatus(PST_ATTACKNOWEAPON);
			}
			SetCurFrame(0);
			if(_not.uParam1 >= 0 &&
				_not.uParam1 <= 7)
			{
				SetDirection((PLAYER_DIRECTION)_not.uParam1);
			}

			/*if(GetType() == OBJ_PLAYER)
			{
				if(GamePlayer::GetInstance()->IsUsingPreLock())
				{
					GamePlayer::GetInstance()->Unlock();
				}
			}*/
		}
		else if(GetType() == OBJ_PLAYER)
		{
			//	攻击包特殊处理
			if(GamePlayer::GetInstance()->IsUsingPreLock())
			{
				SetReallyCoord(LOWORD(_not.uParam0), HIWORD(_not.uParam0));
				SetCurFrame(0);
				if(GetType() == OBJ_MONS)
				{
					SetStatus(PST_ATTACKNOWEAPON);
				}
				else
				{
					SetStatus(PST_ATTACKNOWEAPON);
				}
				SetCurFrame(0);
				if(_not.uParam1 >= 0 &&
					_not.uParam1 <= 7)
				{
					SetDirection((PLAYER_DIRECTION)_not.uParam1);
				}
			}
		}
	}
	else if(_not.uAction == ACTION_DEAD)
	{
		int nCurHP = GetAttrib()->HP;

		SetCurFrame(0);
		SetStatus(PST_DEAD);
		SetReallyCoord(LOWORD(_not.uParam0), HIWORD(_not.uParam0));
		GetAttrib()->HP = 0;

		int nHPChange = GetAttrib()->HP - nCurHP;
		//if(nHPChange < 0)
		{
			GameScene::sThis->InsertNewAttackNumber(abs(nHPChange), nHPChange > 0, this);
		}
		
		if(GamePlayer::GetInstance()->GetAtkMonster() == this)
		{
			GamePlayer::GetInstance()->SetAtkMonster(NULL);
		}
		if(GamePlayer::GetInstance()->GetMgcTarget() == this)
		{
			GamePlayer::GetInstance()->SetMgcTarget(NULL);
		}
		if(GetType() == OBJ_PLAYER)
		{
			GameScene::sThis->GetMainOpt()->GetStaticDlg()->CreateDeadDlg();
		}
	}
	else if(_not.uAction == ACTION_FLY)
	{
		//	播放移动动画
		if(pTheGame->IsInPlayerViewRange(GetCoordX(), GetCoordY()))
		{
			if(GetType() == OBJ_PLAYER)
			{

			}
			else
			{
				GameScene::sThis->PlayTransAnimation(1006, GetCoordX(), GetCoordY());
			}
		}
		SetCurFrame(0);
		SetStatus(PST_STAND);
		SetReallyCoord(LOWORD(_not.uParam1), HIWORD(_not.uParam1));

		if(pTheGame->IsInPlayerViewRange(GetCoordX(), GetCoordY()))
		{
			GameScene::sThis->PlayTransAnimation(1007, GetCoordX(), GetCoordY());
		}
	}
	else if(_not.uAction == ACTION_EXT1)
	{
		SetCurFrame(0);
		SetStatus(PST_EXT1);
		SetReallyCoord(LOWORD(_not.uParam0), HIWORD(_not.uParam0));
		if(_not.uParam1 >= 0 &&
			_not.uParam1 <= 7)
		{
			SetDirection((PLAYER_DIRECTION)_not.uParam1);
		}
	}
	else if(_not.uAction == ACTION_EXT2)
	{
		SetCurFrame(0);
		SetStatus(PST_EXT2);
		SetReallyCoord(LOWORD(_not.uParam0), HIWORD(_not.uParam0));
		if(_not.uParam1 >= 0 &&
			_not.uParam1 <= 7)
		{
			SetDirection((PLAYER_DIRECTION)_not.uParam1);
		}
	}
	else if(_not.uAction == ACTION_EXT3)
	{
		SetCurFrame(0);
		SetStatus(PST_EXT3);
		SetReallyCoord(LOWORD(_not.uParam0), HIWORD(_not.uParam0));
		if(_not.uParam1 >= 0 &&
			_not.uParam1 <= 7)
		{
			SetDirection((PLAYER_DIRECTION)_not.uParam1);
		}
	}
	else if(_not.uAction == ACTION_EXT4)
	{
		SetCurFrame(0);
		SetStatus(PST_EXT4);
		SetReallyCoord(LOWORD(_not.uParam0), HIWORD(_not.uParam0));
		if(_not.uParam1 >= 0 &&
			_not.uParam1 <= 7)
		{
			SetDirection((PLAYER_DIRECTION)_not.uParam1);
		}
	}

	OnObjectAction(_not);
}
/************************************************************************/
/* void DoPacket(const PkgPlayerSayReq& req)
/************************************************************************/
void GameObject::DoPacket(const PkgPlayerSayReq& req)
{
	std::string xSay = req.xName + " : " + req.xWords;
	GameInfoBoardDlg::GetInstance()->InsertBoardMsg(ARGB_WHITE, xSay.c_str());
}
//////////////////////////////////////////////////////////////////////////
void GameObject::SetHumEffect(DWORD _dwMask, DWORD _dwTime)
{
	m_dwHumEffectFlag |= _dwMask;
	_dwTime += GetTickCount();

	if(_dwMask == MMASK_LVDU)
	{
		m_dwHumEffectTime[MMASK_C_LVDU_INDEX] = _dwTime;
	}
	else if(_dwMask == MMASK_HONGDU)
	{
		m_dwHumEffectTime[MMASK_C_HONGDU_INDEX] = _dwTime;
	}
	else if(_dwMask == MMASK_SHIELD)
	{
		m_dwHumEffectTime[MMASK_C_SHEILD_INDEX] = _dwTime;
	}
	else if(_dwMask == MMASK_HIDE)
	{
		m_dwHumEffectTime[MMASK_C_HIDE_INDEX] = _dwTime;
	}
	else if(_dwMask == MMASK_STONE)
	{
		m_dwHumEffectTime[MMASK_C_STONE_INDEX] = _dwTime;
	}
	else if(_dwMask == MMASK_ICE)
	{
		m_dwHumEffectTime[MMASK_C_ICE_INDEX] = _dwTime;
	}
	else if(_dwMask = MMASK_ENERGYSHIELD)
	{
		m_dwHumEffectTime[MMASK_C_ENERGYSHIELD_INDEX] = _dwTime;
	}
}

void GameObject::ResetHumEffect(DWORD _dwMask)
{
	RESET_FLAG(m_dwHumEffectFlag, _dwMask);

	if(_dwMask == MMASK_STONE)
	{
		m_dwHumEffectTime[MMASK_C_STONE_INDEX] = 0;
	}
	else if(_dwMask == MMASK_ICE)
	{
		m_dwHumEffectTime[MMASK_C_ICE_INDEX] = 0;
	}
}

int GameObject::GetSoundIndex()
{
	return 0;
}