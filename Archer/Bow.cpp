#include "Bow.h"

Bow::Bow(sf::RenderWindow* t_window, Player* t_player) : m_windowRef(t_window), m_playerRef(t_player)
{
	if (!m_bowTex.loadFromFile("./resources/sprites/weapons/bow1.png"))
	{
		std::cout << "unable to load bow 1 png" << std::endl;
	}

    m_body.setTexture(m_bowTex);
    m_body.setOrigin(m_body.getOrigin().x, m_body.getGlobalBounds().height / 2.0f);
    m_body.setScale(0.75f, 0.75f);
    lookAtMouse();
}

void Bow::render() const
{
    if (!m_playerRef->isDashing()) 
    { 
        m_windowRef->draw(m_body); 
    }
}

void Bow::update(sf::Time dt)
{
    m_body.setPosition(m_playerRef->getPosition());
	lookAtMouse();
}

void Bow::lookAtMouse()
{
    sf::Vector2f curPos = m_body.getPosition();
    sf::Vector2i position = sf::Mouse::getPosition(*m_windowRef);
    sf::Vector2f worldPos = m_windowRef->mapPixelToCoords(position); // this is needed to convert from window to current view
    // since the view moves around

    // now we have both the sprite position and the cursor
    // position lets do the calculation so our sprite will
    // face the position of the mouse
    const float PI = 3.14159265f;

    float dx = curPos.x - worldPos.x;
    float dy = curPos.y - worldPos.y;

    float rotation = (atan2(dy, dx)) * 180 / PI;

    m_body.setRotation(rotation + 180.0f);
}
