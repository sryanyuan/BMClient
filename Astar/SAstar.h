#ifndef _SASTART_H_
#define _SASTART_H_
#include <Windows.h>
#include <stack>
#include <vector>
#include <list>

#define SEARCH_OFFSET_X	5
#define SEARCH_OFFSET_Y	5

namespace SASTAR
{
	enum ASTAR_DIRECTION
	{
		ADT_TOPLEFT,
		ADT_TOP,
		ADT_TOPRIGHT,
		ADT_LEFT,
		ADT_RIGHT,
		ADT_BOTTOMLEFT,
		ADT_BOTTOM,
		ADT_BOTTOMRIGHT,
		ADT_TOTAL,
	};
	struct SAstarElement
	{
		int nX;
		int nY;
		BOOL bOpen;
		//	计算信息
		//	和
		int nFn;
		//	开始点到当前位置移动距离
		UINT nGn;
		//	当前位置到目标坐标的距离
		UINT nHn;
		SAstarElement* pParent;
	};

	typedef std::list<SAstarElement>								SAstarElements;
	typedef std::list<SAstarElement*>								SAstarPelements;
	typedef std::list<POINT>										SAstarPoints;

	class SAstarDataAdaptor
	{
	public:
		SAstarDataAdaptor()
		{
			m_pSrc = NULL;
		}
		virtual ~SAstarDataAdaptor(){}

	public:
		void SetDataSource(void* _psrc)
		{
			m_pSrc = _psrc;
		}
		//	返回元素nFn为-1表示元素不存在
		virtual void GetData(int _x, int _y, SAstarElement* _pele) const = 0;

	protected:
		void* m_pSrc;
	};

	//	Element
	BOOL Calculate8DirectionNormal(const SAstarDataAdaptor* _psrc, SAstarElements* _pret, const SAstarElement* _pfrom, const SAstarElement* _pto);
	BOOL Calculate8DirectionNoDiag(const SAstarDataAdaptor* _psrc, SAstarElements* _pret, const SAstarElement* _pfrom, const SAstarElement* _pto);
	//	POINT
	BOOL Calculate8DirectionNormal(const SAstarDataAdaptor* _psrc, SAstarPoints* _pret, const POINT* _pfrom, const POINT* _pto);
	BOOL Calculate8DirectionNoDiag(const SAstarDataAdaptor* _psrc, SAstarPoints* _pret, const POINT* _pfrom, const POINT* _pto);

	//	启发式计算Hn
	int GetElementHn(const SAstarElement* _pcur, const SAstarElement* _pdest);
	int GetElementHn(const SAstarElement* _pcur, const POINT* _pdest);
	bool operator== (const SAstarElement& ele1, const SAstarElement& ele2);

	//	释放资源
	void FreeMemory();

	struct SAstarCompGreater
	{
		bool operator()(const SAstarElement* e1, const SAstarElement* e2)
		{
 			if(e1->nFn < e2->nFn)
 			{
 				return true;
 			}
			
			return false;
		}
	};

	struct SAstarEqual
	{
		bool operator()(const SAstarElement& e1, const SAstarElement& e2)
		{
			if(e1.nX == e2.nX &&
				e2.nY == e1.nY)
			{
				return true;
			}
			return false;
		}
	};

};

#endif