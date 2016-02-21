#include "ILuaMethod.h"
#include <functional>

using namespace std;

namespace Phobos
{
	template<int N, typename RT, typename... Args>
	class LuaMethod : ILuaMethod
	{
	private:
		using LuaMethodType = function<RT(Args...)>;
		LuaMethodType mLuaMethod;
		string mName;
		lua_State* mLuaVM;	//For Destruction		

	public:
		//--- Constructors ---
		LuaMethod(lua_State* luavm, const string& name, RT(*func)(Args...))
			: LuaMethod(luavm, name, LuaMethodType{func})
		{

		}

		LuaMethod(lua_State* luavm, string metatablename, const string& name, LuaMethodType func)
		{
			mLuaVM = luavm;
			mName = name;
			mLuaMethod = func;

			//Attempt to Get Metatable
			luaL_getmetatable(mLuaVM, metatablename.c_str());
			
			if(lua_type(luavm, -1) == LUA_TNIL)
			{
				//Create Metatable
				luaL_newmetatable(mLuaVM, metatablename.c_str());
				lua_pushstring(mLuaVM, "__index");
				lua_pushvalue(mLuaVM, -2);
				lua_settable(mLuaVM, -3);
			}

			//Add Pointer to this
			lua_pushlightuserdata(mLuaVM, static_cast<void*>(static_cast<ILuaMethod*>(this)));

			//Push Method
			lua_pushcclosure(mLuaVM, &Laputa::LuaDispatch, 1);			

			//Bind to Name
			lua_setfield(mLuaVM, -2, name.c_str());

			//Pop Metatable
			lua_pop(mLuaVM, 1);
		}

		//Disable Copy Constructor
		LuaMethod(const LuaMethod &other) = delete;
		
		//Move Constructor
		LuaMethod(LuaMethod &&other)
			: mLuaMethod(other.mLuaMethod),	mName(other.mName),	mLuaVM(other.mLuaVM) 
		{
				*other.mLuaMethod = nullptr;
		}


		//--- Destructor ---
		virtual ~LuaMethod()
		{

		}

		//--- Methods ---
		virtual int Run(lua_State* luaVM) override
		{
			tuple<Args...> args = Laputa::GetArgs<Args...>(luaVM);

			RT retval = Laputa::Lift(mLuaMethod, args);

			Laputa::Push<RT>(luaVM, retval);

			return N;
		}
	};

	template<typename... Args>
	class LuaMethod<0, void, Args...> : ILuaMethod
	{
	private:
		using LuaMethodType = function<void(Args...)>;
		LuaMethodType mLuaMethod;
		string mName;
		lua_State* mLuaVM;	//For Destruction		

	public:
		//--- Constructors ---
		LuaMethod(lua_State* luavm, const string& name, void(*func)(Args...))
			: LuaMethod(luavm, name, LuaMethodType{ func })
		{

		}

		LuaMethod(lua_State* luavm, string metatablename, const string& name, LuaMethodType func)
		{
			mLuaVM = luavm;
			mName = name;
			mLuaMethod = func;

			//Attempt to Get Metatable
			luaL_getmetatable(mLuaVM, metatablename.c_str());

			if (lua_type(luavm, -1) == LUA_TNIL)
			{
				//Create Metatable
				luaL_newmetatable(mLuaVM, metatablename.c_str());
				lua_pushstring(mLuaVM, "__index");
				lua_pushvalue(mLuaVM, -2);
				lua_settable(mLuaVM, -3);
			}

			//Add Pointer to this
			lua_pushlightuserdata(mLuaVM, static_cast<void*>(static_cast<ILuaMethod*>(this)));

			//Push Method
			lua_pushcclosure(mLuaVM, &Laputa::LuaDispatch, 1);

			//Bind to Name
			lua_setfield(mLuaVM, -2, name.c_str());

			//Pop Metatable
			lua_pop(mLuaVM, 1);
		}

		//--- Methods ---
		virtual int Run(lua_State* luaVM) override
		{
			tuple<Args...> args = Laputa::GetArgs<Args...>(luaVM);

			Laputa::Lift(mLuaMethod, args);

			return 0;
		}
	};
}