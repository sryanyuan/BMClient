#include "AssistPaneWnd.h"
#include "../tinyxml/tinyxml.h"
#include <Shlwapi.h>
#include <direct.h>
#include <sstream>
#include <hge.h>
#include "../Common/GlobalFunction.h"
#include "../GameScene/GameInfoManager.h"
#include "../BackMir/BackMir.h"
//////////////////////////////////////////////////////////////////////////
using namespace DuiLib;
using std::string;
using std::stringstream;
//////////////////////////////////////////////////////////////////////////
static const int s_nMaxTabButtons = 5;
//////////////////////////////////////////////////////////////////////////
#define EDIT_COLOR_NORMAL 0xFF000000
#define EDIT_COLOR_ERROR 0xFFFF0000
#define LIST_ITEM_ITEMS "list_item_items"
#define LIST_ITEM_MONSS "list_item_monss"
//////////////////////////////////////////////////////////////////////////
int TranslateKey(int _nKey)
{
	char cKey = toupper(_nKey);

	if(cKey >= 'A' &&
		cKey <= 'Z')
	{
		return HGEK_A + cKey - 'A';
	}
	else if(cKey >= '0' &&
		cKey <= '9')
	{
		return HGEK_0 + cKey - '1';
	}

	return 0;
}
//////////////////////////////////////////////////////////////////////////
AssistPaneWnd::AssistPaneWnd()
{
	m_hParentHWND = NULL;

	m_pTabLayout = NULL;
	m_pLastFocusEdit = NULL;
}

AssistPaneWnd::~AssistPaneWnd()
{
	WriteConfigToLocal();
}

DuiLib::CDuiString AssistPaneWnd::GetSkinFolder()
{
	return DuiLib::CDuiString("");
}
DuiLib::CDuiString AssistPaneWnd::GetSkinFile()
{
	return DuiLib::CDuiString("AssistPane/main.xml");
}
LPCTSTR AssistPaneWnd::GetWindowClassName(void) const
{
	return "AssistPaneWndCls";
}

void AssistPaneWnd::AdjustWindowPos()
{
	if(NULL == m_hParentHWND)
	{
		return;
	}

	if(!IsWindow(m_hParentHWND))
	{
		return;
	}

	CDuiRect rcParent;
	GetWindowRect(m_hParentHWND, &rcParent);

	CDuiRect rcPane;
	GetWindowRect(GetHWND(), &rcPane);

	int nPaneWidth = rcPane.GetWidth();
	int nPaneHeight = rcPane.GetHeight();

	rcPane.left = rcParent.right;
	rcPane.right = rcPane.left + nPaneWidth;
	rcPane.top = rcParent.top;
	rcPane.bottom = rcPane.top + nPaneHeight;
	MoveWindow(GetHWND(), rcPane.left, rcPane.top, rcPane.GetWidth(), rcPane.GetHeight(), TRUE);
}

void AssistPaneWnd::TranslateDispatchMessage(const MSG* pMsg) {
	if (!m_PaintManager.TranslateMessage((const LPMSG)pMsg)) {
		::TranslateMessage(pMsg);
		::DispatchMessage(pMsg);
	}
}

void AssistPaneWnd::OnNotifyTextChanged(DuiLib::TNotifyUI& msg) {
	if (msg.pSender->GetName() == "edit_getitem") {
		CEditUI* pInputEdit = (CEditUI*)m_PaintManager.FindControl("edit_getitem");
		if (NULL == pInputEdit) {
			return;
		}
		const char* pszText = pInputEdit->GetText();
		ApplyItemNamesSearch(pszText);
	} else if (msg.pSender->GetName() == "edit_sgetitem") {
		CEditUI* pInputEdit = (CEditUI*)m_PaintManager.FindControl("edit_sgetitem");
		if (NULL == pInputEdit) {
			return;
		}
		const char* pszText = pInputEdit->GetText();
		ApplyItemNamesSearch(pszText);
	} else if (msg.pSender->GetName() == "edit_mons") {
		CEditUI* pInputEdit = (CEditUI*)m_PaintManager.FindControl("edit_mons");
		if (NULL == pInputEdit) {
			return;
		}
		const char* pszText = pInputEdit->GetText();
		ApplyMonsNamesSearch(pszText);
	}
}

