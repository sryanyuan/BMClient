#include "../BackMir/BackMir.h"
#include <io.h>
#include <direct.h>
#include <Shlwapi.h>
#include "../Common/SKnl3Helper.h"
#include "../GameScene/GameTextureManager.h"
#include <time.h>
#include "../Common/GlobalDefine.h"
#include "../Common/GlobalFunction.h"
#include "../GameScene/GameResourceManager.h"
#include "../GameScene/GameMapManager.h"
#include "../GameScene/GamePlayer.h"
#include "../GameDialog/GameInfoBoardDlg.h"
#include "../GameDialog/GameDlgBase.h"
#include "../Common/TimeCounter.h"
#include "../../CommonModule/ByteBuffer.h"
#include "../Net/PacketHandler.h"
#include "../GameScene/MagicEffect.h"
#include "../GameScene/GameSoundManager.h"
#include "../../CommonModule/SaveFile.h"
#include "../GameDialog/GameChatDlg.h"
#include "../resource.h"
#include "../Common/ExceptionHandler.h"
#include "../../CommonModule/CommandLineHelper.h"
#include "../Net/ClientSocket.h"
#include "../LoginScene/SelChrDlg.h"
#include "../BackMir/BackMir.h"
#include "BMPreConnWnd.h"
//////////////////////////////////////////////////////////////////////////
void STDCALL MirGame::_HandleLoginMsg(const void* _pData, unsigned int _len)
{
	pTheGame->HandleLoginMsg(_pData, _len);
}

void MirGame::HandleLoginMsg(const void* _pData, unsigned int _len)
{
	const LoginHeader* pPacket = (const LoginHeader*)_pData;
	LoginHeader* pReadPacket = const_cast<LoginHeader*>(pPacket);
	pReadPacket->uLen = _len;

	unsigned int uOpCode = pPacket->uOp;
	const char* pData = (const char*)_pData;
	pData += 8;

	if(uOpCode == PKG_LOGIN_ADDGAMEROLE_ACK)
	{
		LoginAddGameRoleAck(pData, _len);
	}
	else if(uOpCode == PKG_LOGIN_DELGAMEROLE_ACK)
	{
		LoginDelGameRoleAck(pData, _len);
	}
	else
	{
		g_xBuffer.Reset();
		g_xBuffer.Write(_pData, _len);

		switch(uOpCode)
		{
			DO_LOGIN_PACKET(PKG_LOGIN_QUICKMSG_NOT, PkgLoginQuickMsgNot)
		}
	}
}

void MirGame::LoginDelGameRoleAck(const char* _pData, unsigned int _len)
{
	char namelen = *_pData;
	_pData++;
	char szName[20] = {0};
	memcpy(szName, _pData, namelen);

	if(pTheGame->GetCurStage() == PAGE_LOGIN)
	{
		if(pTheGame->GetLoginScene()->GetPage() == PAGE_SELCHR)
		{
			//	update global header data
			for(int i = 0; i < 3; ++i)
			{
				if(0 == strcmp(g_stHeroHeader[i].szName, szName))
				{
					g_stHeroHeader[i].szName[0] = 0;
					g_stHeroHeader[i].bJob = 0;
					g_stHeroHeader[i].bSex = 0;
					g_stHeroHeader[i].uLevel = 0;
					break;
				}
			}

			if(pTheGame->GetLoginScene()->GetSelChrDlg() == NULL)
			{
				return;
			}

			int nIndex = pTheGame->GetLoginScene()->GetSelChrDlg()->GetHumIndex(szName);
			if(nIndex != -1)
			{
				pTheGame->GetLoginScene()->GetSelChrDlg()->DeleteHum(nIndex);
			}
		}
	}
}

void MirGame::LoginAddGameRoleAck(const char* _pData, unsigned int _len)
{
	char suc = *_pData;
	++_pData;

	if(suc == 0)
	{
		MessageBox(NULL, "ERROR", "Can't Create ..", MB_OK);
	}
	else
	{
		char namelen = *_pData;
		++_pData;
		char szName[20] = {0};
		memcpy(szName, _pData, namelen);
		_pData += namelen;
		char job = *_pData;
		_pData++;
		char sex = *_pData;

		if(strlen(szName) > 0 &&
			strlen(szName) < 19)
		{
			if(pTheGame->GetCurStage() == SCENE_LOGIN)
			{
				if(pTheGame->GetLoginScene()->GetPage() == PAGE_SELCHR)
				{
					//	update global header data
					for(int i = 0; i < 3; ++i)
					{
						if(g_stHeroHeader[i].szName[0] == 0)
						{
							strcpy(g_stHeroHeader[i].szName, szName);
							g_stHeroHeader[i].bJob = job;
							g_stHeroHeader[i].bSex = sex;
							g_stHeroHeader[i].uLevel = 1;
							break;
						}
					}
					if(pTheGame->GetLoginScene()->GetSelChrDlg() == NULL)
					{
						return;
					}
					HeroHeader header;
					strcpy(header.szName, szName);
					header.bJob = job;
					header.bSex = sex;
					header.uLevel = 1;
					pTheGame->GetLoginScene()->GetSelChrDlg()->AddHumData(&header);
				}
			}
		}
	}
}

void MirGame::DoLoginPacket(const PkgLoginQuickMsgNot& not)
{
	const char* pszMsg = NULL;

	if(not.uMsg == 1)
	{
		pszMsg = "没有可用的游戏服务器";
	}
	else if(not.uMsg == 2)
	{
		pszMsg = "人物存档不存在";
	}
	else if(not.uMsg == 3)
	{
		pszMsg = "异常的存档读取";
	}
	else if(not.uMsg == 4)
	{
		pszMsg = "角色名已存在";
	}
	else if(not.uMsg == 5)
	{
		pszMsg = "不存在的玩家数据";
	}
	else if(not.uMsg == 6)
	{
		pszMsg = "无法创建角色";
	}
	else if(not.uMsg == 7)
	{
		pszMsg = "无可用的游戏服务器";
	}
	else if(not.uMsg == 8)
	{
		pszMsg = "用户名或者密码错误";
	}
	else if(not.uMsg == 9)
	{
		pszMsg = "存档失败";
	}

	if(pszMsg != NULL)
	{
		MessageBox(NULL, pszMsg, "TIP", MB_OK);
	}
}