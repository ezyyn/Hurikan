#include "AnimationBlock.h"

void AnimationBlock::Load(const std::vector<FrameSpecification>& frameSpec)
{
	Animation.reserve(frameSpec.size());
	for (size_t i = 0; i < frameSpec.size(); i++)
	{
		FrameData frameData;
		if (frameSpec[i].Blank)
		{
			frameData.Subtexture = nullptr;
			frameData.Color = glm::vec4(0.0f);
		}
		else 
			frameData.Subtexture = SubTexture2D::CreateFromCoords(SpriteSheet, frameSpec[i].Position, SpriteSize);

		frameData.Delay = frameSpec[i].Delay;

		Animation.emplace_back(frameData);
	}
}

void AnimationBlock::OnUpdate(SpriteRendererComponent& src, Timestep ts)
{
	if (FirstFrame)
	{
		FirstFrame = false;
		src.SubTexture = Animation[Index].Subtexture;
		src.Color = Animation[Index].Color;
	}

	if (Animation.empty())
		return;

	if (!Active)
		return;

	CurrentFrameTime += ts;

	if (CurrentFrameTime >= Animation[Index].Delay / 1000) // <- definetely change
	{
		CurrentFrameTime = 0.0f;
		if (NextFrameStop)
		{
			NextFrameStop = false;
			Index = 0;
		}
		else
			Index++;

		//Index = (Index + 1) % Animation.size();
		src.SubTexture = Animation[Index].Subtexture;
		src.Color = Animation[Index].Color;
	}

	if (Index == Animation.size() - 1)
	{
		if (Repeat)
		{
			NextFrameStop = true;
		}
		else
		{
			Index = 0;
			Active = false;
			return;
		}
	}
}
