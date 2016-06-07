#ifndef _INC_GAMEDECOMPOSEDLG_
#define _INC_GAMEDECOMPOSEDLG_
//////////////////////////////////////////////////////////////////////////
#include "GameDlgBase.h"
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
class GameDecomposeDlg : public GameNewDlg
{
public:
	GameDecomposeDlg();
	virtual ~GameDecomposeDlg();

public:
	virtual void Render();
	virtual bool ProcUserCmd(const POINT& _mp);
	virtual bool OnCommonButtonClick(int _id);

	virtual bool OnShowDialog();

public:
	void RenderDecompose();
	void RenderForge();

	void CommandDecompose(const POINT& _ptMouse);
	void CommandForge(const POINT& _ptMouse);

	void SwitchToDecompose();
	void SwitchToForge();

	void OnItemDecompose(int _idx);
	void OnItemForge(int _idx);
	void ParseItemDecompose(ItemAttrib* _pItem);
	void ParseItemForge();

protected:
	BYTE m_bPage;
	RECT m_rcRect[6];
	ItemAttrib m_stItems[6];
	int m_nPriceCost;
};
//////////////////////////////////////////////////////////////////////////
#endif