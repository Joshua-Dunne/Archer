#ifndef ARROW_H
#define ARROW_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include <Thor/Vectors.hpp>
#include "Bow.h"

class Arrow
{
public:
	Arrow() = delete;
	Arrow(Bow* t_bow, float& t_gravity, sf::Texture& t_tex);
	~Arrow();

private:
	Bow* m_bowRef;

	sf::Vector2f m_moveDir{ 0.0f, 0.0f };
	float m_rotation{ 0.0f };
	float m_gravity;

	const float m_shotSpeed{ 1.0f };

	sf::Texture m_arrowTex;
	sf::Sprite m_body;

	bool m_shot{ false };

public:
	void shoot();
	void update(sf::Time& dt);
	void collisionHandling(sf::Time& dt);
	void render(sf::RenderWindow& t_window);
	bool isShot() { return m_shot; };
};

#endif