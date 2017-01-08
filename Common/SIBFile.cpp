#include "SIBFile.h"
#include <Shlwapi.h>
#include <direct.h>
//////////////////////////////////////////////////////////////////////////
#define _CONSOLE_PRINT_
//////////////////////////////////////////////////////////////////////////
SIBFile::SIBFile()
{
	memset(&m_stHeader, 0, sizeof(SIBPackHeader));
	m_pInfo = NULL;
	m_pPalette = NULL;
	m_hFile = INVALID_HANDLE_VALUE;
}

SIBFile::~SIBFile()
{
	UnLoad();
}
//////////////////////////////////////////////////////////////////////////
const SIBPackHeader* SIBFile::GetImageHeader()
{
	return &m_stHeader;
}
//////////////////////////////////////////////////////////////////////////
const SIBImgInfo* SIBFile::GetImageInfo(int _index)
{
	if(_index < 0 ||
		_index >= m_stHeader.uImgCount)
	{
		return NULL;
	}
	return &m_pInfo[_index];
}
//////////////////////////////////////////////////////////////////////////
void SIBFile::UnLoad()
{
	delete[] m_pInfo;
	m_pInfo = NULL;
	delete[] m_pPalette;
	m_pPalette = NULL;
	if(INVALID_HANDLE_VALUE != m_hFile)
	{
		CloseHandle(m_hFile);
		m_hFile = INVALID_HANDLE_VALUE;
	}
}
//////////////////////////////////////////////////////////////////////////
bool SIBFile::Load(const char* _pszFile)
{
	if(m_pInfo != NULL)
	{
		UnLoad();
	}
	bool bRet = true;

	m_hFile = ::CreateFile(_pszFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
	if(m_hFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	//	Read the header
	char szBuf[1024];
	DWORD dwRead = 0;

	ReadFile(m_hFile, &m_stHeader.szInfo, sizeof(m_stHeader.szInfo), &dwRead, NULL);
	ReadFile(m_hFile, &m_stHeader.uImgCount, sizeof(m_stHeader.uImgCount), &dwRead, NULL);
	ReadFile(m_hFile, &m_stHeader.uPalette, sizeof(m_stHeader.uPalette), &dwRead, NULL);

	if(m_stHeader.uPalette != 3 &&
		m_stHeader.uPalette != 256 &&
		m_stHeader.uPalette != 259)
	{
		bRet = false;
		goto lbl_closefile_ret;
	}
	if(m_stHeader.uImgCount == 0)
	{
		bRet = false;
		goto lbl_closefile_ret;
	}
	m_pInfo = new SIBImgInfo[m_stHeader.uImgCount];

	//	Read the palette
	if(m_stHeader.uPalette != 0)
	{
		m_pPalette = new DWORD[m_stHeader.uPalette];
		ReadFile(m_hFile, m_pPalette, m_stHeader.uPalette * sizeof(DWORD), &dwRead, NULL);
	}

	memset(m_pInfo, 0, sizeof(SIBImgInfo) * m_stHeader.uImgCount);
	for(int i = 0; i < m_stHeader.uImgCount; ++i)
	{
		ReadFile(m_hFile, &m_pInfo[i].uImgPointer, sizeof(m_pInfo[i].uImgPointer), &dwRead, NULL);
		if((m_pInfo[i].uImgPointer & (~BITMAP_16BITMASK)) == 0)
		{
			//	No image info
			continue;
		}

		ReadFile(m_hFile, &m_pInfo[i].uImgWidth, sizeof(m_pInfo[i].uImgWidth), &dwRead, NULL);
		ReadFile(m_hFile, &m_pInfo[i].uImgHeight, sizeof(m_pInfo[i].uImgHeight), &dwRead, NULL);
		ReadFile(m_hFile, &m_pInfo[i].sImgOftX, sizeof(m_pInfo[i].sImgOftX), &dwRead, NULL);
		ReadFile(m_hFile, &m_pInfo[i].sImgOftY, sizeof(m_pInfo[i].sImgOftY), &dwRead, NULL);
	}

#ifdef _CONSOLE_PRINT_
	for(int i = 0; i < m_stHeader.uImgCount; ++i)
	{
		printf("%d : Width %d Height %d OffsetX %d OffsetY %d\n",
			i, m_pInfo[i].uImgWidth, m_pInfo[i].uImgHeight, m_pInfo[i].sImgOftX, m_pInfo[i].sImgOftY);
	}
	printf("Palette %d\n", m_stHeader.uPalette);
#endif

	return bRet;

lbl_closefile_ret:
	CloseHandle(m_hFile);
	m_hFile = INVALID_HANDLE_VALUE;
	return false;

lbl_closefile_deleteinfo_ret:
	CloseHandle(m_hFile);
	m_hFile = INVALID_HANDLE_VALUE;
	delete[] m_pInfo;
	m_pInfo = NULL;
}
//////////////////////////////////////////////////////////////////////////
int SIBFile::Extract(const char* _pszPath)
{
	int nRet = PACK_OK;
	if(m_pInfo == NULL ||
		m_pPalette == NULL)
	{
		return PACK_NODATA;
	}

	if(!PathFileExists(_pszPath))
	{
		mkdir(_pszPath);
	}

	char szOutFile[MAX_PATH];
	for(int i = 0; i < m_stHeader.uImgCount; ++i)
	{
		if(m_pInfo[i].uImgWidth == 0)
		{
			continue;
		}
		sprintf(szOutFile, "%s\\%06d.bmp",
			_pszPath, i);
		if(m_pInfo[i].uImgPointer & BITMAP_16BITMASK)
		{
			ExtraBitmap16(szOutFile, i);
		}
		else
		{
			ExtraBitmap8(szOutFile, i);
		}
	}
	return nRet;
}
//////////////////////////////////////////////////////////////////////////
int SIBFile::ExtraBitmap8(const char* _pszDestFile, int _index)
{
	int nRet = PACK_OK;
	HANDLE hDestFile = INVALID_HANDLE_VALUE;
	hDestFile = ::CreateFile(_pszDestFile, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, NULL, NULL);
	if(hDestFile == INVALID_HANDLE_VALUE)
	{
		return PACK_OUTFILE;
	}

	DWORD dwImgSize = (m_pInfo[_index].uImgWidth * 8 + 31) / 32 * 4 * m_pInfo[_index].uImgHeight;
	BITMAPINFOHEADER stBitmapHeader;
	memset(&stBitmapHeader, 0, sizeof(BITMAPINFOHEADER));
	stBitmapHeader.biSize = sizeof(BITMAPINFOHEADER);
	stBitmapHeader.biWidth = m_pInfo[_index].uImgWidth;
	stBitmapHeader.biHeight = m_pInfo[_index].uImgHeight;
	stBitmapHeader.biPlanes = 1;
	stBitmapHeader.biBitCount = 8;
	stBitmapHeader.biCompression = BI_RGB;
	stBitmapHeader.biSizeImage = dwImgSize;
	stBitmapHeader.biClrImportant = 0;
	stBitmapHeader.biClrUsed = 256;

	DWORD dwWrite = 0;
	WriteFile(hDestFile, "BM", 2, &dwWrite, NULL);
	DWORD dwTotalSize = 14 + 40 + 256*4 + dwImgSize;
	//	Write the total size
	WriteFile(hDestFile, &dwTotalSize, sizeof(DWORD), &dwWrite, NULL);
	//	Write the reserver
	dwTotalSize = 0;
	WriteFile(hDestFile, &dwTotalSize, sizeof(DWORD), &dwWrite, NULL);
	//	Write the offset
	dwTotalSize = 14 + 40 + 256*4;
	WriteFile(hDestFile, &dwTotalSize, sizeof(DWORD), &dwWrite, NULL);
	//	Write the header
	WriteFile(hDestFile, &stBitmapHeader, sizeof(stBitmapHeader), &dwWrite, NULL);
	//	Write the palette
	WriteFile(hDestFile, m_pPalette, 256 * 4, &dwWrite, NULL);

	//	Get the filedata
	char szBuf[1024];
	//DWORD dwImgSize = m_pInfo[_index].uImgWidth * m_pInfo[_index].uImgHeight * 1;
	DWORD dwCanRead = 0;
	DWORD dwRead = 0;

	while(dwImgSize > 0)
	{
		dwCanRead = (dwImgSize > sizeof(szBuf) ? sizeof(szBuf) : dwImgSize);
		ReadFile(m_hFile, szBuf, dwCanRead, &dwRead, NULL);
		WriteFile(hDestFile, szBuf, dwRead, &dwWrite, NULL);
		dwImgSize -= dwRead;
	}

#ifdef _CONSOLE_PRINT_
	printf("Extract %s\n",
		_pszDestFile);
#endif
	CloseHandle(hDestFile);

	//	Write offset
	char szPath[MAX_PATH];
	strcpy(szPath, _pszDestFile);
	PathRemoveFileSpec(szPath);
	strcat(szPath, "\\Placements");
	if(!PathFileExists(szPath))
	{
		mkdir(szPath);
	}
	char szName[20];
	sprintf(szName, "\\%06d.txt",
		_index);
	//sprintf(szBuf, "%s%s",
	//szPath, szName);
	strcat(szPath, szName);
	char szOft[10];
	short sToken = 0x0A0D;
	hDestFile = ::CreateFile(szPath, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, NULL, NULL);
	if(hDestFile != INVALID_HANDLE_VALUE)
	{
		sprintf(szOft, "%d", m_pInfo[_index].sImgOftX);
		WriteFile(hDestFile, szOft, strlen(szOft), &dwWrite, NULL);
		WriteFile(hDestFile, &sToken, sizeof(short), &dwWrite, NULL);
		sprintf(szOft, "%d", m_pInfo[_index].sImgOftY);
		WriteFile(hDestFile, szOft, strlen(szOft), &dwWrite, NULL);
		WriteFile(hDestFile, &sToken, sizeof(short), &dwWrite, NULL);
	}
	CloseHandle(hDestFile);

	return nRet;
}
//////////////////////////////////////////////////////////////////////////
int SIBFile::ExtraBitmap16(const char* _pszDestFile, int _index)
{
	int nRet = PACK_OK;
	HANDLE hDestFile = INVALID_HANDLE_VALUE;
	hDestFile = ::CreateFile(_pszDestFile, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, NULL, NULL);
	if(hDestFile == INVALID_HANDLE_VALUE)
	{
		return PACK_OUTFILE;
	}

	DWORD dwImgSize = (m_pInfo[_index].uImgWidth * 16 + 31) / 32 * 4 * m_pInfo[_index].uImgHeight;
	BITMAPINFOHEADER stBitmapHeader;
	memset(&stBitmapHeader, 0, sizeof(BITMAPINFOHEADER));
	stBitmapHeader.biSize = sizeof(BITMAPINFOHEADER);
	stBitmapHeader.biWidth = m_pInfo[_index].uImgWidth;
	stBitmapHeader.biHeight = m_pInfo[_index].uImgHeight;
	stBitmapHeader.biPlanes = 1;
	stBitmapHeader.biBitCount = 16;
	stBitmapHeader.biCompression = BI_BITFIELDS;
	stBitmapHeader.biSizeImage = dwImgSize;
	stBitmapHeader.biClrImportant = 0;
	stBitmapHeader.biClrUsed = 0;

	//	Write the header
	DWORD dwWrite = 0;
	WriteFile(hDestFile, "BM", 2, &dwWrite, NULL);
	DWORD dwTotalSize = 14 + 40 + 3*4 + dwImgSize;
	//	Write the total size
	WriteFile(hDestFile, &dwTotalSize, sizeof(DWORD), &dwWrite, NULL);
	//	Write the reserve
	dwTotalSize = 0;
	WriteFile(hDestFile, &dwTotalSize, sizeof(DWORD), &dwWrite, NULL);
	//	Write the offset
	dwTotalSize = 14 + 40 + 3*4;
	WriteFile(hDestFile, &dwTotalSize, sizeof(DWORD), &dwWrite, NULL);
	//	Write the header
	WriteFile(hDestFile, &stBitmapHeader, sizeof(stBitmapHeader), &dwWrite, NULL);
	//	Write the palette
	WriteFile(hDestFile, &m_pPalette[m_stHeader.uPalette - 3], 3 * 4, &dwWrite, NULL);

	//	Get the filedata
	char szBuf[1024];
	//DWORD dwImgSize = m_pInfo[_index].uImgWidth * m_pInfo[_index].uImgHeight * 2;
	DWORD dwCanRead = 0;
	DWORD dwRead = 0;

	while(dwImgSize > 0)
	{
		dwCanRead = (dwImgSize > sizeof(szBuf) ? sizeof(szBuf) : dwImgSize);
		ReadFile(m_hFile, szBuf, dwCanRead, &dwRead, NULL);
		WriteFile(hDestFile, szBuf, dwRead, &dwWrite, NULL);
		dwImgSize -= dwRead;
	}

#ifdef _CONSOLE_PRINT_
	printf("Extract %s\n",
		_pszDestFile);
#endif

	CloseHandle(hDestFile);

	//	Write offset
	char szPath[MAX_PATH];
	strcpy(szPath, _pszDestFile);
	PathRemoveFileSpec(szPath);
	strcat(szPath, "\\Placements");
	if(!PathFileExists(szPath))
	{
		mkdir(szPath);
	}
	char szName[20];
	sprintf(szName, "\\%06d.txt",
		_index);
	//sprintf(szBuf, "%s%s",
	//szPath, szName);
	strcat(szPath, szName);
	char szOft[10];
	short sToken = 0x0A0D;
	hDestFile = ::CreateFile(szPath, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, NULL, NULL);
	if(hDestFile != INVALID_HANDLE_VALUE)
	{
		sprintf(szOft, "%d", m_pInfo[_index].sImgOftX);
		WriteFile(hDestFile, szOft, strlen(szOft), &dwWrite, NULL);
		WriteFile(hDestFile, &sToken, sizeof(short), &dwWrite, NULL);
		sprintf(szOft, "%d", m_pInfo[_index].sImgOftY);
		WriteFile(hDestFile, szOft, strlen(szOft), &dwWrite, NULL);
		WriteFile(hDestFile, &sToken, sizeof(short), &dwWrite, NULL);
	}
	CloseHandle(hDestFile);

	return nRet;
}
//////////////////////////////////////////////////////////////////////////
HBITMAP SIBFile::GetBitmap(HDC _dc, int _index)
{
	if(_index < 0 ||
		_index >= m_stHeader.uImgCount)
	{
		return 0;
	}
	if(m_pInfo == NULL)
	{
		return 0;
	}
	if(m_hFile == INVALID_HANDLE_VALUE)
	{
		return 0;
	}

	DWORD dwFilePointer = m_pInfo[_index].uImgPointer;
	if(0 == dwFilePointer)
	{
		return 0;
	}
	if(dwFilePointer & BITMAP_16BITMASK)
	{
		return GetBitmap16(_dc, _index);
	}
	else
	{
		return GetBitmap8(_dc, _index);
	}
	return 0;
}
//////////////////////////////////////////////////////////////////////////
HBITMAP SIBFile::GetBitmap8(HDC _dc, int _index)
{
	DWORD dwFilePointer = (m_pInfo[_index].uImgPointer & (~BITMAP_16BITMASK));
	/*DWORD dwImgSize = m_pInfo[_index].uImgWidth * m_pInfo[_index].uImgHeight * 1;*/
	DWORD dwImgSize = (m_pInfo[_index].uImgWidth * 8 + 31) / 32 * 4 * m_pInfo[_index].uImgHeight;
	DWORD dwFileSize = GetFileSize(m_hFile, NULL);
	DWORD dwCanRead = 0;
	DWORD dwRead = 0;

	if(dwFilePointer + dwImgSize > dwFileSize)
	{
		return 0;
	}
	if(m_stHeader.uPalette < 256)
	{
		return 0;
	}

	char* pImageData = new char[dwImgSize];

	SetFilePointer(m_hFile, dwFilePointer, NULL, FILE_BEGIN);
	ReadFile(m_hFile, pImageData, dwImgSize, &dwRead, NULL);

	//	柔性数组
	size_t nHeaderSize = sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD);
	PBITMAPINFO bi;
	bi = (PBITMAPINFO)malloc(nHeaderSize);
	memset(bi, 0, nHeaderSize);
	bi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bi->bmiHeader.biWidth = m_pInfo[_index].uImgWidth;
	bi->bmiHeader.biHeight = m_pInfo[_index].uImgHeight;
	bi->bmiHeader.biPlanes = 1;
	bi->bmiHeader.biBitCount = 8;
	bi->bmiHeader.biCompression = BI_RGB;
	bi->bmiHeader.biSizeImage = dwImgSize;
	bi->bmiHeader.biClrImportant = 0;
	bi->bmiHeader.biClrUsed = 256;

	//	填充调色板
	RGBQUAD quad;
	BYTE* pPaletteData = (BYTE*)m_pPalette;
	
	for(int i = 0; i < 256; ++i)
	{
		quad.rgbRed = pPaletteData[i * 4 + 2];
		quad.rgbGreen = pPaletteData[i * 4 + 1];
		quad.rgbBlue = pPaletteData[i * 4 + 0];
		quad.rgbReserved = pPaletteData[i * 4 + 3];
		bi->bmiColors[i] = quad;
	}

	HBITMAP hOut = CreateDIBitmapFromData(_dc, bi, (BYTE*)pImageData);
	if(hOut == 0)
	{

	}

	free(bi);
	delete [] pImageData;
	return hOut;
}
//////////////////////////////////////////////////////////////////////////
HBITMAP SIBFile::GetBitmap16(HDC _dc, int _index)
{
	DWORD dwFilePointer = (m_pInfo[_index].uImgPointer & (~BITMAP_16BITMASK));
	/*DWORD dwImgSize = m_pInfo[_index].uImgWidth * m_pInfo[_index].uImgHeight * 2;*/
	DWORD dwImgSize = (m_pInfo[_index].uImgWidth * 16 + 31) / 32 * 4 * m_pInfo[_index].uImgHeight;
	DWORD dwFileSize = GetFileSize(m_hFile, NULL);
	DWORD dwCanRead = 0;
	DWORD dwRead = 0;

	if(dwFilePointer + dwImgSize > dwFileSize)
	{
		return 0;
	}
	if(m_stHeader.uPalette < 3)
	{
		return 0;
	}
	char* pImageData = new char[dwImgSize];

	SetFilePointer(m_hFile, dwFilePointer, NULL, FILE_BEGIN);
	ReadFile(m_hFile, pImageData, dwImgSize, &dwRead, NULL);

	//	柔性数组
	size_t nHeaderSize = sizeof(BITMAPINFOHEADER) + 3 * sizeof(RGBQUAD);
	PBITMAPINFO bi;
	bi = (PBITMAPINFO)malloc(nHeaderSize);
	memset(bi, 0, nHeaderSize);
	bi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bi->bmiHeader.biWidth = m_pInfo[_index].uImgWidth;
	bi->bmiHeader.biHeight = m_pInfo[_index].uImgHeight;
	bi->bmiHeader.biPlanes = 1;
	bi->bmiHeader.biBitCount = 16;
	bi->bmiHeader.biCompression = BI_BITFIELDS;
	bi->bmiHeader.biSizeImage = dwImgSize;
	bi->bmiHeader.biClrImportant = 0;
	bi->bmiHeader.biClrUsed = 0;

	//	填充调色板
	*(DWORD*)(&bi->bmiColors[0]) = /*0xF800;*/m_pPalette[m_stHeader.uPalette - 3];
	*(DWORD*)(&bi->bmiColors[1]) = /*0x07E0;*/m_pPalette[m_stHeader.uPalette - 2];
	*(DWORD*)(&bi->bmiColors[2]) = /*0x001F;*/m_pPalette[m_stHeader.uPalette - 1];

	HBITMAP hOut = ::CreateDIBitmap(_dc, (PBITMAPINFOHEADER)bi, CBM_INIT, pImageData, bi, DIB_RGB_COLORS);
	DWORD err = GetLastError();

	free(bi);
	delete [] pImageData;
	return hOut;
}
//////////////////////////////////////////////////////////////////////////
HBITMAP SIBFile::CreateDIBitmapFromData(HDC hDevice, PBITMAPINFO lpBitmapInfo, BYTE* pData)
{
	PLOGPALETTE lpPalette = NULL;
	int nColors = lpBitmapInfo->bmiHeader.biClrUsed > 0 ? lpBitmapInfo->bmiHeader.biClrUsed : 1 << lpBitmapInfo->bmiHeader.biBitCount;

	//	为逻辑调色板分配内存
	lpPalette = (PLOGPALETTE)malloc(sizeof(LOGPALETTE) + nColors * sizeof(PALETTEENTRY));
	lpPalette->palNumEntries = nColors;
	lpPalette->palVersion = 0x300;
	for(int i = 0; i < nColors; ++i)
	{
		lpPalette->palPalEntry[i].peBlue = lpBitmapInfo->bmiColors[i].rgbBlue;
		lpPalette->palPalEntry[i].peGreen = lpBitmapInfo->bmiColors[i].rgbGreen;
		lpPalette->palPalEntry[i].peRed = lpBitmapInfo->bmiColors[i].rgbRed;
		lpPalette->palPalEntry[i].peFlags = 0;
	}

	HPALETTE hPalette= ::CreatePalette(lpPalette);
	HPALETTE hOld = 0;
	free(lpPalette);

	hOld = ::SelectPalette(hDevice, hPalette, FALSE);
	if(GDI_ERROR == ::RealizePalette(hDevice))
	{
		return 0;
	}

	HBITMAP hOutBitmap = ::CreateDIBitmap(hDevice, (PBITMAPINFOHEADER)lpBitmapInfo, CBM_INIT, pData, lpBitmapInfo, DIB_RGB_COLORS);
	::SelectPalette(hDevice, hOld, FALSE);
	::DeleteObject(hPalette);

	if(hOutBitmap == 0)
	{
		char msgbuf[200];
		sprintf(msgbuf, "%d", ::GetLastError());
		msgbuf[199] = 0;
	}

	return hOutBitmap;
}