#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "AnimatedSprite.hpp"

class Player
{
public:
	Player();

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

	int m_dimension{ 16 };

	const float m_scale{ 2.0f };
	const float m_gravity{ 0.035f };
	const float m_dashSpeed{ 1.0f };
	const float m_speed{ 0.25f };
	const float m_jumpSpeed{ 1.0f };
	const float m_dashCooldown{ 2.0f };
	const float m_dashLength{ 0.5f };

	sf::Vector2f m_movement{ 0.0f, 0.0f };


	bool m_noKeyPressed{ false };
	bool m_walkedLeft{ false };
	bool m_jumping{ false };
	bool m_stomping{ false };
	bool m_dashing{ false };

	sf::Time m_dashCounter;

	void setupAnimations();
	void dashHandling(sf::Time dt);
	void jumpHandling(sf::Time dt);
	void walkHandling(sf::Time dt);
	void stompHandling(sf::Time dt);
	void idleHandling(sf::Time dt);
	void movementHandling(sf::Time dt);

public:
	void update(sf::Time dt);
	void render(sf::RenderWindow& t_window);
};

#endif