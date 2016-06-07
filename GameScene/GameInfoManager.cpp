#include "../GameScene/GameInfoManager.h"
#include <stdio.h>
#include "../Common/SKnl3Helper.h"
#include "../Common/GlobalFunction.h"
#include <hge.h>
//#include "../GameScene/GameMagicManager.h"
#include "../GameScene/MagicEffect.h"
#include <io.h>
#include <assert.h>
#include "../GameScene/GameScene.h"
#ifdef _THEMIDA_
#include "../Themida/ThemidaSDK.h"
#endif

#pragma comment(lib, "sqlite3.lib")

// #if _MSC_VER >= 1500
// 
// #ifdef _DEBUG
// #pragma comment(lib, "json_vc71_libmtd.lib")
// #else
// #pragma comment(lib, "json_vc71_libmt.lib")
// #endif
// 
// #else if _MSC_VER >= 1310
// 
// #ifdef _DEBUG
// #pragma comment(lib, "json_vc71_libmtd.lib")
// #else
// #pragma comment(lib, "json_vc71_libmt.lib")
// #endif
// 
// #endif

static const char* g_szAttribName[] = 
{
	"HP","MP","reqValue","reqType","weight","lucky","DC","maxDC","sex","atkSpeed",
		"AC","maxAC"
};

static ItemDesc s_stDefItemDesc = {"", 0, 0, 0};

GameInfoManager::GameInfoManager()
{
#ifdef _THEMIDA_
	VM_START
#endif
	m_sql = NULL;
	//	判断数据库文件是否存在 存在启用数据库 不存在用INI文件存取
	char szDBFile[MAX_PATH];
	SKNL3GetCurrentAppPath(szDBFile, sizeof(szDBFile));
	//sprintf(szDBFile, "%s%s", szDBFile, "Help\\legend.bm");
	strcat(szDBFile, "Help\\legend.bm");
	if(0 == access(szDBFile, 0))
	{
		//	存在
		//	frist open without password
		if(SQLITE_OK == sqlite3_open(szDBFile, &m_sql))
		{
			//	文件打开成功 输入密码
//#ifdef NDEBUG
			DWORD dwPsw[2];
			char* pWrt = (char*)dwPsw;
			pWrt[5] = 0;
			pWrt[4] = 'G';
			pWrt[3] = 'e';
			pWrt[2] = '3';
			pWrt[1] = 'S';
			pWrt[0] = 'x';
			if(SQLITE_OK == sqlite3_key(m_sql, pWrt, 5))
			{

			}
			else
			{
				AfxGetHge()->System_Log("Error on openning database!");
				sqlite3_close(m_sql);
				m_sql = NULL;
			}
//#endif
// 			if(SQLITE_OK == sqlite3_key(m_sql, "makes", 5))
// 			{
// 				//	OK
// 			}
// 			else
// 			{
// 				//	密码错
// 				AfxGetHge()->System_Log("数据库密码错误");
// 				sqlite3_close(m_sql);
// 				m_sql = NULL;
// 			}
		}
		else
		{
			m_sql = NULL;
		}
	}
	MagicRenderInfo* info = new MagicRenderInfo;
	ZeroMemory(info, sizeof(info));
	m_xMagicRenderInfo.insert(std::make_pair(0, info));

	ZeroMemory(m_pItemDescCache, sizeof(m_pItemDescCache));

#ifdef _THEMIDA_
	VM_END
#endif
}

GameInfoManager::~GameInfoManager()
{
	if(m_sql)
	{
		sqlite3_close(m_sql);
	}
}

bool GameInfoManager::CanUseSQL() const
{
	return m_sql != NULL;
}

bool GameInfoManager::GetItemDesc(int _id, ItemDesc* _pDesc)
{
	*_pDesc = s_stDefItemDesc;

	if(_id < 0 ||
		_id >= ITEM_ID_MAX)
	{
		return false;
	}

	//	查找缓存
	const ItemDesc* pDesc = m_pItemDescCache[_id];
	if(NULL != pDesc)
	{
		memcpy(_pDesc, pDesc, sizeof(ItemDesc));
		return true;
	}

	if(m_sql)
	{
		return GetItemDescSQL(_id, _pDesc);
	}
	else
	{
		return GetItemDescINI(_id, _pDesc);
	}
}

