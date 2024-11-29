/**
 *  Copyright (C) 2021 Masatoshi Fukunaga
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to
 *  deal in the Software without restriction, including without limitation the
 *  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 *  sell copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 *  IN THE SOFTWARE.
 */

#ifndef lua_error_h
#define lua_error_h

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
// lua
#include <lauxhlib.h>
#include <lua.h>

#define LUA_ERROR_API            static inline
#define LUA_ERROR_API_DEPRECATED static inline __attribute__((deprecated))

LUA_ERROR_API void lua_error_dostring(lua_State *L, const char *str, int argidx,
                                      int nresults)
{
    int top  = lua_gettop(L);
    int narg = 0;

    if (argidx < 0) {
        // converts relative index to absolute index
        argidx = top + argidx + 1;
    }

    if (argidx > 0) {
        // confirms that the argument exists at the specified index
        luaL_checkany(L, argidx);
        // calculates the number of arguments
        narg = top - argidx + 1;
    }

    // evaluates the string as a Lua code
    if (luaL_loadstring(L, str)) {
        lua_error(L);
    } else if (narg) {
        // move function to the bottom of the first argument
        lua_insert(L, argidx);
    }
    lua_call(L, narg, nresults);
}

LUA_ERROR_API void lua_error_loadlib(lua_State *L, int level)
{
    (void)level;
    lua_error_dostring(L, "require('error')", 0, 0);
}

/**
 * create a new structured message that equivalent to the following code;
 *
 *  error.message.new(message [, op]])
 *
 * push all arguments (including nil) onto the stack and call the API with
 * the msg index.
 * the last argument must be placed at the top of the stack.
 * this function removes all arguments from the stack.
 */
LUA_ERROR_API int lua_error_new_message(lua_State *L, int msgidx)
{
    // create message
    lua_error_dostring(L, "return require('error.message').new(...)", msgidx,
                       1);
    return 1;
}

/**
 * create a new error that equivalent to the following code;
 *
 *  error.new(msg [, wrap [, level [, traceback]]])
 *
 * push all arguments (including nil) onto the stack and call the API with
 * the msg index.
 * the last argument must be placed at the top of the stack.
 * this function removes all arguments from the stack.
 */
LUA_ERROR_API int lua_error_new(lua_State *L, int msgidx)
{
    // create new error
    lua_error_dostring(L, "return require('error').new(...)", msgidx, 1);
    return 1;
}

/**
 * create a new error that equivalent to the following code;
 *
 *  error.format(fmt [, ... [, wrap [, level [, traceback]]]])
 *
 * push all arguments (including nil) onto the stack and call the API with
 * the fmt index.
 * the last argument must be placed at the top of the stack.
 * this function removes all arguments from the stack.
 */
LUA_ERROR_API int lua_error_format(lua_State *L, int fmtidx)
{
    // create error with formatted message
    lua_error_dostring(L, "return require('error').format(...)", fmtidx, 1);
    return 1;
}

/**
 * delete a error type from registry that equivalent to the following code;
 *
 *  ok = error.type.del(name)
 */
LUA_ERROR_API int lua_error_registry_del(lua_State *L, const char *name)
{
    int ok = 0;

    lua_pushstring(L, name);
    lua_error_dostring(L, "return require('error.type').del(...)", 1, 1);
    ok = lua_toboolean(L, -1);
    lua_pop(L, 1);
    return ok;
}

/**
 * get a error type object from registry that equivalent to the following code;
 *
 *  t = error.type.get(name)
 */
LUA_ERROR_API int lua_error_registry_get(lua_State *L, const char *name)
{
    lua_pushstring(L, name);
    lua_error_dostring(L, "return require('error.type').get(...)", 1, 1);
    if (lua_isnil(L, -1)) {
        lua_pop(L, 1);
        return 0;
    }
    return 1;
}

/**
 * create a new error type that equivalent to the following code;
 *
 *  error.type.new(name [, code [, message]])
 *
 * push name arguments onto the stack and call the API with the name index.
 * the last argument must be placed at the top of the stack.
 * this function removes all arguments from the stack.
 */
LUA_ERROR_API int lua_error_new_type(lua_State *L, int nameidx)
{
    // create a new error type
    lua_error_dostring(L, "return require('error.type').new(...)", nameidx, 1);
    return 1;
}

/**
 * create a new typed error that equivalent to the following code;
 *
 *  <myerr>:new([message [, wrap [, level [, traceback]]]])
 *
 * push all arguments (including nil) onto the stack and call the API with
 * the type index.
 * the last argument must be placed at the top of the stack.
 * this function removes all arguments from the stack.
 */
LUA_ERROR_API int lua_error_new_typed_error(lua_State *L, int typeidx)
{
    // create a new typed error
    lua_error_dostring(L, "local t = ...; return t:new(select(2, ...))",
                       typeidx, 1);
    return 1;
}

#endif
