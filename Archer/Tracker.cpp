#include "Tracker.h"

Tracker::Tracker(std::vector<Arrow*>& t_arrowRef, std::vector<Platform*>& t_platforms, Player* t_playerRef)
    : m_arrowRefs(t_arrowRef), m_platformRefs(t_platforms), m_playerRef(t_playerRef), Enemy(),
	m_memoryCounter(sf::seconds(2.0f))
{
	setupAnimations();

	std::cout << "Tracker at: X: " << m_animSprite.getPosition().x << ", Y: " << m_animSprite.getPosition().y << std::endl;

	// hitbox set up
	m_hitbox.setSize(sf::Vector2f{ 16.0f, 16.0f });
	m_hitbox.setOrigin(sf::Vector2f{ 8.0f, 8.0f });
	m_hitbox.setFillColor(sf::Color::Red);
	m_movement.x = 0.0f;
	m_movement.y = 0.0f;
	m_type = EnemyType::TrackerEnem;

	m_visionVisual.setRadius(m_visionRadius);
	m_visionVisual.setOrigin(sf::Vector2f{ m_visionRadius, m_visionRadius });
	m_visionVisual.setFillColor(sf::Color{ 255,0,0,64 });
}

void Tracker::setupAnimations()
{
	if (!m_spriteTexture.loadFromFile("./resources/sprites/enemies/tracker/trackerMega.png"))
	{
		std::cout << "Error loading tracker mega png" << std::endl;
	}

	m_idleAnim.setSpriteSheet(m_spriteTexture);
	m_walkAnim.setSpriteSheet(m_spriteTexture);
	m_attackAnim.setSpriteSheet(m_spriteTexture);
	m_deathAnim.setSpriteSheet(m_spriteTexture);
	
	for (int index = 0; index < 4; ++index) // 4 sprites for idle animation
	{
		m_idleAnim.addFrame(sf::IntRect(index * 16, 48, 16, 16));
	}

	for (int index = 0; index < 6; ++index) // 6 sprites for walk and death animation
	{
		m_walkAnim.addFrame(sf::IntRect(index * 16, 0, 16, 16));
		m_deathAnim.addFrame(sf::IntRect(index * 16, 16, 16, 16));
	}

	m_deathAnim.addFrame(sf::IntRect(64, 48, 16, 16)); // there is no empty frame for death,
	// so i set an empty space on the mega sprite to supplement it

	// attack anim sizes differ so we have to do it manually
	m_attackAnim.addFrame(sf::IntRect(0, 32, 16, 16));
	m_attackAnim.addFrame(sf::IntRect(16, 32, 16, 16));
	m_attackAnim.addFrame(sf::IntRect(32, 32, 16, 20));
	m_attackAnim.addFrame(sf::IntRect(48, 32, 16, 20));

	m_animSprite.setScale(2.0f, 2.0f);

	m_animSprite.setOrigin(8.0f, 8.0f); // all sprites are 16x16

	m_currAnim = &m_idleAnim;
}

void Tracker::collisionHandling(sf::Time& dt)
{
	if (!m_dead && m_active) // if the enemy is still alive and active
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
					// since the tracker was hit, begin the death animation
					std::cout << "Tracker hit!" << std::endl;
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
						m_falling = false; // this tracker is no longer falling
						m_lastPlatformCollision = platColl; // let the tracker know what number was the last collided with
						break;
					}
				}

				platColl++; // increase the number of platforms checked
			}
		}

		if (m_hitbox.getPosition().y > m_screenHeight + m_hitbox.getSize().y)
		{ // if the tracker falls off screen, we no longer require it to be active
			m_active = false;
			m_placed = false;
			m_dead = true;
		}


		// *** player vision radius collisions ***
		// square root of ( (x2 - x1) ^2) + ( (y2 - y1) ^2)
		// gets the distance between two points (player and tracker)
		if (m_memoryClock.getElapsedTime().asSeconds() > m_memoryCounter.asSeconds())
		{ // only perform distance checks once the tracked is about to forget about the player
			//this will help stop sqrt calls every possible frame
			if (sqrt(pow((m_playerRef->getPosition().x - m_hitbox.getPosition().x), 2.0f) +
				pow((m_playerRef->getPosition().y - m_hitbox.getPosition().y), 2.0f)) < m_visionRadius)
				// if the distance between the 2 points is less than the vision radius of the tracker enemy
			{
				m_foundPlayer = true;
				m_memoryClock.restart();
				// any frame where the tracker is still within range of the player
			}
		}
		
	}
}

