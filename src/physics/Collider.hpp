#pragma once
#include <glm/glm.hpp>
#include <memory>

//#include "../GameObject.hpp"

class BoxCollider
{
public:
	//std::weak_ptr<GameObject> gameObject;
	glm::vec3 center;
	glm::vec3 size;


	/*BoxCollider(const std::shared_ptr<GameObject>& gameObject, glm::vec3 center, glm::vec3 size)
		: gameObject(gameObject), center(center), size(size)
	{}*/
};