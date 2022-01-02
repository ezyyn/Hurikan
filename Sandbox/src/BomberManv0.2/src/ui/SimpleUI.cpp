#include "SimpleUI.h"

#include "../sceneobjects/Player.h"
#include "../sceneobjects/Game.h"

extern Player* g_Player;

extern float camera_position_x;
extern float camera_position_y;

void SimpleUI::Init(const Level& level)
{
	m_CurrentLevel = level;

	// Load textures
	Ref<Texture2D> heads_SpriteSheet = Texture2D::Create("assets/textures/player_animation/heads.png");
	m_IdleAnimation = SubTexture2D::CreateFromCoords(heads_SpriteSheet, { 4,0 }, { 13,11 });

	// Basic Init
	m_Camera = m_SceneUI.CreateEntity();

	const auto& [width, height] = Application::Get().GetWindowSize();
	m_Camera.AddComponent<CameraComponent>().Camera.SetViewportSize(width, height);

	auto& camera_cmp = m_Camera.GetComponent<CameraComponent>();
	//camera_cmp.FixedAspectRatio = true;
	auto& camera = camera_cmp.Camera;
	camera.SetProjectionType(SceneCamera::ProjectionType::Orthographic);
	//camera.SetOrthographicSize(16.0f/9.0f);


	//auto& ee = m_SceneUI.CreateEntity();
	//ee.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f)).Texture;

	float offsetX = 15.5f;
	float offsetY = 6.5f;
	
	// Hearts
	for (float i = 0; i < g_Player->GetProperties().Health; i++)
	{
		auto& e = m_SceneUI.CreateEntityWithDrawOrder(1);

		e.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f)).Texture = Texture2D::Create("assets/textures/heart.png");

		e.Transform().Translation.x = -8.4f;
		e.Transform().Translation.y = 3.5f + i * 0.5f;

		e.Transform().Scale.x = 0.4f;
		e.Transform().Scale.y = 0.4f;
	}
	
	// Background
	{
		auto& e = m_SceneUI.CreateEntityWithDrawOrder(0);
		e.AddComponent<SpriteRendererComponent>(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
		auto& transform = e.Transform();

		transform.Translation.x = 0;
		transform.Translation.y = 4;
		transform.Scale.x = 18;
		transform.Scale.y = 2;

		m_TopPanel = e;
	}
	// Animated head
	{
		head = m_SceneUI.CreateEntityWithDrawOrder(1);
		head.AddComponent<SpriteRendererComponent>(glm::vec4(1.0)).SubTexture = m_IdleAnimation;

		auto& transform = head.Transform();

		transform.Translation.x = -6.8f;
		transform.Translation.y = 4.2f;
		transform.Scale.x *= 2.1f;
		transform.Scale.y *= 2.1f;

		m_HeadAnimator.SetTarget(head);
		m_HeadAnimator.Add(ResourceManager::RequestAnimation("HeadUpAnimation"));
		m_HeadAnimator.Add(ResourceManager::RequestAnimation("HeadDownAnimation"));
		m_HeadAnimator.Add(ResourceManager::RequestAnimation("HeadLeftAnimation"));
	}

	{
		auto& e = m_SceneUI.CreateEntityWithDrawOrder(1);
		e.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f)).SubTexture = ResourceManager::RequestSubTexture("ClassicOpt");

		auto& transform = e.Transform();
		transform.Translation.x = -2.0f;
		transform.Translation.y = 3.5f;
		transform.Scale.x = 4.0f;
		transform.Scale.y = 0.5f;
	}
	{
		auto& e = m_SceneUI.CreateEntityWithDrawOrder(1);
		e.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f)).Texture = ResourceManager::RequestTexture("EquippedText");

		auto& transform = e.Transform();
		transform.Translation.x = -2.2f;
		transform.Translation.y = 4.2f;
		transform.Scale.x = 2.25f;
		transform.Scale.y = 0.5f;
	}

	{
		auto& e = m_SceneUI.CreateEntityWithDrawOrder(1);
		e.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f)).SubTexture = ResourceManager::RequestSubTexture("Clock");

		auto& transform = e.Transform();
		transform.Translation.x = 4.2f;
		transform.Translation.y = 4.2f;
		//int timer = 200;
	}

	//string_timer.reserve();

	for (size_t i = 0; i < 3; i++)
	{
		auto& pos = m_SceneUI.CreateEntityWithDrawOrder(1);
		pos.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f)).SubTexture = ResourceManager::RequestSubTexture("0");

		auto& transform = pos.Transform();
		transform.Translation.x = 5.0f + i * 0.4f;
		transform.Translation.y = 4.2f;
		transform.Scale.x = 0.5f;
		transform.Scale.y = 0.5f;

		m_TimerCountDown.push_back(pos);
	}

}

float timerino = 0.0f;
int countdown = 102;

void SimpleUI::OnUpdate(Timestep ts)
{
	timerino += ts;

	if (timerino >= 1)
	{
		timerino = 0.0f;

		std::string string_timer = std::to_string(countdown);

		for (int i = 0; i < 3 - (string_timer.size()); i++)
		{
			string_timer.insert(0, "0");
		}
		HU_INFO(string_timer);
		for (size_t i = 0; i < m_TimerCountDown.size(); i++)
		{
			m_TimerCountDown[i].GetComponent<SpriteRendererComponent>().SubTexture = ResourceManager::RequestSubTexture(std::string(1, string_timer[i]));
		}

		if (countdown == 0)
		{
			// Do something
		}
		countdown--;
	}

	if (m_LastState != g_Player->GetAnimationState())
	{
		m_LastState = g_Player->GetAnimationState();
		switch (g_Player->GetAnimationState())
		{
		case PlayerAnimationState::IDLE:
		{
			head.Transform().Scale.x = glm::abs(head.Transform().Scale.x);
			head.GetComponent<SpriteRendererComponent>().SubTexture = m_IdleAnimation;
			m_HeadAnimator.Stop();
			break;
		}
		case PlayerAnimationState::LEFT:
		{
			head.Transform().Scale.x = glm::abs(head.Transform().Scale.x);
			m_HeadAnimator.Play("LeftAnimation");
			break;
		}
		case PlayerAnimationState::UP:
		{
			head.Transform().Scale.x = glm::abs(head.Transform().Scale.x);
			m_HeadAnimator.Play("UpAnimation");
			break;
		}
		case PlayerAnimationState::RIGHT:
		{
			head.Transform().Scale.x *= -1;
			m_HeadAnimator.Play("LeftAnimation");
			break;
		}
		case PlayerAnimationState::DOWN:
		{
			head.Transform().Scale.x = glm::abs(head.Transform().Scale.x);
			m_HeadAnimator.Play("DownAnimation");
			break;
		}
		case PlayerAnimationState::DEAD:
			//m_HeadAnimator.Play("LeftAnimation");
			break;
		default:
			break;
		}
	}
	m_HeadAnimator.OnUpdate(ts);

	m_SceneUI.OnUpdateRuntime(ts);
}

void SimpleUI::Shutdown()
{

}
