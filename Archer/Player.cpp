#include "Player.h"

Player::Player() : m_animSprite(sf::seconds(0.1f), false, true), m_currentAnimation{nullptr}
{
	// Load textures and set up animations for later use
	setupAnimations();

	m_currentAnimation = &m_idle;

	m_animSprite.setOrigin(8.0f, 8.0f); // all sprites are 16x16
	m_animSprite.setPosition(sf::Vector2f{ 400.0f, 300.0f });
	m_animSprite.setScale(m_scale, m_scale);

	m_dashCounter += sf::seconds(m_dashCooldown); // make it so the player can dash initially without having to wait
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
		}
	}

	if (m_dashing && m_dashCounter.asSeconds() < 0.5f)
	{
		if (m_walkedLeft) // dash in the direction the player was moving
		{
			m_movement.x = -m_speed * 1.5f;
			m_animSprite.setScale(-m_scale, m_scale);
		}
		else
		{
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
		if (!m_jumping)
		{
			m_currentAnimation = &m_walkLeft;
		}

		m_movement.x = -m_speed;
		m_noKeyPressed = false;
		m_walkedLeft = true;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !m_stomping && !m_dashing)
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

void Player::update(sf::Time dt)
{
	m_animSprite.setScale(m_scale, m_scale);

	walkHandling(dt);
	jumpHandling(dt);
	dashHandling(dt);
	stompHandling(dt);
	idleHandling(dt);
	
	if (m_animSprite.getPosition().y > 300.0f)
	{
		m_jumping = false;
		m_stomping = false;
		m_movement.y = 0;
		m_animSprite.setPosition(m_animSprite.getPosition().x, 300.0f);
	}

	m_animSprite.play(*m_currentAnimation);

	movementHandling(dt);

	m_noKeyPressed = true;

	m_animSprite.update(dt);
}

void Player::render(sf::RenderWindow& t_window)
{
	t_window.draw(m_animSprite);
}
