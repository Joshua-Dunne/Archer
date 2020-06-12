#include "Game.h"

Game::Game() : m_window(sf::VideoMode(800, 600), "Archer")
{
	m_window.setFramerateLimit(60u);

	if (!m_font.loadFromFile("./resources/fonts/TrulyHorrible.ttf"))
	{
		std::cout << "error loading TurlyHorrible font" << std::endl;
	}

	m_instructions.setFont(m_font);
	m_instructions.setCharacterSize(16u);
	m_instructions.setString("A/D Key: Move Left/Right\nSpace/W Key: Jump\nShift: Dash\nS Key (while mid-air): Stomp");
	m_instructions.setPosition(sf::Vector2f{ 250.0f, 525.0f });

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

	m_backLayers.push_back(new Background(m_bgTwo, sf::Vector2f{ -1540.0f, 0.0f }));
	m_backLayers.push_back(new Background(m_bgTwo, sf::Vector2f{ 100.0f, 0.0f }));
	m_backLayers.push_back(new Background(m_bgTwo, sf::Vector2f{ 1540.0f, 0.0f }));
	m_middleLayers.push_back(new Background(m_bgThree, sf::Vector2f{ -1440.0f, 0.0f }));
	m_middleLayers.push_back(new Background(m_bgThree, sf::Vector2f{ 0.0f, 0.0f }));
	m_middleLayers.push_back(new Background(m_bgThree, sf::Vector2f{ 1440.0f, 0.0f }));
	m_topLayers.push_back(new Background(m_bgFour, sf::Vector2f{ -1340.0f, 0.0f }));
	m_topLayers.push_back(new Background(m_bgFour, sf::Vector2f{ -100.0f, 0.0f }));
	m_topLayers.push_back(new Background(m_bgFour, sf::Vector2f{ 1340.0f, 0.0f }));


	m_platforms.push_back(new Platform(sf::Vector2f{ 150.0f, 250.0f }, 1.0f));
	m_platforms.push_back(new Platform(sf::Vector2f{ 650.0f, 250.0f }, 1.0f));

	m_platforms.push_back(new Platform(sf::Vector2f{ 400.0f, 450.0f }, 2.0f));
	m_platforms.push_back(new Platform(sf::Vector2f{ 1200.0f, 450.0f}, 8.0f));
	m_platforms.push_back(new Platform(sf::Vector2f{ 2000.0f, 250.0f }, 8.0f));

	m_player = new Player(m_platforms);

	m_bow = new Bow(&m_window, m_player);

	m_instructBg.setPosition(m_instructions.getPosition() - sf::Vector2f{ 10.0f, 10.0f });
	m_instructBg.setSize(sf::Vector2f{ m_instructions.getGlobalBounds().width + 20.0f, m_instructions.getGlobalBounds().height + 20.0f });
	m_instructBg.setFillColor(sf::Color::Black);
}

Game::~Game()
{
	delete m_player;
	delete m_bow;
	delete[] &m_backLayers;
	delete[] &m_middleLayers;
	delete[] &m_topLayers;

	m_platforms.clear();
}

/// <summary>
/// Run the main Game loop
/// </summary>
void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	sf::Time timePerFrame = sf::seconds(1.f / 60.f);

	while (m_window.isOpen())
	{
		processInput();
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			update(timePerFrame);
		}
		render();
	}
}

void Game::processInput()
{
	sf::Event event;

	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			m_window.close();
		}
	}
}

