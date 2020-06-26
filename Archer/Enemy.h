#ifndef ENEMY_H
#define ENEMY_H

#include <iostream>
#include <vector>
#include "AnimatedSprite.hpp"
#include "globals.h"
#include "Arrow.h"
#include "Platform.h"

// Pure Virtual Class - Base of all Enemies

class Enemy
{
public:
	Enemy();
protected:
	virtual void setupAnimations() = 0;

	sf::Vector2f m_movement;
	sf::RectangleShape m_hitbox;
	float m_lastPlatformCollision;
	bool m_falling;

public:
	virtual void update(sf::Time& dt) = 0;
	virtual void render(sf::RenderWindow& t_window) = 0;
	virtual void collisionHandling(sf::Time& dt) = 0;
};

#endif