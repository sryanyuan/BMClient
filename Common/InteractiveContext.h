#ifndef INTERACTIVECONTEXT_H_
#define INTERACTIVECONTEXT_H_

#define INTERACTIVE_BUTTON_PREPAGE		0
#define INTERACTIVE_BUTTON_NEXTPAGE		1
#define INTERACTIVE_BUTTON_CLOSE		2

#include "../../CommonModule/ByteBuffer.h"
#include <Windows.h>

//	> 5 的按钮命令ID 均会被传给脚本处理

#define BUFER_SIZE						1024 * 2


//	互动窗口上下文 页数由0开始 确定按钮处理交给外界处理 其它自己处理
/*  '/'转义符 p该内容页数 h标题 t内容 b按钮
Format: /p/h[TEXT]/t/b[ID][TEXT]/p[NEXTPAGE]
Exp:	/p/h小贩/t请问要买/b3确定...
*/

class InteractiveContext
{
public:
	InteractiveContext();
	~InteractiveContext();

public:
	//	外界接口
	void AddPage();
	void AddButton(int _id, const char* _text);
	void AddHead(const char* _text);
	void AddText(const char* _text);
	//void EndPage();

public:
	//	内部处理
	int GetPageCount();
	int GetCurPage();
	bool NextPage();
	bool PrevPage();
	const char* GetTitle();
	const char* GetText();
	int GetButtonSum();
	int GetButtonID(int _idx);
	const char* GetButtonText(int _idx);

public:
	void Reset();
	void Init();
	bool CheckValid();
	bool LoadFromFile(const char* lpszFileName);

private:
	bool InternalCheck();
	void InternalUpdateReadOffset(bool _forward);
	void InternalUpdateDataLength();

private:
	int m_nCurWritePage;
	int m_nCurReadpage;
	int m_nCurReadPageOffsetBegin;
	int m_nCurReadPageOffsetEnd;
	int m_nCurReadDataLength;
	int m_nPageSum;
	char m_szData[BUFER_SIZE];
	char m_szRet[BUFER_SIZE / 2];
};

struct ContextNode
{
	RECT rcLocate;
	const char *szMsg;
	DWORD dwID;
};
typedef std::list<ContextNode*> CONTEXTNODELIST;

class InteractiveContextEx
{
public:
	InteractiveContextEx();
	~InteractiveContextEx();

public:
	//	All for script
	void AddContextNode(int _x, int _y, const char* _pMsg, int _nId);
	void Clear();

	//	Not for script
	CONTEXTNODELIST& GetNodes()
	{
		return m_xNodes;
	}
	int GetButtonID(int _idx);

private:
	ByteBuffer m_xContext;
	std::list<ContextNode*> m_xNodes;
};
//////////////////////////////////////////////////////////////////////////
#endif