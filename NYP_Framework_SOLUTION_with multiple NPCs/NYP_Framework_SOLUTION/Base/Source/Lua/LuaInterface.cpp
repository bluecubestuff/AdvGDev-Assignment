#include "LuaInterface.h"
#include <iostream>
using std::cout;
using std::endl;

CLuaInterface::CLuaInterface() : theLuaState(nullptr)
{

}

CLuaInterface::~CLuaInterface()
{

}

// Initialisation of lua interface classs
bool CLuaInterface::Init()
{
	bool result = false;

	//create lua state
	theLuaState = lua_open();

	if (theLuaState)
	{
		//Load lua auxiliary libraries
		luaL_openlibs(theLuaState);

		//Load lua script
		luaL_dofile(theLuaState, "Image//DM2240.lua");

		result = true;
	}

	return result;
}

// Run lua interface class
void CLuaInterface::Run()
{
	if (theLuaState == nullptr)
		return;

	// Read the variables
	lua_getglobal(theLuaState, "title");
	// extract value, index -1 as variable is already retrieved using lua_getglobal
	const char* title = lua_tostring(theLuaState, -1);

	lua_getglobal(theLuaState, "width");
	int width = lua_tointeger(theLuaState, -1);

	lua_getglobal(theLuaState, "height");
	int height = lua_tointeger(theLuaState, -1);
	
	// Display on screen
	cout << title << endl;
	cout << "-----------------------------------------\n";
	cout << "Width of screen: " << width << endl;
	cout << "Height of screen: " << height << endl;
}

// Drop lua Interface class
void CLuaInterface::Drop()
{
	if (theLuaState)
		lua_close(theLuaState);
}

// Get a int value through the Lua Interface Class
int CLuaInterface::getIntValue(const char* name)
{
	lua_getglobal(theLuaState, name);
	return lua_tointeger(theLuaState, -1);;
}

// Get a float value through the Lua Interface Class
float CLuaInterface::getFloatValue(const char* varName)
{
	lua_getglobal(theLuaState, varName);
	return (float)lua_tonumber(theLuaState, -1);
}

// Save an integer value through the Lua Interface Class
void CLuaInterface::saveIntValue(const char* varName,
	const int value, const bool bOverwrite)
{
	lua_getglobal(theLuaState, "SaveToLuaFile");
	char outputString[80];
	sprintf(outputString, "%s = %d\n", varName, value);
	lua_pushstring(theLuaState, outputString);
	lua_pushinteger(theLuaState, bOverwrite);
	lua_call(theLuaState, 2, 0); cout << "....................";
}
// Save a float value through the Lua Interface Class
void CLuaInterface::saveFloatValue(const char* varName,
	const float value, const bool bOverwrite)
{
	lua_getglobal(theLuaState, "SaveToLuaFile");
	char outputString[80];
	sprintf(outputString, "%s = %6.4f\n", varName, value);
	lua_pushstring(theLuaState, outputString);
	lua_pushinteger(theLuaState, bOverwrite);
	lua_call(theLuaState, 2, 0);
}