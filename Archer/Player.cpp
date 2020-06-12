#include "Player.h"

Player::Player(std::vector<Platform*>& t_platforms) : 
	m_animSprite(sf::seconds(0.1f), false, true), 
	m_currentAnimation{ nullptr },
	m_platforms(t_platforms)
{
	setupAnimations();

	m_currentAnimation = &m_idle;

	m_animSprite.setOrigin(8.0f, 8.0f); // all sprites are 16x16
	m_animSprite.setPosition(sf::Vector2f{ 200.0f, 50.0f });
	m_animSprite.setScale(m_scale, m_scale);

	m_hitbox.setSize(sf::Vector2f{ 16.0f, 16.0f });
	m_hitbox.setOrigin(sf::Vector2f{ 8.0f, 8.0f });
	m_hitbox.setPosition(m_animSprite.getPosition());

	m_dashCounter += sf::seconds(m_dashCooldown); // make it so the player can dash initially without having to wait
	m_jumping = true;
	m_stomping = false;
	m_movement.y = 0.1f;
}


void Player::setupAnimations()
{
	if (!m_walkingTex.loadFromFile("./resources/sprites/herochar_run_anim_strip_6.png"))
	{
		std::cout << "Error loading run anim strip png" << std::endl;
	}

	if (!m_idleTex.loadFromFile("./resources/sprites/herochar_idle_anim_strip_4.png"))
	{
		std::cout << "Error loading idle anim strip png" << std::endl;
	}

	if (!m_risingTex.loadFromFile("./resources/sprites/herochar_jump_up_anim_strip_3.png"))
	{
		std::cout << "Error loading jump up anim strip png" << std::endl;

	}

	if (!m_fallingTex.loadFromFile("./resources/sprites/herochar_jump_down_anim_strip_3.png"))
	{
		std::cout << "Error loading jump down anim strip png" << std::endl;
	}

	if (!m_dashTex.loadFromFile("./resources/sprites/herochar_pushing_foward_anim_strip_6.png"))
	{
		std::cout << "Error loading jump down anim strip png" << std::endl;
	}


	m_walkRight.setSpriteSheet(m_walkingTex);
	m_walkLeft.setSpriteSheet(m_walkingTex);
	m_dash.setSpriteSheet(m_dashTex);
	for (int index = 0; index < 6; ++index)
	{
		m_walkRight.addFrame(sf::IntRect(index * m_dimension, 0, m_dimension, m_dimension)); // top row
		m_walkLeft.addFrame(sf::IntRect(index * m_dimension, m_dimension, m_dimension, m_dimension)); // bottom row
		m_dash.addFrame(sf::IntRect(index * m_dimension, 0, m_dimension, m_dimension));
	}


	m_idle.setSpriteSheet(m_idleTex);

	for (int index = 0; index < 4; ++index)
	{
		m_idle.addFrame(sf::IntRect(index * m_dimension, 0, m_dimension, m_dimension));
	}

	m_risingJump.setSpriteSheet(m_risingTex);
	for (int index = 0; index < 3; ++index)
	{
		m_risingJump.addFrame(sf::IntRect(index * m_dimension, 0, m_dimension, m_dimension));
	}

	m_fallingJump.setSpriteSheet(m_fallingTex);
	for (int index = 0; index < 3; ++index)
	{
		m_fallingJump.addFrame(sf::IntRect(index * m_dimension, 0, m_dimension, m_dimension));
	}
}

void Player::dashHandling(sf::Time dt)
{
	m_dashCounter += dt;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
	{
		if (m_dashCounter.asSeconds() >= m_dashCooldown && !m_stomping && !m_dashing)
		{
			m_dashCounter = sf::seconds(0.0f);
			m_currentAnimation = &m_dash;
			m_dashing = true;

			if (m_walkedLeft)
			{
				m_animSprite.setScale(-m_scale, m_scale);
			}

			if (m_movement.y > 0.1f)
			{
				m_movement.y = 0.1f;
			}
		}
	}

	if (m_dashing && m_dashCounter.asSeconds() < 0.5f)
	{
		if (m_walkedLeft) // dash in the direction the player was moving
		{
			if (m_animSprite.getPosition().x > m_animSprite.getGlobalBounds().width / 2.0f)
				m_movement.x = -m_speed * 1.5f;

			m_animSprite.setScale(-m_scale, m_scale);
		}
		else
		{
			if (m_animSprite.getPosition().x < 800.0f - m_animSprite.getGlobalBounds().width / 2.0f)
				m_movement.x = m_speed * 1.5f;

			m_animSprite.setScale(m_scale, m_scale);
		}
	}
	else if (m_dashCounter.asSeconds() >= m_dashLength && m_dashing)
	{
		m_dashing = false;
		m_currentAnimation = &m_idle;

		if (m_walkedLeft)
		{
			m_animSprite.setScale(-m_scale, m_scale);
		}
		else
		{
			m_animSprite.setScale(m_scale, m_scale);
		}
	}
}

