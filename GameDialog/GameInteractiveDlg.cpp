#include "GameInteractiveDlg.h"
#include "GameInfoBoardDlg.h"
#include "../Common/SKnl3Helper.h"
#include <assert.h>
#include "../BackMir/BackMir.h"
//////////////////////////////////////////////////////////////////////////
static const int nDrawBeginX = 24;
static const int nDrawBeginY = 80;
//////////////////////////////////////////////////////////////////////////

GameInteractiveDlg::GameInteractiveDlg()
{
	//m_pSpr = NULL;
	m_pRender = NEW_EMPTY_SPRITE;
	m_pRes = NULL;
	m_pFont = NULL;
	m_rcClient.right = 290;
	m_rcClient.bottom = 390;

	INIT_CLOSEBTN(264, 18);
	SetBkStype(1);
	CalcCloseButtonPosition();
	//m_nCreatorID = 0;
	//m_nButtonNum = 0;
	m_bVisible = false;

	SetWindowTitle("对话");
}

GameInteractiveDlg::~GameInteractiveDlg()
{
	//delete m_pSpr;
	SAFE_DELETE(m_pRender);
	delete m_pFont;
}

void GameInteractiveDlg::Init(hgeResourceManager* _res, GameMainOptUI* _parent)
{
	m_pRes = _res;
	m_pParent = _parent;
	m_pFont = new GfxFont("宋体", 12, 0, 0, 0);
//	m_pFont->SetLineFeedNumber(20);
	m_pFont->SetKerningHeight(2.0f);
}

void GameInteractiveDlg::Render()
{
	//	画背景
	/*if(!m_pRes)
	{
		return;
	}

	static const int nDrawBeginX = 24;

	HTEXTURE tex = 0;
	tex = m_pRes->GetTexture("dlg");
	if(tex)
	{
		INITSPRITE(m_pSpr, tex);
		m_pSpr->Render(m_rcClient.left, m_rcClient.top);
	}

	static const int nEachLineOffset = 15;
	//	标题
	int nLine = 0;
	m_pFont->SetColor(ARGB_GREEN);
	m_pFont->Print(RELATIVE_X(nDrawBeginX), RELATIVE_Y(60 + nLine * nEachLineOffset), m_context.GetTitle());
	++nLine;
	++nLine;

	//	内容
	const char* szText = m_context.GetText();
	//const char* szText = "hello";
	int nTextNum = strlen(szText);
	m_pFont->SetColor(ARGB_WHITE);
	m_pFont->PrintEx(RELATIVE_X(nDrawBeginX), RELATIVE_Y(60 + nLine * nEachLineOffset), szText);

	for(int i = 0; i < nTextNum / 20; ++i)
	{
		++nLine;
	}
	++nLine;
	++nLine;

	//	按钮
	int nOffsetX = 0;
	const char* szButton = NULL;
	m_pFont->SetColor(ARGB_YELLOW);
	for(int i = 0; i < m_nButtonNum; ++i)
	{
		szButton = m_context.GetButtonText(i);
		m_buttonArea[i].left = RELATIVE_X(nDrawBeginX + 8 * nOffsetX);
		m_buttonArea[i].right = m_buttonArea[i].left + FONT_WIDTH_WORD * (strlen(szButton) / 2);
		m_buttonArea[i].top = RELATIVE_Y(60 + nLine * nEachLineOffset);
		m_buttonArea[i].bottom = m_buttonArea[i].top + FONT_HEIGHT_WORD;
		m_pFont->Print(m_buttonArea[i].left, m_buttonArea[i].top, m_context.GetButtonText(i));
		nOffsetX += strlen(szButton);
	}*/

	/*HTEXTURE tex = 0;
	tex = m_pRes->GetTexture("dlg");
	if(tex)
	{
		INITSPRITE(m_pRender, tex);
		m_pRender->Render(m_rcClient.left, m_rcClient.top);
	}*/
	__super::Render();

	CONTEXTNODELIST xNodes = m_context.GetNodes();
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

	//__super::Render();
}

void GameInteractiveDlg::Update(float _dt)
{

}

