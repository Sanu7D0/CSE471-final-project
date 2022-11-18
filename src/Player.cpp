#include "Player.hpp"

Player::Player(Transform t) : GameObject(nullptr, t)
{
}

void Player::draw() const
{
}

void Player::move(float dt)
{
	if (glm::epsilonEqual(length(velocity), 0.0f, glm::epsilon<float>()))
		return;

	/*const glm::vec3 forward = glm::vec3(
		glm::cos(glm::radians(bodyYaw)),
		0.0f,
		glm::sin(glm::radians(bodyYaw))
	);*/

	const glm::vec3 forward = transform.rotation * glm::vec3(0.0f, 0.0f, 1.0f);
	//const glm::vec3 left = cross(forward, glm::vec3(0.0f, 1.0f, 0.0f));
	const glm::vec3 right = cross(glm::vec3(0.0f, 1.0f, 0.0f), forward); // right hand rule

	//transform.position += dt * _speed * (forward * -velocity.z + right * -velocity.x);
	transform.position += dt * _speed * (right * velocity.x + forward * velocity.z);
}
