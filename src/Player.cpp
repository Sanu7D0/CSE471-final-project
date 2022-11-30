#include "Player.hpp"

Player::Player(Transform t, Model model_, Gun gun)
	: GameObject(nullptr, t, std::move(model_)),
	RigidBody(transform),
	gun(std::move(gun))
{
	speed = 3.0f;
}

void Player::draw(const Shader& shader) const
{
	const auto modelMat = transform.getModelMatrix();

	shader.use();
	shader.setMat4("model", modelMat);
	shader.setMat4("model_inverse", glm::inverse(modelMat));

	model.draw(shader);

	if (Globals::debug)
	{
		drawAxes();
	}
}

void Player::update(const float dt)
{
	move(dt);
	if (const auto _flashLight = flashLight.lock())
	{
		_flashLight->position = transform.position;
		//_flashLight->direction = transform.forward();
	}
}