bool GameInteractiveDlg::ProcUserCmd(const POINT& _mp)
{
	__super::ProcUserCmd(_mp);

	if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
	{
		CONTEXTNODELIST xNodes = m_context.GetNodes();
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
						else
						{
							PkgPlayerClickNPCReq req;
							req.dwButtonID = pNode->dwID;
							req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
							//req.uTargetId = m_nCreatorID;
							req.uTargetId = m_nHandlerID;
							g_xBuffer.Reset();
							g_xBuffer << req;
							SendBufferToGS(&g_xBuffer);
						}

						GameSoundManager::GetInstancePtr()->PlayGameSound(SDGAME_CLICKBUTTON);

						break;
					}
					++nIndex;
				}
			}
		}
	}
	return true;
}

bool GameInteractiveDlg::IsCaptionBar(int _x, int _y)
{
	return __super::IsCaptionBar(_x, _y);

	if(_x >= RELATIVE_X(0) &&
		_x < RELATIVE_X(290) &&
		_y >= RELATIVE_Y(0) &&
		_y < RELATIVE_Y(40))
	{
		//	关闭按钮
		/*if(_x >= RELATIVE_X(265) &&
			_x <= RELATIVE_X(279) &&
			_y >= RELATIVE_Y(18) &&
			_y <= RELATIVE_Y(33))
		{
			if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
			{
				m_bVisible = false;
			}
			return false;
		}
		else*/
		{
			return true;
		}
	}

	return false;
}

bool GameInteractiveDlg::ScriptCreateDlg(GameObject* _creator)
{
	/*int nRet = 0;
	m_context.Reset();
	ZeroMemory(m_buttonArea, sizeof(m_buttonArea));
	m_nCreatorID = _creator->GetAttrib()->id;

#ifdef _USE_SCRIPT_
	LuaFunction<void> funcSetContext = m_pParent->GetScript()->GetGlobal("SetContext");

	try
	{
		funcSetContext(m_nCreatorID);
	}
	catch(...)
	{
		AfxGetHge()->System_Log("脚本对话框生成失败:ID[%d]", m_nCreatorID);
	}
#endif
	
	m_context.Init();
	m_nButtonNum = m_context.GetButtonSum();
	m_bVisible = true;
	int nOffsetX = m_rcClient.left;
	int nOffsetY = m_rcClient.top;
	m_rcClient.left -= nOffsetX;
	m_rcClient.right -= nOffsetX;
	m_rcClient.top -= nOffsetY;
	m_rcClient.bottom -= nOffsetY;*/
	return true;
}
//////////////////////////////////////////////////////////////////////////
bool GameInteractiveDlg::Create(GameNPC* _pNpc, const InteractiveDialogItemList& _refItems)
{
	int nRet = 0;
	//m_nCreatorID = _npc;
	m_nHandlerID = _pNpc->GetHandlerID();
	m_nCreatorID = _pNpc->GetAttrib()->id;

	m_context.Clear();

	InteractiveDialogItemList::const_iterator begIter = _refItems.begin();
	for(begIter;
		begIter != _refItems.end();
		++begIter)
	{
		const InteractiveDialogItem& refItem = *begIter;
		m_context.AddContextNode(refItem.nX, refItem.nY, refItem.xText.c_str(), refItem.uId);
	}

	SetVisible(true);
	return true;
}
//////////////////////////////////////////////////////////////////////////
/*bool GameInteractiveDlg::Create(int _npc, int _id, int _step)
{
	int nRet = 0;
	m_nCreatorID = _npc;
#ifdef _DEBUG
	int nStackSize = lua_gettop(pTheGame->GetLuaState());
	AfxGetHge()->System_Log("Before call : %d", nStackSize);
#endif
	lua_getglobal(pTheGame->GetLuaState(), "OnSetDialogContext");
	//if(lua_isfunction(pTheGame->GetLuaState(), -1))
	{
		tolua_pushusertype(pTheGame->GetLuaState(), &m_context, "InteractiveContextEx");
		lua_pushinteger(pTheGame->GetLuaState(), _npc);
		lua_pushinteger(pTheGame->GetLuaState(), _id);
		lua_pushinteger(pTheGame->GetLuaState(), _step);
		nRet = lua_pcall(pTheGame->GetLuaState(), 4, 0, 0);
		if(0 != nRet)
		{
			AfxGetHge()->System_Log("Error on call %s, Expression: %s",
				__FUNCTION__, lua_tostring(pTheGame->GetLuaState(), -1));
			lua_pop(pTheGame->GetLuaState(), 1);
			SetVisible(false);
#ifdef _DEBUG
			nStackSize = lua_gettop(pTheGame->GetLuaState());
			AfxGetHge()->System_Log("After call : %d", nStackSize);
#endif
			return false;
		}
		else
		{
			SetVisible(true);
			return true;
		}
	}
	//else
	{
#ifdef _DEBUG
		//nStackSize = lua_gettop(pTheGame->GetLuaState());
		//AfxGetHge()->System_Log("After call : %d", nStackSize);
#endif
		//AfxGetHge()->System_Log("Can't get the OnSetDialogContext function");
		//return false;
	}
}*/
//////////////////////////////////////////////////////////////////////////
bool GameInteractiveDlg::Create(GameNPC* _pNpc, int _id, int _step)
{
	int nRet = 0;
	//m_nCreatorID = _npc;
	m_nHandlerID = _pNpc->GetHandlerID();
	m_nCreatorID = _pNpc->GetAttrib()->id;
#ifdef _DEBUG
	int nStackSize = lua_gettop(pTheGame->GetLuaState());
	AfxGetHge()->System_Log("Before call : %d", nStackSize);
#endif
	lua_getglobal(pTheGame->GetLuaState(), "OnSetDialogContext");
	//if(lua_isfunction(pTheGame->GetLuaState(), -1))
	{
		tolua_pushusertype(pTheGame->GetLuaState(), &m_context, "InteractiveContextEx");
		lua_pushinteger(pTheGame->GetLuaState(), _pNpc->GetAttrib()->id);
		lua_pushinteger(pTheGame->GetLuaState(), _id);
		lua_pushinteger(pTheGame->GetLuaState(), _step);
		nRet = lua_pcall(pTheGame->GetLuaState(), 4, 0, 0);
		if(0 != nRet)
		{
			AfxGetHge()->System_Log("Error on call %s, Expression: %s",
				__FUNCTION__, lua_tostring(pTheGame->GetLuaState(), -1));
			lua_pop(pTheGame->GetLuaState(), 1);
			SetVisible(false);
#ifdef _DEBUG
			nStackSize = lua_gettop(pTheGame->GetLuaState());
			AfxGetHge()->System_Log("After call : %d", nStackSize);
#endif
			return false;
		}
		else
		{
			SetVisible(true);
			return true;
		}
	}
	//else
	{
#ifdef _DEBUG
		//nStackSize = lua_gettop(pTheGame->GetLuaState());
		//AfxGetHge()->System_Log("After call : %d", nStackSize);
#endif
		//AfxGetHge()->System_Log("Can't get the OnSetDialogContext function");
		//return false;
	}
}





