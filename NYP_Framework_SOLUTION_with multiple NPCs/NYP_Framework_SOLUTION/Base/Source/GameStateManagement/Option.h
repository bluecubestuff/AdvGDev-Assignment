#pragma once
#ifndef OPTION_H
#define OPTION_H

#include <vector>

#include "Scene.h"
#include "Mtx44.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "../FPSCamera.h"
#include "../SpriteEntity.h"

struct Button
{
	Button(std::string _name, float posX, float posY, float sizeX, float sizeY);
	~Button(){}
	
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

class SceneManager;
class OptionState : public Scene
{
public:
	OptionState();
	~OptionState();

	float worldX;
	float worldY;
	std::vector<Button*> buttonList;

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	static OptionState* me;

private:
	FPSCamera camera;
	SpriteEntity* OptionStateBackground;
	//	TextEntity* textObj[3];
};

#endif