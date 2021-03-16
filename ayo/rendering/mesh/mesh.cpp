#include "mesh.h"
#include "../shader/shader.h"

#include <unordered_map>
#include <glad/glad.h>
#include <sstream>
#include <iostream>

namespace Ayo
{
	Mesh::Mesh(const std::vector<Vertex>& vertices,
			   const std::vector<unsigned int>& indicies,
			   const std::vector<Texture>& textures) : m_vertices(vertices),
													   m_indicies(indicies),
													   m_textures(textures)
	{
		createMesh();
	}

	void Mesh::render(const ShaderProgram& program) const
	{
		std::unordered_map<std::string, std::size_t> numTexturesByType;
		
		//Activate all of the texture units for this mesh and then
		//Bind them to our shader program
		//TODO: MAy be able to make size_t instead of unsigned int index
		for (unsigned int i = 0; i < m_textures.size(); ++i)
		{
			glActiveTexture(GL_TEXTURE0 + i);

			const std::string textureTypeName = static_cast<std::string>(m_textures[i].textureType);

			if (numTexturesByType.count(textureTypeName) == 0)
			{
				numTexturesByType[textureTypeName] = 1;
			}
			else
			{
				numTexturesByType[textureTypeName]++;
			}

			std::ostringstream materialNameOss;
			const int textureTypeNum = numTexturesByType.at(textureTypeName);

			materialNameOss << "material." << textureTypeName << textureTypeNum;

			program.bindInt(materialNameOss.str(), textureTypeNum);

			glBindTexture(GL_TEXTURE_2D, m_textures[i].id); 
		}
		glActiveTexture(GL_TEXTURE0);

		//Perform the draw call for the mesh
		glBindVertexArray(m_vao); 
		glDrawElements(GL_TRIANGLES, m_indicies.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void Mesh::createMesh()
	{
		glGenVertexArrays(1, &m_vao);

		glGenBuffers(1, &m_vbo);
		glGenBuffers(1, &m_ebo);

		glBindVertexArray(m_vao);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex),
			&m_vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indicies.size() * sizeof(unsigned int),
			&m_indicies[0], GL_STATIC_DRAW);

		//Positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

		//Normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

		//Texture Coordinates
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureCoords));

		//Unbind VAO
		glBindVertexArray(0);
	}

}