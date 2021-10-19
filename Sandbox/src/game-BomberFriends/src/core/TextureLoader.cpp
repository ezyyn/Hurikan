#include "TextureLoader.h"

void TextureLoader::Init()
{
	if (TextureLoader::s_Instance == nullptr)
		TextureLoader::s_Instance = new TextureLoader();
	else
		HU_CORE_ASSERT(false, "");
}

TextureLoader* TextureLoader::Get()
{
	return s_Instance;
}

Hurikan::Ref<Hurikan::Texture2D> TextureLoader::Load(const std::string name)
{
	return m_Map[name];
}

TextureLoader* TextureLoader::s_Instance;
