#ifndef PLATFORM_H
#define PLATFORM_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include "AnimatedSprite.hpp"

class Platform
{
public:
	Platform(sf::Vector2f t_middlePos, float t_length);

private:
	sf::Texture m_platTex;
	sf::Sprite m_left;
	sf::Sprite m_middle;
	sf::Sprite m_right;

	// instead of having to rely on the visual aspect for collisions
	// i will use this rectangleshape instead
	sf::RectangleShape m_hitbox;

public:
	sf::RectangleShape getHitbox();
	bool checkCollision(const sf::RectangleShape& t_playerHitbox);
	bool fallenOff(const sf::RectangleShape& t_playerHitbox);
	void render(sf::RenderWindow& t_window) const;
};

#endif