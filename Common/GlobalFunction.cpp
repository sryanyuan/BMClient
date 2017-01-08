#include "../Common/GlobalFunction.h"
#include <math.h>
#include <hge.h>
#include "../GameScene/GameMapManager.h"
#include "../GameScene/GamePlayer.h"
#include "../GameScene/GameTextureManager.h"
#include "../Common/GfxFont.h"
#include "../Common/SKnl3Helper.h"
#include "../Common/TextPrinter.h"
#include "../Net/ClientSocket.h"
#include "../../CommonModule/ByteBuffer.h"
#include <Shlwapi.h>
#include "../BackMir/BackMir.h"
#include "../../CommonModule/DataEncryptor.h"

static HGE* pHGE = NULL;
static GamePlayer* pPlayer = NULL;
static GameMapManager* pMap = NULL;
static GameTextureManager* texs = NULL;
static GfxFont* font = NULL;
static GfxFont* font10 = NULL;
static GfxFont* font14 = NULL;
static GfxFont* gmfont = NULL;
static char szRootPath[MAX_PATH] = {0};
static TextPrinter* printer = NULL;

HDC g_hWndDC = 0;
char g_szNameMask = 0x72;
ObjRectCache g_xRectCache;

void InitGameFonts()
{
	font = new GfxFont("宋体", 12, FALSE, FALSE, FALSE);
	font10 = new GfxFont("宋体", 10, FALSE, FALSE, FALSE);
	font14 = new GfxFont("宋体", 14, TRUE, FALSE, FALSE);
	gmfont = new GfxFont("宋体", 20, TRUE, FALSE, FALSE);
	printer = new TextPrinter;
}

int GetSelfPlayerHandlerID()
{
	return GamePlayer::GetInstance()->GetHandlerID();
}

const char* GetRootPath()
{
// 	if(szRootPath[0] == '\0')
// 	{
// 		SKNL3GetCurrentAppPath(szRootPath, MAX_PATH);
// 	}
	if(szRootPath[0] == 0)
	{
		GetRootPath(szRootPath, MAX_PATH);
	}
	return szRootPath;
}

void GetRootPath(char* _pszBuf, unsigned int _sz)
{
	GetModuleFileName(NULL, _pszBuf, _sz);
	PathRemoveFileSpec(_pszBuf);
#ifdef _BIN_PATH
	// remove current path
	size_t uStrlen = strlen(_pszBuf);
	if (0 == uStrlen)
	{
		return;
	}
	for (size_t i = uStrlen - 1; i >= 0; --i)
	{
		if (_pszBuf[i] == '\\' ||
			_pszBuf[i] == '/')
		{
			// done
			break;
		}
		_pszBuf[i] = '\0';
	}
#endif
}

GfxFont* AfxGetFont()
{
	return font;
}

GfxFont* AfxGetFont10()
{
	return font10;
}

GfxFont* AfxGetGmFont()
{
	return gmfont;
}

GfxFont* AfxGetFont14()
{
	return font14;
}

TextPrinter* AfxGetPrinter()
{
	return printer;
}

HGE* AfxGetHge()
{
	return pHGE;
}

GamePlayer* AfxGetPlayer()
{
	return pPlayer;
}

GameMapManager* AfxGetMap()
{
	return pMap;
}

GameTextureManager* AfxGetTexture(GAME_RES res)
{
	return &texs[res];
}

void AfxInitHge(HGE* _hge)
{
	pHGE = _hge;
}

void AfxInitPlayer(GamePlayer* _player)
{
	pPlayer = _player;
}

void AfxInitMap(GameMapManager* _map)
{
	pMap = _map;
}

void AfxInitTexture(GameTextureManager* _res)
{
	texs = _res;
}

float GetLengthOf2Point(const POINT& p1, const POINT& p2)
{
	float value = (float)(pow((float)(p1.x - p2.x), 2) + pow((float)(p1.y - p2.y), 2));
	return sqrt(value);
}