void AssistPaneWnd::OnNotifySelectChanged(DuiLib::TNotifyUI& msg) {
	COptionUI* pOption = (COptionUI*)msg.pSender->GetInterface(DUI_CTR_OPTION);

	if(pOption)
	{
		int nSelIndex = -1;
		if(1 == sscanf(pOption->GetName(), "option_tab_%d", &nSelIndex))
		{
			m_pTabLayout->SelectItem(nSelIndex);
		}
	}
}

void AssistPaneWnd::OnNotifyWindowInit(DuiLib::TNotifyUI& msg) {
	m_pTabLayout = (CTabLayoutUI*)m_PaintManager.FindControl("layout_content");

	//	F1-F11
	char szEditName[32] = {0};

	for(int i = 1; i <= 11; ++i)
	{
		sprintf(szEditName, "edit_f%dmap", i);

		CRichEditUI* pEdit = (CRichEditUI*)m_PaintManager.FindControl(szEditName)->GetInterface(DUI_CTR_RICHEDIT);

		if(pEdit)
		{
			pEdit->SetLimitText(1);
		}
	}

	//m_PaintManager.SetTimer(m_pTabLayout, TIMER_REFRESHUI, 2000);
	LoadConfigFromLocal();

	CControlUI *pCtrl = m_PaintManager.FindControl("option_tab_2");
	if (NULL != pCtrl) {
		pCtrl->SetVisible(false);
	}

	// Load all item names and monster names
#ifdef _DEBUG
	std::vector<int> xItemIDs;
	if (GameInfoManager::GetInstance()->GetItemIDs(xItemIDs)) {
		for (int i = 0; i < xItemIDs.size(); i++) {
			ItemAttrib item;
			if (!GameInfoManager::GetInstance()->GetItemAttrib(xItemIDs[i], &item)) {
				continue;
			}
			std::pair<std::string, int> kv;
			kv.first = item.name;
			kv.second = item.id;
			m_xItemNamesSearchMap.insert(kv);
		}
		// Add all item into search panel
		ApplyItemNamesSearch(NULL);
	}
	xItemIDs.clear();
	if (GameInfoManager::GetInstance()->GetMonsterIDs(xItemIDs)) {
		for (int i = 0; i < xItemIDs.size(); i++) {
			ItemAttrib item;
			if (!GameInfoManager::GetInstance()->GetMonsterAttrib(xItemIDs[i], &item)) {
				continue;
			}
			std::pair<std::string, int> kv;
			kv.first = item.name;
			kv.second = item.id;
			m_xMonsNamesSearchMap.insert(kv);
		}
		// Add all monster into search panel
		ApplyMonsNamesSearch(NULL);
	}
#endif
}

void AssistPaneWnd::OnNotifyClick(DuiLib::TNotifyUI& msg) {
	if(msg.pSender->GetName() == "button_tableft" ||
		msg.pSender->GetName() == "button_tabright")
	{
		ProcessTabChange(msg);
	}
	else if(msg.pSender->GetName().Left(13) == "button_tabok_")
	{
		ProcessPageOK(msg);
	}
	else if(msg.pSender->GetName() == "hidebtn")
	{
		ShowWindow(false);
		// Notify
		::PostMessage(m_hParentHWND, WM_HIDE_ASSITWND, 0, 0);
		// ui with name closebtn will be closed by default notify handler
		return;
	}
}

void AssistPaneWnd::OnNotifyTimer(DuiLib::TNotifyUI& msg) {
	if(msg.wParam == TIMER_REFRESHUI)
	{
		m_PaintManager.KillTimer(m_pTabLayout, TIMER_REFRESHUI);
	}
}

