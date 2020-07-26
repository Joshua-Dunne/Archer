#include "EnemyManager.h"

EnemyManager::EnemyManager(std::vector<Arrow*>& t_arrowRef, std::vector<Platform*>& t_platforms, Player* t_playerRef)
{
	m_spawnPoints.push_back(SpawnPoint(sf::Vector2f{ 650.0f, 250.0f }, EnemyType::WalkerEnem));
	m_spawnPoints.push_back(SpawnPoint(sf::Vector2f{ 700.0f, 450.0f }, EnemyType::WalkerEnem));
	m_spawnPoints.push_back(SpawnPoint(sf::Vector2f{ 1200.0f, 450.0f }, EnemyType::WalkerEnem));
	m_spawnPoints.push_back(SpawnPoint(sf::Vector2f{ 1400.0f, 450.0f }, EnemyType::WalkerEnem));
	m_spawnPoints.push_back(SpawnPoint(sf::Vector2f{ 1800.0f, 250.0f }, EnemyType::WalkerEnem));
	m_spawnPoints.push_back(SpawnPoint(sf::Vector2f{ 2000.0f, 250.0f }, EnemyType::WalkerEnem));
	m_spawnPoints.push_back(SpawnPoint(sf::Vector2f{ 2400.0f, 250.0f }, EnemyType::WalkerEnem));

	for (size_t index = 0; index < c_MAX_WALKERS; ++index)
	{
		m_enemies.push_back(new Walker(t_arrowRef, t_platforms, t_playerRef));
	}

	m_spawnPoints.push_back(SpawnPoint(sf::Vector2f{ 150.0f, 250.0f }, EnemyType::TrackerEnem));

	for (size_t index = 0; index < c_MAX_TRACKERS; ++index)
	{
		m_enemies.push_back(new Tracker(t_arrowRef, t_platforms, t_playerRef));
	}
	
}

void EnemyManager::update(sf::Time& dt)
{
	for (auto& enemy : m_enemies)
	{
		enemy->update(dt);
	}

	checkInactive();
}

void EnemyManager::render(sf::RenderWindow& t_window)
{
	for (auto& enemy : m_enemies)
	{
		enemy->render(t_window);
	}
}

/// <summary>
/// Go through all enemies to find one that is inactive
/// </summary>
void EnemyManager::checkInactive()
{
	Enemy* inactiveEnemy{ nullptr };

	for (auto& enemy : m_enemies)
	{
		if (!enemy->m_active && !enemy->m_placed) // if the enemy isn't active and hasn't been placed yet
		{
			inactiveEnemy = enemy;
			break; // we found an enemy so break out
		}
	}

	if (nullptr != inactiveEnemy) // if an enemy was found
	{
		SpawnPoint* point = findSpawn(inactiveEnemy->m_type);

		if (point->m_type != EnemyType::None && !point->m_used)
		{
			std::cout << "Placed enemy at X: " << point->m_spawnPos.x << ", Y: " << point->m_spawnPos.y << std::endl;
			inactiveEnemy->initialize(point->m_spawnPos);
			point->m_used = true;
		}
		else
		{
			//std::cout << "No spawn available!" << std::endl;
			delete point; // since the returned pointer is no longer needed, clean it up
		}
	}
}

SpawnPoint*	EnemyManager::findSpawn(EnemyType& t_enemyType)
{
	for (auto& point : m_spawnPoints) // look for an available spawn of the given type
	{
		if (!point.m_used && point.m_type == t_enemyType)
		{
			return &point;
		}
	}

	// if no spawn was found, just return a spawnpoint with type none
	return new SpawnPoint(sf::Vector2f{}, EnemyType::None);
}


/// <summary>
/// Resets usability of spawn points
/// </summary>
void EnemyManager::resetUsable()
{
	for (auto& points : m_spawnPoints)
	{
		points.m_used = false;
	}

	for (size_t index = 0; index < c_MAX_WALKERS; ++index)
	{
		m_enemies[index]->initialize(m_spawnPoints[index].m_spawnPos); // place available enemies at first few spawn points
		m_spawnPoints[index].m_used = true;
	}
}
