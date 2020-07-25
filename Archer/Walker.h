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
	std::vector<Arrow*>& m_arrowRefs;
	std::vector<Platform*>& m_platformRefs;
	Player* m_playerRef;
	virtual void setupAnimations();
	virtual void collisionHandling(sf::Time& dt);
	void activate(sf::Time& dt);

private:
	const float m_weight{ 4.0f };
	
	Animation m_walkAnim;
	Animation m_deathAnim;
	
	sf::Vector2f m_startingPos;

	bool m_dead{ false };

	sf::Clock m_deathClock;
	sf::Time m_deathCounter;

public:
	virtual void update(sf::Time& dt);
	virtual void render(sf::RenderWindow& t_window);
	virtual void initialize(sf::Vector2f t_pos);
	

};

#endif