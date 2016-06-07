#ifndef GAMEMINIMAPDLG_H_
#define GAMEMINIMAPDLG_H_
#include "../Common/GameRenderObject.h"
#include <hgeSprite.h>
#include "../Common/GlobalFunction.h"
#include "../GameDialog/GameCommonDlg.h"

class GameMiniMapDlg : public RenderObject
{
public:
	GameMiniMapDlg();
	
	~GameMiniMapDlg()
	{
//		delete m_pSpr;
//		m_pSpr = NULL;
		delete m_pPoint;
		m_pPoint = NULL;
		SAFE_DELETE(m_pRender);
		if(m_texPoint)
		{
			AfxGetHge()->Texture_Free(m_texPoint);
			m_texPoint = NULL;
		}
	}

public:
	bool SetMap(const char* lpszMapName);
	bool SetMap(DWORD _dwMapID);
	bool UpdatePos();
	inline HTEXTURE GetPointTexture()
	{
		return m_texPoint;
	}

public:
	virtual void Render();
	virtual void Update(float _dt);
	virtual bool ProcUserCmd(const POINT& _mp);

private:
	//hgeSprite* m_pSpr;
	hgeSprite* m_pPoint;
	//float m_fScaleX;
	//float m_fScaleY;
	//int m_nOffsetX;
	//int m_nOffsetY;
	HTEXTURE m_texPoint;
	HTEXTURE m_texMap;
	bool m_bLoadOk;
	int m_nMMapWidth;
	int m_nMMapHeight;
	int m_nPointPosX;
	int m_nPointPosY;

	BYTE m_bMiniBtnState;
	bool m_bMiniMode;
	RECT m_rcMini;

	DWORD m_dwMapID;
};

#endif
