#ifndef WALKER_H
#define WALKER_H
#include "Enemy.h"

class Walker :
	public Enemy
{
public:
	Walker() = delete;
	Walker(std::vector<Arrow*>& t_arrowRef, std::vector<Platform*>& t_platforms, Player* t_playerRef);

protected:
	virtual void setupAnimations();
	virtual void collisionHandling(sf::Time& dt);
	virtual void activate(sf::Time& dt);
	void checkPlayerCol(sf::Time& dt);

	std::vector<Arrow*>& m_arrowRefs;
	std::vector<Platform*>& m_platformRefs;
	Player* m_playerRef;
	

private:
	const float m_weight{ 4.0f };
	Animation m_walkAnim;
	Animation m_deathAnim;
	sf::Vector2f m_startingPos;
	bool m_dead{ false };

	sf::Clock m_deathClock;
	sf::Time m_deathCounter;

	int m_lastPlatformCollision;
	bool m_falling;

public:
	virtual void update(sf::Time& dt);
	virtual void render(sf::RenderWindow& t_window) const;
	virtual void initialize(sf::Vector2f t_pos);
	

};

#endif