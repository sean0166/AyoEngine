#include "model.h"
#include "mesh.h"
#include "../shader/shader.h"

namespace Ayo
{
	Model::Model(const std::vector<Mesh>& meshes) : m_meshes(meshes)
	{
	}

	void Model::render(const ShaderProgram& program) const
	{
		for (const Mesh& mesh : m_meshes)
		{
			mesh.render(program);
		}
	}

}