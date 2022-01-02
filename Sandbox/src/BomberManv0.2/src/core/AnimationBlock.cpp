#include "AnimationBlock.h"

void AnimationBlock::Load(const std::vector<FrameSpecification>& frameSpec)
{
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

		Animation.push_back(frameData);
	}
}

void AnimationBlock::OnUpdate(SpriteRendererComponent* src, Timestep ts)
{
	if (!Active)
	{
		HU_INFO("????????");
		return;
	}

	if (Animation.empty() && Index >= Animation.size() - 1)
	{
		src->SubTexture = nullptr;
		src->Color = glm::vec4(0.0f);
		return;
	}

	if (Index == 0)
	{
		src->SubTexture = Animation[0].Subtexture;
		src->Color = Animation[0].Color;
	}

	CurrentFrameTime += ts;

	if (CurrentFrameTime >= Animation[Index].Delay / 1000) // <- definetely change
	{
		CurrentFrameTime = 0.0f;
		Index = (Index + 1) % Animation.size();
		src->SubTexture = Animation[Index].Subtexture;
		src->Color = Animation[Index].Color;

		if (NextFrameStop)
		{
			Active = false;
			return;
		}
	}
	
	if (!NextFrameStop && Index == Animation.size() - 1 && Repeat == false)
	{
		HU_INFO("ASD");
		src->SubTexture = Animation[Index].Subtexture;
		src->Color = Animation[Index].Color;
		NextFrameStop = true;
	}
}
