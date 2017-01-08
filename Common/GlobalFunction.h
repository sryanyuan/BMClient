#ifndef _GLOBALFUNCTION_H_
#define _GLOBALFUNCTION_H_

#include <Windows.h>
#include "GlobalDefine.h"
#include <map>
#include <list>
#include <string>

typedef std::list<std::string> StringList;

//#include <hge.h>
//#include "GamePlayer.h"
class GamePlayer;
class GameMapManager;
class HGE;
class GameTextureManager;
class GfxFont;
class TextPrinter;
class ByteBuffer;

void InitGameFonts();

float GetLengthOf2Point(const POINT& p1, const POINT& p2);
PLAYER_DIRECTION GetAngle(const POINT& p1, const POINT& p2);
int GetDirectionOf2(const POINT& atk, const POINT& atked, int atkdis = 1);


HGE* AfxGetHge();
void AfxInitHge(HGE* _hge);

GamePlayer* AfxGetPlayer();
void AfxInitPlayer(GamePlayer* _player);

GameMapManager* AfxGetMap();
void AfxInitMap(GameMapManager* _map);

GameTextureManager* AfxGetTexture(GAME_RES res);
void AfxInitTexture(GameTextureManager* _res);

GfxFont* AfxGetFont();
GfxFont* AfxGetFont10();
GfxFont* AfxGetGmFont();
GfxFont* AfxGetFont14();
TextPrinter* AfxGetPrinter();

const char* GetRootPath();
void GetRootPath(char* _pszBuf, unsigned int _sz);

unsigned int SendBuffer(ByteBuffer* _xBuf, bool _bNeedCheckSum = false);
unsigned int SendBuffer2(ByteBuffer* _xBuf, bool _bNeedCheckSum = false);

unsigned int GetTextWidth(const char* _pText, size_t _nFontSize);
int SplitTextWithLength(const char* _pText, int _nFontSize, int _nLineWidth, StringList& _refStringList);
int GetTextLine(const char* _pText, int _nFontSize, int _nLineWidth);

void MirLog(const char* _pszLog);

int GetDirection16(int _sx, int _sy, int _dx, int _dy);
int GetDirection8(int _sx, int _sy, int _dx, int _dy);

int GetNumberBit(int _number);

int GetSelfPlayerHandlerID();

#define GETSELFPLAYERHANDLERID	GamePlayer::GetInstance()->GetHandlerID()

extern HDC g_hWndDC;
extern char g_szNameMask;

typedef std::map<int, RECT> Int2RectMap;
typedef std::map<int, Int2RectMap*> ObjRectCache;
extern ObjRectCache g_xRectCache;
void ClearRectCache();

std::string UTF8ToGBK(const std::string& strUTF8);
//////////////////////////////////////////////////////////////////////////
#endif