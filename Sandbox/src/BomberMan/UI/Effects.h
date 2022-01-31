#pragma once

#include "BomberMan/Core/ResourceManager.h"

#include <Hurikan/Core/Log.h>
#include <Hurikan/Scene/Entity.h>

#include <Hurikan/Scene/Scene.h>
using namespace Hurikan;

enum class FXType
{
	NONE = -1,
	SCORE_100 = 0,
};

class Effect
{
public:
	Effect(Scene* scene = nullptr, FXType type = FXType::NONE, float time = 0, const glm::vec3& initial_pos = { 0.0f, 0.0f, 0.0f })
		: CurrentScene(scene), Type(type), Time(time), Position(initial_pos), Timer(0.0f)
	{
	}

	virtual bool OnUpdate(Timestep& ts) 
	{
		if (!CurrentScene)
			return false;

		Timer += ts;
		if (Timer >= Time)
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
};


class Score100Effect : public Effect
{
public:
	Score100Effect(Scene* scene, FXType type, float time, const glm::vec3& initial_pos) 
		: Effect(scene, type, time, initial_pos) 
	{
		std::string hundred = "100";

		for (int i = 0; i < hundred.size(); ++i)
		{
			auto& e = Entities.emplace_back(CurrentScene->CreateEntityWithDrawOrder(3));

			e.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f)).SubTexture = ResourceManager::GetSubTexture(hundred[i]);

			e.Transform().Translation.x = Position.x + i * 0.1f;
			e.Transform().Translation.y = Position.y;

			e.Transform().Scale *= 0.2f;
		}
	}

	inline bool OnUpdate(Timestep& ts) override
	{
		return Effect::OnUpdate(ts);
	}

};
