#include "GameQuestDlg.h"
#include "../BackMir/BackMir.h"
#include "../GameScene/GameResourceManager.h"
//////////////////////////////////////////////////////////////////////////
#define DLG_WIDTH	(450+10)
#define DLG_HEIGHT	350

#define QUESTSUMMARY_WIDTH	140
#define QUESTSUMMARY_HEIGHT	(DLG_HEIGHT - 70 - 10 - 6)
#define QUESTSUMMARY_LEFT	(10 + 10)
#define QUESTSUMMARY_TOP	(72 + 5 + 10)
#define QUESTSUMMARY_LINEGAP	14

#define QUESTCONTENT_LEFT	(150 + 10)

#define QUESTBTN_UPARROW_QUEST		0
#define QUESTBTN_DOWNARROW_QUEST	1
#define QUESTBTN_UPARROW_CONTENT	2
#define QUESTBTN_DOWNARROW_CONTENT	3

#define MAX_QUEST_LIST				15
//////////////////////////////////////////////////////////////////////////
GameQuestDlg::GameQuestDlg()
{
	m_rcClient.left = WINDOW_WIDTH / 2 - DLG_WIDTH / 2;
	m_rcClient.right = m_rcClient.left + DLG_WIDTH;
	m_rcClient.top = 50;
	m_rcClient.bottom = m_rcClient.top + DLG_HEIGHT;

	SetBkStype(1);
	CalcCloseButtonPosition();
	SetVisible(false);
	m_pRender = NEW_EMPTY_SPRITE;

	//AddCommonButton(CBTN_UPARROW, QUESTBTN_UPARROW_QUEST, QUESTSUMMARY_WIDTH - 12, QUESTSUMMARY_TOP, NULL);
	//AddCommonButton(CBTN_DOWNARROW, QUESTBTN_DOWNARROW_QUEST, QUESTSUMMARY_WIDTH - 12, DLG_HEIGHT - 38, NULL);
	AddCommonButton(CBTN_UPARROW, QUESTBTN_UPARROW_CONTENT, DLG_WIDTH - 30, QUESTSUMMARY_TOP, NULL);
	AddCommonButton(CBTN_DOWNARROW, QUESTBTN_DOWNARROW_CONTENT, DLG_WIDTH - 30, DLG_HEIGHT - 38, NULL);

	//AddCommonButton(CBTN_CHECKBOX, 4, QUESTSUMMARY_WIDTH + 50, QUESTSUMMARY_TOP + 100, "刀刀刺杀");

	SetWindowTitle("任务");
	SetWindowTip("[帮助]在任务列表中滚动鼠标中键可以快速选择任务");
	//m_nCurSelLine = 0;
	m_nShowIndex = m_nCursorIndex = 0;
}

GameQuestDlg::~GameQuestDlg()
{
	SAFE_DELETE(m_pRender);
}

bool GameQuestDlg::OnCommonButtonClick(int _id)
{
	return true;
}

