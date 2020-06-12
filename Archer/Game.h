#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Background.h"
#include "Platform.h"
#include "Bow.h"

class Game
{
public:
	Game();
	~Game();
private:
	sf::RenderWindow m_window;
	sf::Clock m_updateClock;

	Player* m_player;
	Bow* m_bow;
	

	std::vector<Background*> m_topLayers;
	std::vector<Background*> m_middleLayers;
	std::vector<Background*> m_backLayers;

	std::vector<Platform*> m_platforms;

	sf::Texture m_bgOne;
	sf::Texture m_bgTwo;
	sf::Texture m_bgThree;
	sf::Texture m_bgFour;

	sf::Font m_font;
	sf::Text m_instructions;

	sf::RectangleShape m_instructBg;

public:
	void run();
	void processInput();
	void update(sf::Time dt);
	void render();
};

#endif