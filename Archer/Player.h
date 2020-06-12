#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "AnimatedSprite.hpp"
#include "Platform.h"

class Player
{
public:
	Player() = delete; // don't use default constructor
	Player(std::vector<Platform*>& t_platforms);

private:
	sf::Texture m_walkingTex;
	sf::Texture m_idleTex;
	sf::Texture m_risingTex;
	sf::Texture m_fallingTex;
	sf::Texture m_dashTex;

	Animation* m_currentAnimation;
	Animation m_walkLeft;
	Animation m_walkRight;
	Animation m_idle;
	Animation m_risingJump;
	Animation m_fallingJump;
	Animation m_dash;

	AnimatedSprite m_animSprite;
	sf::RectangleShape m_hitbox;

	int m_dimension{ 16 };
	int m_lastPlatformCollision{ 0 };
	int m_currentPlatform{ 0 };

	const float m_lowestPos{ 600.0f };
	const float m_scale{ 2.0f };
	const float m_gravity{ 0.035f };
	const float m_dashSpeed{ 1.0f };
	const float m_speed{ 0.25f };
	const float m_jumpSpeed{ 1.0f };
	const float m_dashCooldown{ 2.0f };
	const float m_dashLength{ 0.5f };
	const float m_maxFallSpeed{ 0.6f };

	sf::Vector2f m_movement{ 0.0f, 0.0f };

	bool m_noKeyPressed{ false };
	bool m_walkedLeft{ false };
	bool m_jumping{ false };
	bool m_stomping{ false };
	bool m_dashing{ false };

	sf::Time m_dashCounter;

	std::vector<Platform*> m_platforms;

	void setupAnimations();
	void dashHandling(sf::Time dt);
	void jumpHandling(sf::Time dt);
	void walkHandling(sf::Time dt);
	void stompHandling(sf::Time dt);
	void idleHandling(sf::Time dt);
	void movementHandling(sf::Time dt);
	void collisionHandling(sf::Time dt);

public:
	void update(sf::Time dt);
	void render(sf::RenderWindow& t_window);

	inline bool isMoving() const { if (m_movement.x != 0.0f) { return true; } return false; };
	inline float movingDir() const { return m_movement.x; };
};

#endif