void GameQuestDlg::Render()
{
	__super::Render();

	//	画边栏
	HTEXTURE tex = 0;
	hgeResourceManager* pResMgr = pTheGame->GetGameRes();
	GameTextureManager* pPngMgr = GameResourceManager::GetInstance()->GetTexs(RES_CUSTOM);

	tex = pPngMgr->GetTexture(23);
	if(tex)
	{
		m_pRender->SetTexture(tex);
		m_pRender->SetTextureRect(0, 0, 8, QUESTSUMMARY_HEIGHT-5);
		m_pRender->Render(RELATIVE_X(QUESTSUMMARY_WIDTH), RELATIVE_Y(72 + 10));
	}

	int nContentLine = 0;
	int nRenderX = 0;
	int nRenderY = 0;
	int nQuestIndex = 0;
	int nSelectIndex = m_nCursorIndex + m_nShowIndex;

	if(!m_xQuestInfoList.empty())
	{
		QuestInfoList::const_iterator begIter = m_xQuestInfoList.begin();
		QuestInfoList::const_iterator endIter = m_xQuestInfoList.end();

		for(int i = m_nShowIndex; i < m_nShowIndex + MAX_QUEST_LIST; ++i)
		{
			nQuestIndex = 0;
			for(begIter = m_xQuestInfoList.begin();
				begIter != endIter;
				++begIter)
			{
				if(nQuestIndex == i)
				{
					break;
				}
				++nQuestIndex;
			}

			if(nQuestIndex != i)
			{
				break;
			}
			if(begIter == endIter)
			{
				break;
			}

			const QuestInfo& refInfo = *begIter;

			//	主线 支线
			nRenderX = RELATIVE_X(QUESTSUMMARY_LEFT);
			nRenderY = RELATIVE_Y((i - m_nShowIndex) * QUESTSUMMARY_LINEGAP + QUESTSUMMARY_TOP);

			if(begIter->stSummary.nType == 0)
			{
				//	主线
				AfxGetPrinter()->SetColor(ARGB_YELLOW);
				AfxGetPrinter()->PrintWithoutStroke(nRenderX, nRenderY, "主线");
			}
			else
			{
				//	支线
				AfxGetPrinter()->SetColor(ARGB_GREEN);
				AfxGetPrinter()->PrintWithoutStroke(nRenderX, nRenderY, "支线");
			}

			if(nQuestIndex == nSelectIndex)
			{
				AfxGetPrinter()->SetColor(ARGB_WHITE);
			}
			else
			{
				AfxGetPrinter()->SetColor(ARGB(255, 100, 100, 100));
			}
			
			AfxGetPrinter()->PrintWithoutStroke(nRenderX + 12 * 2 + 5, nRenderY, begIter->stSummary.xQuestName.c_str());

			//	任务内容
			if(nQuestIndex == nSelectIndex)
			{
				const char* pszContent = NULL;
				nContentLine = 0;

				AfxGetPrinter()->SetColor(ARGB_RED);
				AfxGetPrinter()->PrintWithoutStroke(RELATIVE_X(QUESTCONTENT_LEFT), RELATIVE_Y(QUESTSUMMARY_TOP), "任务说明：");
				++nContentLine;
				if(!begIter->stTip.xDescript.empty())
				{
					pszContent = begIter->stTip.xDescript.c_str();
				}
				else
				{
					pszContent = "无";
				}
				float fPreGapY = AfxGetPrinter()->GetKerningHeight();
				AfxGetPrinter()->SetKerningHeight(1);
				AfxGetPrinter()->SetColor(ARGB_WHITE);
				nContentLine += AfxGetPrinter()->LineFeedRenderRetLine(RELATIVE_X(QUESTCONTENT_LEFT), RELATIVE_Y(QUESTSUMMARY_TOP) + nContentLine * 13, pszContent, 260, false);
				AfxGetPrinter()->SetKerningHeight(fPreGapY);
				++nContentLine;
				//
				AfxGetPrinter()->SetColor(ARGB_RED);
				AfxGetPrinter()->PrintWithoutStroke(RELATIVE_X(QUESTCONTENT_LEFT), RELATIVE_Y(QUESTSUMMARY_TOP) + nContentLine * 13, "任务要求：");
				++nContentLine;
				if(!begIter->stTip.xRequire.empty())
				{
					pszContent = begIter->stTip.xRequire.c_str();
				}
				else
				{
					pszContent = "无";
				}
				AfxGetPrinter()->SetKerningHeight(1);
				AfxGetPrinter()->SetColor(ARGB_WHITE);
				nContentLine += AfxGetPrinter()->LineFeedRenderRetLine(RELATIVE_X(QUESTCONTENT_LEFT), RELATIVE_Y(QUESTSUMMARY_TOP) + nContentLine * 13, pszContent, 260, false);
				AfxGetPrinter()->SetKerningHeight(fPreGapY);
				++nContentLine;
				//
				AfxGetPrinter()->SetColor(ARGB_RED);
				AfxGetPrinter()->PrintWithoutStroke(RELATIVE_X(QUESTCONTENT_LEFT), RELATIVE_Y(QUESTSUMMARY_TOP) + nContentLine * 13, "任务奖励：");
				++nContentLine;
				if(!begIter->stTip.xReward.empty())
				{
					pszContent = begIter->stTip.xReward.c_str();
				}
				else
				{
					pszContent = "无";
				}
				AfxGetPrinter()->SetKerningHeight(1);
				AfxGetPrinter()->SetColor(ARGB_WHITE);
				nContentLine += AfxGetPrinter()->LineFeedRenderRetLine(RELATIVE_X(QUESTCONTENT_LEFT), RELATIVE_Y(QUESTSUMMARY_TOP) + nContentLine * 13, pszContent, 260, false);
				AfxGetPrinter()->SetKerningHeight(fPreGapY);
				++nContentLine;
			}
		}
	}

	RenderWindowTip();
}

