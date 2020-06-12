#include "Game.h"

Game::Game() : m_window(sf::VideoMode(800, 600), "Text Twist")
{
	m_window.setFramerateLimit(60u);

	if (!m_font.loadFromFile("./resources/fonts/TrulyHorrible.ttf"))
	{
		std::cout << "error loading TurlyHorrible font" << std::endl;
	}

	m_instructions.setFont(m_font);
	m_instructions.setCharacterSize(16u);
	m_instructions.setString("Left/Right Arrow: Move Left/Right\nSpace: Jump\nShift: Dash\nDown Arrow (while mid-air): Stomp");
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

	m_layers.push_back(new Background(m_bgTwo, sf::Vector2f{ -1540.0f, 0.0f }));
	m_layers.push_back(new Background(m_bgTwo, sf::Vector2f{ 100.0f, 0.0f }));
	m_layers.push_back(new Background(m_bgTwo, sf::Vector2f{ 1540.0f, 0.0f }));
	m_layers.push_back(new Background(m_bgThree, sf::Vector2f{ -1440.0f, 0.0f }));
	m_layers.push_back(new Background(m_bgThree, sf::Vector2f{ 0.0f, 0.0f }));
	m_layers.push_back(new Background(m_bgThree, sf::Vector2f{ 1440.0f, 0.0f }));
	m_layers.push_back(new Background(m_bgFour, sf::Vector2f{ -1340.0f, 0.0f }));
	m_layers.push_back(new Background(m_bgFour, sf::Vector2f{ -100.0f, 0.0f }));
	m_layers.push_back(new Background(m_bgFour, sf::Vector2f{ 1340.0f, 0.0f }));


	m_platforms.push_back(new Platform(sf::Vector2f{ 150.0f, 250.0f }, 1.0f));
	m_platforms.push_back(new Platform(sf::Vector2f{ 650.0f, 250.0f }, 1.0f));

	m_platforms.push_back(new Platform(sf::Vector2f{ 400.0f, 450.0f }, 2.0f));
	m_platforms.push_back(new Platform(sf::Vector2f{1200.0f, 450.0f}, 8.0f));

	m_player = new Player(m_platforms);

	m_instructBg.setPosition(m_instructions.getPosition() - sf::Vector2f{ 10.0f, 10.0f });
	m_instructBg.setSize(sf::Vector2f{ m_instructions.getGlobalBounds().width + 20.0f, m_instructions.getGlobalBounds().height + 20.0f });
	m_instructBg.setFillColor(sf::Color::Black);
}

Game::~Game()
{
	delete m_player;
	delete[] &m_layers;

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

	if (m_player->isMoving())
	{
		float xMovement = m_player->movingDir();

		m_layers[0]->move(sf::Vector2f{ xMovement * 0.5f, 0.0f });
		m_layers[1]->move(sf::Vector2f{ xMovement * 0.5f, 0.0f });
		m_layers[2]->move(sf::Vector2f{ xMovement * 0.5f, 0.0f });

		m_layers[3]->move(sf::Vector2f{ xMovement, 0.0f });
		m_layers[4]->move(sf::Vector2f{ xMovement, 0.0f });
		m_layers[5]->move(sf::Vector2f{ xMovement, 0.0f });


		m_layers[6]->move(sf::Vector2f{ xMovement * 2.0f, 0.0f });
		m_layers[7]->move(sf::Vector2f{ xMovement * 2.0f, 0.0f });
		m_layers[8]->move(sf::Vector2f{ xMovement * 2.0f, 0.0f });
	}

	sf::View view(m_window.getView());

	if (view.getCenter().x > m_player->getPosition().x)
		view.move(sf::Vector2f{ m_player->getPosition().x - view.getCenter().x, 0.0f });
	else if (view.getCenter().x < m_player->getPosition().x)
		view.move(sf::Vector2f{ m_player->getPosition().x - view.getCenter().x, 0.0f });

	m_window.setView(view);
}

void Game::render()
{
	m_window.clear(sf::Color(134,177,169,255));

	for (auto layer : m_layers)
	{
		layer->render(m_window);
	}

	m_player->render(m_window);

	m_window.draw(m_instructBg);
	m_window.draw(m_instructions);

	for (auto platform : m_platforms)
	{
		platform->render(m_window);
	}

	m_window.display();
}
