#pragma once
#include "Animal.h"
#include "Framework/Animation.h"

class Rabbit :
	public Animal
{

public:
	Rabbit();
	~Rabbit();

	void handleInput(float dt) override;
	void setDirection();
	void checkWallAndBounce();

private:
	const float DRAG_FACTOR = 0.85f;
	const float COEFF_OF_RESTITUTION = 0.3f;
	const float RABBIT_ACCELERATION = 50.0f;


};