bool GameInfoManager::GetItemDescINI(int _id, ItemDesc* _pDesc)
{
	char buf[256];
	char szId[10];
	char szPath[MAX_PATH];
	//SKNL3GetCurrentAppPath(szPath, MAX_PATH);
	//sprintf(szPath, "%sConfig\\items.ini", szPath);
	sprintf(szPath, "%s\\Config\\items.ini", GetRootPath());
	sprintf(szId, "%d", _id);

	::GetPrivateProfileString(szId, "name", "", buf, sizeof(buf), szPath);
	if(strlen(buf) == 0)
	{
		//	写入默认缓存
		m_pItemDescCache[_id] = &s_stDefItemDesc;
		return false;
	}

	DWORD dwRead = ::GetPrivateProfileString(szId, "desc", "", buf, sizeof(buf), szPath);

	if(dwRead == 0 ||
		buf[0] == 0)
	{
		//	写入默认缓存
		m_pItemDescCache[_id] = &s_stDefItemDesc;
		return false;
	}

	size_t uDescLength = strlen(buf);
	char* pszDesc = new char[uDescLength + 1];
	strcpy(pszDesc, buf);

	ItemDesc* pDesc = new ItemDesc;
	pDesc->pszDesc = pszDesc;
	pDesc->nTextLength = uDescLength;
	pDesc->nTextLine = GetTextLine(pszDesc, 12, ITEM_DESC_TEXT_WIDTH);
	pDesc->nTextWidth = GetTextWidth(pszDesc, 12);
	m_pItemDescCache[_id] = pDesc;
	*_pDesc = *pDesc;

	return true;
}

bool GameInfoManager::GetItemDescSQL(int _id, ItemDesc* _pDesc)
{
	char szExpr[MAX_PATH];
	sprintf(szExpr, "select * from ItemsDesc where id = %d", _id);
	char* pErr = NULL;
	if(SQLITE_OK != sqlite3_exec(m_sql, szExpr, &GameInfoManager::DBCallbackItemDesc, _pDesc, &pErr))
	{
		AfxGetHge()->System_Log("数据库查询[%d]装备失败,%s", _id, pErr);
		sqlite3_free(pErr);
		m_pItemDescCache[_id] = &s_stDefItemDesc;
		return false;
	}

	//	ok
	if(_pDesc->nTextWidth == 0)
	{
		//	not have desc
		m_pItemDescCache[_id] = &s_stDefItemDesc;
		return false;
	}
	else
	{
		ItemDesc* pDesc = new ItemDesc;
		*pDesc = *_pDesc;
		m_pItemDescCache[_id] = pDesc;
		return true;
	}
}

bool GameInfoManager::GetItemAttrib(int _id, ItemAttrib* _pitem)
{
	if(_id < 0)
	{
		return false;
	}

	if(m_sql)
	{
		return GetItemAttribSQL(_id, _pitem);
	}
	else
	{
		return GetItemAttribINI(_id, _pitem);
	}
}

bool GameInfoManager::GetItemAttribINI(int _id, ItemAttrib* _pitem)
{
	char buf[100];
	char szId[10];
	char szPath[MAX_PATH];
	//SKNL3GetCurrentAppPath(szPath, MAX_PATH);
	//sprintf(szPath, "%sConfig\\items.ini", szPath);
	sprintf(szPath, "%s\\Config\\items.ini", GetRootPath());
	sprintf(szId, "%d", _id);
	ZeroMemory(_pitem, sizeof(ItemAttrib));
	_pitem->id = _id;
	bool bGetName = false;

	::GetPrivateProfileString(szId, "name", "", buf, sizeof(buf), szPath);
	if(strlen(buf) == 0)
	{
		return false;
	}
	else
	{
		strcpy(_pitem->name, buf);
	}

	//for(int i = 0; i < sizeof(g_szAttribName) / 4; ++i)
	{
		_pitem->HP = ::GetPrivateProfileInt(szId, "HP", 0, szPath);
		_pitem->MP = ::GetPrivateProfileInt(szId, "MP", 0, szPath);
		_pitem->reqValue = ::GetPrivateProfileInt(szId, "reqValue", 0, szPath);
		_pitem->reqType = ::GetPrivateProfileInt(szId, "reqType", 0, szPath);
		_pitem->weight = ::GetPrivateProfileInt(szId, "weight", 0, szPath);
		_pitem->lucky = ::GetPrivateProfileInt(szId, "lucky", 0, szPath);
		_pitem->DC = ::GetPrivateProfileInt(szId, "DC", 0, szPath);
		_pitem->maxDC = ::GetPrivateProfileInt(szId, "maxDC", 0, szPath);
		_pitem->sex = ::GetPrivateProfileInt(szId, "sex", 0, szPath);
		_pitem->atkSpeed = ::GetPrivateProfileInt(szId, "atkSpeed", 0, szPath);
		_pitem->AC = ::GetPrivateProfileInt(szId, "AC", 0, szPath);
		_pitem->maxAC = ::GetPrivateProfileInt(szId, "maxAC", 0, szPath);
		_pitem->SC = ::GetPrivateProfileInt(szId, "SC", 0, szPath);
		_pitem->maxSC = ::GetPrivateProfileInt(szId, "maxSC", 0, szPath);
		_pitem->MC = ::GetPrivateProfileInt(szId, "MC", 0, szPath);
		_pitem->maxMC = ::GetPrivateProfileInt(szId, "maxMC", 0, szPath);
		_pitem->MAC = ::GetPrivateProfileInt(szId, "MAC", 0, szPath);
		_pitem->maxMAC = ::GetPrivateProfileInt(szId, "maxMAC", 0, szPath);
		_pitem->atkPalsy = ::GetPrivateProfileInt(szId, "atkPalsy", 0, szPath);
		_pitem->atkPois = ::GetPrivateProfileInt(szId, "atkPois", 0, szPath);
		_pitem->moveSpeed = ::GetPrivateProfileInt(szId, "moveSpeed", 0, szPath);
		_pitem->type = ::GetPrivateProfileInt(szId, "type", 0, szPath);
		_pitem->tex = ::GetPrivateProfileInt(szId, "tex", 0, szPath);
		_pitem->accuracy = ::GetPrivateProfileInt(szId, "accuracy", 0, szPath);
	}

	return true;
}

