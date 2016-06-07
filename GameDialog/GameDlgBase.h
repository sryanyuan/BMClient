#ifndef GAMEDLGBASE_H_
#define GAMEDLGBASE_H_

//#include "GameRenderObject.h"
#include "../GameDialog/GameCommonDlg.h"
#include <hgeresource.h>
#include "../GameScene/GamePlayer.h"
#include "../../CommonModule/ByteBuffer.h"
#ifdef _USE_SCRIPT_
#include <luaplusinc.h>
using namespace LuaPlus;
#endif

//#include "../Common/QuestContext.h"
//#include "../GameScene/GameChatDlg.h"


#define INITSPRITE(spr, tex) \
	if(!spr){spr = new hgeSprite(tex, 0, 0, AfxGetHge()->Texture_GetWidth(tex), AfxGetHge()->Texture_GetWidth(tex));}\
	else{spr->SetTexture(tex);spr->SetTextureRect( 0, 0, AfxGetHge()->Texture_GetWidth(tex), AfxGetHge()->Texture_GetHeight(tex));}

#define SETSPRITEWH(tex, spr, width, height) \
	spr->SetTexture(tex);spr->SetTextureRect( 0, 0, width, height);

#define SETSPRITEW(tex, spr, width) \
	spr->SetTexture(tex);spr->SetTextureRect( 0, 0, width, AfxGetHge()->Texture_GetHeight(tex));

#define SETSPRITEH(tex, spr, height) \
	spr->SetTexture(tex);spr->SetTextureRect( 0, 0, AfxGetHge()->Texture_GetWidth(tex), height);

#define SETSPRITE(tex, spr) \
	spr->SetTexture(tex);spr->SetTextureRect( 0, 0, AfxGetHge()->Texture_GetWidth(tex), AfxGetHge()->Texture_GetHeight(tex));

#define RELATIVE_X(x)	(m_rcClient.left + (x))
#define RELATIVE_Y(y)	(m_rcClient.top + (y))

#define NEW_EMPTY_SPRITE	new hgeSprite(0, 0, 0, 0, 0)

#define INIT_CLOSEBTN(x,y)	m_rcCloseBtn.left = x;m_rcCloseBtn.right = m_rcCloseBtn.left + 16;m_rcCloseBtn.top = y;m_rcCloseBtn.bottom = m_rcCloseBtn.top + 16

#define SET_RECT(RC, LEFT, TOP, WIDTH, HEIGHT)	RC.left = LEFT;RC.right = LEFT + WIDTH;RC.top = TOP;RC.bottom = TOP + HEIGHT;
#define SET_POINT(PT, X, Y)						PT.x = X;PT.y = Y;

#define GET_DLG(NAME)		GameMainOptUI::GetInstance()->Get##NAME()
//////////////////////////////////////////////////////////////////////////

class RenderDlgControl : public RenderTarget
{
public:
	RenderDlgControl()
	{
		m_pStaticDlg = NULL;
		m_pSwallowEventDlg = NULL;
	}
public:
	inline bool IsBlockCmd()
	{
		return m_pStaticDlg != NULL;
	}
	inline void SetUnBlock()
	{
		m_pStaticDlg = NULL;
	}
	virtual bool ProcUserCmd(const POINT& _mp);
	bool _InternalProcUserCmd(const POINT& _mp);
	virtual void Render()
	{
		{
			for(OBJECTLIST::const_iterator iter = m_objList.begin(); iter != m_objList.end(); ++iter)
			{
				if((*iter)->IsVisible())
				{
					(*iter)->Render();
				}
			}
		}
	}
	virtual void Update(float _dt)
	{
		{
			for(OBJECTLIST::const_iterator iter = m_objList.begin(); iter != m_objList.end(); ++iter)
			{
				if((*iter)->IsVisible())
				{
					(*iter)->Update(_dt);
				}
			}
		}
	}
	virtual RenderObject* RemoveObject(int _id)
	{
		if(m_pStaticDlg)
		{
			if(m_pStaticDlg->GetID() == _id)
			{
				m_pStaticDlg = NULL;
			}
		}
		if(m_pSwallowEventDlg)
		{
			if(m_pSwallowEventDlg->GetID() == _id)
			{
				m_pSwallowEventDlg = NULL;
			}
		}
		return __super::RemoveObject(_id);
	}

protected:
	RenderObject* m_pStaticDlg;
	RenderObject* m_pSwallowEventDlg;
	//OBJECTLIST m_xNeedTopLevel;
};

