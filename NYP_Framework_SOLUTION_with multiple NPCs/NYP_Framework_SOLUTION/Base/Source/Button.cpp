#include "Button.h"
#include "Application.h"
#include "GameStateManagement\Option.h"
#include "SceneManager.h"
#include "SceneText.h"
#include "MouseController.h"
#include "TextEntity.h"
#include "EntityManager.h"

Button::Button(std::string _name, float posX, float posY, float sizeX, float sizeY)
{
	name = _name;
	ratioPosX = posX;
	ratioPosY = posY;
	ratioSizeX = sizeX;
	ratioSizeY = sizeY;
	scale.x = Application::GetInstance().GetWindowWidth() * sizeX;
	scale.y = Application::GetInstance().GetWindowHeight() * sizeY;
	scale.z = 1;
	pos.x = Application::GetInstance().GetWindowWidth() * posX;
	pos.y = Application::GetInstance().GetWindowHeight() * posY;
	maxX = pos.x + scale.x / 2;
	maxY = pos.y + scale.y / 2;
	minX = pos.x - scale.x / 2;
	minY = pos.y - scale.y / 2;
}

void Button::Render()
{
	//MS ms = GraphicsManager::GetInstance()->GetModelStack();
	//ms.PushMatrix();
	//ms.Translate(pos.x, pos.y, -1.f);
	//ms.Scale(scale.x, scale.y, scale.z);
	//RenderHelper::RenderMesh(buttonMesh);
	//ms.PopMatrix();
}

bool Button::CheckOnMouse(double x, double y)
{
	if (x > minX && x < maxX &&
		y > minY && y < maxY)
	{
		return true;
	}
	return false;
}

void Button::OnClick()
{
	if (name == "1200x900")
	{
		Application::GetInstance().SetWindowSize(1200, 900);
		OptionState::me->Exit();
		OptionState::me->Init();
		for (int i = 0; i < 4; ++i)
		{
			EntityManager::GetInstance()->RemoveEntity(SceneText::textObj[i]);
		}
		float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
		float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.0f;
		float fontSize = 25.0f;
		float halfFontSize = fontSize / 2.0f;
		for (int i = 0; i < 4; ++i)
		{
			SceneText::textObj[i] = Create::Text2DObject("text", Vector3(-halfWindowWidth, -halfWindowHeight + fontSize*i + halfFontSize, 0.0f), "", Vector3(fontSize, fontSize, fontSize), Color(0.0f, 1.0f, 0.0f));
		}
		printf("1200x900 was selected\n");
	}
	else if (name == "1024x720")
	{
		Application::GetInstance().SetWindowSize(1024, 720);
		OptionState::me->Exit();
		OptionState::me->Init();
		for (int i = 0; i < 4; ++i)
		{
			EntityManager::GetInstance()->RemoveEntity(SceneText::textObj[i]);
		}
		float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
		float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.0f;
		float fontSize = 25.0f;
		float halfFontSize = fontSize / 2.0f;
		for (int i = 0; i < 4; ++i)
		{
			SceneText::textObj[i] = Create::Text2DObject("text", Vector3(-halfWindowWidth, -halfWindowHeight + fontSize*i + halfFontSize, 0.0f), "", Vector3(fontSize, fontSize, fontSize), Color(0.0f, 1.0f, 0.0f));
		}
		printf("1024x720 was selected\n");
	}
	else if (name == "800x600")
	{
		Application::GetInstance().SetWindowSize(800, 600);
		OptionState::me->Exit();
		OptionState::me->Init();
		for (int i = 0; i < 4; ++i)
		{
			EntityManager::GetInstance()->RemoveEntity(SceneText::textObj[i]);
		}
		float halfWindowWidth = Application::GetInstance().GetWindowWidth() / 2.0f;
		float halfWindowHeight = Application::GetInstance().GetWindowHeight() / 2.0f;
		float fontSize = 25.0f;
		float halfFontSize = fontSize / 2.0f;
		for (int i = 0; i < 4; ++i)
		{
			SceneText::textObj[i] = Create::Text2DObject("text", Vector3(-halfWindowWidth, -halfWindowHeight + fontSize*i + halfFontSize, 0.0f), "", Vector3(fontSize, fontSize, fontSize), Color(0.0f, 1.0f, 0.0f));
		}
		printf("800x600 was selected\n");
	}
	else if (name == "back")
	{
		std::cout << "Loading MenuState" << std::endl;
		SceneManager::GetInstance()->SetActiveScene("MenuState");
	}
	else if (name == "startButton")
	{
		std::cout << "Loading GameState" << std::endl;
		SceneManager::GetInstance()->SetActiveScene("GameState");
		MouseController::GetInstance()->SetKeepMouseCentered(true);
		Application::SetMouseVisibilty(false);
	}
	else if (name == "option")
	{
		std::cout << "Loading OptionState" << std::endl;
		SceneManager::GetInstance()->SetActiveScene("OptionState");
	}
	else if (name == "resume")
	{
		std::cout << "Loading GameState" << std::endl;
		SceneManager::GetInstance()->SetActiveScene("GameState");
		SceneText::isPause = false;
	}
	else if (name == "quit")
	{
		std::cout << "Loading MenuState" << std::endl;
		SceneManager::GetInstance()->SetActiveScene("MenuState");
		SceneText::isPause = false;
	}
	else if (name == "quitGame")
	{
		std::cout << "Closing Game" << std::endl;
		Application::quitGame = true;
	}
}