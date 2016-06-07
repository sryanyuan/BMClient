#include "GameRankDlg.h"
#include "../GameScene/GameResourceManager.h"
#include "../BackMir/BackMir.h"
//////////////////////////////////////////////////////////////////////////
#define BID_RANK_ALL		1
#define BID_RANK_ZHANSHI	2
#define BID_RANK_FASHI		3
#define BID_RANK_DAOSHI		4

#define SPLIT_POS_X			160
#define SPLIT_POS_Y			80

#define TITLE_POS_X			(SPLIT_POS_X + 40)
#define TITLE_POS_Y			(SPLIT_POS_Y + 20)

#define RANKLIST_POS_X		(SPLIT_POS_X + 10)
#define RANKLIST_POS_Y		(SPLIT_POS_Y + 40)

#define RANK_RANK_POS_X			44
#define RANK_NAME_POS_X			110
#define RANK_LEVEL_POS_X		176
#define RANK_JOB_POS_X			236
#define RANK_POWER_POS_X		296

#define RANK_ITEM_HEIGHT		33
#define RANK_ITEM_POS_Y			130

#define RANK_ITEM_POS_X			169
//////////////////////////////////////////////////////////////////////////
GameRankDlg::GameRankDlg()
{
	m_pRender = new hgeSprite(0, 0, 0, 0, 0);
	m_rcClient.right = 540;
	m_rcClient.bottom = 420 + 50;
	SetWindowTitle("排行榜");
	SetBkStype(1);
	CalcCloseButtonPosition();

	AddCommonButton(CBTN_OPTION, BID_RANK_ALL, 18, 85 + (36 + 2) * 0, "等级榜");
	AddCommonButton(CBTN_OPTION, BID_RANK_ZHANSHI, 18, 85 + (36 + 2) * 1, "圣战榜");
	AddCommonButton(CBTN_OPTION, BID_RANK_FASHI, 18, 85 + (36 + 2) * 2, "法神榜");
	AddCommonButton(CBTN_OPTION, BID_RANK_DAOSHI, 18, 85 + (36 + 2) * 3, "天尊榜");

	GetCommonButtonData(BID_RANK_ALL)->dwColor = ARGB(255, 185, 135, 41);
	GetCommonButtonData(BID_RANK_ALL)->dwOverColor = ARGB(255, 185, 135, 41);
	GetCommonButtonData(BID_RANK_ALL)->dwCheckedColor = ARGB(255, 245, 216, 17);
	GetCommonButtonData(BID_RANK_ZHANSHI)->dwColor = ARGB(255, 185, 135, 41);
	GetCommonButtonData(BID_RANK_ZHANSHI)->dwOverColor = ARGB(255, 185, 135, 41);
	GetCommonButtonData(BID_RANK_ZHANSHI)->dwCheckedColor = ARGB(255, 245, 216, 17);
	GetCommonButtonData(BID_RANK_FASHI)->dwColor = ARGB(255, 185, 135, 41);
	GetCommonButtonData(BID_RANK_FASHI)->dwOverColor = ARGB(255, 185, 135, 41);
	GetCommonButtonData(BID_RANK_FASHI)->dwCheckedColor = ARGB(255, 245, 216, 17);
	GetCommonButtonData(BID_RANK_DAOSHI)->dwColor = ARGB(255, 185, 135, 41);
	GetCommonButtonData(BID_RANK_DAOSHI)->dwOverColor = ARGB(255, 185, 135, 41);
	GetCommonButtonData(BID_RANK_DAOSHI)->dwCheckedColor = ARGB(255, 245, 216, 17);

	GetCommonButtonData(BID_RANK_ALL)->bState = BTN_STATE_CHECKED;
	m_nRankType = RANK_TYPE_LEVEL;
	SelectRank(RANK_TYPE_LEVEL);

	ClearRankItem(-1);
	CenterWindow();
	SetVisible(false);

#ifdef _DEBUG
	/*RankItem item;

	for(int i = 0; i < 9; ++i)
	{
		item.nJob = rand() % 3;
		item.nLevel = rand() % 70;
		item.nPower = 100 + rand() % 100;
		item.nUid = 1 + rand() % 19;
		strcpy(item.szName, "测试的");
		AddRankItem(RANK_TYPE_LEVEL, &item);

		item.nJob = rand() % 3;
		item.nLevel = rand() % 70;
		item.nPower = 100 + rand() % 100;
		item.nUid = 1 + rand() % 19;
		AddRankItem(RANK_TYPE_ZHANSHI, &item);

		item.nJob = rand() % 3;
		item.nLevel = rand() % 70;
		item.nPower = 100 + rand() % 100;
		item.nUid = 1 + rand() % 19;
		AddRankItem(RANK_TYPE_FASHI, &item);

		item.nJob = rand() % 3;
		item.nLevel = rand() % 70;
		item.nPower = 100 + rand() % 100;
		item.nUid = 1 + rand() % 19;
		AddRankItem(RANK_TYPE_DAOSHI, &item);
	}*/
#endif
}

