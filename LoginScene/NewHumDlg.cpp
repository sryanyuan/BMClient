#include "NewHumDlg.h"
#include "SelChrDlg.h"
#include "../../CommonModule/SaveFile.h"
#include <ZipArchive.h>
#include "../BackMir/BackMir.h"
#include "../../CommonModule/ProtoType.h"
#include "../../CommonModule/loginsvr.pb.h"
//////////////////////////////////////////////////////////////////////////
#define NHD_WIDTH	584
#define NHD_HEIGHT	416

#define EDIT_POSX	353
#define EDIT_POSY	88
//////////////////////////////////////////////////////////////////////////
NewHumDlg::NewHumDlg()
{
	SetFixed();
	SetCustomBk();
	SetVisible(false);
	SetWindowTitle("创建人物");

	m_rcClient.left = VIEW_WIDTH / 2 - 584 / 2;
	m_rcClient.right = m_rcClient.left + NHD_WIDTH;
	m_rcClient.top = (VIEW_HEIGHT + VIEW_OPERATE) / 2 - 416 / 2;
	m_rcClient.bottom = m_rcClient.top + NHD_HEIGHT;

	m_pRender = new hgeSprite(0, 0, 0, 0, 0);
	m_pParent = NULL;

	m_pEdit = new MyGfxEdit(135, ARGB_WHITE);
	m_pEdit->SetRenderBorder(false);

	m_nSelJob = 0;
	m_nSelSex = 1;

	AddCommonButton(CBTN_NORMAL, 0, 284 + 300 / 2 - 175 / 2 + 75 * 0 + 25 * 0 - 10, 290, "创建");
	AddCommonButton(CBTN_NORMAL, 1, 284 + 300 / 2 - 175 / 2 + 75 * 1 + 25 * 1 - 10, 290, "关闭");

	m_rcJob[0].left = 327;
	m_rcJob[0].right = m_rcJob[0].left + 40;
	m_rcJob[0].top = 151;
	m_rcJob[0].bottom = m_rcJob[0].top + 38;

	m_rcJob[1].left = 377;
	m_rcJob[1].right = m_rcJob[1].left + 40;
	m_rcJob[1].top = 151;
	m_rcJob[1].bottom = m_rcJob[1].top + 38;

	m_rcJob[2].left = 428;
	m_rcJob[2].right = m_rcJob[2].left + 40;
	m_rcJob[2].top = 151;
	m_rcJob[2].bottom = m_rcJob[2].top + 38;

	m_rcJob[3].left = 479;
	m_rcJob[3].right = m_rcJob[3].left + 40;
	m_rcJob[3].top = 151;
	m_rcJob[3].bottom = m_rcJob[3].top + 38;

	m_rcSex[0].left = 378;
	m_rcSex[0].right = m_rcSex[0].left + 40;
	m_rcSex[0].top = 233;
	m_rcSex[0].bottom = m_rcSex[0].top + 38;

	m_rcSex[1].left = 429;
	m_rcSex[1].right = m_rcSex[1].left + 40;
	m_rcSex[1].top = 233;
	m_rcSex[1].bottom = m_rcSex[1].top + 38;

	m_dwHumFrame = 0;
	m_fHumUpdateTime = 0.0f;
}

NewHumDlg::~NewHumDlg()
{
	SAFE_DELETE(m_pRender);
}
//////////////////////////////////////////////////////////////////////////
bool NewHumDlg::ProcUserCmd(const POINT& _mp)
{
	__super::ProcUserCmd(_mp);
	m_pEdit->Updata();

	if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
	{
		POINT ptTest = _mp;
		ptTest.x -= m_rcClient.left;
		ptTest.y -= m_rcClient.top;

		for(int i = 0; i < 4; ++i)
		{
			if(PtInRect(&m_rcJob[i], ptTest))
			{
				if(i == 3)
				{
					m_nSelJob = rand() % 3;
					m_nSelSex = rand() % 2 + 1;
				}
				else
				{
					m_nSelJob = i;
				}
				
				break;
			}
		}

		for(int i = 0; i < 2; ++i)
		{
			if(PtInRect(&m_rcSex[i], ptTest))
			{
				m_nSelSex = i + 1;
				break;
			}
		}
	}
	return true;
}

