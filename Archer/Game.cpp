#include "Game.h"

Game::Game() :	m_window(sf::VideoMode(m_screenWidth, m_screenHeight), "Archer")
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
	m_instructions.setString("A/D Key: Move Left/Right\nSpace/W Key: Jump\nShift: Dash\nS Key (while mid-air): Stomp\nLeft Click: Shoot Arrow");
	m_instructions.setPosition(sf::Vector2f{ 295.0f, 512.5f });

	m_platforms.push_back(new Platform(sf::Vector2f{ 150.0f, 250.0f }, 1.0f));
	m_platforms.push_back(new Platform(sf::Vector2f{ 650.0f, 250.0f }, 1.0f));
	m_platforms.push_back(new Platform(sf::Vector2f{ 400.0f, 450.0f }, 2.0f));
	m_platforms.push_back(new Platform(sf::Vector2f{ 1200.0f, 450.0f }, 8.0f));
	m_platforms.push_back(new Platform(sf::Vector2f{ 2000.0f, 250.0f }, 8.0f));

	m_player = new Player(m_platforms);
	m_bow = new Bow(&m_window, m_player);

	m_layerManagers.push_back(LayerManager(sf::Vector2f{ 0.0f, 0.0f }, m_player));
	m_layerManagers.push_back(LayerManager(sf::Vector2f{ 1440.0f, 0.0f }, m_player));

	m_instructBg.setPosition(m_instructions.getPosition() - sf::Vector2f{ 10.0f, 10.0f });
	m_instructBg.setSize(sf::Vector2f{ m_instructions.getGlobalBounds().width + 20.0f, m_instructions.getGlobalBounds().height + 20.0f });
	m_instructBg.setFillColor(sf::Color::Black);

	m_arrows.push_back(new Arrow(m_bow, m_arrowTex));
	m_arrows.push_back(new Arrow(m_bow, m_arrowTex));
	m_arrows.push_back(new Arrow(m_bow, m_arrowTex));
	m_arrows.push_back(new Arrow(m_bow, m_arrowTex));
	m_arrows.push_back(new Arrow(m_bow, m_arrowTex));

	m_chargeBar.setSize(sf::Vector2f{100.0f, 10.0f});
	m_chargeBar.setOrigin(sf::Vector2f{ 50.0f, m_chargeBar.getOrigin().y});
	m_chargeBar.setFillColor(sf::Color::Red);
	m_chargeBar.setSize(sf::Vector2f{ 0.0f, 10.0f });

	m_enemyManager = new EnemyManager(m_arrows, m_platforms, m_player);
}

Game::~Game()
{
	delete m_player;
	delete m_bow;
	delete m_enemyManager;

	m_platforms.clear();
	m_layerManagers.clear();
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

		if (event.type == sf::Event::MouseButtonPressed)
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !m_player->isDashing())
			{
				if (!m_mouseHeld)
				{
					m_mouseHeld = true;
				}
			}
		}

		if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			if (m_mouseHeld)
			{
				for (auto& arrow : m_arrows)
				{
					if (!arrow->isShot())
					{
						arrow->shoot(); // find the next available arrow and shoot it
						m_bow->resetMultiplier();
						m_mouseHeld = false;
						m_chargeBar.setSize(sf::Vector2f{ 0.0f, 10.0f }); //reset charge bar
						break;
					}
				}
			}
		}
	}
}

void Game::update(sf::Time& dt)
{
	m_player->update(dt);

	if (m_player->m_fell) // if the player has fallen
	{
		m_enemyManager->resetUsable(); // reset usable enemy spawns
	}

	for (auto layerManager : m_layerManagers)
	{
		layerManager.update(dt);
	}

	moveView(dt);

	m_bow->update(dt);

	for (auto& arrow : m_arrows)
	{
		arrow->update(dt);
	}

	m_enemyManager->update(dt);

	if (m_mouseHeld)
	{ // no need to go into look if the mouse isn't currently held
		for (auto& arrow : m_arrows)
		{
			if (!arrow->isShot())
				// make sure at least one arrow is available
			{	// before letting the player charge another shot
				// if one arrow is available, increase the multiplier for arrow speed
				m_bow->increaseMultiplier(dt);
				m_chargeBar.setSize(sf::Vector2f{(((m_bow->getMultiplier() - 1.0f) * (10.0f))) , 10.0f});
				break;
			}
		}
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

	m_chargeBar.setPosition(m_player->getPosition().x + 40.0f, m_player->getPosition().y - 25.0f);

}

void Game::render()
{
	m_window.clear(sf::Color(134,177,169,255));

	for (auto layerManager : m_layerManagers)
	{
		layerManager.renderBack(m_window);
	}

	for (auto layerManager : m_layerManagers)
	{
		layerManager.renderMid(m_window);
	}

	for (auto layerManager : m_layerManagers)
	{
		layerManager.renderTop(m_window);
	}

	m_player->render(m_window);
	m_enemyManager->render(m_window);
	
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

	m_window.draw(m_chargeBar);

	m_window.display();
}
