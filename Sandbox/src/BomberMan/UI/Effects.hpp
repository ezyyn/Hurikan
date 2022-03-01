#pragma once

#include "BomberMan/Core/ResourceManager.hpp"
#include "BomberMan/Core/Utils.hpp"

#include <Hurikan/Core/Log.h>
#include <Hurikan/Scene/Entity.h>

using namespace Hurikan;

class Effect
{
public:
	Effect(Scene* scene, float time, const glm::vec3& pos)
		: m_CurrentScene(scene), m_Time(time), m_Timer(0.0f), m_Position(pos)
	{
	}
	
	virtual ~Effect() {};

	inline bool OnUpdateInternal(Timestep& ts)
	{
		OnUpdate(ts);

		m_Timer += ts;
		if (m_Timer >= m_Time / 1000.0f)
		{
			for (auto& entity : m_Entities)
			{
				m_CurrentScene->DestroyEntity(entity);
			}

			return true;
		}
		return false;
	}
protected:
	virtual void OnUpdate(Timestep& ts) = 0;
protected:
	Scene* m_CurrentScene;
	std::vector<Entity> m_Entities;

	glm::vec3 m_Position;
private:
	float m_Time, m_Timer;

	friend class FXManager;
};


class Score100Effect : public Effect
{
public:
	Score100Effect(Scene* scene, float time, const glm::vec3& pos) 
		: Effect(scene, time, pos) 
	{
		std::string hundred = "100";

		for (int i = 0; i < hundred.size(); ++i)
		{
			auto& e = m_Entities.emplace_back(m_CurrentScene->CreateEntityWithDrawOrder(3));

			e.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f)).SubTexture = ResourceManager::GetSubTexture(hundred[i]);

			e.Transform().Translation.x = m_Position.x - 0.1f + (i * 0.2f);
			e.Transform().Translation.y = m_Position.y;

			e.Transform().Scale.x = 0.2f;
			e.Transform().Scale.y = 0.2f;
		}
		m_Entities[0].Transform().Translation.x += 0.04f;
	}

	float timer = 0.0f;

	void OnUpdate(Timestep& ts) override
	{
		timer += ts;

		if (timer >= 0.1f)
		{
			timer = 0.0f;

			float r = Utils::Random(-0.3f, 0.3f);
			for (auto& e : m_Entities)
			{
				e.Transform().Translation.y += 0.03f;
				e.Transform().Translation.x += r * ts;
				e.GetComponent<SpriteRendererComponent>().Color -= ts;
			}
		}
	}

};

class Score300Effect : public Effect
{
public:
	Score300Effect(Scene* scene, float time, const glm::vec3& pos)
		: Effect(scene, time, pos)
	{
		std::string thundred = "300";

		for (int i = 0; i < thundred.size(); ++i)
		{
			auto& e = m_Entities.emplace_back(m_CurrentScene->CreateEntityWithDrawOrder(3));

			e.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f)).SubTexture = ResourceManager::GetSubTexture(thundred[i]);

			e.Transform().Translation.x = m_Position.x - 0.1f + (i * 0.2f);
			e.Transform().Translation.y = m_Position.y;

			e.Transform().Scale.x = 0.2f;
			e.Transform().Scale.y = 0.2f;
		}
		m_Entities[0].Transform().Translation.x += 0.04f;
	}

	float timer = 0.0f;

	void OnUpdate(Timestep& ts) override
	{
		timer += ts;

		if (timer >= 0.1f)
		{
			timer = 0.0f;

			float r = Utils::Random(-0.4f, 0.4f);
			for (auto& e : m_Entities)
			{
				e.Transform().Translation.y += 0.03f;
				e.Transform().Translation.x += r * ts;
				e.GetComponent<SpriteRendererComponent>().Color -= ts;
			}
		}
	}
};

class Score500Effect : public Effect
{
public:
	Score500Effect(Scene* scene, float time, const glm::vec3& pos)
		: Effect(scene, time, pos)
	{
		std::string thundred = "500";

		for (int i = 0; i < thundred.size(); ++i)
		{
			auto& e = m_Entities.emplace_back(m_CurrentScene->CreateEntityWithDrawOrder(3));

			e.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f)).SubTexture = ResourceManager::GetSubTexture(thundred[i]);

			e.Transform().Translation.x = m_Position.x - 0.1f + (i * 0.2f);
			e.Transform().Translation.y = m_Position.y;

			e.Transform().Scale.x = 0.2f;
			e.Transform().Scale.y = 0.2f;
		}
		m_Entities[0].Transform().Translation.x += 0.04f;
	}

	float timer = 0.0f;

	void OnUpdate(Timestep& ts) override
	{
		timer += ts;

		if (timer >= 0.1f)
		{
			timer = 0.0f;

			float r = Utils::Random(-0.6f, 0.6f);
			for (auto& e : m_Entities)
			{
				e.Transform().Translation.y += 0.03f;
				e.Transform().Translation.x += r * ts;
				e.GetComponent<SpriteRendererComponent>().Color -= ts;
			}
		}
	}
};