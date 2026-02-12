#include "Rabbit.h"
#include <iostream>

Rabbit::Rabbit()
{
    //m_dragFactor = DRAG_FACTOR;
    //m_restitution = COEFF_OF_RESTITUTION;

    // animations
    for (int i = 0; i < 2; i++)
        m_walkDown.addFrame({ { 64 * (i + 1), 0 }, { 64, 64 } });
    m_walkDown.setLooping(true);
    m_walkDown.setFrameSpeed(0.25f);

    for (int i = 0; i < 2; i++)
        m_walkUp.addFrame({ { (64 * (i + 3)), 0 }, { 64, 64 } });
    m_walkUp.setLooping(true);
    m_walkUp.setFrameSpeed(0.25f);

    m_walkUpRight.addFrame({ { 64 * 7, 0 }, { 64, 64 } });
    m_walkUpRight.addFrame({ { 0, 64 }, { 64, 64 } });
    m_walkUpRight.setLooping(true);
    m_walkUpRight.setFrameSpeed(0.25f);

    for (int i = 0; i < 3; i++)
        // 5th, 6th and 0th frame. I am very clever.
        // To be honest this should just be 3 separate lines for each frame
        // but we've committed now.
        m_walkRight.addFrame({ { (64 * ((i + 5) % 7)), 0 }, { 64, 64 } });
       
    m_walkRight.setLooping(true);
    m_walkRight.setFrameSpeed(0.25f);

    for (int i = 0; i < 2; i++)
        m_walkDownRight.addFrame({ { 64 * (i + 1), 64 }, { 64, 64 } });
    m_walkDownRight.setLooping(true);
    m_walkDownRight.setFrameSpeed(0.25f);

    // defaults   
    
    m_currentAnimation = &m_walkDown;
    setTextureRect(m_currentAnimation->getCurrentFrame());
    setCollisionBox({ { 4,4 }, { 24,24} });

}

Rabbit::~Rabbit()
{
}

void Rabbit::handleInput(float dt)
{
    // Simple player controller for the rabbit (Using the Lab 3: Sheep.cpp phase 1 solution)
    sf::Vector2f inputDirection(0, 0);

    if (m_input->isKeyDown(sf::Keyboard::Scancode::W)) inputDirection.y -= 1;
    if (m_input->isKeyDown(sf::Keyboard::Scancode::D)) inputDirection.x += 1;
    if (m_input->isKeyDown(sf::Keyboard::Scancode::S)) inputDirection.y += 1;
    if (m_input->isKeyDown(sf::Keyboard::Scancode::A)) inputDirection.x -= 1;

    // Check length before normalizing!
    float lenSq = inputDirection.x * inputDirection.x + inputDirection.y * inputDirection.y;
    if (lenSq > 0.f)
    {
        inputDirection = inputDirection.normalized();
    }

    m_acceleration = inputDirection * RABBIT_ACCELERATION; 
}


// update is now handled by the Animal class, nifty eh?