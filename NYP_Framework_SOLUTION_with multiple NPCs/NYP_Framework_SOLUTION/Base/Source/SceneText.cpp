#include "SceneText.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>
#include "KeyboardController.h"
#include "MouseController.h"
#include "SceneManager.h"
#include "GraphicsManager.h"
#include "ShaderProgram.h"
#include "EntityManager.h"

#include "GenericEntity.h"
#include "GroundEntity.h"
#include "TextEntity.h"
#include "SpriteEntity.h"
#include "Light.h"
#include "SkyBox/SkyBoxEntity.h"
#include "SceneGraph\SceneGraph.h"
#include "SpatialPartition\SpatialPartition.h"
#include "Mech.h"
#include "Chassis.h"
#include "Leg.h"
#include "Torso.h"
#include "RenderHelper.h"
#include "WaypointData.h"
#include "Node.h"
#include "Edge.h"
#include "Lua\LuaInterface.h"
#include "KeyManager.h"

#include <iostream>
using namespace std;

#define DIST_BETWEEN_NODES

bool SceneText::isPause = false;
TextEntity* SceneText::textObj[] = {};

//SceneText* SceneText::sInstance = new SceneText(SceneManager::GetInstance());

SceneText::SceneText()
{
	prevScene = nullptr;
}

SceneText::SceneText(SceneManager* _sceneMgr)
{
	//_sceneMgr->AddScene("Start", this);
	prevScene = nullptr;
}

SceneText::~SceneText()
{
	CSpatialPartition::GetInstance()->RemoveCamera();
	CSceneGraph::GetInstance()->Destroy();
}