//	Private
#ifdef _USE_SCRIPT_
void GameInteractiveDlg::Script_RegisterMemberFunction()
{
	//	添加页数
	m_pParent->GetScript()->GetGlobals().RegisterDirect("AddPage", m_context, &InteractiveContext::AddPage);
	//	添加标题
	m_pParent->GetScript()->GetGlobals().RegisterDirect("AddHead", m_context, &InteractiveContext::AddHead);
	//	添加内容
	m_pParent->GetScript()->GetGlobals().RegisterDirect("AddText", m_context, &InteractiveContext::AddText);
	//	添加按钮
	m_pParent->GetScript()->GetGlobals().RegisterDirect("AddButton", m_context, &InteractiveContext::AddButton);
	//	重置数据
	m_pParent->GetScript()->GetGlobals().RegisterDirect("Reset", m_context, &InteractiveContext::Reset);
	//	初始化
	m_pParent->GetScript()->GetGlobals().RegisterDirect("Init", m_context, &InteractiveContext::Init);
}
#endif

void GameInteractiveDlg::OnButtonCmd(int _idx, int _cmd)
{
#ifdef _USE_SCRIPT_
	switch(_cmd)
	{
	case INTERACTIVE_BUTTON_CLOSE:
		{
			m_bVisible = false;
		}break;
	case INTERACTIVE_BUTTON_NEXTPAGE:
		{
			//	下一页
			m_context.NextPage();
		}break;
	case INTERACTIVE_BUTTON_PREPAGE:
		{
			//	上一页
			m_context.PrevPage();
		}break;
	default:
		{
			AfxGetHge()->System_Log("对话框按钮ID[%d]，交给脚本处理", _cmd);
			LuaFunction<void> funcDoButton = m_pParent->GetScript()->GetGlobal("DoButtonCmd");
			funcDoButton(m_nCreatorID, _cmd, m_pParent->GetQuestInfo().GetStage());
		}break;
	}

	m_nButtonNum = m_context.GetButtonSum();
#endif
}

bool GameInteractiveDlg::OnShowDialog()
{
	SetNeedTopLevel(true);
	return true;
}