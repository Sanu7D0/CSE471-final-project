#include "Controller.hpp"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

CameraControl::CameraControl(glm::vec3 center, unsigned width, unsigned height)
	: center(center), width(width), height(height)
{
	// Normalize offset to radius
	offsetInitial = radius * glm::normalize(offsetInitial);
	offset = offsetInitial;
}

// Move camera "center" to following position
void CameraControl::followPlayer(float dt)
{
	// TODO: lerp
	center = player->transform.position;
}

Controller::Controller(MouseControl mouse, CameraControl camera, glm::mat4& view, glm::mat4& projection)
	: mouseControl(mouse), cameraControl(std::move(camera)), viewMatrix(view), projectionMatrix(projection)
{
}

void Controller::init(const std::shared_ptr<Player> &player)
{
	cameraControl.player = player;
	projectionMatrix = glm::perspectiveFov(
		glm::radians(cameraControl.fov),
		static_cast<float>(cameraControl.width),
		static_cast<float>(cameraControl.height),
		cameraControl.near,
		cameraControl.far
	);

	update(0.0f);
}


void Controller::update(float dt)
{
	cameraControl.followPlayer(dt);

	glm::vec3 eye;
	glm::vec3 target;

	switch (cameraControl.mode)
	{
	case ECameraMode::FPS:
		// Rotate player body yaw along camera direction
		cameraControl.player->transform.rotation =
			glm::quat(glm::vec3(0.0, glm::radians(cameraControl.yaw), 0.0f));

		eye = cameraControl.center;
		// position + rotated forward
		target = cameraControl.center
			+ glm::quat(glm::vec3(glm::radians(cameraControl.pitch), glm::radians(cameraControl.yaw), 0.0f))
			* glm::vec3(0.0f, 0.0f, 1.0f);
		break;

	case ECameraMode::TPS:
		eye = cameraControl.center
			+ glm::quat(glm::vec3(glm::radians(cameraControl.pitch), glm::radians(cameraControl.yaw), 0.0f))
			* cameraControl.offsetInitial;
		// player position
		target = cameraControl.center;
		break;
	}

	//viewMatrix = glm::mat4_cast(cameraControl.transform.rotation) * glm::translate(glm::mat4(1.0f), -cameraControl.transform.position); // camera facing -Z axis
	//viewMatrix = glm::lookAt(eye, target, glm::vec3(0.0f, -1.0f, 0.0f)); // -1 to look upside-down
	viewMatrix = glm::lookAt(eye, target, glm::vec3(0.0f, 1.0f, 0.0f));
}

void Controller::onFramebufferSizeCallback(unsigned int width, unsigned int height)
{
	projectionMatrix = glm::perspectiveFov(glm::radians(cameraControl.fov), static_cast<float>(width),
		static_cast<float>(height), cameraControl.near, cameraControl.far);
	cameraControl.width = width;
	cameraControl.height = height;
}

void Controller::onCursorPositionCallback(double xpos, double ypos)
{
	const double dx = xpos - mouseControl.lastX;
	//const double dy = mouseControl.lastY - ypos;
	const double dy = ypos - mouseControl.lastY;
	rotateCameraAlongMouse(static_cast<float>(dx), static_cast<float>(dy));

	// TODO: rotate body orientation along camera

	mouseControl.lastX = xpos;
	mouseControl.lastY = ypos;
}

void Controller::rotateCameraAlongMouse(float dx, float dy)
{
	float& yaw = cameraControl.yaw;
	float& pitch = cameraControl.pitch;
	
	yaw += dx * mouseControl.sensitivity;
	pitch += dy * mouseControl.sensitivity;

	if (pitch > cameraControl.pitchMax)
		pitch = cameraControl.pitchMax;
	else if (pitch < cameraControl.pitchMin)
		pitch = cameraControl.pitchMin;

	switch (cameraControl.mode)
	{
	case FPS:
		/*cameraControl.rotation = glm::quat(
			glm::vec3(glm::radians(pitch), glm::radians(yaw), 0.0f));*/
		break;

	case TPS:
		cameraControl.offset = cameraControl.radius * glm::vec3(0.0f, glm::sin(glm::radians(pitch)), glm::cos(glm::radians(pitch)));
		/*cameraControl.offset = cameraControl.radius * glm::vec3(
			glm::cos(glm::radians(pitch)) * glm::sin(glm::radians(yaw)),
			glm::sin(glm::radians(pitch)),
			glm::cos(glm::radians(pitch)) * glm::cos(glm::radians(yaw)));*/

		break;
	}
}