#include "Player.hpp"

#include <optional>

Player::Player(Transform t, Model model)
	: GameObject(nullptr, t, std::move(model)),
	rigidBody(RigidBody(transform, BoxCollider(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f)))),
	gun(Gun(
		nullptr,
		Transform(glm::vec3(0.0f, 0.0f, 0.0f)),
		Model("resource/model/shotgun/shotgun3.obj")
	))
{
	addChild(gun);
	rigidBody.speed = 10.0f;
}

void Player::draw(const Shader& shader) const
{
	shader.use();
	shader.setVec3("defaultAmbient", glm::vec3(0.3f, 0.3f, 0.3f));

	GameObject::draw(shader);
	gun.draw(shader);

	shader.use();
	shader.setVec3("defaultAmbient", glm::vec3(0.0f, 0.0f, 0.0f));

	if (Globals::debug)
		rigidBody.collider.draw(gAxesShader, translate(glm::mat4(1.0f), transform.position));
	//gun.draw(shader, transform.getModelMatrix());
}

void Player::update(const float dt)
{
	rigidBody.collider.center = transform.position;
	rigidBody.update(dt);

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
