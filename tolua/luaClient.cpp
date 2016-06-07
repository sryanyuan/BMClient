/*
** Lua binding: BackMirClient
** Generated automatically by tolua++-1.0.92 on 12/05/15 10:51:41.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_BackMirClient_open (lua_State* tolua_S);

#include "../Common/InteractiveContext.h"
#include "../Common/GlobalFunction.h"
#include "../../CommonModule/QuestContext.h"
#include "../GameDialog/GameQuestDlg.h"
#include "../GameScene/GamePlayer.h"
#include "../BackMir/BackMir.h"
#include "../Common/GameRenderObject.h"
#include "../GameDialog/GameCommonDlg.h"
#include "../GameDialog/ScriptWnd.h"
#include "../BackMir/LuaClientEngine.h"

/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"InteractiveContextEx");
 tolua_usertype(tolua_S,"QuestContext");
 tolua_usertype(tolua_S,"GameQuestDlg");
 tolua_usertype(tolua_S,"MirGame");
 tolua_usertype(tolua_S,"GameScriptWnd");
 tolua_usertype(tolua_S,"QuestSummary");
 tolua_usertype(tolua_S,"QuestInfo");
 tolua_usertype(tolua_S,"GameCommonDlg");
 tolua_usertype(tolua_S,"GameNewDlg");
 tolua_usertype(tolua_S,"RenderObject");
 tolua_usertype(tolua_S,"GamePlayer");
 tolua_usertype(tolua_S,"QuestTip");
 tolua_usertype(tolua_S,"GameOtherPlayer");
}

/* method: AddContextNode of class  InteractiveContextEx */
#ifndef TOLUA_DISABLE_tolua_BackMirClient_InteractiveContextEx_AddContextNode00
static int tolua_BackMirClient_InteractiveContextEx_AddContextNode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"InteractiveContextEx",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isstring(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  InteractiveContextEx* self = (InteractiveContextEx*)  tolua_tousertype(tolua_S,1,0);
  int _x = ((int)  tolua_tonumber(tolua_S,2,0));
  int _y = ((int)  tolua_tonumber(tolua_S,3,0));
  const char* _pMsg = ((const char*)  tolua_tostring(tolua_S,4,0));
  int _nId = ((int)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddContextNode'", NULL);
#endif
  {
   self->AddContextNode(_x,_y,_pMsg,_nId);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddContextNode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Clear of class  InteractiveContextEx */
#ifndef TOLUA_DISABLE_tolua_BackMirClient_InteractiveContextEx_Clear00
static int tolua_BackMirClient_InteractiveContextEx_Clear00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"InteractiveContextEx",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  InteractiveContextEx* self = (InteractiveContextEx*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Clear'", NULL);
#endif
  {
   self->Clear();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Clear'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: MirLog */
#ifndef TOLUA_DISABLE_tolua_BackMirClient_MirLog00
static int tolua_BackMirClient_MirLog00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* _pszLog = ((const char*)  tolua_tostring(tolua_S,1,0));
  {
   MirLog(_pszLog);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'MirLog'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsQuestComplete of class  QuestContext */
#ifndef TOLUA_DISABLE_tolua_BackMirClient_QuestContext_IsQuestComplete00
static int tolua_BackMirClient_QuestContext_IsQuestComplete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"QuestContext",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  QuestContext* self = (QuestContext*)  tolua_tousertype(tolua_S,1,0);
  int _questid = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsQuestComplete'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsQuestComplete(_questid);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsQuestComplete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetQuestComplete of class  QuestContext */
#ifndef TOLUA_DISABLE_tolua_BackMirClient_QuestContext_SetQuestComplete00
static int tolua_BackMirClient_QuestContext_SetQuestComplete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"QuestContext",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  QuestContext* self = (QuestContext*)  tolua_tousertype(tolua_S,1,0);
  int _questid = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetQuestComplete'", NULL);
#endif
  {
   self->SetQuestComplete(_questid);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetQuestComplete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsQuestAccept of class  QuestContext */
#ifndef TOLUA_DISABLE_tolua_BackMirClient_QuestContext_IsQuestAccept00
static int tolua_BackMirClient_QuestContext_IsQuestAccept00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"QuestContext",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  QuestContext* self = (QuestContext*)  tolua_tousertype(tolua_S,1,0);
  int _questid = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsQuestAccept'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsQuestAccept(_questid);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsQuestAccept'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetQuestStep of class  QuestContext */
#ifndef TOLUA_DISABLE_tolua_BackMirClient_QuestContext_GetQuestStep00
static int tolua_BackMirClient_QuestContext_GetQuestStep00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"QuestContext",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  QuestContext* self = (QuestContext*)  tolua_tousertype(tolua_S,1,0);
  int _questid = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetQuestStep'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetQuestStep(_questid);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetQuestStep'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetQuestStep of class  QuestContext */
#ifndef TOLUA_DISABLE_tolua_BackMirClient_QuestContext_SetQuestStep00
static int tolua_BackMirClient_QuestContext_SetQuestStep00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"QuestContext",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  QuestContext* self = (QuestContext*)  tolua_tousertype(tolua_S,1,0);
  int _questid = ((int)  tolua_tonumber(tolua_S,2,0));
  int _step = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetQuestStep'", NULL);
#endif
  {
   self->SetQuestStep(_questid,_step);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetQuestStep'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetQuestCounter of class  QuestContext */
#ifndef TOLUA_DISABLE_tolua_BackMirClient_QuestContext_GetQuestCounter00
static int tolua_BackMirClient_QuestContext_GetQuestCounter00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"QuestContext",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  QuestContext* self = (QuestContext*)  tolua_tousertype(tolua_S,1,0);
  int _questid = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetQuestCounter'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetQuestCounter(_questid);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetQuestCounter'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetQuestCounter of class  QuestContext */
#ifndef TOLUA_DISABLE_tolua_BackMirClient_QuestContext_SetQuestCounter00
static int tolua_BackMirClient_QuestContext_SetQuestCounter00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"QuestContext",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  QuestContext* self = (QuestContext*)  tolua_tousertype(tolua_S,1,0);
  int _questid = ((int)  tolua_tonumber(tolua_S,2,0));
  int _counter = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetQuestCounter'", NULL);
#endif
  {
   self->SetQuestCounter(_questid,_counter);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetQuestCounter'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: xQuestName of class  QuestSummary */
#ifndef TOLUA_DISABLE_tolua_get_QuestSummary_xQuestName
static int tolua_get_QuestSummary_xQuestName(lua_State* tolua_S)
{
  QuestSummary* self = (QuestSummary*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'xQuestName'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->xQuestName);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: xQuestName of class  QuestSummary */
#ifndef TOLUA_DISABLE_tolua_set_QuestSummary_xQuestName
static int tolua_set_QuestSummary_xQuestName(lua_State* tolua_S)
{
  QuestSummary* self = (QuestSummary*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'xQuestName'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->xQuestName = ((std::string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: nType of class  QuestSummary */
#ifndef TOLUA_DISABLE_tolua_get_QuestSummary_nType
static int tolua_get_QuestSummary_nType(lua_State* tolua_S)
{
  QuestSummary* self = (QuestSummary*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'nType'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->nType);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: nType of class  QuestSummary */
#ifndef TOLUA_DISABLE_tolua_set_QuestSummary_nType
static int tolua_set_QuestSummary_nType(lua_State* tolua_S)
{
  QuestSummary* self = (QuestSummary*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'nType'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->nType = ((int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: xDescript of class  QuestTip */
#ifndef TOLUA_DISABLE_tolua_get_QuestTip_xDescript
static int tolua_get_QuestTip_xDescript(lua_State* tolua_S)
{
  QuestTip* self = (QuestTip*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'xDescript'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->xDescript);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: xDescript of class  QuestTip */
#ifndef TOLUA_DISABLE_tolua_set_QuestTip_xDescript
static int tolua_set_QuestTip_xDescript(lua_State* tolua_S)
{
  QuestTip* self = (QuestTip*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'xDescript'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->xDescript = ((std::string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: xRequire of class  QuestTip */
#ifndef TOLUA_DISABLE_tolua_get_QuestTip_xRequire
static int tolua_get_QuestTip_xRequire(lua_State* tolua_S)
{
  QuestTip* self = (QuestTip*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'xRequire'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->xRequire);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: xRequire of class  QuestTip */
#ifndef TOLUA_DISABLE_tolua_set_QuestTip_xRequire
static int tolua_set_QuestTip_xRequire(lua_State* tolua_S)
{
  QuestTip* self = (QuestTip*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'xRequire'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->xRequire = ((std::string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: xReward of class  QuestTip */
#ifndef TOLUA_DISABLE_tolua_get_QuestTip_xReward
static int tolua_get_QuestTip_xReward(lua_State* tolua_S)
{
  QuestTip* self = (QuestTip*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'xReward'",NULL);
#endif
  tolua_pushcppstring(tolua_S,(const char*)self->xReward);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: xReward of class  QuestTip */
#ifndef TOLUA_DISABLE_tolua_set_QuestTip_xReward
static int tolua_set_QuestTip_xReward(lua_State* tolua_S)
{
  QuestTip* self = (QuestTip*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'xReward'",NULL);
  if (!tolua_iscppstring(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->xReward = ((std::string)  tolua_tocppstring(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: stSummary of class  QuestInfo */
#ifndef TOLUA_DISABLE_tolua_get_QuestInfo_stSummary
static int tolua_get_QuestInfo_stSummary(lua_State* tolua_S)
{
  QuestInfo* self = (QuestInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'stSummary'",NULL);
#endif
   tolua_pushusertype(tolua_S,(void*)&self->stSummary,"QuestSummary");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: stSummary of class  QuestInfo */
#ifndef TOLUA_DISABLE_tolua_set_QuestInfo_stSummary
static int tolua_set_QuestInfo_stSummary(lua_State* tolua_S)
{
  QuestInfo* self = (QuestInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'stSummary'",NULL);
  if ((tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"QuestSummary",0,&tolua_err)))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->stSummary = *((QuestSummary*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: stTip of class  QuestInfo */
#ifndef TOLUA_DISABLE_tolua_get_QuestInfo_stTip
static int tolua_get_QuestInfo_stTip(lua_State* tolua_S)
{
  QuestInfo* self = (QuestInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'stTip'",NULL);
#endif
   tolua_pushusertype(tolua_S,(void*)&self->stTip,"QuestTip");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: stTip of class  QuestInfo */
#ifndef TOLUA_DISABLE_tolua_set_QuestInfo_stTip
static int tolua_set_QuestInfo_stTip(lua_State* tolua_S)
{
  QuestInfo* self = (QuestInfo*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'stTip'",NULL);
  if ((tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"QuestTip",0,&tolua_err)))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->stTip = *((QuestTip*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: PushQuestInfo of class  GameQuestDlg */
#ifndef TOLUA_DISABLE_tolua_BackMirClient_GameQuestDlg_PushQuestInfo00
static int tolua_BackMirClient_GameQuestDlg_PushQuestInfo00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GameQuestDlg",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameQuestDlg* self = (GameQuestDlg*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'PushQuestInfo'", NULL);
#endif
  {
   QuestInfo* tolua_ret = (QuestInfo*)  self->PushQuestInfo();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"QuestInfo");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'PushQuestInfo'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetHeroJob of class  GameOtherPlayer */
#ifndef TOLUA_DISABLE_tolua_BackMirClient_GameOtherPlayer_GetHeroJob00
static int tolua_BackMirClient_GameOtherPlayer_GetHeroJob00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GameOtherPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameOtherPlayer* self = (GameOtherPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetHeroJob'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetHeroJob();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetHeroJob'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetHeroLevel of class  GameOtherPlayer */
#ifndef TOLUA_DISABLE_tolua_BackMirClient_GameOtherPlayer_GetHeroLevel00
static int tolua_BackMirClient_GameOtherPlayer_GetHeroLevel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GameOtherPlayer",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameOtherPlayer* self = (GameOtherPlayer*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetHeroLevel'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetHeroLevel();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetHeroLevel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetInstance of class  MirGame */
#ifndef TOLUA_DISABLE_tolua_BackMirClient_MirGame_GetInstance00
static int tolua_BackMirClient_MirGame_GetInstance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"MirGame",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   MirGame* tolua_ret = (MirGame*)  MirGame::GetInstance();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"MirGame");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetInstance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetDarkMode of class  MirGame */
#ifndef TOLUA_DISABLE_tolua_BackMirClient_MirGame_SetDarkMode00
static int tolua_BackMirClient_MirGame_SetDarkMode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MirGame",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MirGame* self = (MirGame*)  tolua_tousertype(tolua_S,1,0);
  int _nMode = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetDarkMode'", NULL);
#endif
  {
   self->SetDarkMode(_nMode);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetDarkMode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetShowMapSnap of class  MirGame */
#ifndef TOLUA_DISABLE_tolua_BackMirClient_MirGame_SetShowMapSnap00
static int tolua_BackMirClient_MirGame_SetShowMapSnap00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"MirGame",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  MirGame* self = (MirGame*)  tolua_tousertype(tolua_S,1,0);
  bool _bShow = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetShowMapSnap'", NULL);
#endif
  {
   self->SetShowMapSnap(_bShow);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetShowMapSnap'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetVisible of class  RenderObject */
#ifndef TOLUA_DISABLE_tolua_BackMirClient_RenderObject_SetVisible00
static int tolua_BackMirClient_RenderObject_SetVisible00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RenderObject",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RenderObject* self = (RenderObject*)  tolua_tousertype(tolua_S,1,0);
  bool _vb = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetVisible'", NULL);
#endif
  {
   self->SetVisible(_vb);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetVisible'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: IsVisible of class  RenderObject */
#ifndef TOLUA_DISABLE_tolua_BackMirClient_RenderObject_IsVisible00
static int tolua_BackMirClient_RenderObject_IsVisible00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const RenderObject",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const RenderObject* self = (const RenderObject*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'IsVisible'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->IsVisible();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'IsVisible'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetIDInt of class  RenderObject */
#ifndef TOLUA_DISABLE_tolua_BackMirClient_RenderObject_GetIDInt00
static int tolua_BackMirClient_RenderObject_GetIDInt00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"RenderObject",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  RenderObject* self = (RenderObject*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetIDInt'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetIDInt();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetIDInt'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AddCommonButton of class  GameCommonDlg */
#ifndef TOLUA_DISABLE_tolua_BackMirClient_GameCommonDlg_AddCommonButton00
static int tolua_BackMirClient_GameCommonDlg_AddCommonButton00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GameCommonDlg",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isstring(tolua_S,6,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameCommonDlg* self = (GameCommonDlg*)  tolua_tousertype(tolua_S,1,0);
  int _type = ((int)  tolua_tonumber(tolua_S,2,0));
  int _id = ((int)  tolua_tonumber(tolua_S,3,0));
  int _x = ((int)  tolua_tonumber(tolua_S,4,0));
  int _y = ((int)  tolua_tonumber(tolua_S,5,0));
  const char* _pszText = ((const char*)  tolua_tostring(tolua_S,6,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddCommonButton'", NULL);
#endif
  {
   self->AddCommonButton(_type,_id,_x,_y,_pszText);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddCommonButton'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: RemoveCommonButton of class  GameCommonDlg */
#ifndef TOLUA_DISABLE_tolua_BackMirClient_GameCommonDlg_RemoveCommonButton00
static int tolua_BackMirClient_GameCommonDlg_RemoveCommonButton00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GameCommonDlg",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameCommonDlg* self = (GameCommonDlg*)  tolua_tousertype(tolua_S,1,0);
  int _id = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'RemoveCommonButton'", NULL);
#endif
  {
   self->RemoveCommonButton(_id);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'RemoveCommonButton'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: MoveTo of class  GameCommonDlg */
#ifndef TOLUA_DISABLE_tolua_BackMirClient_GameCommonDlg_MoveTo00
static int tolua_BackMirClient_GameCommonDlg_MoveTo00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GameCommonDlg",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameCommonDlg* self = (GameCommonDlg*)  tolua_tousertype(tolua_S,1,0);
  int _x = ((int)  tolua_tonumber(tolua_S,2,0));
  int _y = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'MoveTo'", NULL);
#endif
  {
   self->MoveTo(_x,_y);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'MoveTo'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetWidth of class  GameCommonDlg */
#ifndef TOLUA_DISABLE_tolua_BackMirClient_GameCommonDlg_GetWidth00
static int tolua_BackMirClient_GameCommonDlg_GetWidth00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GameCommonDlg",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameCommonDlg* self = (GameCommonDlg*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetWidth'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetWidth();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetWidth'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: GetHeight of class  GameCommonDlg */
#ifndef TOLUA_DISABLE_tolua_BackMirClient_GameCommonDlg_GetHeight00
static int tolua_BackMirClient_GameCommonDlg_GetHeight00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GameCommonDlg",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameCommonDlg* self = (GameCommonDlg*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'GetHeight'", NULL);
#endif
  {
   int tolua_ret = (int)  self->GetHeight();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetHeight'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetHotKey of class  GameCommonDlg */
#ifndef TOLUA_DISABLE_tolua_BackMirClient_GameCommonDlg_SetHotKey00
static int tolua_BackMirClient_GameCommonDlg_SetHotKey00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GameCommonDlg",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameCommonDlg* self = (GameCommonDlg*)  tolua_tousertype(tolua_S,1,0);
  int _nHotKey = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetHotKey'", NULL);
#endif
  {
   self->SetHotKey(_nHotKey);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetHotKey'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetWndRect of class  GameCommonDlg */
#ifndef TOLUA_DISABLE_tolua_BackMirClient_GameCommonDlg_SetWndRect00
static int tolua_BackMirClient_GameCommonDlg_SetWndRect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GameCommonDlg",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameCommonDlg* self = (GameCommonDlg*)  tolua_tousertype(tolua_S,1,0);
  int _nLeft = ((int)  tolua_tonumber(tolua_S,2,0));
  int _nTop = ((int)  tolua_tonumber(tolua_S,3,0));
  int _nWidth = ((int)  tolua_tonumber(tolua_S,4,0));
  int _nHeight = ((int)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetWndRect'", NULL);
#endif
  {
   self->SetWndRect(_nLeft,_nTop,_nWidth,_nHeight);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetWndRect'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetWindowTitle of class  GameNewDlg */
#ifndef TOLUA_DISABLE_tolua_BackMirClient_GameNewDlg_SetWindowTitle00
static int tolua_BackMirClient_GameNewDlg_SetWindowTitle00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GameNewDlg",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameNewDlg* self = (GameNewDlg*)  tolua_tousertype(tolua_S,1,0);
  const char* _pszTitle = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetWindowTitle'", NULL);
#endif
  {
   self->SetWindowTitle(_pszTitle);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetWindowTitle'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetWindowInfo of class  GameNewDlg */
#ifndef TOLUA_DISABLE_tolua_BackMirClient_GameNewDlg_SetWindowInfo00
static int tolua_BackMirClient_GameNewDlg_SetWindowInfo00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GameNewDlg",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameNewDlg* self = (GameNewDlg*)  tolua_tousertype(tolua_S,1,0);
  const char* _pszInfo = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetWindowInfo'", NULL);
#endif
  {
   self->SetWindowInfo(_pszInfo);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetWindowInfo'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetFixed of class  GameNewDlg */
#ifndef TOLUA_DISABLE_tolua_BackMirClient_GameNewDlg_SetFixed00
static int tolua_BackMirClient_GameNewDlg_SetFixed00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GameNewDlg",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameNewDlg* self = (GameNewDlg*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetFixed'", NULL);
#endif
  {
   self->SetFixed();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetFixed'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetCustomBk of class  GameNewDlg */
#ifndef TOLUA_DISABLE_tolua_BackMirClient_GameNewDlg_SetCustomBk00
static int tolua_BackMirClient_GameNewDlg_SetCustomBk00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GameNewDlg",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameNewDlg* self = (GameNewDlg*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetCustomBk'", NULL);
#endif
  {
   self->SetCustomBk();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetCustomBk'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AddRenderTexture of class  GameScriptWnd */
#ifndef TOLUA_DISABLE_tolua_BackMirClient_GameScriptWnd_AddRenderTexture00
static int tolua_BackMirClient_GameScriptWnd_AddRenderTexture00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GameScriptWnd",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,1,&tolua_err) ||
     !tolua_isnumber(tolua_S,7,1,&tolua_err) ||
     !tolua_isnumber(tolua_S,8,1,&tolua_err) ||
     !tolua_isnumber(tolua_S,9,1,&tolua_err) ||
     !tolua_isnumber(tolua_S,10,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,11,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameScriptWnd* self = (GameScriptWnd*)  tolua_tousertype(tolua_S,1,0);
  const char* _pszFileName = ((const char*)  tolua_tostring(tolua_S,2,0));
  int _nTextureID = ((int)  tolua_tonumber(tolua_S,3,0));
  int _nX = ((int)  tolua_tonumber(tolua_S,4,0));
  int _nY = ((int)  tolua_tonumber(tolua_S,5,0));
  int _nLeft = ((int)  tolua_tonumber(tolua_S,6,0));
  int _nTop = ((int)  tolua_tonumber(tolua_S,7,0));
  int _nWidth = ((int)  tolua_tonumber(tolua_S,8,0));
  int _nHeight = ((int)  tolua_tonumber(tolua_S,9,0));
  unsigned int _uColor = ((unsigned int)  tolua_tonumber(tolua_S,10,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddRenderTexture'", NULL);
#endif
  {
   int tolua_ret = (int)  self->AddRenderTexture(_pszFileName,_nTextureID,_nX,_nY,_nLeft,_nTop,_nWidth,_nHeight,_uColor);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddRenderTexture'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: AddRenderString of class  GameScriptWnd */
#ifndef TOLUA_DISABLE_tolua_BackMirClient_GameScriptWnd_AddRenderString00
static int tolua_BackMirClient_GameScriptWnd_AddRenderString00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GameScriptWnd",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameScriptWnd* self = (GameScriptWnd*)  tolua_tousertype(tolua_S,1,0);
  const char* _pszString = ((const char*)  tolua_tostring(tolua_S,2,0));
  int _nLeft = ((int)  tolua_tonumber(tolua_S,3,0));
  int _nTop = ((int)  tolua_tonumber(tolua_S,4,0));
  unsigned int _uColor = ((unsigned int)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'AddRenderString'", NULL);
#endif
  {
   int tolua_ret = (int)  self->AddRenderString(_pszString,_nLeft,_nTop,_uColor);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'AddRenderString'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: RemoveRenderTexture of class  GameScriptWnd */
#ifndef TOLUA_DISABLE_tolua_BackMirClient_GameScriptWnd_RemoveRenderTexture00
static int tolua_BackMirClient_GameScriptWnd_RemoveRenderTexture00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GameScriptWnd",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameScriptWnd* self = (GameScriptWnd*)  tolua_tousertype(tolua_S,1,0);
  int _nID = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'RemoveRenderTexture'", NULL);
#endif
  {
   self->RemoveRenderTexture(_nID);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'RemoveRenderTexture'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: RemoveRenderString of class  GameScriptWnd */
#ifndef TOLUA_DISABLE_tolua_BackMirClient_GameScriptWnd_RemoveRenderString00
static int tolua_BackMirClient_GameScriptWnd_RemoveRenderString00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GameScriptWnd",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameScriptWnd* self = (GameScriptWnd*)  tolua_tousertype(tolua_S,1,0);
  int _nID = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'RemoveRenderString'", NULL);
#endif
  {
   self->RemoveRenderString(_nID);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'RemoveRenderString'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetRenderTexture_Texture of class  GameScriptWnd */
#ifndef TOLUA_DISABLE_tolua_BackMirClient_GameScriptWnd_SetRenderTexture_Texture00
static int tolua_BackMirClient_GameScriptWnd_SetRenderTexture_Texture00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GameScriptWnd",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameScriptWnd* self = (GameScriptWnd*)  tolua_tousertype(tolua_S,1,0);
  int _nID = ((int)  tolua_tonumber(tolua_S,2,0));
  const char* _pszFileName = ((const char*)  tolua_tostring(tolua_S,3,0));
  int _nTextureID = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetRenderTexture_Texture'", NULL);
#endif
  {
   self->SetRenderTexture_Texture(_nID,_pszFileName,_nTextureID);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetRenderTexture_Texture'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetRenderTexture_Rect of class  GameScriptWnd */
#ifndef TOLUA_DISABLE_tolua_BackMirClient_GameScriptWnd_SetRenderTexture_Rect00
static int tolua_BackMirClient_GameScriptWnd_SetRenderTexture_Rect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GameScriptWnd",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,1,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,1,&tolua_err) ||
     !tolua_isnoobj(tolua_S,7,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameScriptWnd* self = (GameScriptWnd*)  tolua_tousertype(tolua_S,1,0);
  int _nID = ((int)  tolua_tonumber(tolua_S,2,0));
  int _nLeft = ((int)  tolua_tonumber(tolua_S,3,0));
  int _nTop = ((int)  tolua_tonumber(tolua_S,4,0));
  int _nWidth = ((int)  tolua_tonumber(tolua_S,5,0));
  int _nHeight = ((int)  tolua_tonumber(tolua_S,6,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetRenderTexture_Rect'", NULL);
#endif
  {
   self->SetRenderTexture_Rect(_nID,_nLeft,_nTop,_nWidth,_nHeight);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetRenderTexture_Rect'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetRenderTexture_Position of class  GameScriptWnd */
#ifndef TOLUA_DISABLE_tolua_BackMirClient_GameScriptWnd_SetRenderTexture_Position00
static int tolua_BackMirClient_GameScriptWnd_SetRenderTexture_Position00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GameScriptWnd",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameScriptWnd* self = (GameScriptWnd*)  tolua_tousertype(tolua_S,1,0);
  int _nID = ((int)  tolua_tonumber(tolua_S,2,0));
  int _nX = ((int)  tolua_tonumber(tolua_S,3,0));
  int _nY = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetRenderTexture_Position'", NULL);
#endif
  {
   self->SetRenderTexture_Position(_nID,_nX,_nY);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetRenderTexture_Position'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetRenderTexture_Color of class  GameScriptWnd */
#ifndef TOLUA_DISABLE_tolua_BackMirClient_GameScriptWnd_SetRenderTexture_Color00
static int tolua_BackMirClient_GameScriptWnd_SetRenderTexture_Color00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GameScriptWnd",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameScriptWnd* self = (GameScriptWnd*)  tolua_tousertype(tolua_S,1,0);
  int _nID = ((int)  tolua_tonumber(tolua_S,2,0));
  unsigned int _uColor = ((unsigned int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetRenderTexture_Color'", NULL);
#endif
  {
   self->SetRenderTexture_Color(_nID,_uColor);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetRenderTexture_Color'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetRenderString_String of class  GameScriptWnd */
#ifndef TOLUA_DISABLE_tolua_BackMirClient_GameScriptWnd_SetRenderString_String00
static int tolua_BackMirClient_GameScriptWnd_SetRenderString_String00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GameScriptWnd",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameScriptWnd* self = (GameScriptWnd*)  tolua_tousertype(tolua_S,1,0);
  int _nID = ((int)  tolua_tonumber(tolua_S,2,0));
  const char* _pszString = ((const char*)  tolua_tostring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetRenderString_String'", NULL);
#endif
  {
   self->SetRenderString_String(_nID,_pszString);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetRenderString_String'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetRenderString_Position of class  GameScriptWnd */
#ifndef TOLUA_DISABLE_tolua_BackMirClient_GameScriptWnd_SetRenderString_Position00
static int tolua_BackMirClient_GameScriptWnd_SetRenderString_Position00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GameScriptWnd",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameScriptWnd* self = (GameScriptWnd*)  tolua_tousertype(tolua_S,1,0);
  int _nID = ((int)  tolua_tonumber(tolua_S,2,0));
  int _nX = ((int)  tolua_tonumber(tolua_S,3,0));
  int _nY = ((int)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetRenderString_Position'", NULL);
#endif
  {
   self->SetRenderString_Position(_nID,_nX,_nY);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetRenderString_Position'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetRenderString_Color of class  GameScriptWnd */
#ifndef TOLUA_DISABLE_tolua_BackMirClient_GameScriptWnd_SetRenderString_Color00
static int tolua_BackMirClient_GameScriptWnd_SetRenderString_Color00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GameScriptWnd",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameScriptWnd* self = (GameScriptWnd*)  tolua_tousertype(tolua_S,1,0);
  int _nID = ((int)  tolua_tonumber(tolua_S,2,0));
  unsigned int _uColor = ((unsigned int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetRenderString_Color'", NULL);
#endif
  {
   self->SetRenderString_Color(_nID,_uColor);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetRenderString_Color'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetRenderTexture_Visible of class  GameScriptWnd */
#ifndef TOLUA_DISABLE_tolua_BackMirClient_GameScriptWnd_SetRenderTexture_Visible00
static int tolua_BackMirClient_GameScriptWnd_SetRenderTexture_Visible00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GameScriptWnd",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameScriptWnd* self = (GameScriptWnd*)  tolua_tousertype(tolua_S,1,0);
  int _nID = ((int)  tolua_tonumber(tolua_S,2,0));
  bool _bVisible = ((bool)  tolua_toboolean(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetRenderTexture_Visible'", NULL);
#endif
  {
   self->SetRenderTexture_Visible(_nID,_bVisible);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetRenderTexture_Visible'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: SetRenderString_Visible of class  GameScriptWnd */
#ifndef TOLUA_DISABLE_tolua_BackMirClient_GameScriptWnd_SetRenderString_Visible00
static int tolua_BackMirClient_GameScriptWnd_SetRenderString_Visible00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GameScriptWnd",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isboolean(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameScriptWnd* self = (GameScriptWnd*)  tolua_tousertype(tolua_S,1,0);
  int _nID = ((int)  tolua_tonumber(tolua_S,2,0));
  bool _bVisible = ((bool)  tolua_toboolean(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'SetRenderString_Visible'", NULL);
#endif
  {
   self->SetRenderString_Visible(_nID,_bVisible);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'SetRenderString_Visible'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: RemoveAllRenderTexture of class  GameScriptWnd */
#ifndef TOLUA_DISABLE_tolua_BackMirClient_GameScriptWnd_RemoveAllRenderTexture00
static int tolua_BackMirClient_GameScriptWnd_RemoveAllRenderTexture00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GameScriptWnd",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameScriptWnd* self = (GameScriptWnd*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'RemoveAllRenderTexture'", NULL);
#endif
  {
   self->RemoveAllRenderTexture();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'RemoveAllRenderTexture'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: RemoveAllRenderString of class  GameScriptWnd */
#ifndef TOLUA_DISABLE_tolua_BackMirClient_GameScriptWnd_RemoveAllRenderString00
static int tolua_BackMirClient_GameScriptWnd_RemoveAllRenderString00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GameScriptWnd",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameScriptWnd* self = (GameScriptWnd*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'RemoveAllRenderString'", NULL);
#endif
  {
   self->RemoveAllRenderString();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'RemoveAllRenderString'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: RegisterOnButtonClick of class  GameScriptWnd */
#ifndef TOLUA_DISABLE_tolua_BackMirClient_GameScriptWnd_RegisterOnButtonClick00
static int tolua_BackMirClient_GameScriptWnd_RegisterOnButtonClick00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GameScriptWnd",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameScriptWnd* self = (GameScriptWnd*)  tolua_tousertype(tolua_S,1,0);
  const char* _pszFuncName = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'RegisterOnButtonClick'", NULL);
#endif
  {
   self->RegisterOnButtonClick(_pszFuncName);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'RegisterOnButtonClick'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: RegisterOnShowWindow of class  GameScriptWnd */
#ifndef TOLUA_DISABLE_tolua_BackMirClient_GameScriptWnd_RegisterOnShowWindow00
static int tolua_BackMirClient_GameScriptWnd_RegisterOnShowWindow00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GameScriptWnd",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameScriptWnd* self = (GameScriptWnd*)  tolua_tousertype(tolua_S,1,0);
  const char* _pszFuncName = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'RegisterOnShowWindow'", NULL);
#endif
  {
   self->RegisterOnShowWindow(_pszFuncName);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'RegisterOnShowWindow'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: RegisterOnCloseWindow of class  GameScriptWnd */
#ifndef TOLUA_DISABLE_tolua_BackMirClient_GameScriptWnd_RegisterOnCloseWindow00
static int tolua_BackMirClient_GameScriptWnd_RegisterOnCloseWindow00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"GameScriptWnd",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  GameScriptWnd* self = (GameScriptWnd*)  tolua_tousertype(tolua_S,1,0);
  const char* _pszFuncName = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'RegisterOnCloseWindow'", NULL);
#endif
  {
   self->RegisterOnCloseWindow(_pszFuncName);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'RegisterOnCloseWindow'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_BackMirClient_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_cclass(tolua_S,"InteractiveContextEx","InteractiveContextEx","",NULL);
  tolua_beginmodule(tolua_S,"InteractiveContextEx");
   tolua_function(tolua_S,"AddContextNode",tolua_BackMirClient_InteractiveContextEx_AddContextNode00);
   tolua_function(tolua_S,"Clear",tolua_BackMirClient_InteractiveContextEx_Clear00);
  tolua_endmodule(tolua_S);
  tolua_function(tolua_S,"MirLog",tolua_BackMirClient_MirLog00);
  tolua_cclass(tolua_S,"QuestContext","QuestContext","",NULL);
  tolua_beginmodule(tolua_S,"QuestContext");
   tolua_function(tolua_S,"IsQuestComplete",tolua_BackMirClient_QuestContext_IsQuestComplete00);
   tolua_function(tolua_S,"SetQuestComplete",tolua_BackMirClient_QuestContext_SetQuestComplete00);
   tolua_function(tolua_S,"IsQuestAccept",tolua_BackMirClient_QuestContext_IsQuestAccept00);
   tolua_function(tolua_S,"GetQuestStep",tolua_BackMirClient_QuestContext_GetQuestStep00);
   tolua_function(tolua_S,"SetQuestStep",tolua_BackMirClient_QuestContext_SetQuestStep00);
   tolua_function(tolua_S,"GetQuestCounter",tolua_BackMirClient_QuestContext_GetQuestCounter00);
   tolua_function(tolua_S,"SetQuestCounter",tolua_BackMirClient_QuestContext_SetQuestCounter00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"QuestSummary","QuestSummary","",NULL);
  tolua_beginmodule(tolua_S,"QuestSummary");
   tolua_variable(tolua_S,"xQuestName",tolua_get_QuestSummary_xQuestName,tolua_set_QuestSummary_xQuestName);
   tolua_variable(tolua_S,"nType",tolua_get_QuestSummary_nType,tolua_set_QuestSummary_nType);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"QuestTip","QuestTip","",NULL);
  tolua_beginmodule(tolua_S,"QuestTip");
   tolua_variable(tolua_S,"xDescript",tolua_get_QuestTip_xDescript,tolua_set_QuestTip_xDescript);
   tolua_variable(tolua_S,"xRequire",tolua_get_QuestTip_xRequire,tolua_set_QuestTip_xRequire);
   tolua_variable(tolua_S,"xReward",tolua_get_QuestTip_xReward,tolua_set_QuestTip_xReward);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"QuestInfo","QuestInfo","",NULL);
  tolua_beginmodule(tolua_S,"QuestInfo");
   tolua_variable(tolua_S,"stSummary",tolua_get_QuestInfo_stSummary,tolua_set_QuestInfo_stSummary);
   tolua_variable(tolua_S,"stTip",tolua_get_QuestInfo_stTip,tolua_set_QuestInfo_stTip);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"GameQuestDlg","GameQuestDlg","GameNewDlg",NULL);
  tolua_beginmodule(tolua_S,"GameQuestDlg");
   tolua_function(tolua_S,"PushQuestInfo",tolua_BackMirClient_GameQuestDlg_PushQuestInfo00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"GameOtherPlayer","GameOtherPlayer","",NULL);
  tolua_beginmodule(tolua_S,"GameOtherPlayer");
   tolua_function(tolua_S,"GetHeroJob",tolua_BackMirClient_GameOtherPlayer_GetHeroJob00);
   tolua_function(tolua_S,"GetHeroLevel",tolua_BackMirClient_GameOtherPlayer_GetHeroLevel00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"GamePlayer","GamePlayer","GameOtherPlayer",NULL);
  tolua_beginmodule(tolua_S,"GamePlayer");
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"MirGame","MirGame","",NULL);
  tolua_beginmodule(tolua_S,"MirGame");
   tolua_function(tolua_S,"GetInstance",tolua_BackMirClient_MirGame_GetInstance00);
   tolua_function(tolua_S,"SetDarkMode",tolua_BackMirClient_MirGame_SetDarkMode00);
   tolua_function(tolua_S,"SetShowMapSnap",tolua_BackMirClient_MirGame_SetShowMapSnap00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"RenderObject","RenderObject","",NULL);
  tolua_beginmodule(tolua_S,"RenderObject");
   tolua_function(tolua_S,"SetVisible",tolua_BackMirClient_RenderObject_SetVisible00);
   tolua_function(tolua_S,"IsVisible",tolua_BackMirClient_RenderObject_IsVisible00);
   tolua_function(tolua_S,"GetIDInt",tolua_BackMirClient_RenderObject_GetIDInt00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"GameCommonDlg","GameCommonDlg","RenderObject",NULL);
  tolua_beginmodule(tolua_S,"GameCommonDlg");
   tolua_function(tolua_S,"AddCommonButton",tolua_BackMirClient_GameCommonDlg_AddCommonButton00);
   tolua_function(tolua_S,"RemoveCommonButton",tolua_BackMirClient_GameCommonDlg_RemoveCommonButton00);
   tolua_function(tolua_S,"MoveTo",tolua_BackMirClient_GameCommonDlg_MoveTo00);
   tolua_function(tolua_S,"GetWidth",tolua_BackMirClient_GameCommonDlg_GetWidth00);
   tolua_function(tolua_S,"GetHeight",tolua_BackMirClient_GameCommonDlg_GetHeight00);
   tolua_function(tolua_S,"SetHotKey",tolua_BackMirClient_GameCommonDlg_SetHotKey00);
   tolua_function(tolua_S,"SetWndRect",tolua_BackMirClient_GameCommonDlg_SetWndRect00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"GameNewDlg","GameNewDlg","GameCommonDlg",NULL);
  tolua_beginmodule(tolua_S,"GameNewDlg");
   tolua_function(tolua_S,"SetWindowTitle",tolua_BackMirClient_GameNewDlg_SetWindowTitle00);
   tolua_function(tolua_S,"SetWindowInfo",tolua_BackMirClient_GameNewDlg_SetWindowInfo00);
   tolua_function(tolua_S,"SetFixed",tolua_BackMirClient_GameNewDlg_SetFixed00);
   tolua_function(tolua_S,"SetCustomBk",tolua_BackMirClient_GameNewDlg_SetCustomBk00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"GameScriptWnd","GameScriptWnd","GameNewDlg",NULL);
  tolua_beginmodule(tolua_S,"GameScriptWnd");
   tolua_function(tolua_S,"AddRenderTexture",tolua_BackMirClient_GameScriptWnd_AddRenderTexture00);
   tolua_function(tolua_S,"AddRenderString",tolua_BackMirClient_GameScriptWnd_AddRenderString00);
   tolua_function(tolua_S,"RemoveRenderTexture",tolua_BackMirClient_GameScriptWnd_RemoveRenderTexture00);
   tolua_function(tolua_S,"RemoveRenderString",tolua_BackMirClient_GameScriptWnd_RemoveRenderString00);
   tolua_function(tolua_S,"SetRenderTexture_Texture",tolua_BackMirClient_GameScriptWnd_SetRenderTexture_Texture00);
   tolua_function(tolua_S,"SetRenderTexture_Rect",tolua_BackMirClient_GameScriptWnd_SetRenderTexture_Rect00);
   tolua_function(tolua_S,"SetRenderTexture_Position",tolua_BackMirClient_GameScriptWnd_SetRenderTexture_Position00);
   tolua_function(tolua_S,"SetRenderTexture_Color",tolua_BackMirClient_GameScriptWnd_SetRenderTexture_Color00);
   tolua_function(tolua_S,"SetRenderString_String",tolua_BackMirClient_GameScriptWnd_SetRenderString_String00);
   tolua_function(tolua_S,"SetRenderString_Position",tolua_BackMirClient_GameScriptWnd_SetRenderString_Position00);
   tolua_function(tolua_S,"SetRenderString_Color",tolua_BackMirClient_GameScriptWnd_SetRenderString_Color00);
   tolua_function(tolua_S,"SetRenderTexture_Visible",tolua_BackMirClient_GameScriptWnd_SetRenderTexture_Visible00);
   tolua_function(tolua_S,"SetRenderString_Visible",tolua_BackMirClient_GameScriptWnd_SetRenderString_Visible00);
   tolua_function(tolua_S,"RemoveAllRenderTexture",tolua_BackMirClient_GameScriptWnd_RemoveAllRenderTexture00);
   tolua_function(tolua_S,"RemoveAllRenderString",tolua_BackMirClient_GameScriptWnd_RemoveAllRenderString00);
   tolua_function(tolua_S,"RegisterOnButtonClick",tolua_BackMirClient_GameScriptWnd_RegisterOnButtonClick00);
   tolua_function(tolua_S,"RegisterOnShowWindow",tolua_BackMirClient_GameScriptWnd_RegisterOnShowWindow00);
   tolua_function(tolua_S,"RegisterOnCloseWindow",tolua_BackMirClient_GameScriptWnd_RegisterOnCloseWindow00);
  tolua_endmodule(tolua_S);
  tolua_constant(tolua_S,"kLuaEvent_SwitchScene",kLuaEvent_SwitchScene);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_BackMirClient (lua_State* tolua_S) {
 return tolua_BackMirClient_open(tolua_S);
};
#endif

