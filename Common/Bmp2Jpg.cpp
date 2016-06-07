// Bmp2Jpg.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>
#include <GdiPlus.h>
#include <Gdiplusimaging.h>
#include <atlconv.h>

#pragma comment(lib, "gdiplus.lib")

int GetCodecClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes

	Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;

	Gdiplus::GetImageEncodersSize(&num, &size);
	if(size == 0)
		return -1;  // Failure

	pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
	if(pImageCodecInfo == NULL)
		return -1;  // Failure

	GetImageEncoders(num, size, pImageCodecInfo);

	for(size_t j = 0; j < num; ++j)
	{
		if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			return j;  // Success
		}    
	} // for

	return -1;  // Failure

} // GetCodecClsid

BSTR AllocSysString(const char* _pszStr)
{
	int nLen = ::MultiByteToWideChar( CP_ACP, 0, _pszStr, strlen(_pszStr),
		NULL, NULL );
	BSTR bstr = ::SysAllocStringLen( NULL, nLen );
	if( bstr != NULL )
	{
		::MultiByteToWideChar( CP_ACP, 0, _pszStr, strlen(_pszStr),
			bstr, nLen );
	}

	return bstr;
}

bool BmpTrans2Jpg(const char* _pszSrcBmpFile, const char* _pszDestJpgFile)
{
	bool bRet = false;
	Gdiplus::GdiplusStartupInput gdiplusStartupInput; 
	ULONG gdiplusToken; 
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL); 

	{
		CLSID              codecClsid;
		Gdiplus::Status             stat;
		Gdiplus::EncoderParameters  encoderParameters;

		LPOLESTR bstr_src_img;
		LPOLESTR bstr_dst_fn;

		bstr_src_img = AllocSysString(_pszSrcBmpFile);
		bstr_dst_fn = AllocSysString(_pszDestJpgFile);

		Gdiplus::Image image(bstr_src_img);

		// Get the CLSID of the JPEG codec.
		GetCodecClsid(L"image/jpeg", &codecClsid);

		encoderParameters.Count = 1;
		encoderParameters.Parameter[0].Guid = Gdiplus::EncoderQuality;
		encoderParameters.Parameter[0].Type = Gdiplus::EncoderParameterValueTypeLong;
		encoderParameters.Parameter[0].NumberOfValues = 1;

		// Save the image as a JPEG with quality level 0.
		long quality = 100;
		encoderParameters.Parameter[0].Value = &quality;

		stat =image.Save(bstr_dst_fn, &codecClsid, &encoderParameters);   

		if(stat == Gdiplus::Ok)
		{
			bRet = true;
		}
		else
		{
			
		}

		SysFreeString(bstr_dst_fn);
		SysFreeString(bstr_src_img);
	}
	Gdiplus::GdiplusShutdown(gdiplusToken);

	return bRet;
}

