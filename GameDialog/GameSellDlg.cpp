#include "../GameDialog/GameDlgBase.h"
#include "../GameDialog/GameInfoBoardDlg.h"
#include "../GameScene/GameResourceManager.h"
#include "GameDisplayDlg.h"

GameSellDlg::GameSellDlg()
{
	m_pParent = NULL;
	m_pRes =NULL;
	m_pSpr = NULL;
	//m_pFont = NULL;

	m_rcClient.right = 339;
	m_rcClient.bottom = 360;
	m_nPage = 0;

	m_bVisible = false;
	m_nPrintPriceIdx = -1;
	m_nSelIdx = -1;
}

GameSellDlg::~GameSellDlg()
{
	//
	//delete m_pFont;
	delete m_pSpr;
}


void GameSellDlg::Init(hgeResourceManager* _res, GameMainOptUI* _parent)
{
	m_pRes = _res;
	m_pParent = _parent;
	//m_pFont = new GfxFont("宋体", 12, 0, 0, 0);
}

void GameSellDlg::AddItem(int _id)
{

}

void GameSellDlg::Reset()
{

}

bool GameSellDlg::IsCaptionBar(int _x, int _y)
{
	//	是否标题栏
	if(_x >= RELATIVE_X(0) &&
		_x <= RELATIVE_X(338) &&
		_y >= RELATIVE_Y(9) &&
		_y <= RELATIVE_Y(40))
	{
		if(_x > RELATIVE_X(314))
		{
			//	关闭
			if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
			{
				SetVisible(!m_bVisible);
			}
			return false;
		}
		return true;
	}
	return false;
}

bool GameSellDlg::ProcUserCmd(const POINT &_mp)
{
	__super::ProcUserCmd(_mp);

	m_pParent->GetIntroDlg()->ShowNothing();
	m_nPrintPriceIdx = -1;

	int nDetectX = 0;
	int nDetectY = 0;

	//	第一列
	if(_mp.x > RELATIVE_X(35) &&
		_mp.x < RELATIVE_X(73) &&
		_mp.y > RELATIVE_Y(61) &&
		_mp.y < RELATIVE_Y(304))
	{
		//	在一列内
		nDetectX = _mp.x - m_rcClient.left - 35;
		nDetectY = _mp.y - m_rcClient.top - 61;

		if(nDetectY - (nDetectY / 41) * 41 < 37)
		{
			OnItem(0, nDetectY / 41);
			return true;
		}
	}

	//	第二列
	if(_mp.x > RELATIVE_X(178) &&
		_mp.x < RELATIVE_X(215) &&
		_mp.y > RELATIVE_Y(62) &&
		_mp.y < RELATIVE_Y(305))
	{
		//	在第二列内
		nDetectY = _mp.y - m_rcClient.top - 61;

		if(nDetectY - (nDetectY / 41) * 41 < 37)
		{
			OnItem(1, nDetectY / 41);
			return true;
		}
	}

	//	按钮
	if(_mp.x > RELATIVE_X(52) &&
		_mp.x < RELATIVE_X(289) &&
		_mp.y > RELATIVE_Y(332) &&
		_mp.y < RELATIVE_Y(353))
	{
		//	区域内
		if(_mp.x < RELATIVE_X(117))
		{
			OnBuyButton();
			return true;
		}
		if(_mp.x > RELATIVE_X(138) &&
			_mp.x < RELATIVE_X(202))
		{
			OnSellButton();
			return true;
		}
		if(_mp.x > RELATIVE_X(225) &&
			_mp.x < RELATIVE_X(289))
		{
			OnFixButton();
			return true;
		}
	}

	return true;
}

void GameSellDlg::Update(float _dt)
{

}