void Player::jumpHandling(sf::Time dt)
{
	if (m_jumping && !m_dashing)
	{
		m_movement.y += m_gravity;
	}
	else if (m_jumping && m_dashing && m_movement.y < 0.0f)
	{
		m_movement.y += m_gravity;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !m_jumping)
	{
		m_jumping = true;
		m_movement.y = -m_jumpSpeed;

		if (m_walkedLeft)
		{
			m_animSprite.setScale(-m_scale, m_scale);
		}
		else
		{
			m_animSprite.setScale(m_scale, m_scale);
		}
	}

	if (m_jumping && m_movement.y < 0.0f)
	{
		m_currentAnimation = &m_risingJump;

		if (m_walkedLeft)
		{
			m_animSprite.setScale(-m_scale, m_scale);
		}
		else
		{
			m_animSprite.setScale(m_scale, m_scale);
		}
	}
	else if (m_jumping && m_movement.y >= 0.0f)
	{
		m_currentAnimation = &m_fallingJump;

		if (m_walkedLeft)
		{
			m_animSprite.setScale(-m_scale, m_scale);
		}
		else
		{
			m_animSprite.setScale(m_scale, m_scale);
		}
	}
}

void Player::walkHandling(sf::Time dt)
{
	m_animSprite.setFrameTime(sf::seconds(0.1f));

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !m_stomping && !m_dashing)
	{
		if (m_animSprite.getPosition().x > + m_animSprite.getGlobalBounds().width / 2.0f)
		{
			if (!m_jumping)
			{
				m_currentAnimation = &m_walkLeft;
			}

			m_movement.x = -m_speed;
			m_noKeyPressed = false;
			m_walkedLeft = true;
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !m_stomping && !m_dashing)
	{
		if (m_animSprite.getPosition().x < 800.0f - m_animSprite.getGlobalBounds().width / 2.0f)
		{
			if (!m_jumping)
			{
				m_currentAnimation = &m_walkRight;
			}

			m_movement.x = m_speed;
			m_noKeyPressed = false;
			m_walkedLeft = false;
		}
	}
}

void Player::stompHandling(sf::Time dt)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && m_jumping && m_movement.y > 0.1f && !m_dashing)
	{
		m_movement.y = m_jumpSpeed;
		m_movement.x = 0.0f;
		m_stomping = true;
	}
}

void Player::idleHandling(sf::Time dt)
{
	// if no key was pressed stop the animation
	if (m_noKeyPressed && !m_jumping && !m_dashing)
	{
		m_animSprite.setFrameTime(sf::seconds(0.2f)); // move the idle animation ever so slightly faster
		m_movement.x = 0;
		m_currentAnimation = &m_idle;

		if (m_walkedLeft)
		{
			m_animSprite.setScale(-m_scale, m_scale);
		}
	}
	else if (m_noKeyPressed && m_jumping && !m_dashing)
	{
		m_movement.x = 0; // if they aren't moving, but jumping (and not currently dashing), put movement back to 0
	}
}

void Player::movementHandling(sf::Time dt)
{
	if (!m_dashing || (m_dashing && m_movement.y < 0.0f))
		m_animSprite.move(m_movement.x * dt.asMilliseconds(), m_movement.y * dt.asMilliseconds());
	else
		m_animSprite.move(m_movement.x * dt.asMilliseconds(), 0.0f);
}

void Player::collisionHandling(sf::Time dt)
{
	if (m_jumping)
	{
		m_lastPlatformCollision = 0; // start back at 0 for the last platform checked for collision

		for (auto platform : m_platforms)
		{
			if (platform->checkCollision(m_hitbox))
			{
				if (m_animSprite.getPosition().y < platform->getHitbox().getPosition().y)
				{ // if the player was above the hitbox
					// set player to the platform's y position (since x doesn't need to change)
					m_animSprite.setPosition(
						m_animSprite.getPosition().x,
						platform->getHitbox().getPosition().y - m_animSprite.getGlobalBounds().width / 2.0f);

					m_jumping = false;
					m_stomping = false;
					m_movement.y = 0;
					break;
				}
			}

			m_lastPlatformCollision++; // increase the number of platforms checked
		}
	}

	

	if (!m_jumping && !m_dashing)
	{	// we will need to check to see if the player isn't currently jumping
		// or currently dashing to see if the player can fall once more
		bool notAbovePlatform{ false };

		if (m_platforms[m_lastPlatformCollision]->fallenOff(m_hitbox))
			notAbovePlatform = true;
		

		if (notAbovePlatform)
		{
			m_jumping = true;
			m_movement.y = 0.1f;
		}
	}
}

void Player::update(sf::Time dt)
{
	m_animSprite.setScale(m_scale, m_scale);

	walkHandling(dt);
	jumpHandling(dt);
	dashHandling(dt);
	stompHandling(dt);
	idleHandling(dt);
	
	if (m_animSprite.getPosition().y > m_lowestPos)
	{
		m_jumping = true;
		m_stomping = false;
		m_movement.y = 0.1f;
		m_animSprite.setPosition(200.0f, 50.0f);
	}

	collisionHandling(dt);

	m_animSprite.play(*m_currentAnimation);

	movementHandling(dt);

	m_hitbox.setPosition(m_animSprite.getPosition()); // move the hitbox with the player sprite

	m_noKeyPressed = true;

	m_animSprite.update(dt);
}

void Player::render(sf::RenderWindow& t_window)
{
	t_window.draw(m_animSprite);
}
