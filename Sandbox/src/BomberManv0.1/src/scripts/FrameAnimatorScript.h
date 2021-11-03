#pragma once

#include <Hurikan.h>
using namespace Hurikan;

struct BlockAnimation;

class FrameAnimator : public ScriptableEntity
{
public:
	void Add(const BlockAnimation& ba);
	void Switch(const std::string& tag);

	BlockAnimation& GetActiveAnimation();
	BlockAnimation& FrameAnimator::GetAnimationByTag(const std::string& tag);
protected:
	void OnCreate() override;
	void OnDestroy() override;
	void OnUpdate(Timestep ts) override;
private:
	Ref<Texture2D> m_SpriteSheet;
	glm::vec2 m_SingleSpriteSize;
};