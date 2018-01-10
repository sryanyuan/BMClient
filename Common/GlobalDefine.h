#ifndef _GLOBALDEFINE_H_
#define _GLOBALDEFINE_H_

// #define SINGLETON_DECLARE(class_name)				\
// 	public:											\
// 	static class_name* GetInstance()				\
// 	{												\
// 	static class_name* pIns = NULL;				\
// 	if(!pIns)									\
// 		{											\
// 		pIns = new class_name();				\
// 		}											\
// 		return pIns;								\
// 	};	
//#define SAFE_DELETE(p)	{if(p){delete p;p = NULL;}}
//#define SAFE_DELETE_ARRAY(p)	{if(p){delete[] p;p = NULL;}}
//#define SAFE_NEW(p, type)	{if(NULL == p){p = new type;}}

#define RECT_WIDTH(rc)	(rc.right - rc.left)
#define RECT_HEIGHT(rc) (rc.bottom - rc.top)

//	单机 OR 联网模式
//#define NETWORK_MODE
#define MAGIC_PLAY_OFTX		8
#define MAGIC_PLAY_OFTY		8

#define ARGB_WHITE	ARGB(255, 255, 255, 255)
#define ARGB_RED	ARGB(255, 255, 0, 0)
#define ARGB_GREEN	ARGB(255, 0, 255, 0)
#define ARGB_BLUE	ARGB(255, 0, 0, 255)
#define ARGB_YELLOW	ARGB(255, 255, 255, 0)
#define ARGB_BLACK	ARGB(255, 0, 0, 0)

#define COLOR_SYSTEM	0xffe53fc8
#define COLOR_PRIVATECHAT		0xff1E90FF

#define MOUSE_SEARCH_X			2
#define MOUSE_SEARCH_Y			2

#define FONT_WIDTH_WORD			12
#define FONT_HEIGHT_WORD		12
#define FONT_WIDTH_NUMBER		6
#define FONT_HEIGHT_NUMBER		6

#define VIEW_OPERATE			120

#define CENTRAL_X				(48 * 8 - 8)
#define CENTRAL_Y				(32 * 7)

#define WINDOW_WIDTH			800
#define WINDOW_HEIGHT			600

#define VIEW_OFFSET				25
#define INVALID_ITEM_ID			-1

#define _PI_					3.141592

//	怪物爆物品时候 遍历的格子数
#define FLOOR_ITEM_OFFSET		4

//	加血 加蓝的方式	NORMAL : 直接加			SPECL : 根据步长1秒1秒加
#define ADD_NORMAL				0
#define ADD_SPECL				1

//	装备说明换行字数
#define BREAK_NUMBER			28

//	斜着跑的距离
#define RUN_HYPO_DISTANCE		120
//	横着跑的距离
#define WALK_HYPO_DISTANCE		0

//	方向判断
#define DRT_UP					0x01
#define DRT_RIGHTUP				(DRT_RIGHT | DRT_UP)
#define DRT_RIGHT				0x02
#define DRT_RIGHTDOWN			(DRT_RIGHT | DRT_DOWN)
#define DRT_DOWN				0x04
#define DRT_LEFTDOWN			(DRT_LEFT | DRT_DOWN)
#define DRT_LEFT				0x08
#define DRT_LEFTUP				(DRT_LEFT | DRT_UP)

//	最大魔法数量
#define MAX_MGC_NUM				20

//	最长说话长度
#define MAX_SAY_LENGTH			80

//	玩家朝向
enum PLAYER_DIRECTION
{
	PDT_UP = 0,
	PDT_RIGHTUP,
	PDT_RIGHT,
	PDT_RIGHTDOWN,
	PDT_DOWN,
	PDT_LEFTDOWN,
	PDT_LEFT,
	PDT_LEFTUP,
};

//	玩家状态
enum PLAYER_STATUS
{
	PST_NONE,
	PST_STAND,
	PST_WALK,
	PST_RUN,
	PST_ATTACKSTOP,
	PST_ATTACKNOWEAPON,
	PST_ATTACKWEAPON,
	PST_FORCEATTACK,
	PST_SK_LEIDIANSHU,
	PST_WAROU,
	PST_ATTACKED,
	PST_DEAD,
	PST_GROUND,
	PST_APPEAR,
	PST_SHOW,
	PST_SLEEP,
	PST_WAKE,
	PST_EXT1,
	PST_EXT2,
	PST_EXT3,
	PST_EXT4
};

