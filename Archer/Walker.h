#ifndef WALKER_H
#define WALKER_H
#include "Enemy.h"

class Walker :
	public Enemy
{
public:
	Walker() = delete;
	Walker(std::vector<Arrow*>& t_arrowRef, std::vector<Platform*>& t_platforms, sf::Vector2f t_startingPos);

protected:
	std::vector<Arrow*>& m_arrowRefs;
	std::vector<Platform*>& m_platformRefs;
	virtual void setupAnimations();

private:
	const float m_weight{ 2.5f };

public:
	virtual void update(sf::Time& dt);
	virtual void render(sf::RenderWindow& t_window);
	virtual void collisionHandling(sf::Time& dt);

};

#endif