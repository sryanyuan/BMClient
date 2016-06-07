#ifndef _INC_GAMERANKDLG_
#define _INC_GAMERANKDLG_
//////////////////////////////////////////////////////////////////////////
#include "GameDlgBase.h"
#include "GameCommonDlg.h"
//////////////////////////////////////////////////////////////////////////
#define RANK_TYPE_LEVEL		0
#define RANK_TYPE_ZHANSHI	1
#define RANK_TYPE_FASHI		2
#define RANK_TYPE_DAOSHI	3
//////////////////////////////////////////////////////////////////////////
struct RankItem
{
	int nRank;
	int nUid;
	char szName[20];
	int nLevel;
	int nPower;
	int nJob;
};
typedef std::vector<RankItem> RankItemVector;
//////////////////////////////////////////////////////////////////////////
class GameRankDlg : public GameNewDlg
{
public:
	GameRankDlg();
	virtual ~GameRankDlg();

public:
	virtual void Render();
	virtual bool OnShowDialog();
	virtual bool OnCommonButtonClick(int _id);

public:
	void SelectRank(int _nRankType);
	void ClearRankItem(int _nRankType);
	void AddRankItem(int _nRankType, const RankItem* _pItem);

private:
	int m_nRankType;

	RankItemVector m_xRankVecLevel;
	RankItemVector m_xRankVecZhanShi;
	RankItemVector m_xRankVecFashi;
	RankItemVector m_xRankVecDaoShi;
};
//////////////////////////////////////////////////////////////////////////
#endif