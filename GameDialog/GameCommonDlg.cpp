#include "GameCommonDlg.h"
#include "GameDlgBase.h"
#include "../GameScene/GameScene.h"
#include "../BackMir/BackMir.h"
#include "../GameScene/GameResourceManager.h"
#include "../Common/gfx_utils.h"
//////////////////////////////////////////////////////////////////////////

GameCommonDlg::GameCommonDlg()
{
	m_bLButtonDown = false;
	m_ptOrigin.x = m_ptOrigin.y = 0;
	ZeroMemory(&m_rcCloseBtn, sizeof(RECT));
	m_bCloseState = BTN_STATE_NORMAL;
	m_nHotKey = 0;
	m_bESCClose = true;
	m_bCanDrag = true;
}

GameCommonDlg::~GameCommonDlg()
{

}

//////////////////////////////////////////////////////////////////////////
void GameCommonDlg::ResetStatus()
{
	m_bCloseState = BTN_STATE_NORMAL;
	COMMONBUTTONS::iterator begiter = m_xButtons.begin();
	for(begiter; begiter != m_xButtons.end();
		++begiter)
	{
		if(begiter->nType == CBTN_OPTION)
		{
			if(begiter->bState == BTN_STATE_CHECKED)
			{
				//	nothing
			}
			else
			{
				begiter->bState = BTN_STATE_NORMAL;
			}
		}
		else if(begiter->nType != CBTN_CHECKBOX)
		{
			begiter->bState = BTN_STATE_NORMAL;
		}
	}

	m_ptOrigin.x = m_ptOrigin.y = 0;
	m_bLButtonDown = false;
}
//////////////////////////////////////////////////////////////////////////
void GameCommonDlg::Render()
{
	if(m_rcCloseBtn.left == m_rcCloseBtn.right)
	{
		return;
	}

	HTEXTURE tex = 0;
	hgeResourceManager* pMgr = GameScene::sThis->GetMainOpt()->GetResourceMgr();
	if(NULL == pMgr)
	{
		return;
	}

	tex = pMgr->GetTexture("close");
	if(tex &&
		m_pRender)
	{
		m_pRender->SetTexture(tex);
		if(m_bCloseState == BTN_STATE_NORMAL)
		{
			m_pRender->SetTextureRect(0, 0, 16, 16);
		}
		else if(m_bCloseState == BTN_STATE_OVER)
		{
			m_pRender->SetTextureRect(0, 16, 16, 16);
		}
		else if(m_bCloseState == BTN_STATE_DOWN)
		{
			m_pRender->SetTextureRect(0, 16*2, 16, 16);
		}
		m_pRender->Render(RELATIVE_X(m_rcCloseBtn.left), RELATIVE_Y(m_rcCloseBtn.top));
	}


	//	Normal buttons
	HTEXTURE texNormalBtn = pMgr->GetTexture("cnbtn");
	HTEXTURE texLeftArrowBtn = pMgr->GetTexture("clabtn");
	HTEXTURE texRightArrowBtn = pMgr->GetTexture("crabtn");
	GameTextureManager* pTexMgr = GameResourceManager::GetInstance()->GetTexs(RES_OPUI);
	bool bCanRender = false;

	COMMONBUTTONS::iterator begiter = m_xButtons.begin();
	for(begiter; begiter != m_xButtons.end();
		++begiter)
	{
		bCanRender = false;

		if(begiter->nType == CBTN_NORMAL)
		{
			if(texNormalBtn != 0)
			{
				m_pRender->SetTexture(texNormalBtn);
				m_pRender->SetTextureRect(0, begiter->bState * 21, 64, 21);
				bCanRender = true;
			}
		}
		else if(begiter->nType == CBTN_LEFTARROW)
		{
			if(texLeftArrowBtn != 0)
			{
				m_pRender->SetTexture(texLeftArrowBtn);
				m_pRender->SetTextureRect(0, begiter->bState * 15, 12, 15);
				bCanRender = true;
			}
		}
		else if(begiter->nType == CBTN_RIGHTARROW)
		{
			if(texRightArrowBtn)
			{
				m_pRender->SetTexture(texRightArrowBtn);
				m_pRender->SetTextureRect(0, begiter->bState * 15, 12, 15);
				bCanRender = true;
			}
		}
		else if(begiter->nType == CBTN_CHECKBOX)
		{
			if(begiter->bState == BTN_STATE_UNCHECKED ||
				begiter->bState == BTN_STATE_CHECKED)
			{
				tex = pTexMgr->GetTexture(TEX_CHECKBOX + begiter->bState - BTN_STATE_UNCHECKED);
				if(tex)
				{
					m_pRender->SetTexture(tex);
					m_pRender->SetTextureRect(0, 0, 17, 17);
					bCanRender = true;
				}
			}
		}
		else if(begiter->nType == CBTN_UPARROW)
		{
			tex = 0;
			if(begiter->bState == BTN_STATE_OVER ||
				begiter->bState == BTN_STATE_NORMAL)
			{
				tex = pTexMgr->GetTexture(TEX_UPARROW);
				if(tex)
				{
					m_pRender->SetTexture(tex);
					m_pRender->SetTextureRect(0, 0, 14, 16);
					bCanRender = true;
				}
			}
			else if(begiter->bState == BTN_STATE_DOWN)
			{
				tex = pTexMgr->GetTexture(TEX_UPARROW + 1);
				if(tex)
				{
					m_pRender->SetTexture(tex);
					m_pRender->SetTextureRect(0, 0, 14, 16);
					bCanRender = true;
				}
			}
		}
		else if(begiter->nType == CBTN_DOWNARROW)
		{
			tex = 0;
			if(begiter->bState == BTN_STATE_OVER ||
				begiter->bState == BTN_STATE_NORMAL)
			{
				tex = pTexMgr->GetTexture(TEX_DOWNARROW);
				if(tex)
				{
					m_pRender->SetTexture(tex);
					m_pRender->SetTextureRect(0, 0, 14, 16);
					bCanRender = true;
				}
			}
			else if(begiter->bState == BTN_STATE_DOWN)
			{
				tex = pTexMgr->GetTexture(TEX_DOWNARROW + 1);
				if(tex)
				{
					m_pRender->SetTexture(tex);
					m_pRender->SetTextureRect(0, 0, 14, 16);
					bCanRender = true;
				}
			}
		}

		if(bCanRender)
		{
			m_pRender->Render(RELATIVE_X(begiter->stPos.left), RELATIVE_Y(begiter->stPos.top));

			if(strlen(begiter->szText))
			{
				if(begiter->nType == CBTN_CHECKBOX)
				{
					AfxGetPrinter()->SetColor(ARGB_WHITE);
					AfxGetPrinter()->Print(RELATIVE_X(begiter->stPos.left + 20), RELATIVE_Y(begiter->stPos.top + 3),
						begiter->szText);
				}
				else
				{
					int nWidth = GetTextWidth(begiter->szText, 12);
					AfxGetPrinter()->SetColor(ARGB_WHITE);
					AfxGetPrinter()->Print(RELATIVE_X(begiter->stPos.left + (75 - nWidth) / 2), RELATIVE_Y(begiter->stPos.top + 9),
						begiter->szText);
				}
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////
bool GameCommonDlg::ProcUserCmd(const POINT& _mp)
{
	m_bCloseState = BTN_STATE_NORMAL;
	//ResetStatus();
	RECT rcTest;
	COMMONBUTTONS::iterator begiter = m_xButtons.begin();
	for(begiter; begiter != m_xButtons.end();
		++begiter)
	{
		if(begiter->nType != CBTN_CHECKBOX)
		{
			begiter->bState = BTN_STATE_NORMAL;
		}
	}

	if(!IsCaptionBar(_mp.x, _mp.y) &&
		!m_bLButtonDown)
	{
		//m_bFocus = false;
		begiter = m_xButtons.begin();
		for(begiter; begiter != m_xButtons.end();
			++begiter)
		{
			//begiter->bState = BTN_STATE_NORMAL;

			rcTest.left = RELATIVE_X(begiter->stPos.left);
			rcTest.right = RELATIVE_X(begiter->stPos.right);
			rcTest.top = RELATIVE_Y(begiter->stPos.top);
			rcTest.bottom = RELATIVE_Y(begiter->stPos.bottom);
			if(PtInRect(&rcTest, _mp))
			{
				if(begiter->nType == CBTN_CHECKBOX)
				{
					if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
					{
						BYTE bState = begiter->bState;
						if(bState == BTN_STATE_UNCHECKED)
						{
							bState = BTN_STATE_CHECKED;
						}
						else
						{
							bState = BTN_STATE_UNCHECKED;
						}
						begiter->bState = bState;
						OnCommonButtonClick(begiter->nID);
					}
				}
				else
				{
					begiter->bState = BTN_STATE_OVER;

					if(AfxGetHge()->Input_GetKeyState(HGEK_LBUTTON))
					{
						begiter->bState = BTN_STATE_DOWN;
					}
					if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
					{
						begiter->bState = BTN_STATE_OVER;
						OnCommonButtonClick(begiter->nID);
					}
				}
			}
		}

		return false;
	}

	rcTest.left = RELATIVE_X(m_rcCloseBtn.left);
	rcTest.right = RELATIVE_X(m_rcCloseBtn.right);
	rcTest.top = RELATIVE_Y(m_rcCloseBtn.top);
	rcTest.bottom = RELATIVE_Y(m_rcCloseBtn.bottom);

	if(PtInRect(&rcTest, _mp))
	{
		m_bCloseState = BTN_STATE_OVER;
		if(AfxGetHge()->Input_GetKeyState(HGEK_LBUTTON))
		{
			m_bCloseState = BTN_STATE_DOWN;
		}

		if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
		{
			SetVisible(false);
		}

		return false;
	}

	//	以下为拖动代码
	if(AfxGetHge()->Input_KeyDown(HGEK_LBUTTON))
	{
		if(IsCanDrag())
		{
			m_ptOrigin.x = _mp.x;
			m_ptOrigin.y = _mp.y;
			m_bLButtonDown = true;
			SetNeedTopLevel(true);
			SetSwallowEvent(true);
		}
	}
	else if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
	{
		m_ptOrigin.x = m_ptOrigin.y = 0;
		m_bLButtonDown = false;

		if(m_bCloseState != BTN_STATE_NORMAL)
		{
			SetVisible(false);
		}
		//m_bFocus = false;
		SetSwallowEvent(false);
		return true;
	}

	
	if(m_bLButtonDown)
	{
		int nOffsetX = _mp.x - m_ptOrigin.x;
		int nOffsetY = _mp.y - m_ptOrigin.y;
		m_rcClient.left += nOffsetX;
		m_rcClient.right += nOffsetX;
		m_rcClient.top += nOffsetY;
		m_rcClient.bottom += nOffsetY;
		m_ptOrigin.x += nOffsetX;
		m_ptOrigin.y += nOffsetY;

		if(nOffsetX != 0 ||
			nOffsetY != 0)
		{
			OnMove(nOffsetX, nOffsetY);
		}
	}

	return true;
}

bool GameCommonDlg::IsCaptionBar(int _x, int _y)
{
	return false;
}

//////////////////////////////////////////////////////////////////////////
void GameCommonDlg::AddStaticTexture(int _id, int _x, int _y, int _nTexIndex, DWORD _dwColor /* = 0 */)
{
	StaticTexture stTexture;
	ZeroMemory(&stTexture, sizeof(StaticTexture));
	stTexture.nID = _id;
	stTexture.stPos.left = _x;
	stTexture.stPos.top = _y;
	stTexture.dwColor = _dwColor;
	stTexture.bVisible = true;
	stTexture.nTextureIndex = _nTexIndex;

	GameTextureManager* pTexMgr = GameResourceManager::GetInstance()->GetTexs(RES_CUSTOM);
	HTEXTURE tex = pTexMgr->GetTexture(_nTexIndex);
	if (NULL == tex)
	{
		return;
	}

	stTexture.stPos.right = pTexMgr->GetTextureWidth(_nTexIndex);
	stTexture.stPos.bottom = pTexMgr->GetTextureHeight(_nTexIndex);

	m_xTextures.push_back(stTexture);
}

void GameCommonDlg::AddCommonButton(int _type, int _id, int _x, int _y, const char* _pszText, int _nTexIndex /* = 0 */, DWORD _dwColor /* = 0 */)
{
	if(_type < CBTN_NORMAL ||
		_type > CBTN_CHECKBOX)
	{
		return;
	}
	CommonButton stButton;
	stButton.nTexIndex = _nTexIndex;
	stButton.bState = BTN_STATE_NORMAL;
	stButton.nType = _type;
	stButton.nID = _id;
	stButton.stPos.left = _x;
	stButton.stPos.top = _y;
	stButton.dwColor = _dwColor;
	stButton.bVisible = true;
	stButton.nGroup = 0;
	stButton.dwOverColor = stButton.dwDownColor = stButton.dwCheckedColor = 0;

	if(_type == CBTN_CHECKBOX)
	{
		stButton.bState = BTN_STATE_UNCHECKED;
	}

	if(_pszText == NULL)
	{
		stButton.szText[0] = 0;
	}
	else
	{
		if(strlen(_pszText) > 9)
		{
			return;
		}
		else
		{
			strcpy(stButton.szText, _pszText);
		}
	}
	if(_type == CBTN_NORMAL)
	{
		stButton.stPos.right = _x + 64;
		stButton.stPos.bottom = _y + 21;
	}
	else if(_type == CBTN_LEFTARROW ||
		_type == CBTN_RIGHTARROW)
	{
		stButton.stPos.right = _x + 12;
		stButton.stPos.bottom = _y + 15;
	}
	else if(_type == CBTN_CHECKBOX)
	{
		stButton.stPos.right = _x + 17;
		stButton.stPos.bottom = _y + 17;
	}
	else if(_type == CBTN_UPARROW ||
		_type == CBTN_DOWNARROW)
	{
		stButton.stPos.right = _x + 12;
		stButton.stPos.bottom = _y + 16;
	}
	m_xButtons.push_back(stButton);
}
//////////////////////////////////////////////////////////////////////////
void GameCommonDlg::RemoveCommonButton(int _id)
{
	int nSize = m_xButtons.size();
	if(nSize == 0)
	{
		return;
	}
	COMMONBUTTONS::iterator begiter = m_xButtons.begin();
	for(begiter; begiter != m_xButtons.end();
		++begiter)
	{
		if(begiter->nID == _id)
		{
			m_xButtons.erase(begiter);
			return;
		}
	}
}
//////////////////////////////////////////////////////////////////////////
void GameCommonDlg::RemoveStaticTexture(int _id)
{
	int nSize = m_xTextures.size();
	if(nSize == 0)
	{
		return;
	}
	STATICTEXTURES::iterator begiter = m_xTextures.begin();
	for(begiter; begiter != m_xTextures.end();
		++begiter)
	{
		if(begiter->nID == _id)
		{
			m_xTextures.erase(begiter);
			return;
		}
	}
}
//////////////////////////////////////////////////////////////////////////
void GameCommonDlg::MoveTo(int _x, int _y)
{
	int nOftX = _x - m_rcClient.left;
	int nOftY = _y - m_rcClient.top;

	m_rcClient.left += nOftX;
	m_rcClient.right += nOftX;
	m_rcClient.top += nOftY;
	m_rcClient.bottom += nOftY;

	if(nOftX != 0 ||
		nOftY != 0)
	{
		OnMove(nOftX, nOftY);
	}
}
//////////////////////////////////////////////////////////////////////////
void GameCommonDlg::Move(int _x, int _y)
{
	int nOftX = _x;
	int nOftY = _y;

	m_rcClient.left += nOftX;
	m_rcClient.right += nOftX;
	m_rcClient.top += nOftY;
	m_rcClient.bottom += nOftY;

	if(nOftX != 0 ||
		nOftY != 0)
	{
		OnMove(nOftX, nOftY);
	}
}
//////////////////////////////////////////////////////////////////////////
void GameCommonDlg::CenterWindow()
{
	int nCenterX = WINDOW_WIDTH / 2 - RECT_WIDTH(m_rcClient) / 2;
	int nCenterY = WINDOW_HEIGHT / 2 - RECT_HEIGHT(m_rcClient) / 2;
	MoveTo(nCenterX, nCenterY);
}
//////////////////////////////////////////////////////////////////////////
bool RenderDlgControl::ProcUserCmd(const POINT& _mp)
{
	return _InternalProcUserCmd(_mp);
	//	需要反着遍历 不然消息处理不对 在链表最后一个的 应该第一个处理
	POINT ptMouse = _mp;
	bool bProcessed = false;
	RenderObject* pObj = NULL;
	RenderObject* pTopLevel = NULL;

	for(OBJECTLIST::reverse_iterator iter = m_objList.rbegin(); iter != m_objList.rend(); ++iter)
	{
		if(m_pStaticDlg)
		{
			//	Just process this
			if(!m_pStaticDlg->IsVisible())
			{
				m_pStaticDlg = NULL;
			}
			else
			{
				if(m_pStaticDlg->IsPtInObject(_mp) &&
					!bProcessed)
				{
					m_pStaticDlg->ProcUserCmd(_mp);
				}
				//	有静态窗口 总是返回已处理 堵塞游戏
				bProcessed = true;
				break;
			}
		}

		pObj = *iter;
		//	If it is static dialog
		if(pObj->IsStatic() &&
			pObj->IsVisible())
		{
			m_pStaticDlg = pObj;
		}

		//	正常处理窗口
		if(!bProcessed)
		{
			if(pObj->IsVisible() &&
				pObj->IsPtInObject(ptMouse))
			{
				pObj->ProcUserCmd(_mp);
				bProcessed = true;
				/*if(pObj->IsNeedTopLevel())
				{
					pTopLevel = pObj;
				}*/
			}
			else if(pObj->IsVisible())
			{
				pObj->ResetStatus();
			}
		}
		else
		{
			if(pObj != m_pStaticDlg)
			{
				pObj->ResetStatus();
			}
		}

		if(pObj->IsVisible() &&
			pObj->IsNeedTopLevel())
		{
			pTopLevel = pObj;
		}
	}

	if(m_pStaticDlg)
	{
		BringToTop(m_pStaticDlg);
	}
	else
	{
		if(NULL != pTopLevel)
		{
			pTopLevel->SetNeedTopLevel(false);
			BringToTop(pTopLevel);
		}
	}
	return bProcessed;
}
//////////////////////////////////////////////////////////////////////////
bool RenderDlgControl::_InternalProcUserCmd(const POINT& _mp)
{
	//	需要反着遍历 不然消息处理不对 在链表最后一个的 应该第一个处理
	POINT ptMouse = _mp;
	bool bProcessed = false;
	RenderObject* pObj = NULL;
	RenderObject* pTopLevel = NULL;

	bool bSwallowEnable = true;
	if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
	{
		bSwallowEnable = false;
	}

	for(OBJECTLIST::reverse_iterator iter = m_objList.rbegin(); iter != m_objList.rend(); ++iter)
	{
		//	当鼠标按键按起来后 取消所有的吞噬消息窗口
		if(!bSwallowEnable)
		{
			m_pSwallowEventDlg = NULL;
		}

		//	优先处理静态窗口 堵塞整个处理
		if(m_pStaticDlg)
		{
			//	Just process this
			if(!m_pStaticDlg->IsVisible())
			{
				m_pStaticDlg = NULL;
			}
			else
			{
				if(/*m_pStaticDlg->IsPtInObject(_mp) &&*/
					!bProcessed)
				{
					m_pStaticDlg->ProcUserCmd(_mp);
				}
				//	有静态窗口 总是返回已处理 堵塞游戏
				bProcessed = true;
				break;
			}
		}

		//	然后只处理吞噬消息的窗口 拖拉的
		if(m_pSwallowEventDlg)
		{
			if(!m_pSwallowEventDlg->IsVisible())
			{
				m_pSwallowEventDlg->SetSwallowEvent(false);
				m_pSwallowEventDlg = NULL;
			}
			else
			{
				m_pSwallowEventDlg->ProcUserCmd(_mp);
				bProcessed = true;
				break;
			}
		}

		pObj = *iter;
		//	If it is static dialog
		if(pObj->IsStatic() &&
			pObj->IsVisible())
		{
			m_pStaticDlg = pObj;
		}

		if(!bSwallowEnable)
		{
			pObj->SetSwallowEvent(false);
		}

		if(pObj->IsSwallowEvent() &&
			pObj->IsVisible() &&
			bSwallowEnable)
		{
			m_pSwallowEventDlg = pObj;
		}

		if(!bProcessed)
		{
			if(pObj->IsVisible() &&
				pObj->IsPtInObject(ptMouse))
			{
				pObj->ProcUserCmd(_mp);
				bProcessed = true;
				/*if(pObj->IsNeedTopLevel())
				{
					pTopLevel = pObj;
				}*/
			}
			else if(pObj->IsVisible())
			{
				pObj->ResetStatus();
			}
		}
		else
		{
			if(pObj != m_pStaticDlg)
			{
				pObj->ResetStatus();
			}
		}

		if(pObj->IsVisible() &&
			pObj->IsNeedTopLevel())
		{
			pTopLevel = pObj;
		}
	}

	if(m_pStaticDlg)
	{
		BringToTop(m_pStaticDlg);
	}
	else
	{
		if(NULL != pTopLevel)
		{
			pTopLevel->SetNeedTopLevel(false);
			BringToTop(pTopLevel);
		}
	}

	return bProcessed;
}



//////////////////////////////////////////////////////////////////////////
GameNewDlg::GameNewDlg()
{
	m_nInfoWidth = m_nTitleWidth = 0;
	m_bCanMove = true;
	m_bCustomBk = false;
	m_nBkStyle = 0;
	m_nTipWidth = m_nTipLines = 0;
}

GameNewDlg::~GameNewDlg()
{

}

//////////////////////////////////////////////////////////////////////////
bool GameNewDlg::CalcCloseButtonPosition()
{
	if(m_rcClient.left == m_rcClient.right)
	{
		return false;
	}

	if(0 == m_nBkStyle)
	{
		m_rcCloseBtn.left = m_rcClient.right - m_rcClient.left - 43;
		m_rcCloseBtn.top = 0 + 9;
		m_rcCloseBtn.right = m_rcCloseBtn.left + 21;
		m_rcCloseBtn.bottom = 0 + 19 + 9;
	}
	else if(1 == m_nBkStyle)
	{
		m_rcCloseBtn.left = RECT_WIDTH(m_rcClient) - 39;
		m_rcCloseBtn.right = m_rcCloseBtn.left + 23;
		m_rcCloseBtn.top = 36;
		m_rcCloseBtn.bottom = m_rcCloseBtn.top + 23;
	}

	return true;
}
//////////////////////////////////////////////////////////////////////////
bool GameNewDlg::IsCaptionBar(int _x, int _y)
{
	if(!m_bCanMove)
	{
		return false;
	}

	if(m_nBkStyle == 0)
	{
		if(_y < RELATIVE_Y(30) &&
			_y > RELATIVE_Y(0))
		{
			return true;
		}
	}
	else
	{
		if(_y < RELATIVE_Y(59) &&
			_y > RELATIVE_Y(0))
		{
			return true;
		}
	}
	
	return false;
}
//////////////////////////////////////////////////////////////////////////
bool GameNewDlg::ProcUserCmd(const POINT& _mp)
{
	m_bCloseState = BTN_STATE_NORMAL;
	//ResetStatus();
	RECT rcTest;
	COMMONBUTTONS::iterator begiter = m_xButtons.begin();
	for(begiter; begiter != m_xButtons.end();
		++begiter)
	{
		if(begiter->nType == CBTN_OPTION)
		{
			if(begiter->bState == BTN_STATE_CHECKED)
			{
				//	nothing
			}
			else
			{
				begiter->bState = BTN_STATE_NORMAL;
			}
		}
		else
		{
			if(begiter->nType != CBTN_CHECKBOX)
			{
				begiter->bState = BTN_STATE_NORMAL;
			}
		}
	}

	if(!IsCaptionBar(_mp.x, _mp.y) &&
		!m_bLButtonDown)
	{
		//m_bFocus = false;
		begiter = m_xButtons.begin();
		for(begiter; begiter != m_xButtons.end();
			++begiter)
		{
			//begiter->bState = BTN_STATE_NORMAL;
			CommonButton& refButton = *begiter;

			rcTest.left = RELATIVE_X(begiter->stPos.left);
			rcTest.right = RELATIVE_X(begiter->stPos.right);
			rcTest.top = RELATIVE_Y(begiter->stPos.top);
			rcTest.bottom = RELATIVE_Y(begiter->stPos.bottom);
			if(refButton.bVisible &&
				PtInRect(&rcTest, _mp))
			{
				if(begiter->nType == CBTN_CHECKBOX)
				{
					if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
					{
						BYTE bState = begiter->bState;
						if(bState == BTN_STATE_UNCHECKED)
						{
							bState = BTN_STATE_CHECKED;
						}
						else
						{
							bState = BTN_STATE_UNCHECKED;
						}
						begiter->bState = bState;
						OnCommonButtonClick(begiter->nID);
					}
				}
				else if(refButton.nType == CBTN_OPTION)
				{
					if(refButton.bState != BTN_STATE_CHECKED)
					{
						refButton.bState = BTN_STATE_OVER;
					}

					if(AfxGetHge()->Input_GetKeyState(HGEK_LBUTTON))
					{
						refButton.bState = BTN_STATE_DOWN;
					}
					if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
					{
						OnCommonButtonClick(refButton.nID);
						OnOptionClicked(refButton.nID);
					}
				}
				else
				{
					begiter->bState = BTN_STATE_OVER;

					if(AfxGetHge()->Input_GetKeyState(HGEK_LBUTTON))
					{
						begiter->bState = BTN_STATE_DOWN;
					}
					if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
					{
						begiter->bState = BTN_STATE_OVER;
						OnCommonButtonClick(begiter->nID);
					}
				}
			}
		}

		return false;
	}

	
	rcTest.left = RELATIVE_X(m_rcCloseBtn.left);
	rcTest.right = RELATIVE_X(m_rcCloseBtn.right);
	rcTest.top = RELATIVE_Y(m_rcCloseBtn.top);
	rcTest.bottom = RELATIVE_Y(m_rcCloseBtn.bottom);

	if(PtInRect(&rcTest, _mp))
	{
		m_bCloseState = BTN_STATE_OVER;
		if(AfxGetHge()->Input_GetKeyState(HGEK_LBUTTON))
		{
			m_bCloseState = BTN_STATE_DOWN;
		}

		if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
		{
			if(OnCloseDialog())
			{
				SetVisible(false);
			}
		}
		return false;
	}
	

	//	以下为拖动代码
	/*if(!IsCanDrag())
	{
		return true;
	}*/

	if(AfxGetHge()->Input_KeyDown(HGEK_LBUTTON))
	{
		if(IsCanDrag())
		{
			m_ptOrigin.x = _mp.x;
			m_ptOrigin.y = _mp.y;
			m_bLButtonDown = true;
			SetSwallowEvent(true);
			SetNeedTopLevel(true);
		}
	}
	else if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
	{
		m_ptOrigin.x = m_ptOrigin.y = 0;
		if(m_bLButtonDown)
		{
			OnMoveStop();
		}
		m_bLButtonDown = false;

		if(m_bCloseState != BTN_STATE_NORMAL)
		{
			if(OnCloseDialog())
			{
				SetVisible(false);
			}
		}
		SetSwallowEvent(false);
		//m_bFocus = false;
		return true;
	}

	if(m_bLButtonDown)
	{
		int nOffsetX = _mp.x - m_ptOrigin.x;
		int nOffsetY = _mp.y - m_ptOrigin.y;
		m_rcClient.left += nOffsetX;
		m_rcClient.right += nOffsetX;
		m_rcClient.top += nOffsetY;
		m_rcClient.bottom += nOffsetY;
		m_ptOrigin.x += nOffsetX;
		m_ptOrigin.y += nOffsetY;

		if(nOffsetX != 0 ||
			nOffsetY != 0)
		{
			OnMove(nOffsetX, nOffsetY);
		}
	}

	return true;
}
//////////////////////////////////////////////////////////////////////////
void GameNewDlg::ShowDialog()
{
	if(m_bVisible)
	{
		return;
	}
	if(OnShowDialog())
	{
		m_bVisible = true;
	}
}
//////////////////////////////////////////////////////////////////////////
void GameNewDlg::CloseDialog()
{
	if(!m_bVisible)
	{
		return;
	}
	if(OnCloseDialog())
	{
		m_bVisible = false;
	}
}
//////////////////////////////////////////////////////////////////////////
void GameNewDlg::UpdateAlpha()
{
	DWORD dwColor = m_pRender->GetColor();
}
//////////////////////////////////////////////////////////////////////////
CommonButton* GameNewDlg::GetCommonButtonData(int _id)
{
	COMMONBUTTONS::iterator begIter = m_xButtons.begin();
	COMMONBUTTONS::const_iterator endIter = m_xButtons.end();

	for(begIter;
		begIter != endIter;
		++begIter)
	{
		if(begIter->nID == _id)
		{
			return &(*begIter);
		}
	}

	return NULL;
}
//////////////////////////////////////////////////////////////////////////
void GameNewDlg::AddCommonButton(int _type, int _id, int _x, int _y, const char* _pszText, int _nTexIndex /* = 0 */, DWORD _dwColor /* = 0 */)
{
	if(_type < CBTN_NORMAL ||
		_type > CBTN_OPTION)
	{
		return;
	}
	CommonButton stButton;
	stButton.bState = BTN_STATE_NORMAL;
	stButton.nType = _type;
	stButton.nID = _id;
	stButton.stPos.left = _x;
	stButton.stPos.top = _y;
	stButton.nTexIndex = _nTexIndex;
	stButton.dwColor = _dwColor;
	stButton.bVisible = true;
	stButton.nGroup = 0;
	stButton.dwOverColor = stButton.dwDownColor = stButton.dwCheckedColor = 0;

	if(_type == CBTN_CHECKBOX)
	{
		stButton.bState = BTN_STATE_UNCHECKED;
	}

	if(_pszText == NULL)
	{
		stButton.szText[0] = 0;
	}
	else
	{
		if(strlen(_pszText) >= sizeof(stButton.szText))
		{
			return;
		}
		else
		{
			strcpy(stButton.szText, _pszText);
		}
	}
	if(_type == CBTN_NORMAL)
	{
		stButton.stPos.right = _x + 75;
		stButton.stPos.bottom = _y + 30;

		if(stButton.nTexIndex != 0)
		{
			GameTextureManager* pPngMgr = GameResourceManager::GetInstance()->GetTexs(RES_CUSTOM);
			int nTexWidth = pPngMgr->GetTextureWidth(stButton.nTexIndex);
			int nTexHeight = pPngMgr->GetTextureHeight(stButton.nTexIndex);
			if(nTexHeight != 0 &&
				nTexWidth != 0)
			{
				stButton.stPos.right = _x + nTexWidth;
				stButton.stPos.bottom = _y + nTexHeight / 3;
			}
			else
			{
				stButton.nTexIndex = 0;
			}
		}
	}
	else if(_type == CBTN_OPTION)
	{
		stButton.stPos.right = _x + 138;
		stButton.stPos.bottom = _y + 36;

		if(stButton.nTexIndex != 0)
		{
			GameTextureManager* pPngMgr = GameResourceManager::GetInstance()->GetTexs(RES_CUSTOM);
			int nTexWidth = pPngMgr->GetTextureWidth(stButton.nTexIndex);
			int nTexHeight = pPngMgr->GetTextureHeight(stButton.nTexIndex);
			if(nTexHeight != 0 &&
				nTexWidth != 0)
			{
				stButton.stPos.right = _x + nTexWidth;
				stButton.stPos.bottom = _y + nTexHeight / 3;
			}
			else
			{
				stButton.nTexIndex = 0;
			}
		}
	}
	else if(_type == CBTN_LEFTARROW ||
		_type == CBTN_RIGHTARROW)
	{
		stButton.stPos.right = _x + 12;
		stButton.stPos.bottom = _y + 15;
	}
	else if(_type == CBTN_CHECKBOX)
	{
		stButton.stPos.right = _x + 17;
		stButton.stPos.bottom = _y + 17;
	}
	else if(_type == CBTN_UPARROW ||
		_type == CBTN_DOWNARROW)
	{
		stButton.stPos.right = _x + 12;
		stButton.stPos.bottom = _y + 16;
	}

	m_xButtons.push_back(stButton);
}
//////////////////////////////////////////////////////////////////////////
void GameNewDlg::SetCommonButtonState(int _id, int _state)
{
	COMMONBUTTONS::iterator begiter = m_xButtons.begin();
	for(begiter; begiter != m_xButtons.end();
		)
	{
		if(_id < 0)
		{
			begiter->bState = _state;
			++begiter;
		}
		else
		{
			if(_id == begiter->nID)
			{
				begiter->bState = _state;
				return;
			}
			else
			{
				++begiter;
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////
void GameNewDlg::OnOptionClicked(int _nId)
{
	CommonButton* pBtn = GetCommonButtonData(_nId);
	if(NULL == pBtn)
	{
		return;
	}
	if(pBtn->nType != CBTN_OPTION)
	{
		return;
	}

	COMMONBUTTONS::iterator begiter = m_xButtons.begin();
	for(begiter; begiter != m_xButtons.end();
		++begiter)
	{
		CommonButton& refButton = *begiter;

		if(refButton.nType == CBTN_OPTION &&
			refButton.nGroup == pBtn->nGroup)
		{
			refButton.bState = BTN_STATE_NORMAL;
		}
	}
	pBtn->bState = BTN_STATE_CHECKED;
}
//////////////////////////////////////////////////////////////////////////
void GameNewDlg::RenderBkStyle1()
{
	if(m_rcClient.left == m_rcClient.right)
	{
		return;
	}

	HTEXTURE tex = 0;
	GameTextureManager* pRes = GameResourceManager::GetInstance()->GetTexs(RES_CUSTOM);
	tex = pRes->GetTexture(17);

	//	render bk
	if(tex)
	{
		RECT rc9Path = {0};
		rc9Path.left = 132;
		rc9Path.right = 132;
		rc9Path.top = 100;
		rc9Path.bottom = 50;

		RECT rcBk = {0};
		rcBk.right = 308;
		rcBk.bottom = 224;

		Gfx_SetRenderState(kGfx_9Path_CenterStretch, 0);
		Gfx_Render9Path(m_pRender, tex, &rcBk, &rc9Path, m_rcClient.left, m_rcClient.top, RECT_WIDTH(m_rcClient), RECT_HEIGHT(m_rcClient));
	}

	//	close button
	tex = pRes->GetTexture(18);
	bool bCanRender = false;
	if(tex &&
		m_pRender)
	{
		m_pRender->SetTexture(tex);
		if(m_bCloseState == BTN_STATE_NORMAL)
		{
			//m_pRender->SetTextureRect(0, 0, 21, 19);
		}
		else if(m_bCloseState == BTN_STATE_OVER)
		{
			m_pRender->SetTextureRect(0, 23 * 1, 23, 23);
			bCanRender = true;
		}
		else if(m_bCloseState == BTN_STATE_DOWN)
		{
			m_pRender->SetTextureRect(0, 23 * 2, 23, 23);
			bCanRender = true;
		}
		if(bCanRender)
		{
			m_pRender->Render(RELATIVE_X(m_rcCloseBtn.left), RELATIVE_Y(m_rcCloseBtn.top));
		}
	}

	//	Title
	if(!m_xTitle.empty())
	{
		int nTextWidth = (m_rcClient.right - m_rcClient.left - m_nTitleWidth) / 2;
		AfxGetPrinter()->SetColor(ARGB_WHITE);
		AfxGetPrinter()->Print(RELATIVE_X(nTextWidth), RELATIVE_Y(12+8), m_xTitle.c_str());
	}
	//	Info
	if(!m_xInfo.empty())
	{
		int nTextWidth = (m_rcClient.right - m_rcClient.left - m_nInfoWidth) / 2;
		AfxGetPrinter()->SetColor(ARGB_WHITE);
		AfxGetPrinter()->Print(RELATIVE_X(nTextWidth), RELATIVE_Y(43+12), m_xInfo.c_str());
	}
}
//////////////////////////////////////////////////////////////////////////
void GameNewDlg::RenderBkStyle0()
{
	if(m_rcClient.left == m_rcClient.right)
	{
		return;
	}

	//hgeResourceManager* pMgr = GameScene::sThis->GetMainOpt()->GetResourceMgr();
	hgeResourceManager* pMgr = pTheGame->GetGameRes();
	if(NULL == pMgr)
	{
		return;
	}
	if(NULL == m_pRender)
	{
		return;
	}

	bool bCanRender = false;
	HTEXTURE tex = 0;

	//	Draw title
	tex = pMgr->GetTexture("ndlglt");
	if(tex)
	{
		m_pRender->SetTexture(tex);
		m_pRender->SetTextureRect(0, 0, 97, 72);
		m_pRender->Render(m_rcClient.left, m_rcClient.top);
	}
	//	Title border
	int nTileBorderWidth = (int)m_rcClient.right - (int)m_rcClient.left - 97 - 96;
	tex = pMgr->GetTexture("ndlgct");
	if(tex &&
		nTileBorderWidth > 0)
	{
		m_pRender->SetTexture(tex);
		m_pRender->SetTextureRect(0, 0, nTileBorderWidth, 69);
		m_pRender->Render(m_rcClient.left + 97, m_rcClient.top + 3);
	}
	tex = pMgr->GetTexture("ndlgrt");
	if(tex &&
		nTileBorderWidth > 0)
	{
		m_pRender->SetTexture(tex);
		m_pRender->SetTextureRect(0, 0, 96, 73);
		m_pRender->Render(m_rcClient.left + 97 + nTileBorderWidth, m_rcClient.top);
	}

	//	Bk
	tex = pMgr->GetTexture("ndlgbk");
	if(tex)
	{
		m_pRender->SetTexture(tex);
		m_pRender->SetTextureRect(0, 0, m_rcClient.right - m_rcClient.left - 18, m_rcClient.bottom - m_rcClient.top - 72 - 16 + 1);
		m_pRender->Render(m_rcClient.left + 9, m_rcClient.top + 72);
	}

	//	Bord
	tex = pMgr->GetTexture("ndlglb");
	if(tex)
	{
		m_pRender->SetTexture(tex);
		m_pRender->SetTextureRect(0, 0, 10, m_rcClient.bottom - m_rcClient.top - 72 - 16);
		m_pRender->Render(m_rcClient.left, m_rcClient.top + 72);
	}
	tex = pMgr->GetTexture("ndlgrb");
	if(tex)
	{
		m_pRender->SetTexture(tex);
		m_pRender->SetTextureRect(0, 0, 10, m_rcClient.bottom - m_rcClient.top - 72 - 16);
		m_pRender->Render(m_rcClient.right - 10, m_rcClient.top + 72);
	}

	//	Down border
	tex = pMgr->GetTexture("ndlglbn");
	if(tex)
	{
		m_pRender->SetTexture(tex);
		m_pRender->SetTextureRect(0, 0, 55, 50);
		m_pRender->Render(m_rcClient.left, m_rcClient.bottom - 50);
	}
	tex = pMgr->GetTexture("ndlgrbn");
	if(tex)
	{
		m_pRender->SetTexture(tex);
		m_pRender->SetTextureRect(0, 0, 54, 50);
		m_pRender->Render(m_rcClient.right - 54, m_rcClient.bottom - 50);
	}
	tex = pMgr->GetTexture("ndlgdb");
	if(tex)
	{
		m_pRender->SetTexture(tex);
		m_pRender->SetTextureRect(0, 0, m_rcClient.right - m_rcClient.left - 54 - 55 + 6, 12);
		m_pRender->Render(m_rcClient.left + 50, m_rcClient.bottom - 15);
	}

	tex = pMgr->GetTexture("ndlgclose");
	if(tex &&
		m_pRender)
	{
		m_pRender->SetTexture(tex);
		if(m_bCloseState == BTN_STATE_NORMAL)
		{
			//m_pRender->SetTextureRect(0, 0, 21, 19);
		}
		else if(m_bCloseState == BTN_STATE_OVER)
		{
			m_pRender->SetTextureRect(0, 0, 21, 19);
			bCanRender = true;
		}
		else if(m_bCloseState == BTN_STATE_DOWN)
		{
			m_pRender->SetTextureRect(0, 19 * 1, 21, 19);
			bCanRender = true;
		}
		if(bCanRender)
		{
			m_pRender->Render(RELATIVE_X(m_rcCloseBtn.left), RELATIVE_Y(m_rcCloseBtn.top));
		}
	}

	//	Title
	if(!m_xTitle.empty())
	{
		int nTextWidth = (m_rcClient.right - m_rcClient.left - m_nTitleWidth) / 2;
		AfxGetPrinter()->SetColor(ARGB_WHITE);
		AfxGetPrinter()->Print(RELATIVE_X(nTextWidth), RELATIVE_Y(12), m_xTitle.c_str());
	}
	//	Info
	if(!m_xInfo.empty())
	{
		int nTextWidth = (m_rcClient.right - m_rcClient.left - m_nInfoWidth) / 2;
		AfxGetPrinter()->SetColor(ARGB_WHITE);
		AfxGetPrinter()->Print(RELATIVE_X(nTextWidth), RELATIVE_Y(43), m_xInfo.c_str());
	}
}
//////////////////////////////////////////////////////////////////////////
void GameNewDlg::Render()
{
	if(m_rcClient.left == m_rcClient.right)
	{
		return;
	}

	//hgeResourceManager* pMgr = GameScene::sThis->GetMainOpt()->GetResourceMgr();
	hgeResourceManager* pMgr = pTheGame->GetGameRes();
	if(NULL == pMgr)
	{
		return;
	}
	if(NULL == m_pRender)
	{
		return;
	}

	bool bCanRender = false;
	HTEXTURE tex = 0;

	if(!m_bCustomBk)
	{
		if(m_nBkStyle == 0)
		{
			RenderBkStyle0();
		}
		else if(m_nBkStyle == 1)
		{
			RenderBkStyle1();
		}
	}

	GameTextureManager* pTexMgr = GameResourceManager::GetInstance()->GetTexs(RES_OPUI);
	GameTextureManager* pPngMgr = GameResourceManager::GetInstance()->GetTexs(RES_CUSTOM);

	//	static textures
	STATICTEXTURES::iterator begIter = m_xTextures.begin();
	for(begIter;
		begIter != m_xTextures.end();
		++begIter)
	{
		StaticTexture& stTexture = *begIter;

		if(stTexture.nTextureIndex != 0)
		{
			tex = pPngMgr->GetTexture(stTexture.nTextureIndex);
			if(NULL != tex)
			{
				m_pRender->SetTexture(tex);

				DWORD dwPrevColor = m_pRender->GetColor();

				if(stTexture.dwColor != 0)
				{
					m_pRender->SetColor(stTexture.dwColor);
				}

				m_pRender->SetTextureRect(0, 0, stTexture.stPos.right, stTexture.stPos.bottom);
				m_pRender->Render(RELATIVE_X(stTexture.stPos.left), RELATIVE_Y(stTexture.stPos.top));
				
				m_pRender->SetColor(dwPrevColor);
			}
		}
	}

	//	Normal buttons
	//HTEXTURE texNormalBtn = pMgr->GetTexture("cnbtn");
	HTEXTURE texNormalBtn = pMgr->GetTexture("sysbtn");
	HTEXTURE texLeftArrowBtn = pMgr->GetTexture("clabtn");
	HTEXTURE texRightArrowBtn = pMgr->GetTexture("crabtn");
	HTEXTURE texOptinBtn = pPngMgr->GetTexture(22);
	bCanRender = false;

	COMMONBUTTONS::iterator begiter = m_xButtons.begin();
	for(begiter; begiter != m_xButtons.end();
		++begiter)
	{
		bCanRender = false;

		CommonButton& xButton = *begiter;

		if(!xButton.bVisible)
		{
			continue;
		}

		if(begiter->nType == CBTN_NORMAL)
		{
			HTEXTURE texButton = texNormalBtn;

			if(xButton.nTexIndex != 0)
			{
				texButton = pPngMgr->GetTexture(xButton.nTexIndex);
			}

			if(0 == texButton)
			{
				texButton = texNormalBtn;
			}
			if(texButton != 0)
			{
				m_pRender->SetTexture(texButton);
				m_pRender->SetTextureRect(0, begiter->bState * RECT_HEIGHT(xButton.stPos), RECT_WIDTH(xButton.stPos), RECT_HEIGHT(xButton.stPos));
				bCanRender = true;
			}
		}
		else if(xButton.nType == CBTN_OPTION)
		{
			HTEXTURE texButton = texOptinBtn;

			if(xButton.nTexIndex != 0)
			{
				texButton = pPngMgr->GetTexture(xButton.nTexIndex);
			}
			if(0 == texButton)
			{
				texButton = texOptinBtn;
			}
			if(texButton != 0)
			{
				if(xButton.bState == BTN_STATE_CHECKED)
				{
					m_pRender->SetTexture(texButton);
					m_pRender->SetTextureRect(0, 2 * RECT_HEIGHT(xButton.stPos), RECT_WIDTH(xButton.stPos), RECT_HEIGHT(xButton.stPos));
					bCanRender = true;
				}
				else
				{
					m_pRender->SetTexture(texButton);
					m_pRender->SetTextureRect(0, begiter->bState * RECT_HEIGHT(xButton.stPos), RECT_WIDTH(xButton.stPos), RECT_HEIGHT(xButton.stPos));
					bCanRender = true;
				}
			}
		}
		else if(begiter->nType == CBTN_LEFTARROW)
		{
			if(texLeftArrowBtn != 0)
			{
				m_pRender->SetTexture(texLeftArrowBtn);
				m_pRender->SetTextureRect(0, begiter->bState * 15, 12, 15);
				bCanRender = true;
			}
		}
		else if(begiter->nType == CBTN_RIGHTARROW)
		{
			if(texRightArrowBtn)
			{
				m_pRender->SetTexture(texRightArrowBtn);
				m_pRender->SetTextureRect(0, begiter->bState * 15, 12, 15);
				bCanRender = true;
			}
		}
		else if(begiter->nType == CBTN_CHECKBOX)
		{
			if(begiter->bState == BTN_STATE_UNCHECKED ||
				begiter->bState == BTN_STATE_CHECKED)
			{
				tex = pTexMgr->GetTexture(TEX_CHECKBOX + begiter->bState - BTN_STATE_UNCHECKED);
				if(tex)
				{
					m_pRender->SetTexture(tex);
					m_pRender->SetTextureRect(0, 0, 17, 17);
					bCanRender = true;
				}
			}
		}
		else if(begiter->nType == CBTN_UPARROW)
		{
			tex = 0;
			if(begiter->bState == BTN_STATE_OVER ||
				begiter->bState == BTN_STATE_NORMAL)
			{
				tex = pTexMgr->GetTexture(TEX_UPARROW);
				if(tex)
				{
					m_pRender->SetTexture(tex);
					m_pRender->SetTextureRect(0, 0, 14, 16);
					bCanRender = true;
				}
			}
			else if(begiter->bState == BTN_STATE_DOWN)
			{
				tex = pTexMgr->GetTexture(TEX_UPARROW + 1);
				if(tex)
				{
					m_pRender->SetTexture(tex);
					m_pRender->SetTextureRect(0, 0, 14, 16);
					bCanRender = true;
				}
			}
		}
		else if(begiter->nType == CBTN_DOWNARROW)
		{
			tex = 0;
			if(begiter->bState == BTN_STATE_OVER ||
				begiter->bState == BTN_STATE_NORMAL)
			{
				tex = pTexMgr->GetTexture(TEX_DOWNARROW);
				if(tex)
				{
					m_pRender->SetTexture(tex);
					m_pRender->SetTextureRect(0, 0, 14, 16);
					bCanRender = true;
				}
			}
			else if(begiter->bState == BTN_STATE_DOWN)
			{
				tex = pTexMgr->GetTexture(TEX_DOWNARROW + 1);
				if(tex)
				{
					m_pRender->SetTexture(tex);
					m_pRender->SetTextureRect(0, 0, 14, 16);
					bCanRender = true;
				}
			}
		}

		if(bCanRender)
		{
			m_pRender->Render(RELATIVE_X(begiter->stPos.left), RELATIVE_Y(begiter->stPos.top));

			if(strlen(begiter->szText))
			{
				if(begiter->nType == CBTN_CHECKBOX)
				{
					AfxGetPrinter()->SetColor(ARGB_WHITE);
					AfxGetPrinter()->Print(RELATIVE_X(begiter->stPos.left + 20), RELATIVE_Y(begiter->stPos.top + 3),
						begiter->szText);
				}
				else
				{//171,128,25
					int nWidth = GetTextWidth(begiter->szText, 12);

					if(xButton.bState == BTN_STATE_NORMAL)
					{
						if(xButton.dwColor == 0)
						{
							AfxGetPrinter()->SetColor(ARGB_WHITE);
						}
						else
						{
							AfxGetPrinter()->SetColor(xButton.dwColor);
						}
					}
					else if(xButton.bState == BTN_STATE_OVER)
					{
						if(xButton.dwOverColor == 0)
						{
							AfxGetPrinter()->SetColor(ARGB_WHITE);
						}
						else
						{
							AfxGetPrinter()->SetColor(xButton.dwOverColor);
						}
					}
					else if(xButton.bState == BTN_STATE_DOWN)
					{
						if(xButton.dwDownColor == 0)
						{
							AfxGetPrinter()->SetColor(ARGB_WHITE);
						}
						else
						{
							AfxGetPrinter()->SetColor(xButton.dwDownColor);
						}
					}
					else if(xButton.bState == BTN_STATE_CHECKED)
					{
						if(xButton.dwCheckedColor == 0)
						{
							AfxGetPrinter()->SetColor(ARGB_WHITE);
						}
						else
						{
							AfxGetPrinter()->SetColor(xButton.dwCheckedColor);
						}
					}
					else
					{
						AfxGetPrinter()->SetColor(ARGB_WHITE);
					}
					
					/*if(xButton.nTexIndex == 0)
					{
						AfxGetPrinter()->Print(RELATIVE_X(begiter->stPos.left + (75 - nWidth) / 2), RELATIVE_Y(begiter->stPos.top + 9),
							begiter->szText);
					}
					else*/
					{
						int nPrintX = RELATIVE_X(begiter->stPos.left + (RECT_WIDTH(begiter->stPos) - nWidth) / 2);
						int nPrintY = RELATIVE_Y(begiter->stPos.top + (RECT_HEIGHT(begiter->stPos) - 12) / 2);
						AfxGetPrinter()->Print(nPrintX, nPrintY,
							begiter->szText);
					}
				}
			}
		}
	}

	//RenderWindowTip();
}

void GameNewDlg::RenderWindowTip()
{
	if(m_nTipWidth == 0)
	{
		return;
	}

	if(m_xTip.empty())
	{
		return;
	}

	hgeResourceManager* pMgr = pTheGame->GetGameRes();
	GameTextureManager* pPngMgr = GameResourceManager::GetInstance()->GetTexs(RES_CUSTOM);

	if(NULL == pMgr)
	{
		return;
	}

	float x, y;
	AfxGetHge()->Input_GetMousePos(&x, &y);

	//	检测是否在标题栏
	if(!IsCaptionBar(x, y))
	{
		return;
	}
	if(x < m_rcClient.left)
	{
		return;
	}
	if(x > m_rcClient.right - 50)
	{
		return;
	}

	//	调整矩形区域
	RECT rcDrawRect = {0};
	if(-1 != m_nTipLines)
	{
		if(m_nTipLines == 1)
		{
			rcDrawRect.right = m_nTipWidth + 10;
			rcDrawRect.bottom = 12 + 10;
		}
		else
		{
			rcDrawRect.right = 20 * 12 + 10;
			rcDrawRect.bottom = 12 + (m_nTipLines - 1) * 14 + 10;
		}
		rcDrawRect.left += (x - 20);
		rcDrawRect.right += (x - 20);
		rcDrawRect.top += (y + 20);
		rcDrawRect.bottom += (y + 20);

		int nDrawOffset = 0;
		if(rcDrawRect.right >= VIEW_WIDTH)
		{
			nDrawOffset = rcDrawRect.right - VIEW_WIDTH - 1;
			rcDrawRect.right = VIEW_WIDTH - 1;
			rcDrawRect.left -= nDrawOffset;
		}
		if(rcDrawRect.bottom >= VIEW_OPERATE + VIEW_HEIGHT)
		{
			nDrawOffset = rcDrawRect.bottom - VIEW_HEIGHT - VIEW_OPERATE - 1;
			rcDrawRect.bottom = VIEW_HEIGHT + VIEW_OPERATE - 1;
			rcDrawRect.top -= nDrawOffset;
		}

		GamePlayer* pPlayer = GamePlayer::GetInstance();

		//	画底色
		HTEXTURE tex = 0;
		//tex = pMgr->GetTexture("bmcolor");
		tex = pPngMgr->GetTexture(19);
		if(tex)
		{
			/*INITSPRITE(m_pRender, tex);
			SETSPRITEWH(tex, m_pRender, rcDrawRect.right - rcDrawRect.left, rcDrawRect.bottom - rcDrawRect.top);
			m_pRender->Render(rcDrawRect.left, rcDrawRect.top);*/
			RECT rcScale9 = {9, 9, 9, 9};
			RECT rcTex = {0, 0, 32, 32};
			Gfx_SetRenderState(kGfx_9Path_CenterStretch, 1);
			Gfx_Render9Path(m_pRender, tex, &rcTex, &rcScale9, rcDrawRect.left, rcDrawRect.top, RECT_WIDTH(rcDrawRect), RECT_HEIGHT(rcDrawRect));
			/*m_pRender->SetTexture(tex);*/
		}

		//	画边框
		/*tex = pMgr->GetTexture("horbord");
		if(tex)
		{
			SETSPRITEW(tex, m_pRender, rcDrawRect.right - rcDrawRect.left + 1);
			m_pRender->Render( rcDrawRect.left + 1, rcDrawRect.top);
			m_pRender->Render(rcDrawRect.left + 1, rcDrawRect.bottom);
		}
		tex = pMgr->GetTexture("verbord");
		if(tex)
		{
			SETSPRITEH(tex, m_pRender, rcDrawRect.bottom - rcDrawRect.top - 1);
			m_pRender->Render(rcDrawRect.left, rcDrawRect.top + 1 + 1);
			m_pRender->Render(rcDrawRect.right, rcDrawRect.top + 1 + 1);
		}*/
	}
	
	//	文字
	{
		float fKerning = AfxGetPrinter()->GetKerningHeight();
		AfxGetPrinter()->SetKerningHeight(2.0f);
		AfxGetPrinter()->SetColor(ARGB_WHITE);
		int nLines = AfxGetPrinter()->LineFeedRender(rcDrawRect.left + 5, rcDrawRect.top + 5, m_xTip.c_str(), 20 * 12);
		m_nTipLines = nLines;
		AfxGetPrinter()->SetKerningHeight(fKerning);
	}
}

void GameNewDlg::SetWindowTip(const char* _pszTip)
{
	m_xTip = _pszTip;

	m_nTipLines = -1;
	m_nTipWidth = GetTextWidth(_pszTip, 12);
}

void GameNewDlg::SetBkStype(int _nStyle)
{
	m_nBkStyle = _nStyle;

	if(1 == _nStyle)
	{
		//	adjust window rect
		if(RECT_WIDTH(m_rcClient) < /*308*/132*2)
		{
			m_rcClient.right = m_rcClient.left + /*308*/132*2;
		}
		if(RECT_HEIGHT(m_rcClient) < 224)
		{
			m_rcClient.bottom = m_rcClient.top + 224;
		}
	}
}
//////////////////////////////////////////////////////////////////////////