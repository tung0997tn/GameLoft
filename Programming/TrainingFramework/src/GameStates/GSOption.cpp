#include "GSOption.h"
/*
#include "Shaders.h"
#include "Texture.h"
#include "Models.h"
#include "Camera.h"
#include "Font.h"
#include "Sprite2D.h"
#include "Sprite3D.h"
#include "Text.h"
*/
//extern int screenWidth; //need get on Graphic engine
//extern int screenHeight; //need get on Graphic engine

GSOption::GSOption()
{
}


GSOption::~GSOption()
{

}


void GSOption::Init()
{
	auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
	auto texture = ResourceManagers::GetInstance()->GetTexture("bg_play");

	//BackGround
	auto shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
	m_BackGround = std::make_shared<Sprite2D>(model, shader, texture);
	m_BackGround->Set2DPosition(Application::screenWidth / 2, Application::screenHeight / 2);
	m_BackGround->SetSize(Application::screenWidth, Application::screenHeight);

	//back button
	texture = ResourceManagers::GetInstance()->GetTexture("icon_back");
	std::shared_ptr<GameButton> button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition(Application::screenWidth - 50, 50);
	button->SetSize(50, 50);
	button->SetOnClick([]() {
		SoundManager::GetInstance()->PlaySound("click");
		GameStateMachine::GetInstance()->ChangeState(StateTypes::STATE_Menu);
		});
	m_listButton.push_back(button);


	//text game title
//	shader = ResourceManagers::GetInstance()->GetShader("TextShader");
//	std::shared_ptr<Font> font = ResourceManagers::GetInstance()->GetFont("arialbd");
}

void GSOption::Exit()
{

}


void GSOption::Pause()
{

}

void GSOption::Resume()
{

}


void GSOption::HandleEvents()
{

}

void GSOption::HandleKeyEvents(int key, bool bIsPressed)
{

}

void GSOption::HandleTouchEvents(int x, int y, bool bIsPressed)
{
	for (auto it : m_listButton)
	{
		(it)->HandleTouchEvents(x, y, bIsPressed);
		if ((it)->IsHandle()) break;
	}
}

void GSOption::HandleMouseEvents(int x, int y)
{
}

void GSOption::Update(float deltaTime)
{
	m_BackGround->Update(deltaTime);
	for (auto it : m_listButton)
	{
		it->Update(deltaTime);
	}
}

void GSOption::Draw()
{
	m_BackGround->Draw();
	for (auto it : m_listButton)
	{
		it->Draw();
	}
	//m_Text_gameName->Draw();
}

