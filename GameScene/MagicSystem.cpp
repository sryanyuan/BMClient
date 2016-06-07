#include "../GameScene/MagicSystem.h"
#include "../GameScene/GameInfoManager.h"
#include "../GameScene/GameResourceManager.h"
#include "../Common/GlobalFunction.h"
#include <math.h>
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
/*
 *	MagicObject 
 */
//////////////////////////////////////////////////////////////////////////
/*
hgeSprite* MagicObject::s_pSpr = new hgeSprite(0, 0, 0, 0, 0);

MagicObject::MagicObject(int _nId, int _nMgcId)
{
	m_nId = _nId;
	ZeroMemory(&m_stInfo, sizeof(MagicRenderInfo));
	//m_stInfo.id = _nMgcId;
	m_pTextureMgr = NULL;
	m_bValid = false;
	m_pFunc = NULL;
}

MagicObject::~MagicObject()
{
	//
}

//////////////////////////////////////////////////////////////////////////

/ ************************************************************************ /
/ * Update
/ ************************************************************************ /
void MagicObject::Update(float _dt)
{
	//
}

/ ************************************************************************ /
/ * Render
/ ************************************************************************ /
void MagicObject::Render()
{
	//
	//
	if(!m_bValid)
	{
		return;
	}
	if(m_stInfo.end)
	{
		return;
	}
	if(!m_pTextureMgr)
	{
		return;
	}

	HTEXTURE tex = 0;
	short sDrawOftX = 0;
	short sDrawOftY = 0;

	switch(m_stInfo.stage)
	{
	case MAGIC_STAGE_SELF:
		{
			//
			tex = m_pTextureMgr->GetTexture(m_stInfo.prenderinfo->selfbegin + m_stInfo.curframe);
		}break;
	case MAGIC_STAGE_WAY:
		{
			//
			tex = m_pTextureMgr->GetTexture(m_stInfo.prenderinfo->waybegin + m_stInfo.drt * m_stInfo.prenderinfo->waytotal + m_stInfo.curframe);
		}break;
	case MAGIC_STAGE_DEST:
		{
			//
			tex = m_pTextureMgr->GetTexture(m_stInfo.prenderinfo->destbegin + m_stInfo.curframe);
		}break;
	}

	if(tex)
	{
		s_pSpr->SetTexture(tex);
		s_pSpr->SetTextureRect(0, 0,
			m_pTextureMgr->GetTextureWidth(tex),
			m_pTextureMgr->GetTextureHeight(tex));
		sDrawOftX = m_pTextureMgr->GetTextureOffsetX(tex);
		sDrawOftY = m_pTextureMgr->GetTextureOffsetY(tex);
		float fDrawX = MapPositionToScreenPositionX(m_stInfo.curx + sDrawOftX);
		float fDrawY = MapPositionToScreenPositionY(m_stInfo.cury + sDrawOftY);
		if(InScreen(fDrawX, fDrawY))
		{
			s_pSpr->Render(fDrawX, fDrawY);
		}
	}
}

/ ************************************************************************ /
/ * Init
/ ************************************************************************ /
bool MagicObject::Init()
{
	m_stInfo.curframe = 0;
	m_stInfo.createtime = ::GetTickCount();
	m_stInfo.lastupdatetime = m_stInfo.updatetotaltime = m_stInfo.updatetotaltime = 0;

	if(m_stInfo.prenderinfo->selfframe == 0 &&
		m_stInfo.prenderinfo->wayframe == 0 &&
		m_stInfo.prenderinfo->destframe == 0)
	{
		//bool bRet = GameInfoManager::GetInstance()->GetMagicRenderInfo(m_stRenderInfo.id, &m_stRenderInfo);
		//if(!bRet)
		{
			return false;
		}
	}

	int nResIdx = RES_MAGIC + m_stInfo.prenderinfo->wil;
	if(nResIdx > RES_MAGIC + 1)
	{
		return false;
	}
	m_pTextureMgr = GameResourceManager::GetInstance()->GetTexs(nResIdx);
	if(!m_pTextureMgr)
	{
		return false;
	}
	UpdateValid();
	m_stInfo.createtime = ::GetTickCount();

	//	设置stage
	m_stInfo.stage = MAGIC_STAGE_SELF;
	return true;
}






//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
/ *
 *	MagicFireball
 * /
//////////////////////////////////////////////////////////////////////////
void MagicFireball::Update(float _dt)
{
	//
	if(!m_bValid)
	{
		return;
	}
	if(m_stInfo.end)
	{
		return;
	}

	DWORD dwCurTime = ::GetTickCount();
	if(dwCurTime - m_stInfo.lastupdatetime < m_stInfo.speed)
	{
		return;
	}
	if(dwCurTime - m_stInfo.createtime > 10000)
	{
		//	10s后自动销毁
		m_stInfo.end = 1;
		return;
	}

	switch(m_stInfo.stage)
	{
	case MAGIC_STAGE_NONE:
	case MAGIC_STAGE_SELF:
		{
			//
			if(m_stInfo.curframe == m_stInfo.prenderinfo->selfframe - 1)
			{
				//	last frame
				m_stInfo.stage = MAGIC_STAGE_WAY;
				m_stInfo.curframe = 0;
				POINT p1, p2;
				p1.x = m_stInfo.srcx;
				p1.y = m_stInfo.srcy;
				p2.x = m_stInfo.destx;
				p2.y = m_stInfo.desty;
				m_stInfo.drt = GetAngle(p1, p2);
				
				float fOftX = m_stInfo.destx - m_stInfo.srcx;
				float fOftY = m_stInfo.desty - m_stInfo.srcy;
				if(fOftY == 0)
				{
					m_stInfo.angle = -1000.0f;
				}
				else if(fOftX == 0)
				{
					m_stInfo.angle = -2000.0f;
				}
				else
				{
					m_stInfo.angle = atan(abs((double)fOftY) / abs((double)fOftX));
					m_bRightMove = m_stInfo.destx >= m_stInfo.curx;
					m_bDownMove = m_stInfo.desty >= m_stInfo.cury;
					m_fDistance = sqrt(pow((float)fOftX, 2) + pow((float)fOftY, 2));
				}
			}
			else if(m_stInfo.curframe == 0)
			{
				//	first frame
				m_stInfo.curx = m_stInfo.srcx;
				m_stInfo.cury = m_stInfo.srcy;
				++m_stInfo.curframe;
			}
			else
			{
				//	just increase the frame counter
				++m_stInfo.curframe;
			}
		}break;
	case MAGIC_STAGE_WAY:
		{
			//
			float fOftX = m_stInfo.destx - m_stInfo.curx;
			float fOftY = m_stInfo.desty - m_stInfo.cury;
			if(m_stInfo.curframe == m_stInfo.prenderinfo->wayframe - 1)
			{
				//	last frame
				m_stInfo.curframe = 0;
			}
			else
			{
				//	just increase the frame counter
				++m_stInfo.curframe;
			}

			//	移动过程中判断是否到了目标点
			if((m_stInfo.curx == m_stInfo.destx &&
				m_stInfo.cury == m_stInfo.desty)/ *
				 ||
								m_fDistance <= 0.0001f* /
				)
			{
				//	到达 下一阶段
				m_stInfo.stage = MAGIC_STAGE_DEST;
				m_stInfo.curframe = 0;
			}
			else
			{
				//	按比例移动
				float fMoveX = 0.0f;
				float fMoveY = 0.0f;
				float fMove = 0.0f;
				if(m_stInfo.angle == -1000.0f)
				{
					//	y为0 只更新x
					fMoveX = m_fDistance > MAGIC_MOVE_OFFSET ? MAGIC_MOVE_OFFSET : m_fDistance;
					fMoveX = m_bRightMove ? fMoveX : (-fMoveX);
					m_stInfo.curx += fMoveX;
					m_fDistance -= fMoveX;
					if(m_fDistance == 0.0f)
					{
						m_stInfo.curx = m_stInfo.destx;
						m_stInfo.cury = m_stInfo.desty;
					}
				}
				else if(m_stInfo.angle == -2000.0f)
				{
					//	x为0 只更新y
					fMoveY = m_fDistance > MAGIC_MOVE_OFFSET ? MAGIC_MOVE_OFFSET : m_fDistance;
					fMoveY = m_bDownMove ? fMoveY : (-fMoveY);
					m_stInfo.cury += fMoveY;
					m_fDistance -= fMoveY;
					if(m_fDistance == 0.0f)
					{
						m_stInfo.curx = m_stInfo.destx;
						m_stInfo.cury = m_stInfo.desty;
					}
				}
				else
				{
					if(m_fDistance > MAGIC_MOVE_OFFSET)
					{
						fMoveX = abs(MAGIC_MOVE_OFFSET * cos(m_stInfo.angle));
						fMoveY = abs(MAGIC_MOVE_OFFSET * sin(m_stInfo.angle));
						fMoveX = m_bRightMove ? fMoveX : (-fMoveX);
						fMoveY = m_bDownMove ? fMoveY : (-fMoveY);
						m_stInfo.curx += fMoveX;
						m_stInfo.cury += fMoveY;
						m_fDistance -= MAGIC_MOVE_OFFSET;
						//AfxGetHge()->System_Log("移动%f,%f", fMoveX, fMoveY);
					}
					else
					{
						m_stInfo.curx = m_stInfo.destx;
						m_stInfo.cury = m_stInfo.desty;
					}
				}
			}
		}break;
	case MAGIC_STAGE_DEST:
		{
			//
			if(m_stInfo.curframe == m_stInfo.prenderinfo->destframe - 1)
			{
				m_stInfo.end = 1;

				//	invoke
				if(m_pFunc)
				{
					m_pFunc(this);
				}
			}
			else
			{
				//	just increase the frame counter
				++m_stInfo.curframe;
			}
		}break;
	}
	m_stInfo.lastupdatetime = dwCurTime;
}

/ ************************************************************************ /
/ * Init
/ ************************************************************************ /
bool MagicFireball::Init()
{
	bool bRet = __super::Init();
	if(m_stInfo.prenderinfo->id == 12 ||
		m_stInfo.prenderinfo->id == 13 ||
		m_stInfo.prenderinfo->id == 14)
	{
		m_stInfo.stage = MAGIC_STAGE_NONE;
	}
	return bRet;
}


//////////////////////////////////////////////////////////////////////////
/ *
 *	MagicHeal
 * /
//////////////////////////////////////////////////////////////////////////
void MagicHeal::Update(float _dt)
{
	//
	if(!m_bValid)
	{
		return;
	}
	if(m_stInfo.end)
	{
		return;
	}

	DWORD dwCurTime = ::GetTickCount();
	if(dwCurTime - m_stInfo.lastupdatetime < m_stInfo.prenderinfo->speed)
	{
		return;
	}
	if(dwCurTime - m_stInfo.createtime > 10000)
	{
		//	10s后自动销毁
		m_stInfo.end = 1;
		return;
	}

	switch (m_stInfo.stage)
	{
	case MAGIC_STAGE_SELF:
		{
			//
			if(m_stInfo.curframe == m_stInfo.prenderinfo->selfframe - 1)
			{
				//	last frame.. remove it
				m_stInfo.end = 1;
				//	invoke
				if(m_pFunc)
				{
					m_pFunc(this);
				}
			}
			else if(m_stInfo.curframe == 0)
			{
				m_stInfo.curx = m_stInfo.srcx;
				m_stInfo.cury = m_stInfo.srcy;
				++m_stInfo.curframe;
			}
			else
			{
				//	just increase the frame count
				++m_stInfo.curframe;
			}
		}break;
	}

	m_stInfo.lastupdatetime = dwCurTime;
}

//////////////////////////////////////////////////////////////////////////
/ *
 *	MagicIceRoar
 * /
//////////////////////////////////////////////////////////////////////////
void MagicIceRoar::Update(float _dt)
{
	if(!m_bValid)
	{
		return;
	}
	if(m_stInfo.end)
	{
		return;
	}

	DWORD dwCurTime = ::GetTickCount();
	if(dwCurTime - m_stInfo.lastupdatetime < m_stInfo.prenderinfo->speed)
	{
		return;
	}
	if(dwCurTime - m_stInfo.createtime > 10000)
	{
		//	10s后自动销毁
		//m_stRenderInfo.end = 1;
		//return;
	}

	switch (m_stInfo.stage)
	{
	case MAGIC_STAGE_NONE:
	case MAGIC_STAGE_SELF:
		{
			//
			if(m_stInfo.curframe == m_stInfo.prenderinfo->selfframe - 1)
			{
				//	last frame, turn to dest
				m_stInfo.stage = MAGIC_STAGE_DEST;
				m_stInfo.curframe = 0;
				m_stInfo.curx = m_stInfo.destx;
				m_stInfo.cury = m_stInfo.desty;
			}
			else if(m_stInfo.curframe == 0)
			{
				m_stInfo.curx = m_stInfo.srcx;
				m_stInfo.cury = m_stInfo.srcy;
				++m_stInfo.curframe;
			}
			else
			{
				//	just increase the frame count
				++m_stInfo.curframe;
			}
		}break;
	case MAGIC_STAGE_DEST:
		{
			//
			if(m_stInfo.curframe == m_stInfo.prenderinfo->destframe - 1)
			{
				//	last frame
				m_stInfo.end = 1;
				if(m_pFunc)
				{
					m_pFunc(this);
				}
			}
			else
			{
				++m_stInfo.curframe;
			}
		}break;
	}

	m_stInfo.lastupdatetime = dwCurTime;
}

/ ************************************************************************ /
/ * Init
/ ************************************************************************ /
bool MagicIceRoar::Init()
{
	bool bRet = __super::Init();
	if(m_stInfo.prenderinfo->id == 11 ||
		m_stInfo.prenderinfo->id == 47)
	{
		m_stInfo.stage = MAGIC_STAGE_NONE;
	}
	return bRet;
}


//////////////////////////////////////////////////////////////////////////
/ *
 *	MagicChainThunder
 * /
//////////////////////////////////////////////////////////////////////////
void MagicChainThunder::Update(float _dt)
{
	if(!m_bValid)
	{
		return;
	}
	if(m_stInfo.end)
	{
		return;
	}

	DWORD dwCurTime = ::GetTickCount();
	if(dwCurTime - m_stInfo.lastupdatetime < m_stInfo.prenderinfo->speed)
	{
		return;
	}
	if(dwCurTime - m_stInfo.createtime > 10000)
	{
		//	10s后自动销毁
		m_stInfo.end = 1;
		return;
	}

	switch (m_stInfo.stage)
	{
	}
}

//////////////////////////////////////////////////////////////////////////
/ *
 *	MagicFireWall
 * /
//////////////////////////////////////////////////////////////////////////
void MagicFireWall::Update(float _dt)
{
	if(!m_bValid)
	{
		return;
	}
	if(m_stInfo.end)
	{
		return;
	}
	DWORD dwCurTime = ::GetTickCount();
	if(dwCurTime - m_stInfo.lastupdatetime < m_stInfo.prenderinfo->speed)
	{
		return;
	}

	switch (m_stInfo.stage)
	{
	case MAGIC_STAGE_SELF:
		{
			//
			if(m_stInfo.curframe == m_stInfo.prenderinfo->selfframe - 1)
			{
				//	last frame, turn to dest
				m_stInfo.stage = MAGIC_STAGE_DEST;
				m_stInfo.curframe = 0;
				m_stInfo.curx = m_stInfo.destx;
				m_stInfo.cury = m_stInfo.desty;
			}
			else if(m_stInfo.curframe == 0)
			{
				m_stInfo.curx = m_stInfo.srcx;
				m_stInfo.cury = m_stInfo.srcy;
				++m_stInfo.curframe;
			}
			else
			{
				//	just increase the frame count
				++m_stInfo.curframe;
			}
		}break;
	case MAGIC_STAGE_DEST:
		{
			//
			if(m_stInfo.curframe == m_stInfo.destframe - 1)
			{
				//	last frame
				if(m_stInfo.updatetotaltime > m_stInfo.looptime)
				{
					//	end
					m_stInfo.end = 1;
				}
				else
				{
					m_stInfo.curframe = 0;
				}
			}
			else if(m_stInfo.curframe == 0)
			{
				m_stInfo.curx = m_stInfo.destx;
				m_stInfo.cury = m_stInfo.desty;
				++m_stInfo.curframe;
			}
			else
			{
				++m_stInfo.curframe;
			}
			m_stInfo.updatetotaltime += (dwCurTime - m_stInfo.lastupdatetime);
		}break;
	}

	m_stInfo.lastupdatetime = dwCurTime;
}

void MagicFireWall::Render()
{
	if(!m_bValid)
	{
		return;
	}
	if(m_stInfo.end)
	{
		return;
	}
	if(!m_pTextureMgr)
	{
		return;
	}

	HTEXTURE tex = 0;
	short sDrawOftX = 0;
	short sDrawOftY = 0;

	switch(m_stInfo.stage)
	{
	case MAGIC_STAGE_SELF:
		{
			//
			tex = m_pTextureMgr->GetTexture(m_stInfo.prenderinfo->selfbegin + m_stInfo.curframe);
		}break;
	case MAGIC_STAGE_WAY:
		{
			//
			tex = m_pTextureMgr->GetTexture(m_stInfo.prenderinfo->waybegin + m_stInfo.drt * m_stInfo.prenderinfo->waytotal + m_stInfo.curframe);
		}break;
	case MAGIC_STAGE_DEST:
		{
			//
			tex = m_pTextureMgr->GetTexture(m_stInfo.prenderinfo->destbegin + m_stInfo.curframe);
		}break;
	}

	if(tex)
	{
		s_pSpr->SetTexture(tex);
		s_pSpr->SetTextureRect(0, 0,
			m_pTextureMgr->GetTextureWidth(tex),
			m_pTextureMgr->GetTextureHeight(tex));
		sDrawOftX = m_pTextureMgr->GetTextureOffsetX(tex);
		sDrawOftY = m_pTextureMgr->GetTextureOffsetY(tex);
		int nDrawX = 0;
		int nDrawY = 0;
		nDrawX = ((int)m_stInfo.curx) / UNIT_WIDTH * UNIT_WIDTH;
		nDrawY = ((int)m_stInfo.cury) / UNIT_HEIGHT * UNIT_HEIGHT;
		float fDrawX = MapPositionToScreenPositionX(nDrawX + sDrawOftX);
		float fDrawY = MapPositionToScreenPositionY(nDrawY + sDrawOftY);
		if(InScreen(fDrawX, fDrawY))
		{
			s_pSpr->Render(fDrawX, fDrawY);

			if(m_stInfo.stage == MAGIC_STAGE_DEST)
			{
				static const int s_nOft[] = {0, -1,
					-1, 0,
					1,0,
					0,1};
				for(int i = 0; i < 4; ++i)
				{
					fDrawX = MapPositionToScreenPositionX(nDrawX + sDrawOftX + s_nOft[2 * i] * UNIT_WIDTH);
					fDrawY = MapPositionToScreenPositionY(nDrawY + sDrawOftY + s_nOft[2 * i + 1] * UNIT_HEIGHT);
					s_pSpr->Render(fDrawX, fDrawY);
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
/ *
 *	MagicSwordKee
 * /
//////////////////////////////////////////////////////////////////////////
void MagicSwordKee::Update(float _dt)
{
	if(!m_bValid)
	{
		return;
	}
	if(m_stInfo.end)
	{
		//return;
	}

	switch (m_stInfo.stage)
	{
	case MAGIC_STAGE_SELF:
		{
			//
			if(m_stInfo.psrc->GetStatus() != PST_ATTACKNOWEAPON &&
				m_stInfo.psrc->GetStatus() != PST_ATTACKWEAPON)
			{
				m_stInfo.end = 1;
				m_stInfo.curframe = 0;
				return;
			}
			else
			{
				m_stInfo.end = 0;
			}

			m_stInfo.curframe = m_stInfo.psrc->GetCurFrame();
			if(m_stInfo.curframe == 0)
			{
				m_stInfo.curx = m_stInfo.psrc->GetObjx();
				m_stInfo.cury = m_stInfo.psrc->GetObjy();
				m_stInfo.drt = m_stInfo.psrc->GetDirection();
			}
			if(m_stInfo.curframe == m_stInfo.prenderinfo->selfframe - 1)
			{
				//	last frame
				m_stInfo.end = 1;
			}
		}break;
	}
}

bool MagicSwordKee::Init()
{
	bool bRet = __super::Init();
	//m_bValid = true;
	return bRet;
}

void MagicSwordKee::Render()
{
	//
	//
	if(!m_bValid)
	{
		return;
	}
	if(m_stInfo.end)
	{
		return;
	}
	if(!m_pTextureMgr)
	{
		return;
	}

	HTEXTURE tex = 0;
	short sDrawOftX = 0;
	short sDrawOftY = 0;

	switch(m_stInfo.stage)
	{
	case MAGIC_STAGE_SELF:
		{
			//
			tex = m_pTextureMgr->GetTexture(m_stInfo.prenderinfo->selfbegin + m_stInfo.curframe + m_stInfo.drt * m_stInfo.prenderinfo->selftotal);
		}break;
	case MAGIC_STAGE_WAY:
		{
			//
			tex = m_pTextureMgr->GetTexture(m_stInfo.prenderinfo->waybegin + m_stInfo.drt * m_stInfo.prenderinfo->waytotal + m_stInfo.curframe);
		}break;
	case MAGIC_STAGE_DEST:
		{
			//
			tex = m_pTextureMgr->GetTexture(m_stInfo.prenderinfo->destbegin + m_stInfo.curframe);
		}break;
	}

	if(tex)
	{
		s_pSpr->SetTexture(tex);
		s_pSpr->SetTextureRect(0, 0,
			m_pTextureMgr->GetTextureWidth(tex),
			m_pTextureMgr->GetTextureHeight(tex));
		sDrawOftX = m_pTextureMgr->GetTextureOffsetX(tex);
		sDrawOftY = m_pTextureMgr->GetTextureOffsetY(tex);
		float fDrawX = MapPositionToScreenPositionX(m_stInfo.curx + sDrawOftX);
		float fDrawY = MapPositionToScreenPositionY(m_stInfo.cury + sDrawOftY);
		if(InScreen(fDrawX, fDrawY))
		{
			s_pSpr->Render(fDrawX, fDrawY);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
/ *
 *	MagicShield
 * /
//////////////////////////////////////////////////////////////////////////
void MagicShield::Update(float _dt)
{
	//
	if(!m_bValid)
	{
		return;
	}
	if(m_stInfo.end)
	{
		return;
	}

	DWORD dwCurTime = ::GetTickCount();

	switch (m_stInfo.stage)
	{
	case MAGIC_STAGE_SELF:
		{
			//
			if(dwCurTime - m_stInfo.lastupdatetime < m_stInfo.prenderinfo->speed)
			{
				return;
			}

			if(m_stInfo.curframe == 0)
			{
				m_stInfo.curx = m_stInfo.psrc->GetObjx();
				m_stInfo.cury = m_stInfo.psrc->GetObjy();
				++m_stInfo.curframe;
			}
			else if(m_stInfo.curframe == m_stInfo.prenderinfo->selfframe - 1)
			{
				//	last frame
				m_stInfo.stage = MAGIC_STAGE_DEST;
				m_stInfo.curframe = 0;
			}
		}break;
	case MAGIC_STAGE_DEST:
		{
			if(dwCurTime - m_stInfo.lastupdatetime < m_stInfo.prenderinfo->speed)
			{
				return;
			}
			m_stInfo.curx = m_stInfo.psrc->GetObjx();
			m_stInfo.cury = m_stInfo.psrc->GetObjy();

			if(m_stInfo.updatetotaltime > m_stInfo.looptime)
			{
				m_stInfo.end = 1;
			}
			else
			{
				if(m_stInfo.curframe == m_stInfo.prenderinfo->destframe - 1)
				{
					//	lastframe
					m_stInfo.curframe = 0;
				}
				else
				{
					++m_stInfo.curframe;
				}
			}
			m_stInfo.updatetotaltime += (dwCurTime - m_stInfo.lastupdatetime);
		}
	}

	m_stInfo.lastupdatetime = dwCurTime;
}

// void MagicShield::Render()
// {
// 	//
// 	__super::Render();
// }

































//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
/ *
 *	MagicSystem
 * /
//////////////////////////////////////////////////////////////////////////
MagicSystem::MagicSystem()
{
	//
}

MagicSystem::~MagicSystem()
{
	//
	MagicObject* pObj = NULL;
	MagicObjectList::const_iterator iter;
	for(iter = m_xObjects.begin();
		iter != m_xObjects.end();
		++iter)
	{
		pObj = *iter;
		delete pObj;
	}
	m_xObjects.clear();
}


//////////////////////////////////////////////////////////////////////////

/ ************************************************************************ /
/ * CreateMagic
/ ************************************************************************ /
MagicObject* MagicSystem::CreateMagic(int _nId, int _nMgcId)
{
	if(_nId < 0 ||
		_nMgcId < 0)
	{
		return NULL;
	}

	MagicObject* pRet = NULL;
	switch(_nMgcId)
	{
		//	根据魔法id返回不同的魔法对象
	case 1://	小火球
	case 5://	大火球
	case 12://	幽灵盾
	case 13://	神圣战甲术
	case 14://	灵魂火符
		{
			pRet = new MagicFireball(_nId, _nMgcId);
			if(pRet->Init())
			{
				return pRet;
			}
			else
			{
				delete pRet;
				return NULL;
			}
		}break;
	case 10://	爆裂火焰
	case 11://	雷电术
	case 15://	困魔咒
	case 18://	诱惑之光
	case 21://	爆裂火焰
	case 24://	群体治愈术
	case 44://	冰咆哮
	case 46://	圣言术
	case 47://	心灵启示
		{
			pRet = new MagicIceRoar(_nId, _nMgcId);
			if(pRet->Init())
			{
				return pRet;
			}
			else
			{
				delete pRet;
				return NULL;
			}
		}break;
	case 22://	地狱雷光
	case 19://	瞬息移动
		{
			pRet = new MagicHeal(_nId, _nMgcId);
			if(pRet->Init())
			{
				return pRet;
			}
			else
			{
				delete pRet;
				return NULL;
			}
		}break;
	case 20://	火墙
		{
			pRet = new MagicFireWall(_nId, _nMgcId);
			if(pRet->Init())
			{
				return pRet;
			}
			else
			{
				delete pRet;
				return NULL;
			}
		}break;
	default:
		{
			return NULL;
		}break;
	}
	return NULL;
}

/ ************************************************************************ /
/ * CreateStaticMagic
/ ************************************************************************ /
MagicObject* MagicSystem::CreateStaticMagic(int _nMgcId, GameObject* _pObj)
{
	MagicObject* pRet = NULL;

	switch (_nMgcId)
	{
	case 7://	攻杀
	case 16://	刺杀
	case 23://	半月
	case 25://	攻杀1
	case 26://	攻杀2
	case 27://	攻杀3
	case 28://	刺杀1
	case 29://	刺杀2
	case 30://	刺杀3
	case 31://	刺杀4
	case 32://	半月1
	case 33://	半月2
	case 34://	半月3
	case 35://	半月4
	case 36://	
	case 43://	烈火
		{
			//
			pRet = new MagicSwordKee(_nMgcId, _pObj);
			if(pRet->Init())
			{
				return pRet;
			}
			else
			{
				delete pRet;
				return NULL;
			}
		}break;
	case 45://	魔法盾
		{
			//
			pRet = new MagicShield(_nMgcId, _pObj);
			if(pRet->Init())
			{
				return pRet;
			}
			else
			{
				delete pRet;
				return NULL;
			}
		}break;
	}

	return NULL;
}

/ ************************************************************************ /
/ * PushMagic
/ ************************************************************************ /
void MagicSystem::PushMagic(MagicObject* _pMgc)
{
	m_xObjects.push_back(_pMgc);
}

/ ************************************************************************ /
/ * RemoveMagic
/ ************************************************************************ /
void MagicSystem::RemoveMagic(int _nId)
{
	MagicObject* pObj = NULL;
	MagicObjectList::iterator iter;
	for(iter = m_xObjects.begin();
		iter != m_xObjects.end();
		)
	{
		pObj = *iter;
		if(pObj->m_nId == _nId)
		{
			delete pObj;
			iter = m_xObjects.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

/ ************************************************************************ /
/ * Update
/ ************************************************************************ /
void MagicSystem::Update(float _dt)
{
	//
	MagicObject* pObj = NULL;
	MagicObjectList::iterator iter;
	for(iter = m_xObjects.begin();
		iter != m_xObjects.end();
		)
	{
		pObj = *iter;
		if(pObj->m_stInfo.end)
		{
			delete pObj;
			iter = m_xObjects.erase(iter);
		}
		else
		{
			pObj->Update(_dt);
			++iter;
		}
	}
}

/ ************************************************************************ /
/ * Render
/ ************************************************************************ /
void MagicSystem::Render()
{
	MagicObject* pObj = NULL;
	MagicObjectList::const_iterator iter;
	for(iter = m_xObjects.begin();
		iter != m_xObjects.end();
		++iter)
	{
		pObj = *iter;
		pObj->Render();
	}
}*/
