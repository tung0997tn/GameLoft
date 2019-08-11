#include <sstream>
#include <iomanip>
#include <thread>

#include "GSPlay.h"
#include "Shaders.h"
#include "Texture.h"
#include "Models.h"
#include "Camera.h"
#include "Font.h"
#include "Text.h"
#include "Player.h"
#include "Enermy.h"
#include "Heart.h"
#include "GameButton.h"
#include "ExplosiveEffect.h"

int GSPlay::m_score = 0;
GSPlay::GSPlay()
{
	m_SpawnCooldown = 0.5;
	m_SpawnCooldownHeart = 25;
	m_score = 0;
	m_CreatButton = 0;
}


GSPlay::~GSPlay()
{
}


void GSPlay::Init()
{
	auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
	auto texture = ResourceManagers::GetInstance()->GetTexture("bg_play");

	//BackGround
	auto shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
	m_BackGround = std::make_shared<Sprite2D>(model, shader, texture);
	m_BackGround->Set2DPosition(Application::screenWidth / 2, Application::screenHeight / 2);
	m_BackGround->SetSize(Application::screenWidth, Application::screenHeight);
	
	//player
	//texture = ResourceManagers::GetInstance()->GetTexture("Monkey");
	//m_Player = std::make_shared<Player >(model, shader, texture);
	//m_Player->Set2DPosition(150, 345);
	//m_Player->SetSize(170, 170);

	//text game title
	shader = ResourceManagers::GetInstance()->GetShader("TextShader");
	std::shared_ptr<Font> font = ResourceManagers::GetInstance()->GetFont("Chercher-Bold");
	m_scoreText = std::make_shared< Text>(shader, font, "SCORE: ", TEXT_COLOR::BLACK, 1.0);
	m_scoreText->Set2DPosition(Vector2(10, 25));
	m_playerHealText = std::make_shared< Text>(shader, font, "HEAL: ", TEXT_COLOR::BLACK, 1.0);
	m_playerHealText->Set2DPosition(Vector2(10, 50));

	//player
	model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
	texture = ResourceManagers::GetInstance()->GetTexture("monkey1");
	shader = ResourceManagers::GetInstance()->GetShader("SpriteShader");
	m_Player = std::make_shared<Player>(model, shader, texture, Vector2(568, 340), Vector2(142, 170), 0, 7, 0.5);
	m_Player ->SetSize(170, 170);
	m_Player->Set2DPosition(150, 340);
	m_Player->SetActive(true);

	//init effect
	model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
	texture = ResourceManagers::GetInstance()->GetTexture("explosive1");
	shader = ResourceManagers::GetInstance()->GetShader("SpriteShader");
	std::shared_ptr<ExplosiveEffect> exp = std::make_shared<ExplosiveEffect>(model, shader, texture, Vector2(714, 74), Vector2(102, 74), 0, 6, 0.5);
	exp->SetSize(100, 100);
	exp->SetActive(false);
	m_listExplosiveEffect.push_back(exp);

	//init sound
	SoundManager::GetInstance()->AddSound("wrong");
	SoundManager::GetInstance()->AddSound("play_bground");
	SoundManager::GetInstance()->AddSound("jump");
	SoundManager::GetInstance()->AddSound("dead");
	
	SoundManager::GetInstance()->PlaySound("play_bground");
}

