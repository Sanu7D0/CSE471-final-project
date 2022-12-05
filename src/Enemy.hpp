#pragma once

#include <list>

#include "GameObject.hpp"
#include "physics/RigidBody.hpp"
#include "physics/Collider.hpp"

class Enemy : public GameObject
{
public:
	Enemy(Transform transform, Model model);

	RigidBody rigidBody;
	glm::vec3 targetPosition;
	BoxCollider collider = BoxCollider(glm::vec3(2.0f, 2.0f, 2.0f));

	float hp = 10.0f;
	float damage = 25.0f;
	double lastAttackedTime = 0.0;
	double attackDelay = 1.0;

	void update(const float dt) override;
	void draw(const Shader& shader) const override;
	void takeDamage(float damage);
	float dealDamage();
};

class EnemyContainer
{
private:
	static EnemyContainer* _instance;
	std::list<Enemy*> enemies;

	EnemyContainer() {};
	~EnemyContainer() {};
	EnemyContainer(EnemyContainer const&) = delete;
	EnemyContainer& operator=(EnemyContainer const&) = delete;

public:
	static EnemyContainer* Instance()
	{
		if (_instance == nullptr) _instance = new EnemyContainer();
		return _instance;
	}

	std::list<Enemy*>& getContainer()
	{
		return enemies;
	}

	void addEnemy(Enemy* enemy)
	{
		enemies.push_back(enemy);
	}

	void removeEnemy(Enemy* enemy)
	{
		enemies.remove(enemy);
		delete enemy;
	}
};