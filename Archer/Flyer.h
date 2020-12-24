#ifndef FLYER_H
#define FLYER_H

#include "Enemy.h"


class Flyer :
	public Enemy
{
public:
	Flyer() = delete;
	Flyer(std::vector<Arrow*>& t_arrowRef, std::vector<Platform*>& t_platforms, Player* t_playerRef);

protected:
	virtual void setupAnimations();
	virtual void collisionHandling(sf::Time& dt);
	virtual void activate(sf::Time& dt);
	void checkPlayerCol(sf::Time& dt);

	std::vector<Arrow*>& m_arrowRefs;
	std::vector<Platform*>& m_platformRefs;
	Player* m_playerRef;

private:
	Animation m_idleAnim;
	Animation m_flyAnim;
	Animation m_fallAnim;
	Animation m_deathAnim;

	bool m_foundPlayer{ false }; // tells Flyer to stay idle if the Player isn't close enough
	bool m_flying{ false }; // if true, the flyer will flap wings and gain height, if false, flyer will fall from gravity
	bool m_dead{ false };

	sf::Clock m_deathClock; // clock which counts how long before the enemy is counted as dead
	sf::Time m_deathCounter; // amount of time that must pass before enemy is counted as dead

	float m_visionRadius{ 120.0f };
	const float m_weight{ 2.5f };
	const float m_maxFlightSpeed{ 30.0f };
	const float m_maxVerticalSpeed{ 85.0f };
	const float m_initialSpeedBurst{ 100.0f };
	const float m_horizontalSpeed{ 5.0f };

	int m_lastPlatformCollision{ 0 };

public:
	virtual void update(sf::Time& dt);
	virtual void render(sf::RenderWindow& t_window) const;
	virtual void initialize(sf::Vector2f t_pos); // used to place and prep enemies

	void movementHandling(sf::Time& dt);
};

#endif