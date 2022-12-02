#pragma once

#include <string>
#include <map>

#include "rendering/Shader.hpp"
#include "rendering/TextRenderer.hpp"

class ResourceManager
{
private:
	static ResourceManager* _instance;

	ResourceManager();
	~ResourceManager();

public:
	static ResourceManager* Instance()
	{
		if (_instance == nullptr) _instance = new ResourceManager();
		return _instance;
	}

	//static Shader& LoadShader(const std::string& name, const char* vShaderPath, const char* fShaderPath);
	//static Shader& GetShader(const std::string& name);
	static TextRenderer* LoadTextRenderer(float width, float height);

private:
	//static std::map<std::string, Shader> _shaders;
	//static TextRenderer* _textRenderer;
};