using std::string;

class GameIntroDlg;
class GamePlayer;
class GameHumDlg;
class GameBagDlg;
class GameInteractiveDlg;
class GameSellDlg;
class GameChatDlg;
class GameShopDlg;
class GameNPC;
class GameSysMenuDlg;
class GameStaticDlg;
class GameSkillDlg;
class GameMapDlg;
class GameStoreDlg;
class GameHumDlg2;
class GameHelpDlg;
class GameBagDlg2;
class GameCubeDlg;
class HumStateDlg;
class GameQuitDlg;
class GameAssistDlg;
class GameTeamDlg;
class GameDecomposeDlg;
class GameQuestDlg;
class GameSettingDlg;
class GameShopDlg2;
class GameStoreDlg2;
class GameOtherHumDlg;
class GameScriptWnd;
class GameAttribDlg;
class GameRoleInfoDlg;
class GameTeamMenu;
class GameInputDlg;
class GameDonateDlg;
class OfflineSellListDlg;
class GameDisplayDlg;
class OfflineSellDlg;
class GameIdentifyDlg;
class GameRankDlg;
class OlShopDlg;
class GameStoveDlg;
class GameChargeDlg;
class GameBigStoreDlg;
//	Ex
class GameMainOptUI : public RenderObject
{
public:
	enum RECT_REGION
	{
		RECT_HPMP,
		RECT_ACC1,
		RECT_ACC2,
		RECT_ACC3,
		RECT_ACC4,
		RECT_ACC5,
		RECT_ACC6,
		RECT_BTNHUM,
		RECT_BTNBAG,
		RECT_BTNSKILL,
		RECT_BTNTASK,
		RECT_BTNHANDMADE,
		RECT_SYSMENU,
		RECT_OFFSELL,
		RECT_OLSHOP,
		RECT_TOTAL,
	};

public:
	static GameMainOptUI* GetInstance()
	{
		static GameMainOptUI* pIns = NULL;
		if(!pIns)
		{
			pIns = new GameMainOptUI;
		}
		return pIns;
	}

protected:
	GameMainOptUI();

public:
	virtual ~GameMainOptUI();

public:
	void SetMapName(const char* lpszMapName);
	bool Init();
	bool IsBlockCmd()
	{
		return m_dlgControl.IsBlockCmd();
	}

public:
	virtual void Render();
	virtual void Update(float _dt);
	virtual bool ProcUserCmd(const POINT& _mp);
	virtual void ResetStatus();

public:
	void CloseAllDialog();

