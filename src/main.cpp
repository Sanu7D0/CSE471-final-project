#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <memory>

#include "Game.hpp"

//constexpr unsigned int SCREEN_WIDTH = 800;
//constexpr unsigned int SCREEN_HEIGHT = 600;

std::unique_ptr<Game> gGame;

static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
static void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

bool init(GLFWwindow*& window);
void initLight();

int main(int argc, char* argv[])
{
	GLFWwindow* window;
	if (!init(window)) return -1;

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	while (!glfwWindowShouldClose(window))
	{
		const auto currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glfwPollEvents();

		// manage user input
		gGame->processInput(deltaTime);

		// update game state
		gGame->update(deltaTime);

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

	//window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "BackToCG", nullptr, nullptr);
	window = glfwCreateWindow(mode->width, mode->height, "BackToCG", monitor, nullptr);
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
	//glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glViewport(0, 0, mode->width, mode->height);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	initLight();

	double firstX, firstY;
	glfwGetCursorPos(window, &firstX, &firstY);

	gGame = std::make_unique<Game>(firstX, firstY, mode->width, mode->height);
	gGame->init();

	return true;
}

void initLight()
{
	// TODO
}

static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	std::clog << "Key callback: key=" << key << " scancode=" << scancode << " action=" << action << " mode=" << mode <<
		"\n";
	// when a user presses the escape key, we set the WindowShouldClose property to true, closing the application
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

	//GLFW_REPEAT = key holding
}

static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	std::clog << "Mouse button callback: button=" << button << " action=" << action << " mods=" << mods << "\n";
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		//
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		//
	}
}

static void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
	//std::clog << "Cursor position callback: " << std::fixed << xpos << " " << ypos << "\n";
	gGame->controller.onCursorPositionCallback(xpos, ypos);
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	gGame->controller.onFramebufferSizeCallback(width, height);
}