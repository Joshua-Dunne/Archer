#include "Tracker.h"

Tracker::Tracker(std::vector<Arrow*>& t_arrowRef, std::vector<Platform*>& t_platforms, Player* t_playerRef)
    : m_arrowRefs(t_arrowRef), m_platformRefs(t_platforms), m_playerRef(t_playerRef), Enemy()
{
	setupAnimations();

	m_currAnim = &m_walkAnim;

	// hitbox set up

	m_hitbox.setSize(sf::Vector2f{ 16.0f, 16.0f });
	m_hitbox.setOrigin(sf::Vector2f{ 8.0f, 8.0f });
	m_hitbox.setFillColor(sf::Color::Red);
	m_movement.x = 0.0f;
	m_movement.y = 0.0f;
	m_type = EnemyType::TrackerEnem;
}

void Tracker::setupAnimations()
{
}

void Tracker::collisionHandling(sf::Time& dt)
{
}

void Tracker::activate(sf::Time& dt)
{
}

void Tracker::update(sf::Time& dt)
{
}

void Tracker::render(sf::RenderWindow& t_window) const
{
}

void Tracker::initialize(sf::Vector2f t_pos)
{
}
