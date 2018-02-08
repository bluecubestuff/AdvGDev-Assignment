#ifndef SCENE_TEXT_H
#define SCENE_TEXT_H

#include "Scene.h"
#include "Mtx44.h"
#include "PlayerInfo/PlayerInfo.h"
#include "GroundEntity.h"
#include "FPSCamera.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "GenericEntity.h"
#include "SceneGraph/UpdateTransformation.h"
#include "Enemy\Enemy.h"
#include <vector>

class ShaderProgram;
class SceneManager;
class TextEntity;
class Light;
class SceneText : public Scene
{	
public:
	SceneText();
	~SceneText();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	static bool isPause;

private:
	SceneText(SceneManager* _sceneMgr); // This is used to register to SceneManager

	int max_enemies;

	ShaderProgram* currProg;
	CPlayerInfo* playerInfo;
	GroundEntity* groundEntity;
	FPSCamera camera;
	TextEntity* textObj[4];
	Light* lights[2];

	GenericEntity* theCube;
	CEnemy* theEnemy;

	std::vector<Mech*> enemyMechList;
	//std::vector<CEnemy*> enemyList;

	static SceneText* sInstance; // The pointer to the object that gets registered
};

#endif