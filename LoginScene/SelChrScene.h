#ifndef SELCHRSCENE_H
#define SELCHRSCENE_H

#include "../GameScene/GameTextureManager.h"
#include "../Widget/Widget.h"
#include "../Common/ResourceID.h"
class hgeResourceManager;
/************************************************************************/
/* SelChrScene 选择职业场景
/************************************************************************/
class SelChrScene : public sgui::WidgetBox
{
public:
	SelChrScene();
	~SelChrScene();

public:
	bool Init(hgeResourceManager* _pRes);

public:
	void Update();
	virtual BOOL OnChildNotify(WORD _wID, DWORD _dwNot, DWORD _dwMsg);
	virtual void Render();

public:
	char* m_szHero;

private:
	hgeResourceManager* m_pxResMgr;
	sgui::Button* m_pxBtnDC;
	sgui::Button* m_pxBtnMC;
	sgui::Button* m_pxBtnSC;
	sgui::Button* m_pxBtnOk;
	sgui::Image* m_pxImgBk;

	char m_cCurSel;
	GameTextureManager* m_pxTexMgr;
};

#endif