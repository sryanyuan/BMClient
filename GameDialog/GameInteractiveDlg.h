#ifndef GAMEINTERACTIVEDLG_H_
#define GAMEINTERACTIVEDLG_H_

#include "../GameDialog/GameDlgBase.h"
#include "../Common/InteractiveContext.h"
#include "../GameScene/GameNPC.h"
//#include <luaplusinc.h>
//using namespace LuaPlus;

class GameInteractiveDlg : public GameNewDlg
{
public:
	GameInteractiveDlg();
	~GameInteractiveDlg();

public:
	virtual void Render();
	virtual void Update(float _dt);
	virtual bool ProcUserCmd(const POINT& _mp);
	virtual bool IsCaptionBar(int _x, int _y);
	virtual bool OnShowDialog();

public:
	void Init(hgeResourceManager* _res, GameMainOptUI* _parent);
	bool ScriptCreateDlg(GameObject* _creator);
	//bool Create(int _npc, int _id, int _step);
	bool Create(GameNPC* _pNpc, int _id, int _step);
	bool Create(GameNPC* _pNpc, const InteractiveDialogItemList& _refItems);
private:
#ifdef _USE_SCRIPT_
	void Script_RegisterMemberFunction();
#endif
	void OnButtonCmd(int _idx, int _cmd);

private:
	InteractiveContextEx m_context;
	//hgeSprite* m_pSpr;
	hgeResourceManager* m_pRes;
	int m_nCreatorID;
	int m_nHandlerID;
	GameMainOptUI* m_pParent;
	//LuaStateOwner m_Script;
	GfxFont* m_pFont;
	//GameNPC* m_pNPC;
	//	支持10个按钮
	//RECT m_buttonArea[10];
	//int m_nButtonNum;

	//	动态绘制相关
	 
};


#endif