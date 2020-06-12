#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <iostream>
#include <SFML/Graphics.hpp>

class Background
{
public:
	Background(sf::Texture& m_tex);
	Background(sf::Texture& m_tex, sf::Vector2f t_initPos);
private:
	sf::Texture m_bgTex;
	sf::Sprite m_background;
public:
	inline void setPosition(const sf::Vector2f& t_pos) { m_background.setPosition(t_pos); };
	inline void move(const sf::Vector2f& t_dir) { m_background.move(t_dir); };
	void render(sf::RenderWindow& t_window) const;
};

#endif