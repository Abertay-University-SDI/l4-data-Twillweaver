#pragma once
#include "Animal.h"
#include "Framework/Animation.h"

class Sheep :
	public Animal
{

public:
	Sheep(sf::Vector2f initial_position, GameObject* p_rabbit);
	~Sheep();

	void update(float dt) override;
	void collideWithGoal(GameObject& goal);

private:
	const float ACCELERATION = 50.0f;
	const float DRAG_FACTOR = 0.85f;
	const float COEFF_OF_RESTITUTION = 0.3f;
	const float ALERT_DISTANCE = 50.0f;
	GameObject* m_rabbitPointer;

};

