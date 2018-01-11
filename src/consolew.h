
// #define LUA_VERSION_MAJOR	"5"
// #define LUA_VERSION_MINOR	"3"
// #define LUA_VERSION_NUM		503
// #define LUA_VERSION_RELEASE	"4"

// #define LUA_VERSION	"Lua " LUA_VERSION_MAJOR "." LUA_VERSION_MINOR
// #define LUA_RELEASE	LUA_VERSION "." LUA_VERSION_RELEASE
// #define LUA_COPYRIGHT	LUA_RELEASE "  Copyright (C) 1994-2017 Lua.org, PUC-Rio"
// #define LUA_AUTHORS	"R. Ierusalimschy, L. H. de Figueiredo, W. Celes"

#define LUA_COPYRIGHT "THIS IS LUA COPYRIGHT"

#define LUAI_MAXSTACK		1000000
#define LUA_REGISTRYINDEX	(-LUAI_MAXSTACK - 1000)
#define LUA_MULTRET			-1
#define lua_Integer		int

#define LUA_TNONE		(-1)

#define LUA_TNIL		0
#define LUA_TBOOLEAN		1
#define LUA_TLIGHTUSERDATA	2
#define LUA_TNUMBER		3
#define LUA_TSTRING		4
#define LUA_TTABLE		5
#define LUA_TFUNCTION		6
#define LUA_TUSERDATA		7
#define LUA_TTHREAD		8

#define lua_Number float


typedef struct lua_State lua_State;

typedef intptr_t lua_KContext;

typedef int (*_lua_CFunction) (lua_State *L);
typedef int (*_lua_KFunction)(lua_State *L, int, lua_KContext);




#define lua_newtable(L)		lua_createtable(L, 0, 0)
typedef void (*_lua_createtable)(lua_State*, int, int);

typedef lua_State* (*_luaL_newstate)(void);
typedef void (*_lua_close)(lua_State*);



typedef void (*_lua_setglobal)(lua_State*, const char*);
typedef int (*_lua_getglobal)(lua_State*, const char*);
typedef int (*_lua_gettop)(lua_State*);
typedef void (*_lua_settop)(lua_State*, int);

#define lua_pop(L,n)		lua_settop(L, -(n)-1)

typedef void (*_lua_rotate)(lua_State*, int, int);
#define lua_insert(L,idx)	lua_rotate(L, (idx), 1)

typedef void (*_lua_settable)(lua_State*, int);
typedef void (*_lua_setfield)(lua_State*, int, const char*);
typedef int (*_lua_getfield)(lua_State*, int, const char*);



typedef void (*_lua_callk)(lua_State*, int, int, lua_KContext, _lua_KFunction);
#define lua_call(L,n,r)		lua_callk(L, (n), (r), 0, NULL)

typedef int (*_lua_pcallk)(lua_State*, int, int, int, lua_KContext, _lua_KFunction);
#define lua_pcall(L,n,r,f)	lua_pcallk(L, (n), (r), (f), 0, NULL)



typedef void (*_lua_pushinteger)(lua_State*, lua_Integer);
typedef const char* (*_lua_pushlstring)(lua_State*, const char*, size_t);
typedef void (*_lua_pushcclosure)(lua_State*, _lua_CFunction, int);
typedef void (*_lua_pushboolean)(lua_State*, int);



typedef int (*_lua_type)(lua_State*, int);
typedef const char* (*_lua_tolstring)(lua_State*, int, size_t);
#define lua_tostring(L,i)	lua_tolstring(L, (i), 0)
typedef int (*_lua_toboolean)(lua_State*, int);
#define lua_tonumber(L,i)	lua_tonumberx(L,(i),NULL)
typedef float (*_lua_tonumberx)(lua_State*, int, int*);
typedef const void* (*_lua_topointer)(lua_State*, int);
typedef const char* (*_lua_typename)(lua_State*, int);

#define lua_istable(L,n)	(lua_type(L, (n)) == LUA_TTABLE)



typedef void (*_luaL_openlibs)(lua_State*);