void AssistPaneWnd::OnNotifyReturn(DuiLib::TNotifyUI& msg) {
	if (msg.pSender->GetName() == "edit_getitem") {
#ifdef _DEBUG
		CEditUI* pInputEdit = (CEditUI*)m_PaintManager.FindControl("edit_getitem");
		if (NULL == pInputEdit) {
			return;
		}
		const char* pszText = pInputEdit->GetText();
		if (NULL == pszText ||
			0 == pszText[0]) {
				return;
		}
		std::string xKey = pszText;
		std::map<std::string, int>::const_iterator it = m_xItemNamesSearchMap.find(xKey);
		if (it == m_xItemNamesSearchMap.end()) {
			::MessageBox(NULL, "No item found", "ERROR", MB_ICONERROR | MB_OK);
			return;
		}

		PkgPlayerSpeOperateReq req;
		req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
		req.dwOp = CMD_OP_GET;
		req.dwParam = MAKELONG(it->second, 1);
		g_xBuffer.Reset();
		g_xBuffer << req;
		SendBufferToGS(&g_xBuffer);
#endif
	} else if (msg.pSender->GetName() == "edit_sgetitem") {
#ifdef _DEBUG
		CEditUI* pInputEdit = (CEditUI*)m_PaintManager.FindControl("edit_sgetitem");
		if (NULL == pInputEdit) {
			return;
		}
		const char* pszText = pInputEdit->GetText();
		if (NULL == pszText ||
			0 == pszText[0]) {
				return;
		}
		std::string xKey = pszText;
		std::map<std::string, int>::const_iterator it = m_xItemNamesSearchMap.find(xKey);
		if (it == m_xItemNamesSearchMap.end()) {
			::MessageBox(NULL, "No item found", "ERROR", MB_ICONERROR | MB_OK);
			return;
		}

		// Get item point
#if _MSC_VER == 1800
		CComboUI* pBox = (CComboUI*)m_PaintManager.FindControl("sitem_level");
#else
		CComboBoxUI* pBox = (CComboBoxUI*)m_PaintManager.FindControl("sitem_level");
#endif
		if (NULL == pBox) {
			return;
		}
		int nValue = pBox->GetCurSel();

		PkgPlayerSpeOperateReq req;
		req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
		req.dwOp = CMD_OP_SGET;
		req.dwParam = MAKELONG(it->second, nValue);
		g_xBuffer.Reset();
		g_xBuffer << req;
		SendBufferToGS(&g_xBuffer);
#endif
	} else if (msg.pSender->GetName() == "edit_mons") {
#ifdef _DEBUG
		CEditUI* pInputEdit = (CEditUI*)m_PaintManager.FindControl("edit_mons");
		if (NULL == pInputEdit) {
			return;
		}
		const char* pszText = pInputEdit->GetText();
		if (NULL == pszText ||
			0 == pszText[0]) {
				return;
		}
		std::string xKey = pszText;
		std::map<std::string, int>::const_iterator it = m_xMonsNamesSearchMap.find(xKey);
		if (it == m_xMonsNamesSearchMap.end()) {
			::MessageBox(NULL, "No monster found", "ERROR", MB_ICONERROR | MB_OK);
			return;
		}

		// Get item point
#if _MSC_VER == 1800
		CComboUI* pBox = (CComboUI*)m_PaintManager.FindControl("mons_level");
#else
		CComboBoxUI* pBox = (CComboBoxUI*)m_PaintManager.FindControl("mons_level");
#endif
		if (NULL == pBox) {
			return;
		}
		int nValue = pBox->GetCurSel();

		PkgPlayerSpeOperateReq req;
		req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
		// Determine the command
		if (1 == nValue) {
			req.dwOp = CMD_OP_EMONSTER;
			req.dwParam = it->second;
		} else if (2 == nValue) {
			req.dwOp = CMD_OP_LMONSTER;
			req.dwParam = it->second;
		} else {
			req.dwOp = CMD_OP_MONSTER;
			req.dwParam = MAKELONG(it->second, 1);
		}
		g_xBuffer.Reset();
		g_xBuffer << req;
		SendBufferToGS(&g_xBuffer);
#endif
	} else if (msg.pSender->GetName() == "edit_levelup") {
#ifdef _DEBUG
		CEditUI* pInputEdit = (CEditUI*)m_PaintManager.FindControl("edit_levelup");
		if (NULL == pInputEdit) {
			return;
		}
		const char* pszText = pInputEdit->GetText();
		if (NULL == pszText ||
			0 == pszText[0]) {
				::MessageBox(NULL, "Invalid level", "ERROR", MB_ICONERROR | MB_OK);
				return;
		}

		int nLevel = atoi(pszText);
		if (nLevel <= 0 ||
			nLevel > MAX_LEVEL) {
				::MessageBox(NULL, "Invalid level", "ERROR", MB_ICONERROR | MB_OK);
				return;
		}

		PkgPlayerSpeOperateReq req;
		req.uUserId = GamePlayer::GetInstance()->GetHandlerID();
		req.dwOp = CMD_OP_LEVELUP;
		req.dwParam = nLevel;
		g_xBuffer.Reset();
		g_xBuffer << req;
		SendBufferToGS(&g_xBuffer);
#endif
	}
}

