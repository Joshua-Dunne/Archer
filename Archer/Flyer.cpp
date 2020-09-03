#include "Flyer.h"

Flyer::Flyer(std::vector<Arrow*>& t_arrowRef, std::vector<Platform*>& t_platforms, Player* t_playerRef) :
	m_arrowRefs(t_arrowRef), m_platformRefs(t_platforms), m_playerRef(t_playerRef)
{
	m_hitbox.setSize(sf::Vector2f{ 8.0f, 8.0f });
	m_hitbox.setOrigin(sf::Vector2f{ 4.0f, 4.0f });
	m_movement = sf::Vector2f{ 0.0f, 0.0f };
	m_type = EnemyType::FlyerEnem;
}

void Flyer::setupAnimations()
{
	if (!m_spriteTexture.loadFromFile("./resources/sprites/enemies/flyer/flyerNormMega.png"))
	{
		std::cout << "Error loading flyer norm mega png" << std::endl;
	}

	m_idleAnim.setSpriteSheet(m_spriteTexture);
	m_flyAnim.setSpriteSheet(m_spriteTexture);
	m_fallAnim.setSpriteSheet(m_spriteTexture);
	m_deathAnim.setSpriteSheet(m_spriteTexture);

	for (int index = 0; index < 5; ++index) // 8 sprites for walk animation
	{
		m_deathAnim.addFrame(sf::IntRect(index * 8, 0, 8, 8));
	}

	for (int index = 0; index < 3; ++index) // 8 sprites for walk animation
	{
		m_idleAnim.addFrame(sf::IntRect(index * 8, 8, 8, 8));
	}

	for (int index = 0; index < 3; ++index) // 8 sprites for walk animation
	{
		m_flyAnim.addFrame(sf::IntRect(index * 8, 24, 8, 8));
	}

	m_fallAnim.addFrame(sf::IntRect(0, 24, 8, 8)); // only 1 frame for falling

	if (m_playerRef->getPosition().x < m_hitbox.getPosition().x)
	{
		m_animSprite.setScale(2.0f, 2.0f);
	}
	else
	{
		m_animSprite.setScale(-2.0f, 2.0f);
	}

	m_animSprite.setOrigin(4.0f, 4.0f); // flyer sprites are 8x8

	m_currAnim = &m_idleAnim;
}

void Flyer::collisionHandling(sf::Time& dt)
{
	if (!m_foundPlayer)
	{
		sf::CircleShape m_tempVision;
		m_tempVision.setRadius(m_visionRadius);
		m_tempVision.setOrigin(m_visionRadius / 2.0f, m_visionRadius / 2.0f);
		m_tempVision.setPosition(m_hitbox.getPosition());

		if (sqrt(pow((m_playerRef->getPosition().x - m_hitbox.getPosition().x), 2.0f) +
			pow((m_playerRef->getPosition().y - m_hitbox.getPosition().y), 2.0f)) < m_visionRadius)
			// if the distance between the 2 points is less than the vision radius of the Flyer enemy
		{
			m_foundPlayer = true;
			m_currAnim = &m_fallAnim;
			m_movement.y = -2.0f;
			std::cout << "Player touched Flyer" << std::endl;
		}

		for (auto& arrow : m_arrowRefs)
		{
			if (arrow->getGlobalBounds().intersects(m_tempVision.getGlobalBounds()))
			{
				m_foundPlayer = true;
				m_currAnim = &m_fallAnim;
				m_movement.y = -m_initialSpeedBurst;
				break; // break out if one is found, since we don't need to check any further ones
			}
		}
	}
}

void Flyer::activate(sf::Time& dt)
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

void Flyer::update(sf::Time& dt)
{
	activate(dt);

	if (m_active)
	{
		if (!m_dead)
		{
			movementHandling(dt);
			collisionHandling(dt); // check for collisions both before and after movement

			m_animSprite.play(*m_currAnim);

			// movement is handelled outside, so if the tracker forgets the player mid-air, they will fall down
			m_animSprite.move(m_movement.x / dt.asMilliseconds(), m_movement.y / dt.asMilliseconds());
			m_hitbox.setPosition(m_animSprite.getPosition());
			m_animSprite.update(dt);

			collisionHandling(dt);
		}
		else
		{
			m_animSprite.play(*m_currAnim);
			m_animSprite.update(dt);

			if (m_deathClock.getElapsedTime().asSeconds() > 2.0f)
			{
				m_active = false;
				m_placed = false;
				m_currAnim = &m_idleAnim; // reset animationm
				m_movement.x = 0.0f;
				m_animSprite.setLooped(true);
				m_deathClock.restart();
			}
		}
	}
}

