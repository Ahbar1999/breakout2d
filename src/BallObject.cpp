#include "BallObject.h"

BallObject::BallObject(): GameObject(), Stuck(true), Radius(12.5f)
{

}

BallObject::BallObject(glm::vec2 position, float radius, glm::vec2 velocity, Texture2D sprite) : GameObject(position, glm::vec2(radius*2, radius*2), sprite, glm::vec3(1.0f), velocity), Stuck(true), Radius(radius)
{

}

void BallObject::Reset(glm::vec2 position, glm::vec2 velocity)
{
	this->Position = position;
	this->Velocity = velocity;
	this->Stuck = true;
}

glm::vec2 BallObject::Move(float dt, unsigned int window_width)
{
	if (!this->Stuck)
	{
		this->Position += this->Velocity * dt;
	
		if (this->Position.x <= 0.0f)
		{
			this->Velocity.x = -this->Velocity.x;
			this->Position.x = 0.0f;
		}
		else if (this->Position.x + this->Size.x >= window_width)
		{
			this->Velocity.x = -this->Velocity.x;
			this->Position.x = window_width - this->Size.x;
		}
		if (this->Position.y < 0.0f)
		{
			this->Velocity.y = -this->Velocity.y;
			this->Position.y = 0.0f;
		}
	}

	return this->Position;
}