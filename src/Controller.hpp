#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Player.hpp"

enum ECameraMode
{
	FPS,
	TPS
};

struct MouseControl
{
	double lastX, lastY;
	float sensitivity = 0.05f;

	MouseControl(double lastX, double lastY) : lastX(lastX), lastY(lastY)
	{
	}
};

struct CameraControl
{
	//Transform transform;
	glm::vec3 center;
	glm::vec3 offsetInitial = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 offset = offsetInitial;
	std::shared_ptr<Player> player;

	float yaw = 0.0f; // in degree
	float pitch = 0.0f; // in degree
	//float roll = 0.0f; // in degree

	unsigned int width, height;
	ECameraMode mode = ECameraMode::TPS;

	const float radius = 3.0f;
	const float pitchMin = -89.0f; // in degree
	const float pitchMax = 89.0f; // in degree
	const float fov = 50.0f;
	const float near = 0.1f;
	const float far = 30.0f;

	CameraControl(glm::vec3 center, unsigned int width, unsigned int height);

	void followPlayer(float dt);
};

class Controller
{
public:
	MouseControl mouseControl;
	CameraControl cameraControl;

	glm::mat4& viewMatrix;
	glm::mat4& projectionMatrix;

	Controller(MouseControl mouse, CameraControl camera, glm::mat4& view, glm::mat4& projection);

	void init(const std::shared_ptr<Player> &player);
	void update(float dt);

	void onFramebufferSizeCallback(unsigned int width, unsigned int height);
	void onCursorPositionCallback(double xpos, double ypos);

private:
	void rotateCameraAlongMouse(float dx, float dy);
};