bool GameInfoManager::GetItemAttribSQL(int _id, ItemAttrib* _pitem)
{
	char szExpr[MAX_PATH];
	sprintf(szExpr, "select * from Items where id = %d", _id);
	char* pErr = NULL;
	if(SQLITE_OK != sqlite3_exec(m_sql, szExpr, &GameInfoManager::DBCallbackItemAttrib, _pitem, &pErr))
	{
		AfxGetHge()->System_Log("数据库查询[%d]装备失败,%s", _id, pErr);
		return false;
	}

	return true;
}

bool GameInfoManager::InitMgcCostTable()
{
	char szExpr[MAX_PATH];
	sprintf(szExpr, "select * from Magics");
	char* pErr = NULL;
	if(SQLITE_OK != sqlite3_exec(m_sql, szExpr, &GameInfoManager::DBCallbackMagicCost, NULL, &pErr))
	{
		AfxGetHge()->System_Log("魔法数据获取失败,%s", pErr);
		return false;
	}

	return true;
}

bool GameInfoManager::GetMonsterItems(int _id, int* _sum, ItemAttrib** _items)
{
	char buf[100];
	char szId[10];
	char szPath[MAX_PATH];
	//SKNL3GetCurrentAppPath(szPath, MAX_PATH);
	//sprintf(szPath, "%sConfig\\iteminmons.ini", szPath);
	sprintf(szPath, "%s\\Config\\iteminmons.ini", GetRootPath());
	sprintf(szId, "%d", _id);
	*_sum = 0;

	int nSum = ::GetPrivateProfileInt(szId, "sum", 0, szPath);
	if(0 == nSum)
	{
		return false;
	}

	if(nSum == 0)
	{
		return true;
	}
	*_items = new ItemAttrib[nSum];
	ZeroMemory(*_items, sizeof(ItemAttrib) * nSum);
	int nType = 0;
	int nProb = 0;
	int nRandom = 0;

	for(int i = 0; i < nSum; ++i)
	{
		sprintf(buf, "%dtype", i);
		nType = ::GetPrivateProfileInt(szId, buf, 0, szPath);
		sprintf(buf, "%dprob", i);
		nProb = ::GetPrivateProfileInt(szId, buf, 0, szPath);

		nRandom = AfxGetHge()->Random_Int(0, 1000);
		if(nRandom <= nProb)
		{
			GetItemAttrib(nType, &(*_items)[*_sum]);
			++(*_sum);
		}
	}
	return true;
}

bool GameInfoManager::GetMonsterAttrib(int _id, ItemAttrib* _pitem)
{
	if(_id < 0)
	{
		return false;
	}

	if(m_sql)
	{
		return GetMonsterAttribSQL(_id, _pitem);
	}
	else
	{
		return GetMonsterAttribINI(_id, _pitem);
	}
}