	GameHumDlg* GetHumDlg()
	{
		return m_pHumDlg;
	}
	GameBagDlg* GetBagDlg()
	{
		return m_pBagDlg;
	}
	GameIntroDlg* GetIntroDlg()
	{
		return m_pIntroDlg;
	}
	GameInteractiveDlg* GetActiveDlg()
	{
		return m_pActiveDlg;
	}
	GameSellDlg* GetSellDlg()
	{
		return m_pSellDlg;
	}
	RenderDlgControl* GetDlgControl()
	{
		return &m_dlgControl;
	}
	GameChatDlg* GetChatDlg()
	{
		return m_pChatDlg;
	}
	GameShopDlg* GetShopDlg()
	{
		return m_pShopDlg;
	}
	GameSysMenuDlg* GetSysMenuDlg()
	{
		return m_pSysMenuDlg;
	}
	GameStaticDlg* GetStaticDlg()
	{
		return m_pStaticDlg;
	}
	GameSkillDlg* GetSkillDlg()
	{
		return m_pSkillDlg;
	}
	GameMapDlg* GetMapDlg()
	{
		return m_pMapDlg;
	}
	GameStoreDlg* GetStoreDlg()
	{
		return m_pStoreDlg;
	}
	GameHumDlg2* GetHumDlg2()
	{
		return m_pHumDlg2;
	}
	GameHelpDlg* GetHelpDlg()
	{
		return m_pHelpDlg;
	}
	GameBagDlg2* GetBagDlg2()
	{
		return m_pBagDlg2;
	}
	GameCubeDlg* GetCubeDlg()
	{
		return m_pCubeDlg;
	}
	GameQuitDlg* GetQuitDlg()
	{
		return m_pQuitDlg;
	}
	GameAssistDlg* GetAssistDlg()
	{
		return m_pAssistDlg;
	}
	GameTeamDlg* GetTeamDlg()
	{
		return m_pTeamDlg;
	}
	GameDecomposeDlg* GetDecomposeDlg()
	{
		return m_pDecomposeDlg;
	}
	GameQuestDlg* GetQuestDlg()
	{
		return m_pQuestDlg;
	}
	GameSettingDlg* GetSettingDlg()
	{
		return m_pSettingDlg;
	}
	GameShopDlg2* GetShopDlg2()
	{
		return m_pShopDlg2;
	}
	GameStoreDlg2* GetStoreDlg2()
	{
		return m_pStoreDlg2;
	}
	GameOtherHumDlg* GetOtherHumDlg()
	{
		return m_pOtherHumDlg;
	}
	GameAttribDlg* GetAttribDlg()
	{
		return m_pAttribDlg;
	}
	GameRoleInfoDlg* GetRoleInfoDlg()
	{
		return m_pRoleInfoDlg;
	}
	GameTeamMenu* GetTeamMenu()
	{
		return m_pTeamMenu;
	}
	GameInputDlg* GetInputDlg()
	{
		return m_pInputDlg;
	}
	GameDonateDlg* GetDonateDlg()
	{
		return m_pDonateDlg;
	}
	OfflineSellListDlg* GetOfflineSellListDlg()
	{
		return  m_pOfflineSellListDlg;
	}
	GameDisplayDlg* GetDisplayDlg()
	{
		return m_pDisplayDlg;
	}
	OfflineSellDlg* GetOfflineSellDlg()
	{
		return m_pOfflineSellDlg;
	}
	GameIdentifyDlg* GetIdentifyDlg()
	{
		return m_pIdentifyDlg;
	}
	GameRankDlg* GetRankDlg()
	{
		return m_pRankDlg;
	}
	OlShopDlg* GetOlShopDlg()
	{
		return m_pOlShopDlg;
	}
	GameStoveDlg* GetStoveDlg()
	{
		return m_pStoveDlg;
	}
	GameChargeDlg* GetChargeDlg()
	{
		return m_pChargeDlg;
	}
	GameBigStoreDlg* GetBigStoreDlg()
	{
		return m_pBigStoreDlg;
	}
#ifdef _USE_SCRIPT
	LuaStateOwner& GetScript()
	{
		return m_Script;
	}
#endif
	/*QuestContext& GetQuestInfo()
	{
		return m_quest;
	}*/
	inline hgeResourceManager* GetResourceMgr()
	{
		return m_pResManager;
	}

public:
	bool IsAssistItemSel();
	ItemAttrib* GetAssistItemSel();
	int GetAssistSelIndex();
	void CancelSel();
	//int GetAssistItemsEmptySum();

