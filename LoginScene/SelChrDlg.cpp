#include "SelChrDlg.h"
#include "../BackMir/BackMir.h"
#include "../Net/ClientSocket.h"
#include "../LoginScene/DeleteConfirmDlg.h"
#include "../LoginScene/NewHumDlg.h"
#include <ZipArchive.h>
#include "../BackMir/BMPreConnWnd.h"
#include "../GameScene/GameResourceManager.h"
#include "../Common/gfx_utils.h"
//////////////////////////////////////////////////////////////////////////
const char* g_szHeader[] =
{
	"head",
	"head1",
	"head2"
};

const char* g_szData[] =
{
	"data",
	"data1",
	"data2"
};

#define HUM_POSX		400
#define HUM_POSY		250

#define HUM_ANITIME		0.15f
//////////////////////////////////////////////////////////////////////////
SelChrDlg::SelChrDlg()
{
	SetFixed();
	SetCustomBk();
	m_rcClient.right = VIEW_WIDTH;
	m_rcClient.bottom = VIEW_HEIGHT + VIEW_OPERATE;
	m_pRender = new hgeSprite(0, 0, 0, 0, 0);
	m_nCurHumOrder = 0;
	m_bIsConnecting = false;
	ZeroMemory(m_stHero, sizeof(m_stHero));

	AddCommonButton(CBTN_NORMAL, 0, VIEW_WIDTH / 2 - 325 / 2 + 75 * 0 + 50 * 0, VIEW_HEIGHT + VIEW_OPERATE - 60, "创建人物");
	AddCommonButton(CBTN_NORMAL, 1, VIEW_WIDTH / 2 - 325 / 2 + 75 * 1 + 50 * 1, VIEW_HEIGHT + VIEW_OPERATE - 60, "删除人物");
	AddCommonButton(CBTN_NORMAL, 2, VIEW_WIDTH / 2 - 325 / 2 + 75 * 2 + 50 * 2, VIEW_HEIGHT + VIEW_OPERATE - 60, "进入游戏");

	AddCommonButton(CBTN_LEFTARROW, 3, VIEW_WIDTH / 2 - 124 / 2 + 12 * 0 + 100 * 0, VIEW_HEIGHT + VIEW_OPERATE - 100, NULL);
	AddCommonButton(CBTN_RIGHTARROW, 4, VIEW_WIDTH / 2 - 124 / 2 + 12 * 1 + 100 * 1, VIEW_HEIGHT + VIEW_OPERATE - 100, NULL);

	m_pDelCfmDlg = new DeleteConfirmDlg;
	m_pDelCfmDlg->SetSelChrDlg(this);
	m_xDlgCtl.InsertObject(m_pDelCfmDlg);
	m_pNewHumDlg = new NewHumDlg;
	m_pNewHumDlg->SetSelChr(this);
	m_xDlgCtl.InsertObject(m_pNewHumDlg);
	m_fConnectTotalTime = 0.0f;

	//g_xClientSocket.InitSocket(NULL);

	m_pSelChrRes = new GameTextureManagerSib;
	char szPath[MAX_PATH];
	sprintf(szPath, "%s\\Data\\selchr.sib",
		GetRootPath());
	if(!m_pSelChrRes->LoadPackage(szPath))
	{
		AfxGetHge()->System_Log("can't load package [selchr.sib]");
	}

	m_pEffRender = new hgeSprite(0, 0, 0, 0, 0);
	m_pEffRender->SetBlendMode(BLEND_ALPHAADD | BLEND_NOZWRITE);

	m_dwHumFrame = 0;
	m_fHumUpdateTime = 0.0f;
	m_dwFireFrame = 0;
	m_fFireUpdateTime = 0.0f;
}