void SceneText::Init()
{

	lights[0] = new Light();
	GraphicsManager::GetInstance()->AddLight("lights[0]", lights[0]);
	lights[0]->type = Light::LIGHT_DIRECTIONAL;
	lights[0]->position.Set(1, 1, 0);
	lights[0]->color.Set(1, 1, 1);
	lights[0]->power = 1;
	lights[0]->kC = 1.f;
	lights[0]->kL = 0.01f;
	lights[0]->kQ = 0.001f;
	lights[0]->cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0]->cosInner = cos(Math::DegreeToRadian(30));
	lights[0]->exponent = 3.f;
	lights[0]->spotDirection.Set(0.f, 1.f, 0.f);
	lights[0]->name = "lights[0]";

	lights[1] = new Light();
	GraphicsManager::GetInstance()->AddLight("lights[1]", lights[1]);
	lights[1]->type = Light::LIGHT_DIRECTIONAL;
	lights[1]->position.Set(1, 1, 0);
	lights[1]->color.Set(1, 1, 0.5f);
	lights[1]->power = 0.4f;
	lights[1]->name = "lights[1]";

	// Load all the meshes
	MeshBuilder::GetInstance()->GenerateAxes("reference");
	MeshBuilder::GetInstance()->GenerateCrossHair("crosshair");
	MeshBuilder::GetInstance()->GenerateQuad("quad", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("quad")->textureID = LoadTGA("Image//calibri.tga");
	MeshBuilder::GetInstance()->GenerateText("text", 16, 16);
	MeshBuilder::GetInstance()->GetMesh("text")->textureID = LoadTGA("Image//calibri.tga");
	MeshBuilder::GetInstance()->GetMesh("text")->material.kAmbient.Set(1, 0, 0);
	MeshBuilder::GetInstance()->GenerateOBJ("Chair", "OBJ//chair.obj");
	MeshBuilder::GetInstance()->GetMesh("Chair")->textureID = LoadTGA("Image//chair.tga");
	MeshBuilder::GetInstance()->GenerateRing("ring", Color(1, 0, 1), 36, 1, 0.5f);
	MeshBuilder::GetInstance()->GenerateSphere("lightball", Color(1, 1, 1), 18, 36, 1.f);
	MeshBuilder::GetInstance()->GenerateSphere("sphere", Color(1, 0, 0), 18, 36, 0.5f);
	MeshBuilder::GetInstance()->GenerateCone("cone", Color(0.5f, 1, 0.3f), 36, 10.f, 10.f);
	MeshBuilder::GetInstance()->GenerateCube("cube", Color(1.0f, 1.0f, 0.0f), 1.0f);
	MeshBuilder::GetInstance()->GetMesh("cone")->material.kDiffuse.Set(0.99f, 0.99f, 0.99f);
	MeshBuilder::GetInstance()->GetMesh("cone")->material.kSpecular.Set(0.f, 0.f, 0.f);
	MeshBuilder::GetInstance()->GenerateQuad("GRASS_DARKGREEN", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("GRASS_DARKGREEN")->textureID = LoadTGA("Image//grass_darkgreen.tga");
	MeshBuilder::GetInstance()->GenerateQuad("GEO_GRASS_LIGHTGREEN", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("GEO_GRASS_LIGHTGREEN")->textureID = LoadTGA("Image//grass_lightgreen.tga");
	MeshBuilder::GetInstance()->GenerateCube("cubeSG", Color(1.0f, 0.64f, 0.0f), 1.0f);

	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_FRONT", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_BACK", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_LEFT", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_RIGHT", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_TOP", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateQuad("SKYBOX_BOTTOM", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_FRONT")->textureID = LoadTGA("Image//front.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_BACK")->textureID = LoadTGA("Image//back.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_LEFT")->textureID = LoadTGA("Image//right.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_RIGHT")->textureID = LoadTGA("Image//left.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_TOP")->textureID = LoadTGA("Image//top.tga");
	MeshBuilder::GetInstance()->GetMesh("SKYBOX_BOTTOM")->textureID = LoadTGA("Image//bottom.tga");
	MeshBuilder::GetInstance()->GenerateRay("laser", 10.0f);
	MeshBuilder::GetInstance()->GenerateQuad("GRIDMESH", Color(1, 1, 1), 1.f);

	//Mech mesh generation
	//MeshBuilder::GetInstance()->GenerateCube("torso", Color(0, 1, 0), 1.f);
	//MeshBuilder::GetInstance()->GenerateCube("leg", Color(0, 0, 1), 1.f);
	MeshBuilder::GetInstance()->GenerateOBJ("lowTorso", "OBJ//cube.obj")->textureID = LoadTGA("Image//Yellow.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("lowLeg", "OBJ//cube.obj")->textureID = LoadTGA("Image//yee.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("mediumTorso", "OBJ//mediumEnemy.obj")->textureID = LoadTGA("Image//mEnemy.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("mediumLeg", "OBJ//mediumLeg.obj")->textureID = LoadTGA("Image//mLeg.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("highTorso", "OBJ//highEnemy.obj")->textureID = LoadTGA("Image//hEnemy.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("highLeg", "OBJ//highLeg.obj")->textureID = LoadTGA("Image//hLeg.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("destroyedLeg", "OBJ//cube.obj")->textureID = LoadTGA("Image//boom.tga");

	//for lod requirement
	MeshBuilder::GetInstance()->GenerateOBJ("buildingLow", "OBJ//cube.obj")->textureID = LoadTGA("Image//front.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("buildingMedium", "OBJ//cube.obj")->textureID = LoadTGA("Image//yelow.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("buildingHigh", "OBJ//cube.obj")->textureID = LoadTGA("Image//white.tga");

	MeshBuilder::GetInstance()->GenerateOBJ("1", "OBJ//cube.obj")->textureID = LoadTGA("Image//color.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("2", "OBJ//cube.obj")->textureID = LoadTGA("Image//suzanne.tga");
	MeshBuilder::GetInstance()->GenerateOBJ("3", "OBJ//cube.obj")->textureID = LoadTGA("Image//uvmap.tga");

	//random stuff mesh 
	MeshBuilder::GetInstance()->GenerateCube("greencube", Color(0, 0.3, 0), 1.0f);
	MeshBuilder::GetInstance()->GenerateCube("bluecube", Color(0, 0, 0.5), 0.9f);
	MeshBuilder::GetInstance()->GenerateCube("blackcube", Color(0, 0, 0), 0.8f);
	MeshBuilder::GetInstance()->GenerateSphere("midsphere", Color(0.8, 0, 0), 18, 36, 0.4f);

	// Set up the Spatial Partition and pass it to the EntityManager to manage
	CSpatialPartition::GetInstance()->Init(100, 100, 10, 10);
	CSpatialPartition::GetInstance()->SetMesh("GRIDMESH");
	CSpatialPartition::GetInstance()->SetCamera(&camera);
	CSpatialPartition::GetInstance()->SetLevelOfDetails(40000.0f, 160000.0f);
	EntityManager::GetInstance()->SetSpatialPartition(CSpatialPartition::GetInstance());

	// Create entities into the scene
	//Create::Entity("reference", Vector3(0.0f, 0.0f, 0.0f)); // Reference
	//Create::Entity("lightball", Vector3(lights[0]->position.x, lights[0]->position.y, lights[0]->position.z)); // Lightball

	GenericEntity* wall = Create::Entity("bluecube", Vector3(-10.0f, 0.0f, -20.0f));
	wall->SetScale(Vector3(10.f,10.f,0.5f));
	wall->SetCollider(true);
	wall->SetAABB(Vector3(10.f, 10.f, 0.5f), Vector3(-10.f, -10.f, -0.5f));
	wall->InitLOD("greencube", "bluecube", "blackcube");


	GenericEntity* sphere = Create::Entity("bluecube", Vector3(-30.0f, 0.0f, -20.0f));
	sphere->SetScale(Vector3(2.f, 2.f, 2.f));
	sphere->SetCollider(true);
	sphere->SetAABB(Vector3(5.f, 5.f, 5.f), Vector3(-5.f, -5.f, -5.f));
	sphere->InitLOD("sphere", "midsphere", "blackcube");
	//============================================================================
	GenericEntity* aCube = Create::Entity("bluecube", Vector3(-20.0f, 0.0f, -20.0f));
	aCube->SetScale(Vector3(5, 20, 5));
	aCube->SetCollider(true);
	aCube->SetAABB(Vector3(5, 20, 5), Vector3(-5, 0, -5));
	aCube->InitLOD("buildingLow", "buildingMedium", "buildingHigh");

	aCube = Create::Entity("bluecube", Vector3(-100.f, 0.0f, -100.f));
	aCube->SetScale(Vector3(5, 20, 5));
	aCube->SetCollider(true);
	aCube->SetAABB(Vector3(5, 20, 5), Vector3(-5, 0, -5));
	aCube->InitLOD("buildingLow", "buildingMedium", "buildingHigh");

	aCube = Create::Entity("bluecube", Vector3(100.f, 0.0f, 100.f));
	aCube->SetScale(Vector3(5, 20, 5));
	aCube->SetCollider(true);
	aCube->SetAABB(Vector3(5, 20, 5), Vector3(-5, 0, -5));
	aCube->InitLOD("buildingLow", "buildingMedium", "buildingHigh");

	aCube = Create::Entity("bluecube", Vector3(-200.0f, -5.f, -200.0f));
	aCube->SetScale(Vector3(40, 5, 20));
	aCube->SetCollider(true);
	aCube->SetAABB(Vector3(20, 5, 10), Vector3(-20, 0, -10));
	aCube->InitLOD("1", "2", "3");

	aCube = Create::Entity("bluecube", Vector3(-50.f, -5.f, -10.f));
	aCube->SetScale(Vector3(40, 5, 20));;
	aCube->SetCollider(true);
	aCube->SetAABB(Vector3(20, 5, 10), Vector3(-20, 0, -10));
	aCube->InitLOD("1", "2", "3");

	// //Add the pointer to this new entity to the Scene Graph
	//CSceneNode* theNode = CSceneGraph::GetInstance()->AddNode(aCube);
	//if (theNode == NULL)
	//	cout << "EntityManager::AddEntity: Unable to add to scene graph!" << endl;

	//GenericEntity* anotherCube = Create::Entity("cube", Vector3(-20.0f, 1.1f, -20.0f));
	//anotherCube->SetCollider(true);
	//anotherCube->SetAABB(Vector3(1.f, 1.f, 1.f), Vector3(-1.f, -1.f, -1.f));
	//CSceneNode* anotherNode = theNode->AddChild(anotherCube);
	//if (anotherNode == NULL)
	//	cout << "EntityManager::AddEntity: Unable to add to scene graph!" << endl;
	//============================================================================

	/*
	GenericEntity* baseCube = Create::Asset("cube", Vector3(0.0f, 0.0f, 0.0f));
	CSceneNode* baseNode = CSceneGraph::GetInstance()->AddNode(baseCube);

	CUpdateTransformation* baseMtx = new CUpdateTransformation();
	baseMtx->ApplyUpdate(1.0f, 0.0f, 0.0f, 1.0f);
	baseMtx->SetSteps(-60, 60);
	baseNode->SetUpdateTransformation(baseMtx);

	GenericEntity* childCube = Create::Asset("cubeSG", Vector3(0.0f, 0.0f, 0.0f));
	CSceneNode* childNode = baseNode->AddChild(childCube);
	childNode->ApplyTranslate(0.0f, 1.0f, 0.0f);

	GenericEntity* grandchildCube = Create::Asset("cubeSG", Vector3(0.0f, 0.0f, 0.0f));
	CSceneNode* grandchildNode = childNode->AddChild(grandchildCube);
	grandchildNode->ApplyTranslate(0.0f, 0.0f, 1.0f);
	CUpdateTransformation* aRotateMtx = new CUpdateTransformation();
	aRotateMtx->ApplyUpdate(1.0f, 0.0f, 0.0f, 1.0f);
	aRotateMtx->SetSteps(-120, 60);
	grandchildNode->SetUpdateTransformation(aRotateMtx);*/

	// Create the playerinfo instance, which manages all information about the player
	playerInfo = CPlayerInfo::GetInstance();
	playerInfo->Init();

	// Create and attach the camera to the scene
	//camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	camera.Init(playerInfo->GetPos(), playerInfo->GetTarget(), playerInfo->GetUp());
	playerInfo->AttachCamera(&camera);
	GraphicsManager::GetInstance()->AttachCamera(&camera);
	
	groundEntity = Create::Ground("GRASS_DARKGREEN", "GEO_GRASS_LIGHTGREEN");
	//Create::Text3DObject("text", Vector3(0.0f, 0.0f, 0.0f), "DM2210", Vector3(10.0f, 10.0f, 10.0f), Color(0, 1, 1));
	Create::Sprite2DObject("crosshair", Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f));

	SkyBoxEntity* theSkyBox = Create::SkyBox("SKYBOX_FRONT", "SKYBOX_BACK",
											 "SKYBOX_LEFT", "SKYBOX_RIGHT",
											 "SKYBOX_TOP", "SKYBOX_BOTTOM");

	// Customise the ground entity
	groundEntity->SetPosition(Vector3(0, -10, 0));
	groundEntity->SetScale(Vector3(100.0f, 100.0f, 100.0f));
	groundEntity->SetGrids(Vector3(10.0f, 1.0f, 10.0f));
	playerInfo->SetTerrain(groundEntity);

	// Create a CEnemy instance
	//srand(time(NULL));
	//for (int i = 0; i < 10; i++)
	//{
	//	theEnemy = new CEnemy();
	//	float x = 1.0f + (i * rand() % 1000 - 500.0f);
	//	float y = 1.0f + (i * rand() % 1000 - 500.0f);
	//	theEnemy->SetRandomSeed(rand());
	//	theEnemy->Init(x, y);
	//	theEnemy->SetTerrain(groundEntity);
	//	theEnemy->SetTarget(theEnemy->GenerateTarget());
	//	theEnemy = NULL;
	//}

	// Setup the 2D entities
	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.0f;
	float fontSize = 25.0f;
	float halfFontSize = fontSize / 2.0f;
	for (int i = 0; i < 4; ++i)
	{
		textObj[i] = Create::Text2DObject("text", Vector3(-halfWindowWidth, -halfWindowHeight + fontSize*i + halfFontSize, 0.0f), "", Vector3(fontSize, fontSize, fontSize), Color(0.0f,1.0f,0.0f));
	}
	textObj[0]->SetText("HELLO WORLD");

	//WaypointData::GetInstance()->Init();
	WaypointData::GetInstance()->GenerateFromFile();
	max_enemies = CLuaInterface::GetInstance()->getIntValue("max_enemies");

	MouseController::GetInstance()->SetKeepMouseCentered(true);
	Application::SetMouseVisibilty(false);

	isPause = false;

	Math::InitRNG();
}

void SceneText::Update(double dt)
{
	MouseController::GetInstance()->SetKeepMouseCentered(true);
	Application::SetMouseVisibilty(false);
	if (KeyboardController::GetInstance()->IsKeyPressed(KeyManager::GetInstance()->GetKey("pause")))
	{
		if (!isPause)
		{
			isPause = true;
			SceneManager::GetInstance()->SetActiveScene("PauseState", false);
		}
		else
		{
			camera.Init(CPlayerInfo::GetInstance()->GetPos(), CPlayerInfo::GetInstance()->GetTarget(), CPlayerInfo::GetInstance()->GetUp());
			isPause = false;
		}
	}

	if (!isPause)
	{
		// Update our entities
		EntityManager::GetInstance()->Update(dt);

		// THIS WHOLE CHUNK TILL <THERE> CAN REMOVE INTO ENTITIES LOGIC! Or maybe into a scene function to keep the update clean
		if (KeyboardController::GetInstance()->IsKeyDown('1'))
			glEnable(GL_CULL_FACE);
		if (KeyboardController::GetInstance()->IsKeyDown('2'))
			glDisable(GL_CULL_FACE);
		if (KeyboardController::GetInstance()->IsKeyDown('3'))
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		if (KeyboardController::GetInstance()->IsKeyDown('4'))
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		if (KeyboardController::GetInstance()->IsKeyDown('5'))
		{
			lights[0]->type = Light::LIGHT_POINT;
		}
		else if (KeyboardController::GetInstance()->IsKeyDown('6'))
		{
			lights[0]->type = Light::LIGHT_DIRECTIONAL;
		}
		else if (KeyboardController::GetInstance()->IsKeyDown('7'))
		{
			lights[0]->type = Light::LIGHT_SPOT;
		}

		if (KeyboardController::GetInstance()->IsKeyDown('I'))
			lights[0]->position.z -= (float)(10.f * dt);
		if (KeyboardController::GetInstance()->IsKeyDown('K'))
			lights[0]->position.z += (float)(10.f * dt);
		if (KeyboardController::GetInstance()->IsKeyDown('J'))
			lights[0]->position.x -= (float)(10.f * dt);
		if (KeyboardController::GetInstance()->IsKeyDown('L'))
			lights[0]->position.x += (float)(10.f * dt);
		if (KeyboardController::GetInstance()->IsKeyDown('O'))
			lights[0]->position.y -= (float)(10.f * dt);
		if (KeyboardController::GetInstance()->IsKeyDown('P'))
			lights[0]->position.y += (float)(10.f * dt);

		if (KeyboardController::GetInstance()->IsKeyReleased('M'))
		{
			CSceneNode* theNode = CSceneGraph::GetInstance()->GetNode(1);
			Vector3 pos = theNode->GetEntity()->GetPosition();
			theNode->GetEntity()->SetPosition(Vector3(pos.x + 50.0f, pos.y, pos.z + 50.0f));
		}
		if (KeyboardController::GetInstance()->IsKeyReleased('N'))
		{
			CSpatialPartition::GetInstance()->PrintSelf();
		}

		if (KeyboardController::GetInstance()->IsKeyPressed(VK_F5))
		{
			char variableName[80] = "";
			CLuaInterface::GetInstance()->saveIntValue("WAYPOINTS", "LuaFile/Waypoint.lua", (int)(WaypointData::GetInstance()->nodeList.size()), true);
			for (auto it : WaypointData::GetInstance()->nodeList)
			{
				sprintf(variableName, "NodeX_%d", it->ID);
				CLuaInterface::GetInstance()->saveFloatValue(variableName, "LuaFile/Waypoint.lua", it->x);
				sprintf(variableName, "NodeZ_%d", it->ID);
				CLuaInterface::GetInstance()->saveFloatValue(variableName, "LuaFile/Waypoint.lua", it->y);
			}
		}

		// if the left mouse button was released
		if (MouseController::GetInstance()->IsButtonReleased(MouseController::LMB))
		{
			cout << "Left Mouse Button was released!" << endl;
		}
		if (MouseController::GetInstance()->IsButtonReleased(MouseController::RMB))
		{
			cout << "Right Mouse Button was released!" << endl;
		}
		if (MouseController::GetInstance()->IsButtonReleased(MouseController::MMB))
		{
			cout << "Middle Mouse Button was released!" << endl;
		}
		if (MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_XOFFSET) != 0.0)
		{
			cout << "Mouse Wheel has offset in X-axis of " << MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_XOFFSET) << endl;
		}
		if (MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) != 0.0)
		{
			cout << "Mouse Wheel has offset in Y-axis of " << MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) << endl;
		}
		// <THERE>

		// Update the player position and other details based on keyboard and mouse inputs
		playerInfo->Update(dt);

		//camera.Update(dt); // Can put the camera into an entity rather than here (Then we don't have to write this)

		GraphicsManager::GetInstance()->UpdateLights(dt);

		for (auto it : enemyMechList) {
			it->Update(dt);
			if (it->isDead == true)
				it->bDelete = true;
		}

		std::vector<Mech*>::iterator it;
		it = enemyMechList.begin();
		while (it != enemyMechList.end()) {
			if ((*it)->bDelete == true) {
				it = enemyMechList.erase(it);
			}
			else
				++it;
		}

		// Update the 2 text object values. NOTE: Can do this in their own class but i'm lazy to do it now :P
		// Eg. FPSRenderEntity or inside RenderUI for LightEntity
		std::ostringstream ss;
		ss.precision(5);
		float fps = (float)(1.f / dt);
		ss << "FPS: " << fps;
		textObj[0]->SetText(ss.str());

		std::ostringstream ss2;
		if (CPlayerInfo::GetInstance()->GetMech()->chassis->GetTorso()->GetHP() > 0) {
			ss2.precision(4);
			ss2 << "Player Leg HP:" << playerInfo->GetMech()->chassis->GetLeg()->GetHP();
			textObj[1]->SetText(ss2.str());
		}
		else {
			ss2.precision(4);
			ss2 << "Leg Deaded";
			textObj[1]->SetText(ss2.str());
		}

		std::ostringstream ss1;
		ss1.precision(4);
		ss1 << "Player Torso HP:" << playerInfo->GetMech()->chassis->GetTorso()->GetHP();
		textObj[2]->SetText(ss1.str());

		if (CPlayerInfo::GetInstance()->GetMech()->chassis->GetTorso()->GetHP() <= 0) {
			std::ostringstream ss3;
			ss3 << "Deaded";
			textObj[2]->SetText(ss3.str());
		}

		std::ostringstream ss4;
		ss4 << '[' << playerInfo->GetPos().x << ' ' << playerInfo->GetPos().y << ' ' << playerInfo->GetPos().z << ']';
		textObj[3]->SetText(ss4.str());

		static float timer = 0.f;
		timer += dt;
		if (timer > 5.f) {
			if (enemyMechList.size() < max_enemies) {
				//generate a random pos a certain dist away
				float dist = Math::RandFloatMinMax(80, 100);
				//generate a random direction;
				Vector3 dir;
				dir.Set(Math::RandFloatMinMax(-1, 1), 0, Math::RandFloatMinMax(-1, 1));
				dir.Normalize();
				Vector3 newPos = dir * dist;
				CEnemy* test = new CEnemy();
				test->Init(newPos.x, newPos.z);
				test->SetTerrain(groundEntity);
				test->SetTarget(test->GenerateTarget());
				//enemyList.push_back(test);
				Mech* mech = new Mech();
				mech->Init(test);
				enemyMechList.push_back(mech);
				std::cout << "spawn enemy\n";
				timer = 0;
			}
		}
	}

	if (prevScene)
		prevScene->Update(dt);
}

