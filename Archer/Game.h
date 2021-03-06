#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "LayerManager.h"
#include "EnemyManager.h"
#include "Platform.h"
#include "Bow.h"
#include "Arrow.h"
#include "globals.h"
#include "Walker.h"

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

	float m_gravity{ 0.035f };
	
	std::vector<LayerManager> m_layerManagers;
	EnemyManager* m_enemyManager;

	std::vector<Platform*> m_platforms;

	std::vector<Arrow*> m_arrows;

	sf::Font m_font;
	sf::Text m_instructions;

	sf::RectangleShape m_instructBg;
	sf::RectangleShape m_chargeBar;

	sf::Texture m_arrowTex;

	bool m_mouseHeld{ false };

public:
	void run();
	void processInput();
	void update(sf::Time& dt);
	void moveView(sf::Time& dt);
	void render();
};

#endif