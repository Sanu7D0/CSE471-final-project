#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	unsigned int ID;

	Shader(const char* vShaderPath, const char* fShaderPath)
	{
		GLuint vShaderID = glCreateShader(GL_VERTEX_SHADER);
		GLuint fShaderID = glCreateShader(GL_FRAGMENT_SHADER);

		// Read the vertex shader code from the file
		std::string vShaderCode;
		readShaderFile(vShaderPath, vShaderCode);

		// Read the fragment shader code from the file
		std::string fShaderCode;
		readShaderFile(fShaderPath, fShaderCode);

		// Compile vertex shader
		std::clog << "Compiling shader " << vShaderPath << "\n";
		const char* vSourcePtr = vShaderCode.c_str();
		glShaderSource(vShaderID, 1, &vSourcePtr, nullptr);
		glCompileShader(vShaderID);
		// Check vertex shader
		checkCompileErrors(vShaderID, "VERTEX");

		// Compile fragment shader
		std::clog << "Compiling shader " << fShaderPath << "\n";
		const char* fSourcePtr = fShaderCode.c_str();
		glShaderSource(fShaderID, 1, &fSourcePtr, nullptr);
		glCompileShader(fShaderID);
		// Check fragment shader
		checkCompileErrors(fShaderID, "FRAGMENT");

		// Link the program
		std::clog << "Linking program\n";
		ID = glCreateProgram();
		glAttachShader(ID, vShaderID);
		glAttachShader(ID, fShaderID);
		glLinkProgram(ID);
		// Check the program
		checkCompileErrors(ID, "PROGRAM");

		glDetachShader(ID, vShaderID);
		glDetachShader(ID, fShaderID);

		glDeleteShader(vShaderID);
		glDeleteShader(fShaderID);

		std::clog << "Shader loading complete with id " << ID << std::endl;
	}

	~Shader()
	{
		if (ID == 0) return;

		glDeleteProgram(ID);
	}

	void use() const
	{
		if (ID == 0) return;

		glUseProgram(ID);
	}

	// utility uniform functions
	void setBool(const std::string &name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	void setInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setVec3(const std::string& name, const glm::vec3& vec) const
	{
		glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(vec));
	}
	void setMat4(const std::string& name, const glm::mat4& mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
	}

private:
	void readShaderFile(const char* path, std::string& code)
	{
		std::string shaderCode;
		std::ifstream shaderStream(path, std::ios::in);
		if (shaderStream.is_open())
		{
			std::stringstream tempStream;
			tempStream << shaderStream.rdbuf();
			code = tempStream.str();
			shaderStream.close();
		}
		else
		{
			std::cerr << "Failed to open " << path << std::endl;
		}
	}

	void checkCompileErrors(unsigned int shader, const std::string& type) const
	{
		int success;
		char infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog
					<< "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog
					<< "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
	}
};
