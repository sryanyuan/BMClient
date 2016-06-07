#ifndef _INC_GAMETEXTUREMANAGERSIB_
#define _INC_GAMETEXTUREMANAGERSIB_
//////////////////////////////////////////////////////////////////////////
#include "GameTextureManager.h"
#include "../Common/SIBFile.h"
//////////////////////////////////////////////////////////////////////////
class GameTextureManagerSib : public SIBFile, public GameTextureManager
{
public:
	GameTextureManagerSib();
	virtual ~GameTextureManagerSib();

public:
	virtual bool LoadPackage(const char* lpszFileName);

	virtual HTEXTURE GetTexture(int _index);
	virtual int GetTextureWidth(int _idx);
	virtual int GetTextureHeight(int _idx);
	virtual void ReleaseAllTexture();
	virtual short GetTextureOffsetX(int _idx);
	virtual short GetTextureOffsetY(int _idx);

protected:
	ImageInfo* m_pImageInfo;
	DWORD* m_pTexIndexTable;
};
//////////////////////////////////////////////////////////////////////////
#endif