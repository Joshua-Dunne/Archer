#include "Background.h"

Background::Background(sf::Texture& m_tex) : m_bgTex(m_tex)
{
	m_background.setTexture(m_bgTex);
}

Background::Background(sf::Texture& m_tex, sf::Vector2f t_initPos) : m_bgTex(m_tex)
{
	m_background.setTexture(m_bgTex);
	m_background.setPosition(t_initPos);
}

void Background::render(sf::RenderWindow& t_window) const
{
	if (m_background.getPosition().x > t_window.getView().getCenter().x - m_background.getGlobalBounds().width * 2.0f
		&& m_background.getPosition().x < t_window.getView().getCenter().x + m_background.getGlobalBounds().width * 2.0f)
		t_window.draw(m_background);
}
