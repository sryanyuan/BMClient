#ifndef _SGAMEMAP_H_
#define _SGAMEMAP_H_

#include <hge.h>
#include "../Common/MirMap.h"
#include "../GameScene/GameTextureManager.h"
#include "../GameScene/GamePlayer.h"
#include <hgesprite.h>
#include "../Common/GlobalDefine.h"
//#include "GameFloorItemManager.h"
#include "../Common/GameMapDataAdaptor.h"
#include "../Common/DrawWndBuffer.h"
#include "../Common/MapSurface.h"

class GameMonsterManager;
class GameMonster;

class GameMapManager : public RenderObject
{
public:
	enum MAPRES_TYPE
	{
		MAPRES_RES_TILES = 0,
		MAPRES_RES_SMTILES,
		MAPRES_RES_HUM,
		MAPRES_RES_OBJECTS,
		MAPRES_RES_OBJECTS2,
		MAPRES_RES_OBJECTS3,
		MAPRES_RES_OBJECTS4,
		MAPRES_RES_OBJECTS5,
		MAPRES_RES_OBJECTS6,
		MAPRES_RES_OBJECTS7,
		MAPRES_RES_DNITEMS,
		MAPRES_RES_TOTAL,
	};
protected:
	GameMapManager();
public:
	~GameMapManager();
public:
	//bool LoadMap(int _mapid);
	bool LoadMap(const char* lpszMapName);
	bool LoadMapReadOnly(const char* lpszMapName);
	//	渲染背景
	void RenderBk();
	void RenderBkToTileSurface();
	void RenderBkWithTileSurface();
	//	渲染小背景
	void RenderMid();
	//	渲染物体
	void RenderFrImg();

	//	重新绘制地图
	void RedrawBuffer();

	inline bool IsEnableTileSurface()
	{
		return m_bUseTileTarget;
	}
	inline void SetEnableTileSurface(bool _b)
	{
		m_bUseTileTarget = _b;
		m_bUseTileTarget = false;
	}

public:
	//	地图辅助判断函数
	bool CanThrough(int _x, int _y);
	int GetDoorIndex(int _x, int _y);
	bool IsDoorOpen(int _x, int _y);
	bool HaveDoor(int _x, int _y);
	bool IsValidPoint(const POINT& _pt);

public:
	//	地面物体管理
	bool AddFloorItem(int _centerx, int _centery, ItemAttrib* _pitem);
	ItemAttrib* GetFloorItem(int _x, int _y);
	void AddMouseItem(GroundItem* _pItem)
	{
		m_xMouseItems.push_back(_pItem);
	}

public:
	//	怪物管理
// 	bool CreateMonster(int _x, int _y, int _id);
// 	GameObject* GetMonster(int _x, int _y);
// 	void DeleteMonster(int _x, int _y);
// 	void RemoveAllMonster();
// 	bool MoveMonster(int _dx, int _dy, GameMonster* _mons);

public:
	virtual void Update(float _dt);
	virtual void Render();

public:
	static GameMapManager* GetInstance()
	{
		static GameMapManager* pIns = NULL;
		if(pIns == NULL)
		{
			pIns = new GameMapManager();
		}
		return pIns;
	}

public:
	TileMap* GetMapData()
	{
		return &m_mapData;
	}
	void UpdateClientRect();

private:
	void UpdateTheMonster(GameMonster* _pmons, float _dt);

private:
	TileMap m_mapData;
	GameTextureManager* m_res[MAPRES_RES_TOTAL];

	//OBJECTLIST* m_pObjs;
	//GamePlayer* m_pPlayer;
	hgeSprite* m_pSpr;
	//GameFloorItemManager m_floorItems;
	//GfxFont* m_pFont;
	//	地图数据适配器
	GameMapAdaptor m_dataAdaptor;
	//GameMonsterManager* m_pMons;

	//	上一个矩形区域
	RECT m_rcOldClient;

	DrawWndBuffer m_TileMapSurface;

	MapSurface* m_pxSurface;

	PLAYERLIST m_xAlphaDrawObjects;
	GROUNDITEMS m_xShowItems;
	GROUNDITEMS m_xMouseItems;

	//	tile target
	HTARGET m_hTileTarget;
	bool m_bUseTileTarget;

	//	target sprite
	hgeSprite* m_pTargetRender;
};

#endif