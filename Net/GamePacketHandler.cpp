#include "../Net/GamePacketHandler.h"
#include "../GameScene/GamePlayer.h"
#include "../GameScene/GameMapManager.h"
#include "../GameDialog/GameInfoBoardDlg.h"
#include "../Common/GlobalFunction.h"
#include "../GameScene/GameScene.h"
#include "../GameDialog/GameChatDlg.h"
#include "../BackMir/BackMir.h"
#include "../GameDialog/GameDlgBase.h"
#include "../GameDialog/GameInteractiveDlg.h"
//////////////////////////////////////////////////////////////////////////
//	Global object
/*
extern ByteBuffer g_xBuffer;
//////////////////////////////////////////////////////////////////////////
//	BasePacketHandler
//////////////////////////////////////////////////////////////////////////
BasePacketHandler::BasePacketHandler(GameObject* _pObj)
{
	assert(_pObj != NULL);
	m_pObject = _pObj;
}

BasePacketHandler::~BasePacketHandler()
{

}

//////////////////////////////////////////////////////////////////////////
/ ************************************************************************ /
/ * virtual void OnPacket(const PacketHeader* _pPkt)
/ ************************************************************************ /
void BasePacketHandler::OnPacket(const PacketHeader* _pPkt)
{
	g_xBuffer.Reset();
	unsigned int uWriteLen = g_xBuffer.Write(_pPkt, _pPkt->uLen);
	if(uWriteLen != _pPkt->uLen)
	{
		ALERT_MSGBOX("写入全局Buffer失败");
		::PostQuitMessage(0);
		return;
	}

	switch(_pPkt->uOp)
	{
	case PKG_GAME_OBJECT_ACTION_NOT:
		{
			PkgObjectActionNot not;
			try
			{
				g_xBuffer >> not;
			}
			BUFFER_EXCEPTION_CATCH_RETURN_VOID;
			DoPacket(not);
		}break;
	case PKG_GAME_PLAYER_SAY_REQ:
		{
			PkgPlayerSayReq req;
			try
			{
				g_xBuffer >> req;
			}
			BUFFER_EXCEPTION_CATCH_RETURN_VOID;
			DoPacket(req);
		}break;
	default:
		{
			//ALERT_MSGBOX("Unknown opcode!");
			AfxGetHge()->System_Log("未知的数据包[%d]",
				_pPkt->uOp);
		}break;
	}
}

/ ************************************************************************ /
/ * void DoPacket(const PkgObjectActionNot& _not)
/ ************************************************************************ /
void BasePacketHandler::DoPacket(const PkgObjectActionNot& _not)
{
	static const int s_nMoveOftTable[] = 
	{
		0,-1,	1,-1,	1,0,
		1,1,	0,1,	-1,1,
		-1,0,	-1,-1
	};
	static const int s_nRunOftTable[] = 
	{
		0,-2,	2,-2,	2,0,
		2,2,	0,2,	-2,2,
		-2,0,	-2,-2
	};

	if(_not.uAction == ACTION_CREATE)
	{
		//	Create Monster
		//	first check if the object is hero
		if(m_pObject->GetType() >= OBJ_PLAYER)
		{
			//	create the monster
			WORD wMonsterID = (WORD)_not.uParam0;
			WORD wCoordX = LOWORD(_not.uParam1);
			WORD wCoordY = HIWORD(_not.uParam1);
			//GameMapManager::GetInstance()->CreateMonster(wCoordX, wCoordY, wMonsterID);
		}
	}//	create end
	else if(_not.uAction == ACTION_STATUS)
	{
		//	change object's state
		int nDelta = _not.uParam1;
		switch(_not.uParam0)
		{
		case 0:
			{
				//	hp
				m_pObject->GetAttrib()->HP += nDelta;

				PkgServerStatusReq req;
				req.uSign = 1;
				g_xBuffer.Reset();
				g_xBuffer << req;
				SendBuffer(&g_xBuffer);
			}break;
		case 1:
			{
				//	mp
				m_pObject->GetAttrib()->MP += nDelta;
			}break;
		case 2:
			{
				//	exp
				m_pObject->GetAttrib()->EXPR += nDelta;
			}break;
		}
	}//	status end
	else if(_not.uAction == ACTION_TURN)
	{
		//	change the object's direction
		PLAYER_DIRECTION pd = (PLAYER_DIRECTION)_not.uParam1;
		if(pd >= PDT_UP &&
			pd <= PDT_LEFTUP)
		{
			m_pObject->SetDirection(pd);
		}
		m_pObject->SetReallyCoord(LOWORD(_not.uParam0), HIWORD(_not.uParam0));
	}//	turn end
	else if(_not.uAction == ACTION_WALK)
	{
		WORD dwPreCoordX = LOWORD(_not.uParam0);
		WORD dwPreCoordY = HIWORD(_not.uParam0);
		WORD dwNowCoordX = LOWORD(_not.uParam1);
		WORD dwNowCoordY = HIWORD(_not.uParam1);

		m_pObject->SetReallyCoord(dwPreCoordX, dwPreCoordY);
		m_pObject->MoveToDestination(dwNowCoordX, dwNowCoordY, MOVE_WALK);
	}
	else if(_not.uAction == ACTION_RUN)
	{
		WORD dwPreCoordX = LOWORD(_not.uParam0);
		WORD dwPreCoordY = HIWORD(_not.uParam0);
		WORD dwNowCoordX = LOWORD(_not.uParam1);
		WORD dwNowCoordY = HIWORD(_not.uParam1);

		m_pObject->SetReallyCoord(dwPreCoordX, dwPreCoordY);
		m_pObject->MoveToDestination(dwNowCoordX, dwNowCoordY, MOVE_RUN);
	}
	else if(_not.uAction == ACTION_STRUCK)
	{
		//	struck
// 		m_pObject->GetAttrib()->HP = _not.uParam0;
// 		m_pObject->SetReallyCoord(LOWORD(_not.uParam1), HIWORD(_not.uParam1));
// 		m_pObject->SetStatus(PST_ATTACKED);
// 		m_pObject->SetCurFrame(0);
		m_pObject->SetReallyCoord(LOWORD(_not.uParam0), HIWORD(_not.uParam0));
		if(_not.uParam1 <= 7)
		{
			m_pObject->SetDirection((PLAYER_DIRECTION)_not.uParam1);
		}
		m_pObject->SetStatus(PST_ATTACKED);
		m_pObject->GetAttrib()->HP = LOWORD(_not.uParam2);
		m_pObject->GetAttrib()->maxHP = HIWORD(_not.uParam2);
		m_pObject->SetCurFrame(0);
	}//struck end
	else if(_not.uAction == ACTION_ATTACK)
	{
		if(m_pObject->GetType() == OBJ_PLAYER ||
			m_pObject->GetType() == OBJ_OTHERPLAYER ||
			m_pObject->GetType() == OBJ_MONS)
		{
			m_pObject->SetReallyCoord(LOWORD(_not.uParam1), HIWORD(_not.uParam1));
			if(m_pObject->GetType() == OBJ_MONS)
			{
				m_pObject->SetStatus(PST_ATTACKNOWEAPON);
			}
			else
			{
				m_pObject->SetStatus(PST_ATTACKWEAPON);
			}
			m_pObject->SetCurFrame(0);
			if(_not.uParam2 >= 0 &&
				_not.uParam2 <= 7)
			{
				m_pObject->SetDirection((PLAYER_DIRECTION)_not.uParam2);
			}
		}
	}
	else if(_not.uAction == ACTION_DEAD)
	{
		m_pObject->SetCurFrame(0);
		m_pObject->SetStatus(PST_DEAD);
		m_pObject->SetReallyCoord(LOWORD(_not.uParam0), HIWORD(_not.uParam0));
		m_pObject->GetAttrib()->HP = 0;
		
		if(GamePlayer::GetInstance()->GetAtkMonster() == m_pObject)
		{
			GamePlayer::GetInstance()->SetAtkMonster(NULL);
			GamePlayer::GetInstance()->SetStatus(PST_STAND);
			GamePlayer::GetInstance()->SetCurFrame(0);
		}
	}
	else if(_not.uAction == ACTION_FLY)
	{
		m_pObject->SetCurFrame(0);
		m_pObject->SetStatus(PST_STAND);
		m_pObject->SetReallyCoord(LOWORD(_not.uParam1), HIWORD(_not.uParam1));
	}
}
/ ************************************************************************ /
/ * void DoPacket(const PkgPlayerSayReq& req)
/ ************************************************************************ /
void BasePacketHandler::DoPacket(const PkgPlayerSayReq& req)
{
	std::string xSay = req.xName + " : " + req.xWords;
	GameInfoBoardDlg::GetInstance()->InsertBoardMsg(ARGB_WHITE, xSay.c_str());
}



//////////////////////////////////////////////////////////////////////////
//	MyHeroPacketHandler
//////////////////////////////////////////////////////////////////////////
MyHeroPacketHandler::MyHeroPacketHandler(GameObject* _pObj) :
					HeroPacketHandler(_pObj)
{

}

MyHeroPacketHandler::~MyHeroPacketHandler()
{

}

//////////////////////////////////////////////////////////////////////////
/ ************************************************************************ /
/ * virtual void OnPacket(const PacketHeader* _pPkt)
/ ************************************************************************ /
void MyHeroPacketHandler::OnPacket(const PacketHeader* _pPkt)
{
	return __super::OnPacket(_pPkt);
	switch(_pPkt->uOp)
	{
	case PKG_GAME_OBJECT_ACTION_NOT:
		{
			//	create monster or NPC
			const PkgObjectActionNot* pPkt = static_cast<const PkgObjectActionNot*>(_pPkt);
			if(pPkt->uAction == ACTION_CREATE)
			{
				WORD wMonsterID = (WORD)pPkt->uParam0;
				WORD wCoordX = LOWORD(pPkt->uParam1);
				WORD wCoordY = HIWORD(pPkt->uParam1);
				//GameMapManager::GetInstance()->CreateMonster(wCoordX, wCoordY, wMonsterID);
			}
			//	change player's hp,mp,exp
			else if(pPkt->uAction == ACTION_STATUS)
			{
				DWORD dwWhich = pPkt->uParam0;
				bool bInc = LOWORD(pPkt->uParam1) == 0 ? true : false;
				if(bInc)
				{
					GamePlayer::GetInstance()->AddHP(HIWORD(pPkt->uParam1));
				}
			}
		}break;
	}
}

//////////////////////////////////////////////////////////////////////////
//	AIPacketHandler
//////////////////////////////////////////////////////////////////////////
AIPacketHandler::AIPacketHandler(GameObject* _pObj) : BasePacketHandler(_pObj)
{

}

AIPacketHandler::~AIPacketHandler()
{

}
//////////////////////////////////////////////////////////////////////////
void AIPacketHandler::OnPacket(const PacketHeader* _pPkt)
{
	g_xBuffer.Reset();
	unsigned int uWriteLen = g_xBuffer.Write(_pPkt, _pPkt->uLen);
	if(uWriteLen != _pPkt->uLen)
	{
		ALERT_MSGBOX("写入全局Buffer失败");
		::PostQuitMessage(0);
		return;
	}

	BEGIN_HANDLE_PACKET(_pPkt)
		HANDLE_PACKET(PKG_GAME_CHAT_NOT,					PkgChatNot)
		HANDLE_PACKET(PKG_GAME_OBJECT_ACTION_NOT,			PkgObjectActionNot)
		HANDLE_PACKET(PKG_GAME_PLAYER_CLICKNPC_ACK,			PkgPlayerClickNPCAck)
		HANDLE_DEFAULT(_pPkt)
	END_HANDLE_PACKET(_pPkt)
}
//////////////////////////////////////////////////////////////////////////
void AIPacketHandler::DoPacket(const PkgChatNot& not)
{
	std::string xSay;
	xSay = not.xMsg;
	m_pObject->SetSaying(not.xMsg.c_str());
}
//////////////////////////////////////////////////////////////////////////
void AIPacketHandler::DoPacket(const PkgObjectActionNot& not)
{
	if(not.uAction == ACTION_APPEAR ||
		not.uAction == ACTION_GROUND)
	{
		if(not.uAction == ACTION_APPEAR)
		{
			m_pObject->SetStatus(PST_APPEAR);
			//m_pObject->GetAttrib()->curse = 1;
			m_pObject->SetCurFrame(0);
			m_pObject->SetVisible(true);
		}
		else
		{
			m_pObject->SetStatus(PST_GROUND);
			m_pObject->SetCurFrame(0);
		}
	}
	/ *
	else if(not.uAction == ACTION_ATTACK &&
			m_pObject->GetAttrib()->id == MONSTER_CHULONGSHEN)
		{
			m_pObject->SetReallyCoord(LOWORD(_not.uParam1), HIWORD(_not.uParam1));
			if(m_pObject->GetType() == OBJ_MONS)
			{
				m_pObject->SetStatus(PST_ATTACKNOWEAPON);
			}
			else
			{
				m_pObject->SetStatus(PST_ATTACKWEAPON);
			}
			m_pObject->SetCurFrame(0);
			if(_not.uParam2 >= 0 &&
				_not.uParam2 <= 7)
			{
				m_pObject->SetDirection((PLAYER_DIRECTION)_not.uParam2);
			}
		}* /
	
	else
	{
		m_pObject->SetVisible(true);
		BasePacketHandler::DoPacket(not);
	}
}
//////////////////////////////////////////////////////////////////////////
void AIPacketHandler::DoPacket(const PkgPlayerClickNPCAck& not)
{
	int nQuestID = LOWORD(not.dwParam);
	int nStepID = HIWORD(not.dwParam);
	int nNPCID = (int)not.dwNPCID;

	//if(nQuestID == 0)
	//{
		//	SellDlg or Other dlg
	//}
	//else
	{
		//pTheGame->GetGameScene()->GetMainOpt()->GetActiveDlg()->Create(nNPCID, nQuestID, nStepID);
		if(m_pObject->GetType() == OBJ_NPC)
		{
			pTheGame->GetGameScene()->GetMainOpt()->GetActiveDlg()->Create((GameNPC*)m_pObject, nQuestID, nStepID);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//	HeroPacketHandler
//////////////////////////////////////////////////////////////////////////
HeroPacketHandler::HeroPacketHandler(GameObject* _pObj) : BasePacketHandler(_pObj)
{

}

HeroPacketHandler::~HeroPacketHandler()
{

}
//////////////////////////////////////////////////////////////////////////
void HeroPacketHandler::OnPacket(const PacketHeader* _pPkt)
{
	g_xBuffer.Reset();
	unsigned int uWriteLen = g_xBuffer.Write(_pPkt, _pPkt->uLen);
	if(uWriteLen != _pPkt->uLen)
	{
		ALERT_MSGBOX("写入全局Buffer失败");
		::PostQuitMessage(0);
		return;
	}

	BEGIN_HANDLE_PACKET(_pPkt)
		HANDLE_PACKET(PKG_GAME_CHAT_NOT,					PkgChatNot)
		HANDLE_PACKET(PKG_GAME_PLAYER_CHANGEEQUIPMENT_NOT,	PkgPlayerChangeEquipNot)
		HANDLE_PACKET(PKG_GAME_PLAYER_GAINITEM_NTF,			PkgPlayerGainItemNtf)
		HANDLE_PACKET(PKG_GAME_PLAYER_DROPITEM_NOT,			PkgPlayerDropItemNot)
		HANDLE_PACKET(PKG_GAME_PLAYER_LOSTITEM_ACK,			PkgPlayerLostItemAck)
		HANDLE_PACKET(PKG_GAME_PLAYER_CLEARITEM_NTF,		PkgPlayerClearItemNtf)
		HANDLE_PACKET(PKG_GAME_PLAYER_SYNCATTRIB_NTF,		PkgPlayerSyncAttribNtf)
		HANDLE_PACKET(PKG_GAME_PLAYER_UNDRESSITEM_ACK,		PkgPlayerUndressItemAck)
		HANDLE_PACKET(PKG_GAME_PLAYER_DRESSITEM_ACK,		PkgPlayerDressItemAck)
		HANDLE_PACKET(PKG_GAME_PLAYER_USEITEM_ACK,			PkgPlayerUseItemAck)
		HANDLE_PACKET(PKG_GAME_PLAYER_UPDATEATTRIB_NTF,		PkgPlayerUpdateAttribNtf)
		HANDLE_PACKET(PKG_GAME_PLAYER_STRUCK_ACK,			PkgPlayerStruckAck)
		HANDLE_PACKET(PKG_GAME_PLAYER_FORCEACTION_ACK,		PkgPlayerForceActionAck)
		HANDLE_PACKET(PKG_GAME_PLAYER_CLEARBAG_NTF,			PkgPlayerClearBagNtf)
		HANDLE_DEFAULT(_pPkt)
	END_HANDLE_PACKET(_pPkt)
}

//////////////////////////////////////////////////////////////////////////

void HeroPacketHandler::DoPacket(const PkgObjectActionNot& not)
{
	if(not.uAction == ACTION_WALK)
	{
		WORD dwPreCoordX = LOWORD(not.uParam0);
		WORD dwPreCoordY = HIWORD(not.uParam0);
		WORD dwNowCoordX = LOWORD(not.uParam1);
		WORD dwNowCoordY = HIWORD(not.uParam1);

		m_pObject->SetReallyCoord(dwPreCoordX, dwPreCoordY);
		m_pObject->MoveToDestination(dwNowCoordX, dwNowCoordY, MOVE_WALK);
	}
	else if(not.uAction == ACTION_RUN)
	{
		WORD dwPreCoordX = LOWORD(not.uParam0);
		WORD dwPreCoordY = HIWORD(not.uParam0);
		WORD dwNowCoordX = LOWORD(not.uParam1);
		WORD dwNowCoordY = HIWORD(not.uParam1);

		m_pObject->SetReallyCoord(dwPreCoordX, dwPreCoordY);
		m_pObject->MoveToDestination(dwNowCoordX, dwNowCoordY, MOVE_RUN);
	}
	else if(not.uAction == ACTION_TURN)
	{
		WORD wCoordX = LOWORD(not.uParam0);
		WORD wCoordY = HIWORD(not.uParam0);
		m_pObject->SetReallyCoord(wCoordX, wCoordY);
		if(not.uParam1 >= 0 &&
			not.uParam1 < 8)
		{
			m_pObject->SetDirection((PLAYER_DIRECTION)not.uParam1);
		}
	}
	else if(not.uAction == ACTION_STRUCK)
	{
/ *
		if(m_pObject->GetType() == OBJ_PLAYER)
		{
			if(not.uParam2 == 0)
			{
				//	struck request
				m_pObject->SetHurt();
			}
			else
			{
				//	get the hp
				m_pObject->GetAttrib()->HP = LOWORD(not.uParam2);
				m_pObject->GetAttrib()->maxHP = HIWORD(not.uParam2);
			}
		}
		else* /
		{
			m_pObject->SetReallyCoord(LOWORD(not.uParam0), HIWORD(not.uParam0));
			if(not.uParam1 <= 7)
			{
				m_pObject->SetDirection((PLAYER_DIRECTION)not.uParam1);
			}
			m_pObject->SetStatus(PST_ATTACKED);
			m_pObject->GetAttrib()->HP = LOWORD(not.uParam2);
			m_pObject->GetAttrib()->maxHP = HIWORD(not.uParam2);
			m_pObject->SetCurFrame(0);
		}
	}
	else if(not.uAction == ACTION_DEAD)
	{
		m_pObject->GetAttrib()->HP = 0;
		m_pObject->SetReallyCoord(LOWORD(not.uParam0), HIWORD(not.uParam0));
		m_pObject->SetStatus(PST_DEAD);
		m_pObject->SetCurFrame(0);
	}
}

void HeroPacketHandler::DoPacket(const PkgPlayerChangeEquipNot& not)
{
	if(m_pObject->GetType() == OBJ_OTHERPLAYER)
	{
		GameOtherPlayer* pPlayer = (GameOtherPlayer*)m_pObject;
		
		if(not.stItem.type == ITEM_WEAPON)
		{
			memcpy(pPlayer->GetPlayerItem(PLAYER_ITEM_WEAPON),
				&not.stItem, sizeof(ItemAttrib));
		}
		else if(not.stItem.type == ITEM_CLOTH)
		{
			memcpy(pPlayer->GetPlayerItem(PLAYER_ITEM_CLOTH),
				&not.stItem, sizeof(ItemAttrib));
		}
	}
}

void HeroPacketHandler::DoPacket(const PkgChatNot& not)
{
	std::string xSay;
	xSay += m_pObject->GetAttrib()->name;
	xSay += "说:";
	xSay += not.xMsg;
	m_pObject->SetSaying(not.xMsg.c_str());
	GameScene::sThis->GetMainOpt()->GetChatDlg()->GetHistoryDlg()->InsertChatMessage(xSay.c_str(), not.uExtra);
}

void HeroPacketHandler::DoPacket(const PkgPlayerGainItemNtf& ntf)
{
	if(m_pObject->GetType() == OBJ_PLAYER)
	{
		GamePlayer* pPlayer = (GamePlayer*)m_pObject;
		pPlayer->GetPlayerBag()->AddItem(&ntf.stItem);
		GameInfoBoardDlg::GetInstance()->ShowItemMsg(&ntf.stItem);
	}
}

void HeroPacketHandler::DoPacket(const PkgPlayerDropItemNot& not)
{
	//if(m_pObject->GetType() == OBJ_PLAYER ||
		//m_pObject->GetType() == OBJ_OTHERPLAYER)
	{
		//GamePlayer* pPlayer = (GamePlayer*)m_pObject;
		GroundItem item;
		item.stAttrib.id = not.dwID;
		GameInfoManager::GetInstance()->GetItemAttrib(not.dwID, &item.stAttrib);
		item.stAttrib.tag = not.dwTag;
		item.wPosX = not.wCoordX;
		item.wPosY = not.wCoordY;
		GameScene::sThis->InsertNewGroundItem(&item);
	}
}

void HeroPacketHandler::DoPacket(const PkgPlayerLostItemAck& ack)
{
	if(m_pObject->GetType() == OBJ_PLAYER)
	{
		GamePlayer* pPlayer = (GamePlayer*)m_pObject;
		ItemAttrib* pItem = pPlayer->GetPlayerBag()->GetItemByTag(ack.dwTag);
		if(pItem)
		{
			ZeroMemory(pItem, sizeof(ItemAttrib));
		}
	}
}

void HeroPacketHandler::DoPacket(const PkgPlayerClearItemNtf& ntf)
{
	GameScene::sThis->RemoveGroundItem(ntf.dwTag);
}

void HeroPacketHandler::DoPacket(const PkgPlayerSyncAttribNtf& ntf)
{
	memcpy(m_pObject->GetAttrib(), &ntf.stAttrib, sizeof(ItemAttrib));
}

void HeroPacketHandler::DoPacket(const PkgPlayerUndressItemAck& ack)
{
	if(m_pObject->GetType() == OBJ_PLAYER)
	{
		GamePlayer* pPlayer = (GamePlayer*)m_pObject;

		ItemAttrib* pItem = pPlayer->GetPlayerItem((PLAYER_ITEM_TYPE)ack.bPos);
		if(pItem->tag == ack.dwTag)
		{
			pPlayer->GetPlayerBag()->AddItem(pItem);
			ZeroMemory(pItem, sizeof(ItemAttrib));
		}
		pPlayer->UpdatePlayerAttrib();
	}
	else
	{
		GameOtherPlayer* pPlayer = (GameOtherPlayer*)m_pObject;

		ItemAttrib* pItem = pPlayer->GetPlayerItem((PLAYER_ITEM_TYPE)ack.bPos);
		//if(pItem->tag == ack.dwTag)
		{
			ZeroMemory(pItem, sizeof(ItemAttrib));
		}
	}
}

void HeroPacketHandler::DoPacket(const PkgPlayerDressItemAck& ack)
{
	if(0 != ack.bRet)
	{
		GameInfoBoardDlg::GetInstance()->InsertEquipErrorMsg(ack.bRet);
	}
	else
	{
		if(m_pObject->GetType() == OBJ_PLAYER)
		{
			//	self
			GamePlayer* pPlayer = (GamePlayer*)m_pObject;
			ItemAttrib* pItem = pPlayer->GetPlayerItem((PLAYER_ITEM_TYPE)ack.bPos);
			ItemAttrib* pWantDressItem = pPlayer->GetPlayerBag()->GetItemByTag(ack.dwTag);
			if(pItem &&
				pWantDressItem)
			{
				if(pItem->type == ITEM_NO)
				{
					memcpy(pItem, pWantDressItem, sizeof(ItemAttrib));
					memset(pWantDressItem, 0, sizeof(ItemAttrib));
				}
				else
				{
					ItemAttrib item;
					memcpy(&item, pItem, sizeof(ItemAttrib));
					memcpy(pItem, pWantDressItem, sizeof(ItemAttrib));
					memcpy(pWantDressItem, &item, sizeof(ItemAttrib));
				}
			}
			pPlayer->UpdatePlayerAttrib();
			pTheGame->GetGameScene()->GetMainOpt()->GetBagDlg()->CancelSel();
		}
		else
		{
			//	other players
			GameOtherPlayer* pPlayer = (GameOtherPlayer*)m_pObject;
			ItemAttrib* pItem = pPlayer->GetPlayerItem((PLAYER_ITEM_TYPE)ack.bPos);
			if(pItem)
			{
				pItem->type = ITEM_WEAPON;
				pItem->tex = ack.dwTex;
			}
		}
	}
}

void HeroPacketHandler::DoPacket(const PkgPlayerUseItemAck& ack)
{
	if(m_pObject->GetType() == OBJ_PLAYER)
	{
		//	self
		bool bInAssist = false;
		GamePlayer* pPlayer = (GamePlayer*)m_pObject;
		ItemAttrib* pItem = pPlayer->GetPlayerBag()->GetItemByTag(ack.dwTag);
		if(pItem == NULL)
		{
			pItem = pPlayer->GetPlayerBag()->GetAssitItemByTag(ack.dwTag);
			if(pItem)
			{
				bInAssist = true;
			}
		}

		/ *switch(ack.dwUsage)
		{
		case USE_RANDOM_POS:
			{
				pPlayer->SetReallyCoord(LOWORD(ack.dwParam0), HIWORD(ack.dwParam0));
				pPlayer->SetCurFrame(0);
				pPlayer->SetStatus(PST_STAND);
			}break;
		}

		if(pItem)
		{
			if(pPlayer->IsCostItem(*pItem))
			{
				if(bInAssist)
				{
					int nUseID = pItem->id;
					ZeroMemory(pItem, sizeof(ItemAttrib));
					pPlayer->GetPlayerBag()->MoveBagItemToAssistItem(nUseID);
				}

				if(!bInAssist)
				{
					memset(pItem, 0, sizeof(ItemAttrib));
				}
			}
			else
			{
				memset(pItem, 0, sizeof(ItemAttrib));
			}
		}* /

		PkgPlayerSyncAssistReq req;
		req.uUserId = pPlayer->GetHandlerID();
		req.bNum = pPlayer->GetAssistItemsEmptySum();
		g_xBuffer.Reset();
		g_xBuffer << req;
		SendBuffer(&g_xBuffer);
	}
	else
	{
		//	other players
		/ *GameOtherPlayer* pPlayer = (GameOtherPlayer*)m_pObject;
		pPlayer->SetReallyCoord(LOWORD(ack.dwParam0), HIWORD(ack.dwParam0));
		pPlayer->SetCurFrame(0);
		pPlayer->SetStatus(PST_STAND);* /
	}
}

void HeroPacketHandler::DoPacket(const PkgPlayerUpdateAttribNtf& ntf)
{
	switch(LOWORD(ntf.dwParam))
	{
	case UPDATE_HP:
		{
			m_pObject->GetAttrib()->HP = HIWORD(ntf.dwParam);
		}break;
	case UPDATE_MP:
		{
			m_pObject->GetAttrib()->MP = HIWORD(ntf.dwParam);
		}break;
	}
}

void HeroPacketHandler::DoPacket(const PkgPlayerForceActionAck& ntf)
{
	if(ntf.bType == FORCE_POSITION)
	{
		m_pObject->SetReallyCoord(LOWORD(ntf.dwData), HIWORD(ntf.dwData));
		m_pObject->SetStatus(PST_STAND);
		m_pObject->SetCurFrame(0);
	}
}

void HeroPacketHandler::DoPacket(const PkgPlayerClearBagNtf& ntf)
{
	if(m_pObject->GetType() == OBJ_PLAYER)
	{
		GamePlayer* pPlayer = (GamePlayer*)m_pObject;
		ItemAttrib* pItem = pPlayer->GetPlayerBag()->GetItemByTag(ntf.dwTag);
		if(pItem)
		{
			ZeroMemory(pItem, sizeof(ItemAttrib))
;		}
	}
}
//////////////////////////////////////////////////////////////////////////
void HeroPacketHandler::DoPacket(const PkgPlayerStruckAck& ack)
{
	m_pObject->SetHurt();
}*/
