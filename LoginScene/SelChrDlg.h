#ifndef _INC_SELCHRDLG_
#define _INC_SELCHRDLG_
//////////////////////////////////////////////////////////////////////////
#include "../GameDialog/GameCommonDlg.h"
#include "../GameDialog/GameDlgBase.h"
#include "../GameScene/GameTextureManagerSib.h"
//////////////////////////////////////////////////////////////////////////
class DeleteConfirmDlg;
class NewHumDlg;
//////////////////////////////////////////////////////////////////////////
class SelChrDlg : public GameNewDlg
{
public:
	SelChrDlg();
	virtual ~SelChrDlg();

public:
	virtual void Render();
	virtual bool ProcUserCmd(const POINT& _mp);

	virtual bool OnCloseDialog();
	virtual bool OnCommonButtonClick(int _id);

public:
	inline void SetData(const HeroHeader* _pHeader)
	{
		memcpy(&m_stHero, _pHeader, sizeof(HeroHeader) * 3);
		for(int i = 0; i < 3; ++i)
		{
			if(m_stHero[i].szName[0] != 0)
			{
				m_nCurHumOrder = i;
				break;
			}
		}
	}
	inline void SetHumOrder(int _order)
	{
		m_nCurHumOrder = _order;
	}
	void OnConnect();
	inline void DeleteHum(int _idx)
	{
		ZeroMemory(&m_stHero[_idx], sizeof(HeroHeader));
		bool bFindNew = false;
		for(int i = 0; i < 3; ++i)
		{
			if(m_stHero[i].szName[0] != 0)
			{
				m_nCurHumOrder = i;
				bFindNew = true;
				break;
			}
		}

		if(!bFindNew)
		{
			m_nCurHumOrder = 0;
		}
	}
	inline void SetHumData(int _idx, const HeroHeader* _pHeader)
	{
		memcpy(&m_stHero[_idx], _pHeader, sizeof(HeroHeader));
	}
	inline const char* GetHumName(int _idx)
	{
		if(_idx >= 0 &&
			_idx <= 2)
		{
			return m_stHero[_idx].szName;
		}
		return "";
	}
	inline int GetHumIndex(const char* _pszName)
	{
		for(int i = 0; i < 3; ++i)
		{
			if(0 == strcmp(_pszName, m_stHero[i].szName))
			{
				return i;
			}
		}
		return -1;
	}
	inline void AddHumData(const HeroHeader* _pHeader)
	{
		for(int i = 0; i < 3; ++i)
		{
			if(m_stHero[i].szName[0] == 0)
			{
				memcpy(&m_stHero[i], _pHeader, sizeof(HeroHeader));
				SetHumOrder(i);
				break;
			}
		}
	}
	inline void DelHumData(const char* _pszName)
	{
		for(int i = 0 ; i < 3; ++i)
		{
			if(0 == strcmp(m_stHero[i].szName, _pszName))
			{
				ZeroMemory(&m_stHero[i], sizeof(HeroHeader));
				break;
			}
		}
	}
	inline hgeSprite* GetEffRender()
	{
		return m_pEffRender;
	}
	inline GameTextureManager* GetSelChrRes()
	{
		return m_pSelChrRes;
	}
	int GetHumCount();

	void OnCreateNewHum();
	void OnDeleteHum();
	void OnEnterGame();
	void LoginOnEnterGame();

private:
	int m_nCurHumOrder;
	HeroHeader m_stHero[3];
	bool m_bIsConnecting;
	RenderDlgControl m_xDlgCtl;
	DeleteConfirmDlg* m_pDelCfmDlg;
	NewHumDlg* m_pNewHumDlg;
	float m_fConnectTotalTime;
	GameTextureManager* m_pSelChrRes;
	hgeSprite* m_pEffRender;

	float m_fHumUpdateTime;
	DWORD m_dwHumFrame;
	float m_fFireUpdateTime;
	DWORD m_dwFireFrame;
};
//////////////////////////////////////////////////////////////////////////
#endif