void AssistPaneWnd::OnNotifySetFocus(DuiLib::TNotifyUI& msg) {
	static const char* s_szRecordFocusEditNames[] = {
		"edit_getitem",
		"edit_sgetitem",
		"edit_mons",
	};
	for (int i = 0; i < sizeof(s_szRecordFocusEditNames) / sizeof(s_szRecordFocusEditNames[0]); i++) {
		if (msg.pSender->GetName() == s_szRecordFocusEditNames[i]) {
			m_pLastFocusEdit = (CEditUI*)msg.pSender->GetInterface(DUI_CTR_EDIT);
			break;
		}
	}
}

void AssistPaneWnd::OnNotifyItemClick(DuiLib::TNotifyUI& msg) {
	if (msg.pSender->GetUserData() == LIST_ITEM_ITEMS ||
		msg.pSender->GetUserData() == LIST_ITEM_MONSS) {
			if (NULL != m_pLastFocusEdit) {
				m_pLastFocusEdit->SetText(msg.pSender->GetText());
			}
	}
}

void AssistPaneWnd::Notify(DuiLib::TNotifyUI& msg)
{
	if(msg.sType == DUI_MSGTYPE_SELECTCHANGED) {
		OnNotifySelectChanged(msg);
	} else if(msg.sType == DUI_MSGTYPE_WINDOWINIT) {
		OnNotifyWindowInit(msg);
	} else if(msg.sType == DUI_MSGTYPE_CLICK) {
		OnNotifyClick(msg);
	} else if(msg.sType == DUI_MSGTYPE_TIMER) {
		OnNotifyTimer(msg);	
	} else if (msg.sType == DUI_MSGTYPE_TEXTCHANGED) {
		OnNotifyTextChanged(msg);
	} else if (msg.sType == DUI_MSGTYPE_RETURN) {
		OnNotifyReturn(msg);
	} else if (msg.sType == DUI_MSGTYPE_SETFOCUS) {
		OnNotifySetFocus(msg);
	} else if (msg.sType == DUI_MSGTYPE_ITEMCLICK) {
		OnNotifyItemClick(msg);
	}

	__super::Notify(msg);
}

void AssistPaneWnd::OnFinalMessage( HWND hWnd ) {
	// Skip close message
	__super::OnFinalMessage(hWnd);
}

void AssistPaneWnd::ApplyItemNamesSearch(const char* _pszPattern) {
#ifdef _DEBUG
	CListUI* pList = (CListUI*)m_PaintManager.FindControl("itemList");
	if (NULL == pList) {
		return;
	}
	if (pList->GetCount() == 0 &&
		!m_xItemNamesSearchMap.empty()) {
		// Empty, not initialized
		CListHeaderItemUI* pHeader = new CListHeaderItemUI;
		pHeader->SetText("物品列表");
		pList->Add(pHeader);
		std::map<std::string, int>::const_iterator it = m_xItemNamesSearchMap.begin();
		for (it; it != m_xItemNamesSearchMap.end(); it++) {
			CListLabelElementUI* pEle = new CListLabelElementUI;
			pEle->SetText(it->first.c_str());
			pEle->SetAttribute("align", "hcenter");
			pEle->SetUserData(LIST_ITEM_ITEMS);
			pList->Add(pEle);
		}
	}
	// Hide some item with pattern if needed
	for (int i = 0; i < pList->GetCount(); i++) {
		CControlUI* pChild = pList->GetItemAt(i);
		if (NULL == pChild) {
			continue;
		}
		// Check match the pattern
		if (NULL == _pszPattern ||
			_pszPattern[0] == '\0') {
			pChild->SetVisible(true);
			continue;
		}
		if (pChild->GetText().Find(_pszPattern) >= 0) {
			pChild->SetVisible(true);
		} else {
			pChild->SetVisible(false);
		}
	}
#endif
}

