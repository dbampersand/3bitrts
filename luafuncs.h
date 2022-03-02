#include "lua-5.4.3/src/lua.h"
#include "lua-5.4.3/src/lauxlib.h"
#include "lua-5.4.3/src/lualib.h"

typedef struct GameObject GameObject;
typedef struct Ability Ability;
lua_State* luaState;
GameObject* currGameObjRunning;
Ability* currAbilityRunning;
void CallLuaFunc(int funcID);
void init_lua();
void LoadLuaFile(const char* filename, GameObject* g);
void SetLuaFuncs();
