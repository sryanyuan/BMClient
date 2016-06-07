#ifndef GAMEMAPADAPTOR_H_
#define GAMEMAPADAPTOR_H_
#include "../Astar/SAstar.h"

class GameMapAdaptor : public SASTAR::SAstarDataAdaptor
{
// public:
// 	static GameMapAdaptor* GetInstance()
// 	{
// 		static GameMapAdaptor* pIns = NULL;
// 		if(!pIns)
// 		{
// 			pIns = new GameMapAdaptor;
// 		}
// 		return pIns;
// 	}
public:
	virtual void GetData(int _x, int _y, SASTAR::SAstarElement* _pele) const;
};
#endif