PLAYER_DIRECTION GetAngle(const POINT& p1, const POINT& p2)
{
	float len1 = 0;
	float len2 = 0;
	float value = 0;

	if(p2.x >= p1.x &&
		p1.y >= p2.y)
	{
		//	左 左上 上
		len1 = p1.y - p2.y;
		len2 = GetLengthOf2Point(p1, p2);
		value = len1 / len2;
		value = asin(value);

		if(value < _PI_ / 8)
		{
			return PDT_RIGHT;
		}
		else if(value < _PI_ * 3 / 8)
		{
			return PDT_RIGHTUP;
		}
		else
		{
			return PDT_UP;
		}
	}
	else if(p2.x <= p1.x &&
		p2.y <= p1.y)
	{
		len1 = p1.x - p2.x;
		len2 = GetLengthOf2Point(p1, p2);
		value = len1 / len2;
		value = asin(value);

		if(value < _PI_ / 8)
		{
			return PDT_UP;
		}
		else if( value < _PI_ * 3 / 8)
		{
			return PDT_LEFTUP;
		}
		else
		{
			return PDT_LEFT;
		}
	}
	else if(p2.x <= p1.x &&
		p2.y >= p1.y)
	{
		len1 = p2.y - p1.y;
		len2 = GetLengthOf2Point(p1, p2);
		value = len1 / len2;
		value = asin(value);

		if(value < _PI_ / 8)
		{
			return PDT_LEFT;
		}
		else if(value < _PI_ * 3 / 8)
		{
			return PDT_LEFTDOWN;
		}
		else
		{
			return PDT_DOWN;
		}
	}
	else
	{
		len1 = p2.y - p1.y;
		len2 = GetLengthOf2Point(p1, p2);
		value = len1 / len2;
		value = asin(value);

		if(value < _PI_ / 8)
		{
			return PDT_RIGHT;
		}
		else if(value < _PI_ * 3 / 8)
		{
			return PDT_RIGHTDOWN;
		}
		else
		{
			return PDT_DOWN;
		}
	}
}

int GetDirectionOf2(const POINT& atk, const POINT& atked, int atkdis)
{
	for(int i = 1; i < atkdis + 1; ++i)
	{
		if(atk.x == atked.x &&
			atk.y == atked.y - i)
		{
			return PDT_DOWN;
		}
		if(atk.x == atked.x &&
			atk.y == atked.y + i)
		{
			return PDT_UP;
		}
		if(atk.y == atked.y &&
			atk.x == atked.x - i)
		{
			return PDT_RIGHT;
		}
		if(atk.y == atked.y &&
			atk.x == atked.x + i)
		{
			return PDT_LEFT;
		}
		if(atk.x == atked.x - i &&
			atk.y == atked.y - i)
		{
			return PDT_RIGHTDOWN;
		}
		if(atk.x == atked.x - i &&
			atk.y == atked.y + i)
		{
			return PDT_RIGHTUP;
		}
		if(atk.x == atked.x + i &&
			atk.y == atked.y - i)
		{
			return PDT_LEFTDOWN;
		}
		if(atk.x == atked.x + i &&
			atk.y == atked.y + i)
		{
			return PDT_LEFTUP;
		}
	}
	
	return PDT_LEFTUP + 1;
}

unsigned int SendBuffer(ByteBuffer* _xBuf, bool _bNeedCheckSum)
{
	DWORD dwPktLength = _xBuf->GetLength();
	DWORD* pWriteData = (DWORD*)const_cast<unsigned char*>(_xBuf->GetBuffer());
	*pWriteData = htonl(dwPktLength);

	if(_bNeedCheckSum)
	{
		const char* pBuffer = (const char*)pWriteData + 8;
		DWORD dwOpCode = *(pWriteData + 1);
		unsigned int uLen = dwPktLength - 8;
		WORD wCheckSum = DataEncryptor::GetCheckSum(dwOpCode, pBuffer, uLen);

		DWORD dwData = MAKELONG(dwOpCode, wCheckSum);
		*(pWriteData + 1) = dwData;
	}

	return g_xClientSocket.SendToServer((const char*)_xBuf->GetBuffer(), _xBuf->GetLength());
}

unsigned int SendBuffer2(ByteBuffer* _xBuf, bool _bNeedCheckSum)
{
	DWORD dwPktLength = _xBuf->GetLength();
	DWORD* pWriteData = (DWORD*)const_cast<unsigned char*>(_xBuf->GetBuffer());
	*pWriteData = htonl(dwPktLength);

	if(_bNeedCheckSum)
	{
		const char* pBuffer = (const char*)pWriteData + 8;
		DWORD dwOpCode = *(pWriteData + 1);
		unsigned int uLen = dwPktLength - 8;
		WORD wCheckSum = DataEncryptor::GetCheckSum(dwOpCode, pBuffer, uLen);

		DWORD dwData = MAKELONG(dwOpCode, wCheckSum);
		*(pWriteData + 1) = dwData;
	}

	return g_xClientSocket2.SendToServer((const char*)_xBuf->GetBuffer(), _xBuf->GetLength());
}

