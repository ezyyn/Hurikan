#pragma once

#include "../animations/FrameAnimator.h"

#include <Hurikan.h>
using namespace Hurikan;

class ResourceManager
{
public:
	~ResourceManager() = default;

	inline static void Init(std::string&& filepath) { s_Instance.InitImpl(std::forward<std::string>(filepath)); }
	inline static const Ref<Texture2D>& RequestTexture(std::string&& tag) { return s_Instance.RequestTextureImpl(std::forward<std::string>(tag)); }
	inline static const Ref<SubTexture2D>& RequestSubTexture(const std::string& tag) { return s_Instance.RequestSubTextureImpl(tag); }
	inline static const Ref<SubTexture2D>& RequestNumberSubTexture(int number) { return s_Instance.RequestSubTextureImpl(number); }
	inline static const AnimationDetails& RequestAnimation(std::string&& tag) { return s_Instance.RequestAnimationImpl(std::forward<std::string>(tag)); }
private:
	ResourceManager() = default;
	ResourceManager(const ResourceManager&) = default;

	void InitImpl(std::string&& filepath);

	const Ref<Texture2D>& RequestTextureImpl(std::string&& tag);
	const Ref<SubTexture2D>& RequestSubTextureImpl(const std::string& tag);
	const Ref<SubTexture2D>& RequestSubTextureImpl(int number);
	const AnimationDetails& RequestAnimationImpl(std::string&& tag);
	std::string m_Filepath = "assets/textures/";
	
	std::unordered_map<std::string, Ref<Texture2D>> m_TextureMap;
	std::unordered_map<std::string, Ref<SubTexture2D>> m_SubTextureMap;
	std::unordered_map<std::string, AnimationDetails> m_AnimationMap;
	std::array<Ref<SubTexture2D>, 10> m_NumbersMap;

	static ResourceManager s_Instance;
};
