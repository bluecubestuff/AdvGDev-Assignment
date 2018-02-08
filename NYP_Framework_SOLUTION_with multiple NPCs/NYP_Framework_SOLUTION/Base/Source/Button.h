#pragma once

#include <string>
#include "Vector3.h"

class SpriteEntity;
struct Button
{
	Button(std::string _name, float posX, float posY, float sizeX, float sizeY);
	~Button() {}

	void Render();
	bool CheckOnMouse(double x, double y);
	void OnClick();

	std::string name;
	int maxX, maxY, minX, minY;
	Vector3 pos;
	Vector3 scale;
	float ratioPosX;
	float ratioPosY;
	float ratioSizeX;
	float ratioSizeY;
	SpriteEntity* buttonMesh;
};