	bool IsEditInFocus();

private:
	void OnAcc(int _idx);
	void OnBtn(int _idx);
	void OnSysMenuBtn();

private:
	//	脚本交互函数
	int ScriptCommand(int _cmd, int _param0, int _param1, int _param2);
	void LoadScriptWnd();
	void DeleteScriptWnd();
	void UpdateScriptWnd();
	void CloseAllScriptWnd();
	//int GetCommand(int _cmd, int _wparam, int _lparam);
	/*void RegisterMemberFunction();


	//	脚本处理函数
	int Script_AddItem(int _param0, int _param1, int _param2);
	int Script_HaveItem(int _param0, int _param1, int _param2);
	int Script_EmptyItem(int _param0, int _param1, int _param2);
	int Script_ChangeHP(int _param0, int _param1, int _param2);
	int Script_ChangeMP(int _param0, int _param1, int _param2);
	int Script_ChangeEXP(int _param0, int _param1, int _param2);
	int Script_MovePlayer(int _param0, int _param1, int _param2);
	int Script_CloseDlg(int _param0, int _param1, int _param2);
	int Script_CloseAllDlg(int _param0, int _param1, int _param2);
	int Script_CreateNPC(int _param0, int _param1, int _param2);
	int Script_ShowTradeDlg(int _param0, int _param1, int _param2);
	int Script_GetPlayerCoordX(int _param0, int _param1, int _param2);
	int Script_GetPlayerCoordY(int _param0, int _param1, int _param2);
	int Script_ExecuteError(int _param0, int _param1, int _param2);
	int Script_GetQuestStage(int _param0, int _param1, int _param2);
	int Script_AddQuestStage(int _param0, int _param1, int _param2);*/

private:
	hgeSprite* m_pSpr;
	//GfxFont* m_pFont;
	hgeResourceManager* m_pResManager;

	GamePlayer* m_pPlayer;

	char m_szMapName[20];
	int m_nSel;

	//	Ex
	GameHumDlg* m_pHumDlg;
	GameBagDlg* m_pBagDlg;
	GameIntroDlg* m_pIntroDlg;
	GameChatDlg* m_pChatDlg;
	//	对话窗口
	GameInteractiveDlg* m_pActiveDlg;
	//	商店窗口
	GameSellDlg* m_pSellDlg;
	GameShopDlg* m_pShopDlg;
	//	系统菜单窗口
	GameSysMenuDlg* m_pSysMenuDlg;
	//	静态提示窗口
	GameStaticDlg* m_pStaticDlg;
	//	技能窗口
	GameSkillDlg* m_pSkillDlg;
	//	大地图窗口
	GameMapDlg* m_pMapDlg;
	//	仓库窗口
	GameStoreDlg* m_pStoreDlg;
	//	人物窗口
	GameHumDlg2* m_pHumDlg2;
	//	帮助窗口
	GameHelpDlg* m_pHelpDlg;
	//	背包窗口
	GameBagDlg2* m_pBagDlg2;
	//	合成窗口
	GameCubeDlg* m_pCubeDlg;
	//	人物属性窗口
	HumStateDlg* m_pHumStateDlg;
	//	退出确认窗口
	GameQuitDlg* m_pQuitDlg;
	//	快捷窗口
	GameAssistDlg* m_pAssistDlg;
	//	组队窗口
	GameTeamDlg* m_pTeamDlg;
	//	分解窗口
	GameDecomposeDlg* m_pDecomposeDlg;
	//	任务窗口
	GameQuestDlg* m_pQuestDlg;
	//	设置窗口
	GameSettingDlg* m_pSettingDlg;
	//	商店窗口2
	GameShopDlg2* m_pShopDlg2;
	//	仓库窗口2
	GameStoreDlg2* m_pStoreDlg2;
	//	其它玩家窗口
	GameOtherHumDlg* m_pOtherHumDlg;
	//	人物属性窗口
	GameAttribDlg* m_pAttribDlg;
	//	人物信息窗口
	GameRoleInfoDlg* m_pRoleInfoDlg;
	//	组队菜单
	GameTeamMenu* m_pTeamMenu;
	//	输入窗口
	GameInputDlg* m_pInputDlg;
	//	捐赠窗口
	GameDonateDlg* m_pDonateDlg;
	//	网络寄售
	OfflineSellListDlg* m_pOfflineSellListDlg;
	//	装备属性窗口
	GameDisplayDlg* m_pDisplayDlg;
	//	网络寄售出售
	OfflineSellDlg* m_pOfflineSellDlg;
	//	辨识，解绑窗口
	GameIdentifyDlg* m_pIdentifyDlg;
	//	排行榜窗口
	GameRankDlg* m_pRankDlg;
	//	商城窗口
	OlShopDlg* m_pOlShopDlg;
	//	手工窗口
	GameStoveDlg* m_pStoveDlg;
	//	充值窗口
	GameChargeDlg* m_pChargeDlg;
	//	储藏室窗口
	GameBigStoreDlg* m_pBigStoreDlg;

