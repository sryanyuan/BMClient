#ifndef _INC_SIBFILE_
#define _INC_SIBFILE_
//////////////////////////////////////////////////////////////////////////
#include "SIBDefine.h"
//////////////////////////////////////////////////////////////////////////
class SIBFile
{
public:
	SIBFile();
	virtual ~SIBFile();

public:
	bool Load(const char* _pszFile);
	void UnLoad();

	HBITMAP GetBitmap(HDC _dc, int _index);
	const SIBImgInfo* GetImageInfo(int _index);
	const SIBPackHeader* GetImageHeader();

	int Extract(const char* _pszPath);

protected:
	HBITMAP GetBitmap8(HDC _dc, int _index);
	HBITMAP GetBitmap16(HDC _dc, int _index);
	HBITMAP CreateDIBitmapFromData(HDC hDevice, PBITMAPINFO lpBitmapInfo, BYTE* pData);

	int ExtraBitmap8(const char* _pszDestFile, int _index);
	int ExtraBitmap16(const char* _pszDestFile, int _index);

protected:
	SIBPackHeader m_stHeader;
	SIBImgInfo* m_pInfo;
	DWORD* m_pPalette;
	HANDLE m_hFile;
};
//////////////////////////////////////////////////////////////////////////
#endif