GameRankDlg::~GameRankDlg()
{

}

bool GameRankDlg::OnShowDialog()
{
	//	request for rank list data
	PkgPlayerRankListReq req;
	g_xBuffer.Reset();
	g_xBuffer << req;
	SendBufferToGS(&g_xBuffer);

	return true;
}

bool GameRankDlg::OnCommonButtonClick(int _id)
{
	if(_id >= BID_RANK_ALL &&
		_id <= BID_RANK_DAOSHI)
	{
		SelectRank(_id - 1);
	}

	return true;
}

void GameRankDlg::Render()
{
	__super::Render();

	GameTextureManager* pPngMgr = GameResourceManager::GetInstance()->GetTexs(RES_CUSTOM);

	HTEXTURE tex = 0;

	//	splitters
	tex = pPngMgr->GetTexture(23);
	if(tex)
	{
		m_pRender->SetTexture(tex);
		m_pRender->SetTextureRect(0, 0, 8, 383);
		m_pRender->Render(RELATIVE_X(SPLIT_POS_X), RELATIVE_Y(SPLIT_POS_Y));
	}

	//	draw text
	AfxGetPrinter()->SetColor(ARGB(255, 185, 135, 41));
	AfxGetPrinter()->PrintWithoutStroke(RELATIVE_X(TITLE_POS_X), RELATIVE_Y(TITLE_POS_Y), "排名      角色名      等级      职业      战力");

	//	rank list bk
	tex = pPngMgr->GetTexture(24);
	if(tex)
	{
		m_pRender->SetTexture(tex);
		m_pRender->SetTextureRect(0, 0, 346, 328);
		m_pRender->Render(RELATIVE_X(RANKLIST_POS_X), RELATIVE_Y(RANKLIST_POS_Y));
	}

	//	draw rank list
	RankItemVector* pItems = NULL;
	if(m_nRankType == RANK_TYPE_LEVEL)
	{
		pItems = &m_xRankVecLevel;
	}
	else if(m_nRankType == RANK_TYPE_ZHANSHI)
	{
		pItems = &m_xRankVecZhanShi;
	}
	else if(m_nRankType == RANK_TYPE_FASHI)
	{
		pItems = &m_xRankVecFashi;
	}
	else if(m_nRankType == RANK_TYPE_DAOSHI)
	{
		pItems = &m_xRankVecDaoShi;
	}

	if(NULL != pItems)
	{
		int nSize = pItems->size();
		if(nSize > 10)
		{
			nSize = 10;
		}

		for(int i = 0; i < nSize; ++i)
		{
			RankItem refItem = (*pItems)[i];
			if(refItem.nUid == 0)
			{
				continue;
			}

			DWORD dwColor = ARGB_WHITE;
			if(i == 0)
			{
				dwColor = ARGB_YELLOW;
			}
			else if(i == 1)
			{
				dwColor = ARGB_RED;
			}
			else if(i == 2)
			{
				dwColor = ARGB_GREEN;
			}
			AfxGetPrinter()->SetColor(dwColor);

			int nRenderX = RANK_ITEM_POS_X;
			int nRenderY = RANK_ITEM_POS_Y + i * RANK_ITEM_HEIGHT;

			//	rank
			char szTextBuf[20] = {0};
			sprintf(szTextBuf, "%d", i+1);
			int nTextWidth = GetTextWidth(szTextBuf, 12);
			AfxGetPrinter()->PrintWithoutStroke(RELATIVE_X(nRenderX + RANK_RANK_POS_X - nTextWidth / 2), RELATIVE_Y(nRenderY), szTextBuf);

			//	name
			nTextWidth = GetTextWidth(refItem.szName, 12);
			AfxGetPrinter()->PrintWithoutStroke(RELATIVE_X(nRenderX + RANK_NAME_POS_X - nTextWidth / 2), RELATIVE_Y(nRenderY), refItem.szName);

			//	level
			sprintf(szTextBuf, "%d", refItem.nLevel);
			nTextWidth = GetTextWidth(szTextBuf, 12);
			AfxGetPrinter()->PrintWithoutStroke(RELATIVE_X(nRenderX + RANK_LEVEL_POS_X - nTextWidth / 2), RELATIVE_Y(nRenderY), szTextBuf);

			//	job
			const char* pszJob = "未知";
			if(0 == refItem.nJob)
			{
				pszJob = "战士";
			}
			else if(1 == refItem.nJob)
			{
				pszJob = "法师";
			}
			else if(2 == refItem.nJob)
			{
				pszJob = "道士";
			}
			nTextWidth = 24;
			AfxGetPrinter()->PrintWithoutStroke(RELATIVE_X(nRenderX + RANK_JOB_POS_X - nTextWidth / 2), RELATIVE_Y(nRenderY), pszJob);

			//	power
			sprintf(szTextBuf, "%d", refItem.nPower);
			nTextWidth = GetTextWidth(szTextBuf, 12);
			AfxGetPrinter()->PrintWithoutStroke(RELATIVE_X(nRenderX + RANK_POWER_POS_X - nTextWidth / 2), RELATIVE_Y(nRenderY), szTextBuf);
		}
	}
}

