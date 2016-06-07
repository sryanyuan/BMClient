#ifndef _INC_GAMETEXTUREMANAGERWIL_
#define _INC_GAMETEXTUREMANAGERWIL_
//////////////////////////////////////////////////////////////////////////
#include "GameTextureManager.h"
#include "../Common/MirMap.h"
//////////////////////////////////////////////////////////////////////////
class GameTextureManagerWil : public SWilFile, public GameTextureManager
{
public:
	GameTextureManagerWil();
	virtual ~GameTextureManagerWil();

public:
	virtual bool LoadPackage(const char* lpszFileName);

	virtual HTEXTURE GetTexture(int _index);
	virtual int GetTextureWidth(int _idx);
	virtual int GetTextureHeight(int _idx);
	virtual void ReleaseAllTexture();
	virtual short GetTextureOffsetX(int _idx);
	virtual short GetTextureOffsetY(int _idx);

private:
	bool LoadImageInfo();

protected:
	DWORD* m_pTexIndexTable;
	ImageInfo* m_pImageInfo;
};
//////////////////////////////////////////////////////////////////////////
#endif