typedef void (*_luaL_traceback)(lua_State*, lua_State*, const char*, int);
typedef int (*_luaL_loadstring)(lua_State*, const char*);

#define luaL_loadbuffer(L,s,sz,n)	luaL_loadbufferx(L,s,sz,n,NULL)
typedef int (*_luaL_loadbufferx)(lua_State*, const char*, size_t sz, const char*, const char*);

#define luaL_loadfile(L,f)	luaL_loadfilex(L,f,NULL)
typedef int (*_luaL_loadfilex)(lua_State*, const char*, const char*);




_lua_createtable lua_createtable;

_luaL_newstate luaL_newstate;
_lua_close lua_close;



_lua_setglobal lua_setglobal;
_lua_getglobal lua_getglobal;
_lua_gettop lua_gettop;
_lua_settop lua_settop;

_lua_rotate lua_rotate;

_lua_settable lua_settable;
_lua_setfield lua_setfield;
_lua_getfield lua_getfield;



_lua_callk lua_callk;
_lua_pcallk lua_pcallk;



_lua_pushinteger lua_pushinteger;
_lua_pushlstring lua_pushlstring;
_lua_pushcclosure lua_pushcclosure;
_lua_pushboolean lua_pushboolean;



_lua_type lua_type;
_lua_tolstring lua_tolstring;
_lua_toboolean lua_toboolean;
_lua_tonumberx lua_tonumberx;
_lua_topointer lua_topointer;
_lua_typename lua_typename;



_luaL_openlibs luaL_openlibs;



_luaL_traceback luaL_traceback;
_luaL_loadstring luaL_loadstring;
_luaL_loadbufferx luaL_loadbufferx;
_luaL_loadfilex luaL_loadfilex;


/*
** LuaJIT -- a Just-In-Time Compiler for Lua. http://luajit.org/
**
** Copyright (C) 2005-2017 Mike Pall. All rights reserved.
**
** Permission is hereby granted, free of charge, to any person obtaining
** a copy of this software and associated documentation files (the
** "Software"), to deal in the Software without restriction, including
** without limitation the rights to use, copy, modify, merge, publish,
** distribute, sublicense, and/or sell copies of the Software, and to
** permit persons to whom the Software is furnished to do so, subject to
** the following conditions:
**
** The above copyright notice and this permission notice shall be
** included in all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
** EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
** MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
** IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
** CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
** TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
** SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**
** [ MIT license: http://www.opensource.org/licenses/mit-license.php ]
*/

#ifndef _LUAJIT_H
#define _LUAJIT_H

//#include "lua.h"

#define LUAJIT_VERSION		"LuaJIT 2.0.5"
#define LUAJIT_VERSION_NUM	20005  /* Version 2.0.5 = 02.00.05. */
#define LUAJIT_VERSION_SYM	luaJIT_version_2_0_5
#define LUAJIT_COPYRIGHT	"Copyright (C) 2005-2017 Mike Pall"
#define LUAJIT_URL		"http://luajit.org/"

/* Modes for luaJIT_setmode. */
#define LUAJIT_MODE_MASK	0x00ff

enum {
  LUAJIT_MODE_ENGINE,		/* Set mode for whole JIT engine. */
  LUAJIT_MODE_DEBUG,		/* Set debug mode (idx = level). */

  LUAJIT_MODE_FUNC,		/* Change mode for a function. */
  LUAJIT_MODE_ALLFUNC,		/* Recurse into subroutine protos. */
  LUAJIT_MODE_ALLSUBFUNC,	/* Change only the subroutines. */

  LUAJIT_MODE_TRACE,		/* Flush a compiled trace. */

  LUAJIT_MODE_WRAPCFUNC = 0x10,	/* Set wrapper mode for C function calls. */

  LUAJIT_MODE_MAX
};

/* Flags or'ed in to the mode. */
#define LUAJIT_MODE_OFF		0x0000	/* Turn feature off. */
#define LUAJIT_MODE_ON		0x0100	/* Turn feature on. */
#define LUAJIT_MODE_FLUSH	0x0200	/* Flush JIT-compiled code. */

