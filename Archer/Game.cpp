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

	

	m_platforms.push_back(new Platform(sf::Vector2f{ 150.0f, 250.0f }, 1.0f));
	m_platforms.push_back(new Platform(sf::Vector2f{ 650.0f, 250.0f }, 1.0f));
	m_platforms.push_back(new Platform(sf::Vector2f{ 400.0f, 450.0f }, 2.0f));
	m_platforms.push_back(new Platform(sf::Vector2f{ 1200.0f, 450.0f }, 8.0f));
	m_platforms.push_back(new Platform(sf::Vector2f{ 2000.0f, 250.0f }, 8.0f));

	m_player = new Player(m_platforms);
	m_bow = new Bow(&m_window, m_player);
	

	/*m_managers.push_back(LayerManager(
		Background(m_bgTwo, sf::Vector2f{ -1540.0f, 0.0f }),
		Background(m_bgThree, sf::Vector2f{ -1440.0f, 0.0f }),
		Background(m_bgFour, sf::Vector2f{ -1340.0f, 0.0f }),
		m_player));*/

	//Background testOne{ m_bgTwo, sf::Vector2f{ 100.0f, 0.0f } };
	//Background testTwo{ m_bgThree, sf::Vector2f{ 0.0f, 0.0f } };
	//Background testThree{ m_bgFour, sf::Vector2f{ -100.0f, 0.0f } };

	m_managers.push_back(LayerManager(sf::Vector2f{ 0.0f, 0.0f }, m_player));
	m_managers.push_back(LayerManager(sf::Vector2f{ 1440.0f, 0.0f }, m_player));
	//m_managers.push_back(LayerManager(sf::Vector2f{ 1840.0f, 0.0f }, m_player));


	m_instructBg.setPosition(m_instructions.getPosition() - sf::Vector2f{ 10.0f, 10.0f });
	m_instructBg.setSize(sf::Vector2f{ m_instructions.getGlobalBounds().width + 20.0f, m_instructions.getGlobalBounds().height + 20.0f });
	m_instructBg.setFillColor(sf::Color::Black);
}

Game::~Game()
{
	delete m_player;
	delete m_bow;

	m_platforms.clear();
	m_managers.clear();
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

	for (auto manager : m_managers)
	{
		manager.update(dt);
	}

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

	for (auto manager : m_managers)
	{
		manager.renderBack(m_window);
	}

	for (auto manager : m_managers)
	{
		manager.renderMid(m_window);
	}

	for (auto manager : m_managers)
	{
		manager.renderTop(m_window);
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
