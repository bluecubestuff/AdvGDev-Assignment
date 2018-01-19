#ifndef LUA_INTERFACE_H
#define LUA_INTERFACE_H

#include <lua.hpp>
#include "SingletonTemplate.h"

class CLuaInterface : public Singleton<CLuaInterface>
{
	friend Singleton<CLuaInterface>;
private:
	CLuaInterface();
public:
	virtual ~CLuaInterface();

	// Initialisation of the Lua Interface Class
	bool Init();
	// Run Lua Interface class 
	void Run();
	// Drop Lua Interface class
	void Drop();
	// Get int value
	int getIntValue(const char* name);
	// Get a float value through the Lua Interface Class
	float getFloatValue(const char* varName);
	// Save an integer value through the Lua Interface Class
	void saveIntValue(const char* varName, const int value, const bool bOverwrite = NULL);
	// Save a float value through the Lua Interface Class
	void saveFloatValue(const char* varName, const float value, const bool bOverwrite = NULL);

	// pointer to lua state
	lua_State* theLuaState;
};



#endif
