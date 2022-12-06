#pragma once
#include <glad/glad.h>

#include <string>

#include "Shader.hpp"

class Image
{
public:
	Image(const std::string& path)
	{
		load(path);
	}

	void draw(const Shader& shader)
	{
		shader.use();
	}

private:
	GLuint _textureID;
	int _width, _height;

	void load(const std::string& path)
	{
		int nrComponents;
		if (unsigned char* data = stbi_load(path.c_str(), &_width, &_height, &nrComponents, 0))
		{

			GLenum format = GL_RGB;
			if (nrComponents == 1)
				format = GL_RED;
			else if (nrComponents == 3)
				format = GL_RGB;
			else if (nrComponents == 4)
				format = GL_RGBA;

			glGenTextures(1, &_textureID);
			glBindTexture(GL_TEXTURE_2D, _textureID);

			//glTexImage2D(GL_TEXTURE_2D, 0, format, _width, _height, 0, format, GL_UNSIGNED_BYTE, data);
			glTexImage2D(GL_TEXTURE_2D, 0, format, _width, _height, 0, format, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glBindTexture(GL_TEXTURE_2D, 0);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Texture failed to load at path: " << path << std::endl;
			stbi_image_free(data);
			return;
		}
	}
};