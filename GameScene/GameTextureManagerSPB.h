#ifndef _INC_GAMETEXTUREMANAGERSPB_
#define _INC_GAMETEXTUREMANAGERSPB_
//////////////////////////////////////////////////////////////////////////
#include "GameTextureManager.h"
#include "../Common/SPBFile.h"
//////////////////////////////////////////////////////////////////////////
class GameTextureManagerSpb : public SPBFile, public GameTextureManager
{
public:
	GameTextureManagerSpb();
	virtual ~GameTextureManagerSpb();

public:
	virtual bool LoadPackage(const char* lpszFileName);

	virtual HTEXTURE GetTexture(int _index);
	virtual int GetTextureWidth(int _idx);
	virtual int GetTextureHeight(int _idx);
	virtual void ReleaseAllTexture();
	virtual short GetTextureOffsetX(int _idx);
	virtual short GetTextureOffsetY(int _idx);

protected:
	DWORD* m_pTexIndexTable;
};
//////////////////////////////////////////////////////////////////////////
#endif