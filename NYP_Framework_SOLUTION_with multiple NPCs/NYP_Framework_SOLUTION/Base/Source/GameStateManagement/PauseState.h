#pragma once

#include <vector>

#include "Scene.h"
#include "Mtx44.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "../FPSCamera.h"
#include "../SpriteEntity.h"

class SceneManager;
class Button;
//class TextEntity;
class CPauseState : public Scene
{
public:
	CPauseState();
	~CPauseState();

	std::vector<Button*> buttonList;

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	FPSCamera camera;
	SpriteEntity* PauseStateBackground;
	//	TextEntity* textObj[3];
};