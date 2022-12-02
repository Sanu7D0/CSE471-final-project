#include "ResourceManger.hpp"

ResourceManager* ResourceManager::_instance = nullptr;

//Shader& ResourceManager::LoadShader(const std::string& name, const char* vShaderPath, const char* fShaderPath)
//{
//	/*_shaders[name] = Shader(vShaderPath, fShaderPath);
//	return _shaders[name];*/
//}
//
//Shader& ResourceManager::GetShader(const std::string& name)
//{
//	//return _shaders[name];
//}

TextRenderer* ResourceManager::LoadTextRenderer(float width, float height)
{
	/*_shaders["text"] = Shader("resource/shader/Text.vert", "resource/shader/Text.frag");
	_textRenderer = new TextRenderer(width, height, "resource/font/DroidSansMono.ttf");
	return _textRenderer;*/
	return nullptr;
}


