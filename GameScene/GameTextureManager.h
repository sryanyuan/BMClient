#ifndef GAMETEXTUREMANAGER_H_
#define GAMETEXTUREMANAGER_H_

#include "../Common/MirMap.h"
#include <hge.h>
#include <hash_map>
#include <map>
#include <Windows.h>

//using std::hash_map;
using std::map;

typedef map<HTEXTURE, int> HashTex2Index;
typedef map<int, HTEXTURE> HashIndex2Tex;

class GameTextureManager
{
public:
	GameTextureManager();
	virtual ~GameTextureManager();

public:
	virtual bool LoadPackage(const char* _pszFile) = 0;
	virtual HTEXTURE GetTexture(int _index) = 0;
	
	virtual int GetTextureWidth(int _index) = 0;
	virtual int GetTextureHeight(int _index) = 0;
	virtual short GetTextureOffsetX(int _index) = 0;
	virtual short GetTextureOffsetY(int _index) = 0;
	virtual void ReleaseAllTexture() = 0;

public:
	static void Init(HDC _dc, HGE* _phge);

protected:
	static HDC s_HDC;
	static HGE* s_pHGE;
};



#endif