bool GameInfoManager::GetMonsterAttribINI(int _id, ItemAttrib* _pitem)
{
	char buf[100];
	char szId[10];
	char szPath[MAX_PATH];
	//SKNL3GetCurrentAppPath(szPath, MAX_PATH);
	//sprintf(szPath, "%sConfig\\monsa.ini", szPath);
	sprintf(szPath, "%s\\Config\\monsa.ini", GetRootPath());
	sprintf(szId, "%d", _id);
	ZeroMemory(_pitem, sizeof(ItemAttrib));
	_pitem->id = _id;

	::GetPrivateProfileString(szId, "name", "", buf, sizeof(buf), szPath);
	if(strlen(buf) == 0)
	{
		return false;
	}
	else
	{
		strcpy(_pitem->name, buf);
	}

	//for(int i = 0; i < sizeof(g_szAttribName) / 4; ++i)
	{
		_pitem->HP = ::GetPrivateProfileInt(szId, "HP", 0, szPath);
		_pitem->maxHP = ::GetPrivateProfileInt(szId, "maxHP", 0, szPath);
		_pitem->MP = ::GetPrivateProfileInt(szId, "MP", 0, szPath);
		_pitem->maxMP = ::GetPrivateProfileInt(szId, "maxMP", 0, szPath);
		_pitem->reqValue = ::GetPrivateProfileInt(szId, "reqValue", 0, szPath);
		_pitem->reqType = ::GetPrivateProfileInt(szId, "reqType", 0, szPath);
		_pitem->weight = ::GetPrivateProfileInt(szId, "weight", 0, szPath);
		_pitem->lucky = ::GetPrivateProfileInt(szId, "lucky", 0, szPath);
		_pitem->DC = ::GetPrivateProfileInt(szId, "DC", 0, szPath);
		_pitem->maxDC = ::GetPrivateProfileInt(szId, "maxDC", 0, szPath);
		_pitem->sex = ::GetPrivateProfileInt(szId, "sex", 0, szPath);
		_pitem->atkSpeed = ::GetPrivateProfileInt(szId, "atkSpeed", 0, szPath);
		_pitem->AC = ::GetPrivateProfileInt(szId, "AC", 0, szPath);
		_pitem->maxAC = ::GetPrivateProfileInt(szId, "maxAC", 0, szPath);
		_pitem->SC = ::GetPrivateProfileInt(szId, "SC", 0, szPath);
		_pitem->maxSC = ::GetPrivateProfileInt(szId, "maxSC", 0, szPath);
		_pitem->MC = ::GetPrivateProfileInt(szId, "MC", 0, szPath);
		_pitem->maxMC = ::GetPrivateProfileInt(szId, "maxMC", 0, szPath);
		_pitem->MAC = ::GetPrivateProfileInt(szId, "MAC", 0, szPath);
		_pitem->maxMAC = ::GetPrivateProfileInt(szId, "maxMAC", 0, szPath);
		_pitem->atkPalsy = ::GetPrivateProfileInt(szId, "atkPalsy", 0, szPath);
		_pitem->atkPois = ::GetPrivateProfileInt(szId, "atkPois", 0, szPath);
		_pitem->moveSpeed = ::GetPrivateProfileInt(szId, "moveSpeed", 0, szPath);
		_pitem->type = ::GetPrivateProfileInt(szId, "type", 0, szPath);
		_pitem->tex = ::GetPrivateProfileInt(szId, "tex", 0, szPath);
	}
	return true;
}

bool GameInfoManager::GetMonsterAttribSQL(int _id, ItemAttrib* _pitem)
{
	char szExpr[MAX_PATH];
	sprintf(szExpr, "select * from Monsters where id = %d", _id);
	char* pErr = NULL;
	if(SQLITE_OK != sqlite3_exec(m_sql, szExpr, &GameInfoManager::DBCallbackItemAttrib, _pitem, &pErr))
	{
		AfxGetHge()->System_Log("数据库查询[%d]怪物失败,%s", _id, pErr);
		return false;
	}

	return true;
}

void GameInfoManager::ReloadRenderInfo()
{
	std::map<int, RenderInfo*>::const_iterator begIter = m_xMonsterRenderInfo.begin();
	RenderInfo* pRenderInfo = NULL;

	for(begIter;
		begIter != m_xMonsterRenderInfo.end();
		++begIter)
	{
		pRenderInfo = begIter->second;
		GetMonsterRenderInfo(pRenderInfo->id, pRenderInfo);
	}
}

bool GameInfoManager::GetMonsterRenderInfo(int _id, const RenderInfo** _info)
{
	RenderInfo** pOut = const_cast<RenderInfo**>(_info);

	std::map<int, RenderInfo*>::const_iterator iter = m_xMonsterRenderInfo.find(_id);
	if(iter != m_xMonsterRenderInfo.end())
	{
		*pOut = iter->second;
		return true;
	}
	else
	{
		RenderInfo* pInfo = new RenderInfo;
		ZeroMemory(pInfo, sizeof(RenderInfo));
		if(GetMonsterRenderInfo(_id, pInfo))
		{
			*pOut = pInfo;
			m_xMonsterRenderInfo.insert(std::make_pair(_id, pInfo));
			return true;
		}
		else
		{
			delete pInfo;
		}
	}
	return false;
}

