#ifndef _INC_SPBFILE_
#define _INC_SPBFILE_
//////////////////////////////////////////////////////////////////////////
#include <Windows.h>
//////////////////////////////////////////////////////////////////////////
#define SPBFILEDESC		"#sryan's png box#"
//////////////////////////////////////////////////////////////////////////
struct SPBFileHeader
{
	char szFileDesc[20];
	int nImgCount;
};

struct SPBFileInfo
{
	DWORD dwOffset;
	unsigned int uSize;
	int nWidth;
	int nHeight;
	short sOftX;
	short sOftY;
};

class SPBFile
{
public:
	SPBFile();
	virtual ~SPBFile();

public:
	static int Pack(const char* _pszInputDirectory, const char* _pszOutFile);
	static int Unpack(const char* _pszInputFile, const char* _pszOutDirectory);

public:
	bool Load(const char* _pszFile);
	void Unload();
	int GetImgCount();

	const SPBFileInfo* GetImgInfo(int _nIndex);
	const char* GetImgData(int _nIndex, DWORD* _pSize);

protected:
	SPBFileHeader m_stHeader;
	//int m_nImgCount;
	//UINT* m_pImgOffsetArray;
	SPBFileInfo* m_pImgInfoArray;
	HANDLE m_hFile;
};
//////////////////////////////////////////////////////////////////////////
#endif