unsigned int GetTextWidth(const char* _pText, size_t _nFontSize)
{
	size_t nLen = strlen(_pText);
	unsigned int nWidth = 0;

	for(size_t i = 0; i < nLen; ++i)
	{
		if(_pText[i] < 0)
		{
			//	The highest bit is '1', it is a chinese charactor
			nWidth += _nFontSize;
			++i;
		}
		else
		{
			nWidth += _nFontSize / 2;
		}
	}
	return nWidth;
}

int GetTextLine(const char* _pText, int _nFontSize, int _nLineWidth)
{
	char szTextBuf[MAX_PATH] = {0};
	int nTextBufLen = 0;
	int nWidth = 0;
	int nLines = 0;
	size_t nLen = strlen(_pText);

	for(size_t i = 0; i < nLen; ++i)
	{
		if(_pText[i] < 0)
		{
			nWidth += _nFontSize;
		}
		else
		{
			nWidth += _nFontSize / 2;
		}

		if(nWidth <= _nLineWidth)
		{
			//	还没到长度，缓存到文字缓存中
			szTextBuf[nTextBufLen++] = _pText[i];

			if(_pText[i] < 0)
			{
				szTextBuf[nTextBufLen++] = _pText[i + 1];
			}
		}
		else
		{
			++nLines;

			//	重置缓存
			ZeroMemory(szTextBuf, sizeof(szTextBuf));
			nTextBufLen = 0;
			nWidth = 0;

			//	再次进行一次该坐标字符的扫描
			--i;
			continue;
		}

		if(_pText[i] < 0)
		{
			//	中文 双字符 向后移一个字节
			++i;
		}
	}

	if(szTextBuf[0] != 0)
	{
		++nLines;
	}

	return nLines;
}

int SplitTextWithLength(const char* _pText, int _nFontSize ,int _nLineWidth, StringList& _refStringList)
{
	char szTextBuf[MAX_PATH] = {0};
	int nTextBufLen = 0;
	int nWidth = 0;
	int nLines = 0;
	size_t nLen = strlen(_pText);

	for(size_t i = 0; i < nLen; ++i)
	{
		if(_pText[i] < 0)
		{
			nWidth += _nFontSize;
		}
		else
		{
			nWidth += _nFontSize / 2;
		}

		if(nWidth <= _nLineWidth)
		{
			//	还没到长度，缓存到文字缓存中
			szTextBuf[nTextBufLen++] = _pText[i];
			
			if(_pText[i] < 0)
			{
				szTextBuf[nTextBufLen++] = _pText[i + 1];
			}
		}
		else
		{
			//	加入字符串列表中
			_refStringList.push_back(std::string(szTextBuf));
			++nLines;

			//	重置缓存
			ZeroMemory(szTextBuf, sizeof(szTextBuf));
			nTextBufLen = 0;
			nWidth = 0;

			//	再次进行一次该坐标字符的扫描
			--i;
			continue;
		}

		if(_pText[i] < 0)
		{
			//	中文 双字符 向后移一个字节
			++i;
		}
	}

	if(szTextBuf[0] != 0)
	{
		_refStringList.push_back(std::string(szTextBuf));
		++nLines;
	}

	return nLines;
}

void MirLog(const char* _pszLog)
{
	HGE* pHge = AfxGetHge();
	if(pHge)
	{
		pHge->System_Log(_pszLog);
	}
}