bool GameInfoManager::GetMonsterRenderInfo(int _id, RenderInfo* _info)
{
	//char buf[100];
	char szId[10];
	char szPath[MAX_PATH];

	sprintf(szPath, "%s\\Config\\mons.ini", GetRootPath());
	sprintf(szId, "%d", _id);
	ZeroMemory(_info, sizeof(RenderInfo));
	_info->wil = ::GetPrivateProfileInt(szId, "wil", -1, szPath);
	if(_info->wil == -1)
	{
		return false;
	}

	{
		_info->id = _id;
		_info->atkbegin = ::GetPrivateProfileInt(szId, "atkbegin", 0, szPath);
		_info->atkframe = ::GetPrivateProfileInt(szId, "atkframe", 0, szPath);
		_info->atktotal = ::GetPrivateProfileInt(szId, "atktotal", 0, szPath);
		_info->deadbegin = ::GetPrivateProfileInt(szId, "deadbegin", 0, szPath);
		_info->deadframe = ::GetPrivateProfileInt(szId, "deadframe", 0, szPath);
		_info->deadtotal = ::GetPrivateProfileInt(szId, "deadtotal", 0, szPath);
		_info->hurtbegin = ::GetPrivateProfileInt(szId, "hurtbegin", 0, szPath);
		_info->hurtframe = ::GetPrivateProfileInt(szId, "hurtframe", 0, szPath);
		_info->hurttotal = ::GetPrivateProfileInt(szId, "hurttotal", 0, szPath);
		_info->magicbegin = ::GetPrivateProfileInt(szId, "magicbegin", 0, szPath);
		_info->magicframe = ::GetPrivateProfileInt(szId, "magicframe", 0, szPath);
		_info->magictotal = ::GetPrivateProfileInt(szId, "magictotal", 0, szPath);
		_info->standbegin = ::GetPrivateProfileInt(szId, "standbegin", 0, szPath);
		_info->standframe = ::GetPrivateProfileInt(szId, "standframe", 0, szPath);
		_info->standtotal = ::GetPrivateProfileInt(szId, "standtotal", 0, szPath);
		_info->walkbegin = ::GetPrivateProfileInt(szId, "walkbegin", 0, szPath);
		_info->walkframe = ::GetPrivateProfileInt(szId, "walkframe", 0, szPath);
		_info->walktotal = ::GetPrivateProfileInt(szId, "walktotal", 0, szPath);
		_info->atkeftbegin = ::GetPrivateProfileInt(szId, "atkeftbegin", 0, szPath);
		_info->atkeftframe = ::GetPrivateProfileInt(szId, "atkeftframe", 0, szPath);
		_info->atkefttotal = ::GetPrivateProfileInt(szId, "atkefttotal", 0, szPath);
		_info->drts = ::GetPrivateProfileInt(szId, "drts", 0, szPath);
		_info->index = ::GetPrivateProfileInt(szId, "index", 0, szPath);
		_info->showbegin = ::GetPrivateProfileInt(szId, "showbegin", 0, szPath);
		_info->showframe = ::GetPrivateProfileInt(szId, "showframe", 0, szPath);
		_info->sndindex = ::GetPrivateProfileInt(szId, "sndindex", 0, szPath);
	}
	return true;
}

void GameInfoManager::ReloadMagicRenderInfo()
{
	std::map<int, MagicRenderInfo*>::const_iterator begIter = m_xMagicRenderInfo.begin();
	MagicRenderInfo* pRenderInfo = NULL;

	for(begIter;
		begIter != m_xMagicRenderInfo.end();
		++begIter)
	{
		pRenderInfo = begIter->second;
		GetMagicRenderInfo(pRenderInfo->id, pRenderInfo);
	}
}

bool GameInfoManager::GetMagicRenderInfo(int _id, const MagicRenderInfo** _info)
{
	MagicRenderInfo** pOut = const_cast<MagicRenderInfo**>(_info);

	std::map<int, MagicRenderInfo*>::const_iterator iter = m_xMagicRenderInfo.find(_id);
	if(iter != m_xMagicRenderInfo.end())
	{
		*pOut = iter->second;
		return true;
	}
	else
	{
		MagicRenderInfo* pInfo = new MagicRenderInfo;
		ZeroMemory(pInfo, sizeof(MagicRenderInfo));
		if(GetMagicRenderInfo(_id, pInfo))
		{
			*pOut = pInfo;
			m_xMagicRenderInfo.insert(std::make_pair(_id, pInfo));
			return true;
		}
		else
		{
			*pOut = NULL;
			delete pInfo;
		}
	}
	return false;
}

