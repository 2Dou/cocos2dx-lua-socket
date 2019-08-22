// Copyright @2Dou@github.com

#include "lua_cocos2dx_socket_manual.hpp"
#include "lua_cocos2dx_socket_auto.hpp"
#include "cocos2d_socket.hpp"
#include "scripting/lua-bindings/manual/LuaBasicConversions.h"

int lua_cocos2dx_socket_Cocos2dSocket_ConnectHost(lua_State* tolua_S)
{
    int argc = 0;
    Cocos2dSocket* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1 || COCOS2D_BINDING >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1 || COCOS2D_BINDING >= 1
    if (!tolua_isusertype(tolua_S,1,"Cocos2dSocket",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (Cocos2dSocket*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1 || COCOS2D_BINDING >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_cocos2dx_socket_Cocos2dSocket_ConnectHost'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2 || argc == 3)
    {
        std::string arg0;
        int arg1, arg2;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "Cocos2dSocket:ConnectHost");
        ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1, "Cocos2dSocket:ConnectHost");

        if (argc == 3) {
            ok &= luaval_to_int32(tolua_S, 4,(int *)&arg2, "Cocos2dSocket:ConnectHost");
        }

        if(!ok) {
            tolua_error(tolua_S,"invalid arguments in function 'lua_cocos2dx_socket_Cocos2dSocket_ConnectHost'", nullptr);
            return 0;
        }

        if (argc == 2) {
            cobj->ConnectHost(arg0, arg1);
        } else {
            cobj->ConnectHost(arg0, arg1, arg2);
        }
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "Cocos2dSocket:ConnectHost",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1 || COCOS2D_BINDING >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_socket_Cocos2dSocket_ConnectHost'.",&tolua_err);
#endif

    return 0;
}

int lua_cocos2dx_socket_Cocos2dSocket_ConnectIP(lua_State* tolua_S)
{
    int argc = 0;
    Cocos2dSocket* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1 || COCOS2D_BINDING >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1 || COCOS2D_BINDING >= 1
    if (!tolua_isusertype(tolua_S,1,"Cocos2dSocket",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (Cocos2dSocket*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1 || COCOS2D_BINDING >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_cocos2dx_socket_Cocos2dSocket_ConnectIP'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2 || argc == 3)
    {
        std::string arg0;
        int arg1, arg2;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "Cocos2dSocket:ConnectIP");
        ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1, "Cocos2dSocket:ConnectIP");
        if (argc == 3) {
            ok &= luaval_to_int32(tolua_S, 4,(int *)&arg2, "Cocos2dSocket:ConnectIP");
        }
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_cocos2dx_socket_Cocos2dSocket_ConnectIP'", nullptr);
            return 0;
        }

        if (argc == 2) {
            cobj->ConnectIP(arg0, arg1);
        } else {
            cobj->ConnectIP(arg0, arg1, arg2);
        }
        lua_settop(tolua_S, 1);
        return 1;
    }

    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "Cocos2dSocket:ConnectIP",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1 || COCOS2D_BINDING >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_socket_Cocos2dSocket_ConnectIP'.",&tolua_err);
#endif

    return 0;
}

int lua_cocos2dx_socket_Cocos2dSocket_Send(lua_State* tolua_S)
{
    int argc = 0;
    Cocos2dSocket* cobj = nullptr;

#if COCOS2D_DEBUG >= 1 || COCOS2D_BINDING >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1 || COCOS2D_BINDING >= 1
    if (!tolua_isusertype(tolua_S,1,"Cocos2dSocket",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (Cocos2dSocket*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1 || COCOS2D_BINDING >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_cocos2dx_socket_Cocos2dSocket_Send'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1)
    {
        size_t size;
        const char *data = luaL_checklstring(tolua_S, 2, &size);
        cobj->Send(data, (int)size);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "Cocos2dSocket:send",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1 || COCOS2D_BINDING >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_cocos2dx_socket_Cocos2dSocket_Send'.",&tolua_err);
#endif

    return 0;
}

int lua_register_cocos2dx_socket_Cocos2dSocket_manual(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"Cocos2dSocket");
    tolua_cclass(tolua_S,"Cocos2dSocket","Cocos2dSocket","cc.Ref",nullptr);

    tolua_beginmodule(tolua_S,"Cocos2dSocket");
    tolua_function(tolua_S,"Send",lua_cocos2dx_socket_Cocos2dSocket_Send);
    tolua_function(tolua_S,"ConnectHost",lua_cocos2dx_socket_Cocos2dSocket_ConnectHost);
    tolua_function(tolua_S,"ConnectIP",lua_cocos2dx_socket_Cocos2dSocket_ConnectIP);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(Cocos2dSocket).name();
    g_luaType[typeName] = "Cocos2dSocket";
    g_typeCast["Cocos2dSocket"] = "Cocos2dSocket";
    return 1;
}


TOLUA_API int register_all_cocos2dx_socket_manual(lua_State* tolua_S)
{
    lua_getglobal(tolua_S, "_G");
    if (lua_istable(tolua_S,-1)) {
        tolua_open(tolua_S);

        tolua_module(tolua_S,"sock",0);
        tolua_beginmodule(tolua_S,"sock");

        lua_register_cocos2dx_socket_Cocos2dSocket_manual(tolua_S);

        tolua_endmodule(tolua_S);

        register_all_cocos2dx_socket(tolua_S);
    }
    lua_pop(tolua_S, 1);
    return 1;
}
