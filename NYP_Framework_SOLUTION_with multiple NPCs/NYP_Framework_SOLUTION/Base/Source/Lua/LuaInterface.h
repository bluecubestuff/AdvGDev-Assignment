#ifndef LUA_INTERFACE_H
#define LUA_INTERFACE_H

#include <lua.hpp>
#include "SingletonTemplate.h"
#include <map>
#include <string>

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
	int getIntValue(const char* name, std::string luaStateKey = "default");
	// Get a float value through the Lua Interface Class
	float getFloatValue(const char* varName, std::string luaStateKey = "default");
	// Get a char value through lua interface
	char getCharValue(const char* varName, std::string luaStateKey = "default");
	// Extract a field from table
	float GetField(const char* key, std::string luaStateKey = "default");

	// Save an integer value through the Lua Interface Class
	void saveIntValue(const char* varName, const int value, const bool bOverwrite = NULL);
	// Save a float value through the Lua Interface Class
	void saveFloatValue(const char* varName, const float value, const bool bOverwrite = NULL);

	// pointer to lua state
	//lua_State* theLuaState;
	std::map<std::string, lua_State*> theLuaState;
};



#endif