SelChrDlg::~SelChrDlg()
{
	SAFE_DELETE(m_pRender);
	SAFE_DELETE(m_pDelCfmDlg);
	SAFE_DELETE(m_pNewHumDlg);
	SAFE_DELETE(m_pSelChrRes);
	SAFE_DELETE(m_pEffRender);
}
//////////////////////////////////////////////////////////////////////////
void SelChrDlg::Render()
{
	GameTextureManager* pPngMgr = GameResourceManager::GetInstance()->GetTexs(RES_CUSTOM);
	HTEXTURE tex = 0;
	tex = pTheGame->GetLoginRes()->GetTexture("selchr");
	if(tex)
	{
		m_pRender->SetTexture(tex);
		m_pRender->SetTextureRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
		m_pRender->Render(0, 0);
	}

	m_fFireUpdateTime += AfxGetHge()->Timer_GetDelta();
	if(m_fFireUpdateTime > 0.05f)
	{
		m_fFireUpdateTime = 0.0f;
		++m_dwFireFrame;
		if(m_dwFireFrame == 40)
		{
			m_dwFireFrame = 0;
		}
	}

	tex = m_pSelChrRes->GetTexture(720 + m_dwFireFrame);
	if(tex)
	{
		m_pEffRender->SetTexture(tex);
		m_pEffRender->SetTextureRect(0, 0,
			m_pSelChrRes->GetTextureWidth(720 + m_dwFireFrame),
			m_pSelChrRes->GetTextureHeight(720 + m_dwFireFrame));
		m_pEffRender->Render(RELATIVE_X(857) + m_pSelChrRes->GetTextureOffsetX(720 + m_dwFireFrame),
			RELATIVE_Y(229) + m_pSelChrRes->GetTextureOffsetY(720 + m_dwFireFrame));
	}

	tex = m_pSelChrRes->GetTexture(770 + m_dwFireFrame);
	if(tex)
	{
		m_pEffRender->SetTexture(tex);
		m_pEffRender->SetTextureRect(0, 0,
			m_pSelChrRes->GetTextureWidth(770 + m_dwFireFrame),
			m_pSelChrRes->GetTextureHeight(770 + m_dwFireFrame));
		m_pEffRender->Render(RELATIVE_X(-53) + m_pSelChrRes->GetTextureOffsetX(770 + m_dwFireFrame),
			RELATIVE_Y(229) + m_pSelChrRes->GetTextureOffsetY(770 + m_dwFireFrame));
	}

	tex = m_pSelChrRes->GetTexture(820 + m_dwFireFrame);
	if(tex)
	{
		m_pEffRender->SetTexture(tex);
		m_pEffRender->SetTextureRect(0, 0,
			m_pSelChrRes->GetTextureWidth(820 + m_dwFireFrame),
			m_pSelChrRes->GetTextureHeight(820 + m_dwFireFrame));
		m_pEffRender->Render(RELATIVE_X(398) + m_pSelChrRes->GetTextureOffsetX(820 + m_dwFireFrame),
			RELATIVE_Y(196) + m_pSelChrRes->GetTextureOffsetY(820 + m_dwFireFrame));
	}

	tex = m_pSelChrRes->GetTexture(870 + m_dwFireFrame);
	if(tex)
	{
		m_pEffRender->SetTexture(tex);
		m_pEffRender->SetTextureRect(0, 0,
			m_pSelChrRes->GetTextureWidth(870 + m_dwFireFrame),
			m_pSelChrRes->GetTextureHeight(870 + m_dwFireFrame));
		m_pEffRender->Render(RELATIVE_X(449) + m_pSelChrRes->GetTextureOffsetX(870 + m_dwFireFrame),
			RELATIVE_Y(200) + m_pSelChrRes->GetTextureOffsetY(870 + m_dwFireFrame));
	}

	__super::Render();

	int nNowHumCount = 0;
	for(int i = 0; i < 3; ++i)
	{
		if(m_stHero[i].szName[0] != 0)
		{
			++nNowHumCount;
		}
	}

	if(nNowHumCount == 0)
	{
		const char* pszTip = "请点击创建人物后开始游戏";

		int nTexWidth = GetTextWidth(pszTip, 12);
		int nRenderX = VIEW_WIDTH / 2 - nTexWidth / 2;

		//tex = pTheGame->GetLoginRes()->GetTexture("bmcolor");
		tex = pPngMgr->GetTexture(19);
		if(tex)
		{
			/*m_pRender->SetTexture(tex);
			m_pRender->SetTextureRect(0, 0, nTexWidth + 10, 12 + 4);
			m_pRender->SetColor(ARGB(192, 255, 255, 255));
			m_pRender->Render(nRenderX - 5, 50 - 2);
			m_pRender->SetColor(ARGB_WHITE);*/
			RECT rcScale9 = {9, 9, 9, 9};
			RECT rcTex = {0, 0, 32, 32};
			Gfx_SetRenderState(kGfx_9Path_CenterStretch, 1);
			Gfx_Render9Path(m_pRender, tex, &rcTex, &rcScale9, nRenderX - 5, 50 - 2, nTexWidth + 10, 12 + 4);
		}
		AfxGetPrinter()->SetColor(ARGB_WHITE);
		AfxGetPrinter()->Print(nRenderX, 50, pszTip);
	}
	else
	{
		if(m_nCurHumOrder >= 0 &&
			m_nCurHumOrder < 3)
		{
			if(m_stHero[m_nCurHumOrder].szName[0] != 0)
			{
				HeroHeader& stHeader = m_stHero[m_nCurHumOrder];
				char szInfo[150];
				strcpy(szInfo, m_stHero[m_nCurHumOrder].szName);
				if(stHeader.bJob == 0)
				{
					strcat(szInfo, " 职业:战士");
				}
				else if(stHeader.bJob == 1)
				{
					strcat(szInfo, " 职业:法师");
				}
				else if(stHeader.bJob == 2)
				{
					strcat(szInfo, " 职业:道士");
				}

				char szLv[10];
				sprintf(szLv, " LV:%d",
					stHeader.uLevel);
				strcat(szInfo, szLv);

				int nTexWidth = GetTextWidth(szInfo, 12);
				int nRenderX = VIEW_WIDTH / 2 - nTexWidth / 2;

				//tex = pTheGame->GetLoginRes()->GetTexture("bmcolor");
				tex = pPngMgr->GetTexture(19);
				if(tex)
				{
					/*m_pRender->SetTexture(tex);
					m_pRender->SetTextureRect(0, 0, nTexWidth + 10, 12 + 4);
					m_pRender->SetColor(ARGB(192, 255, 255, 255));
					m_pRender->Render(nRenderX - 5, 50 - 2);
					m_pRender->SetColor(ARGB_WHITE);*/
					RECT rcScale9 = {9, 9, 9, 9};
					RECT rcTex = {0, 0, 32, 32};
					Gfx_SetRenderState(kGfx_9Path_CenterStretch, 1);
					Gfx_Render9Path(m_pRender, tex, &rcTex, &rcScale9, nRenderX - 5, 50 - 2, nTexWidth + 10, 12 + 4);
				}
				AfxGetPrinter()->SetColor(ARGB_WHITE);
				AfxGetPrinter()->Print(nRenderX, 50, szInfo);

				m_fHumUpdateTime += AfxGetHge()->Timer_GetDelta();
				if(m_fHumUpdateTime > HUM_ANITIME)
				{
					m_fHumUpdateTime = 0.0f;
					++m_dwHumFrame;
					if(m_dwHumFrame == 16)
					{
						m_dwHumFrame = 0;
					}
				}

				int nTextIndex = 20 + stHeader.bJob * 60;
				if(stHeader.bSex == 2)
				{
					nTextIndex += 180;
				}
				nTextIndex += m_dwHumFrame;
				tex = m_pSelChrRes->GetTexture(nTextIndex);
				if(NULL != tex)
				{
					m_pRender->SetTexture(tex);
					m_pRender->SetTextureRect(0, 0,
						m_pSelChrRes->GetTextureWidth(nTextIndex),
						m_pSelChrRes->GetTextureHeight(nTextIndex));
					m_pRender->Render(HUM_POSX + m_pSelChrRes->GetTextureOffsetX(nTextIndex),
						HUM_POSY + m_pSelChrRes->GetTextureOffsetY(nTextIndex));
				}
				tex = m_pSelChrRes->GetTexture(nTextIndex + 20);
				if(NULL != tex)
				{
					m_pEffRender->SetTexture(tex);
					m_pEffRender->SetTextureRect(0, 0,
						m_pSelChrRes->GetTextureWidth(nTextIndex),
						m_pSelChrRes->GetTextureHeight(nTextIndex));
					m_pEffRender->Render(HUM_POSX + m_pSelChrRes->GetTextureOffsetX(nTextIndex),
						HUM_POSY + m_pSelChrRes->GetTextureOffsetY(nTextIndex));
				}
			}
		}
	}

	if(m_bIsConnecting)
	{
		//tex = pTheGame->GetLoginRes()->GetTexture("bmcolor");
		tex = pPngMgr->GetTexture(19);
		if(tex)
		{
			/*m_pRender->SetTexture(tex);
			m_pRender->SetTextureRect(0, 0, 250, 50);
			m_pRender->SetColor(ARGB(192, 255, 255, 255));
			m_pRender->Render(VIEW_WIDTH / 2 - 250 / 2, 250);
			m_pRender->SetColor(ARGB(255, 255, 255, 255));*/
			RECT rcScale9 = {9, 9, 9, 9};
			RECT rcTex = {0, 0, 32, 32};
			Gfx_SetRenderState(kGfx_9Path_CenterStretch, 1);
			Gfx_Render9Path(m_pRender, tex, &rcTex, &rcScale9, VIEW_WIDTH / 2 - 250 / 2, 250, 250, 50);
		}
		static float s_fTime = 0.0f;
		s_fTime += AfxGetHge()->Timer_GetDelta();
		char szBuf[50];

		if(m_fConnectTotalTime < 15.0f)
		{
			if(s_fTime > 3.0f)
			{
				s_fTime = 0.0f;
				sprintf(szBuf, "正在进入游戏.");
			}
			else if(s_fTime > 2.0f)
			{
				sprintf(szBuf, "正在进入游戏...");
			}
			else if(s_fTime > 1.0f)
			{
				sprintf(szBuf, "正在进入游戏..");
			}
			else
			{
				sprintf(szBuf, "正在进入游戏.");
			}
		}
		else
		{
			strcpy(szBuf, "请求超时");
		}
		
		AfxGetPrinter()->SetColor(ARGB_WHITE);
		AfxGetPrinter()->Print(360, 250 + 25 - 6, szBuf);
	}

	m_xDlgCtl.Render();
}

