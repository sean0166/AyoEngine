#pragma once
#include "texture.h"
#include "vertex.h"

#include <vector>

namespace Ayo
{
	class ShaderProgram;

	class Mesh
	{
	public: 
		Mesh(const std::vector<Vertex>& vertices,
			 const std::vector<unsigned int>& indicies,
			 const std::vector<Texture>& textures);

		void render(const ShaderProgram& program) const;

	private:
		void createMesh();

		std::vector<Vertex> m_vertices;
		std::vector<unsigned int> m_indicies;
		std::vector<Texture> m_textures;

		unsigned int m_vao, m_vbo, m_ebo;
	};
}