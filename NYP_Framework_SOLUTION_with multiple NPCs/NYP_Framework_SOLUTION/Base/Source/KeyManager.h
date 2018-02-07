#ifndef KEY_MANAGER_H
#define KEY_MANAGER_H

#include "SingletonTemplate.h"
#include <map>
#include <string>

class KeyManager : public Singleton<KeyManager>
{
	friend Singleton<KeyManager>;
private:
	KeyManager() {};
	virtual ~KeyManager() {};

	std::map<std::string, char> keyMap;

public:

	void Init();

	inline char GetKey(std::string index) { return keyMap[index]; }
	inline void SetKey(std::string index, char key) { keyMap[index] = key; }
};

#endif
