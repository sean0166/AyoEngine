#pragma once
#include <map>
#include <string>
#include <functional>
#include <assimp/material.h>
#include <iostream>

namespace Ayo
{
	/*
	* This is a wrapper class around the TextureTypeEnum type that will allow us to
	* associated each emumeration to a string.
	* TODO: This can be made faster using a bimap. Look into this in the future.
	*/
	struct TextureType
	{
		enum class TextureTypeEnum
		{
			kDiffuseTexture,
			kSpecularTexture,
			kNormalTexture,
			kHeightTexture
		};

		struct TextureTypePairings
		{
			std::string name;
			aiTextureType aiTexType;
		};

		using TextureTypeMappingPair = std::pair<TextureTypeEnum, TextureTypePairings>;
		using TextureTypeMapping = std::map<TextureTypeEnum, TextureTypePairings>;

		explicit TextureType(TextureTypeEnum type)
			: m_type(type)
		{};

		explicit TextureType(const std::string& typeStr)
			: m_type(stringToTextureType(typeStr))
		{};

		TextureType(const TextureType&) = default;
		TextureType& operator=(const TextureType&) = default; 


		operator std::string() const { return textureTypeToString(m_type); };
		operator TextureTypeEnum () const { return m_type; };
		operator aiTextureType() const;

		friend std::ostream& operator<< (std::ostream& os, const TextureType& type)
		{
			os << static_cast<std::string>(type);
			return os;
		}

		static int numTypes() { return m_enumTypePairings.size(); };

		static void runForEachTypeEnum(const std::function<void(TextureTypeEnum)>& func); 

	private:
		std::string textureTypeToString(TextureTypeEnum type) const;
		TextureTypeEnum stringToTextureType(const std::string& typeStr) const;

		aiTextureType textureTypeToAssimpTextureType(TextureTypeEnum type) const;
		TextureTypeEnum assimpEnumToTextureType(aiTextureType aiType) const;

		TextureTypeEnum m_type;

		static TextureTypeMapping m_enumTypePairings; 
	};
}