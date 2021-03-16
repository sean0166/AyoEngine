#include "shader.h"
#include <glad/glad.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Ayo
{
	ShaderProgram::ShaderProgram(std::string const& vertShaderSource, std::string const& fragShaderSource)
	{
		Shader vertShader;
		vertShader.shaderSource = vertShaderSource;
		vertShader.shaderType = GL_VERTEX_SHADER;

		Shader fragShader;
		fragShader.shaderSource = fragShaderSource;
		fragShader.shaderType = GL_FRAGMENT_SHADER;

		std::vector<Shader> shaders{ vertShader, fragShader };

		createProgram(shaders);

	}


	void ShaderProgram::setActive() const
	{
		glUseProgram(m_programId);
	}

	void ShaderProgram::bindInt(const std::string& varName, int value) const
	{
		glUniform1i(glGetUniformLocation(m_programId, varName.c_str()), value);
	}

	void ShaderProgram::bindFloat(const std::string& varName, float value) const
	{
		glUniform1f(glGetUniformLocation(m_programId, varName.c_str()), value);
	}

	void ShaderProgram::bindBool(const std::string& varName, bool value) const
	{
		glUniform1i(glGetUniformLocation(m_programId, varName.c_str()), static_cast<int>(value));
	}

	void ShaderProgram::bindMat4(const std::string& varName, glm::mat4 value) const
	{
		glUniformMatrix4fv(glGetUniformLocation(m_programId, varName.c_str()), 1, 
												GL_FALSE, glm::value_ptr(value));
	}


	void ShaderProgram::createProgram(const std::vector<Shader>& shaderList)
	{
		std::vector<unsigned int> shaderIds;
		char programLinkBuffer[512];

		//Create and compile shaders
		for (const auto& shader : shaderList)
		{
			shaderIds.push_back(
				createShader(shader.shaderSource, shader.shaderType)
			);
		}

		m_programId = glCreateProgram();

		//Link shaders
		for (const auto& shaderId : shaderIds)
		{
			glAttachShader(m_programId, shaderId);
		}

		//Link program
		glLinkProgram(m_programId);

		int linkSuccessful;
		glGetProgramiv(m_programId, GL_LINK_STATUS, &linkSuccessful);

		if (!linkSuccessful)
		{
			glGetProgramInfoLog(m_programId, 512, NULL, programLinkBuffer);
			std::cout << "The following program failed to link: \n"
				<< programLinkBuffer;
		}

		//The shaders are linked, so we can get rid of the data
		for (const auto & shaderId : shaderIds)
		{
			glDeleteShader(shaderId);
		}
	}


	unsigned int ShaderProgram::createShader(const std::string& shaderSource, unsigned int shaderType)
	{
		unsigned int shaderId;
		const char* shaderSourceBuf = shaderSource.c_str();

		shaderId = glCreateShader(shaderType);
		glShaderSource(shaderId, 1, &shaderSourceBuf, NULL);

		glCompileShader(shaderId);

		int success;
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			char shaderInfoLog[512];
			glGetShaderInfoLog(shaderId, 512, NULL, shaderInfoLog);
			//Shader failed to compile
			//TODO: log the error message
			//Also, add way to detect specific shader that failed to compile
			// and to make sure that we do not link that shader
			// I would store in the shader vector a compile/link status
			std::cout << "A shader has failed to compile! Info: \n"
				<< shaderInfoLog;
		}

		return shaderId;
	}
}