#pragma once
#include <vector>

namespace Ayo
{
	class Mesh;
	class ShaderProgram;

	class Model
	{
	public:
		explicit Model(const std::vector<Mesh>& meshes);
		void render(const ShaderProgram& program) const;

	private:
		std::vector<Mesh> m_meshes;

	};
}