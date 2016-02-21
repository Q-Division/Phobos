
#ifndef PHOBOS_PRIMITIVES_H
#define PHOBOS_PRIMITIVES_H

#include <string>
#include "TypeName.h"

extern "C"
{
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
}

using namespace std;

namespace Phobos
{
	namespace Laputa
	{
		template<typename T>
		inline T CheckGet(lua_State* luaVM, int index)
		{
			void* ud = luaL_checkudata(luaVM, index, TypeName<T>::Get());

			T t = *(T*)ud;

			return t;
		}

		template<>
		inline string CheckGet(lua_State* luaVM, int index)
		{
			return luaL_checkstring(luaVM, index);
		}

		template<>
		inline int CheckGet(lua_State* luaVM, int index)
		{
			return luaL_checkinteger(luaVM, index);
		}

		template<>
		inline float CheckGet(lua_State* luaVM, int index)
		{
			return (float)luaL_checknumber(luaVM, index);
		}

		template<>
		inline double CheckGet(lua_State* luaVM, int index)
		{
			return luaL_checknumber(luaVM, index);
		}

		template<typename T>
		inline void Push(lua_State* luaVM, T t)
		{
			*(void**)(lua_newuserdata(luaVM, sizeof(void*))) = t;
			luaL_getmetatable(luaVM, TypeName<T>::Get());
			lua_setmetatable(luaVM, -2);
		}

		template<>
		inline void Push(lua_State* luaVM, string t)
		{
			lua_pushstring(luaVM, t.c_str());
		}
	}
}

#endif //PHOBOS_PRIMITIVES_H