/* LuaJIT public C API. */

/* Control the JIT engine. */
extern int luaJIT_setmode(lua_State *L, int idx, int mode);

/* Enforce (dynamic) linker error for version mismatches. Call from main. */
extern void LUAJIT_VERSION_SYM(void);

#endif

/*
** Standard library header.
** Copyright (C) 2005-2017 Mike Pall. See Copyright Notice in luajit.h
*/

#ifndef _LUALIB_H
#define _LUALIB_H

// #include "lua.h"

#define LUA_FILEHANDLE	"FILE*"

#define LUA_COLIBNAME	"coroutine"
#define LUA_MATHLIBNAME	"math"
#define LUA_STRLIBNAME	"string"
#define LUA_TABLIBNAME	"table"
#define LUA_IOLIBNAME	"io"
#define LUA_OSLIBNAME	"os"
#define LUA_LOADLIBNAME	"package"
#define LUA_DBLIBNAME	"debug"
#define LUA_BITLIBNAME	"bit"
#define LUA_JITLIBNAME	"jit"
#define LUA_FFILIBNAME	"ffi"

extern int luaopen_base(lua_State *L);
extern int luaopen_math(lua_State *L);
extern int luaopen_string(lua_State *L);
extern int luaopen_table(lua_State *L);
extern int luaopen_io(lua_State *L);
extern int luaopen_os(lua_State *L);
extern int luaopen_package(lua_State *L);
extern int luaopen_debug(lua_State *L);
extern int luaopen_bit(lua_State *L);
extern int luaopen_jit(lua_State *L);
extern int luaopen_ffi(lua_State *L);

#ifndef lua_assert
#define lua_assert(x)	((void)0)
#endif

#endif



/*
** $Id: lauxlib.h,v 1.88.1.1 2007/12/27 13:02:25 roberto Exp $
** Auxiliary functions for building Lua libraries
** See Copyright Notice in lua.h
*/


#ifndef lauxlib_h
#define lauxlib_h


#include <stddef.h>
#include <stdio.h>

// #include "lua.h"


#define luaL_getn(L,i)          ((int)lua_objlen(L, i))
#define luaL_setn(L,i,j)        ((void)0)  /* no op! */

/* extra error code for `luaL_load' */
#define LUA_ERRFILE     (LUA_ERRERR+1)

typedef struct luaL_Reg {
  const char *name;
  _lua_CFunction func;
} luaL_Reg;

extern void (luaL_openlib) (lua_State *L, const char *libname,
                                const luaL_Reg *l, int nup);
extern void (luaL_register) (lua_State *L, const char *libname,
                                const luaL_Reg *l);
extern int (luaL_getmetafield) (lua_State *L, int obj, const char *e);
extern int (luaL_callmeta) (lua_State *L, int obj, const char *e);
extern int (luaL_typerror) (lua_State *L, int narg, const char *tname);
extern int (luaL_argerror) (lua_State *L, int numarg, const char *extramsg);
extern const char *(luaL_checklstring) (lua_State *L, int numArg,
                                                          size_t *l);
extern const char *(luaL_optlstring) (lua_State *L, int numArg,
                                          const char *def, size_t *l);
extern lua_Number (luaL_checknumber) (lua_State *L, int numArg);
extern lua_Number (luaL_optnumber) (lua_State *L, int nArg, lua_Number def);

extern lua_Integer (luaL_checkinteger) (lua_State *L, int numArg);
extern lua_Integer (luaL_optinteger) (lua_State *L, int nArg,
                                          lua_Integer def);

extern void (luaL_checkstack) (lua_State *L, int sz, const char *msg);
extern void (luaL_checktype) (lua_State *L, int narg, int t);
extern void (luaL_checkany) (lua_State *L, int narg);

extern int   (luaL_newmetatable) (lua_State *L, const char *tname);
extern void *(luaL_checkudata) (lua_State *L, int ud, const char *tname);

extern void (luaL_where) (lua_State *L, int lvl);
extern int (luaL_error) (lua_State *L, const char *fmt, ...);

