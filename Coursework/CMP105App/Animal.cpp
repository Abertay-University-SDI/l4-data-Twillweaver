#include "Animal.h"
#include <cmath>

Animal::Animal()
{
    // Initialize the pointer to null so it's not pointing at random memory
    m_currentAnimation = nullptr;

    // Set default physics values
    m_dragFactor = 0.9f;    // Example default
    m_restitution = 0.8f;    // Example default
    m_velocity = { 0, 0 };
    m_acceleration = { 0, 0 };

    
}

Animal::~Animal() {}

void Animal::update(float dt)
{
    // Lab 3 work used here, just apply the physics
    m_velocity += m_acceleration * dt;
    m_velocity *= m_dragFactor;
    
    // handle animation (lab 2 work + new setDirection method)
    setDirection();

    // Check if the pointer is valid before using it
    if (m_currentAnimation != nullptr)
    {
        m_currentAnimation->animate(dt);
        setTextureRect(m_currentAnimation->getCurrentFrame());
    }

    //old code:
    //m_currentAnimation->animate(dt); 
    //setTextureRect(m_currentAnimation->getCurrentFrame());

    checkWallAndBounce();
    move(m_velocity);
}

void Animal::setWorldSize(float x, float y)
{
    m_worldSize = { x, y };
}

// default collision response is to bounce. Likely only used for Sheep-Sheep collision and magnetism should prevent Sheep-Rabbit
void Animal::collisionResponse(GameObject& collider)
{
    m_velocity *= -m_restitution;

    // Calculate squared length to avoid expensive square root if not needed
    float lengthSq = m_velocity.x * m_velocity.x + m_velocity.y * m_velocity.y;

    if (lengthSq > 0.0001f) // Only normalize if there is actually movement
    {
        sf::Vector2f normalizedVel = m_velocity.normalized();

        if (lengthSq < 150.f)
            move(normalizedVel * 10.f);
        else
            move(normalizedVel * 5.f);
    }

    setDirection();       // handle animation check AGAIN (update usually comes before collisions in level::update)
}

// don't be hitting walls
void Animal::checkWallAndBounce()
{
    sf::Vector2f pos = getPosition();
    sf::Vector2f size = getSize();

    // horizontal wall check
    if (pos.x < 0 && m_velocity.x < 0) {
        m_velocity.x *= -m_restitution;
        setPosition({ 0.f, pos.y }); // Snap to left edge
    }
    else if (pos.x + size.x > m_worldSize.x && m_velocity.x > 0) {
        m_velocity.x *= -m_restitution;
        setPosition({ m_worldSize.x - size.x, pos.y }); // Snap to right edge
    }

    // Vertical Wall check
    pos = getPosition(); // Update pos after potential horizontal snap
    if (pos.y < 0 && m_velocity.y < 0) {
        m_velocity.y *= -m_restitution;
        setPosition({ pos.x, 0.f }); // Snap to top edge
    }
    else if (pos.y + size.y > m_worldSize.y && m_velocity.y > 0) {
        m_velocity.y *= -m_restitution;
        setPosition({ pos.x, m_worldSize.y - size.y }); // Snap to bottom edge
    }

    // old code:
    //if ((pos.x < 0 && m_velocity.x < 0) || (pos.x + size.x > m_worldSize.x && m_velocity.x > 0))
    //    m_velocity.x *= -m_restitution;

    //if ((pos.y < 0 && m_velocity.y < 0) || (pos.y + size.y > m_worldSize.y && m_velocity.y > 0))
    //    m_velocity.y *= -m_restitution;
}

// sets animation pointer based on direction
void Animal::setDirection()
{
    // Use a small epsilon check
    if (m_velocity.x * m_velocity.x + m_velocity.y * m_velocity.y < 0.1f) return;

    // atan2 is safe with (0,0), but normalized() is not.
    // If you use normalized() anywhere else in this function, wrap it in a length check.
    float angle = std::atan2(m_velocity.y, m_velocity.x) * (180.0f / 3.14159f);

    if (angle > -22.5f && angle <= 22.5f) {
        m_currentAnimation = &m_walkRight;
        m_currentAnimation->setFlipped(false);
    }
    else if (angle > 22.5f && angle <= 67.5f) {
        m_currentAnimation = &m_walkDownRight;
        m_currentAnimation->setFlipped(false);
    }
    else if (angle > 67.5f && angle <= 112.5f) {
        m_currentAnimation = &m_walkDown;
        m_currentAnimation->setFlipped(false);
    }
    else if (angle > 112.5f && angle <= 157.5f) {
        m_currentAnimation = &m_walkDownRight;
        m_currentAnimation->setFlipped(true);
    }
    else if (angle > 157.5f || angle <= -157.5f) {
        m_currentAnimation = &m_walkRight;
        m_currentAnimation->setFlipped(true);
    }
    else if (angle > -157.5f && angle <= -112.5f) {
        m_currentAnimation = &m_walkUpRight;
        m_currentAnimation->setFlipped(true);
    }
    else if (angle > -112.5f && angle <= -67.5f) {
        m_currentAnimation = &m_walkUp;
        m_currentAnimation->setFlipped(false);
    }
    else if (angle > -67.5f && angle <= -22.5f) {
        m_currentAnimation = &m_walkUpRight;
        m_currentAnimation->setFlipped(false);
    }
}