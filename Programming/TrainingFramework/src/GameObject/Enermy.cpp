#include "Enermy.h"
#include "GameManager/ResourceManagers.h"
#include <GameStates\GSPlay.h>

Enermy::Enermy(std::shared_ptr<Models>& model, std::shared_ptr<Shaders>& shader, std::shared_ptr<Texture>& texture)
	:Sprite2D(model, shader, texture)
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

Enermy::~Enermy()
{
}

void Enermy::Update(float deltaTime)
{
	if (!m_active)
		return;

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
		pos.x = pos.x - (GSPlay::m_score / 1000) * m_speed * deltaTime;
	}
	Set2DPosition(pos);

	if (pos.x < 0)
		m_active = false;
}

float Enermy::distance(Vector2 pos, Vector2 target)
{
	return sqrt((pos.x - target.x) * (pos.x - target.x) + (pos.y - target.y) * (pos.y - target.y));
}

bool Enermy::IsActive()
{
	return m_active;
}

void Enermy::SetActive(bool status)
{
	m_active = status;
	m_Explosive = false;
	m_Heal = 10;
}

void Enermy::SetDamage(float damage)
{
	m_Damage = damage;
}

float Enermy::GetDamage()
{
	return m_Damage;
}

void Enermy::Explosive()
{
	m_Explosive = true;
}

bool Enermy::IsExplosive()
{
	return m_Explosive;
}


void Enermy::SetColliderSize(float size)
{
	m_SizeCollider = size;
}
float Enermy::GetColliderSize()
{
	return m_SizeCollider;
}