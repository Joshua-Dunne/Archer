#ifndef ENEMY_MANAGER_H
#define ENEMY_MANAGER_H

#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Walker.h"
#include "SpawnPoint.h"
#include <vector>
#include <typeinfo>

class EnemyManager
{
public:
	EnemyManager() = delete;
	EnemyManager(std::vector<Arrow*>& t_arrowRef, std::vector<Platform*>& t_platforms, Player* t_playerRef);
private:
	std::vector<Enemy*> m_enemies;
	std::vector<Walker*> m_walkers;
	std::vector<SpawnPoint> m_spawnPoints;

	const size_t c_MAX_WALKERS{ 3 };

public:
	void update(sf::Time& dt);
	void render(sf::RenderWindow& t_window);
	void checkInactive(); // see if any enemies are currently inactive
	SpawnPoint* findSpawn(EnemyType& t_enemyType); // find a suitable spawnpoint
	void resetUsable();
};

#endif