	//	脚本窗口列表
	std::list<GameScriptWnd*> m_xScriptWndList;

	int m_testX;
	int m_testY;

	RECT m_rcOtherRect[RECT_TOTAL];

	//	BTN state
	BYTE m_bSysMenuBtnState;

	//	窗口控制器
	RenderDlgControl m_dlgControl;
#ifdef _USE_SCRIPT_
	//	脚本
	LuaStateOwner m_Script;
#endif
	//	任务
	//QuestContext m_quest;

	//	按钮状态
	BYTE m_bBtnState[RECT_TOTAL];
};

struct SuitAttribRenderInfo
{
	bool bCanShow;

	//	对应的 装备存在的位置
	int nActiveItemPos[10];
	//	激活的装备数量
	int nActiveItemSum;
	//	需要激活的装备数量
	int nActiveItemAll;
	//	激活的属性数量
	int nActiveAttribSum;
	//	所有的属性数量
	//int nAttribAll;

	const ItemExtraAttribList* pExtraAttribList;
};

class GameIntroDlg : public GameObject
{
public:
	GameIntroDlg();
	~GameIntroDlg();

public:
	virtual void Render();
	virtual void Update(float _dt);

public:
	void Init(hgeResourceManager* _res);
	void ShowItemAttrib(ItemAttrib* _item);
	void ShowExp();
	void ShowNothing();
	void ShowHP();
	void ShowMP();
	void ShowString(const char* lpszInfo);
	void ShowLinefeedString(const char* _pszText, int _nwords);

	void RenderItemAttrib();
	bool RenderItemAttribSec();
	bool CalcItemAttribRect();

public:
	void RenderSuitAttrib();
	void CalcSuitAttrib();

private:
	hgeSprite* m_pSpr;
	hgeResourceManager* m_pResManager;
	GfxFont* m_pFont;

	//	Src
	ItemAttrib* m_pItemAttrib;
	bool m_bShowExp;
	bool m_bShowHP;
	bool m_bShowMP;
	std::string m_strInfo;
	std::string m_xLinefeedStr;
	int m_nLinefeedNum;
	DWORD m_dwItemNameColor;
	int m_nColorIndex;

	//	Suit Attrib
	SuitAttribRenderInfo m_stSuitAttribRenderInfo;
	std::string m_xSuitItemName[10];
	RECT m_rcSuit;

	//	Item Rect
	RECT m_rcItemRect;
	int m_nNamePosX;
	ItemAttrib m_oriItem;

	//	加成
	int m_nSameUpgradeAddition;
	int m_nMinEquipGrade;
};

//	Bag
class GameBagDlg : public GameCommonDlg
{
public:
	GameBagDlg();
	~GameBagDlg();

public:
	bool IsItemSel();
	ItemAttrib* GetSelItem();
	void CancelSel();
	void SetSel(int _idx)
	{
		m_nSel = _idx;
	}
	int GetSelIndex() const
	{
		return m_nSel;
	}

public:
	void SetVisible(bool _vb)
	{
		__super::SetVisible(_vb);

		if(!_vb)
		{
			CancelSel();
		}
	}

public:
	void Init(hgeResourceManager* _res, GameMainOptUI* _parent)
	{
		m_pResManager = _res;
		m_pParent = _parent;
	}

public:
	virtual void Render();
	virtual void Update(float _dt);
	virtual bool ProcUserCmd(const POINT& _mp);
	virtual bool IsCaptionBar(int _x, int _y);

private:
	void UpdateRect();
	void OnItem(int _x, int _y);

private:
	//hgeSprite* m_pSpr;
	//GfxFont* m_pFont;
	hgeResourceManager* m_pResManager;
	GameMainOptUI* m_pParent;
	int m_nSel;
};


