#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <memory>

#include "Game.hpp"
#include "Globals.hpp"

constexpr unsigned int SCREEN_WIDTH = 1600;
constexpr unsigned int SCREEN_HEIGHT = 1200;

std::unique_ptr<Game> gGame;

static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
static void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

bool init(GLFWwindow*& window);

int main(int argc, char* argv[])
{
	GLFWwindow* window;
	if (!init(window)) return -1;

	double prevTime = glfwGetTime();
	double lastTime = glfwGetTime();
	uint32_t frames = 0;

	while (!glfwWindowShouldClose(window))
	{
		const double currentTime = glfwGetTime();
		const double deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		// frame counter
		frames++;
		if (currentTime - prevTime >= 1.0) // per 1.0 sec
		{
			Globals::fps = double(frames);
			prevTime = currentTime;
			frames = 0;
		}

		glfwPollEvents();

		// manage user input
		gGame->processInput(static_cast<float>(deltaTime));

		// update game state
		gGame->update(static_cast<float>(deltaTime));

		// render
		gGame->render();

		glfwSwapBuffers(window);
	}

	// ResourceManager::Clear();

	glfwTerminate();
	return 0;
}

bool init(GLFWwindow*& window)
{
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return false;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// glfwWindowHint(GLFW_RESIZABLE, false);

	const auto monitor = glfwGetPrimaryMonitor();
	const auto mode = glfwGetVideoMode(monitor);

	const unsigned int width = mode->width;
	const unsigned int height = mode->height;
	//const float width = SCREEN_WIDTH;
	//const float height = SCREEN_HEIGHT;

	//window = glfwCreateWindow(width, height, "TOOM Eternal", monitor, nullptr); // Full screen
	window = glfwCreateWindow(width, height, "TOOM Eternal", nullptr, nullptr);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Hide cursor
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return false;
	}

	glfwSetKeyCallback(window, KeyCallback);
	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
	glfwSetCursorPosCallback(window, CursorPositionCallback);
	glfwSetMouseButtonCallback(window, MouseButtonCallback);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glViewport(0, 0, width, height);

	double firstX, firstY;
	glfwGetCursorPos(window, &firstX, &firstY);

	gGame = std::make_unique<Game>(firstX, firstY, width, height);
	gGame->init();

	return true;
}

static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	//std::clog << "Key callback: key=" << key << " scancode=" << scancode << " action=" << action << " mode=" << mode << "\n";
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS) gGame->keys[key] = true;
		else if (action == GLFW_RELEASE)
		{
			gGame->keys[key] = false;
			gGame->keysProcessed[key] = false;
		}
	}
}

static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	//std::clog << "Mouse button callback: button=" << button << " action=" << action << " mods=" << mods << "\n";
	if (button >= 0 && button < 8)
	{
		if (action == GLFW_PRESS)
		{
			gGame->mouseButtons[button] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			gGame->mouseButtons[button] = false;
		}
	}
}

static void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
	//std::clog << "Cursor position callback: " << std::fixed << xpos << " " << ypos << "\n";
	gGame->viewController.onCursorPositionCallback(xpos, ypos);
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	gGame->viewController.onFramebufferSizeCallback(width, height);
}