void AssistPaneWnd::ApplyMonsNamesSearch(const char* _pszPattern) {
#ifdef _DEBUG
	CListUI* pList = (CListUI*)m_PaintManager.FindControl("monsList");
	if (NULL == pList) {
		return;
	}
	if (pList->GetCount() == 0 &&
		!m_xMonsNamesSearchMap.empty()) {
			// Empty, not initialized
			CListHeaderItemUI* pHeader = new CListHeaderItemUI;
			pHeader->SetText("NPC列表");
			pList->Add(pHeader);
			std::map<std::string, int>::const_iterator it = m_xMonsNamesSearchMap.begin();
			for (it; it != m_xMonsNamesSearchMap.end(); it++) {
				CListLabelElementUI* pEle = new CListLabelElementUI;
				pEle->SetText(it->first.c_str());
				pEle->SetAttribute("align", "hcenter");
				pEle->SetUserData(LIST_ITEM_MONSS);
				pList->Add(pEle);
			}
	}
	// Hide some item with pattern if needed
	for (int i = 0; i < pList->GetCount(); i++) {
		CControlUI* pChild = pList->GetItemAt(i);
		if (NULL == pChild) {
			continue;
		}
		// Check match the pattern
		if (NULL == _pszPattern ||
			_pszPattern[0] == '\0') {
				pChild->SetVisible(true);
				continue;
		}
		if (pChild->GetText().Find(_pszPattern) >= 0) {
			pChild->SetVisible(true);
		} else {
			pChild->SetVisible(false);
		}
	}
#endif
}

void AssistPaneWnd::ProcessPageOK(DuiLib::TNotifyUI& msg)
{
	int nPage = -1;
	if(1 != sscanf(msg.pSender->GetName(), "button_tabok_%d", &nPage))
	{
		return;
	}

	if(-1 == nPage)
	{
		return;
	}

	switch(nPage)
	{
		//	item visible
	case 0:
		{
			ApplyItemVisible();
		}break;
		//	key map
	case 1:
		{
			ApplyKeyMap();
		}break;
	}
}

void AssistPaneWnd::ApplyItemVisible()
{
	CRichEditUI* pEdit = (CRichEditUI*)m_PaintManager.FindControl("richedit_itemvisible")->GetInterface(DUI_CTR_RICHEDIT);
	if(NULL == pEdit)
	{
		return;
	}

	m_xItemVisibleSet.clear();

	CDuiString xEditContent = pEdit->GetText();

	string xContent = xEditContent;
	std::list<string> xStrItems;

	StringSplit(xContent, "\r\n", xStrItems);

	if(!xStrItems.empty())
	{
		std::list<string>::iterator begIter = xStrItems.begin();
		std::list<string>::const_iterator endIter = xStrItems.end();

		for(begIter;
			begIter != endIter;
			++begIter)
		{
			string& refItem = *begIter;

			if(!refItem.empty())
			{
				m_xItemVisibleSet.insert(refItem);
			}
		}
	}
}

void AssistPaneWnd::ApplyKeyMap()
{
	//	F1-F11
	char szEditName[32] = {0};
	m_xKeyMap.clear();

	for(int i = 1; i <= 11; ++i)
	{
		sprintf(szEditName, "edit_f%dmap", i);

		CRichEditUI* pEdit = (CRichEditUI*)m_PaintManager.FindControl(szEditName)->GetInterface(DUI_CTR_RICHEDIT);

		if(pEdit)
		{
			string xInput = pEdit->GetText();

			if(xInput.length() == 1 &&
				(isdigit(xInput[0]) || isalpha(xInput[0])))
			{
				int nTranslateKey = TranslateKey(xInput[0]);

				if(nTranslateKey > 0)
				{
					int nMappedKey = HGEK_F1 + (i - 1);
					m_xKeyMap.insert(std::make_pair(nMappedKey, nTranslateKey));
				}
				else
				{
					pEdit->SetText("");
				}
			}
		}
	}
}