//	HUM
class GameHumDlg : public GameCommonDlg
{
public:
	enum RECT_CHILD
	{
// 		RECT_EQUIP_BEGIN,
// 		RECT_EQUIP_WEAPON,
// 		RECT_EQUIP_CLOTH,
// 		RECT_EQUIP_HELMET,
// 		RECT_EQUIP_NECKLACE,
// 		RECT_EQUIP_BRACELAT1,
// 		RECT_EQUIP_BRACELAT2,
// 		RECT_EQUIP_RING1,
// 		RECT_EQUIP_RING2,
// 		RECT_EQUIP_MEDAL,
// 		RECT_EQUIP_END,

		RECT_NEXT,
		RECT_PRE,
		RECT_TOTAL,
	};
	enum HUM_PAGE
	{
		PAGE_HUM,
		PAGE_ATTRIB,
		PAGE_TOTAL,
	};
public:
	GameHumDlg();
	~GameHumDlg();

public:
	void SetDlgPage(HUM_PAGE _page){}
	HUM_PAGE GetCurPage(){return m_page;}

public:
	virtual void Render();
	virtual void Update(float _dt);
	virtual bool ProcUserCmd(const POINT& _mp);
	virtual bool IsCaptionBar(int _x, int _y);
	virtual bool OnCommonButtonClick(int _id);

public:
	void Init(hgeResourceManager* _res, GameMainOptUI* _parent);

private:
	void UpdateRect();
	void OnNext();
	void OnPre();

	void OnHumPage(POINT& _pt);
	void OnAttribPage(POINT& _pt);

	void OnItem(PLAYER_ITEM_TYPE _itemrc);

private:
	RECT m_rcOther[RECT_TOTAL];

	//hgeSprite* m_pSpr;
	hgeResourceManager* m_pResourceMangager;
	HUM_PAGE m_page;
	GameMainOptUI* m_pParent;
	//GfxFont* m_pFont;
};


//	商店窗口
#define SELLDLG_PER_PAGE	12
class GameSellDlg : public GameCommonDlg
{
public:
	GameSellDlg();
	~GameSellDlg();

public:
	virtual void Render();
	virtual bool ProcUserCmd(const POINT& _mp);
	virtual bool IsCaptionBar(int _x, int _y);
	virtual void Update(float _dt);

public:
	void Init(hgeResourceManager* _res, GameMainOptUI* _parent);
	void CreateSellDlg(BYTE _type, int _minlevel, int _maxlevel);
	void AddItem(int _id);
	void Reset();
	int GetSelIndex() const
	{
		return m_nSelIdx;
	}
	void CancelSel()
	{
		m_nSelIdx = -1;
	}

private:
	void OnItem(int _x, int _y);
	void OnBuyButton();
	void OnSellButton();
	void OnFixButton();

private:
	hgeResourceManager* m_pRes;
	GameMainOptUI* m_pParent;
	hgeSprite* m_pSpr;
	//GfxFont* m_pFont;

	int m_nPage;
	ItemList m_items;
	int m_nPrintPriceIdx;

	//	选择的物品
	int m_nSelIdx;
};