bool GameInfoManager::GetMagicRenderInfo(int _id, MagicRenderInfo* _info)
{
		//char buf[100];
		char szId[10];
		char szPath[MAX_PATH];

		sprintf(szPath, "%s\\Config\\magic.ini", GetRootPath());
		sprintf(szId, "%d", _id);
		ZeroMemory(_info, sizeof(MagicRenderInfo));
		_info->wil = ::GetPrivateProfileInt(szId, "wil", 0xFF, szPath);
		_info->id = _id;
		if(_info->wil == 0xFF)
		{
			return false;
		}

		
		{
			_info->selfbegin = ::GetPrivateProfileInt(szId, "selfbegin", 0, szPath);
			_info->selfframe = ::GetPrivateProfileInt(szId, "selfframe", 0, szPath);
			_info->selftotal = ::GetPrivateProfileInt(szId, "selftotal", 0, szPath);
			_info->waybegin = ::GetPrivateProfileInt(szId, "waybegin", 0, szPath);
			_info->wayframe = ::GetPrivateProfileInt(szId, "wayframe", 0, szPath);
			_info->waytotal = ::GetPrivateProfileInt(szId, "waytotal", 0, szPath);
			_info->destbegin = ::GetPrivateProfileInt(szId, "destbegin", 0, szPath);
			_info->destframe = ::GetPrivateProfileInt(szId, "destframe", 0, szPath);
			_info->desttotal = ::GetPrivateProfileInt(szId, "desttotal", 0, szPath);
			_info->speed = ::GetPrivateProfileInt(szId, "speed", 0, szPath);
			_info->sndindex = ::GetPrivateProfileInt(szId, "sndindex", 0, szPath);
		}
		
		return true;
}

bool GameInfoManager::GetEffectHumInfo(int _id, EffectHumInfo** _ppEffectHumInfo)
{
	EffectHumInfo** pOut = _ppEffectHumInfo;

	std::map<int, EffectHumInfo*>::const_iterator iter = m_xEffectHumInfo.find(_id);
	if(iter != m_xEffectHumInfo.end())
	{
		*pOut = iter->second;
		return true;
	}
	else
	{
		EffectHumInfo* pInfo = new EffectHumInfo;
		ZeroMemory(pInfo, sizeof(EffectHumInfo));
		if(GetEffectHumInfo(_id, pInfo))
		{
			*pOut = pInfo;
			m_xEffectHumInfo.insert(std::make_pair(_id, pInfo));
			return true;
		}
		else
		{
			*pOut = NULL;
			delete pInfo;
		}
	}
	return false;
}

bool GameInfoManager::GetEffectHumInfo(int _id, EffectHumInfo* _pEffectHumInfo)
{
	//char buf[100];
	char szId[10];
	char szPath[MAX_PATH];

	sprintf(szPath, "%s\\Config\\humeffect.ini", GetRootPath());
	sprintf(szId, "%d", _id);
	ZeroMemory(_pEffectHumInfo, sizeof(EffectHumInfo));
	_pEffectHumInfo->wil = ::GetPrivateProfileInt(szId, "wil", 0xFF, szPath);
	_pEffectHumInfo->id = _id;
	if(_pEffectHumInfo->wil == 0xFF)
	{
		return false;
	}


	{
		_pEffectHumInfo->effectbegin = ::GetPrivateProfileInt(szId, "effectbegin", 0, szPath);
		_pEffectHumInfo->effectframe = ::GetPrivateProfileInt(szId, "effectframe", 0, szPath);
		_pEffectHumInfo->speed = ::GetPrivateProfileInt(szId, "speed", 0, szPath);
	}

	return true;
}

