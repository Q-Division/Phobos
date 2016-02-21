

#ifndef ILUAMETHOD_H
#define ILUAMETHOD_H

#include "Indices.h"
#include "Primitives.h"

extern "C"
{
	#include <lua.h>
}

namespace Phobos
{
	class ILuaMethod
	{
	public:
		virtual ~ILuaMethod() {}
		virtual int Run(lua_State* luaVM) = 0;
	};

	namespace Laputa
	{
		inline int LuaDispatch(lua_State* luaVM)
		{
			ILuaMethod* luamethod = static_cast<ILuaMethod*>(lua_touserdata(luaVM, lua_upvalueindex(1)));
			return luamethod->Run(luaVM);
		}

		template<typename RT, typename... Args, size_t... N>
		inline RT Lift(function<RT(Args...)> func, tuple<Args...> args, Indices<N...>)
		{
			//get<i> gets the ith element of a tuple, get<N>(args)...
			return func(get<N>(args)...);
		}

		template<typename RT, typename... Args>
		inline RT Lift(function<RT(Args...)> func, tuple<Args...> args)
		{
			return Lift(func, args, typename IndicesBuilder<sizeof...(Args)>::type());
		}

		template<typename... T, size_t... N>
		inline tuple<T...> GetArgs(lua_State* luaVM, Indices<N...>)
		{
			//Needs a Function CheckGet<T>(luaVM, i) which check the type of the ith element on the
			//stack and return it.
			//Note the +1 in the function call to shift to Lua's 1 based counting system.
			return tuple<T...> { CheckGet<T>(luaVM, N + 1)... };
		}

		//Helper Function to Auto Generate the Indices
		template<typename... T>
		inline tuple<T...> GetArgs(lua_State* luaVM)
		{
			return GetArgs<T...>(luaVM, typename IndicesBuilder<sizeof...(T)>::type());
		}
	}
}

#endif //ILUAMETHOD_H