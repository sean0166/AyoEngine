#pragma once
#include <string>
#include <vector>

struct aiNode;
struct aiScene;
struct aiMesh;
struct aiMaterial;

namespace Ayo
{
	class Mesh;
	class Model;
	struct TextureType;
	struct Texture;

	namespace ResourceLoading
	{
		Model loadModel(const std::string& path);
	}


}