/*
bool GameInfoManager::GetMgcInfo(int _id, MagicInfo* _mgc)
{
	//char buf[100];
	char szId[10];
	char szPath[MAX_PATH];
	//SKNL3GetCurrentAppPath(szPath, MAX_PATH);
	//sprintf(szPath, "%sConfig\\magic.ini", szPath);
	sprintf(szPath, "%s\\Config\\magic.ini", GetRootPath());
	sprintf(szId, "%d", _id);

	_mgc->mgcfileindex = ::GetPrivateProfileInt(szId, "mgcfileindex", 255, szPath);
	if(_mgc->mgcfileindex == 255)
	{
		return false;
	}
	_mgc->id = _id;
	_mgc->selfbegin = ::GetPrivateProfileInt(szId, "selfbegin", 0, szPath);
	_mgc->selftotal = ::GetPrivateProfileInt(szId, "selftotal", 0, szPath);
	_mgc->waybegin = ::GetPrivateProfileInt(szId, "waybegin", 0, szPath);
	_mgc->waytotal = ::GetPrivateProfileInt(szId, "waytotal", 0, szPath); 
	_mgc->destbegin = ::GetPrivateProfileInt(szId, "destbegin", 0, szPath);
	_mgc->desttotal = ::GetPrivateProfileInt(szId, "desttotal", 0, szPath); 
	return true;
}*/


bool GameInfoManager::GetMiniMapIndex(const char* lpMapName, int* _pvalue)
{
	bool bIsFullPath = false;
	for(int i = 0; i < strlen(lpMapName); ++i)
	{
		if(lpMapName[i] == '\\')
		{
			bIsFullPath = true;
			break;
		}
	}

	char szBuf[MAX_PATH] = {0};
	if(bIsFullPath)
	{
		int nName = 0;
		for(int i = strlen(lpMapName) - 1; i >= 0; --i)
		{
			if(lpMapName[i] == '\\')
			{
				break;
			}
			++nName;
		}

		for(int i = strlen(lpMapName) - nName + 1; i < strlen(lpMapName) + 1; ++i)
		{
			szBuf[i - strlen(lpMapName) + nName - 1] = lpMapName[i];
		}
	}
	else
	{
		strcpy(szBuf, lpMapName);
	}

	char szPath[MAX_PATH] = {0};
	//SKNL3GetCurrentAppPath(szPath, MAX_PATH);
	//sprintf(szPath, "%sConfig\\minimap.ini", szPath);
	sprintf(szPath, "%s\\Config\\minimap.ini", GetRootPath());
	*_pvalue = ::GetPrivateProfileInt("MiniMap", szBuf, -1, szPath);
	if(*_pvalue == -1)
	{
		return false;
	}
	return true;
}

int GameInfoManager::DBCallbackItemDesc(void* _param, int _count, char** _value,char** _name)
{
	assert(_count == 2);
	if(_count != 2)
	{
		AfxGetHge()->System_Log("查询装备描述列数出错");
		return -1;
	}

	ItemDesc* pItem = (ItemDesc*)_param;
	if(_value[1][0] == 0)
	{
		return -1;
	}

	size_t uLength = strlen(_value[1]);
	char* pszDesc = new char[uLength + 1];
	strcpy(pszDesc, _value[1]);
	pItem->pszDesc = pszDesc;
	pItem->nTextLength = uLength;
	pItem->nTextWidth = GetTextWidth(pszDesc, 12);
	pItem->nTextLine = GetTextLine(pszDesc, 12, ITEM_DESC_TEXT_WIDTH);

	return 0;
}

int GameInfoManager::DBCallbackItemAttrib(void* _param, int _count, char** _value,char** _name)
{
	assert(_count == 35);
	if(_count < 35)
	{
		AfxGetHge()->System_Log("查询装备列数出错");
		return -1;
	}

	ItemAttrib* pItem = (ItemAttrib*)_param;

	pItem->id		 = atoi(_value[0]);
	strcpy(pItem->name, _value[1]);
	pItem->lucky	 = atoi(_value[2]);
	pItem->curse	 = atoi(_value[3]);
	pItem->hide		 = atoi(_value[4]);
	pItem->accuracy	 = atoi(_value[5]);
	pItem->atkSpeed	 = atoi(_value[6]);
	pItem->atkPalsy  = atoi(_value[7]);
	pItem->atkPois  = atoi(_value[8]);
	pItem->moveSpeed  = atoi(_value[9]);
	pItem->weight  = atoi(_value[10]);
	pItem->reqType  = atoi(_value[11]);
	pItem->reqValue  = atoi(_value[12]);
	pItem->sex  = atoi(_value[13]);
	pItem->type  = atoi(_value[14]);
	pItem->maxDC  = atoi(_value[15]);
	pItem->DC  = atoi(_value[16]);
	pItem->maxAC  = atoi(_value[17]);
	pItem->AC  = atoi(_value[18]);
	pItem->maxMAC  = atoi(_value[19]);
	pItem->MAC  = atoi(_value[20]);
	pItem->maxSC  = atoi(_value[21]);
	pItem->SC  = atoi(_value[22]);
	pItem->maxMC  = atoi(_value[23]);
	pItem->MC  = atoi(_value[24]);
	pItem->maxHP  = atoi(_value[25]);
	pItem->HP  = atoi(_value[26]);
	pItem->maxMP  = atoi(_value[27]);
	pItem->MP  = atoi(_value[28]);
	pItem->maxEXPR	= atoi(_value[29]);
	pItem->EXPR  = atoi(_value[30]);
	pItem->level  = atoi(_value[31]);
	pItem->extra  = atoi(_value[32]);
	pItem->tex  = atoi(_value[33]);
	pItem->price = atoi(_value[34]);

	return 0;
}

