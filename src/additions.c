/* MIT License
 * 
 * Copyright (c) 2017-2018 Cody Tilkins
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#if defined(linux) || defined(__linux__) || defined(__linux)
#	include <unistd.h>
#	include <stdio.h>
#	include <stdlib.h>
#	define CLEAR_CONSOLE "clear"
#elif defined(unix) || defined(__unix__) || defined(__unix)
#	include <unistd.h>
#	include <stdio.h>
#	include <stdlib.h>
#	define CLEAR_CONSOLE "clear"
#elif defined(__APPLE__) || defined(__MACH__)
#	include <unistd.h>
#	include <stdio.h>
#	include <stdlib.h>
#	define CLEAR_CONSOLE "clear"
#elif defined(_WIN32) || defined(_WIN64)
#	include <windows.h>
#	include <stdio.h>
#	include <stdlib.h>
#	define CLEAR_CONSOLE "cls"
#else
#	error "Not familiar. Set up headers accordingly, or -D__linux__ of -Dunix or -D__APPLE__ or -D_WIN32"
#endif


#include <string.h>
#include <dirent.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>


// Please migrate your lua h's to a proper directory so versions don't collide
//   luajit make install puts them in /usr/local/include/luajit-X.X/*
//   lua51/52/53 puts them directly in /usr/local/include/* with version collision
#if defined(LUA_51)
#	include "lua51/lua.h"
#	include "lua51/lualib.h"
#	include "lua51/lauxlib.h"
#elif defined(LUA_52)
#	include "lua52/lua.h"
#	include "lua52/lualib.h"
#	include "lua52/lauxlib.h"
#elif defined(LUA_53)
#	include "lua53/lua.h"
#	include "lua53/lualib.h"
#	include "lua53/lauxlib.h"
#elif defined(LUA_JIT_51)
#	include "luajit-2.0/lua.h"
#	include "luajit-2.0/lualib.h"
#	include "luajit-2.0/lauxlib.h"
#	include "luajit-2.0/luajit.h"
#else
#	warning "Lua version not defined."
#	error "Define the version to use. '-DLUA_53' '-DLUA_52' '-DLUA_51' '-DLUA_JIT_51'"
#endif


#include "additions.h"


// string getcwd()
// returns current working directory as string
static int lua_cwd_getcwd(lua_State* L) {
	static char buffer[PATH_MAX];
	luaL_checkstack(L, 1, NULL);
	lua_pushstring(L, getcwd(buffer, PATH_MAX));	
	return 1;
}

// boolean setcwd(string)
// string [1] : directory to change to
// returns if changing directories was sucessful
static int lua_cwd_setcwd(lua_State* L) {
	luaL_checkstack(L, 1, NULL);
	const char* path = luaL_checkstring(L, 1);
	if(chdir(path) == 0)
		lua_pushboolean(L, 1);
	else lua_pushboolean(L, 0);
	return 1;
}

// array getdir(string, boolean)
// string [1] : directory to find listing of
// boolean [2] : should stat be used
// returns nil or error, or array with strings of all element names
static int lua_cwd_getdir(lua_State* L) {
	static DIR* dp;
	static struct dirent* ep;
	
	const char* path = luaL_checkstring(L, 1);
	
	luaL_checkstack(L, 1, NULL);
	
	dp = opendir(path);
	if(dp == NULL) {
		lua_pushnil(L);
		return 1;
	}
	
	size_t i = 1;
	lua_newtable(L);
	int top = lua_gettop(L);
	while((ep = readdir(dp)) != NULL) {
		lua_pushlstring(L, ep->d_name, strlen(ep->d_name));
		lua_rawseti(L, top, i++);
	}
	
	closedir(dp);
	return 1;
}

// void clear_window()
static int lua_clear_window(lua_State* L) {
	system(CLEAR_CONSOLE);
	return 0;
}

// void stack_dump(...)
// prints out anything left on the stack in a verbose way
static int stack_dump(lua_State *L) {
	int i = lua_gettop(L);
	printf("--------------- Stack Dump ----------------\n");
	while(i) {
		int t = lua_type(L, i);
		switch (t) {
		case LUA_TSTRING:
			fprintf(stdout, "%d:(String):`%s`\n", i, lua_tostring(L, i));
			break;
		case LUA_TBOOLEAN:
			fprintf(stdout, "%d:(Boolean):`%s`\n", i, lua_toboolean(L, i) ? "true" : "false");
			break;
		case LUA_TNUMBER:
			fprintf(stdout, "%d:(Number):`%g`\n", i, lua_tonumber(L, i));
			break;
		case LUA_TFUNCTION:
			fprintf(stdout, "%d:(Function):`@0x%p`\n", i, lua_topointer(L, i));
			break;
		case LUA_TTABLE:
			fprintf(stdout, "%d:(Table):`@0x%p`\n", i, lua_topointer(L, i));
			break;
		case LUA_TUSERDATA:
			fprintf(stdout, "%d:(Userdata):`@0x%p`\n", i, lua_topointer(L, i));
			break;
		case LUA_TLIGHTUSERDATA:
			fprintf(stdout, "%d:(LUserdata):`0x@%p`\n", i, lua_topointer(L, i));
			break;
		default:
			fprintf(stdout, "%d:(Object):%s:`0x@%p`\n", i, lua_typename(L, t), lua_topointer(L, i));
			break;
		}
		i--;
	}
	printf("----------- Stack Dump Finished -----------\n");
	return 0;
}

// number fstat_time(string)
// string [1] : directory or file to check mtime
// returns nil on error or the mtime of the file
static int lua_fstat_time(lua_State* L) {
	static struct stat st;
	const char* path = luaL_checkstring(L, 1);
	luaL_checkstack(L, 1, NULL);
	if(stat(path, &st) == -1) {
		lua_pushnil(L);
		return 1;
	}
	lua_pushnumber(L, st.st_mtime);
	return 1;
}



// sets up the environment
// stackdump
// io.mtime		io.dir
// os.setcwd	os.getcwd	os.clear
LUA_DLL_ENTRY luaopen_luaadd(lua_State* L) {
	
	//// stack dump addition
	lua_pushcfunction(L, stack_dump);
	lua_setglobal(L, "stackdump");
	
	
	//// io table addition
	lua_getglobal(L, "io");
	if(lua_isnil(L, 1) == 1)
		lua_createtable(L, 1, 0);
	
	// mtime
	lua_pushcfunction(L, lua_fstat_time);
	lua_setfield(L, -2, "mtime");
	
	// dir
	lua_pushcfunction(L, lua_cwd_getdir);
	lua_setfield(L, -2, "dir");
	
	lua_setglobal(L, "io");
	
	
	//// os table addition
	lua_getglobal(L, "os");
	if(lua_isnil(L, 1) == 1)
		lua_createtable(L, 3, 0);
	
	// cwd
	lua_pushcfunction(L, lua_cwd_setcwd);
	lua_setfield(L, -2, "setcwd");
	lua_pushcfunction(L, lua_cwd_getcwd);
	lua_setfield(L, -2, "getcwd");
	lua_pushcfunction(L, lua_clear_window);
	
	// clear console
	lua_setfield(L, -2, "clear");
	
	lua_setglobal(L, "os");
	
	return 0;
}

