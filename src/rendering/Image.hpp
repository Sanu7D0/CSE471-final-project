#pragma once
#include <glad/glad.h>

#include <string>

#include "Shader.hpp"

class Image
{
public:
	Image(const std::string& path, bool repeat = true)
	{
		load(path, repeat);
	}

	void draw(const Shader& shader)
	{
		shader.use();

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void bindTexture(const Shader& shader, const std::string& name, int unit = 0)
	{
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, _textureID);
	}

private:
	GLuint _textureID;
	int _width, _height;

	void load(const std::string& path, bool repeat=false)
	{
		int nrComponents;
		if (unsigned char* data = stbi_load(path.c_str(), &_width, &_height, &nrComponents, 0))
		{
			glGenTextures(1, &_textureID);
			glBindTexture(GL_TEXTURE_2D, _textureID);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			if (repeat)
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			}

			glBindTexture(GL_TEXTURE_2D, 0);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Texture failed to load at path: " << path << std::endl;
			stbi_image_free(data);
			return;
		}
		std::cout << "Load image: " << path << "\n";
	}
};