void GSPlay::CreatButton() {
	//playon button
	auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
	auto shader = ResourceManagers::GetInstance()->GetShader("TextureShader");

	auto texture = ResourceManagers::GetInstance()->GetTexture("icon_play");
	std::shared_ptr<GameButton> button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition(Application::screenWidth / 2, Application::screenHeight / 2);
	button->SetSize(200, 200);
	button->SetOnClick([]() {
		SoundManager::GetInstance()->PauseSound("play_bground");
		SoundManager::GetInstance()->PlaySound("click");
		GameStateMachine::GetInstance()->ChangeState(StateTypes::STATE_Play);
		});
	m_listButton.push_back(button);

	//reload button
	texture = ResourceManagers::GetInstance()->GetTexture("icon_load");
	button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition(Application::screenWidth / 2 - 200, Application::screenHeight / 2 + 50);
	button->SetSize(100, 100);
	button->SetOnClick([]() {
		SoundManager::GetInstance()->PauseSound("play_bground");
		SoundManager::GetInstance()->PlaySound("click");
		GameStateMachine::GetInstance()->ChangeState(StateTypes::STATE_Play);
		});
	m_listButton.push_back(button);

	//menu button
	texture = ResourceManagers::GetInstance()->GetTexture("icon_menu");
	button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition(Application::screenWidth / 2 + 200, Application::screenHeight / 2 + 50);
	button->SetSize(100, 100);
	button->SetOnClick([]() {
		SoundManager::GetInstance()->PauseSound("play_bground");
		SoundManager::GetInstance()->PlaySound("click");
		GameStateMachine::GetInstance()->ChangeState(StateTypes::STATE_Menu);
		});
	m_listButton.push_back(button);

	texture = ResourceManagers::GetInstance()->GetTexture("bg_play");


	//text game over
	shader = ResourceManagers::GetInstance()->GetShader("TextShader");
	std::shared_ptr<Font> font = ResourceManagers::GetInstance()->GetFont("Chercher-Bold");
	m_gameover = std::make_shared< Text>(shader, font, "GAME OVER", TEXT_COLOR::BLACK, 3.0);
	m_gameover->Set2DPosition(Vector2(265 , 120));

}

void GSPlay::Exit()
{

}


void GSPlay::Pause()
{

}

void GSPlay::Resume()
{

}


void GSPlay::HandleEvents()
{

}

void GSPlay::HandleKeyEvents(int key, bool bIsPressed)
{

}

void GSPlay::HandleMouseEvents(int x, int y)
{
	m_Player->MoveToPossition(Vector2(x, y));
}

void GSPlay::HandleTouchEvents(int x, int y, bool bIsPressed)
{
	for (auto it : m_listButton)
	{
		(it)->HandleTouchEvents(x, y, bIsPressed);
		if ((it)->IsHandle()) break;
	}
	if (bIsPressed) {
		if (m_Player->IsAlive()) {
			//m_Player->SetJump(true);
			m_Player->Jump(1);
		}
	}
}

void GSPlay::Update(float deltaTime)
{
	if (m_CreatButton == 1) return;

	//Spawn random enemy
	float num =rand() % 2 + 2 - (2 * (GSPlay::m_score))/((GSPlay::m_score)+2000);
	if (m_SpawnCooldown > 0)
	{
		m_SpawnCooldown -= deltaTime;
	}
	if (m_SpawnCooldown <= 0)
	{
		CreateRandomEnermy();
		m_SpawnCooldown = num;
	}

	//Spawn random heart
	int num1 = 25 - (10 * (GSPlay::m_score)) / ((GSPlay::m_score) + 2000);
	if (m_SpawnCooldownHeart > 0)
	{
		m_SpawnCooldownHeart -= deltaTime;
	}
	if (m_SpawnCooldownHeart <= 0)
	{
		CreateRandomHeart();
		m_SpawnCooldownHeart = num1;
	}
	//update player
	if (m_Player->IsAlive())
	{
		m_Player->Update(deltaTime);
		//if (m_Player->CanJump()){
		//	m_Player->Jump(deltaTime);
		//}
		m_Player->CheckCollider(m_listEnermy, m_listHeart);
	}
	//creat button 1 times
	if (m_CreatButton == 0) {
		if (!(m_Player->IsAlive())) {
			CreatButton();
			m_CreatButton++;
		}
	}
	//update enermies
	for (auto enermy : m_listEnermy)
	{
		if (enermy->IsActive())
		{
			if (enermy->IsExplosive())
			{
				enermy->SetActive(false);
				SpawnExplosive(enermy->Get2DPosition());
				continue;
			}
			enermy->Update(deltaTime);
		}
	}
	//update heart
	for (auto heart : m_listHeart)
	{
		if (heart->IsActive())
		{
			if (heart->IsExplosive())
			{
				heart->SetActive(false);
				SoundManager::GetInstance()->PlaySound("click");
				continue;
			}
			heart->Update(deltaTime);
		}
	}
	//effect
	for (auto exp : m_listExplosiveEffect)
	{
		if (exp->IsActive())
		{
			exp->Update(deltaTime);
		}
	}
	//update Score
	std::stringstream stream;
	stream << std::fixed << std::setprecision(0) << m_score;
	std::string score = "SCORE: " + stream.str();
	m_scoreText->setText(score);
	std::stringstream stream2;
	stream2 << std::fixed << std::setprecision(0) << m_Player->GetHeal();
	std::string heal = "HEAL: " + stream2.str();
	m_playerHealText->setText(heal);
}

