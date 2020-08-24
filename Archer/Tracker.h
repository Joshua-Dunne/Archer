#ifndef TRACKER_H
#define TRACKER_H
#include "Enemy.h"


class Tracker : public Enemy
{
public:
	Tracker() = delete;
	Tracker(std::vector<Arrow*>& t_arrowRef, std::vector<Platform*>& t_platforms, Player* t_playerRef);

protected:
	virtual void setupAnimations();
	virtual void activate(sf::Time& dt);
	virtual void collisionHandling(sf::Time& dt);

	std::vector<Arrow*>& m_arrowRefs;
	std::vector<Platform*>& m_platformRefs;
	Player* m_playerRef;

private:
	void movementHandling(sf::Time& dt);
	void attackHandling(sf::Time& dt);

	const float m_weight{ 5.5f };

	Animation m_idleAnim;
	Animation m_walkAnim;
	Animation m_attackAnim;
	Animation m_deathAnim;

	sf::Vector2f m_startingPos;
	bool m_dead{ false };

	sf::Clock m_deathClock; // clock which counts how long before the enemy is counted as dead
	sf::Time m_deathCounter; // amount of time that must pass before enemy is counted as dead

	sf::Clock m_memoryClock; // clock to judge how long the tracker will wait before forgetting the player
	sf::Time m_memoryCounter; // time that must pass for tracker to forget player

	int m_lastPlatformCollision;
	bool m_falling;

	sf::RectangleShape m_attackHitbox;
	bool m_attackActive{ false };
	sf::Time m_attackActiveClock;

	const float m_visionRadius{ 90.0f };
	const float m_speed{ 0.15f };
	bool m_foundPlayer{ false };
	bool m_nearPlayer{ false };

	sf::RectangleShape m_knifeHitbox;
	

public:

	sf::CircleShape m_visionVisual;

	virtual void update(sf::Time& dt);
	virtual void render(sf::RenderWindow& t_window) const;
	virtual void initialize(sf::Vector2f t_pos);

};

#endif