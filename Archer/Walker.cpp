#include "Walker.h"

Walker::Walker(std::vector<Arrow*>& t_arrowRef, std::vector<Platform*>& t_platforms, sf::Vector2f t_startingPos) : 
	m_arrowRefs(t_arrowRef), 
	m_platformRefs(t_platforms)
{
	// hitbox set up
	m_hitbox.setSize(sf::Vector2f{ 16.0f, 16.0f });
	m_hitbox.setOrigin(sf::Vector2f{ 8.0f, 8.0f });
	m_hitbox.setFillColor(sf::Color::Red);
	m_hitbox.setPosition(t_startingPos);
	m_movement.x = -0.10f;
	m_movement.y = 0.0f;
	m_falling = true;
	m_lastPlatformCollision = 0;

	// we need to figure out what platform (if any) that the walker is currently above
	for (auto& platform : m_platformRefs)
	{
		if (!platform->fallenOff(m_hitbox))
		{
			break; // since the enemy is within the range of this platform, they must be above it
			// so we dont need to continue the loop anymore
		}
		
		m_lastPlatformCollision++; // since it wasnt this platform, check the next one
	}
}

void Walker::setupAnimations()
{
	// include code to set up animations for Walker
}

void Walker::update(sf::Time& dt)
{
	if (m_falling)
	{
		m_movement.y += (m_gravity * m_weight) / dt.asMilliseconds(); //  * dt.asMilliseconds()
	}

	collisionHandling(dt);
	
	m_hitbox.move(m_movement.x * dt.asMilliseconds(), m_movement.y * dt.asMilliseconds());
}

void Walker::render(sf::RenderWindow& t_window)
{
	t_window.draw(m_hitbox);
}

void Walker::collisionHandling(sf::Time& dt)
{
	if (!m_falling)
	{
		if (m_platformRefs[m_lastPlatformCollision]->fallenOff(m_hitbox))
			// make sure the enemy isn't already falling when checking
		{ // if the enemy fell off, show it
			m_falling = true;
		}
	}
	

	if (m_movement.y > 0.0f && m_falling)
	{
		int platColl = 0; // start back at 0 for the last platform checked for collision

		for (auto platform : m_platformRefs)
		{
			if (platform->checkCollision(m_hitbox))
			{
				if (m_hitbox.getPosition().y < platform->getHitbox().getPosition().y)
				{ // check to see if enemy has touched hitbox
					m_hitbox.setPosition(
						m_hitbox.getPosition().x,
						platform->getHitbox().getPosition().y - m_hitbox.getGlobalBounds().width / 2.0f);

					m_movement.y = 0; // reset y movement, so no more falling happens
					m_falling = false; // this walker is no longer falling
					m_lastPlatformCollision = platColl; // let the walker know what number was the last collided with
					break;
				}
			}

			platColl++; // increase the number of platforms checked
		}
	}
}