int GetDirection16(int _sx, int _sy, int _dx, int _dy)
{
	float fDeltaX = _dx - _sx;
	float fDeltaY = _dy - _sy;
	_sx = 0;
	_sy = 0;
	int nRet = 0;

	if(fDeltaX == 0)
	{
		if(fDeltaY < 0)
		{
			return 0;
		}
		else
		{
			return 8;
		}
	}

	if(fDeltaY == 0)
	{
		if(fDeltaX < 0)
		{
			return 12;
		}
		else
		{
			return 4;
		}
	}

	if(fDeltaX > 0 &&
		fDeltaY < 0)
	{
		nRet = 4;

		if(-fDeltaY > fDeltaX / 4)
		{
			nRet = 3;
		}
		if(-fDeltaY > fDeltaX / 1.9f)
		{
			nRet = 2;
		}
		if(-fDeltaY > fDeltaX * 1.4f)
		{
			nRet = 1;
		}
		if(-fDeltaY > fDeltaX * 4)
		{
			nRet = 0;
		}
	}
	else if(fDeltaX > 0 &&
		fDeltaY > 0)
	{
		nRet = 4;

		if(fDeltaY > fDeltaX / 4)
		{
			nRet = 5;
		}
		if(fDeltaY > fDeltaX / 1.9f)
		{
			nRet = 6;
		}
		if(fDeltaY > fDeltaX * 1.4f)
		{
			nRet = 7;
		}
		if(fDeltaY > fDeltaX * 4)
		{
			nRet = 8;
		}
	}
	else if(fDeltaX < 0 &&
		fDeltaY > 0)
	{
		nRet = 12;
		if(fDeltaY > (-fDeltaX / 4))
		{
			nRet = 11;
		}
		if(fDeltaY > (-fDeltaX / 1.9f))
		{
			nRet = 10;
		}
		if(fDeltaY > (-fDeltaX * 1.4f))
		{
			nRet = 9;
		}
		if(fDeltaY > (-fDeltaX * 4))
		{
			nRet = 8;
		}
	}
	else if(fDeltaX < 0 &&
		fDeltaY < 0)
	{
		nRet = 12;
		if((-fDeltaY) > (-fDeltaX / 4))
		{
			nRet = 13;
		}
		if((-fDeltaY) > (-fDeltaX / 1.9f))
		{
			nRet = 14;
		}
		if((-fDeltaY) > (-fDeltaX * 1.4f))
		{
			nRet = 15;
		}
		if((-fDeltaY) > (-fDeltaX * 4))
		{
			nRet = 0;
		}
	}

	return nRet;
}

int GetDirection8(int _sx, int _sy, int _dx, int _dy)
{
	int nDrt = 0;

	while(1)
	{
		if(_sx < _dx)
		{
			nDrt = PDT_RIGHT;

			if(_sy < _dy)
			{
				nDrt = PDT_RIGHTDOWN;
				break;
			}

			if(_sy > _dy)
			{
				nDrt = PDT_RIGHTUP;
				break;
			}
			break;
		}
		else if(_sx > _dx)
		{
			nDrt = PDT_LEFT;

			if(_sy < _dy)
			{
				nDrt = PDT_LEFTDOWN;
				break;
			}

			if(_sy > _dy)
			{
				nDrt = PDT_LEFTUP;
				break;
			}
			break;
		}

		if(_sy > _dy)
		{
			nDrt = PDT_UP;
			break;
		}

		if(_sy < _dy)
		{
			nDrt = PDT_DOWN;
			break;
		}
		break;
	}

	return nDrt;
}

int GetNumberBit(int _number)
{
	int nMaxBit = 8;

	for(int i = 1; i <= nMaxBit; ++i)
	{
		if(_number < pow((double)10, i))
		{
			return i;
		}
	}
	return 0;
}

void ClearRectCache()
{
	if(!g_xRectCache.empty())
	{
		Int2RectMap* pData = NULL;

		ObjRectCache::iterator begIter = g_xRectCache.begin();
		ObjRectCache::const_iterator endIter = g_xRectCache.end();

		for(begIter;
			begIter != endIter;
			++begIter)
		{
			pData = begIter->second;
			SAFE_DELETE(pData);
		}

		g_xRectCache.clear();
	}
}

std::string UTF8ToGBK(const std::string& strUTF8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, NULL, 0);
	unsigned short * wszGBK = new unsigned short[len + 1]; memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUTF8.c_str(), -1, (LPWSTR)wszGBK, len);

	len = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wszGBK, -1, NULL, 0, NULL, NULL);
	char *szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wszGBK, -1, szGBK, len, NULL, NULL); //strUTF8 = szGBK; 
	std::string strTemp(szGBK);
	delete[]szGBK;
	delete[]wszGBK;
	return strTemp;
}