bool GameQuestDlg::ProcUserCmd(const POINT& _mp)
{
	POINT ptRelative = _mp;
	ptRelative.x -= m_rcClient.left;
	ptRelative.y -= m_rcClient.top;
	int nClickLine = 0;

	if(ptRelative.x >= QUESTSUMMARY_LEFT &&
		ptRelative.x <= QUESTSUMMARY_WIDTH - QUESTSUMMARY_LEFT &&
		ptRelative.y >= QUESTSUMMARY_TOP &&
		ptRelative.y <= QUESTSUMMARY_TOP + MAX_QUEST_LIST * 14)
	{
		if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
		{
			nClickLine = (ptRelative.y - QUESTSUMMARY_TOP) / 14;

			if(nClickLine + m_nShowIndex >= 0 &&
				nClickLine + m_nShowIndex < m_xQuestInfoList.size())
			{
				m_nCursorIndex = nClickLine;
			}
		}

		int nMidButtonScroll = AfxGetHge()->Input_GetMouseWheel();
		if(0 != nMidButtonScroll)
		{
			CursorScroll(nMidButtonScroll);
		}
	}
	return __super::ProcUserCmd(_mp);
}

QuestInfo* GameQuestDlg::PushQuestInfo()
{
	QuestInfo info;
	info.stSummary.nType = 0;
	m_xQuestInfoList.push_back(info);

	return &m_xQuestInfoList.back();
}

bool GameQuestDlg::OnShowDialog()
{
	m_xQuestInfoList.clear();

	lua_State* pState = pTheGame->GetLuaState();
	lua_getglobal(pState, "CreateQuestDlg");
	tolua_pushusertype(pState, this, "GameQuestDlg");
	tolua_pushusertype(pState, GamePlayer::GetInstance()->GetQuest(), "QuestContext");
	tolua_pushusertype(pState, GamePlayer::GetInstance(), "GamePlayer");

	int nRet = lua_pcall(pState, 3, 0, 0);
	if(nRet != 0)
	{
		AfxGetHge()->System_Log("Create quest dialog error : %s",
			lua_tostring(pState, -1));
		lua_pop(pState, 1);
		//return true;
	}
	else
	{
		//return true;
	}

	SetNeedTopLevel(true);
	return true;
}

bool GameQuestDlg::CursorScrollUp()
{
	if(0 == m_nCursorIndex)
	{
		if(m_nShowIndex > 0)
		{
			--m_nShowIndex;
		}
		else
		{
			//	到顶了
			return false;
		}
	}
	else
	{
		//	不在最顶部
		--m_nCursorIndex;
	}

	return true;
}

bool GameQuestDlg::CursorScrollDown()
{
	int nCurrentSel = m_nShowIndex + m_nCursorIndex;
	if(nCurrentSel >= m_xQuestInfoList.size() - 1)
	{
		//	到底部了
		return false;
	}
	else
	{
		//	还能往下
		++m_nCursorIndex;
		if(m_nCursorIndex >= MAX_QUEST_LIST)
		{
			m_nCursorIndex = MAX_QUEST_LIST - 1;
			++m_nShowIndex;
		}
	}

	return true;
}

bool GameQuestDlg::CursorScroll(int _nTimes)
{
	bool bScrolled = false;

	if(_nTimes > 0)
	{
		for(int i = 0; i < _nTimes; ++i)
		{
			if(CursorScrollUp())
			{
				bScrolled = true;
			}
		}
	}
	else
	{
		for(int i = 0; i < -_nTimes; ++i)
		{
			if(CursorScrollDown())
			{
				bScrolled = true;
			}
		}
	}

	return bScrolled;
}