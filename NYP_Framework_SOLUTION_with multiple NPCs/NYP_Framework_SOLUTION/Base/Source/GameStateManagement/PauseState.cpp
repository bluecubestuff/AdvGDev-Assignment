#include "PauseState.h"
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
#include "MouseController.h"

#include "KeyboardController.h"
#include "SceneManager.h"
#include "../KeyManager.h"
#include "../PlayerInfo/PlayerInfo.h"

CPauseState::CPauseState()
{
	prevScene = nullptr;
}
CPauseState::~CPauseState()
{

}

void CPauseState::Init()
{
	// Create and attach the camera to the scene
	//camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	camera.Init(CPlayerInfo::GetInstance()->GetPos(), CPlayerInfo::GetInstance()->GetTarget(), CPlayerInfo::GetInstance()->GetUp());
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	MeshBuilder::GetInstance()->GenerateQuad("resumeButton", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("resumeButton")->textureID = LoadTGA("Image//resumebutton.tga");

	Button* newBut = new Button("resume", 0.5, 0.5, 0.4, 0.2);
	newBut->buttonMesh = Create::Sprite2DObject("resumeButton", newBut->pos, newBut->scale);
	buttonList.push_back(newBut);

	// Load all the meshes
	MeshBuilder::GetInstance()->GenerateQuad("INTROSTATE_BKGROUND", Color(1, 1, 1), 1.f);
	MeshBuilder::GetInstance()->GetMesh("INTROSTATE_BKGROUND")->textureID = LoadTGA("Image//IntroState.tga");
	float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
	float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.0f;
	PauseStateBackground = Create::Sprite2DObject("INTROSTATE_BKGROUND",
		Vector3(halfWindowWidth, halfWindowHeight, 0.0f),
		Vector3((float)(Application::GetInstance().GetWindowWidth() * 0.8f), (float)(Application::GetInstance().GetWindowHeight() * 0.8f), 0.0f));

	MouseController::GetInstance()->SetKeepMouseCentered(false);
	Application::SetMouseVisibilty(true);

	cout << "CIntroState loaded\n" << endl;
}
void CPauseState::Update(double dt)
{
	if (prevScene)
		prevScene->Update(dt);

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
}
void CPauseState::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//GraphicsManager::GetInstance()->UpdateLightUniforms();

	// Setup 3D pipeline then render 3D
	GraphicsManager::GetInstance()->SetPerspectiveProjection(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	// Render the required entities
	EntityManager::GetInstance()->Render();

	// Setup 2D pipeline then render 2D
	GraphicsManager::GetInstance()->SetOrthographicProjection(0,
		Application::GetInstance().GetWindowWidth(),
		0,
		Application::GetInstance().GetWindowHeight(),
		-10, 10);
	GraphicsManager::GetInstance()->DetachCamera();

	// Render the required entities
	EntityManager::GetInstance()->RenderUI();
}
void CPauseState::Exit()
{
	// Remove the entity from EntityManager
	EntityManager::GetInstance()->RemoveEntity(PauseStateBackground);

	// Remove the meshes which are specific to CIntroState
	MeshBuilder::GetInstance()->RemoveMesh("INTROSTATE_BKGROUND");

	// Detach camera from other entities
	GraphicsManager::GetInstance()->DetachCamera();

	for (int i = 0; i < buttonList.size(); ++i)
	{
		EntityManager::GetInstance()->RemoveEntity(buttonList[i]->buttonMesh);
		delete buttonList[i];
		buttonList[i] = nullptr;
	}
	buttonList.clear();

	MouseController::GetInstance()->SetKeepMouseCentered(true);
	Application::SetMouseVisibilty(false);

	if (prevScene != nullptr)
		prevScene->Exit();
}