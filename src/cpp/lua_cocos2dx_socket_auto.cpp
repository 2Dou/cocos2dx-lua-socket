#include "lua_cocos2dx_socket_auto.hpp"
#include "cocos2d_socket.hpp"
#include "scripting/lua-bindings/manual/tolua_fix.h"
#include "scripting/lua-bindings/manual/LuaBasicConversions.h"

int lua_cocos2dx_socket_Cocos2dSocket_constructor(lua_State* tolua_S)
{
    int argc = 0;
    Cocos2dSocket* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1 || COCOS2D_BINDING >= 1
    tolua_Error tolua_err;
#endif



    argc = lua_gettop(tolua_S)-1;
    if (argc == 1)
    {
        cocos2d::LUA_FUNCTION arg0;

        arg0 = toluafix_ref_function(tolua_S, 2, 0); ok &= arg0>0;
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_cocos2dx_socket_Cocos2dSocket_constructor'", nullptr);
            return 0;
        }
        cobj = new Cocos2dSocket(arg0);
        cobj->autorelease();
        int ID =  (int)cobj->_ID ;
        int* luaID =  &cobj->_luaID ;
        toluafix_pushusertype_ccobject(tolua_S, ID, luaID, (void*)cobj,"Cocos2dSocket");
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "Cocos2dSocket:Cocos2dSocket",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1 || COCOS2D_BINDING >= 1
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_socket_Cocos2dSocket_constructor'.",&tolua_err);
#endif

    return 0;
}

static int lua_cocos2dx_socket_Cocos2dSocket_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (Cocos2dSocket)");
    return 0;
}

int lua_register_cocos2dx_socket_Cocos2dSocket(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"Cocos2dSocket");
    tolua_cclass(tolua_S,"Cocos2dSocket","Cocos2dSocket","cc.Ref",nullptr);

    tolua_beginmodule(tolua_S,"Cocos2dSocket");
        tolua_function(tolua_S,"new",lua_cocos2dx_socket_Cocos2dSocket_constructor);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(Cocos2dSocket).name();
    g_luaType[typeName] = "Cocos2dSocket";
    g_typeCast["Cocos2dSocket"] = "Cocos2dSocket";
    return 1;
}
TOLUA_API int register_all_cocos2dx_socket(lua_State* tolua_S)
{
	tolua_open(tolua_S);

	tolua_module(tolua_S,"sock",0);
	tolua_beginmodule(tolua_S,"sock");

	lua_register_cocos2dx_socket_Cocos2dSocket(tolua_S);

	tolua_endmodule(tolua_S);
	return 1;
}