void SceneText::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GraphicsManager::GetInstance()->UpdateLightUniforms();

	// Setup 3D pipeline then render 3D
	GraphicsManager::GetInstance()->SetPerspectiveProjection(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	//testing if the stupid line working
	Vector3 lineEnd = CPlayerInfo::GetInstance()->GetMech()->legDirection;
	Vector3 playerPos = CPlayerInfo::GetInstance()->GetPos();
	Mesh* line = MeshBuilder::GetInstance()->GenerateLine(Vector3(playerPos.x, playerPos.y - 5, playerPos.z), Vector3(lineEnd.x + playerPos.x, lineEnd.y, lineEnd.z + playerPos.z), Color(0, 1, 0));
	RenderHelper::RenderMesh(line);
	delete line;
	float rotate;
	MS &ms = GraphicsManager::GetInstance()->GetModelStack();
	//render mech
	for (auto it : enemyMechList) {
		rotate = Math::RadianToDegree(atan2(it->legDirection.z, it->legDirection.x));
		ms.PushMatrix();
		ms.Translate(it->chassis->GetLeg()->GetPosition().x, it->chassis->GetLeg()->GetPosition().y, it->chassis->GetLeg()->GetPosition().z);
		ms.Rotate(-rotate + 90, 0, 1, 0);
		ms.Scale(it->chassis->GetLeg()->GetSize(), it->chassis->GetLeg()->GetSize(), it->chassis->GetLeg()->GetSize());
		RenderHelper::RenderMesh(it->legMesh);
		ms.PopMatrix();

		rotate = Math::RadianToDegree(atan2(it->torsoDirection.z, it->torsoDirection.x));
		ms.PushMatrix();
		ms.Translate(it->chassis->GetTorso()->GetPosition().x, it->chassis->GetTorso()->GetPosition().y, it->chassis->GetTorso()->GetPosition().z);
		ms.Rotate(-rotate + 90, 0, 1, 0);
		ms.Scale(it->chassis->GetTorso()->GetSize(), it->chassis->GetTorso()->GetSize(), it->chassis->GetTorso()->GetSize());
		RenderHelper::RenderMesh(it->torsoMesh);
		ms.PopMatrix();

		//render a line to enemy
		lineEnd = it->position;
		playerPos = CPlayerInfo::GetInstance()->GetPos();
		line = MeshBuilder::GetInstance()->GenerateLine(Vector3(playerPos.x, playerPos.y - 5, playerPos.z), lineEnd, Color(1, 0, 0));
		RenderHelper::RenderMesh(line);
		delete line;

		float dist = (CPlayerInfo::GetInstance()->GetPos() - it->position).Length();
		if (dist > 0 && dist < 200) {
			it->legMesh = MeshBuilder::GetInstance()->GetMesh("highLeg");
			it->torsoMesh = MeshBuilder::GetInstance()->GetMesh("highTorso");
		}
		else if (dist > 200 && dist < 400) {
			it->legMesh = MeshBuilder::GetInstance()->GetMesh("mediumLeg");
			it->torsoMesh = MeshBuilder::GetInstance()->GetMesh("mediumTorso");
		}
		else {
			it->legMesh = MeshBuilder::GetInstance()->GetMesh("lowLeg");
			it->torsoMesh = MeshBuilder::GetInstance()->GetMesh("lowTorso");
		}
	}

	//render all the path node 
	for (auto it : WaypointData::GetInstance()->nodeList)
	{
		if (it->active)
		{
			//cout << it->x << ":" << it->y << "\n";

			ms.PushMatrix();
			ms.Translate(it->x, -10, it->y);
			ms.Scale(it->size, it->size, it->size);
			RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("sphere"));
			ms.PopMatrix();

			//for (auto it2 : it->Edges)
			//{
			//	ms.PushMatrix();
			//	ms.LoadIdentity();
			//	RenderHelper::RenderMesh(it2->mesh);
			//	ms.PopMatrix();
			//}
		}
	}

	EntityManager::GetInstance()->Render();

	// Setup 2D pipeline then render 2D
	int halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2;
	int halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2;
	GraphicsManager::GetInstance()->SetOrthographicProjection(-halfWindowWidth, halfWindowWidth, -halfWindowHeight, halfWindowHeight, -10, 10);
	GraphicsManager::GetInstance()->DetachCamera();
	EntityManager::GetInstance()->RenderUI();
}

void SceneText::Exit()
{
	// Detach camera from other entities
	GraphicsManager::GetInstance()->DetachCamera();
	playerInfo->DetachCamera();

	if (playerInfo->DropInstance() == false)
	{
#if _DEBUGMODE==1
		cout << "Unable to drop PlayerInfo class" << endl;
#endif
	}

	// Delete the lights
	GraphicsManager::GetInstance()->RemoveLight("lights[0]");
	GraphicsManager::GetInstance()->RemoveLight("lights[1]");

	if (prevScene != nullptr)
		prevScene->Exit();

	//EntityManager::GetInstance()->RemoveAllEntity();
	//WaypointData::GetInstance()->DeleteWaypoints();
	//if (groundEntity)
	//	delete groundEntity;

	//std::vector<CEnemy*>::iterator it = enemyList.begin();
	//while (it != enemyList.end())
	//{
	//	// Delete if done
	//	delete *it;
	//	it = enemyList.erase(it);

	//	++it;
	//}
	
	//delete lights[0];
	//delete lights[1];
}
