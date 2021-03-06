#include <iostream>
using namespace std;

#include "Option.h"
#include "GL\glew.h"
#include "../Application.h"
#include "LoadTGA.h"
#include "GraphicsManager.h"
#include "MeshBuilder.h"
#include "../TextEntity.h"
#include "RenderHelper.h"
#include "../SpriteEntity.h"
#include "../EntityManager.h"
#include "../Button.h"

#include "KeyboardController.h"
#include "MouseController.h"
#include "SceneManager.h"

OptionState* OptionState::me = nullptr;

OptionState::OptionState()
{
	prevScene = nullptr;
}
OptionState::~OptionState()
{

}

void OptionState::Init()
{
	// Create and attach the camera to the scene
	camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	MeshBuilder::GetInstance()->GenerateQuad("1200x900", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("1200x900")->textureID = LoadTGA("Image//1200x900.tga");
	MeshBuilder::GetInstance()->GenerateQuad("1024x720", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("1024x720")->textureID = LoadTGA("Image//1024x720.tga");
	MeshBuilder::GetInstance()->GenerateQuad("800x600", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("800x600")->textureID = LoadTGA("Image//800x600.tga");
	MeshBuilder::GetInstance()->GenerateQuad("400x300", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("400x300")->textureID = LoadTGA("Image//400x300.tga");
	MeshBuilder::GetInstance()->GenerateQuad("BackButton", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("BackButton")->textureID = LoadTGA("Image//backbutton.tga");

	worldX = Application::GetInstance().GetWindowWidth();
	worldY = Application::GetInstance().GetWindowHeight();

	Button* newBut = new Button("1200x900", 0.25, 0.75, 0.2, 0.1);
	newBut->buttonMesh = Create::Sprite2DObject("1200x900", newBut->pos, newBut->scale);
	buttonList.push_back(newBut);

	newBut = new Button("1024x720", 0.5, 0.75, 0.2, 0.1);
	newBut->buttonMesh = Create::Sprite2DObject("1024x720", newBut->pos, newBut->scale);
	buttonList.push_back(newBut);

	newBut = new Button("800x600", 0.75, 0.75, 0.2, 0.1);
	newBut->buttonMesh = Create::Sprite2DObject("800x600", newBut->pos, newBut->scale);
	buttonList.push_back(newBut);

	newBut = new Button("back", 0.75, 0.1, 0.2, 0.1);
	newBut->buttonMesh = Create::Sprite2DObject("BackButton", newBut->pos, newBut->scale);
	buttonList.push_back(newBut);

	// Load all the meshes
	MeshBuilder::GetInstance()->GenerateQuad("OPTION_BKGROUND", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("OPTION_BKGROUND")->textureID = LoadTGA("Image//optionState.tga");
	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.0f;
	OptionStateBackground = Create::Sprite2DObject("OPTION_BKGROUND",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3((float)(Application::GetInstance().GetWindowWidth()), (float)(Application::GetInstance().GetWindowHeight()), 0.0f));

	MouseController::GetInstance()->SetKeepMouseCentered(false);
	Application::SetMouseVisibilty(true);
	me = this;

	cout << "OptionState loaded\n" << endl;
}
void OptionState::Update(double dt)
{
	if (MouseController::GetInstance()->IsButtonPressed(MouseController::LMB))
	{
		double x, y;
		MouseController::GetInstance()->GetMousePosition(x, y);
		int w = Application::GetInstance().GetWindowWidth();
		int h = Application::GetInstance().GetWindowHeight();
		float posX = static_cast<float>(x);
		float posY = (h - static_cast<float>(y));

		std::cout << posX << " " << posY << '\n';

		for (auto it : buttonList)
		{
			std::cout << it->name << ": x." << it->pos.x << " y." << it->pos.y << '\n';
			if (it->CheckOnMouse(posX, posY))
			{
				it->OnClick();
				break;
			}
		}
	}

	if (prevScene)
		prevScene->Update(dt);
}
void OptionState::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//GraphicsManager::GetInstance()->UpdateLightUniforms();

	// Setup 3D pipeline then render 3D
	GraphicsManager::GetInstance()->SetPerspectiveProjection(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	// Render the required entities
	EntityManager::GetInstance()->Render();

	// Setup 2D pipeline then render 2D
	GraphicsManager::GetInstance()->SetOrthographicProjection(0, Application::GetInstance().GetWindowWidth(), 0, Application::GetInstance().GetWindowHeight(), -10, 10);
	GraphicsManager::GetInstance()->DetachCamera();

	for (auto it : buttonList)
		it->Render();

	// Render the required entities
	EntityManager::GetInstance()->RenderUI();
}
void OptionState::Exit()
{
	// Remove the entity from EntityManager
	EntityManager::GetInstance()->RemoveEntity(OptionStateBackground);

	// Remove the meshes which are specific to OptionState
	MeshBuilder::GetInstance()->RemoveMesh("OPTION_BKGROUND");

	for (int i = 0; i < buttonList.size(); ++i)
	{
		EntityManager::GetInstance()->RemoveEntity(buttonList[i]->buttonMesh);
		delete buttonList[i];
		buttonList[i] = nullptr;
	}
	buttonList.clear();

	MouseController::GetInstance()->SetKeepMouseCentered(true);
	Application::SetMouseVisibilty(false);

	// Detach camera from other entities
	GraphicsManager::GetInstance()->DetachCamera();

	if (prevScene != nullptr)
		prevScene->Exit();
}
