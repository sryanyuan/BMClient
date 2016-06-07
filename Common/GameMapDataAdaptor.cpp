#include "GameMapDataAdaptor.h"
#include "../GameScene/GameMapManager.h"

void GameMapAdaptor::GetData(int _x, int _y, SASTAR::SAstarElement* _pele) const
{
	//assert(m_pSrc != NULL);
	MapInfo info;
	GameMapManager::GetInstance()->GetMapData()->GetMapInfo(info);

	ZeroMemory(_pele, sizeof(SASTAR::SAstarElement));
	_pele->nX = _x;
	_pele->nY = _y;

	if(_x < 0 ||
		_x > info.nCol - 1 ||
		_y < 0 ||
		_y > info.nRow - 1)
	{
		_pele->nFn = -1;
		return;
	}

	if(GameMapManager::GetInstance()->CanThrough(_x, _y))
	{
		_pele->bOpen = 1;
	}
	else
	{
		_pele->bOpen = 0;
	}
}