#define MAX_SHOP_ITEM	72
#define MAX_SHOP_NUM	36
class GameShopDlg : public GameCommonDlg
{
public:
	GameShopDlg();
	virtual ~GameShopDlg();

public:
	virtual void Render();
	virtual void Update(float _dt);
	virtual bool ProcUserCmd(const POINT& _mp);
	virtual bool IsCaptionBar(int _x, int _y);

public:
	void Init(hgeResourceManager* _res, GameMainOptUI* _parent)
	{
		m_pRes = _res;
		m_pParent = _parent;
	}
	//void CreateShopDlg();
	void AddItem(int _id);
	void ClearItem();
	inline void PrevPage()		{--m_nPage;if(m_nPage < 0) m_nPage = 0;}
	inline void NextPage()		{++m_nPage;if(m_nPage > 1) m_nPage = 1;}
	void OnItem(int _x, int _y);
	void CreateByID(GameNPC* _pnpc);

private:
	hgeResourceManager* m_pRes;
	GameMainOptUI* m_pParent;
	ItemAttrib m_xItems[MAX_SHOP_ITEM];
	int m_nItemCounter;
	int m_nPage;
	int m_nPrice;
	int m_nAttribID;
	int m_nHandleID;
};
//////////////////////////////////////////////////////////////////////////
#define MAX_STORE_NUMBER	36
#define MAX_BIGSTORE_NUMBER	80
class GameStoreDlg : public GameCommonDlg
{
public:
	GameStoreDlg();
	virtual ~GameStoreDlg();

public:
	virtual void Render();
	virtual void Update(float _dt);
	virtual bool ProcUserCmd(const POINT& _mp);
	virtual bool IsCaptionBar(int _x, int _y);

	void Init(GameMainOptUI* _pParent)
	{
		m_pParent = _pParent;
	}
	
public:
	void ClearItem();
	void AddItem(const ItemAttrib* _pItem);
	void OnItem(int _x, int _y);
	ItemAttrib* GetItem(DWORD _dwTag);

private:
	ItemAttrib m_xItems[MAX_STORE_NUMBER];
	GameMainOptUI* m_pParent;
};
//////////////////////////////////////////////////////////////////////////
class GameSysMenuDlg : public GameCommonDlg
{
public:
	enum RECT_TYPE
	{
		RT_CONFIG,
		RT_SAVE,
		RT_QUIT,
		RT_DONATE,
		RT_HELP,
		RT_RANK,
		RT_SMALLQUIT,
		RT_TOTAL,
	};
public:
	GameSysMenuDlg();
	virtual ~GameSysMenuDlg();

public:
	virtual void Render();
	virtual void Update(float _dt){}
	virtual bool ProcUserCmd(const POINT& _mp);
	virtual void ResetStatus()
	{
		for(int i = 0; i < RT_TOTAL; ++i)
		{
			m_bBtnFlag[i] = 0;
		}
	}

public:
	inline void Init(GameMainOptUI* _pParent)
	{
		m_pParent = _pParent;
	}
	void OnBtn(int _index);

private:
	GameMainOptUI* m_pParent;
	BYTE m_bBtnFlag[RT_TOTAL];
};
//////////////////////////////////////////////////////////////////////////
#define MODE_STATIC	0
#define MODE_SKILL	1
#define MODE_DEAD	2
#define MODE_DROP	3
#define MODE_SAVE	4
#define MODE_CLOSE	5
#define MODE_MERGY	6

class GameStaticDlg : public RenderObject
{
public:
	GameStaticDlg();
	virtual ~GameStaticDlg();

public:
	inline void Init(GameMainOptUI* _pParent)
	{
		m_pParent = _pParent;
	}
	inline void SetText(const char* _pszText)
	{
		m_xMsg = _pszText;
	}
	inline void SetTitle(const char* _pszTitle)
	{
		m_xTitle = _pszTitle;
		m_nTitleX = (352 - GetTextWidth(_pszTitle, 12)) / 2;
	}
	void Create(const char* _pszText, DWORD _dwAutoHide = 0)
	{
		if(_dwAutoHide == 0)
		{
			m_dwAutoHide = 0;
		}
		else
		{
			m_dwAutoHide = GetTickCount() + _dwAutoHide;
		}
		m_xTitle.clear();
		SetText(_pszText);
		SetVisible(true);
		m_bMode = MODE_STATIC;
	}
	bool CreateSkillButtonDlg(DWORD _dwMgcID);
	bool CreateDeadDlg();
	bool CreateDestoryItemDlg(DWORD _dwTag);
	bool CreateSaveBindDlg();
	bool CreateCloseDlg();
	bool CreateItemMergyDlg(DWORD _dwMergyDestTag, DWORD _dwMergySrcTag);

public:
	virtual void Update(float _dt){}
	virtual void Render();
	virtual bool ProcUserCmd(const POINT& _mp);

private:
	GameMainOptUI* m_pParent;
	std::string m_xMsg;
	std::string m_xTitle;
	int m_nTitleX;
	DWORD m_dwAutoHide;
	BYTE m_bMode;
	int m_nSkillDown;
	DWORD m_dwMgc;
	RECT m_rcRevive[2];
	BYTE m_bBtnState[2];
	DWORD m_dwDropTag;
	DWORD m_dwMergyDestTag;
	DWORD m_dwMergySrcTag;
};

