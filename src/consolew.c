
/*
 * Lua.c Console
 * 
 *		- Line by Line interpretation
 *		- Files executed by passing
 *		- Working directory support
 *		- Built in stack-dump
 * Works with:
 *		Lua5.3.x, Lua5.2.x, Lua5.1.x
 * 
 * 5.2.x and 5.1.x assume that you have enough memory to start initial functions.
 * 
 */

#if defined(__linux__) || defined(__unix__)
#include <unistd.h>
#define CHDIR_FUNC chdir
#else
#include <dirent.h>
#define CHDIR_FUNC _chdir
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#include "additions.h"


#define LUA_CONSOLE_COPYRIGHT "LuaConsole Copyright MIT (C) 2017 Hydroque\n"

#define PRIMARY_BUFFER_SIZE 1025
#define SECONDARY_BUFFER_SIZE 1033


// internal enums, represent lua error category
typedef enum LuaConsoleError {
	INTERNAL_ERROR = 0,
	SYNTAX_ERROR = 1,
	RUNTIME_ERROR = 2,
} LuaConsoleError;


// usage message
const char HELP_MESSAGE[] = 
	"Lua Console | Version: 5/6/2017\n"
	LUA_COPYRIGHT
	LUA_CONSOLE_COPYRIGHT
	"\n"
	"Supports Lua5.3, Lua5.2, Lua5.1\n"
	"5.2.x and 5.1.x assume that you have enough memory for initial functions.\n"
	"\n"
	"\t- Line by Line interpretation\n"
	"\t- Files executed by passing\n"
	"\t- Working directory support\n"
	"\t- Built in stack-dump\n"
	"\n"
	"Usage: lua.exe [FILE_PATH] [-v] [-e] [-s START_PATH] [-p] [-a] [-c] [-?] \n"
	"\n"
	"-v \t Prints the Lua version in use\n"
	"-e \t Prevents lua core libraries from loading\n"
	"-s \t Issues a new root path\n"
	"-p \t Has console post exist after script in line by line mode\n"
	"-a \t Removes the additions\n"
	"-c \t No copyright on init\n"
	"-? \t Displays this help message\n";



// one environment per process
static lua_State* L;


// necessary to put this outside of main, print doesn't work
static int no_libraries = 0;
static int squelch = 0;




// comprehensive error output
static void print_error(LuaConsoleError error) {
	const char* msg = lua_tostring(L, 1);
	switch(error) {
	case INTERNAL_ERROR:
		printf("(Internal)");
		break;
	case SYNTAX_ERROR:
		printf("(Syntax)");
		break;
	case RUNTIME_ERROR:
		printf("(Runtime)");
		break;
	}
	int top = lua_gettop(L);
	fprintf(stderr, " | Stack Top: %d | %s\n", lua_gettop(L), msg);
	if(top > 1) // other than error message
		stack_dump(L);
	lua_pop(L, 1);
}


// handles the execution of a file.lua
static int lua_main_dofile(lua_State* L) {
	const char* file = lua_tostring(L, 1);
	lua_pop(L, 1);
	if(luaL_loadfile(L, file) != 0) {
		print_error(SYNTAX_ERROR);
		return 0;
	}
	if(lua_pcall(L, 0, 0, 0) != 0) {
		print_error(RUNTIME_ERROR);
		return 0;
	}
	return 0;
}



// all-in-one function to handle file and line by line interpretation
int start_protective_mode(lua_CFunction func, const char* file) {
	lua_pushcclosure(L, func, 0); /* possible out of memory error in 5.2/5.1 */
	lua_pushlstring(L, file, strlen(file)); /* possible out of memory error in 5.2/5.1 */
	int status = lua_pcall(L, 1, 0, 0);
	if(status != 0) {
		print_error(INTERNAL_ERROR);
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

// handles arguments, cwd, loads necessary data, executes lua
int main(int argc, char* argv[])
{
	int print_version = 0;
	int change_start = 0;
	int no_file = 0;
	char* start = 0;
	int no_additions = 0;
	
	
	// handle arguments
	if(argc == 1) { // post-exist if !(arguments > 1)
		no_file = 1;
	} else {
		// don't try to execute file if it isn't first argument
		if(argv[1][0] == '-')
			no_file = 1;
		int i;
		for (i=1; i<argc; i++) {
			// don't parse non-switches
			switch(argv[i][0]) {
			case '/':
			case '-':
				break;
			default:
				continue;
			}
			// set variables up for later parsing
			switch(argv[i][1]) {
			case 'v': case 'V':
				print_version = 1;
				break;
			case 'e': case 'E':
				no_libraries = 1;
				break;
			case 's': case 'S':
				change_start = 1;
				start = argv[i+1];
				break;
			case 'a': case 'A':
				no_additions = 1;
				break;
			case 'c': case 'C':
				squelch = 1;
				break;
			case '?':
				fprintf(stdout, "%s\n", HELP_MESSAGE);
				return EXIT_SUCCESS;
			}
		}
	}
	
	
	// make sure to start in the requested directory, if any
	if(change_start == 1 && CHDIR_FUNC(start) == -1) {
		fprintf(stderr, "%s\n", "Invalid start directory supplied");
		fprintf(stdout, "%s\n", HELP_MESSAGE);
		return EXIT_FAILURE;
	}
	
	
	// initiate lua
	L = luaL_newstate();
	if(L == 0) {
		fprintf(stderr, "%s\n", "Allocation Failed: Out of Memory");
		return EXIT_FAILURE;
	}
	
	
	// initiate the libraries
	if(no_libraries == 0)
		luaL_openlibs(L);
	
	
	// print out the version, new state because no_libraries can be 1
	if(print_version == 1) {
		lua_State* gL = luaL_newstate();
		if(gL == 0) {
			fprintf(stderr, "%s\n", "Allocation Failed: Out of Memory");
			return EXIT_FAILURE;
		}
		
		luaL_openlibs(gL);
		
		// print lua version
		lua_getglobal(gL, "_VERSION");
		fprintf(stdout, "%s\n", lua_tostring(gL, 1));
		lua_pop(gL, 1);
		lua_close(gL);
	}
	
	
	// if there is nothing to do, then exit, as there is nothing left to do
	if(no_file == 1) {
		lua_close(L);
		return EXIT_SUCCESS;
	}
	
	
	// add additions
	if(no_additions == 0)
		additions_add(L);
	
	
	// load function into protected mode (pcall)
	int status = start_protective_mode(&lua_main_dofile, argv[1]);
		
	lua_close(L);
	
	return status;
}