void AssistPaneWnd::CopyKeySettingToUI(int _nKey, int _nMappedKey)
{
	if((_nKey >= HGEK_F1 && _nKey <= HGEK_F11) ||
		(_nKey >= HGEK_A && _nKey <= HGEK_Z))
	{
		int nUpperKey = toupper(_nMappedKey);
		bool bCanSet = false;
		int nRetranslateKey = 0;
		char szCtrlName[32] = {0};

		if(_nKey >= HGEK_F1 && _nKey <= HGEK_F11)
		{
			sprintf(szCtrlName, "edit_f%dmap", _nKey - HGEK_F1 + 1);
		}
		else if(_nKey >= HGEK_A && _nKey <= HGEK_Z)
		{
			sprintf(szCtrlName, "edit_%cmap", _nKey - HGEK_A + 'a');
		}

		if(nUpperKey >= HGEK_A &&
			nUpperKey <= HGEK_Z)
		{
			bCanSet = true;
			nRetranslateKey = nUpperKey - HGEK_A + 'A';
		}
		if(nUpperKey >= HGEK_0 &&
			nUpperKey <= HGEK_9)
		{
			bCanSet = true;
			nRetranslateKey = nUpperKey - HGEK_0 + '0';
		}

		if(!bCanSet)
		{
			return;
		}

		char szKey[2] = {0};
		szKey[0] = nRetranslateKey;

		CRichEditUI* pEdit = (CRichEditUI*)m_PaintManager.FindControl(szCtrlName)->GetInterface(DUI_CTR_RICHEDIT);
		if(pEdit)
		{
			pEdit->Clear();
			pEdit->AppendText(szKey);
		}
	}
}

void AssistPaneWnd::InsertItemVisible(const char* _pszItem)
{
	char szItem[50] = {0};
	if(strlen(_pszItem) > 19)
	{
		return;
	}

	strcpy(szItem, _pszItem);
	strcat(szItem, "\r\n");

	CRichEditUI* pEdit = (CRichEditUI*)m_PaintManager.FindControl("richedit_itemvisible")->GetInterface(DUI_CTR_RICHEDIT);
	if(NULL == pEdit)
	{
		return;
	}
	pEdit->AppendText(szItem);
}

void AssistPaneWnd::ProcessTabChange(DuiLib::TNotifyUI& msg)
{
	bool bLeft = true;

	if(msg.pSender->GetName() == "button_tabright")
	{
		bLeft = false;
	}

#if _MSC_VER == 1800
	CDuiPtrArray* pOptions = m_PaintManager.GetOptionGroup("group_tabsel");
#else
	CStdPtrArray* pOptions = m_PaintManager.GetOptionGroup("group_tabsel");
#endif
	COptionUI* pCurOption = NULL;
	
	for(int i = 0; i < pOptions->GetSize(); ++i)
	{
		COptionUI* pOption = (COptionUI*)pOptions->GetAt(i);

		if(pOption->IsSelected())
		{
			pCurOption = pOption;
			break;
		}
	}

	if(NULL == pCurOption)
	{
		return;
	}

	int nNextSel = -1;
	int nCurSel = -1;

	if(1 != sscanf(pCurOption->GetName(), "option_tab_%d", &nCurSel))
	{
		return;
	}

	if(nCurSel == -1)
	{
		return;
	}

	nNextSel = nCurSel;

	if(bLeft)
	{
		--nNextSel;
	}
	else
	{
		++nNextSel;
	}

	if(nNextSel >= 0 &&
		nNextSel < pOptions->GetSize())
	{
		char szOptionName[32] = {0};
		sprintf(szOptionName, "option_tab_%d", nNextSel);

		COptionUI* pSelOption = (COptionUI*)m_PaintManager.FindControl(szOptionName)->GetInterface(DUI_CTR_OPTION);

		if(pSelOption)
		{
			pSelOption->Selected(true);
		}

		if(bLeft)
		{
			int nBeforeItems = 0;
			int nBeforeVisibleItems = 0;

			for(int i = 0; i < pOptions->GetSize(); ++i)
			{
				COptionUI* pOption = (COptionUI*)pOptions->GetAt(i);

				if(pOption != pCurOption)
				{
					++nBeforeItems;

					if(pOption->IsVisible())
					{
						++nBeforeVisibleItems;
					}
				}
				else
				{
					break;
				}
			}

			if(nBeforeItems > 0 &&
				nBeforeVisibleItems == 0)
			{
				bool bCanShow = false;
				int nVisibleCounter = 0;

				for(int i = 0; i < pOptions->GetSize(); ++i)
				{
					COptionUI* pOption = (COptionUI*)pOptions->GetAt(i);
					pOption->SetVisible(false);
				}

				for(int i = 0; i < pOptions->GetSize(); ++i)
				{
					COptionUI* pOption = (COptionUI*)pOptions->GetAt(i);
					
					if(pOption == pCurOption)
					{
						int nBeforeShowIndex = i - 1;
						if(nBeforeShowIndex >= 0 &&
							nBeforeShowIndex < pOptions->GetSize())
						{
							COptionUI* pOptionBefore = (COptionUI*)pOptions->GetAt(nBeforeShowIndex);
							pOptionBefore->SetVisible();
							++nVisibleCounter;
						}

						bCanShow = true;
					}

					if(bCanShow)
					{
						pOption->SetVisible(true);
						++nVisibleCounter;
					}

					if(nVisibleCounter >= s_nMaxTabButtons)
					{
						break;
					}
				}
			}
		}
		else
		{
			int nVisibleIndex = 0;
			int nAfterInvisible = 0;

			for(int i = 0; i < pOptions->GetSize(); ++i)
			{
				COptionUI* pOption = (COptionUI*)pOptions->GetAt(i);

				if(pOption->IsVisible())
				{
					++nVisibleIndex;
				}
				
				if(pOption == pCurOption)
				{
					break;
				}
			}

			for(int i = pOptions->GetSize() - 1; i >= 0; --i)
			{
				COptionUI* pOption = (COptionUI*)pOptions->GetAt(i);

				if(!pOption->IsVisible())
				{
					++nAfterInvisible;
				}

				if(pOption == pCurOption)
				{
					break;
				}
			}

			if(nAfterInvisible > 0 &&
				nVisibleIndex == s_nMaxTabButtons)
			{
				bool bCanShow = false;
				int nVisibleCounter = 0;

				for(int i = pOptions->GetSize() - 1; i >= 0; --i)
				{
					COptionUI* pOption = (COptionUI*)pOptions->GetAt(i);
					pOption->SetVisible(false);
				}

				for(int i = pOptions->GetSize() - 1; i >= 0; --i)
				{
					COptionUI* pOption = (COptionUI*)pOptions->GetAt(i);

					if(pOption == pCurOption)
					{
						int nAfterShowIndex = i + 1;
						if(nAfterShowIndex >= 0 &&
							nAfterShowIndex < pOptions->GetSize())
						{
							COptionUI* pOptionAfter = (COptionUI*)pOptions->GetAt(nAfterShowIndex);
							pOptionAfter->SetVisible();
							++nVisibleCounter;
						}

						bCanShow = true;
					}

					if(bCanShow)
					{
						pOption->SetVisible(true);
						++nVisibleCounter;
					}

					if(nVisibleCounter >= s_nMaxTabButtons)
					{
						break;
					}
				}
			}
		}
	}
}

