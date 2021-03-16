#pragma once
#include<string>
#include <vector>
#include <stdint.h>
#include <glm\mat4x4.hpp>

namespace Ayo
{

	struct Shader
	{
		std::string shaderSource;
		unsigned int shaderType;
	};

	class ShaderProgram
	{
	public:
		//TODO: Make this shader usable with more forms of shaders
		/*explicit ShaderProgram(std::vector<Shader> const& shaders);*/
		ShaderProgram(const std::string& vertShaderSource,
					  const std::string& fragShaderSource);

		void setActive() const; 

		//Uniform bindings
		void bindInt(const std::string& varName, int value) const;
		void bindFloat(const std::string& varName, float value) const;
		void bindBool(const std::string& varName, bool value) const;
		void bindMat4(const std::string& varName, glm::mat4 value) const;


	private:
		unsigned int m_programId;
		//std::vector<Shader> m_shaders;

		void createProgram(const std::vector<Shader>& shaderList);
		unsigned int createShader(const std::string& shaderSource, unsigned int shaderType);
	};
}