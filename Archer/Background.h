#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include <math.h>

class Background
{
public:
	Background();
	Background(sf::Texture& m_tex);
	Background(sf::Texture& m_tex, sf::Vector2f t_initPos);

private:
	sf::Texture m_bgTex;
	sf::Sprite m_background;
	sf::Vector2f m_initialPos;

public:
	inline void setPosition(const sf::Vector2f& t_pos) { m_background.setPosition(t_pos); };
	inline void move(const sf::Vector2f& t_dir) { m_background.move(t_dir); };
	void render(sf::RenderWindow& t_window) const;
	inline sf::Vector2f getPosition() { return m_background.getPosition(); };
	inline void resetPos() { m_background.setPosition(m_initialPos); };
	inline sf::FloatRect getGlobalBounds() { return m_background.getGlobalBounds(); };
};

#endif