void GameSellDlg::Render()
{
	const int nDrawY[] =
	{
		64,104,144,184,224,264
	};
	const int nDrawX[] =
	{
		35,178
	};

	if(!m_pRes)
	{
		return;
	}

	HTEXTURE tex = 0;
	tex = m_pRes->GetTexture("selldlg");
	if(tex)
	{
		INITSPRITE(m_pSpr, tex);
		m_pSpr->Render(m_rcClient.left, m_rcClient.top);
	}

	int nSize = m_items.size();
	int nDrawItemIdx = m_nPage * 12;
	GameTextureManager* ttex = GameResourceManager::GetInstance()->GetTexs(RES_ITEMS);
	int nTexIdx = 0;
	USHORT sTexWidth = 0;
	USHORT sTexHeight = 0;

	if(nSize > 0 &&
		nDrawItemIdx < nSize)
	{
		//	画商店物品
		if(ttex)
		{
			for(int i = 0; i < 12; ++i)
			{
				if(nDrawItemIdx >= nSize)
				{
					break;
				}
				//	Draw items
				nTexIdx = m_items[nDrawItemIdx].tex;
				tex = ttex->GetTexture(nTexIdx);
				if(tex)
				{
					m_pSpr->SetTexture(tex);
					sTexWidth = ttex->GetTextureWidth(nTexIdx);
					sTexHeight = ttex->GetTextureHeight(nTexIdx);
					m_pSpr->SetTextureRect(0, 0, sTexWidth, sTexHeight);
					m_pSpr->Render(RELATIVE_X((i < 6 ? nDrawX[0] : nDrawX[1]) + abs(sTexWidth / 2 - 17)),
						RELATIVE_Y(nDrawY[i % 6] + abs(sTexHeight / 2 - 17)));
				}

				++nDrawItemIdx;
			}
		}
	}

	//	价格
	if(m_nPrintPriceIdx != -1)
	{
		if(m_nPrintPriceIdx < nSize)
		{
			int nPrice = GameInfoManager::GetInstance()->GetItemPrice(&m_items[m_nPrintPriceIdx]);
			//m_pFont->SetColor(ARGB_YELLOW);
			AfxGetPrinter()->SetColor(ARGB_YELLOW);
// 			m_pFont->Print(RELATIVE_X(m_nPrintPriceIdx < 6 ? (nDrawX[0] + 45) : (nDrawX[1] + 45)),
// 				RELATIVE_Y(nDrawY[m_nPrintPriceIdx % 6] + 10),
// 				"价格: %d", nPrice);
			AfxGetPrinter()->Print(RELATIVE_X(m_nPrintPriceIdx < 6 ? (nDrawX[0] + 45) : (nDrawX[1] + 45)),
				RELATIVE_Y(nDrawY[m_nPrintPriceIdx % 6] + 10),
				"价格: %d", nPrice);
		}
	}

	//	选择
	if(m_nSelIdx != -1)
	{
		if(m_nSelIdx < 12)
		{
			tex = m_pRes->GetTexture("select");
			if(tex)
			{
				SETSPRITE(tex, m_pSpr);
				m_pSpr->Render(RELATIVE_X(m_nSelIdx < 6 ? (nDrawX[0] + 2) : (nDrawX[1] + 2)),
					RELATIVE_Y(nDrawY[m_nSelIdx % 6] - 1));
			}
		}
	}
}

void GameSellDlg::OnItem(int _x, int _y)
{
	int nIndex = _y + 6 * _x + m_nPage * 12;
	if(nIndex < m_items.size())
	{
		m_pParent->GetIntroDlg()->ShowItemAttrib(&m_items[nIndex]);
		m_pParent->GetDisplayDlg()->ShowItemAttrib(&m_items[nIndex]);
		m_nPrintPriceIdx = nIndex;

		if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
		{
			m_nSelIdx = nIndex;
			m_nSelIdx %= 12;
		}
	}
}

void GameSellDlg::OnBuyButton()
{
	if(AfxGetHge()->Input_KeyUp(HGEK_LBUTTON))
	{
		//	左键按下
		int nIndex = m_nPage * SELLDLG_PER_PAGE + m_nSelIdx;
		if(nIndex < m_items.size())
		{
			//	Valid
			GamePlayer* pPlayer = GamePlayer::GetInstance();
			int nMoney = pPlayer->GetPlayerBag()->GetMoney();
			int nPrice = GameInfoManager::GetInstance()->GetItemPrice(&m_items[nIndex]);
			if(nMoney >= nPrice)
			{
				if(pPlayer->GetPlayerBag()->AddItem(&m_items[nIndex]))
				{
					//	成功
					GameInfoBoardDlg::GetInstance()->ShowItemMsg(&m_items[nIndex]);
					ZeroMemory(&m_items[nIndex], sizeof(ItemAttrib));
				}
				else
				{
					//	失败
					GameInfoBoardDlg::GetInstance()->InsertBoardMsg(ARGB_RED, "您的背包已满");
				}
			}
			else
			{
				//	钱不够
				GameInfoBoardDlg::GetInstance()->InsertBoardMsg(ARGB_RED, "您的金币不够");
			}
		}
	}
}

void GameSellDlg::OnSellButton()
{
	int nIndex = m_nPage * SELLDLG_PER_PAGE + m_nSelIdx;
	if(nIndex < m_items.size())
	{
		//	Valid
		GamePlayer* pPlayer = GamePlayer::GetInstance();
	}
}

void GameSellDlg::OnFixButton()
{
	int nIndex = m_nPage * SELLDLG_PER_PAGE + m_nSelIdx;
	if(nIndex < m_items.size())
	{
		//	Valid
		GamePlayer* pPlayer = GamePlayer::GetInstance();
	}
}

void GameSellDlg::CreateSellDlg(BYTE _type, int _minlevel, int _maxlevel)
{
	if(!GameInfoManager::GetInstance()->CanUseSQL())
	{
		AfxGetHge()->System_Log("由于没有加载数据库，无法生成商店窗口");
		return;
	}

	m_items.clear();

	if(!GameInfoManager::GetInstance()->GetShopSellItems(&m_items, _type, _minlevel, _maxlevel))
	{
		AfxGetHge()->System_Log("创建商店窗口失败 TYPE[%d] MINLEVEL[%d] MAXLEVEL[%d]",
			_type,
			_minlevel,
			_maxlevel);
		return;
	}

	//	更新页数
	m_nPage = m_items.size() / 12;

	m_bVisible = true;
}