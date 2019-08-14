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
#include "Coin.h"
#include "GameButton.h"
#include "ExplosiveEffect.h"

int GSPlay::m_score = 0;
int GSPlay::m_money = 0;
GSPlay::GSPlay()
{
	m_SpawnCooldown = 0.5;
	m_SpawnCooldownHeart = 25;
	m_SpawnCooldownCoin = 1;
	m_money = 0;
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
	
	texture = ResourceManagers::GetInstance()->GetTexture("icon_coin");
	m_icon_coin = std::make_shared<Sprite2D>(model, shader, texture);
	m_icon_coin->Set2DPosition(Application::screenWidth - 125, 20);
	m_icon_coin->SetSize(25, 25);

	texture = ResourceManagers::GetInstance()->GetTexture("icon_heal");
	m_icon_heal = std::make_shared<Sprite2D>(model, shader, texture);
	m_icon_heal->Set2DPosition(20, 20);
	m_icon_heal->SetSize(25, 25);

	//text game title
	shader = ResourceManagers::GetInstance()->GetShader("TextShader");
	std::shared_ptr<Font> font = ResourceManagers::GetInstance()->GetFont("Chercher-Bold");
	m_moneyText = std::make_shared< Text>(shader, font, "MONEY", TEXT_COLOR::BLACK, 1.0);
	m_moneyText->Set2DPosition(Vector2(Application::screenWidth - 110, 25));
	m_scoreText = std::make_shared< Text>(shader, font, "SCORE", TEXT_COLOR::BLACK, 1.0);
	m_scoreText->Set2DPosition(Vector2(Application::screenWidth/2 - 80, 25));
	m_playerHealText = std::make_shared< Text>(shader, font, "HEAL", TEXT_COLOR::BLACK, 1.0);
	m_playerHealText->Set2DPosition(Vector2(35, 25));

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
	SoundManager::GetInstance()->AddSound("coin");

	
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
	if (key == 32 && bIsPressed) {
		if (m_Player->IsAlive()) {
			m_Player->Jump(1);
		}
	}
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
	float num = rand() % 2 + 2;
	if (m_score > 2000) num = rand() % 2 + 1 - (m_score / (m_score + 2000));
	if (m_score > 4000) num = 1 - (m_score / (m_score + 4000));
	if (m_score > 6000) num = 1 - (m_score / (m_score + 6000));
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
	int heartCD = 25 - (10 * (m_score)) / ((m_score) + 2000);
	if (m_SpawnCooldownHeart > 0)
	{
		m_SpawnCooldownHeart -= deltaTime;
	}
	if (m_SpawnCooldownHeart <= 0)
	{
		CreateRandomHeart();
		m_SpawnCooldownHeart = heartCD;
	}
	//Spawn random coin
	float coinCD = 1;
	if (m_score > 1000) coinCD = 1 - 0.1 * (m_score / 1000);
	if (m_score > 9000) coinCD = 0.1;
	if (m_SpawnCooldownCoin > 0)
	{
		m_SpawnCooldownCoin -= deltaTime;
	}
	if (m_SpawnCooldownCoin <= 0)
	{
		CreateRandomCoin();
		m_SpawnCooldownCoin = coinCD;
	}
	//update player
	if (m_Player->IsAlive())
	{
		m_Player->Update(deltaTime);
		//if (m_Player->CanJump()){
		//	m_Player->Jump(deltaTime);
		//}
		m_Player->CheckCollider(m_listEnermy, m_listHeart, m_listCoin);
	}
	//creat button 1 times
	if (m_CreatButton == 0) {
		if (!(m_Player->IsAlive())) {
			CreatButton();
			m_CreatButton++;
		}
	}
	//update enermy
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
	//update coin
	for (auto coin : m_listCoin)
	{
		if (coin->IsActive())
		{
			if (coin->IsExplosive())
			{
				coin->SetActive(false);
				SoundManager::GetInstance()->PlaySound("coin");
				continue;
			}
			coin->Update(deltaTime);
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
	std::string heal = " X " + stream2.str();
	m_playerHealText->setText(heal);
	std::stringstream stream3;
	stream3 << std::fixed << std::setprecision(0) << m_money;
	std::string money = " X " + stream3.str();
	m_moneyText->setText(money);
}

void GSPlay::Draw()
{
	//ground
	m_BackGround->Draw();
	m_icon_coin->Draw();
	m_icon_heal->Draw();
	//enemy
	for (auto enermy : m_listEnermy)
		if (enermy->IsActive())
			enermy->Draw();
	//heart
	for (auto heart : m_listHeart)
		if (heart->IsActive())
			heart->Draw();
	//coin
	for (auto coin : m_listCoin)
		if (coin->IsActive())
			coin->Draw();
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
	m_moneyText->Draw();
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
	m_listHeart.push_back(heart);
}

void GSPlay::CreateRandomCoin()
{
	int num = 150 + rand() % 110;
	Vector2 pos;
	pos.x = Application::screenWidth - 10;
	pos.y = num;

	for (auto coin : m_listCoin)
	{
		if (!coin->IsActive())
		{
			coin->SetActive(true);
			coin->Set2DPosition(pos);
			return;
		}
	}
	auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
	auto shader = ResourceManagers::GetInstance()->GetShader("SpriteShader");
	auto texture = ResourceManagers::GetInstance()->GetTexture("coin");

	std::shared_ptr<Coin> coin = std::make_shared<Coin>(model, shader, texture, Vector2(936, 52), Vector2(52, 52), 0, 17, 1.0);
	coin->Set2DPosition(pos);
	coin->SetSize(52, 52);
	coin->SetActive(true);
	m_listCoin.push_back(coin);
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
