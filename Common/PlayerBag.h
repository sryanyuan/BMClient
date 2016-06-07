#ifndef PLAYERBAG_H_
#define PLAYERBAG_H_
#include "GamePlayer.h"
#include "GameInfoManager.h"
#include "GameRenderObject.h"
#include "../../CommonModule/ShareDefine.h"

//	Íæ¼Ò±³°ü
class PlayerBag
{
public:
	PlayerBag() : 
	  m_items(HERO_MAINBAG_SIZE_CUR),
	  m_assistItems(HERO_CLIENT_ASSIST_SIZE)
	{
		for(size_t i = 0; i < m_items.size(); ++i)
		{
			memset(&m_items[i], 0, sizeof(ItemAttrib));
			//m_items[i].id = INVALID_ITEM_ID;
			//m_items[i].type = ITEM_NO;
		}
		for(size_t i = 0; i < m_assistItems.size(); ++i)
		{
			memset(&m_assistItems[i], 0, sizeof(ItemAttrib));
			//m_assistItems[i].id = INVALID_ITEM_ID;
			//m_assistItems[i].type = ITEM_NO;
		}

		//m_items[35].id = 57;
		m_nMoney = 0;
		m_nCounter = 0;

#ifdef _DEBUG
		//DebugTest();
#endif
	}
	~PlayerBag()
	{

	}

public:
	void Clear()
	{
		for(size_t i = 0; i < m_items.size(); ++i)
		{
			memset(&m_items[i], 0, sizeof(ItemAttrib));
			//m_items[i].id = INVALID_ITEM_ID;
			//m_items[i].type = ITEM_NO;
		}
		for(size_t i = 0; i < m_assistItems.size(); ++i)
		{
			memset(&m_assistItems[i], 0, sizeof(ItemAttrib));
			//m_assistItems[i].id = INVALID_ITEM_ID;
			//m_assistItems[i].type = ITEM_NO;
		}
	}
	int GetMoney()
	{
		return m_nMoney;
	}
	void SetMoney(int _money)
	{
		m_nMoney = _money;
	}
	void AddMoney(int _money)
	{
		m_nMoney += _money;
	}
	ItemList& GetItemList()
	{
		return m_items;
	}
	bool AddItem(const ItemAttrib* _item)
	{
		if(_item->type == ITEM_NO)
		{
			return false;
		}
		for(size_t i = 0; i < HERO_MAINBAG_SIZE_CUR; ++i)
		{
			if(m_items[i].type == ITEM_NO)
			{
				memcpy(&m_items[i], _item, sizeof(ItemAttrib));
				return true;
			}
		}
		return false;
	}
	bool AddAssistItem(const ItemAttrib* _pItem)
	{
		if(_pItem->type == ITEM_NO)
		{
			return false;
		}
		for(size_t i = 0; i < HERO_CLIENT_ASSIST_SIZE; ++i)
		{
			if(m_assistItems[i].type == ITEM_NO)
			{
				memcpy(&m_assistItems[i], _pItem, sizeof(ItemAttrib));
				return true;
			}
		}
		return false;
	}
	void ChangeBagItem(int _dest, int _src)
	{
		ItemAttrib item;
		memcpy(&item, &m_items[_dest], sizeof(ItemAttrib));
		memcpy(&m_items[_dest], &m_items[_src], sizeof(ItemAttrib));
		memcpy(&m_items[_src], &item, sizeof(ItemAttrib));
	}
	void ChangeItem(ItemAttrib* _one, ItemAttrib* _another)
	{
		ItemAttrib item;
		memcpy(&item, _one, sizeof(ItemAttrib));
		memcpy(_one, _another, sizeof(ItemAttrib));
		memcpy(_another, &item, sizeof(ItemAttrib));
	}
	void ChangeAssistItem(int _dest, int _src)
	{
		ItemAttrib item;
		memcpy(&item, &m_assistItems[_dest], sizeof(ItemAttrib));
		memcpy(&m_assistItems[_dest], &m_assistItems[_src], sizeof(ItemAttrib));
		memcpy(&m_assistItems[_src], &item, sizeof(ItemAttrib));
	}

	void MoveAssistItemToBag(int _bagidx, int _astidx)
	{
		memcpy(&m_items[_bagidx], &m_assistItems[_astidx], sizeof(ItemAttrib));
		ZeroMemory(&m_assistItems[_astidx], sizeof(ItemAttrib));
	}

	bool MoveToAssistItem(int _itemidx, int _astitemidx)
	{
		return false;

		if(_itemidx >= HERO_MAINBAG_SIZE_CUR ||
			_itemidx < 0 ||
			_astitemidx >= HERO_ASSISTBAG_SIZE_CUR ||
			_astitemidx < 0)
		{
			return false;
		}

		/*if(m_items[_itemidx].type == ITEM_YAO_SPE ||
			m_items[_itemidx].type == ITEM_YAP)*/
		if(m_items[_itemidx].type == ITEM_COST)
		{
			for(int i = 0; i < HERO_ASSISTBAG_SIZE_CUR; ++i)
			{
				if(m_assistItems[i].type == ITEM_NO)
				{
					m_assistItems[i] = m_items[_itemidx];
					ZeroMemory(&m_items[_itemidx], sizeof(ItemAttrib));
					return true;
				}
			}
		}
		return false;
	}

