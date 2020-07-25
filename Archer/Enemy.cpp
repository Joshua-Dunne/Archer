#include "Enemy.h"

Enemy::Enemy() : 
	m_animSprite(sf::seconds(0.1f), false, true), 
	m_active(false), 
	m_lastPlatformCollision(0),
	m_type(EnemyType::None),
	m_placed(false),
	m_falling(false)
{
}
