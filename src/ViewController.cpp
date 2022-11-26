#include "ViewController.hpp"

#include <glm/gtc/matrix_transform.hpp>

CameraControl::CameraControl(glm::vec3 center, unsigned int width, unsigned int height)
	: center(center), eye(center), followingTarget(center), width(width), height(height)
{
	// Normalize offset to radius
	offsetInitial = radius * glm::normalize(offsetInitial);
	offset = offsetInitial;
}

// Move camera "center" to following position
void CameraControl::followTarget(float dt)
{
	// TODO: lerp
	center = followingTarget;
}

ViewController::ViewController(MouseControl mouse, CameraControl camera, glm::mat4& view, glm::mat4& projection)
	: mouseControl(mouse), cameraControl(camera), viewMatrix(view), projectionMatrix(projection)
{
	projectionMatrix = glm::perspectiveFov(
		glm::radians(cameraControl.fov),
		static_cast<float>(cameraControl.width),
		static_cast<float>(cameraControl.height),
		cameraControl.near,
		cameraControl.far
	);
}

void ViewController::update(float dt)
{
	cameraControl.followTarget(dt);

	glm::vec3 at;
	if (cameraControl.mode == ECameraMode::FPS)
	{
		cameraControl.eye = cameraControl.center;
		at = cameraControl.center
			+ glm::quat(glm::vec3(glm::radians(cameraControl.pitch), glm::radians(cameraControl.yaw), 0.0f))
			* glm::vec3(0.0f, 0.0f, 1.0f); // position + rotated forward
	}
	else if (cameraControl.mode == ECameraMode::TPS)
	{
		cameraControl.eye = cameraControl.center
			+ glm::quat(glm::vec3(glm::radians(cameraControl.pitch), glm::radians(cameraControl.yaw), 0.0f))
			* cameraControl.offsetInitial;
		at = cameraControl.center;
	}

	viewMatrix = glm::lookAt(cameraControl.eye, at, glm::vec3(0.0f, 1.0f, 0.0f));
}

void ViewController::onFramebufferSizeCallback(unsigned int width, unsigned int height)
{
	projectionMatrix = glm::perspectiveFov(glm::radians(cameraControl.fov), static_cast<float>(width),
		static_cast<float>(height), cameraControl.near, cameraControl.far);
	cameraControl.width = width;
	cameraControl.height = height;
}

void ViewController::onCursorPositionCallback(double xpos, double ypos)
{
	const double dx = xpos - mouseControl.lastX;
	const double dy = ypos - mouseControl.lastY;
	rotateCameraAlongMouse(static_cast<float>(dx), static_cast<float>(dy));

	mouseControl.lastX = xpos;
	mouseControl.lastY = ypos;
}

void ViewController::rotateCameraAlongMouse(float dx, float dy)
{
	float& yaw = cameraControl.yaw;
	float& pitch = cameraControl.pitch;
	
	yaw += dx * mouseControl.sensitivity;
	pitch += dy * mouseControl.sensitivity;

	if (pitch > cameraControl.pitchMax)
		pitch = cameraControl.pitchMax;
	else if (pitch < cameraControl.pitchMin)
		pitch = cameraControl.pitchMin;

	if (cameraControl.mode == ECameraMode::TPS)
	{
		cameraControl.offset = 
			cameraControl.radius * glm::vec3(0.0f, glm::sin(glm::radians(pitch)), glm::cos(glm::radians(pitch)));
		/*cameraControl.offset = cameraControl.radius * glm::vec3(
			glm::cos(glm::radians(pitch)) * glm::sin(glm::radians(yaw)),
			glm::sin(glm::radians(pitch)),
			glm::cos(glm::radians(pitch)) * glm::cos(glm::radians(yaw)));*/
	}
}