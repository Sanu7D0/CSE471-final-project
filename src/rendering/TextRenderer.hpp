#pragma once
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <map>
#include <iostream>
#include <string>

#include "Shader.hpp"

struct Character
{
	GLuint textureID;
	glm::ivec2 size;
	glm::ivec2 bearing;
	uint32_t advance;
};

class TextRenderer
{
public:
	TextRenderer(float width, float height, const std::string& fontPath)
		: _width(width), _height(height), _projection(glm::ortho(0.0f, _width, 0.0f, _height))
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		loadFont(fontPath);
	}

	void renderText(const Shader& shader, const std::string& text, float x, float y, float scale, glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f))
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		shader.use();
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(_projection));
		glUniform3f(glGetUniformLocation(shader.ID, "textColor"), color.x, color.y, color.z);
		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(VAO);

		for (const auto& c: text)
		{
			const auto& ch = _characters[c];

			float xpos = x + ch.bearing.x * scale;
			float ypos = y - (ch.size.y - ch.bearing.y) * scale;
			float w = ch.size.x * scale;
			float h = ch.size.y * scale;

			float vertices[6][4] = {
				{xpos, ypos + h, 0.0f, 0.0f},
				{xpos, ypos, 0.0f, 1.0f},
				{xpos + w, ypos, 1.0f, 1.0f},

				{xpos, ypos + h, 0.0f, 0.0f},
				{xpos + w, ypos, 1.0f, 1.0f},
				{xpos + w, ypos + h, 1.0f, 0.0f}
			};

			glBindTexture(GL_TEXTURE_2D, ch.textureID);

			// Update content of VBO memory
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glDrawArrays(GL_TRIANGLES, 0, 6);

			x += static_cast<float>(ch.advance >> 6) * scale; // divide by 64 to get value in pixels
		}

		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void onFrameBufferSizeCallback(int width, int height)
	{
		_width = static_cast<float>(width);
		_height = static_cast<float>(height);
		_projection = glm::ortho(0.0f, _width, 0.0f, _height);
	}

private:
	float _width, _height;
	glm::mat4 _projection;
	GLuint VAO, VBO;
	std::map<char, Character> _characters;

	void loadFont(const std::string& path)
	{
		FT_Library ft;
		if (FT_Init_FreeType(&ft))
		{
			std::cerr << "FreeType Library init failed" << std::endl;
			return;
		}

		// TODO: Root dir?
		FT_Face face;
		if (FT_New_Face(ft, path.c_str(), 0, &face))
		{
			std::cerr << "Font load failed: " << path << std::endl;
			return;
		}

		FT_Set_Pixel_Sizes(face, 0, 48);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

		// Load first 128 characters of ASCII set
		for (unsigned char c = 0; c < 128; ++c)
		{
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				std::cerr << "Glyph load failed: " << c << std::endl;
				continue;
			}

			// Generate texture
			GLuint textureID;
			glGenTextures(1, &textureID);
			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
			);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			// Store character
			auto character = Character(
				textureID,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				static_cast<uint32_t>(face->glyph->advance.x)
			);
			_characters[c] = std::move(character);
		}
		glBindTexture(GL_TEXTURE_2D, 0);

		FT_Done_Face(face);
		FT_Done_FreeType(ft);

		std::cout << "Load font: " << path << std::endl;
	}
};