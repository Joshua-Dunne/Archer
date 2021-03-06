#ifndef BOW_H
#define BOW_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include <math.h>
#include "Player.h"

class Bow
{
public:
	Bow(sf::RenderWindow* t_window, Player* t_player);
private:
	sf::Texture m_bowTex;
	sf::Sprite m_body;

	sf::RenderWindow* m_windowRef;
	Player* m_playerRef;

	float m_speedMultiplier{ 1.0f };
	const float m_maxMultiplier{ 3.0f };

public:
	inline void setPosition(const sf::Vector2f& t_pos) { m_body.setPosition(t_pos); };
	void render() const;
	void update(sf::Time dt);
	void lookAtMouse();
	inline float getRotation() { return m_body.getRotation(); };
	inline sf::Vector2f getPosition() { return m_body.getPosition(); };
	inline float getMovement() { return m_playerRef->movingDir(); };
	void increaseMultiplier(sf::Time& dt);
	inline void resetMultiplier() { m_speedMultiplier = 1.0f; };
	inline float getMultiplier() { return m_speedMultiplier; };
};

#endif