void Game::update(sf::Time dt)
{
	m_player->update(dt);
	if (m_player->hasFell())
	{
		for (auto topLayer : m_topLayers)
		{
			topLayer->resetPos();
		}

		for (auto midLayer : m_middleLayers)
		{
			midLayer->resetPos();
		}

		for (auto backLayer : m_backLayers)
		{
			backLayer->resetPos();
		}
	}

	if (m_player->isMoving())
	{
		float xMovement = m_player->movingDir();

		for (auto topLayer : m_topLayers)
		{
			topLayer->move(sf::Vector2f{ xMovement * 2.0f, 0.0f });
		}

		for (auto midLayer : m_middleLayers)
		{
			midLayer->move(sf::Vector2f{ xMovement, 0.0f });
		}

		for (auto backLayer : m_backLayers)
		{
			backLayer->move(sf::Vector2f{ xMovement * 0.5f, 0.0f });
		}
	}

	// ----------------------------------------------------------------------------------------------------
	// Layer Management (Make a Background Manager!!)
	if (m_backLayers[0]->getDistanceToPlayer(m_player->getPosition()) > 2000.0f
		&& m_backLayers[0]->getPosition().x < m_player->getPosition().x - 400.0f)
	{
		if (m_backLayers[1]->getPosition().x > m_backLayers[2]->getPosition().x)
		{
			m_backLayers[0]->setPosition(sf::Vector2f{ m_backLayers[1]->getPosition().x + 1440.0f, m_backLayers[1]->getPosition().y });
		}
		else
		{
			m_backLayers[0]->setPosition(sf::Vector2f{ m_backLayers[2]->getPosition().x + 1440.0f, m_backLayers[2]->getPosition().y });
		}
	}

	if (m_backLayers[1]->getDistanceToPlayer(m_player->getPosition()) > 2000.0f
		&& m_backLayers[1]->getPosition().x < m_player->getPosition().x - 400.0f)
	{
		if (m_backLayers[2]->getPosition().x > m_backLayers[0]->getPosition().x)
		{
			m_backLayers[1]->setPosition(sf::Vector2f{ m_backLayers[2]->getPosition().x + 1440.0f, m_backLayers[2]->getPosition().y });
		}
		else
		{
			m_backLayers[1]->setPosition(sf::Vector2f{ m_backLayers[0]->getPosition().x + 1440.0f, m_backLayers[0]->getPosition().y });
		}
	}

	if (m_backLayers[2]->getDistanceToPlayer(m_player->getPosition()) > 2000.0f
		&& m_backLayers[2]->getPosition().x < m_player->getPosition().x - 400.0f)
	{
		if (m_backLayers[1]->getPosition().x > m_backLayers[0]->getPosition().x)
		{
			m_backLayers[2]->setPosition(sf::Vector2f{ m_backLayers[1]->getPosition().x + 1440.0f, m_backLayers[1]->getPosition().y });
		}
		else
		{
			m_backLayers[2]->setPosition(sf::Vector2f{ m_backLayers[0]->getPosition().x + 1440.0f, m_backLayers[0]->getPosition().y });
		}
	}

	// ----------------------------------------------------------------------------------------------------

	if (m_middleLayers[0]->getDistanceToPlayer(m_player->getPosition()) > 2000.0f
		&& m_middleLayers[0]->getPosition().x < m_player->getPosition().x - 400.0f)
	{
		if (m_middleLayers[1]->getPosition().x > m_middleLayers[2]->getPosition().x)
		{
			m_middleLayers[0]->setPosition(sf::Vector2f{ m_middleLayers[1]->getPosition().x + 1440.0f, m_middleLayers[1]->getPosition().y });
		}
		else
		{
			m_middleLayers[0]->setPosition(sf::Vector2f{ m_middleLayers[2]->getPosition().x + 1440.0f, m_middleLayers[2]->getPosition().y });
		}
	}

	if (m_middleLayers[1]->getDistanceToPlayer(m_player->getPosition()) > 2000.0f
		&& m_middleLayers[1]->getPosition().x < m_player->getPosition().x - 400.0f)
	{
		if (m_middleLayers[2]->getPosition().x > m_middleLayers[0]->getPosition().x)
		{
			m_middleLayers[1]->setPosition(sf::Vector2f{ m_middleLayers[2]->getPosition().x + 1440.0f, m_middleLayers[2]->getPosition().y });
		}
		else
		{
			m_middleLayers[1]->setPosition(sf::Vector2f{ m_middleLayers[0]->getPosition().x + 1440.0f, m_middleLayers[0]->getPosition().y });
		}
	}

	if (m_middleLayers[2]->getDistanceToPlayer(m_player->getPosition()) > 2000.0f
		&& m_middleLayers[2]->getPosition().x < m_player->getPosition().x - 400.0f)
	{
		if (m_middleLayers[1]->getPosition().x > m_middleLayers[0]->getPosition().x)
		{
			m_middleLayers[2]->setPosition(sf::Vector2f{ m_middleLayers[1]->getPosition().x + 1440.0f, m_middleLayers[1]->getPosition().y });
		}
		else
		{
			m_middleLayers[2]->setPosition(sf::Vector2f{ m_middleLayers[0]->getPosition().x + 1440.0f, m_middleLayers[0]->getPosition().y });
		}
	}

	// ----------------------------------------------------------------------------------------------------

	if (m_topLayers[0]->getDistanceToPlayer(m_player->getPosition()) > 2000.0f
		&& m_topLayers[0]->getPosition().x < m_player->getPosition().x - 400.0f)
	{
		if (m_topLayers[1]->getPosition().x > m_topLayers[2]->getPosition().x)
		{
			m_topLayers[0]->setPosition(sf::Vector2f{ m_topLayers[1]->getPosition().x + 1440.0f, m_topLayers[1]->getPosition().y });
		}
		else
		{
			m_topLayers[0]->setPosition(sf::Vector2f{ m_topLayers[2]->getPosition().x + 1440.0f, m_topLayers[2]->getPosition().y });
		}
	}

	if (m_topLayers[1]->getDistanceToPlayer(m_player->getPosition()) > 2000.0f
		&& m_topLayers[1]->getPosition().x < m_player->getPosition().x - 400.0f)
	{
		if (m_topLayers[2]->getPosition().x > m_topLayers[0]->getPosition().x)
		{
			m_topLayers[1]->setPosition(sf::Vector2f{ m_topLayers[2]->getPosition().x + 1440.0f, m_topLayers[2]->getPosition().y });
		}
		else
		{
			m_topLayers[1]->setPosition(sf::Vector2f{ m_topLayers[0]->getPosition().x + 1440.0f, m_topLayers[0]->getPosition().y });
		}
	}

	if (m_topLayers[2]->getDistanceToPlayer(m_player->getPosition()) > 2000.0f
		&& m_topLayers[2]->getPosition().x < m_player->getPosition().x - 400.0f)
	{
		if (m_topLayers[1]->getPosition().x > m_topLayers[0]->getPosition().x)
		{
			m_topLayers[2]->setPosition(sf::Vector2f{ m_topLayers[1]->getPosition().x + 1440.0f, m_topLayers[1]->getPosition().y });
		}
		else
		{
			m_topLayers[2]->setPosition(sf::Vector2f{ m_topLayers[0]->getPosition().x + 1440.0f, m_topLayers[0]->getPosition().y });
		}
	}

	// ----------------------------------------------------------------------------------------------------

	sf::View view(m_window.getView());

	if (view.getCenter().x > m_player->getPosition().x)
		view.move(sf::Vector2f{ m_player->getPosition().x - view.getCenter().x, 0.0f });
	else if (view.getCenter().x < m_player->getPosition().x)
		view.move(sf::Vector2f{ m_player->getPosition().x - view.getCenter().x, 0.0f });

	m_window.setView(view);

	m_bow->update(dt);
}

void Game::render()
{
	m_window.clear(sf::Color(134,177,169,255));

	for (auto backLayer : m_backLayers)
	{
		backLayer->render(m_window);
	}

	for (auto midLayer : m_middleLayers)
	{
		midLayer->render(m_window);
	}

	for (auto topLayer : m_topLayers)
	{
		topLayer->render(m_window);
	}

	m_player->render(m_window);
	

	m_window.draw(m_instructBg);
	m_window.draw(m_instructions);

	for (auto platform : m_platforms)
	{
		platform->render(m_window);
	}

	m_bow->render();

	m_window.display();
}
