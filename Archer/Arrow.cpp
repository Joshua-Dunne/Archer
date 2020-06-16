#include "Arrow.h"

Arrow::Arrow(Bow* t_bow, float& t_gravity, sf::Texture& t_tex) : m_bowRef(t_bow), m_gravity(t_gravity), m_arrowTex(t_tex)
{
	m_body.setTexture(m_arrowTex);
	m_body.setOrigin(m_body.getGlobalBounds().width / 2.0f, m_body.getGlobalBounds().height / 2.0f);
}

Arrow::~Arrow()
{
	std::cout << "arrow destructor called" << std::endl;
}

void Arrow::shoot()
{
	if (!m_shot)
	{
		m_body.setPosition(m_bowRef->getPosition());

		// get the direction the vector will face based on the rotation of the bow
		sf::Vector2f rotationVector = thor::rotatedVector(m_moveDir, m_body.getRotation());

		// get a smaller fraction of that direction vector
		if(m_moveDir.x != 0 || m_moveDir.y != 0)
			m_moveDir = m_moveDir / thor::length(m_moveDir);

		// multiply unit vector by shot speed, so it will move faster in the chosen direction
		m_moveDir *= m_shotSpeed; 

		m_body.setRotation(atan2(m_moveDir.y, m_moveDir.x) * 180.0f / 3.14159f);

		m_shot = true;
	}
	
}

void Arrow::update(sf::Time& dt)
{
	if (m_shot)
	{
		collisionHandling(dt);

		m_moveDir.y += m_gravity;

		collisionHandling(dt);

		std::cout << m_moveDir.y << std::endl;

		m_body.move(m_moveDir);
		m_body.setRotation(atan2(m_moveDir.y, m_moveDir.x) * 180.0f / 3.14159f);
	}
}

void Arrow::collisionHandling(sf::Time& dt)
{
	if (m_body.getPosition().y > 800.0f + m_body.getGlobalBounds().height / 2.0f)
	{ // if the arrow has gone off the bottom of the screen
		m_shot = false;
		m_moveDir = sf::Vector2f{ 0.0f, 0.0f };
		// we no longer need this arrow
	}
}

void Arrow::render(sf::RenderWindow& t_window)
{
	if (m_shot) 
	{ 
		t_window.draw(m_body); 
	}
}