void AssistPaneWnd::LoadConfigFromLocal()
{
	char szRootPath[MAX_PATH] = {0};
	GetRootPath(szRootPath, MAX_PATH);
	strcat(szRootPath, "/assist/local.xml");

	if(!PathFileExists(szRootPath))
	{
		return;
	}

	TiXmlDocument xmldoc;
	if(!xmldoc.LoadFile(szRootPath))
	{
		return;
	}

	TiXmlElement* pRoot = xmldoc.RootElement();
	//	item visible
	TiXmlElement* pItemVisibleRoot = pRoot->FirstChildElement("ItemVisible");
	if(pItemVisibleRoot)
	{
		int nSectionCounter = 0;
		const char* pszSecValue = pItemVisibleRoot->Attribute("counter");

		if(NULL != pszSecValue)
		{
			nSectionCounter = atoi(pszSecValue);
		}

		if(nSectionCounter > 0)
		{
			for(int i = 0; i < nSectionCounter; ++i)
			{
				char szSection[32] = {0};
				sprintf(szSection, "data_%d", i);

				TiXmlElement* pNode = pItemVisibleRoot->FirstChildElement(szSection);

				if(pNode)
				{
					const char* pszName = pNode->Attribute("name");

					if(pszName)
					{
						m_xItemVisibleSet.insert(pszName);
						InsertItemVisible(pszName);
					}
				}
			}
		}
	}

	//	key map
	TiXmlElement* pKeyMapRoot = pRoot->FirstChildElement("KeyMap");
	if(pKeyMapRoot)
	{
		int nSectionCounter = 0;
		const char* pszSecValue = pKeyMapRoot->Attribute("counter");

		if(NULL != pszSecValue)
		{
			nSectionCounter = atoi(pszSecValue);
		}

		if(nSectionCounter > 0)
		{
			for(int i = 0; i < nSectionCounter; ++i)
			{
				char szSection[32] = {0};
				sprintf(szSection, "data_%d", i);

				TiXmlElement* pNode = pKeyMapRoot->FirstChildElement(szSection);

				if(pNode)
				{
					const char* pszToBeMappedKey = pNode->Attribute("tobemappedkey");
					const char* pszMappedKey = pNode->Attribute("mappedkey");

					if(pszToBeMappedKey &&
						pszMappedKey &&
						(0 != strcmp("0", pszToBeMappedKey)) &&
						(0 != strcmp("0", pszMappedKey)))
					{
						m_xKeyMap.insert(std::make_pair(atoi(pszToBeMappedKey), atoi(pszMappedKey)));
						CopyKeySettingToUI(atoi(pszToBeMappedKey), atoi(pszMappedKey));
					}
				}
			}
		}
	}
}

