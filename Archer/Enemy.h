#ifndef ENEMY_H
#define ENEMY_H

#include <iostream>
#include <vector>
#include "AnimatedSprite.hpp"
#include "globals.h"
#include "Arrow.h"

// Pure Virtual Class - Base of all Enemies

class Enemy
{
public:
	Enemy();
protected:
	virtual void setupAnimations() = 0;

	sf::RectangleShape m_hitbox;

public:
	virtual void update(sf::Time& dt) = 0;
	virtual void render(sf::RenderWindow& t_window) = 0;
};

#endif