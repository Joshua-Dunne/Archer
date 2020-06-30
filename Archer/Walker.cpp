#include "Walker.h"

Walker::Walker(std::vector<Arrow*>& t_arrowRef, std::vector<Platform*>& t_platforms, sf::Vector2f t_startingPos) :
	m_arrowRefs(t_arrowRef),
	m_platformRefs(t_platforms),
	Enemy()
{
	setupAnimations();

	m_currAnim = &m_walkAnim;

	m_startingPos = t_startingPos;

	// hitbox set up
	
	m_animSprite.setPosition(t_startingPos);
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
	if (!m_spriteTexture.loadFromFile("./resources/sprites/enemies/walker/walkerMega.png"))
	{
		std::cout << "Error loading run anim strip png" << std::endl;
	}

	m_walkAnim.setSpriteSheet(m_spriteTexture);

	for (int index = 0; index < 8; ++index) // 8 sprites for walk animation
	{
		m_walkAnim.addFrame(sf::IntRect(index * 16, 0, 16, 16));
	}

	if (m_movement.x < 0.0f)
	{
		m_animSprite.setScale(2.0f, 2.0f);
	}
	else
	{
		m_animSprite.setScale(-2.0f, 2.0f);
	}

	m_animSprite.setOrigin(8.0f, 8.0f); // all sprites are 16x16
}

void Walker::update(sf::Time& dt)
{
	if (m_falling)
	{
		m_movement.y += (m_gravity * m_weight) / dt.asMilliseconds(); //  * dt.asMilliseconds()
	}

	m_animSprite.play(*m_currAnim);
	
	m_animSprite.move(m_movement.x * dt.asMilliseconds(), m_movement.y * dt.asMilliseconds());
	m_hitbox.setPosition(m_animSprite.getPosition());

	collisionHandling(dt);

	if (m_animSprite.getPosition().y > m_screenHeight)
	{
		m_movement.y = 0.0f;
		m_animSprite.setPosition(m_startingPos);
		m_hitbox.setPosition(m_animSprite.getPosition());
	}

	m_animSprite.update(dt);
}

void Walker::render(sf::RenderWindow& t_window)
{
	t_window.draw(m_animSprite);
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
					m_animSprite.setPosition(
						m_animSprite.getPosition().x,
						platform->getHitbox().getPosition().y - m_animSprite.getGlobalBounds().width / 2.0f);
					m_hitbox.setPosition(m_animSprite.getPosition());

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
