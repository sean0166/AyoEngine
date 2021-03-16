#include <memory>
#include <iostream>
#include <string>
#include <glad/glad.h>
#include <vector>

#include <ayo/rendering/window/window.h>
#include <ayo/rendering/mesh/texture_type.h>
#include <ayo/rendering/shader/shader.h>
#include <ayo/rendering/mesh/mesh.h>
#include <ayo/rendering/mesh/model.h>
#include <ayo/utility/resource_loading/model_loader.h>


#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <stb_image/stb_image.h>

namespace WindowConstants
{
	static const int kHeight		= 600;
	static const int kWidth			= 800;
	static const std::string kTitle = "Ayo Engine Game";
	static const unsigned int kFrameRate = 60;
}

int main(int argc, char** argv)
{
	auto pWindow = std::make_unique<Ayo::Window>(WindowConstants::kHeight,
		WindowConstants::kWidth,
		WindowConstants::kTitle);

	std::vector<Ayo::Vertex> v{ Ayo::Vertex() };
	std::vector<unsigned int> i{ 1,2,3 };
	std::vector<Ayo::Texture> t{ Ayo::Texture(1, Ayo::TextureType::TextureTypeEnum::kSpecularTexture, "blah") };
	
	const char* pVert =
		"#version 330 core\n"
		"layout(location = 0) in vec3 aPos;\n"
		"layout(location = 1) in vec3 aNormal;\n"
		"layout(location = 2) in vec2 aTexCoords;\n"
		"out vec2 TexCoords;\n"
		"out vec3 SomeColorTemp;\n"
		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"
		"void main()\n"
		"{\n"
		"TexCoords = aTexCoords;\n"
		"SomeColorTemp = aPos;\n"
		"gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
		"}\n";

	const char* pFrag =
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"in vec2 TexCoords;\n"
		"in vec3 SomeColorTemp;\n"
		"uniform sampler2D texture_diffuse1;\n"
		"void main()\n"
		"{\n"
		"FragColor = texture(texture_diffuse1, TexCoords);\n"
		//"FragColor = vec4(SomeColorTemp, 0.0f);\n"
		"}\n";


	Ayo::ShaderProgram p{
		pVert,
		pFrag
	};

	/*unsigned int ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	*/
	float  j = 0.0f;
	double  test = 0.0f;
	float  angle = 0.1f;
	float  incAmt = 1.0f;
	bool bSubtract = false;

	float currentTime = 0.0f;
	float previousTime = 0.0f;

	glm::vec3 gravity{ 0, -2, 0 };


	float dt = 0.0f;
	//auto backpack = Ayo::ResourceLoading::loadModel("backpack/source/Survival_BackPack_2.fbx");

	auto head = Ayo::ResourceLoading::loadModel("backpack/backpack.obj");

	while (!pWindow->isCloseRequested())
	{

		/*model = glm::rotate(model, glm::radians(angle),
						glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(std::cos(angle)),
			glm::vec3(0.0f, 1.0f, 0.0f));*/
		pWindow->clearColorBuffer();
		p.setActive();
		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f,-8.0f));
		p.bindMat4("projection", projection);
		p.bindMat4("view", view);

		// render the loaded model
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	
		model = glm::rotate(model, glm::radians(dt), glm::vec3(0.0f, 1.0f, 0.0f));	
		p.bindMat4("model", model);

		head.render(p);

		pWindow->update();
		pWindow->processInput();
		dt += 0.05;
	}
	return 0;
}