#ifndef SPAWN_POINT_H
#define SPAWN_POINT_H

#include <SFML/Graphics.hpp>
#include "EnemyType.h"

/// <summary>
/// Simple struct intended to hold positions where enemies will be placed when inactive.
/// </summary>

struct SpawnPoint
{
	SpawnPoint(sf::Vector2f t_pos, EnemyType t_type);

	sf::Vector2f m_spawnPos; // where the spawner is
	EnemyType m_type; // what enemy will spawn from it
	bool m_used; // if an enemy has already been placed here
};

#endif