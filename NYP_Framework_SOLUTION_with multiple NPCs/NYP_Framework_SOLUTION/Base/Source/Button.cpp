#include "Button.h"
#include "Application.h"
#include "GameStateManagement\Option.h"
#include "SceneManager.h"

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
		printf("1200x900 was selected\n");
	}
	else if (name == "1024x720")
	{
		Application::GetInstance().SetWindowSize(1024, 720);
		OptionState::me->Exit();
		OptionState::me->Init();
		printf("1024x720 was selected\n");
	}
	else if (name == "800x600")
	{
		Application::GetInstance().SetWindowSize(800, 600);
		OptionState::me->Exit();
		OptionState::me->Init();
		printf("800x600 was selected\n");
	}
	else if (name == "back")
	{
		std::cout << "Loading MenuState" << std::endl;
		SceneManager::GetInstance()->SetActiveScene("MenuState");
	}
}