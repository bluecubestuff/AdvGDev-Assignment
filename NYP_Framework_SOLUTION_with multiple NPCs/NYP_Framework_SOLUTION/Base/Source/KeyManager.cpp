#include "KeyManager.h"
#include "LuaInterface.h"

void KeyManager::Init()
{
	keyMap["forward"] = CLuaInterface::GetInstance()->getCharValue("forward");
	keyMap["back"] = CLuaInterface::GetInstance()->getCharValue("back");
	keyMap["right"] = CLuaInterface::GetInstance()->getCharValue("right");
	keyMap["left"] = CLuaInterface::GetInstance()->getCharValue("left");
	keyMap["reload"] = CLuaInterface::GetInstance()->getCharValue("reload");
	keyMap["grenade"] = CLuaInterface::GetInstance()->getCharValue("grenade");
	keyMap["pause"] = CLuaInterface::GetInstance()->getCharValue("pause");
}
