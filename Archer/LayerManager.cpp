#include "LayerManager.h"

LayerManager::LayerManager(sf::Vector2f t_pos, Player* t_player) : m_playerRef(t_player)
{
	if (!m_bgTwo.loadFromFile("./resources/sprites/forest/bg/background_B_layer_2.png"))
	{
		std::cout << "error loading background b layer 2 png" << std::endl;
	}

	if (!m_bgThree.loadFromFile("./resources/sprites/forest/bg/background_B_layer_3.png"))
	{
		std::cout << "error loading background b layer 3 png" << std::endl;
	}

	if (!m_bgFour.loadFromFile("./resources/sprites/forest/bg/background_B_layer_4.png"))
	{
		std::cout << "error loading background b layer 4 png" << std::endl;
	}

	m_topLayer = new Background(m_bgFour, t_pos);
	m_midLayer = new Background(m_bgThree, t_pos);
	m_backLayer = new Background(m_bgTwo, t_pos);
}

void LayerManager::topLayerHandling()
{
	// if the player moves, move this layer by their move speed
	if (m_playerRef->isMoving())
	{
		m_topLayer->move(sf::Vector2f{ m_playerRef->movingDir() * 2.0f, 0.0f });
	}


	if (m_topNearPlayer)
		// only check if the player has walked away, if the player is within the range of the background being displayed
	{
		if (m_playerRef->getPosition().x > m_topLayer->getPosition().x + (m_topLayer->getGlobalBounds().width))
		{
			m_topLayer->setPosition(sf::Vector2f{
				m_playerRef->getPosition().x + m_topLayer->getGlobalBounds().width,
				m_topLayer->getPosition().y });
				
			m_topNearPlayer = false;
		}
		else if (m_playerRef->getPosition().x < m_topLayer->getPosition().x - (m_topLayer->getGlobalBounds().width))
		{
			m_topLayer->setPosition(sf::Vector2f{
				m_playerRef->getPosition().x - m_topLayer->getGlobalBounds().width,
				m_topLayer->getPosition().y });

			m_topNearPlayer = false;
		}
	}


	// make sure the player is within the range of the background
	if (m_playerRef->getPosition().x < m_topLayer->getPosition().x + (m_topLayer->getGlobalBounds().width / 2.0f)
		&& m_playerRef->getPosition().x > m_topLayer->getPosition().x - (m_topLayer->getGlobalBounds().width / 2.0f)
		&& !m_topNearPlayer)
	{
		m_topNearPlayer = true;
		
	}
	
	
}

void LayerManager::midLayerHandling()
{
	if (m_playerRef->isMoving())
	{
		m_midLayer->move(sf::Vector2f{ m_playerRef->movingDir(), 0.0f });
	}

	if (m_midNearPlayer)
	{
		if (m_playerRef->getPosition().x > m_midLayer->getPosition().x + (m_midLayer->getGlobalBounds().width))
		{
			m_midLayer->setPosition(sf::Vector2f{
				m_playerRef->getPosition().x + m_midLayer->getGlobalBounds().width,
				m_midLayer->getPosition().y });

			m_midNearPlayer = false;
		}
		else if (m_playerRef->getPosition().x < m_midLayer->getPosition().x - (m_midLayer->getGlobalBounds().width))
		{
			m_midLayer->setPosition(sf::Vector2f{
				m_playerRef->getPosition().x - m_midLayer->getGlobalBounds().width,
				m_midLayer->getPosition().y });

			m_midNearPlayer = false;
		}
	}

	if (m_playerRef->getPosition().x < m_midLayer->getPosition().x + (m_midLayer->getGlobalBounds().width / 2.0f)
		&& m_playerRef->getPosition().x > m_midLayer->getPosition().x - (m_midLayer->getGlobalBounds().width / 2.0f)
		&& !m_midNearPlayer)
	{
		m_midNearPlayer = true;
	}
}

void LayerManager::backLayerHandling()
{
	if (m_playerRef->isMoving())
	{
		m_backLayer->move(sf::Vector2f{ m_playerRef->movingDir() * 0.5f, 0.0f });
	}

	if (m_backNearPlayer)
	{
		if (m_playerRef->getPosition().x > m_backLayer->getPosition().x + (m_backLayer->getGlobalBounds().width))
		{
			m_backLayer->setPosition(sf::Vector2f{
				m_playerRef->getPosition().x + m_backLayer->getGlobalBounds().width,
				m_backLayer->getPosition().y });

			m_backNearPlayer = false;
		}
		else if (m_playerRef->getPosition().x < m_backLayer->getPosition().x - (m_backLayer->getGlobalBounds().width))
		{
			m_backLayer->setPosition(sf::Vector2f{
				m_playerRef->getPosition().x - m_backLayer->getGlobalBounds().width,
				m_backLayer->getPosition().y });

			m_backNearPlayer = false;
		}
	}

	if (m_playerRef->getPosition().x < m_backLayer->getPosition().x + (m_backLayer->getGlobalBounds().width / 2.0f)
		&& m_playerRef->getPosition().x > m_backLayer->getPosition().x - (m_backLayer->getGlobalBounds().width / 2.0f)
		&& !m_backNearPlayer)
	{
		m_backNearPlayer = true;
	}
	
}

void LayerManager::update(sf::Time dt)
{
	topLayerHandling();
	midLayerHandling();
	backLayerHandling();

	if (m_playerRef->hasFell())
	{
		m_topLayer->resetPos();
		m_midLayer->resetPos();
		m_backLayer->resetPos();
	}
}


void LayerManager::renderTop(sf::RenderWindow& t_window)
{
	m_topLayer->render(t_window);
}

void LayerManager::renderMid(sf::RenderWindow& t_window)
{
	m_midLayer->render(t_window);
}

void LayerManager::renderBack(sf::RenderWindow& t_window)
{
	m_backLayer->render(t_window);
}