bool SelChrDlg::ProcUserCmd(const POINT& _mp)
{
	if(m_bIsConnecting)
	{
		m_fConnectTotalTime += AfxGetHge()->Timer_GetDelta();
		if(m_fConnectTotalTime > 20.0f)
		{
			m_fConnectTotalTime = 0.0f;
			m_bIsConnecting = false;
		}

		return false;
	}

	if(!m_xDlgCtl.ProcUserCmd(_mp))
	{
		__super::ProcUserCmd(_mp);
	}
	return true;
}

bool SelChrDlg::OnCloseDialog()
{
	return false;
}

bool SelChrDlg::OnCommonButtonClick(int _id)
{
	int nNowHumCount = 0;
	for(int i = 0; i < 3; ++i)
	{
		if(m_stHero[i].szName[0] != 0)
		{
			++nNowHumCount;
		}
	}
	
	int nNowOrder = m_nCurHumOrder;
	bool bFindNew = false;

	if(_id == 0)
	{
		OnCreateNewHum();
		GameSoundManager::GetInstancePtr()->PlayGameSound(SDGAME_CLICKBUTTON);
	}
	else if(_id == 1)
	{
		OnDeleteHum();
		GameSoundManager::GetInstancePtr()->PlayGameSound(SDGAME_CLICKBUTTON);
	}
	else if(_id == 2)
	{
		OnEnterGame();
		GameSoundManager::GetInstancePtr()->PlayGameSound(SDGAME_CLICKBUTTON);
	}
	else if(_id == 3)
	{
		//	prev page
		--m_nCurHumOrder;
		if(m_nCurHumOrder < 0)
		{
			m_nCurHumOrder = 0;
		}

		if(m_stHero[m_nCurHumOrder].szName[0] == 0)
		{
			for(int i = m_nCurHumOrder; i >= 0; --i)
			{
				if(m_stHero[i].szName[i] != 0)
				{
					bFindNew = true;
					m_nCurHumOrder = i;
					m_dwHumFrame = 0;
					break;
				}
			}

			if(!bFindNew)
			{
				m_nCurHumOrder = nNowOrder;
			}
		}

		GameSoundManager::GetInstancePtr()->PlayGameSound(SDGAME_CLICKBUTTON);
	}
	else if(_id == 4)
	{
		//	next page
		++m_nCurHumOrder;
		if(m_nCurHumOrder > 3 - 1)
		{
			m_nCurHumOrder = 3 - 1;
		}

		if(m_stHero[m_nCurHumOrder].szName[0] == 0)
		{
			for(int i = m_nCurHumOrder; i < 3; ++i)
			{
				if(m_stHero[i].szName[0] != 0)
				{
					bFindNew = true;
					m_nCurHumOrder = i;
					m_dwHumFrame = 0;
					break;
				}
			}

			if(!bFindNew)
			{
				m_nCurHumOrder = nNowOrder;
			}
		}

		GameSoundManager::GetInstancePtr()->PlayGameSound(SDGAME_CLICKBUTTON);
	}
	return true;
}