void NewHumDlg::Render()
{
	HTEXTURE tex = 0;
	tex = m_pParent->GetSelChrRes()->GetTexture(436);
	if(tex)
	{
		m_pRender->SetTexture(tex);
		m_pRender->SetTextureRect(0, 0,
			m_pParent->GetSelChrRes()->GetTextureWidth(436),
			m_pParent->GetSelChrRes()->GetTextureHeight(436));
		m_pRender->Render(m_rcClient.left, m_rcClient.top);
	}
	__super::Render();

	m_pEdit->Render(RELATIVE_X(EDIT_POSX), RELATIVE_Y(EDIT_POSY));

	tex = m_pParent->GetSelChrRes()->GetTexture(441 + 3 * m_nSelJob);
	if(tex)
	{
		m_pRender->SetTexture(tex);
		m_pRender->SetTextureRect(0, 0,
			m_pParent->GetSelChrRes()->GetTextureWidth(441 + 3 * m_nSelJob),
			m_pParent->GetSelChrRes()->GetTextureHeight(441 + 3 * m_nSelJob));
		m_pRender->Render(RELATIVE_X(m_rcJob[m_nSelJob].left), RELATIVE_Y(m_rcJob[m_nSelJob].top));
	}

	tex = m_pParent->GetSelChrRes()->GetTexture(450 + 3 * m_nSelSex - 3);
	if(tex)
	{
		m_pRender->SetTexture(tex);
		m_pRender->SetTextureRect(0, 0,
			m_pParent->GetSelChrRes()->GetTextureWidth(450 + 3 * m_nSelSex - 3),
			m_pParent->GetSelChrRes()->GetTextureHeight(450 + 3 * m_nSelSex - 3));
		m_pRender->Render(RELATIVE_X(m_rcSex[m_nSelSex - 1].left), RELATIVE_Y(m_rcSex[m_nSelSex - 1].top));
	}

	m_fHumUpdateTime += AfxGetHge()->Timer_GetDelta();
	if(m_fHumUpdateTime > 0.15f)
	{
		m_fHumUpdateTime = 0.0f;
		++m_dwHumFrame;
		if(m_dwHumFrame == 16)
		{
			m_dwHumFrame = 0;
		}
	}

	int nTextIndex = 20 + m_nSelJob * 60;
	if(m_nSelSex == 2)
	{
		nTextIndex += 180;
	}
	nTextIndex += m_dwHumFrame;
	tex = m_pParent->GetSelChrRes()->GetTexture(nTextIndex);
	if(NULL != tex)
	{
		m_pRender->SetTexture(tex);
		m_pRender->SetTextureRect(0, 0,
			m_pParent->GetSelChrRes()->GetTextureWidth(nTextIndex),
			m_pParent->GetSelChrRes()->GetTextureHeight(nTextIndex));
		m_pRender->Render(RELATIVE_X(170) + m_pParent->GetSelChrRes()->GetTextureOffsetX(nTextIndex),
			RELATIVE_Y(200) + m_pParent->GetSelChrRes()->GetTextureOffsetY(nTextIndex));
	}
	tex = m_pParent->GetSelChrRes()->GetTexture(nTextIndex + 20);
	if(NULL != tex)
	{
		m_pParent->GetEffRender()->SetTexture(tex);
		m_pParent->GetEffRender()->SetTextureRect(0, 0,
			m_pParent->GetSelChrRes()->GetTextureWidth(nTextIndex),
			m_pParent->GetSelChrRes()->GetTextureHeight(nTextIndex));
		m_pParent->GetEffRender()->Render(RELATIVE_X(170) + m_pParent->GetSelChrRes()->GetTextureOffsetX(nTextIndex),
			RELATIVE_Y(200) + m_pParent->GetSelChrRes()->GetTextureOffsetY(nTextIndex));
	}

	static const char* s_szJobDescriptor[] =
	{
		"战士: 以强有力的体格为基础，特殊的地方在于用剑法及道法等技术。对打猎、战斗比较适用。体力强的战士能带许多东西，即便带沉重的武器及铠甲也可以自由活动。但战士所带的铠甲对魔法的防御能力相对较低。 ",
		"法师: 以长时间锻炼的内功为基础，能发挥强大的攻击型魔法。魔法攻击卓越。体力较弱，对体力上直接受到攻击的防御能力较低，还有能发挥高水平的魔法时需要较长时间，此时可能受到对方的快速攻击。魔法师的魔法比任何攻击能力都强大，能有效的威胁对方。",
		"道士: 以强大的精神力作为基础，只能使用治疗术帮助别人。对自然很熟悉，在用毒方面的能力最强。博学多知，能使用剑术和魔法。所以每时每刻都能发挥多样的法术，随机应变性强。"
	};
	AfxGetPrinter()->LineFeedRender(RELATIVE_X(290 + 5), RELATIVE_Y(330), s_szJobDescriptor[m_nSelJob], 12 * 23, false);
}

bool NewHumDlg::OnShowDialog()
{
	m_pEdit->ClearCookie();
	SetWindowInfo("");
	return true;
}

bool NewHumDlg::OnCommonButtonClick(int _id)
{
	if(_id == 1)
	{
		SetVisible(false);
		GameSoundManager::GetInstancePtr()->PlayGameSound(SDGAME_CLICKBUTTON);
	}
	else
	{
		SetVisible(false);
		if(pTheGame->GetGameMode() == GM_NORMAL)
		{
			ExecuteNewHum();
		}
		else if(pTheGame->GetGameMode() == GM_LOGIN)
		{
			LoginExecuteNewHum();
		}

		GameSoundManager::GetInstancePtr()->PlayGameSound(SDGAME_CLICKBUTTON);
	}
	return true;
}

