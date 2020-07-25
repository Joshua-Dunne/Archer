#include "SpawnPoint.h"

SpawnPoint::SpawnPoint(sf::Vector2f t_pos, EnemyType t_type) : m_spawnPos(t_pos), m_type(t_type), m_used(false)
{ // assume unused upon initial construction
}