void SelChrDlg::OnConnect()
{
	extern ByteBuffer g_xBuffer;
	g_xBuffer.Reset();

	pTheGame->SetConnect();

	char szPath[MAX_PATH];
	sprintf(szPath, "%s\\Save\\hum.sav",
		GetRootPath());
	if(PathFileExists(szPath))
	{
		CZipArchive xSave;
		if(xSave.Open(szPath))
		{
			xSave.SetPassword(SaveFile::CalcInternalPassword());

			ZIP_INDEX_TYPE zIndex = xSave.FindFile(g_szHeader[m_nCurHumOrder]);
			if(zIndex != ZIP_FILE_INDEX_NOT_FOUND)
			{
				PkgUserLoginReq req;
				CZipFileHeader* pHeader = xSave.GetFileInfo(zIndex);
				if(NULL != pHeader)
				{
					xSave.OpenFile(zIndex);
					char* pBuf = new char[pHeader->m_uUncomprSize];
					xSave.ReadFile(pBuf, pHeader->m_uUncomprSize);
					g_xBuffer.Write(pBuf, pHeader->m_uUncomprSize);
					SAFE_DELETE_ARRAY(pBuf);

					HeroHeader stHeader;
					g_xBuffer >> stHeader;

					GamePlayer::GetInstance()->SetHeroJob(stHeader.bJob);
					GamePlayer::GetInstance()->GetAttrib()->sex = stHeader.bSex;
					GamePlayer::GetInstance()->SetHeroNameInMask(stHeader.szName);
					xSave.CloseFile();

					memcpy(&req.stHeader, &stHeader, sizeof(HeroHeader));

					zIndex = xSave.FindFile(g_szData[m_nCurHumOrder]);
					if(zIndex != ZIP_FILE_INDEX_NOT_FOUND)
					{
						pHeader = xSave.GetFileInfo(zIndex);
						if(NULL != pHeader)
						{
							xSave.OpenFile(zIndex);
							pBuf = new char[pHeader->m_uUncomprSize];
							xSave.ReadFile(pBuf, pHeader->m_uUncomprSize);
							g_xBuffer.Reset();
							g_xBuffer.Write(pBuf, pHeader->m_uUncomprSize);
							SAFE_DELETE_ARRAY(pBuf);
							xSave.CloseFile();

							g_xBuffer >> req.xData;
						}
					}

					g_xBuffer.Reset();
					g_xBuffer << req;
					SendBufferToGS(&g_xBuffer);

					//	extend big storage data
					PkgPlayerLoginExtDataReq ereq;
					char szExtFile[10] = {0};
					sprintf(szExtFile, "ext0_%d", m_nCurHumOrder);
					zIndex = xSave.FindFile(szExtFile);
					if(zIndex != ZIP_FILE_INDEX_NOT_FOUND)
					{
						pHeader = xSave.GetFileInfo(zIndex);
						if(NULL != pHeader)
						{
							if(xSave.OpenFile(zIndex))
							{
								pBuf = new char[pHeader->m_uUncomprSize];
								DWORD dwRead = xSave.ReadFile(pBuf, pHeader->m_uUncomprSize);
								g_xBuffer.Reset();
								g_xBuffer.Write(pBuf, pHeader->m_uUncomprSize);
								SAFE_DELETE_ARRAY(pBuf);
								xSave.CloseFile();

								g_xBuffer >> ereq.xData;
							}
						}
					}

					g_xBuffer.Reset();
					g_xBuffer << ereq;
					SendBufferToGS(&g_xBuffer);

					xSave.Close();
				}
			}
		}
	}
}