//////////////////////////////////////////////////////////////////////////
class GameMapDlg : public GameNewDlg
{
public:
	GameMapDlg();
	virtual ~GameMapDlg();

public:
	virtual void Render();
	virtual bool ProcUserCmd(const POINT& _mp);
	virtual void ResetStatus();

public:
	bool CreateDlg();
	inline void Reset()
	{
		m_ptLeftTopPos.x = m_ptLeftTopPos.y = 0;
	}
	void AutoLocate();

protected:
	RECT m_rcMapArea;
	POINT m_ptLeftTopPos;
	RECT m_rcMapView;

	bool m_bRightDown;
	POINT m_ptRightPos;
	int m_nMapWidth;
	int m_nMapHeight;

	int m_nShowX;
	int m_nShowY;
	POINT m_ptMouse;
};
//////////////////////////////////////////////////////////////////////////
class GameHumDlg2 : public GameNewDlg
{
public:
	enum HUM_PAGE
	{
		HUM_EQUIP,
		HUM_ATTRIB
	};
public:
	GameHumDlg2();
	virtual ~GameHumDlg2();

public:
	virtual void Render();
	virtual bool OnCommonButtonClick(int _id);
	virtual bool ProcUserCmd(const POINT& _mp);
	virtual bool OnShowDialog();
	virtual bool OnCloseDialog();
	virtual void OnMove(int _nOftX, int _nOftY);

public:
	void OnPre();
	void OnNext();
	void OnItem(PLAYER_ITEM_TYPE _type);
	void OnHumPage(const POINT& _mp);

private:
	int m_nPage;
	GameMainOptUI* m_pParent;
	POINT m_ptItems[PLAYER_ITEM_TOTAL];
	bool m_bShowAttribWhenOpen;
	//GameAttribDlg* m_pAttribDlg;
};
//////////////////////////////////////////////////////////////////////////
#define INVALID_SEL_INDEX	-1
class GameBagDlg2 : public GameNewDlg
{
public:
	GameBagDlg2();
	virtual ~GameBagDlg2();

public:
	virtual void Render();
	virtual bool ProcUserCmd(const POINT& _mp);

	virtual bool OnShowDialog();
	virtual bool OnCloseDialog();

public:
	void OnItem(int _x, int _y);
	bool IsItemSel();
	ItemAttrib* GetSelItem();
	void CancelSel();
	inline int GetSelIndex() const 		{return m_nSel;}

protected:
	int m_nSel;
};
//////////////////////////////////////////////////////////////////////////
#define CUBE_ITEM_NUM	8
class GameCubeDlg : public GameNewDlg
{
public:
	GameCubeDlg();
	virtual ~GameCubeDlg();

public:
	virtual void Render();
	virtual bool ProcUserCmd(const POINT& _mp);
	virtual bool OnCommonButtonClick(int _id);

	virtual bool OnShowDialog();
	virtual bool OnCloseDialog();

public:
	void OnItem(int _idx);
	void ResetAllItems();
	void InsertItems(ItemAttrib* _pItem);

private:
	ItemAttrib* m_pItems[CUBE_ITEM_NUM];
};
//////////////////////////////////////////////////////////////////////////


#endif