bool GameInfoManager::GetShopSellItems(ItemList* _list, BYTE _type, int _minlevel, int _maxlevel)
{
	//	
	char szExpr[MAX_PATH];
	sprintf(szExpr, "select * from Items where type = %d and reqtype = 1 and reqvalue >= %d and reqvalue <= %d",
		_type,
		_minlevel,
		_maxlevel);

	char* pErr = NULL;
	if(SQLITE_OK != sqlite3_exec(m_sql, szExpr, &GameInfoManager::DBCallbackShopItems, _list, &pErr))
	{
		AfxGetHge()->System_Log("数据库查询[%d]商店信息失败,%s", _type, pErr);
		return false;
	}

	return true;
}


int GameInfoManager::DBCallbackShopItems(void* _param, int _count, char** _value,char** _name)
{
	assert(_count == 34);
	if(_count < 34)
	{
		AfxGetHge()->System_Log("查询装备列数出错");
		return -1;
	}

	ItemList* pList = (ItemList*)_param;
	ItemAttrib item;
	ItemAttrib* pItem = &item;

	pItem->id		 = atoi(_value[0]);
	strcpy(pItem->name, _value[1]);
	pItem->lucky	 = atoi(_value[2]);
	pItem->curse	 = atoi(_value[3]);
	pItem->hide		 = atoi(_value[4]);
	pItem->accuracy	 = atoi(_value[5]);
	pItem->atkSpeed	 = atoi(_value[6]);
	pItem->atkPalsy  = atoi(_value[7]);
	pItem->atkPois  = atoi(_value[8]);
	pItem->moveSpeed  = atoi(_value[9]);
	pItem->weight  = atoi(_value[10]);
	pItem->reqType  = atoi(_value[11]);
	pItem->reqValue  = atoi(_value[12]);
	pItem->sex  = atoi(_value[13]);
	pItem->type  = atoi(_value[14]);
	pItem->maxDC  = atoi(_value[15]);
	pItem->DC  = atoi(_value[16]);
	pItem->maxAC  = atoi(_value[17]);
	pItem->AC  = atoi(_value[18]);
	pItem->maxMAC  = atoi(_value[19]);
	pItem->MAC  = atoi(_value[20]);
	pItem->maxSC  = atoi(_value[21]);
	pItem->SC  = atoi(_value[22]);
	pItem->maxMC  = atoi(_value[23]);
	pItem->MC  = atoi(_value[24]);
	pItem->maxHP  = atoi(_value[25]);
	pItem->HP  = atoi(_value[26]);
	pItem->maxMP  = atoi(_value[27]);
	pItem->MP  = atoi(_value[28]);
	pItem->maxEXPR	= atoi(_value[29]);
	pItem->EXPR  = atoi(_value[30]);
	pItem->level  = atoi(_value[31]);
	pItem->extra  = atoi(_value[32]);
	pItem->tex  = atoi(_value[33]);

	pList->push_back(item);

	return 0;
}

int GameInfoManager::GetItemPrice(ItemAttrib* _item)
{
	int nPrice = 0;

	switch(_item->reqType)
	{
	case REQ_LEVEL:
		{
			nPrice = _item->reqValue * 1000;
		}break;
	case REQ_DC:
	case REQ_MC:
	case REQ_SC:
		{
			nPrice = _item->reqValue * 2000;
		}break;
	}

	return nPrice;
}

int GameInfoManager::DBCallbackMagicCost(void* _param, int _count, char** _value,char** _name)
{
	assert(_count == 13);
	if(_count < 13)
	{
		AfxGetHge()->System_Log("查询魔法列数出错");
		return -1;
	}

	int nID = atoi(_value[0]);
	if(nID >= 0 &&
		nID < MEFF_USERTOTAL)
	{
		GameScene::sThis->m_dwMgcCostTable[nID] = atoi(_value[11]);
		GameScene::sThis->m_dwMgcDelayTable[nID] = atoi(_value[12]);
	}

	return 0;
}