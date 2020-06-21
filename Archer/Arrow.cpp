#include "Arrow.h"

Arrow::Arrow(Bow* t_bow, sf::Texture& t_tex) : m_bowRef(t_bow), m_arrowTex(t_tex)
{
	m_body.setTexture(m_arrowTex);
	m_body.setOrigin(m_body.getGlobalBounds().width / 2.0f, m_body.getGlobalBounds().height / 2.0f);
	m_body.setScale(0.25f, 0.25f);
}

Arrow::~Arrow()
{
	std::cout << "arrow destructor called" << std::endl;
}

void Arrow::shoot(float t_speedMultiplier)
{
	if (!m_shot)
	{
		m_body.setPosition(m_bowRef->getPosition());

		float rot = m_bowRef->getRotation();

		// calcuate direction based on rotation
		m_moveDir.x = cos(rot * (3.14159f / 180.0f));
		m_moveDir.y = sin(rot * (3.14159f / 180.0f));

		m_body.setRotation(atan2(m_moveDir.y, m_moveDir.x) * 3.14159f / 180.0f);

		// multiply unit vector by shot speed, so it will move faster in the chosen direction
		m_moveDir *= (m_shotSpeed * t_speedMultiplier);

		m_moveDir.x += m_bowRef->getMovement(); 
		// add to x movement any movement the bow is also feeling at the moment an arrow is shot

		m_shot = true;
	}
	
}

void Arrow::update(sf::Time& dt)
{
	if (m_shot)
	{
		// check collisions twice to make sure the arrow doesn't move through platforms (or other collidables)
		collisionHandling(dt);

		m_moveDir.y += m_gravity * m_weight;

		collisionHandling(dt);

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
