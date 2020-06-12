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
