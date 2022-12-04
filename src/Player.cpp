#include "Player.hpp"

#include <optional>

Player::Player(Transform t, Model model)
	: GameObject(nullptr, t, std::move(model)),
	RigidBody(transform, BoxCollider(glm::vec3(4.0f, 4.0f, 4.0f), collider.center)),
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
	gun.draw(shader);
	if (Globals::debug)
		collider.draw(gAxesShader, transform.getModelMatrix());
	//gun.draw(shader, transform.getModelMatrix());
}

void Player::update(const float dt)
{
	
	RigidBody::update(dt);
	collider.center = transform.position;

	// TODO: hierachy
	
	gun.transform.position = transform.position;
	gun.transform.rotation = transform.rotation;
	gun.muzzleFlash->position = gun.transform.position;


	if (const auto _flashLight = flashLight.lock())
	{
		_flashLight->position = transform.position;
		//_flashLight->direction = transform.forward();
	}
}
