#include "GameDonateDlg.h"
#include "GameDlgBase.h"
#include "../BackMir/BackMir.h"
//////////////////////////////////////////////////////////////////////////
static const int nDrawBeginX = 18;
static const int nDrawBeginY = 80;

#define HELP_DLG_WIDTH	280
#define HELP_DLG_HEIGHT	400
//////////////////////////////////////////////////////////////////////////
GameDonateDlg::GameDonateDlg()
{
	m_rcClient.left = (VIEW_WIDTH - HELP_DLG_WIDTH) / 2;
	m_rcClient.right = m_rcClient.left + HELP_DLG_WIDTH;
	m_rcClient.top = (VIEW_HEIGHT - HELP_DLG_HEIGHT) / 2;
	m_rcClient.bottom = m_rcClient.top + HELP_DLG_HEIGHT;

	CalcCloseButtonPosition();
	SetWindowTitle("捐赠信息");
	SetVisible(false);
	m_nHelpID = 0;

	SAFE_NEW(m_pRender, hgeSprite(0, 0, 0, 0, 0));
}

GameDonateDlg::~GameDonateDlg()
{
	SAFE_DELETE(m_pRender);
}

bool GameDonateDlg::ProcUserCmd(const POINT& _mp)
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
							CreateDonateDlg(m_nHelpID);
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

void GameDonateDlg::Render()
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

void GameDonateDlg::CreateDonateDlg(int _nDlgID)
{
	m_xCtx.Clear();

	lua_State* pState = pTheGame->GetLuaState();
	lua_getglobal(pState, "ShowDonate");
	tolua_pushusertype(pState, &m_xCtx, "InteractiveContextEx");
	lua_pushinteger(pState, _nDlgID);

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
		m_nHelpID = _nDlgID;
	}
}

void GameDonateDlg::OnNativeCommand(DWORD _dwCmd)
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
}

bool GameDonateDlg::OnShowDialog()
{
	SetNeedTopLevel(true);
	return true;
}