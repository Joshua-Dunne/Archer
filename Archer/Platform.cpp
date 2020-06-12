#include "Platform.h"

/// <summary>
/// Overloaded constructor to allow user to place platforms and determine length of platforms
/// </summary>
/// <param name="t_middlePos">Position of middle piece, to place left and right pieces</param>
/// <param name="t_length">Length of middle piece</param>
Platform::Platform(sf::Vector2f t_middlePos, float t_length)
{
	if (!m_platTex.loadFromFile("./resources/sprites/forest/tiles/tileset.png"))
	{
		std::cout << "error loading forest tileset png" << std::endl;
	}

	m_left.setTexture(m_platTex);
	m_left.setTextureRect(sf::IntRect(0, 64, 45, 128));

	m_right.setTexture(m_platTex);
	m_right.setTextureRect(sf::IntRect(146, 64, 45, 128));

	
	m_middle.setTexture(m_platTex);
	m_middle.setTextureRect(sf::IntRect(45, 64, 101, 128));

	
	m_middle.setOrigin(sf::Vector2f{
		m_middle.getGlobalBounds().width / 2.0f,
		m_middle.getGlobalBounds().height / 2.0f });

	// set at specified position
	m_middle.setPosition(t_middlePos);
	m_middle.setScale(t_length, 1.0f);

	m_left.setPosition(
		sf::Vector2f{ t_middlePos.x - ((m_middle.getGlobalBounds().width / 2.0f) + m_left.getGlobalBounds().width), 
		t_middlePos.y - m_middle.getGlobalBounds().height / 2.0f
		});

	m_right.setPosition(
		sf::Vector2f{ t_middlePos.x + ((m_middle.getGlobalBounds().width / 2.0f)),
		t_middlePos.y - m_middle.getGlobalBounds().height / 2.0f
		});

	
	// now that the visuals have been set up, we will now set up the hitbox
	m_hitbox.setPosition(m_left.getPosition()); // set the hitbox to start at the left most part of the platform
	m_hitbox.setSize(sf::Vector2f{
		m_left.getGlobalBounds().width + m_middle.getGlobalBounds().width + m_right.getGlobalBounds().width,
		32.0f});

	m_hitbox.setFillColor(sf::Color(255,0,0,128)); // transparent red
}

sf::RectangleShape Platform::getHitbox()
{
	return m_hitbox;
}

bool Platform::checkCollision(const sf::RectangleShape& t_playerHitbox)
{
	return m_hitbox.getGlobalBounds().intersects(t_playerHitbox.getGlobalBounds());
}

bool Platform::fallenOff(const sf::RectangleShape& t_playerHitbox)
{
	if (t_playerHitbox.getPosition().x < m_hitbox.getPosition().x
		|| t_playerHitbox.getPosition().x > m_hitbox.getPosition().x + m_hitbox.getGlobalBounds().width)
	{
		return true;
	}

	return false;
}

void Platform::render(sf::RenderWindow& t_window) const
{
	t_window.draw(m_middle);
	t_window.draw(m_left);
	t_window.draw(m_right);

#ifdef _DEBUG
	t_window.draw(m_hitbox);
#endif
	
}