void AssistPaneWnd::WriteConfigToLocal()
{
	char szRootPath[MAX_PATH] = {0};
	GetRootPath(szRootPath, MAX_PATH);
	strcat(szRootPath, "/assist/");

	if(!PathFileExists(szRootPath))
	{
		mkdir(szRootPath);
	}

	strcat(szRootPath, "local.xml");

	TiXmlDocument xmldoc;

	//	root
	TiXmlNode* pRoot = xmldoc.InsertEndChild(TiXmlElement("AssistConfig"));

	//	item visible
	TiXmlElement xItemVisibleNode("ItemVisible");
	xItemVisibleNode.SetAttribute("counter", m_xItemVisibleSet.size());
	TiXmlNode* pItemVisibleRoot = pRoot->InsertEndChild(xItemVisibleNode);
	
	if(!m_xItemVisibleSet.empty())
	{
		StringSet::iterator begIter = m_xItemVisibleSet.begin();
		StringSet::iterator endIter = m_xItemVisibleSet.end();
		int nSectionCounter = 0;

		for(begIter;
			begIter != endIter;
			++begIter)
		{
			const string& refValue = *begIter;

			char szSection[32] = {0};
			sprintf(szSection, "data_%d", nSectionCounter);
			TiXmlElement xSubNode(szSection);
			xSubNode.SetAttribute("name", refValue.c_str());

			pItemVisibleRoot->InsertEndChild(xSubNode);
			++nSectionCounter;
		}
	}

	//	key map
	TiXmlElement xKeyMapNode("KeyMap");
	xKeyMapNode.SetAttribute("counter", m_xKeyMap.size());
	TiXmlNode* pKeyMapRoot = pRoot->InsertEndChild(xKeyMapNode);

	if(!m_xKeyMap.empty())
	{
		KintVintMap::iterator begIter = m_xKeyMap.begin();
		KintVintMap::const_iterator endIter = m_xKeyMap.end();
		int nSectionCounter = 0;

		for(begIter;
			begIter != endIter;
			begIter++)
		{
			int nToBeMappedKey = begIter->first;
			int nMappedKey = begIter->second;

			if(nToBeMappedKey != 0 &&
				nMappedKey != 0)
			{
				char szKeyBuf[32] = {0};
				char szSection[32] = {0};
				sprintf(szSection, "data_%d", nSectionCounter);

				TiXmlElement xSubNode(szSection);
				xSubNode.SetAttribute("tobemappedkey", itoa(nToBeMappedKey, szKeyBuf, 10));
				xSubNode.SetAttribute("mappedkey", itoa(nMappedKey, szKeyBuf, 10));

				pKeyMapRoot->InsertEndChild(xSubNode);
				++nSectionCounter;
			}
		}
	}

	xmldoc.SaveFile(szRootPath);
}





bool AssistPaneWnd::CheckItemAlert(const char* _pszItemName)
{
	return false;
}

bool AssistPaneWnd::CheckItemVisible(const char* _pszItemName)
{
	if(m_xItemVisibleSet.find(_pszItemName) != m_xItemVisibleSet.end())
	{
		return false;
	}
	return true;
}

int AssistPaneWnd::CheckMappedKey(int _nKey)
{
	//return 0;

	KintVintMap::const_iterator fndIter = m_xKeyMap.find(_nKey);

	if(fndIter != m_xKeyMap.end())
	{
		return fndIter->second;
	}

	return 0;
}

LRESULT AssistPaneWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	/*MSG msg;
	msg.hwnd = GetHWND();
	msg.message = uMsg;
	msg.wParam = wParam;
	msg.lParam = lParam;
	TranslateMessage(&msg);*/

	return __super::HandleMessage(uMsg, wParam, lParam);
}