void Tracker::activate(sf::Time& dt)
{
	if (!m_active && !m_dead && m_placed)
	{ // make sure tracker is ready to be activated, and isn't dead currently, and has been placed at a spawn point
		if (m_hitbox.getPosition().x < m_playerRef->getPosition().x + (m_screenWidth / 2.0f) + (m_hitbox.getSize().x * 2.0f)
			&& m_hitbox.getPosition().x > m_playerRef->getPosition().x - (m_screenWidth / 2.0f) + (m_hitbox.getSize().x * 2.0f))
		{ // if the hitbox is within the range of the camera (player + and - screen's width, as the player is centered)
			m_active = true;
		}
	}
}

void Tracker::update(sf::Time& dt)
{
	activate(dt);

	if (m_active)
	{
		if (!m_dead)
		{
			if (m_movement.x < 0.0f)
			{
				m_animSprite.setScale(-2.0f, 2.0f);
			}
			else if (m_movement.x > 0.0f)
			{
				m_animSprite.setScale(2.0f, 2.0f);
			}
			// don't do any sprite flipping if the tracker isn't moving

			if (m_falling)
			{
				m_movement.y += (m_gravity * m_weight) / dt.asMilliseconds(); //  * dt.asMilliseconds()
			}

			if (m_foundPlayer) // as long as the tracker knows the player is there
			{
				if (m_memoryClock.getElapsedTime().asSeconds() > m_memoryCounter.asSeconds())
				{ // if the tracker goes over the amount of time he can remember the player
					m_foundPlayer = false; // then the tracker will stop following the player
					m_movement.x = 0.0f; // stop any x movement now that the tracker can't see the player
					// y movement is unaffected so tracker is affected by gravity still
					std::cout << "tracker forgot player!" << std::endl;
				}
				else
				{ // otherwise keep following the player
					std::cout << "tracker following player!" << std::endl;
					// first we need to find which side the tracker is on
					if (m_hitbox.getPosition().x < m_playerRef->getPosition().x - m_hitbox.getSize().x * 2.0f)
					{ // if the tracker is on the left side of the player
						m_movement.x = 0.15f;
					}
					else if (m_hitbox.getPosition().x > m_playerRef->getPosition().x + m_hitbox.getSize().x * 2.0f)
					{ // if the tracker is on the left side of the player
						m_movement.x = -0.15f;
					}
					else
					{
						m_movement.x = 0.0f;
					}
					// if the tracker is on neither side (or perfectly standing next to the player) then no movements are needed
					// * TODO: Implement attacking if neither is true


				}
			}

			m_animSprite.play(*m_currAnim);

			// movement is handelled outside, so if the tracker forgets the player mid-air, they will fall down
			m_animSprite.move(m_movement.x * dt.asMilliseconds(), m_movement.y * dt.asMilliseconds());
			m_hitbox.setPosition(m_animSprite.getPosition());

#ifdef _DEBUG
			m_visionVisual.setPosition(m_animSprite.getPosition());
#endif

			collisionHandling(dt);

			m_animSprite.update(dt);
		}
		else
		{
			m_animSprite.play(*m_currAnim);
			m_animSprite.update(dt);

			if (m_deathClock.getElapsedTime().asSeconds() > 2.0f)
			{
				m_active = false;
				m_placed = false;
				m_currAnim = &m_walkAnim; // reset animationm
				m_movement.x = 0.0f;
				m_animSprite.setLooped(true);
				m_deathClock.restart();
			}
		}
	}
}

void Tracker::render(sf::RenderWindow& t_window) const
{
	if (m_active)
	{
		t_window.draw(m_animSprite);

#ifdef _DEBUG
		t_window.draw(m_visionVisual);
#endif
	}
}

void Tracker::initialize(sf::Vector2f t_pos)
{
	m_startingPos = t_pos;

	m_animSprite.setPosition(t_pos);
	m_hitbox.setPosition(t_pos);

	m_active = false; // not yet active
	m_dead = false; // enemy is alive
	m_placed = true; // enemy has been placed
	m_foundPlayer = false; // has not found player

	m_lastPlatformCollision = 0;

	// we need to figure out what platform (if any) that the tracker is currently at
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
	m_falling = false; // this tracker is no longer falling
}
