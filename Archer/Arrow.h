#ifndef ARROW_H
#define ARROW_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Bow.h"
#include "globals.h"

class Arrow
{
public:
	Arrow() = delete;
	Arrow(Bow* t_bow, sf::Texture& t_tex);
	~Arrow();

private:
	Bow* m_bowRef;

	sf::Vector2f m_moveDir{ 0.0f, 0.0f };
	float m_rotation{ 0.0f };

	const float m_shotSpeed{ 6.0f };
	float m_weight{ 2.0f};

	sf::Texture m_arrowTex;
	sf::Sprite m_body;

	bool m_shot{ false };

public:
	void shoot();
	void update(sf::Time& dt);
	void collisionHandling(sf::Time& dt);
	void render(sf::RenderWindow& t_window) const;
	inline bool isShot() const { return m_shot; };
	inline sf::FloatRect getGlobalBounds() const { return m_body.getGlobalBounds(); };
	inline void hit() { m_shot = false; m_moveDir = sf::Vector2f{ 0.0f, 0.0f };};
};

#endif