void GSPlay::Draw()
{
	//ground
	m_BackGround->Draw();
	//enemy
	for (auto enermy : m_listEnermy)
		if (enermy->IsActive())
			enermy->Draw();
	//heart
	for (auto heart : m_listHeart)
		if (heart->IsActive())
			heart->Draw();
	//player
	if (m_Player->IsAlive())
		m_Player->Draw();
	if(!(m_Player->IsAlive())){
		m_gameover->Draw();
		for (auto it : m_listButton)
		{
			it->Draw();
		}
	}
	//effect
	for (auto exp : m_listExplosiveEffect)
	{
		if (exp->IsActive())
		{
			exp->Draw();
		}
	}
	//UI
	m_scoreText->Draw();
	m_playerHealText->Draw();
}

void GSPlay::CreateRandomEnermy()
{
	Vector2 pos;
	pos.x = Application::screenWidth - 10;
	pos.y = 360;
	int style = rand() % 3 + 1;
	for (auto enermy : m_listEnermy)
	{
		if (!enermy->IsActive())
		{
			enermy->SetActive(true);
			enermy->Set2DPosition(pos);
			return;
		}
	}
	auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
	auto shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
	auto texture = ResourceManagers::GetInstance()->GetTexture("banana_peel");
	if (style == 1) texture = ResourceManagers::GetInstance()->GetTexture("rock");
	if (style == 2) texture = ResourceManagers::GetInstance()->GetTexture("wood");
	std::shared_ptr<Enermy> enermy = std::make_shared<Enermy>(model, shader, texture);
	enermy->Set2DPosition(pos);
	enermy->SetSize(120, 120);
	//enermy->SetRotation(180);
	m_listEnermy.push_back(enermy);
}

void GSPlay::CreateRandomHeart()
{
	Vector2 pos;
	pos.x = Application::screenWidth - 10;
	pos.y = 150;

	for (auto heart : m_listHeart)
	{
		if (!heart->IsActive())
		{
			heart->SetActive(true);
			heart->Set2DPosition(pos);
			return;
		}
	}
	auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
	auto shader = ResourceManagers::GetInstance()->GetShader("SpriteShader");
	auto texture = ResourceManagers::GetInstance()->GetTexture("banana");

	std::shared_ptr<Heart> heart = std::make_shared<Heart>(model, shader, texture, Vector2(3200, 123), Vector2(160, 123), 0, 19, 1.0);
	heart->Set2DPosition(pos);
	heart->SetSize(100, 77);
	heart->SetActive(true);
	//heart->SetRotation(180);
	m_listHeart.push_back(heart);
}

void GSPlay::SpawnExplosive(Vector2 pos)
{
	for (auto exp : m_listExplosiveEffect)
	{
		if (!exp->IsActive())
		{
			exp->SetActive(true);
			exp->Set2DPosition(pos);
			SoundManager::GetInstance()->PlaySound("wrong");
			return;
		}
	}
	//animation
	auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
	auto texture = ResourceManagers::GetInstance()->GetTexture("explosive1");
	auto shader = ResourceManagers::GetInstance()->GetShader("SpriteShader");
	std::shared_ptr<ExplosiveEffect> exp = std::make_shared<ExplosiveEffect>(model, shader, texture, Vector2(714, 74), Vector2(102, 74), 0, 6, 0.5);
	exp->SetSize(160, 160);
	exp->Set2DPosition(pos);
	m_listExplosiveEffect.push_back(exp);
}
