#include "SPBFile.h"
#include <Shlwapi.h>
//////////////////////////////////////////////////////////////////////////
#pragma comment(lib, "shlwapi.lib")
//////////////////////////////////////////////////////////////////////////
SPBFile::SPBFile()
{
	ZeroMemory(&m_stHeader, sizeof(m_stHeader));
	//m_pImgOffsetArray = NULL;
	//m_nImgCount = 0;
	m_pImgInfoArray = NULL;
	m_hFile = INVALID_HANDLE_VALUE;
}

SPBFile::~SPBFile()
{
	Unload();
}

void SPBFile::Unload()
{
	if(m_hFile)
	{
		CloseHandle(m_hFile);
		m_hFile = INVALID_HANDLE_VALUE;
	}

	if(m_pImgInfoArray)
	{
		delete[] m_pImgInfoArray;
		m_pImgInfoArray = NULL;
	}

	ZeroMemory(&m_stHeader, sizeof(m_stHeader));

	/*if(m_pImgOffsetArray)
	{
		delete[] m_pImgOffsetArray;
		m_pImgOffsetArray = NULL;
	}*/

	//m_nImgCount = 0;
}

bool SPBFile::Load(const char* _pszFile)
{
	if(INVALID_HANDLE_VALUE != m_hFile)
	{
		return true;
	}

	if(!PathFileExists(_pszFile))
	{
		return false;
	}

	m_hFile = ::CreateFile(_pszFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
	if(m_hFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	DWORD dwRead = 0;
	bool bReadOk = true;

	//	read file header
	ReadFile(m_hFile, m_stHeader.szFileDesc, sizeof(m_stHeader.szFileDesc), &dwRead, NULL);
	ReadFile(m_hFile, &m_stHeader.nImgCount, sizeof(m_stHeader.nImgCount), &dwRead, NULL);

	do 
	{
		if(m_stHeader.nImgCount == 0)
		{
			break;
		}

		/*m_pImgOffsetArray = new UINT[m_stHeader.nImgCount];
		ZeroMemory(m_pImgOffsetArray, m_stHeader.nImgCount * sizeof(UINT));*/

		m_pImgInfoArray = new SPBFileInfo[m_stHeader.nImgCount];
		ZeroMemory(m_pImgInfoArray, m_stHeader.nImgCount * sizeof(SPBFileInfo));

		//	read file info
		/*ReadFile(m_hFile, m_pImgOffsetArray, sizeof(UINT) * m_stHeader.nImgCount, &dwRead, NULL);
		if(dwRead != sizeof(UINT) * m_stHeader.nImgCount)
		{
			bReadOk = false;
			break;
		}*/

		for(int i = 0; i < GetImgCount(); ++i)
		{
			ReadFile(m_hFile, &m_pImgInfoArray[i].dwOffset, sizeof(DWORD), &dwRead, NULL);
			ReadFile(m_hFile, &m_pImgInfoArray[i].uSize, sizeof(UINT), &dwRead, NULL);
			ReadFile(m_hFile, &m_pImgInfoArray[i].nWidth, sizeof(int), &dwRead, NULL);
			ReadFile(m_hFile, &m_pImgInfoArray[i].nHeight, sizeof(int), &dwRead, NULL);
			ReadFile(m_hFile, &m_pImgInfoArray[i].sOftX, sizeof(short), &dwRead, NULL);
			ReadFile(m_hFile, &m_pImgInfoArray[i].sOftY, sizeof(short), &dwRead, NULL);
		}
	} while (0);

	if(!bReadOk)
	{
		CloseHandle(m_hFile);
		m_hFile = INVALID_HANDLE_VALUE;

		delete[] m_pImgInfoArray;
		m_pImgInfoArray = NULL;

		/*delete[] m_pImgOffsetArray;
		m_pImgOffsetArray = NULL;*/
	}

	return bReadOk;
}

int SPBFile::GetImgCount()
{
	return m_stHeader.nImgCount;
}

const char* SPBFile::GetImgData(int _nIndex, DWORD* _pSize)
{
	if(_nIndex < 0 ||
		_nIndex >= GetImgCount())
	{
		return NULL;
	}

	/*if(NULL == m_pImgOffsetArray)
	{
		return NULL;
	}*/

	const SPBFileInfo* pInfo = GetImgInfo(_nIndex);
	if(NULL == pInfo)
	{
		return NULL;
	}

	if(pInfo->uSize == 0 ||
		pInfo->dwOffset == 0)
	{
		return NULL;
	}

	//DWORD dwFileOffset = (DWORD)m_pImgOffsetArray[_nIndex];
	DWORD dwFileOffset = pInfo->dwOffset;
	char* pFileBuffer = new char[pInfo->uSize];

	DWORD dwRead = 0;
	SetFilePointer(m_hFile, dwFileOffset, NULL, FILE_BEGIN);
	ReadFile(m_hFile, pFileBuffer, pInfo->uSize, &dwRead, NULL);

	if(dwRead != pInfo->uSize)
	{
		delete[] pFileBuffer;
		pFileBuffer = NULL;
		return NULL;
	}

	if(NULL != _pSize)
	{
		*_pSize = pInfo->uSize;
	}
	
	return pFileBuffer;
}

const SPBFileInfo* SPBFile::GetImgInfo(int _nIndex)
{
	if(_nIndex < 0 ||
		_nIndex >= GetImgCount())
	{
		return NULL;
	}

	if(NULL == m_pImgInfoArray)
	{
		return NULL;
	}

	return &m_pImgInfoArray[_nIndex];
}


//////////////////////////////////////////////////////////////////////////
typedef unsigned short int uint16;  
typedef unsigned long int uint32;  
 
#define BigLittleSwap16(A)  ((((uint16)(A) & 0xff00) >> 8) | \
	(((uint16)(A) & 0x00ff) << 8))  

#define BigLittleSwap32(A)  ((((uint32)(A) & 0xff000000) >> 24) | \
	(((uint32)(A) & 0x00ff0000) >> 8) | \
	(((uint32)(A) & 0x0000ff00) << 8) | \
	(((uint32)(A) & 0x000000ff) << 24))

