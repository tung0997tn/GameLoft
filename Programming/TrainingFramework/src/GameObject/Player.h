#pragma once
#include "Sprite2D.h"
#include <Enermy.h>
#include <Heart.h>
#include <Coin.h>
#include "SpriteSheet.h"

class Player : public SpriteSheet
{
	
public:
	Player(std::shared_ptr<Models>& model, std::shared_ptr<Shaders>& shader, std::shared_ptr<Texture>& texture, Vector2& pictureSize, Vector2& spriteSize, int start, int end, float lengthTime = 1.0);
	~Player();

	void MoveToPossition(Vector2 pos);
	void Update(GLfloat deltatime) override;

	void Jump(GLfloat deltatime);
	void CheckCollider(std::vector<std::shared_ptr<Enermy>> listEnermy, std::vector<std::shared_ptr<Heart>> listHeart, std::vector<std::shared_ptr<Coin>> listCoin);

	void		SetColliderSize(float size);
	float		GetColliderSize();
	int			GetHeal();
	
	void		SetJump(bool jump);
	bool		CanJump();
	bool		IsAlive();
	bool		IsActive();
	void		SetActive(bool status);
private:
	Vector2 m_TargetPosition;
	int		m_Heal;
	bool	m_isAlive;
	float	m_speed;
	float	m_MaxSpeed;
	float	m_ground;
	float	m_SizeCollider;
	bool	m_active;
	bool	m_jump;
	float distance(Vector2 pos, Vector2 target);
};
