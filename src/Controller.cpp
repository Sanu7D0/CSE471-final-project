#include "Controller.hpp"

#include <iostream>

void CameraControl::followTargetPosition(float dt)
{
	if (transform.position == followPosition)
		return;
	transform.position = followPosition;
	// TODO: lerp
}

Controller::Controller(MouseControl mouse, CameraControl camera, glm::mat4& view, glm::mat4& projection)
	: mouseControl(mouse), cameraControl(camera), viewMatrix(view), projectionMatrix(projection)
{
}

void Controller::update(float dt)
{
	cameraControl.followTargetPosition(dt);

	//viewMatrix = glm::mat4_cast(cameraControl.transform.rotation) * glm::translate(glm::mat4(1.0f), -cameraControl.transform.position); // camera facing -Z axis
	const glm::vec3 at = cameraControl.transform.position + cameraControl.transform.rotation * glm::vec3(0.0f, 0.0f, 1.0f); // position + forward

	viewMatrix = glm::lookAt(cameraControl.transform.position, at, glm::vec3(0.0f, -1.0f, 0.0f)); // -1 to look upside-down
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
	const double dy = mouseControl.lastY - ypos;
	//const double dy = ypos - mouseControl.lastY;
	rotateCameraAlongMouse(static_cast<float>(dx), static_cast<float>(dy));

	// TODO: rotate body orientation along camera

	mouseControl.lastX = xpos;
	mouseControl.lastY = ypos;
}

void Controller::rotateCameraAlongMouse(float dx, float dy)
{
	dx *= mouseControl.sensitivity;
	dy *= mouseControl.sensitivity;

	switch (cameraControl.mode)
	{
	case TPS:
		break;
	case FPS:
		cameraControl.yaw += dx;
		cameraControl.pitch += dy;

		/*if (yaw > 360.0f)
			yaw -= 360.0f;
		else if (yaw < -360.0f)
			yaw += 360.0f;*/
		if (cameraControl.pitch > cameraControl.pitchMax)
			cameraControl.pitch = cameraControl.pitchMax;
		else if (cameraControl.pitch < cameraControl.pitchMin)
			cameraControl.pitch = cameraControl.pitchMin;

		cameraControl.transform.rotation = glm::quat(
			glm::vec3(glm::radians(cameraControl.pitch), glm::radians(cameraControl.yaw), 0.0f));

		break;
	}
}