extern int (luaL_checkoption) (lua_State *L, int narg, const char *def,
                                   const char *const lst[]);

extern int (luaL_ref) (lua_State *L, int t);
extern void (luaL_unref) (lua_State *L, int t, int ref);

extern int (luaL_loadfile) (lua_State *L, const char *filename);
extern int (luaL_loadbuffer) (lua_State *L, const char *buff, size_t sz,
                                  const char *name);



extern const char *(luaL_gsub) (lua_State *L, const char *s, const char *p,
                                                  const char *r);

extern const char *(luaL_findtable) (lua_State *L, int idx,
                                         const char *fname, int szhint);

/* From Lua 5.2. */
extern int luaL_fileresult(lua_State *L, int stat, const char *fname);
extern int luaL_execresult(lua_State *L, int stat);


/*
** ===============================================================
** some useful macros
** ===============================================================
*/

#define luaL_argcheck(L, cond,numarg,extramsg)	\
		((void)((cond) || luaL_argerror(L, (numarg), (extramsg))))
#define luaL_checkstring(L,n)	(luaL_checklstring(L, (n), NULL))
#define luaL_optstring(L,n,d)	(luaL_optlstring(L, (n), (d), NULL))
#define luaL_checkint(L,n)	((int)luaL_checkinteger(L, (n)))
#define luaL_optint(L,n,d)	((int)luaL_optinteger(L, (n), (d)))
#define luaL_checklong(L,n)	((long)luaL_checkinteger(L, (n)))
#define luaL_optlong(L,n,d)	((long)luaL_optinteger(L, (n), (d)))

#define luaL_typename(L,i)	lua_typename(L, lua_type(L,(i)))

#define luaL_dofile(L, fn) \
	(luaL_loadfile(L, fn) || lua_pcall(L, 0, LUA_MULTRET, 0))

#define luaL_dostring(L, s) \
	(luaL_loadstring(L, s) || lua_pcall(L, 0, LUA_MULTRET, 0))

#define luaL_getmetatable(L,n)	(lua_getfield(L, LUA_REGISTRYINDEX, (n)))

#define luaL_opt(L,f,n,d)	(lua_isnoneornil(L,(n)) ? (d) : f(L,(n)))

/*
** {======================================================
** Generic Buffer manipulation
** =======================================================
*/

#define LUAL_BUFFERSIZE		8192

typedef struct luaL_Buffer {
  char *p;			/* current position in buffer */
  int lvl;  /* number of strings in the stack (level) */
  lua_State *L;
  char buffer[LUAL_BUFFERSIZE];
} luaL_Buffer;

#define luaL_addchar(B,c) \
  ((void)((B)->p < ((B)->buffer+LUAL_BUFFERSIZE) || luaL_prepbuffer(B)), \
   (*(B)->p++ = (char)(c)))

/* compatibility only */
#define luaL_putchar(B,c)	luaL_addchar(B,c)

#define luaL_addsize(B,n)	((B)->p += (n))

extern void (luaL_buffinit) (lua_State *L, luaL_Buffer *B);
extern char *(luaL_prepbuffer) (luaL_Buffer *B);
extern void (luaL_addlstring) (luaL_Buffer *B, const char *s, size_t l);
extern void (luaL_addstring) (luaL_Buffer *B, const char *s);
extern void (luaL_addvalue) (luaL_Buffer *B);
extern void (luaL_pushresult) (luaL_Buffer *B);


/* }====================================================== */


/* compatibility with ref system */

/* pre-defined references */
#define LUA_NOREF       (-2)
#define LUA_REFNIL      (-1)

#define lua_ref(L,lock) ((lock) ? luaL_ref(L, LUA_REGISTRYINDEX) : \
      (lua_pushstring(L, "unlocked references are obsolete"), lua_error(L), 0))

#define lua_unref(L,ref)        luaL_unref(L, LUA_REGISTRYINDEX, (ref))

#define lua_getref(L,ref)       lua_rawgeti(L, LUA_REGISTRYINDEX, (ref))


#define luaL_reg	luaL_Reg

#endif