int checkCPUendian()  
{  
	union{  
		unsigned long int i;  
		unsigned char s[4];  
	}c;  

	c.i = 0x12345678;  
	return (0x12 == c.s[0]);  
}  

// 模拟htonl函数，本机字节序转网络字节序  
unsigned long int t_htonl(unsigned long int h)  
{  
	// 若本机为大端，与网络字节序同，直接返回  
	// 若本机为小端，转换成大端再返回  
	return checkCPUendian() ? h : BigLittleSwap32(h);  
}  

// 模拟ntohl函数，网络字节序转本机字节序  
unsigned long int t_ntohl(unsigned long int n)  
{  
	// 若本机为大端，与网络字节序同，直接返回  
	// 若本机为小端，网络数据转换成小端再返回  
	return checkCPUendian() ? n : BigLittleSwap32(n);  
} 
//////////////////////////////////////////////////////////////////////////
#include <string>
#include <vector>
#include <algorithm>
#include <io.h>
using std::string;
typedef std::vector<string> FILENAMES;
typedef std::vector<int> FILENUMBERS;
//////////////////////////////////////////////////////////////////////////
int GetDirectoryFiles(const char* _pszPath, const char* _pszExt, FILENAMES& _xFiles)
{
	if(0 != _access(_pszPath, 0))
	{
		return 1;
	}

	int nRet = 0;
	WIN32_FIND_DATA stFindData;
	HANDLE hFile = INVALID_HANDLE_VALUE;
	char szPath[MAX_PATH];
	DWORD dwLastError = 0;

	FILENAMES& xFileNames = _xFiles;

	sprintf(szPath, "%s\\*.%s",
		_pszPath, _pszExt);
	hFile = FindFirstFile(szPath, &stFindData);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		nRet = 2;
	}
	else
	{
		if(stFindData.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
		{
			xFileNames.push_back(stFindData.cFileName);
		}
	}

	while(FindNextFile(hFile, &stFindData))
	{
		if(stFindData.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
		{
			xFileNames.push_back(stFindData.cFileName);
		}
	}

	FindClose(hFile);

	return nRet;
}

int MakeNumber(FILENAMES& _xNames, FILENUMBERS& _xNumbers)
{
	size_t nSize = _xNames.size();
	if(nSize == 0)
	{
		return 0;
	}
	int nMaxNumber = 0;
	int nGetNumber = 0;

	for(size_t i = 0; i < nSize; ++i)
	{
		nGetNumber = atoi(_xNames[i].c_str());
		_xNumbers.push_back(nGetNumber);
		if(nGetNumber > nMaxNumber)
		{
			nMaxNumber = nGetNumber;
		}
	}

	return nMaxNumber;
}

int SPBFile::Pack(const char* _pszInputDirectory, const char* _pszOutFile)
{
	int nRet = 0;

	//	scan valid files
	FILENAMES xFileNames;
	nRet = GetDirectoryFiles(_pszInputDirectory, "png", xFileNames);
	if(0 != nRet)
	{
		return -3;
	}
	FILENUMBERS xFileNumbers;
	int nMaxNumber = MakeNumber(xFileNames, xFileNumbers);

	++nMaxNumber;

	if(!PathFileExists(_pszInputDirectory))
	{
		return -1;
	}

	HANDLE hFile = ::CreateFile(_pszOutFile, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, NULL, NULL);
	if(INVALID_HANDLE_VALUE == hFile)
	{
		return -2;
	}

	SPBFileHeader header;
	ZeroMemory(&header, sizeof(header));
	strcpy(header.szFileDesc, SPBFILEDESC);
	header.nImgCount = nMaxNumber;

	/*UINT* pImgOffsetArray = new UINT[nMaxNumber];
	ZeroMemory(pImgOffsetArray, sizeof(UINT) * nMaxNumber);*/
	SPBFileInfo* pImgInfoArray = new SPBFileInfo[nMaxNumber];
	ZeroMemory(pImgInfoArray, sizeof(SPBFileInfo) * nMaxNumber);

	DWORD dwFileDataBegIndex = sizeof(header.nImgCount) + sizeof(header.szFileDesc) /*+ sizeof(UINT) * nMaxNumber*/ + /*sizeof(SPBFileInfo)*/20 * nMaxNumber;
	DWORD dwPrevPngOft = 0;
	DWORD dwWrite = 0;
	DWORD dwRead = 0;

	FILENUMBERS::const_iterator fndIter = xFileNumbers.end();

	//	make header info
	for(int i = 0; i < nMaxNumber; ++i)
	{
		fndIter = std::find(xFileNumbers.begin(), xFileNumbers.end(), i);

		if(fndIter != xFileNumbers.end())
		{
			int nFileNumber = *fndIter;

			char szFileName[MAX_PATH];
			sprintf(szFileName, "%s/%06d.png",
				_pszInputDirectory, nFileNumber);

			if(PathFileExists(szFileName))
			{
				HANDLE hPngFile = ::CreateFile(szFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
				if(hPngFile != INVALID_HANDLE_VALUE)
				{
					pImgInfoArray[i].uSize = GetFileSize(hPngFile, NULL);

					SetFilePointer(hPngFile, 16, NULL, FILE_BEGIN);
					DWORD dwWidth = 0;
					DWORD dwHeight = 0;
					
					ReadFile(hPngFile, &dwWidth, sizeof(DWORD), &dwRead, NULL);
					ReadFile(hPngFile, &dwHeight, sizeof(DWORD), &dwRead, NULL);

					dwWidth = t_ntohl(dwWidth);
					dwHeight = t_ntohl(dwHeight);
					pImgInfoArray[i].nWidth = dwWidth;
					pImgInfoArray[i].nHeight = dwHeight;
					pImgInfoArray[i].dwOffset = dwFileDataBegIndex + dwPrevPngOft;
					dwPrevPngOft += pImgInfoArray[i].uSize;

					CloseHandle(hPngFile);
					hPngFile = INVALID_HANDLE_VALUE;
				}
			}
		}
	}

	//	write file header
	WriteFile(hFile, header.szFileDesc, sizeof(header.szFileDesc), &dwWrite, NULL);
	WriteFile(hFile, &header.nImgCount, sizeof(header.nImgCount), &dwWrite, NULL);

	//	write file info
	for(int i = 0; i < nMaxNumber; ++i)
	{
		WriteFile(hFile, &pImgInfoArray[i].dwOffset, sizeof(DWORD), &dwWrite, NULL);
		WriteFile(hFile, &pImgInfoArray[i].uSize, sizeof(unsigned int), &dwWrite, NULL);
		WriteFile(hFile, &pImgInfoArray[i].nWidth, sizeof(int), &dwWrite, NULL);
		WriteFile(hFile, &pImgInfoArray[i].nHeight, sizeof(int), &dwWrite, NULL);
		WriteFile(hFile, &pImgInfoArray[i].sOftX, sizeof(short), &dwWrite, NULL);
		WriteFile(hFile, &pImgInfoArray[i].sOftX, sizeof(short), &dwWrite, NULL);
	}

	//	write png file data
	char szBuf[10240];
	DWORD dwCanRead = 0;

	for(int i = 0; i < nMaxNumber; ++i)
	{
		fndIter = std::find(xFileNumbers.begin(), xFileNumbers.end(), i);

		if(fndIter != xFileNumbers.end())
		{
			int nFileNumber = *fndIter;

			char szFileName[MAX_PATH];
			sprintf(szFileName, "%s/%06d.png",
				_pszInputDirectory, nFileNumber);

			if(PathFileExists(szFileName))
			{
				HANDLE hPngFile = ::CreateFile(szFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
				if(hPngFile != INVALID_HANDLE_VALUE)
				{
					DWORD dwSize = pImgInfoArray[i].uSize;

					while(dwSize > 0)
					{
						dwCanRead = (dwSize > sizeof(szBuf) ? sizeof(szBuf) : dwSize);
						ReadFile(hPngFile, szBuf, dwCanRead, &dwRead, NULL);
						WriteFile(hFile, szBuf, dwCanRead, &dwWrite, NULL);
						dwSize -= dwCanRead;
					}

					printf("Pack %06d.png\n", nFileNumber);
					CloseHandle(hPngFile);
					hPngFile = INVALID_HANDLE_VALUE;
				}
			}
		}
	}

	//	ok
	CloseHandle(hFile);
	hFile = INVALID_HANDLE_VALUE;

	return nRet;
}

int SPBFile::Unpack(const char* _pszInputFile, const char* _pszOutDirectory)
{
	if(!PathFileExists(_pszInputFile))
	{
		return -1;
	}
	if(!PathFileExists(_pszOutDirectory))
	{
		return -2;
	}

	SPBFile xFile;
	if(!xFile.Load(_pszInputFile))
	{
		return -3;
	}

	int nImgCount = xFile.GetImgCount();
	for(int i = 0; i < nImgCount; ++i)
	{
		char szDestFile[MAX_PATH];
		sprintf(szDestFile, "%s/%06d.png",
			_pszOutDirectory, i);

		const SPBFileInfo* pInfo = xFile.GetImgInfo(i);
		if(pInfo &&
			pInfo->dwOffset &&
			pInfo->uSize)
		{
			const char* pszData = xFile.GetImgData(i, NULL);

			if(pszData)
			{
				DWORD dwWrite = 0;
				HANDLE hFile = ::CreateFile(szDestFile, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, NULL, NULL);
				WriteFile(hFile, pszData, pInfo->uSize, &dwWrite, NULL);
				if(dwWrite == pInfo->uSize)
				{
					printf("extract %06d.png ok\n", i);
				}
				CloseHandle(hFile);
				hFile = INVALID_HANDLE_VALUE;
			}
		}
	}

	xFile.Unload();

	return 0;
}