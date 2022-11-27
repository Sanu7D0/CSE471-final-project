#include "Player.hpp"

Player::Player(Transform t, Model model_) : GameObject(nullptr, t, std::move(model_))
{
}

void Player::update(const float dt)
{
	move(dt);
	if (const auto _flashLight = flashLight.lock())
	{
		_flashLight->position = transform.position;
		_flashLight->direction = transform.forward();
	}
}

void Player::move(const float dt)
{
	if (glm::epsilonEqual(length(velocity), 0.0f, glm::epsilon<float>()))
		return;

	const glm::vec3 forward = transform.rotation * glm::vec3(0.0f, 0.0f, 1.0f);
	//const glm::vec3 left = cross(forward, glm::vec3(0.0f, 1.0f, 0.0f));
	const glm::vec3 right = cross(glm::vec3(0.0f, -1.0f, 0.0f), forward); // right hand rule

	transform.position += dt * _speed * (right * velocity.x + forward * velocity.z);
}

