#include "SimpleUI.h"

#include "BomberMan/Core/LevelManager.h"
#include "BomberMan/Core/ResourceManager.h"
#include "BomberMan/Core/SaveManager.h"

#include <Hurikan/Core/Input.h>
#include <Hurikan/Core/KeyCodes.h>
#include <Hurikan/Core/Application.h>

void SimpleUI::Init()
{
	// Load textures
	Ref<Texture2D> heads_SpriteSheet = Texture2D::Create("assets/textures/player_animation/heads.png");
	m_IdleAnimation = SubTexture2D::CreateFromCoords(heads_SpriteSheet, { 4,0 }, { 13,11 });

	// Basic Init
	m_Camera = m_SceneUI.CreateEntity();

	const auto& [width, height] = Application::Get().GetWindowSize();
	m_Camera.AddComponent<CameraComponent>().Camera.SetViewportSize(width, height);

	auto& camera_cmp = m_Camera.GetComponent<CameraComponent>();
	auto& camera = camera_cmp.Camera;
	camera.SetOrthographicSize(12);
	camera.SetProjectionType(SceneCamera::ProjectionType::Orthographic);

	glm::vec2 panel_offset = { 0,1.05f };

	// Hearts
	for (float i = 0; i < /*g_Player->GetProperties().Health*/ 3; i++)
	{
		auto& e = m_SceneUI.CreateEntityWithDrawOrder(1);

		e.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f)).Texture = ResourceManager::GetTexture("Heart");

		e.Transform().Translation.x = -8.4f;
		e.Transform().Translation.y = 3.5f + panel_offset.y + i * 0.5f;

		e.Transform().Scale.x = 0.3f;
		e.Transform().Scale.y = 0.3f;
	}
	
	// Background
	{
		auto& e = m_SceneUI.CreateEntityWithDrawOrder(0);
		e.AddComponent<SpriteRendererComponent>(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
		auto& transform = e.Transform();

		transform.Translation.x = 0;
		transform.Translation.y = 4.0f + panel_offset.y;
		transform.Scale.x = 22.1f;
		transform.Scale.y = 2.1f;

		m_TopPanel = e;
	}


	// Animated head
	{
		m_Head = m_SceneUI.CreateEntityWithDrawOrder(1);
		m_Head.AddComponent<SpriteRendererComponent>(glm::vec4(1.0)).SubTexture = m_IdleAnimation;

		auto& transform = m_Head.Transform();

		transform.Translation.x = -6.8f;
		transform.Translation.y = 4.1f + panel_offset.y;
		transform.Scale.x *= 1.8f;
		transform.Scale.y *= 1.8f;

		m_HeadAnimator.SetTarget(m_Head);
		m_HeadAnimator.SetDebugTag("PlayerHead");
		m_HeadAnimator.Add(ResourceManager::GetAnimation("HeadUpAnimation"));
		m_HeadAnimator.Add(ResourceManager::GetAnimation("HeadDownAnimation"));
		m_HeadAnimator.Add(ResourceManager::GetAnimation("HeadLeftAnimation"));
	}

/*
	// Options - Dynamite, classic
	{
		auto& e = m_SceneUI.CreateEntityWithDrawOrder(1);
		e.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f)).SubTexture = ResourceManager::GetSubTexture("ClassicOpt");

		auto& transform = e.Transform();
		transform.Translation.x = -2.0f;
		transform.Translation.y = 3.5f + panel_offset.y;
		transform.Scale.x = 4.0f;
		transform.Scale.y = 0.5f;
	}*/
	// Text - Score
	{
		auto& e = m_SceneUI.CreateEntityWithDrawOrder(1);
		e.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f)).Texture = ResourceManager::GetTexture("ScoreText");

		auto& transform = e.Transform();
		transform.Translation.x = -2.2f;
		transform.Translation.y = 4.2f + panel_offset.y;
		transform.Scale.x = 2.25f;
		transform.Scale.y = 0.5f;
	}

	{// numbas

		for (int i = 0; i < 5; ++i)
		{
			auto& e = m_ScoreCounter.emplace_back(m_SceneUI.CreateEntityWithDrawOrder(1));

			e.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f)).SubTexture = ResourceManager::GetSubTexture("0");

			auto& transform = e.Transform();
			transform.Translation.x = -0.2f + i * 0.5f;
			transform.Translation.y = 4.2f + panel_offset.y;

			e.Transform().Scale.x = 0.5f;
			e.Transform().Scale.y = 0.5f;
		}
	}
	// Clock
	{
		auto& e = m_SceneUI.CreateEntityWithDrawOrder(1);
		e.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f)).SubTexture = ResourceManager::GetSubTexture("Clock");

		auto& transform = e.Transform();
		transform.Translation.x = 4.2f;
		transform.Translation.y = 4.2f + panel_offset.y;

		std::string string_timer = std::to_string(LevelManager::GetCurrentLevel().Time);

		string_timer.insert(0, 3 - string_timer.size(), '0');
		for (size_t i = 0; i < 3; ++i)
		{
			auto& pos = m_SceneUI.CreateEntityWithDrawOrder(1);
			pos.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f)).SubTexture = ResourceManager::GetSubTexture(std::string(1, string_timer[i]));

			auto& transform = pos.Transform();
			transform.Translation.x = 5.0f + i * 0.5f;
			transform.Translation.y = 4.2f + panel_offset.y;
			transform.Scale.x = 0.5f;
			transform.Scale.y = 0.5f;

			m_TimerCountDown.emplace_back(pos);
		}
	}
}

