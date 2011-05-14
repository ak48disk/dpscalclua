#include "Global.h"

class CPlayer;
class CCreature;
class CEvent;

extern bool SetEventSystem(CEventSystem* pES);
extern bool SetCurrentPlayer(CPlayer* pPlayer);
extern bool SetCurrentTarget(CCreature* pTarget);
extern bool SetCurrentEvent(CEvent*	pEvent);
extern bool RunLuaCode(const char* strCode);
extern bool Lua_Initialize(char* PreloadAdditionalLua);
extern bool Lua_CleanUp();
extern void CallLuaFunction(const char * strFunctionName,int nParam);

#define LUA_DUMMY_FUNCTION(x) extern "C" int LUAEXPORT L_##x##(lua_State* L){return 0;}
#define LUA_REGISTER(x) lua_register(pLuaState,#x,L_##x##)
#define LUA_TO_STRING(L,i,c) if (!lua_isstring(L,i)) {lua_pushstring(L,"Error parameter #"#i", string expected");lua_error(L);} else c##=lua_tostring(L,i)
#define LUA_TO_NUMBER(L,i,c) if (!lua_isnumber(L,i)) {lua_pushstring(L,"Error parameter #"#i", number expected");lua_error(L);} else c##=lua_tonumber(L,i)
#define LUA_TO_INTEGER(L,i,c) if (!lua_isnumber(L,i)) {lua_pushstring(L,"Error parameter #"#i", integer expected");lua_error(L);} else c##=lua_tointeger(L,i)
#define LUA_TO_BOOLEAN(L,i,c) if (lua_isnil(L,i)|| lua_isnone(L,i)) c=false; else if (!lua_isboolean(L,i))  {lua_pushstring(L,"Error parameter#"#i", boolean expected");lua_error(L);} else c##=lua_toboolean(L,i)
#define LUA_ERROR(L,str) lua_pushstring(L,str); lua_error(L)
