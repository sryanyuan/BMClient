#ifndef SCRIPTCOMMAND_H_
#define SCRIPTCOMMAND_H_

enum SCRIPT_COMMAND
{
	SCRIPT_CMD_BEGIN	=	0,


	//	1	添加物品 第一个参数为物品id
	SCRIPT_CMD_ADDITEM,
	//	2	检测是否有物品 param0 物品id 返回值物品数量
	//	扩展 -1:获取玩家金钱 -2:获取玩家等级
	SCRIPT_CMD_HAVEITEM,
	//	3	检测背包空余格子数 返回值为格子数
	SCRIPT_CMD_EMPTY_ITEM,
	//	4	给玩家加血	param0 改变数 可以正可以负
	SCRIPT_CMD_CHANGEHP,
	//	5	改变玩家MP param0 改变数 可以正可以负
	SCRIPT_CMD_CHANGEMP,
	//	6	改变玩家EXP param0 改变数
	SCRIPT_CMD_CHANGEEXP,
	//	7	传送玩家 修改玩家坐标 param0 x坐标 param1 y坐标
	SCRIPT_CMD_MOVEPLAYER,
	//	8	关闭对话框 param0 哪个对话框
	SCRIPT_CMD_CLOSEDLG,
	//	9	关闭所有对话框
	SCRIPT_CMD_CLOSEALLDLG,
	//	10	显示装备交易窗 param0 种类 param1最小等级 param2 最大等级
	SCRIPT_CMD_SHOWTRADEDLG,
	//	11	创建怪物/NPC param0 ID param1 x坐标 param2 y坐标 返回值 是否成功 0/1
	SCRIPT_CMD_CREATENPC,
	//	12	获得玩家X坐标
	SCRIPT_CMD_GETPLAYERCOORDX,
	//	13	获得玩家Y坐标
	SCRIPT_CMD_GETPLAYERCOORDY,
	//	14	输出脚本错误信息 param0 文件 param1 函数 param2 待定
	SCRIPT_CMD_EXECUTEERROR,
	//	15	得到玩家的任务stage 参数无要求 返回值任务stage
	SCRIPT_CMD_GETQUESTSTAGE,
	//	16	增加玩家人物stage param0 增加值
	SCRIPT_CMD_ADDQUESTSTAGE,


	SCRIPT_CMD_END,
};

#endif