void GameRankDlg::SelectRank(int _nRankType)
{
	if(_nRankType < RANK_TYPE_LEVEL ||
		_nRankType > RANK_TYPE_DAOSHI)
	{
		return;
	}

	m_nRankType = _nRankType;
}

void GameRankDlg::AddRankItem(int _nRankType, const RankItem *_pItem)
{
	RankItemVector* pItems = NULL;
	if(_nRankType == RANK_TYPE_LEVEL)
	{
		pItems = &m_xRankVecLevel;
	}
	else if(_nRankType == RANK_TYPE_ZHANSHI)
	{
		pItems = &m_xRankVecZhanShi;
	}
	else if(_nRankType == RANK_TYPE_FASHI)
	{
		pItems = &m_xRankVecFashi;
	}
	else if(_nRankType == RANK_TYPE_DAOSHI)
	{
		pItems = &m_xRankVecDaoShi;
	}

	if(NULL == pItems)
	{
		return;
	}

	RankItemVector& refItems = *pItems;
	for(int i = 0; i < refItems.size(); ++i)
	{
		if(refItems[i].nUid == 0)
		{
			memcpy(&refItems[i], _pItem, sizeof(RankItem));
			break;
		}
	}
}

void GameRankDlg::ClearRankItem(int _nRankType)
{
	if(-1 == _nRankType ||
		_nRankType == RANK_TYPE_LEVEL)
	{
		m_xRankVecLevel.resize(10);
		for(int i = 0; i < 10; ++i)
		{
			m_xRankVecLevel[i].nUid = 0;
		}
	}

	if(-1 == _nRankType ||
		_nRankType == RANK_TYPE_ZHANSHI)
	{
		m_xRankVecZhanShi.resize(10);
		for(int i = 0; i < 10; ++i)
		{
			m_xRankVecZhanShi[i].nUid = 0;
		}
	}

	if(-1 == _nRankType ||
		_nRankType == RANK_TYPE_FASHI)
	{
		m_xRankVecFashi.resize(10);
		for(int i = 0; i < 10; ++i)
		{
			m_xRankVecFashi[i].nUid = 0;
		}
	}

	if(-1 == _nRankType ||
		_nRankType == RANK_TYPE_DAOSHI)
	{
		m_xRankVecDaoShi.resize(10);
		for(int i = 0; i < 10; ++i)
		{
			m_xRankVecDaoShi[i].nUid = 0;
		}
	}
}