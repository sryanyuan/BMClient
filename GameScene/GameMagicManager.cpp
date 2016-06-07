#include "GameMagicManager.h"
#include "GameInfoManager.h"
#include "GameResourceManager.h"
#include "GamePlayer.h"

bool GameMagicManager::InsertMagic(GameObject* _from, GameObject* _to, MagicInfo* _mgc, int _spd)
{
// 	MagicInfo info;
// 	info.sender = _from;
// 	info.receiver = _to;
// 	info.id = _mgcid;
// 	info.spd = _spd;
// 	info.curframe = 0;
// 	info.time = 0.0f;	

// 	 if(!GameInfoManager::GetInstance()->GetMgcInfo(_mgcid, &info))
// 	 {
// 		 return false;
// 	 }
// 	 else
	 {
		 if(_to == NULL)
		 {
			 float x,y;
			 AfxGetHge()->Input_GetMousePos(&x, &y);
			 _mgc->extrax = x;
			 _mgc->extray = y;
		 }
		 _mgc->sender = _from;
		 _mgc->receiver = _to;
		 _mgc->curframe = 0;
		 _mgc->time = 0.0f;
		 _mgc->spd = _spd;
		 _mgc->mgcx = _from->GetObjx();
		 _mgc->mgcy = _from->GetObjy();

		 if(_mgc->dist == 1)
		 {
			 _mgc->lock = 1;
		 }

		 for(int i = 0; i < 3; ++i)
		 {
			 if(i == 0)
			 {
				 if(_mgc->selftotal != 0)
				 {
					 _mgc->stage = MAGIC_SELF;
					 break;
				 }
			 }
			 else if(i == 1)
			 {
				 if(_mgc->waytotal != 0)
				 {
					 _mgc->stage = MAGIC_WAY;
					 break;
				 }
			 }
			 else if(i == 2)
			 {
				 if(_mgc->desttotal != 0)
				 {
					 _mgc->stage = MAGIC_DEST;
					 break;
				 }
			 }
		 }
		 m_mgc.push_back(*_mgc);
		 return true;
	 }
}

void GameMagicManager::Update(float _dt)
{
	for(MagicList::iterator iter = m_mgc.begin();
		iter != m_mgc.end();
		)
	{
// 		if(iter->dist == 1)
// 		{
// 			if(GamePlayer::GetInstance()->GetCurFrame() == 0 &&
// 				GamePlayer::GetInstance()->GetStatus() == PST_
// 			if(iter->lock)
// 			{
// 				break;
// 			}
// 		}
		iter->time += _dt;
		switch(iter->stage)
		{
		case MAGIC_SELF:
			{
				if(iter->dist == 0)
				{
					//	0为远程魔法
					iter->curframe = iter->time / (1.0f / (float)iter->spd);
					if(iter->curframe >= iter->selftotal)
					{
						iter->stage = MAGIC_WAY;
						iter->curframe = 0;
						iter->time = 0.0f;
					}
					++iter;
				}
				else
				{
					//	近程
					iter->curframe = iter->time / (1.0f / (float)iter->spd);
					if(iter->curframe >= iter->selftotal)
					{
						iter = m_mgc.erase(iter);
					}
				}
			}break;
		case MAGIC_WAY:
			{
				if(iter->waytotal == 0)
				{
					iter->stage = MAGIC_DEST;
					iter->curframe = 0;
					iter->time = 0.0f;
				}
				else
				{
					iter->curframe = iter->time / (1.0f / (float)iter->spd);
 					if(iter->curframe >= iter->waytotal)
 					{
 						iter->stage = MAGIC_DEST;
 						iter->curframe = 0;
 						iter->time = 0.0f;
 					}
				}
				++iter;
			}break;
		case MAGIC_DEST:
			{
				if(iter->desttotal == 0)
				{
					//	移出列表
					iter = m_mgc.erase(iter);
				}
				else
				{
					MagicInfo mi = *iter;
					iter->curframe = iter->time / (1.0f / (float)iter->spd);
					if(iter->curframe >= iter->desttotal)
					{
						iter = m_mgc.erase(iter);
					}
					else
					{
						++iter;
					}
				}
			}
		}
	}
}

