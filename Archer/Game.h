#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "LayerManager.h"
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
	
	std::vector<LayerManager> m_managers;
	LayerManager* m_layerManager;

	std::vector<Platform*> m_platforms;

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