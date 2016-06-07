#ifndef _INC_GAMESTOVEDLG_
#define _INC_GAMESTOVEDLG_
//////////////////////////////////////////////////////////////////////////
#include "GameCommonDlg.h"
#include <list>
#include <string>
#include <vector>
//////////////////////////////////////////////////////////////////////////
using std::list;
using std::string;
using std::vector;
//////////////////////////////////////////////////////////////////////////
enum StoveDlgPageType
{
	//	生活技能
	kStoveDlgPage_LifeSkill = 0,
	//	冶炼材料
	kStoveDlgPage_Smelt,
	//	制造装备
	kStoveDlgPage_MakeEquip,
	//	开启潜能
	kStoveDlgPage_OpenPotential,
};

struct MakeListItem
{
	string xName;
	int nItemId;
};
typedef vector<MakeListItem> MakeListItemVector;
//////////////////////////////////////////////////////////////////////////
class GameStoveDlg : public GameNewDlg
{
public:
	GameStoveDlg();

public:
	virtual void Render();
	virtual bool ProcUserCmd(const POINT& _mp);
	virtual bool OnShowDialog();
	virtual bool OnCommonButtonClick(int _id);

public:
	void SwitchTab(StoveDlgPageType _eType);
	void OnButtonOK();

	void RenderTabLifeSkill();
	bool ProcTabLifeSkill(const POINT& _mp);

	void RenderTabSmelt();
	bool ProcTabSmelt(const POINT& _mp);

	void RenderTabMakeEquip();
	bool ProcTabMakeEquip(const POINT& _mp);

	void RenderTabOpenPotential();
	bool ProcTabOpenPotential(const POINT& _mp);

	void ResetData();
	void SetMakeCategory(const int* _pArray, size_t _uSize);

	void OnMakeListSelectChanged();
	bool CursorScrollUp();
	bool CursorScrollDown();
	bool CursorScroll(int _nTimes);

private:
	StoveDlgPageType m_ePage;

	//	for smelt
	ItemAttrib m_aySmeltItems[5];
	ItemAttrib m_stSmeltedItem;
	int m_nSmeltPrice;

	//	for make equip
	ItemAttrib m_ayMakeItems[5];
	int m_nMakeNeedCount[5];
	ItemAttrib m_stMadeItem;
	int m_nMakePrice;
	int m_nMakeNeedLevel;
	int m_nMakeExp;

	int m_nMakeTotalCount;
	int m_nMakeShowIndex;
	int m_nMakeCursorIndex;
	MakeListItemVector m_xMakeItemVector;

	//	for open potential
	ItemAttrib m_stSourceItem;
	ItemAttrib m_stOpenStone;
	int m_nPotentialPrice;
};
//////////////////////////////////////////////////////////////////////////
#endif