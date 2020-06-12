#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Player.h"

class Game
{
public:
	Game();
	~Game();
private:
	sf::RenderWindow m_window;
	sf::Clock m_updateClock;

	Player m_player;

public:
	void run();
	void processInput();
	void update(sf::Time dt);
	void render();
};

#endif