#include "model_loader.h"
#include "texture_loader.h"

#include <ayo/rendering/mesh/mesh.h>
#include <ayo/rendering/mesh/model.h>
#include <ayo/rendering/mesh/texture_type.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>


namespace Ayo
{
	std::vector<Mesh>& processNode(aiNode* node, const aiScene* scene,
		const std::string& directory, std::vector<Mesh>& meshes);

	Mesh processMesh(aiMesh* mesh, const aiScene* scene, const std::string& directory);

	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, const TextureType& textureType, const std::string& directory);

	Model ResourceLoading::loadModel(const std::string& path)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
		

		//TODO: Add logger logging
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
			std::terminate();
		}
		std::string directory = path.substr(0, path.find_last_of('/'));
		std::vector<Mesh> modelMeshes;

		return Model(processNode(scene->mRootNode, scene, directory, modelMeshes));
	}

	std::vector<Mesh>& processNode(aiNode* node, const aiScene* scene, const std::string& directory, std::vector<Mesh>& meshes)
	{
		for (std::size_t i = 0; i < node->mNumMeshes; ++i)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(processMesh(mesh, scene, directory));
		}

		//Process all of the children of this node
		for (size_t i = 0; i < node->mNumChildren; ++i)
		{
			processNode(node->mChildren[i], scene, directory, meshes);
		}

		return meshes;
	}

	Mesh processMesh(aiMesh* mesh, const aiScene* scene, const std::string& directory)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indicies;
		std::vector<Texture> textures;
		
		const auto store_to_glm_vec3 = [](glm::vec3& to, const auto& from) 
		{
			to.x = from.x;
			to.y = from.y;
			to.z = from.z;
		};

		//Store vertices
		for (size_t i = 0; i < mesh->mNumVertices; ++i)
		{
			Vertex v;
			store_to_glm_vec3(v.position, mesh->mVertices[i]);
			store_to_glm_vec3(v.normal, mesh->mNormals[i]);

			if (mesh->mTextureCoords[0])
			{
				//We have texture coordinates
				v.textureCoords.x = mesh->mTextureCoords[0][i].x;
				v.textureCoords.y = mesh->mTextureCoords[0][i].y;
			}
			else
			{
				//Default texture coordinates
				v.textureCoords = glm::vec2(0.0f, 0.0f);
			}
			vertices.push_back(v);
		}

		//Store indicies
		for (size_t i = 0; i < mesh->mNumFaces; ++i)
		{
			aiFace curFace = mesh->mFaces[i];

			for (size_t j = 0; j < curFace.mNumIndices; ++j)
			{
				indicies.push_back(curFace.mIndices[j]);
			}
		}

		//Store Materials/Textures
		//TODO: pair aiTextureType to TextureType in the future
		//This way we can just pass TextureType to loadMaterialTextures
		
		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

			auto loadMaterialTextureForType = 
			[&textures, &material, &directory](TextureType::TextureTypeEnum type)
			{
				auto texturesForType = loadMaterialTextures(material, TextureType(type), directory);
				textures.insert(textures.end(), texturesForType.begin(), texturesForType.end());
			}; 

			TextureType::runForEachTypeEnum(loadMaterialTextureForType);
		}

		return Mesh(vertices, indicies, textures);
	}

	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, const TextureType& textureType, const std::string& directory)
	{
		std::vector<Texture> textures;
		aiTextureType aiEnumTextureType = static_cast<aiTextureType>(textureType);
		auto test = mat->GetTextureCount(aiEnumTextureType);

		for (size_t i = 0; i < mat->GetTextureCount(aiEnumTextureType); ++i)
		{
			aiString texturePath;
			mat->GetTexture(aiEnumTextureType, i, &texturePath);

			unsigned int textureId = ResourceLoading::LoadTexture(texturePath.C_Str(), directory);;
			textures.push_back(Texture(textureId, textureType, texturePath.C_Str()));
		}
		
		return textures;
	}
}