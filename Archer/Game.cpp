#include "Game.h"

Game::Game() : m_window(sf::VideoMode(800, 600), "Archer")
{
	m_window.setFramerateLimit(60u);

	if (!m_font.loadFromFile("./resources/fonts/TrulyHorrible.ttf"))
	{
		std::cout << "error loading TurlyHorrible font" << std::endl;
	}

	if (!m_arrowTex.loadFromFile("./resources/sprites/arrows/arrow1.png"))
	{
		std::cout << "error loading arrow 1 png" << std::endl;
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

	m_managers.push_back(LayerManager(sf::Vector2f{ 0.0f, 0.0f }, m_player));
	m_managers.push_back(LayerManager(sf::Vector2f{ 1440.0f, 0.0f }, m_player));

	m_instructBg.setPosition(m_instructions.getPosition() - sf::Vector2f{ 10.0f, 10.0f });
	m_instructBg.setSize(sf::Vector2f{ m_instructions.getGlobalBounds().width + 20.0f, m_instructions.getGlobalBounds().height + 20.0f });
	m_instructBg.setFillColor(sf::Color::Black);

	m_arrows.push_back(new Arrow(m_bow, m_gravity, m_arrowTex));
}

Game::~Game()
{
	delete m_player;
	delete m_bow;

	m_platforms.clear();
	m_managers.clear();
	m_arrows.clear();
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

		if (event.type == sf::Event::MouseButtonPressed && !m_mouseHeld)
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				for (auto& arrow : m_arrows)
				{
					if (!arrow->isShot())
					{
						arrow->shoot(); // find the next available arrow and shoot it
						std::cout << "arrow shot" << std::endl;
						break;
					}
				}
			}

			m_mouseHeld = true;
		}

		if (event.type == sf::Event::MouseButtonReleased && m_mouseHeld)
		{
			if (event.type == sf::Event::MouseLeft) // if it was the left mouse
				m_mouseHeld = false;
		}
	}
}

void Game::update(sf::Time& dt)
{
	m_player->update(dt);

	for (auto manager : m_managers)
	{
		manager.update(dt);
	}

	moveView(dt);

	m_bow->update(dt);

	bool nullFound{ false };

	for (auto& arrow : m_arrows)
	{
		arrow->update(dt);
	}
}

void Game::moveView(sf::Time& dt)
{
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

	for (auto& arrow : m_arrows)
	{
		arrow->render(m_window);
	}

	m_window.display();
}
