#ifndef LAYERMANAGER_H
#define LAYERMANAGER_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include "Background.h"
#include "Player.h"

class LayerManager
{
public:
	LayerManager(sf::Vector2f t_pos, Player* t_player);

private:
	sf::Texture m_bgTwo;
	sf::Texture m_bgThree;
	sf::Texture m_bgFour;

	Background* m_topLayer;
	Background* m_midLayer;
	Background* m_backLayer;

	const Player* m_playerRef;

	void topLayerHandling();
	void midLayerHandling();
	void backLayerHandling();

	bool m_topNearPlayer{ true };
	bool m_midNearPlayer{ true };
	bool m_backNearPlayer{ true };

public:
	void update(sf::Time dt);
	void renderTop(sf::RenderWindow& t_window);
	void renderMid(sf::RenderWindow& t_window);
	void renderBack(sf::RenderWindow& t_window);
};

#endif