void NewHumDlg::ExecuteNewHum()
{
	if(m_nSelJob < 0 ||
		m_nSelJob > 2)
	{
		return;
	}
	if(m_nSelSex < 1 ||
		m_nSelSex > 2)
	{
		return;
	}
	if(strlen(m_pEdit->GetCookie()) == 0)
	{
		return;
	}
	if(strlen(m_pEdit->GetCookie()) >= 19)
	{
		return;
	}

	if(m_pParent->GetHumCount() < 3)
	{
		int nEmptyHum = -1;
		ZIP_INDEX_TYPE zIndex = 0;

		char szPath[MAX_PATH];
		sprintf(szPath, "%s\\save\\hum.sav",
			GetRootPath());
		if(PathFileExists(szPath))
		{
			CZipArchive xSave;
			if(xSave.Open(szPath))
			{
				xSave.SetPassword(SaveFile::CalcInternalPassword());
				char szFile[10];
				strcpy(szFile, "head");

				zIndex = xSave.FindFile(szFile);
				if(zIndex == ZIP_FILE_INDEX_NOT_FOUND)
				{
					nEmptyHum = 0;
				}
				else
				{
					zIndex = xSave.FindFile("head1");
					if(zIndex == ZIP_FILE_INDEX_NOT_FOUND)
					{
						nEmptyHum = 1;
					}
					else
					{
						zIndex = xSave.FindFile("head2");
						if(zIndex == ZIP_FILE_INDEX_NOT_FOUND)
						{
							nEmptyHum = 2;
						}
					}
				}

				const char* pHeadName = NULL;
				if(nEmptyHum != -1)
				{
					if(nEmptyHum == 0)
					{
						pHeadName = "head";
					}
					else if(nEmptyHum == 1)
					{
						pHeadName = "head1";
					}
					else if(nEmptyHum == 2)
					{
						pHeadName = "head2";
					}

					const char* pszHeroName = m_pEdit->GetCookie();
					if(strlen(pszHeroName) < 19)
					{
						if(m_nSelJob >= 0 &&
							m_nSelJob <= 2 &&
							m_nSelSex >= 1 &&
							m_nSelSex <= 2)
						{
							HeroHeader stHeader;
							strcpy(stHeader.szName, pszHeroName);
							stHeader.bJob = m_nSelJob;
							stHeader.bSex = m_nSelSex;
							stHeader.uLevel = 1;

							g_xBuffer.Reset();
							g_xBuffer << stHeader;

							CZipFileHeader zHeader;
							zHeader.SetFileName(pHeadName);
							xSave.OpenNewFile(zHeader);
							xSave.WriteNewFile(g_xBuffer.GetBuffer(), g_xBuffer.GetLength());
							xSave.CloseNewFile();

							m_pParent->SetHumData(nEmptyHum, &stHeader);
							m_pParent->SetHumOrder(nEmptyHum);
						}
					}
					else
					{
						SetWindowInfo("英雄名字应该限制在20个字符以下");
					}
				}
				else
				{
					SetWindowInfo("最多创建3个人物");
				}

				xSave.Close();
			}
		}
	}
}

void NewHumDlg::LoginExecuteNewHum()
{
	if(m_nSelJob < 0 ||
		m_nSelJob > 2)
	{
		return;
	}
	if(m_nSelSex < 1 ||
		m_nSelSex > 2)
	{
		return;
	}
	if(strlen(m_pEdit->GetCookie()) == 0)
	{
		return;
	}
	if(strlen(m_pEdit->GetCookie()) >= 19)
	{
		return;
	}

	const char* pszHeroName = m_pEdit->GetCookie();
	if(strlen(pszHeroName) > 19)
	{
		SetWindowInfo("英雄名字应该限制在20个字符以下");
		return;
	}

	if(m_pParent->GetHumCount() < 3)
	{
		if (GetProtoType() == ProtoType_ByteBuffer)
		{
			g_xBuffer.Reset();
			g_xBuffer << (int)0;
			g_xBuffer << (int)PKG_LOGIN_ADDGAMEROLE_REQ;
			g_xBuffer << (char)strlen(pszHeroName);
			g_xBuffer.Write(pszHeroName, strlen(pszHeroName));
			g_xBuffer << (char)m_nSelJob;
			g_xBuffer << (char)m_nSelSex;
			SendBufferToLS(g_xBuffer);
		}
		else
		{
			protocol::MCreateHumReq req;
			req.set_name(pszHeroName);
			req.set_job(m_nSelJob);
			req.set_sex(m_nSelSex);
			SendProto(protocol::CreateHumReq, req);
		}
	}
	else
	{
		SetWindowInfo("最多创建3个人物");
	}
}