void SimpleUI::OnUpdate(Timestep& ts)
{
	//if(m_Effect != nullptr)
	//	m_Effect->OnUpdate(ts);

	if (SaveManager::Data().Score - m_ScoreOnScreen > 0)
	{
		OnUpdateScore(ts);
	}

	OnUpdateClock(ts);

	m_HeadAnimator.OnUpdate(ts);
	m_SceneUI.OnUpdateRuntime(ts);
}

void SimpleUI::Shutdown()
{
}

void SimpleUI::OnGameEvent(GameEvent& e)
{
	switch (e.Type)
	{
	case GameEventType::PLAYER_IDLE:
	{
		m_Head.Transform().Scale.x = glm::abs(m_Head.Transform().Scale.x);
		m_Head.GetComponent<SpriteRendererComponent>().SubTexture = m_IdleAnimation;
		m_HeadAnimator.Stop();
		break;
	}

	case GameEventType::PLAYER_DIR_UP:
	{
		m_Head.Transform().Scale.x = glm::abs(m_Head.Transform().Scale.x);
		m_HeadAnimator.Play("UpAnimation");
		break;
	}

	case GameEventType::PLAYER_DIR_DOWN:
	{
		m_Head.Transform().Scale.x = glm::abs(m_Head.Transform().Scale.x);
		m_HeadAnimator.Play("DownAnimation");
		break;
	}
	case GameEventType::PLAYER_DIR_LEFT:
	{
		m_Head.Transform().Scale.x = glm::abs(m_Head.Transform().Scale.x);
		m_HeadAnimator.Play("LeftAnimation");
		break;
	}
	case GameEventType::PLAYER_DIR_RIGHT:
	{
		m_Head.Transform().Scale.x *= -1;
		m_HeadAnimator.Play("LeftAnimation");
		break;
	}
	case GameEventType::BREAK_WALL:
	{
		auto& GRID_ENTITY = std::any_cast<Entity>(e.Data);
		
		break;
	}
	}
}

void SimpleUI::OnUpdateScore(Timestep& ts)
{
	static float timerino = 0.0f;

	timerino += ts;
	if (timerino >= 0.005f)
	{
		timerino = 0.0f;

		m_ScoreOnScreen += 2;

		std::string s = std::to_string(m_ScoreOnScreen);

		s.insert(0, m_ScoreCounter.size() - s.size(), '0');

		for (int i = 0; i < m_ScoreCounter.size(); ++i)
		{
			auto& e = m_ScoreCounter[i];

			e.GetComponent<SpriteRendererComponent>().SubTexture = ResourceManager::GetSubTexture(std::string(1, s[i]));
		}
	}
}

void SimpleUI::OnUpdateClock(Timestep& ts)
{
	static float timerino = 1.0f;

	timerino += ts;

	if (timerino >= 1.0f)
	{
		timerino = 0.0f;

		std::string string_timer = std::to_string(LevelManager::GetCurrentLevel().CurrentTime);

		string_timer.insert(0, m_TimerCountDown.size() - string_timer.size(), '0');
		//HU_INFO(string_timer);
		for (size_t i = 0; i < m_TimerCountDown.size(); ++i)
		{
			m_TimerCountDown[i].GetComponent<SpriteRendererComponent>().SubTexture = ResourceManager::GetSubTexture(std::string(1, string_timer[i]));
		}

		if (LevelManager::GetCurrentLevel().CurrentTime == 0)
		{
			// Level failed
		}
		LevelManager::GetCurrentLevel().CurrentTime--;
	}
}