	bool MoveBagItemToAssistItem(int _bagid)
	{
		return false;

		int nCounter = -1;
		for(int i = 0; i < HERO_MAINBAG_SIZE_CUR; ++i)
		{
			if(m_items[i].id == _bagid)
			{
				nCounter = i;
				break;
			}
		}

		if(nCounter == -1)
		{
			return false;
		}

		for(int i = 0; i < HERO_ASSISTBAG_SIZE_CUR; ++i)
		{
			if(m_assistItems[i].type == ITEM_NO)
			{
				memcpy(&m_assistItems[i], &m_items[nCounter], sizeof(ItemAttrib));
				ZeroMemory(&m_items[nCounter], sizeof(ItemAttrib));
				return true;
			}
		}

		return false;
	}
// 	void MoveBagItemToAssist(int _id, int _astidx)
// 	{
// 		if(_astidx < 0 ||
// 			_astidx >= ASSIST_BAG_SIZE)
// 		{
// 			return;
// 		}
// 
// 		ItemAttrib* pItem = GetItem(_id);
// 		if(pItem)
// 		{
// 			if(m_assistItems[_astidx].type == ITEM_NO)
// 			{
// 				ChangeItem(pItem, &m_assistItems[_astidx]);
// 				ZeroMemory(pItem, sizeof(ItemAttrib));
// 			}
// 		}
// 	}
	ItemList& GetAssistItemList()
	{
		return m_assistItems;
	}
	ItemAttrib* GetItem(int _id)
	{
		for(size_t i = 0; i < m_items.size(); ++i)
		{
			if(m_items[i].id == _id)
			{
				return &m_items[i];
			}
		}
		return NULL;
	}
	ItemAttrib* GetItemByTag(DWORD _dwTag)
	{
		for(size_t i = 0; i < m_items.size(); ++i)
		{
			if(m_items[i].tag == _dwTag)
			{
				return &m_items[i];
			}
		}
		return NULL;
	}
	ItemAttrib* GetAssitItemByTag(DWORD _dwTag)
	{
		return NULL;

		for(size_t i = 0; i < m_assistItems.size(); ++i)
		{
			if(m_assistItems[i].tag == _dwTag)
			{
				return &m_assistItems[i];
			}
		}
		return NULL;
	}
	int GetEmptyBagSum()
	{
		int nCounter = 0;

		for(size_t i = 0; i < m_items.size(); ++i)
		{
			if(m_items[i].type == ITEM_NO)
			{
				++nCounter;
			}
		}

		return nCounter;
	}
	int GetBagTotalRoom()
	{
		return m_items.size();
	}
	int CountItem(int _nItemId)
	{
		int nCount = 0;

		for(int i = 0; i < m_items.size(); ++i)
		{
			if(m_items[i].type == ITEM_NO)
			{
				continue;
			}

			const ItemAttrib& refItem = m_items[i];

			if(refItem.id == _nItemId)
			{
				if(refItem.type == ITEM_COST)
				{
					nCount += refItem.atkSpeed;
				}
				else
				{
					nCount += 1;
				}
			}
		}

		return nCount;
	}
	void DebugTest()
	{
		m_items[0].id = 57;
		m_items[1].id = 63;
		m_items[2].id = 110;
		m_items[3].id = 147;
		m_items[4].id = 182;
		m_items[5].id = 207;
		m_items[6].id = 210;
		m_items[7].id = 241;
		m_items[8].id = 517;
		m_items[9].id = 548;
		m_items[10].id = 393;
		m_items[11].id = 83;
		m_items[12].id = 89;
		m_items[13].id = 162;
		m_items[14].id = 206;
		m_items[15].id = 88;
		m_items[16].id = 70;
		m_items[17].id = 219;
		m_items[18].id = 521;
		m_items[19].id = 260;
		m_items[20].id = 260;
		m_items[21].id = 260;
		m_items[22].id = 260;
		m_items[23].id = 260;
		m_items[24].id = 66;
		m_items[25].id = 171;
		m_items[25].id = 85;

		m_assistItems[0].id = 9;
		m_assistItems[1].id = 10;
		m_assistItems[2].id = 260;
		m_assistItems[3].id = 10;
		m_assistItems[4].id = 408;
		m_assistItems[5].id = 260;

		for(int i = 0; i < HERO_MAINBAG_SIZE_CUR; ++i)
		{
			GameInfoManager::GetInstance()->GetItemAttrib(m_items[i].id, &m_items[i]);
		}
		for(int i = 0; i < HERO_ASSISTBAG_SIZE_CUR; ++i)
		{
			GameInfoManager::GetInstance()->GetItemAttrib(m_assistItems[i].id, &m_assistItems[i]);
		}
	}
private:
	ItemList m_items;
	ItemList m_assistItems;
	int m_nMoney;
	int m_nCounter;
};

#endif