#include "texture_loader.h"

#include <glad/glad.h> 
#include <stb_image/stb_image.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>

unsigned int Ayo::ResourceLoading::LoadTexture(const std::string& path, const std::string& directory)
{
	const std::string filePath = directory + "/" + path;

	unsigned int textureId; 
	glGenTextures(1, &textureId);

	int width, height;
	int numChannels;

	stbi_set_flip_vertically_on_load(true);
	unsigned char* pTextureData = stbi_load(filePath.c_str(), &width, &height, &numChannels, 0);

	if (pTextureData)
	{
		GLenum textureFormat;
		switch (numChannels)
		{
		case 1:
			textureFormat = GL_RED;
			break;
		case 3:
			textureFormat = GL_RGB; 
			break;
		case 4:
			textureFormat = GL_RGBA;
			break;
		default:
			textureFormat = GL_RED;
		}

		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, textureFormat, width, height, 0, 
					 textureFormat, GL_UNSIGNED_BYTE, pTextureData);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(pTextureData);
	}
	else
	{
		//TODO: Add To logging!
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(pTextureData);
	}

	return textureId;
}
