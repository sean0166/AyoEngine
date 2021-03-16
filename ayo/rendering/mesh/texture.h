#pragma once
#include "texture_type.h"
#include <string>

namespace Ayo
{
	struct Texture
	{
		Texture(unsigned int id, TextureType::TextureTypeEnum texType, const std::string& path) 
			: id(id),
			  textureType(texType),
			  path(path)
		{
		};

		Texture(unsigned int id, TextureType texType, const std::string& path)
			: id(id),
			textureType(texType),
			path(path)
		{
		};

		unsigned int id;

		TextureType textureType;

		/* 
		* This is a path to the texture.
		* We will use this to avoid loading two of the sames texture into memory.
		*/
		std::string path;
	};
}