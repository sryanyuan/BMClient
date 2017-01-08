// shlwapi.lib
#pragma comment(lib, "shlwapi.lib")

// duilib
#ifdef _DEBUG
#pragma comment(lib, "duilib_d.lib")
#else
#pragma comment(lib, "duilib.lib")
#endif

//	protobuf
#ifdef _DEBUG
#pragma comment(lib, "libprotobuf_D.lib")
#else
#pragma comment(lib, "libprotobuf.lib")
#endif

// hge
#pragma comment(lib,"hge.lib")
#pragma comment(lib,"hgehelp.lib")

// gdiplus
#pragma comment(lib, "gdiplus.lib")

// dbghelp
#pragma comment(lib, "DbgHelp.lib")

// sqlite3
#pragma comment(lib, "sqlite3.lib")

// zlib
#ifdef _DEBUG
#pragma comment(lib, "zlibwapi_d.lib")
#else
#pragma comment(lib, "zlibwapi.lib")
#endif

// ziparchive
#ifdef _DEBUG
#pragma comment(lib, "ZipArchive_D.lib")
#else
#pragma comment(lib, "ZipArchive.lib")
#endif

// fmodex
#pragma comment(lib, "fmodex.lib")

// dx8
#pragma comment(lib, "d3d8.lib")
#pragma comment(lib, "d3dx8.lib")

// client site using lua + tolua++
// new version using the same as server (luajit + tolua++)
// libtolua (tolua++ with lua)
#ifdef _DEBUG
#pragma comment(lib, "libtolua_d.lib")
#else
#pragma comment(lib, "libtolua.lib")
#endif

// luajit
#ifdef _DEBUG
#pragma comment(lib, "lua51d.lib")
#else
#pragma comment(lib, "lua51.lib")
#endif

// libcurl
#ifdef _DEBUG
#pragma comment(lib, "libcurld.lib")
#else
#pragma comment(lib, "libcurl.lib")
#endif