#pragma once
#include "gamestatebase.h"
#include "GameButton.h"

class Player;
class Bullet;
class Enermy;
class Heart;
class Coin;
class Text;
class ExplosiveEffect;

class GSPlay :
	public GameStateBase
{
public:
	GSPlay();
	~GSPlay();

	void Init()override;
	void Exit()override;

	void Pause()override;
	void Resume()override;

	void HandleEvents() override;
	void HandleKeyEvents(int key, bool bIsPressed) override;
	void HandleMouseEvents(int x, int y) override;
	void HandleTouchEvents(int x, int y, bool bIsPressed) override;
	void CreatButton();
	void Update(float deltaTime) override;
	void Draw() override;
	
	static int m_score;
	static int m_money;
private:
	std::shared_ptr<Sprite2D> m_BackGround;
	std::shared_ptr<Sprite2D> m_icon_coin;
	std::shared_ptr<Sprite2D> m_icon_heal;
	std::list<std::shared_ptr<GameButton>>	m_listButton;
	std::shared_ptr<Text>  m_scoreText;
	std::shared_ptr<Text>  m_moneyText;
	std::shared_ptr<Text>  m_playerHealText;
	std::shared_ptr<Text>  m_gameover;
	std::shared_ptr<Player> m_Player;
	std::vector<std::shared_ptr<Enermy>> m_listEnermy;
	std::vector<std::shared_ptr<Heart>> m_listHeart;
	std::vector<std::shared_ptr<Coin>> m_listCoin;
	std::vector<std::shared_ptr<ExplosiveEffect>> m_listExplosiveEffect;

	int m_CreatButton;
	float m_SpawnCooldown;
	float m_SpawnCooldownHeart;
	float m_SpawnCooldownCoin;

	void CreateRandomEnermy();
	void CreateRandomHeart();
	void CreateRandomCoin();
	void SpawnExplosive(Vector2 pos);
};

