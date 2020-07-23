#ifndef ENEMY_H
#define ENEMY_H

#include <iostream>
#include <vector>
#include "AnimatedSprite.hpp"
#include "globals.h"
#include "Arrow.h"
#include "Platform.h"
#include "Player.h"

// Pure Virtual Class - Base of all Enemies

class Enemy
{
public:
	Enemy();
protected:
	virtual void setupAnimations() = 0;
	virtual void collisionHandling(sf::Time& dt) = 0;

	sf::Vector2f m_movement;
	sf::RectangleShape m_hitbox;
	sf::Texture m_spriteTexture;
	AnimatedSprite m_animSprite;
	Animation* m_currAnim;
	int m_lastPlatformCollision;
	bool m_falling;

public:
	virtual void update(sf::Time& dt) = 0;
	virtual void render(sf::RenderWindow& t_window) = 0;
	virtual void initialize(sf::Vector2f t_pos) = 0; // used to place and prep enemies
};

#endif