void GameMagicManager::Render()
{
	GameTextureManager* ttex = NULL;
	HTEXTURE tex = 0;
	int nOffsetX = 0;
	int nOffsetY = 0;
	static const int nRenderX = VIEW_WIDTH / 2 - 16 - 5;
	static const int nRenderY = VIEW_HEIGHT / 2 - 12 - 5;

	for(MagicList::iterator iter = m_mgc.begin();
		iter != m_mgc.end();
		++iter)
	{
		ttex = GameResourceManager::GetInstance()->GetTexs(RES_MAGIC + iter->mgcfileindex);
		if(ttex)
		{
			switch (iter->stage)
			{
			case MAGIC_SELF:
				{
 					if(iter->selftotal == 0)
 					{
 						break;
 					}
					tex = ttex->GetTexture(iter->selfbegin + iter->curframe);
					if(tex)
					{
						nOffsetX = ttex->GetTextureOffsetX(tex);
						nOffsetY = ttex->GetTextureOffsetY(tex);
						if(!m_pSprMgc)
						{
							//m_pSprMgc = new hgeSprite(tex, 0, 0, ttex->GetTextureWidth(tex), ttex->GetTextureHeight(tex));
							m_pSprMgc = new hgeSprite(tex, 0, 0, ttex->GetTextureWidthEx(iter->selfbegin + iter->curframe), 
								ttex->GetTextureHeightEx(iter->selfbegin + iter->curframe));
							m_pSprMgc->SetBlendMode(BLEND_ALPHAADD | BLEND_NOZWRITE);
						}
						else
						{
							m_pSprMgc->SetTexture(tex);
							//m_pSprMgc->SetTextureRect(0, 0, ttex->GetTextureWidth(tex), ttex->GetTextureHeight(tex));
							m_pSprMgc->SetTextureRect(0, 0, ttex->GetTextureWidthEx(iter->selfbegin + iter->curframe), 
								ttex->GetTextureHeightEx(iter->selfbegin + iter->curframe));
						}
						m_pSprMgc->Render(nRenderX + nOffsetX, nRenderY + nOffsetY);
					}
				}break;
			case MAGIC_WAY:
				{
 					if(iter->waytotal == 0)
 					{
 						break;
 					}
					tex = ttex->GetTexture(iter->waybegin + iter->curframe);
					if(tex)
					{
						nOffsetX = ttex->GetTextureOffsetX(tex);
						nOffsetY = ttex->GetTextureOffsetY(tex);
						if(!m_pSprMgc)
						{
							m_pSprMgc = new hgeSprite(tex, 0, 0, ttex->GetTextureWidth(tex), ttex->GetTextureHeight(tex));
							m_pSprMgc->SetBlendMode(BLEND_ALPHAADD | BLEND_NOZWRITE);
						}
						else
						{
							m_pSprMgc->SetTexture(tex);
							m_pSprMgc->SetTextureRect(0, 0, ttex->GetTextureWidth(tex), ttex->GetTextureHeight(tex));
						}
						m_pSprMgc->Render(nRenderX + nOffsetX, nRenderY + nOffsetY);
					}
				}break;
			case MAGIC_DEST:
				{
 					if(iter->desttotal == 0)
 					{
 						break;
 					}
					tex = ttex->GetTexture(iter->destbegin + iter->curframe);
					if(tex)
					{
						nOffsetX = ttex->GetTextureOffsetX(tex);
						nOffsetY = ttex->GetTextureOffsetY(tex);
						if(!m_pSprMgc)
						{
							m_pSprMgc = new hgeSprite(tex, 0, 0, ttex->GetTextureWidth(tex), ttex->GetTextureHeight(tex));
							m_pSprMgc->SetBlendMode(BLEND_ALPHAADD | BLEND_NOZWRITE);
						}
						else
						{
							m_pSprMgc->SetTexture(tex);
							m_pSprMgc->SetTextureRect(0, 0, ttex->GetTextureWidth(tex), ttex->GetTextureHeight(tex));
						}
						float fRenderX = 0.0f;
						float fRenderY = 0.0f;
						if(iter->receiver != NULL)
						{
							fRenderX = iter->receiver->GetObjx() + nOffsetX - GamePlayer::GetInstance()->GetScreenOffsetX();
							fRenderY = iter->receiver->GetObjy() + nOffsetY - GamePlayer::GetInstance()->GetScreenOffsetY();
							m_pSprMgc->Render(iter->receiver->GetObjx() + nOffsetX - GamePlayer::GetInstance()->GetScreenOffsetX(),
								iter->receiver->GetObjy() + nOffsetY - GamePlayer::GetInstance()->GetScreenOffsetY());
						}
						else
						{
							fRenderX = iter->extrax;
							fRenderY = iter->extray;
							m_pSprMgc->Render(fRenderX + nOffsetX, fRenderY + nOffsetY);
						}
						
						
					}
				}break;
			}
		}
	}
}