void SelChrDlg::OnCreateNewHum()
{
	m_pNewHumDlg->ShowDialog();
}

void SelChrDlg::OnDeleteHum()
{
	m_pDelCfmDlg->SetCurSel(m_nCurHumOrder);
	m_pDelCfmDlg->ShowDialog();
}

void SelChrDlg::LoginOnEnterGame()
{
	
}

void SelChrDlg::OnEnterGame()
{
	if(m_stHero[m_nCurHumOrder].szName[0] != 0)
	{
		if(pTheGame->GetGameMode() == GM_NORMAL)
		{
			m_bIsConnecting = true;
			m_fConnectTotalTime = 0.0f;
			GamePlayer::GetInstance()->SetSaveIndex(m_nCurHumOrder);
			OnConnect();
		}
		else if(pTheGame->GetGameMode() == GM_LOGIN)
		{
			g_xBuffer.Reset();
			g_xBuffer << (int)0;
			g_xBuffer << PKG_LOGIN_STARTGAME_REQ;
			g_xBuffer << (char)strlen(m_stHero[m_nCurHumOrder].szName);
			g_xBuffer.Write(m_stHero[m_nCurHumOrder].szName, strlen(m_stHero[m_nCurHumOrder].szName));
			g_xBuffer << (short)0;
			SendBufferToLS(g_xBuffer);
			m_bIsConnecting = true;
			m_fConnectTotalTime = 0.0f;
			GamePlayer::GetInstance()->SetSaveIndex(m_nCurHumOrder);
			GamePlayer::GetInstance()->SetHeroJob(m_stHero[m_nCurHumOrder].bJob);
			GamePlayer::GetInstance()->GetAttrib()->sex = m_stHero[m_nCurHumOrder].bSex;
			GamePlayer::GetInstance()->SetHeroNameInMask(m_stHero[m_nCurHumOrder].szName);
		}
		else
		{
			if(m_bIsConnecting)
			{
				return;
			}
			ResetStatus();
			HWND hWnd = AfxGetHge()->System_GetState(HGE_HWND);
			//if(!g_xClientSocket.ConnectToServer(hWnd, m_szIP, m_wPort))
			if(!pTheGame->IsConnect())
			{
				if(!g_xClientSocket.ConnectToServer(hWnd, pTheGame->GetLoginScene()->m_szIP,
					pTheGame->GetLoginScene()->m_wPort))
				{
					::MessageBox(hWnd, "无法连接服务器", "错误", MB_ICONERROR | MB_TASKMODAL);
					m_bIsConnecting = false;
				}
				else
				{
					m_bIsConnecting = true;
					m_fConnectTotalTime = 0.0f;
					GamePlayer::GetInstance()->SetSaveIndex(m_nCurHumOrder);
				}
			}
		}
	}
}

int SelChrDlg::GetHumCount()
{
	int nHumCount = 0;

	for(int i = 0; i < 3; ++i)
	{
		if(m_stHero[i].szName[0] != 0)
		{
			++nHumCount;
		}
	}
	return nHumCount;
}