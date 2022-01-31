#pragma once

#include "BomberMan/Core/ResourceManager.h"

#include <Hurikan/Core/Log.h>
#include <Hurikan/Scene/Entity.h>

using namespace Hurikan;

enum class FXType
{
	SCORE_100 = 0,
};

class Effect
{
public:
	Effect(Scene* scene, FXType type, float time, const glm::vec3& pos)
		: CurrentScene(scene), Type(type), Time(time), Timer(0.0f), Position(pos)
	{
	}
protected:
	virtual bool OnUpdate(Timestep& ts) 
	{
		Timer += ts;
		if (Timer >= Time / 1000.0f)
		{
			for (auto& entity : Entities)
			{
				CurrentScene->DestroyEntity(entity);
			}

			return true;
		}
		return false;
	}

protected:
	Scene* CurrentScene;
	std::vector<Entity> Entities;

	float Timer;
	glm::vec3 Position;
	FXType Type;
	float Time;

	friend class FXManager;
};


class Score100Effect : public Effect
{
public:
	Score100Effect(Scene* scene, FXType type, float time, const glm::vec3& pos) 
		: Effect(scene, type, time, pos) 
	{
		std::string hundred = "100";

		for (int i = 0; i < hundred.size(); ++i)
		{
			auto& e = Entities.emplace_back(CurrentScene->CreateEntityWithDrawOrder(3));

			e.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f)).SubTexture = ResourceManager::GetSubTexture(hundred[i]);

			e.Transform().Translation.x = Position.x - 0.1f + (i * 0.2f);
			e.Transform().Translation.y = Position.y;

			e.Transform().Scale.x = 0.2f;
			e.Transform().Scale.y = 0.2f;
		}
		Entities[0].Transform().Translation.x += 0.04f;
	}

	float timer = 0.0f;

	inline bool OnUpdate(Timestep& ts) override
	{
		timer += ts;

		if (timer >= 0.1f)
		{
			timer = 0.0f;

			float r3 = -0.3f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (0.3f - (-0.3f))));
			for (auto& e : Entities)
			{
				e.Transform().Translation.y += 0.03f;
				e.Transform().Translation.x += r3 * ts;
				e.GetComponent<SpriteRendererComponent>().Color -= ts;
			}
		}

		return Effect::OnUpdate(ts);
	}

};
