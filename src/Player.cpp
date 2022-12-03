#include "Player.hpp"

#include <optional>

Player::Player(Transform t, Model model)
	: GameObject(nullptr, t, std::move(model)),
	RigidBody(transform, std::optional<BoxCollider>(std::nullopt)),
	gun(Gun(
		nullptr,
		Transform(glm::vec3(0.0f, 0.0f, 0.0f)), // TODO: set with gun offset
		Model("resource/model/stick.obj")
	))
{
	addChild(gun);
	speed = 4.0f;
}

void Player::draw(const Shader& shader) const
{
	GameObject::draw(shader);

	//gun.draw(shader, transform.getModelMatrix());
}

void Player::update(const float dt)
{
	RigidBody::update(dt);
	gun.update(dt);

	if (const auto _flashLight = flashLight.lock())
	{
		_flashLight->position = transform.position;
		//_flashLight->direction = transform.forward();
	}
}
