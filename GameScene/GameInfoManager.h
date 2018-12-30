#ifndef GAMEINFOMANAGER_H_
#define GAMEINFOMANAGER_H_
//////////////////////////////////////////////////////////////////////////
#include "../../CommonModule/ObjectData.h"
#include <sqlite3.h>
#include <map>
//////////////////////////////////////////////////////////////////////////
#define ITEM_ID_MAX	2000
#define ITEM_DESC_TEXT_WIDTH	(12 * 12)
//////////////////////////////////////////////////////////////////////////
struct MagicInfo;
struct MagicRenderInfo;
struct EffectHumInfo;
struct lua_State;
struct ItemExtraAttribList;

struct ItemDesc
{
	const char* pszDesc;
	int nTextLength;
	int nTextWidth;
	int nTextLine;
};
//////////////////////////////////////////////////////////////////////////

class GameInfoManager
{
	//SINGLETON_DECLARE(GameInfoManager);
protected:
	GameInfoManager();
public:
	~GameInfoManager();

public:
	//	物品信息
	bool GetItemAttrib(int _id, ItemAttrib* _pitem);
	bool GetItemAttribINI(int _id, ItemAttrib* _pitem);
	bool GetItemAttribSQL(int _id, ItemAttrib* _pitem);
	//	怪物NPC绘制信息
	bool GetMonsterRenderInfo(int _id, RenderInfo* _info);
	bool GetMonsterRenderInfo(int _id, const RenderInfo** _info);
	//	怪物属性
	bool GetMonsterAttrib(int _id, ItemAttrib* _pitem);
	bool GetMonsterAttribINI(int _id, ItemAttrib* _pitem);
	bool GetMonsterAttribSQL(int _id, ItemAttrib* _pitem);
	//	怪物掉落物品
	bool GetMonsterItems(int _id, int* _sum, ItemAttrib** _items);
	//	小地图索引
	bool GetMiniMapIndex(const char* lpMapName, int* _pvalue);
	//	魔法信息
	//bool GetMgcInfo(int _id, MagicInfo* _mgc);
	bool GetMagicRenderInfo(int _id, const MagicRenderInfo** _info);
	bool GetMagicRenderInfo(int _id, MagicRenderInfo* _info);
	//	人物效果信息
	bool GetEffectHumInfo(int _id, EffectHumInfo** _ppEffectHumInfo);
	bool GetEffectHumInfo(int _id, EffectHumInfo* _pEffectHumInfo);
	//	物品描述信息
	bool GetItemDesc(int _id, ItemDesc* _pDesc);
	bool GetItemDescINI(int _id, ItemDesc* _pDesc);
	bool GetItemDescSQL(int _id, ItemDesc* _pDesc);
	// 物品等级信息
	int GetItemGradeInFullAttrib(int id);
	ItemExtraAttribList *GetItemExtraSuitAttribList(int nSuitID);

	//	NPC脚本信息

	//	商店物品信息
	bool GetShopSellItems(ItemList* _list, BYTE _type, int _minlevel, int _maxlevel);
	//	商品价格信息
	int GetItemPrice(ItemAttrib* _item);
	//	魔法消耗
	bool InitMgcCostTable();

	void ReloadRenderInfo();
	void ReloadMagicRenderInfo();

	// 获取所有Item id
	bool GetItemIDs(std::vector<int>& _refItemIDs);
	bool GetItemIDsINI(std::vector<int>& _refItemIDs);
	bool GetItemIDsSQL(std::vector<int>& _refItemIDs);

	bool GetMonsterIDs(std::vector<int>& _refMonsIDs);
	bool GetMonsterIDsINI(std::vector<int>& _refMonsIDs);
	bool GetMonsterIDsSQL(std::vector<int>& _refMonsIDs);

public:
	bool CanUseSQL() const;
	bool DataFromLua(lua_State *L);

 public:
 	static GameInfoManager* GetInstance()
 	{
 		static GameInfoManager* pIns = NULL;
 		if(!pIns)
 		{
 			pIns = new GameInfoManager;
 		}
 		return pIns;
 	}

private:
	static int DBCallbackMonsID(void* _pParam, int _nCnt, char **_value, char** _name);
	static int DBCallbackItemID(void* _pParam, int _nCnt, char **_value, char** _name);
	static int DBCallbackItemAttrib(void* _param, int _count, char** _value,char** _name);
	static int DBCallbackItemDesc(void* _param, int _count, char** _value,char** _name);
	static int DBCallbackShopItems(void* _param, int _count, char** _value,char** _name);
	static int DBCallbackMagicCost(void* _param, int _count, char** _value,char** _name);

private:
	bool m_bLuaConfig;
	std::map<int, ItemFullAttrib> m_xItemFullAttribMap;
	std::map<int, MonsFullAttrib> m_xMonsFullAttribMap;
	std::map<int, ItemExtraAttribList*> m_xItemExtraSuitAttribMap;

	sqlite3* m_sql;
	std::map<int, RenderInfo*> m_xMonsterRenderInfo;
	std::map<int, MagicRenderInfo*> m_xMagicRenderInfo;
	std::map<int, EffectHumInfo*> m_xEffectHumInfo;
	const ItemDesc* m_pItemDescCache[ITEM_ID_MAX];
};

#endif