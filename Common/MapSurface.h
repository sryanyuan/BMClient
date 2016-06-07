#ifndef MAP_SURFACE_H_
#define MAP_SURFACE_H_

#include "CTextureBuffer.h"

/************************************************************************/
/* µØÍ¼»º´æ Æ«ÒÆ»æÖÆ
/************************************************************************/
#define TOP_OFFSET_ROW		2
#define BOTTOM_OFFSET_ROW	4
#define LEFT_OFFSET_COL		2
#define RIGHT_OFFSET_COL	2

class GameMapManager;

class MapSurface : public CTextureBuffer
{
public:
	MapSurface();
	~MapSurface();

public:
	bool ScrollMap(int _nRow, int _nCol);
	bool DrawEntire();
	bool EnterMap();

protected:
	void DrawOneCol(int _nCol);
	void DrawOneRow(int _nRow);

private:
	static GameMapManager* s_pMapMgr;
};

#endif