#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "GameObject.hpp"

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
	Transform transform;
	glm::vec3 followPosition;

	float yaw = 0.0f; // in degree
	float pitch = 0.0f; // in degree

	unsigned int width, height;
	ECameraMode mode = ECameraMode::FPS;

	const float pitchMin = -89.0f; // in degree
	const float pitchMax = 89.0f; // in degree
	const float fov = 50.0f;
	const float near = 0.1f;
	const float far = 30.0f;

	CameraControl(Transform transform, unsigned int width, unsigned int height)
		: transform(transform), followPosition(transform.position), width(width), height(height)
	{
	}

	void followTargetPosition(float dt);
};

class Controller
{
public:
	MouseControl mouseControl;
	CameraControl cameraControl;

	glm::mat4& viewMatrix;
	glm::mat4& projectionMatrix;

	Controller(MouseControl mouse, CameraControl camera, glm::mat4& view, glm::mat4& projection);

	void update(float dt);

	void onFramebufferSizeCallback(unsigned int width, unsigned int height);
	void onCursorPositionCallback(double xpos, double ypos);

private:
	void rotateCameraAlongMouse(float dx, float dy);
};
