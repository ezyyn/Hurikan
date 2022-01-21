#pragma once

#include "BomberMan/Core/Animator.h"

#include <unordered_map>
#include <Hurikan/Renderer/SubTexture2D.h>
using namespace Hurikan;

class ResourceManager
{
public:
	static inline void Init() { s_Instance.Init_Impl(); }
	static inline const Ref<SubTexture2D>& GetSubTexture(const std::string& tag) { return s_Instance.m_SubTextureMap[tag]; }
	static inline const Ref<Texture2D>& GetTexture(const std::string& tag) { return s_Instance.m_TextureMap[tag]; }
	static inline const Animation& GetAnimation(const std::string& tag) { return s_Instance.m_AnimationMap[tag]; }
private:
	void Init_Impl();
private:
	ResourceManager() = default;

	std::unordered_map<std::string, Ref<SubTexture2D>> m_SubTextureMap;
	std::unordered_map<std::string, Ref<Texture2D>> m_TextureMap;
	std::unordered_map<std::string, Animation> m_AnimationMap;

	static ResourceManager s_Instance;
};