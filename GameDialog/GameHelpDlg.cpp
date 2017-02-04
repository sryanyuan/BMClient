#include "GameHelpDlg.h"
#include "GameDlgBase.h"
#include <tolua++.h>
#include "GameChargeDlg.h"
#include "../BackMir/BackMir.h"
//////////////////////////////////////////////////////////////////////////
static const int nDrawBeginX = 18;
static const int nDrawBeginY = 80;

#define HELP_DLG_WIDTH	280
#define HELP_DLG_HEIGHT	400
//////////////////////////////////////////////////////////////////////////
GameHelpDlg::GameHelpDlg()
{
	m_rcClient.left = (VIEW_WIDTH - HELP_DLG_WIDTH) / 2;
	m_rcClient.right = m_rcClient.left + HELP_DLG_WIDTH;
	m_rcClient.top = (VIEW_HEIGHT - HELP_DLG_HEIGHT) / 2;
	m_rcClient.bottom = m_rcClient.top + HELP_DLG_HEIGHT;

	SetBkStype(1);
	CalcCloseButtonPosition();
	SetWindowTitle("游戏帮助");
	SetVisible(false);
	m_nHelpID = 0;

	SAFE_NEW(m_pRender, hgeSprite(0, 0, 0, 0, 0));
}

GameHelpDlg::~GameHelpDlg()
{
	SAFE_DELETE(m_pRender);
}

bool GameHelpDlg::ProcUserCmd(const POINT& _mp)
{
	__super::ProcUserCmd(_mp);

	if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
	{
		CONTEXTNODELIST xNodes = m_xCtx.GetNodes();
		POINT ptTest;
		int nIndex = 0;
		if(!xNodes.empty())
		{
			CONTEXTNODELIST::const_iterator begiter = xNodes.begin();
			CONTEXTNODELIST::const_iterator enditer = xNodes.end();
			ContextNode* pNode = NULL;

			for(;begiter != enditer; ++begiter)
			{
				pNode = *begiter;
				//	Zero is a string not a  button
				if(pNode->dwID != 0)
				{
					ptTest = _mp;
					ptTest.x -= m_rcClient.left;
					ptTest.x -= nDrawBeginX;
					ptTest.y -= m_rcClient.top;
					ptTest.y -= nDrawBeginY;
					if(::PtInRect(&pNode->rcLocate, ptTest))
					{
						//	Send the packet
						if(pNode->dwID == 1)
						{
							//	Just close the dialog
							SetVisible(false);
						}
						/*else if(pNode->dwID == 2)
						{
							//	Next page
							++m_nHelpID;
							CreateHelpDlg(m_nHelpID);
						}
						else if(pNode->dwID == 3)
						{
							//	Previous page
							--m_nHelpID;
							if(m_nHelpID < 0)
							{
								m_nHelpID = 0;
							}
							CreateHelpDlg(m_nHelpID);
						}*/
						else if(pNode->dwID >= 10000)
						{
							OnNativeCommand(pNode->dwID);
						}
						else
						{
							m_nHelpID = pNode->dwID;
							CreateHelpDlg(m_nHelpID);
						}
						break;
					}
					++nIndex;
				}
			}
		}
	}
	return true;
}

void GameHelpDlg::Render()
{
	__super::Render();

	CONTEXTNODELIST xNodes = m_xCtx.GetNodes();
	if(!xNodes.empty())
	{
		float fKerning = AfxGetPrinter()->GetKerningHeight();

		AfxGetPrinter()->SetKerningHeight(2.0f);
		CONTEXTNODELIST::const_iterator begiter = xNodes.begin();
		CONTEXTNODELIST::const_iterator enditer = xNodes.end();
		ContextNode* pNode = NULL;

		for(;begiter != enditer; ++begiter)
		{
			pNode = *begiter;
			if(pNode->dwID == 0)
			{
				//	Text
				AfxGetPrinter()->SetColor(ARGB_WHITE);
				AfxGetPrinter()->LineFeedRender(nDrawBeginX + RELATIVE_X(pNode->rcLocate.left),
					nDrawBeginY + RELATIVE_Y(pNode->rcLocate.top),
					pNode->szMsg,
					20 * 12, false);
			}
			else
			{
				AfxGetPrinter()->SetColor(ARGB_YELLOW);
				AfxGetPrinter()->Print(nDrawBeginX + RELATIVE_X(pNode->rcLocate.left),
					nDrawBeginY + RELATIVE_Y(pNode->rcLocate.top),
					pNode->szMsg);
				//	Draw the underline
				AfxGetHge()->Gfx_RenderLine(nDrawBeginX + RELATIVE_X((float)pNode->rcLocate.left),
					nDrawBeginY + RELATIVE_Y((float)pNode->rcLocate.bottom),
					nDrawBeginX + RELATIVE_X((float)pNode->rcLocate.right),
					nDrawBeginY + RELATIVE_Y((float)pNode->rcLocate.bottom), ARGB_YELLOW);
			}
		}
		AfxGetPrinter()->SetKerningHeight(fKerning);
	}
}

void GameHelpDlg::CreateHelpDlg(int _nHelpID)
{
	m_xCtx.Clear();

	lua_State* pState = pTheGame->GetLuaState();
	lua_getglobal(pState, "ShowHelp");
	tolua_pushusertype(pState, &m_xCtx, "InteractiveContextEx");
	lua_pushinteger(pState, _nHelpID);

	int nRet = lua_pcall(pState, 2, 0, 0);
	if(nRet != 0)
	{
		AfxGetHge()->System_Log("Show help dialog error : %s",
			lua_tostring(pState, -1));
		lua_pop(pState, 1);
		SetVisible(false);
	}
	else
	{
		SetVisible(true);
		m_nHelpID = _nHelpID;
	}
}

void GameHelpDlg::OnNativeCommand(DWORD _dwCmd)
{
	if(_dwCmd == 10000)
	{
		HWND hWnd = AfxGetHge()->System_GetState(HGE_HWND);
		char szURL[] = "http://sryanyuan.github.io/web/";
		ShellExecute(hWnd, "open", "iexplore.exe", szURL, "", SW_SHOW);
		//MessageBox(NULL, "支付宝账户为:sryan@qq.com,感谢您的捐助", "捐赠信息", MB_OK | MB_ICONINFORMATION);
	}
	else if(_dwCmd == 10001)
	{
		HWND hWnd = AfxGetHge()->System_GetState(HGE_HWND);
		char szURL[] = "http://tieba.baidu.com/f?kw=backmir&fr=index";
		ShellExecute(hWnd, "open", "iexplore.exe", szURL, "", SW_SHOW);
	}
	else if(_dwCmd >= 10002 &&
		_dwCmd <= 10004)
	{
		PkgPlayerDifficultyLevelReq req;
		req.cLevel = _dwCmd - 10002;
		if(req.cLevel == 0)
		{
			req.cLevel = kDifficultyNormal;
		}
		g_xBuffer.Reset();
		g_xBuffer << req;
		SendBufferToGS(&g_xBuffer);
	}
	else if (_dwCmd == 10010)
	{
		// 微信支付
		GameChargeDlg* pDlg = GameMainOptUI::GetInstance()->GetChargeDlg();
		pDlg->ShowDialog();
	}
	else if (_dwCmd == 10011)
	{
		// 支付宝支付
		GameChargeDlg* pDlg = GameMainOptUI::GetInstance()->GetChargeDlg();
		pDlg->ShowDialog();
	}
}

bool GameHelpDlg::OnShowDialog()
{
	SetNeedTopLevel(true);
	return true;
}