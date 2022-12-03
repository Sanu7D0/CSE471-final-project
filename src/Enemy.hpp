#pragma once

#include <list>

#include "GameObject.hpp"
#include "physics/RigidBody.hpp"
#include "physics/Collider.hpp"

class Enemy : public GameObject, public RigidBody
{
public:
	Enemy(Transform transform, Model model);

	float hp = 10.0f;
	glm::vec3 targetPosition;
	BoxCollider collider = BoxCollider(glm::vec3(1.0f, 1.0f, 1.0f));
	void update(const float dt) override;
	void takeDamage(float damage);
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

	const std::list<Enemy*>& getContainer()
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