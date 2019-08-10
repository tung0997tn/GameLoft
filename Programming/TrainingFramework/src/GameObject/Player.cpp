#include "Player.h"
#include "GameManager/ResourceManagers.h"
#include <GameStates\GSPlay.h>

Player::Player(std::shared_ptr<Models>& model, std::shared_ptr<Shaders>& shader, std::shared_ptr<Texture>& texture, Vector2& pictureSize, Vector2& spriteSize, int start, int end, float lengthTime)
	:SpriteSheet(model, shader, texture, pictureSize, spriteSize, start, end, lengthTime)
{
	m_TargetPosition = Vector2(0, 0);
	m_speed = 150;
	m_MaxSpeed = 250;
	m_ground = 340;
	m_Heal = 3;
	m_SizeCollider = 50;
	m_jump = false;
	m_active = true;
	m_isAlive = true;
}

Player::~Player()
{
}


void Player::MoveToPossition(Vector2 pos)
{
	m_TargetPosition = pos;
}

bool Player::CanJump() {
	return m_jump;
}

void Player::Jump(GLfloat deltaTime)
{
	Vector2 pos = Get2DPosition();
	if (pos.y < m_ground)
		return;
	SoundManager::GetInstance()->PlaySound("jump");
	pos.y = pos.y - m_MaxSpeed;
	m_jump = false;
	Set2DPosition(pos);
}

void Player::Update(GLfloat deltaTime)
{
	if (!m_isAlive)
		return;
	SpriteSheet::Update(deltaTime);
	if (m_Heal <= 0)
	{
		SoundManager::GetInstance()->PlaySound("dead");
		m_isAlive = false;
		return;
	}
	
	Vector2 pos = Get2DPosition();
	if (pos.y < m_ground) {
		if (GSPlay::m_score <= 1000) {
			pos.y = pos.y + m_speed * deltaTime;
		}
		if (GSPlay::m_score > 1000) {
			pos.y = pos.y + (GSPlay::m_score / 1000) * m_speed * deltaTime;
		}
	}

	Set2DPosition(pos);
	GSPlay::m_score++;
}

float Player::distance(Vector2 pos, Vector2 target)
{
	return sqrt((pos.x - target.x) * (pos.x - target.x) + (pos.y - target.y) * (pos.y - target.y));
}

void Player::CheckCollider(std::vector<std::shared_ptr<Enermy>> listEnermy, std::vector<std::shared_ptr<Heart>> listHeart)
{
	Vector2 pos = Get2DPosition();
	for (auto enermy : listEnermy)
	{
		if (enermy->IsActive())
		{
			if (distance(pos, enermy->Get2DPosition()) < m_SizeCollider + enermy->GetColliderSize())
			{
				enermy->Explosive();
				m_Heal -= enermy->GetDamage();
			}
		}
	}
	for (auto heart : listHeart)
	{
		if (heart->IsActive())
		{
			if (distance(pos, heart->Get2DPosition()) < m_SizeCollider + heart->GetColliderSize())
			{
				heart->Explosive();
				m_Heal += heart->GetDamage();
			}
		}
	}
	
}

bool Player::IsActive()
{
	return m_active;
}

void Player::SetActive(bool status)
{
	m_active = status;
	if (m_active)
	{
		SpriteSheet::StartAnimation();
	}
}

void Player::SetJump(bool jump) {
	m_jump = jump;
}

void Player::SetColliderSize(float size)
{
	m_SizeCollider = size;
}
float Player::GetColliderSize()
{
	return m_SizeCollider;
}

int Player::GetHeal()
{
	return m_Heal;
}

bool Player::IsAlive()
{
	return m_isAlive;
}
