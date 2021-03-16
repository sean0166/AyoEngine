#include "texture_type.h"
#include <algorithm> 
#include <cassert>

//Definition of the texture type mappings
Ayo::TextureType::TextureTypeMapping Ayo::TextureType::m_enumTypePairings = {
    { TextureTypeEnum::kDiffuseTexture, {"texture_diffuse", aiTextureType::aiTextureType_DIFFUSE} },
    { TextureTypeEnum::kSpecularTexture, {"texture_specular", aiTextureType::aiTextureType_SPECULAR} },
    { TextureTypeEnum::kNormalTexture, {"texture_normal", aiTextureType::aiTextureType_HEIGHT} },
    { TextureTypeEnum::kHeightTexture, {"texture_height", aiTextureType::aiTextureType_AMBIENT} },

};

Ayo::TextureType::operator aiTextureType() const
{
    return textureTypeToAssimpTextureType(m_type);
}


void Ayo::TextureType::runForEachTypeEnum(const std::function<void(TextureTypeEnum)>& func)
{
    std::for_each(m_enumTypePairings.begin(), m_enumTypePairings.end(), [&func](const TextureType::TextureTypeMappingPair& pair)
    {
        func(pair.first);
    });
}

//TODO: Assert message and then display to log
std::string Ayo::TextureType::textureTypeToString(TextureTypeEnum type) const
{
    assert(m_enumTypePairings.count(type) > 0,
    "One of the Texture Type Enum values that was passed to textureTypeToString "
    "does not have an appropriate pairing. Create a pairing for this value in the "
    "enumTypePairings map"
    );
    
    return m_enumTypePairings.at(type).name;
}

//TODO: Assert message and then output to log
Ayo::TextureType::TextureTypeEnum Ayo::TextureType::stringToTextureType(const std::string& typeStr) const
{
    auto findResult = std::find_if(m_enumTypePairings.begin(), m_enumTypePairings.end(), [&typeStr](const TextureTypeMappingPair& pair)
    {
        return pair.second.name == typeStr;
    });

    assert(findResult != m_enumTypePairings.end(),
    "One of the Texture Type strings that was passed to stringToTextureType "
    "does not have an appropriate pairing. Create a pairing for this value in the "
    "enumTypePairings map"
    );

    return findResult->first;
}

aiTextureType Ayo::TextureType::textureTypeToAssimpTextureType(TextureTypeEnum type) const
{
    assert(m_enumTypePairings.count(type) > 0,
        "One of the Texture Type Enum values that was passed to textureTypeToAssimpTextureType "
        "does not have an appropriate pairing. Create a pairing for this value in the "
        "enumTypePairings map"
    );

    return m_enumTypePairings.at(type).aiTexType;
}

Ayo::TextureType::TextureTypeEnum Ayo::TextureType::assimpEnumToTextureType(aiTextureType aiType) const
{
    auto findResult = std::find_if(m_enumTypePairings.begin(), m_enumTypePairings.end(), [&aiType](const TextureTypeMappingPair& pair)
    {
        return pair.second.aiTexType == aiType;
    });

    assert(findResult != m_enumTypePairings.end(),
        "One of the Texture Type strings that was passed to assimpEnumToTextureType "
        "does not have an appropriate pairing. Create a pairing for this value in the "
        "enumTypePairings map"
    );

    return findResult->first;
}