void Flyer::render(sf::RenderWindow& t_window) const
{
	if (m_active)
	{
		t_window.draw(m_animSprite);

#ifdef _DEBUG
		sf::CircleShape m_tempVision;
		m_tempVision.setRadius(m_visionRadius);
		m_tempVision.setOrigin(m_visionRadius, m_visionRadius);
		m_tempVision.setPosition(m_hitbox.getPosition());
		m_tempVision.setFillColor(sf::Color::Cyan - sf::Color{ 0,0,0,160 }); // make the cyan transparent

		t_window.draw(m_tempVision);
#endif
	}
}

void Flyer::initialize(sf::Vector2f t_pos)
{
	m_animSprite.setPosition(t_pos);
	m_hitbox.setPosition(t_pos);

	setupAnimations();

	m_currAnim = &m_idleAnim; // Flyer will never go back to idle
	m_movement = sf::Vector2f{ 0.0f, 0.0f };

	m_active = false; // not yet active
	m_dead = false; // enemy is alive
	m_placed = true; // enemy has been placed
	m_foundPlayer = false; // has not found player
	m_flying = false; // this tracker is no longer falling

	m_lastPlatformCollision = 0;

	// we need to figure out what platform (if any) that the flyer is currently at
	for (auto& platform : m_platformRefs)
	{
		if (!platform->fallenOff(m_hitbox))
		{
			break; // since the enemy is within the range of this platform, they must be above it
			// so we dont need to continue the loop anymore
		}

		m_lastPlatformCollision++; // since it wasnt this platform, check the next one
	}

	// position the tracker so they are now on top of the platform
	m_animSprite.setPosition(m_animSprite.getPosition().x,
		m_platformRefs[m_lastPlatformCollision]->getHitbox().getPosition().y - m_hitbox.getSize().y);
	m_hitbox.setPosition(m_animSprite.getPosition());

	m_movement.y = 0; // reset y movement, so no more falling happens

}

void Flyer::movementHandling(sf::Time& dt)
{
	if (!m_dead && m_active && m_foundPlayer)
	{
		if (m_hitbox.getPosition().y < m_playerRef->getPosition().y && m_flying)
		{ // stop flapping wings if y speed exceeds 5.0f
			m_flying = false;
			m_currAnim = &m_fallAnim;
		}
		else if (m_hitbox.getPosition().y > m_playerRef->getPosition().y && !m_flying)
		{ // begin flapping wings if falling speed exceeds -3.0f
			m_flying = true;
			m_currAnim = &m_flyAnim;
		}

		if (m_flying)
		{
			if (m_movement.y > -m_maxVerticalSpeed)
			{
				m_movement.y -= m_weight;
			}
		}
		else
		{
			if (m_movement.y < m_maxVerticalSpeed)
			{
				m_movement.y += m_gravity + m_weight;
			}
		}

		if (m_movement.y < 5.1f)
		{	
			// if the flyer is shot at, it gains an initial upward burst of 6.0f
			// we don't want the flyer to begin moving left or right if they use this speed burst
			// so wait until we're back down to regular speed before deciding which way to move
			if (m_playerRef->getPosition().x < m_hitbox.getPosition().x && m_movement.x > -m_maxFlightSpeed)
			{
				m_movement.x = m_movement.x - m_horizontalSpeed;
				m_animSprite.setScale(2.0f, 2.0f);
			}
			else if (m_playerRef->getPosition().x > m_hitbox.getPosition().x && m_movement.x < m_maxFlightSpeed)
			{
				m_movement.x = m_movement.x + m_horizontalSpeed;
				m_animSprite.setScale(-2.0f, 2.0f);
			}
		}
	
	}
}
