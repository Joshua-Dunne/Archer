#include "Walker.h"

Walker::Walker(std::vector<Arrow*>& t_arrowRef, std::vector<Platform*>& t_platforms, Player* t_playerRef) :
	m_arrowRefs(t_arrowRef),
	m_platformRefs(t_platforms),
	m_playerRef(t_playerRef),
	Enemy()
{
	setupAnimations();

	m_currAnim = &m_walkAnim;

	// hitbox set up
	
	m_hitbox.setSize(sf::Vector2f{ 16.0f, 16.0f });
	m_hitbox.setOrigin(sf::Vector2f{ 8.0f, 8.0f });
	m_hitbox.setFillColor(sf::Color::Red);
	m_movement.x = -0.1f;
	m_movement.y = 0.0f;
	m_type = EnemyType::WalkerEnem;

	m_deathCounter = sf::seconds(2.0f);
}

void Walker::setupAnimations()
{
	if (!m_spriteTexture.loadFromFile("./resources/sprites/enemies/walker/walkerMega.png"))
	{
		std::cout << "Error loading walker mega png" << std::endl;
	}

	m_walkAnim.setSpriteSheet(m_spriteTexture);
	m_deathAnim.setSpriteSheet(m_spriteTexture);

	for (int index = 0; index < 8; ++index) // 8 sprites for walk animation
	{
		m_walkAnim.addFrame(sf::IntRect(index * 16, 0, 16, 16));
	}

	for (int index = 0; index < 8; ++index) // 8 sprites for walk animation
	{
		m_deathAnim.addFrame(sf::IntRect(index * 16, 32, 16, 16));
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
	activate(dt);

	if (m_active)
	{
		if (!m_dead)
		{
			if (m_falling)
			{
				m_movement.y += (m_gravity * m_weight) / dt.asMilliseconds(); //  * dt.asMilliseconds()
			}

			m_animSprite.play(*m_currAnim);

			m_animSprite.move(m_movement.x * dt.asMilliseconds(), m_movement.y * dt.asMilliseconds());
			m_hitbox.setPosition(m_animSprite.getPosition());

			collisionHandling(dt);

			m_animSprite.update(dt);
		}
		else
		{
			m_animSprite.play(*m_currAnim);
			m_animSprite.update(dt);

			if (m_deathClock.getElapsedTime().asSeconds() > m_deathCounter.asSeconds())
			{
				m_active = false;
				m_placed = false;
				m_currAnim = &m_walkAnim; // reset animationm
				m_movement.x = -0.1f;
				m_animSprite.setLooped(true);
				m_deathClock.restart();
			}
		}
	}
}

void Walker::render(sf::RenderWindow& t_window) const
{
	if (m_active)
	{
		t_window.draw(m_animSprite);
	}
}

void Walker::initialize(sf::Vector2f t_pos)
{
	m_startingPos = t_pos;

	m_animSprite.setPosition(t_pos);
	m_hitbox.setPosition(t_pos);

	m_active = false; // not yet active
	m_dead = false; // enemy is alive
	m_placed = true;

	m_lastPlatformCollision = 0;

	// we need to figure out what platform (if any) that the walker is currently at
	for (auto& platform : m_platformRefs)
	{
		if (!platform->fallenOff(m_hitbox))
		{
			break; // since the enemy is within the range of this platform, they must be above it
			// so we dont need to continue the loop anymore
		}

		m_lastPlatformCollision++; // since it wasnt this platform, check the next one
	}


	// position the walker so they are now on top of the platform
	m_animSprite.setPosition(m_animSprite.getPosition().x,
				m_platformRefs[m_lastPlatformCollision]->getHitbox().getPosition().y - m_hitbox.getSize().y);
	m_hitbox.setPosition(m_animSprite.getPosition());

	m_movement.y = 0; // reset y movement, so no more falling happens
	m_falling = false; // this walker is no longer falling
		
}

void Walker::collisionHandling(sf::Time& dt)
{
	if (!m_dead) // if the enemy is still alive
	{
		if (!m_falling)
		{
			if (m_platformRefs[m_lastPlatformCollision]->fallenOff(m_hitbox))
				// make sure the enemy isn't already falling when checking
			{ // if the enemy fell off, show it
				m_falling = true;
			}
		}

		for (auto arrow : m_arrowRefs)
		{
			if (arrow->isShot())
			{
				if (m_hitbox.getGlobalBounds().intersects(arrow->getGlobalBounds()))
				{
					// since the walker was hit, begin the death animation
#ifdef _DEBUG
					std::cout << "Walker hit!" << std::endl;
#endif
					arrow->hit(); // tell the arrow it hit something, so it can disable itself for use again
					m_dead = true;
					m_movement = sf::Vector2f{ 0.0f, 0.0f }; // stop any movement
					m_currAnim = &m_deathAnim;
					m_animSprite.setLooped(false);
					m_deathClock.restart();
				}
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

		if (m_hitbox.getPosition().y > m_screenHeight + m_hitbox.getSize().y)
		{ // if the walker falls off screen, we no longer require it to be active
			m_active = false;
			m_placed = false;
			m_dead = true;
		}

		checkPlayerCol(dt);
	}
}

void Walker::activate(sf::Time& dt)
{
	if (!m_active && !m_dead && m_placed)
	{ // make sure walker is ready to be activated, and isn't dead currently
		if (m_hitbox.getPosition().x < m_playerRef->getPosition().x + (m_screenWidth / 2.0f) + m_hitbox.getSize().x
			&& m_hitbox.getPosition().x > m_playerRef->getPosition().x - (m_screenWidth / 2.0f) + m_hitbox.getSize().x)
		{ // if the sprite is within the range of the camera (player + and - screen's width, as the player is centered)
			m_active = true;
		}
	}	
}

void Walker::checkPlayerCol(sf::Time& dt)
{
	if (m_hitbox.getGlobalBounds().intersects(m_playerRef->getHitBoxBounds()))
	{
#ifdef _DEBUG
		//std::cout << "Player touched by Walker!" << std::endl;
#endif
		m_playerRef->killPlayer();
	}
}
