#include "Heart.h"
#include "GameManager/ResourceManagers.h"
#include <GameStates\GSPlay.h>

Heart::Heart(std::shared_ptr<Models>& model, std::shared_ptr<Shaders>& shader, std::shared_ptr<Texture>& texture, Vector2& pictureSize, Vector2& spriteSize, int start, int end, float lengthTime)
	:SpriteSheet(model, shader, texture, pictureSize, spriteSize, start, end, lengthTime)
{
	m_active = false;
	m_MaxCooldown = 0.3;
	m_Cooldown = 0.0;
	m_speed = 250;
	m_MaxSpeed = 500;
	m_Heal = 5;
	m_Damage = 1;
	m_Explosive = false;
	m_SizeCollider = 60;
}

Heart::~Heart()
{
}

void Heart::Update(float deltaTime)
{
	if (!m_active)
		return;
	SpriteSheet::Update(deltaTime);
	if (m_Heal <= 0 || m_Explosive)
	{
		m_Explosive = true;
		return;
	}

	if (m_Cooldown > 0)
	{
		m_Cooldown -= deltaTime;
	}

	Vector2 pos = Get2DPosition();
	if (GSPlay::m_score <= 1000) {
		pos.x = pos.x - m_speed * deltaTime;
	}
	if (GSPlay::m_score > 1000) {
		pos.x = pos.x - (GSPlay::m_score/1000) * m_speed * deltaTime;
	}
	Set2DPosition(pos);

	if (pos.x < 0)
		m_active = false;
}

float Heart::distance(Vector2 pos, Vector2 target)
{
	return sqrt((pos.x - target.x) * (pos.x - target.x) + (pos.y - target.y) * (pos.y - target.y));
}

bool Heart::IsActive()
{
	return m_active;
}

void Heart::SetActive(bool status)
{
	m_active = status;
	if (m_active)
	{
		SpriteSheet::StartAnimation();
	}
	m_Explosive = false;
	m_Heal = 10;
}

void Heart::SetDamage(float damage)
{
	m_Damage = damage;
}

float Heart::GetDamage()
{
	return m_Damage;
}

void Heart::Explosive()
{
	m_Explosive = true;
}

bool Heart::IsExplosive()
{
	return m_Explosive;
}

void Heart::SetColliderSize(float size)
{
	m_SizeCollider = size;
}
float Heart::GetColliderSize()
{
	return m_SizeCollider;
}
