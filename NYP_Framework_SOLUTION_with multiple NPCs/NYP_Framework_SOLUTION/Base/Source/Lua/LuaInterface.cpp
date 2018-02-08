#include "LuaInterface.h"
#include <iostream>
using std::cout;
using std::endl;

CLuaInterface::CLuaInterface()
{
	theLuaState["default"] = nullptr;
	theLuaState["waypoint"] = nullptr;
	theLuaState["resolution"] = nullptr;
}

CLuaInterface::~CLuaInterface()
{

}

// Initialisation of lua interface classs
bool CLuaInterface::Init()
{
	bool result = false;

	//create lua state
	theLuaState["default"] = lua_open();
	theLuaState["waypoint"] = lua_open();
	theLuaState["resolution"] = lua_open();

	if (theLuaState["resolution"])
	{
		//Load lua auxiliary libraries
		luaL_openlibs(theLuaState["resolution"]);
		//Load lua script
		luaL_dofile(theLuaState["resolution"], "LuaFile//resolution.lua");

		result = true;
	}

	if (theLuaState["waypoint"])
	{
		//Load lua auxiliary libraries
		luaL_openlibs(theLuaState["waypoint"]);

		//Load lua script
		luaL_dofile(theLuaState["waypoint"], "LuaFile//Waypoint.lua");

		result = true;
	}

	if (theLuaState["default"])
	{
		//Load lua auxiliary libraries
		luaL_openlibs(theLuaState["default"]);

		//Load lua script
		luaL_dofile(theLuaState["default"], "LuaFile//DM2240.lua");

		result = true;
	}

	return result;
}

// Run lua interface class
void CLuaInterface::Run()
{
	if (theLuaState["default"] == nullptr || theLuaState["waypoint"] == nullptr)
		return;

	// Read the variables
	lua_getglobal(theLuaState["default"], "title");
	// extract value, index -1 as variable is already retrieved using lua_getglobal
	const char* title = lua_tostring(theLuaState["default"], -1);

	lua_getglobal(theLuaState["default"], "width");
	int width = lua_tointeger(theLuaState["default"], -1);

	lua_getglobal(theLuaState["default"], "height");
	int height = lua_tointeger(theLuaState["default"], -1);
	
	// Display on screen
	cout << title << endl;
	cout << "-----------------------------------------\n";
	cout << "Width of screen: " << width << endl;
	cout << "Height of screen: " << height << endl;
}

// Drop lua Interface class
void CLuaInterface::Drop()
{
	if (theLuaState["default"])
		lua_close(theLuaState["default"]);
	if(theLuaState["waypoint"])
		lua_close(theLuaState["waypoint"]);
}

// Get a int value through the Lua Interface Class
int CLuaInterface::getIntValue(const char* name, std::string luaStateKey)
{
	lua_getglobal(theLuaState[luaStateKey], name);
	int result = lua_tointeger(theLuaState[luaStateKey], -1);
	lua_pop(theLuaState[luaStateKey], 1);
	return result;
}

// Get a float value through the Lua Interface Class
float CLuaInterface::getFloatValue(const char* varName, std::string luaStateKey)
{
	lua_getglobal(theLuaState[luaStateKey], varName);
	float result = (float)lua_tonumber(theLuaState[luaStateKey], -1);
	lua_pop(theLuaState[luaStateKey], 1);
	return result;
}

char CLuaInterface::getCharValue(const char * varName, std::string luaStateKey)
{
	lua_getglobal(theLuaState[luaStateKey], varName);

	size_t len;
	const char* cstr = lua_tolstring(theLuaState[luaStateKey], -1, &len);
	if (len > 0)
	{
		lua_pop(theLuaState[luaStateKey], 1);
		return cstr[0];
	}

	return 0;
}

float CLuaInterface::GetField(const char * key, std::string luaStateKey)
{
	int result = 0;
	lua_pushstring(theLuaState[luaStateKey], key);
	lua_gettable(theLuaState[luaStateKey], -2);
	result = (int)lua_tonumber(theLuaState[luaStateKey], -1);
	lua_pop(theLuaState[luaStateKey], 1);
	return result;
}

// Save an integer value through the Lua Interface Class
void CLuaInterface::saveIntValue(const char* varName, const char* fileName,
	const int value, const bool bOverwrite)
{
	lua_getglobal(theLuaState["default"], "SaveToLuaFile");
	char outputString[80];
	sprintf(outputString, "%s = %d\n", varName, value);
	lua_pushstring(theLuaState["default"], outputString);
	lua_pushstring(theLuaState["default"], fileName);
	lua_pushinteger(theLuaState["default"], bOverwrite);
	lua_call(theLuaState["default"], 3, 0); cout << "....................";
}

// Save a float value through the Lua Interface Class
void CLuaInterface::saveFloatValue(const char* varName, const char* fileName,
	const float value, const bool bOverwrite)
{
	lua_getglobal(theLuaState["default"], "SaveToLuaFile");
	char outputString[80];
	sprintf(outputString, "%s = %6.4f\n", varName, value);
	lua_pushstring(theLuaState["default"], outputString);
	lua_pushstring(theLuaState["default"], fileName);
	lua_pushinteger(theLuaState["default"], bOverwrite);
	lua_call(theLuaState["default"], 3, 0);
}