//	玩家性别
enum PLAYER_SEX
{
	PSX_NONE,
	PSX_MAN,
	PSX_WOMAN,
};

//	NPC类型
enum OBJECT_TYPE
{
	OBJ_NPC,
	OBJ_MONS,
	OBJ_PLAYER,
	OBJ_OTHERPLAYER,
	OBJ_SELF,
// 	OBJ_ZHANSHI,
//  	OBJ_FASHI,
//  	OBJ_DAOSHI,
	OBJ_UNKNOW,
};

//	职业
#define JOB_DC	0
#define JOB_MC	1
#define JOB_SC	2

//	游戏资源
enum GAME_RES
{
	RES_TILES = 0,
	RES_SMTILES,
	RES_HUM,
	RES_HUM2,
	RES_OBJECTS,
	RES_OBJECTS2,
	RES_OBJECTS3,
	RES_OBJECTS4,
	RES_OBJECTS5,
	RES_OBJECTS6,
	RES_OBJECTS7,
	RES_OBJECTS8,
	RES_OBJECTS9,
	RES_OBJECTS10,
	RES_OBJECTS11,
	RES_OBJECTS12,
	RES_OBJECTS13,
	RES_OBJECTS14,
	RES_HAIR,
	RES_HAIR2,
	RES_WEAPON,
	RES_WEAPON2,
	RES_WEAPONEFFECT,
	//RES_PRGUSE,
	//RES_PRGUSE2,
	RES_OPUI,
	RES_ITEMS,
	RES_DNITEMS,
	RES_MON1,
	RES_MON2,
	RES_MON3,
	RES_MON4,
	RES_MON5,
	RES_MON6,
	RES_MON7,
	RES_MON8,
	RES_MON9,
	RES_MON10,
	RES_MON11,
	RES_MON12,
	RES_MON13,
	RES_MON14,
	RES_MON15,
	RES_MON16,
	RES_MON17,
	RES_MON18,
	RES_MON19,
	RES_MON20,
	RES_MON21,
	RES_MON22,
	RES_MON23,
	RES_MON24,
	RES_MON25,
	RES_MON26,
	RES_MON27,
	RES_MON28,
	RES_MON29,
	RES_MON30,
	RES_MON31,
	RES_MON32,
	RES_MON33,
	RES_MMAP,
	RES_BMAP,
	RES_MAGIC,
	RES_MAGIC2,
	RES_MAGIC3,
	RES_MAGIC4,
	RES_MAGIC5,
	RES_MAGIC6,
	RES_MAGIC7,
	RES_MAGIC8,
	RES_MAGIC9,
	RES_MAGIC10,
	RES_NPC,
	RES_STATE,
	RES_HUMEFFECT,
	RES_HUMEFFECT2,
	RES_MAGICICON,
	RES_STATUS,
	RES_EFFECT,
	RES_CUSTOM,
	RES_ANI,
	//RES_CHRSEL,
	RES_TOTAL,
};

#define MAX_OBJECTS	RES_OBJECTS14

//	窗口置顶消息
#define WM_DLG_BRINGTOTOP	(0x0400 + 1)
//	Socket消息
#define WM_SOCKMSG			(0x0400 + 2)
//	场景切换进度消息
#define WM_SCENE_PROG		(0x0400 + 3)
//	进入游戏场景
#define WM_ENTER_GAMESCENE	(0x0400 + 4)
//	小退 回到人物界面
#define WM_BACK_SELCHR		(0x0400 + 5)
// 关闭辅助窗口
#define WM_HIDE_ASSITWND	(0x0400 + 6)

//	定时器
#define TIMER_CONNECTSVR	(101)
#define TIMER_CONNTIMEOUT	(102)
#define TIMER_CLOSEGAME		(103)
#define TIMER_LOADDATA		(104)
#define TIMER_RUNGAME		(105)
#define TIMER_FTYPETIMEOUT	(106)
#define TIMER_STYPETIMEOUT	(107)
#define TIMER_VERIFYTIMEOUT	(108)
#define TIMER_SEND_LOGIN_HEARTBEAT	(109)

#define TIMER_PRECONN_UPDATE (100)

//	场景信息
#define SCENE_LOGIN		0
#define SCENE_SELCHR	1
#define SCENE_GAME		2

#endif