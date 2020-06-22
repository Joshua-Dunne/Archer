#ifndef WALKER_H
#define WALKER_H
#include "Enemy.h"

class Walker :
	public Enemy
{
public:
	Walker() = delete;
	Walker(std::vector<Arrow*>& t_arrowRef);

protected:
	std::vector<Arrow*>& m_arrowRefs;
	virtual void setupAnimations();

public:
	virtual void update(sf::Time& dt);
	virtual void render(sf::RenderWindow& t_window);

};

#endif