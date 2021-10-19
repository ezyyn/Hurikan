#include "Player.h"

#include "TextureLoader.h"

#include <thread>

void Player::Init(Ref<Scene> scene)
{
	m_BombAnimation.AddFrame(TextureLoader::Get()->Load("Bomb-Frame1"), 10);
	m_BombAnimation.AddFrame(TextureLoader::Get()->Load("Bomb-Frame2"), 10);
	m_BombAnimation.AddFrame(TextureLoader::Get()->Load("Bomb-Frame3"), 10);
	m_BombAnimation.AddFrame(TextureLoader::Get()->Load("Bomb-Frame4"), 10);
	m_BombAnimation.AddFrame(TextureLoader::Get()->Load("Bomb-Frame5"), 10);
	m_BombAnimation.AddFrame(TextureLoader::Get()->Load("Bomb-Frame6"), 10);
	m_BombAnimation.AddFrame(TextureLoader::Get()->Load("Bomb-Frame7"), 10);
	m_BombAnimation.AddFrame(TextureLoader::Get()->Load("Bomb-Frame8"), 10);
	m_BombAnimation.AddFrame(TextureLoader::Get()->Load("Bomb-Frame9"), 10);
	m_BombAnimation.AddFrame(TextureLoader::Get()->Load("Bomb-Frame10"), 10);
	m_BombAnimation.AddFrame(TextureLoader::Get()->Load("Bomb-Frame11"), 10);
	m_BombAnimation.AddFrame(TextureLoader::Get()->Load("Bomb-Frame12"), 10);
	m_BombAnimation.AddFrame(TextureLoader::Get()->Load("Bomb-Frame13"), 10);
	m_BombAnimation.AddFrame(TextureLoader::Get()->Load("Bomb-Frame14"), 10);
	m_BombAnimation.AddFrame(TextureLoader::Get()->Load("Bomb-Frame15"), 10);
	m_BombAnimation.AddFrame(TextureLoader::Get()->Load("Bomb-Frame16"), 10);
	m_BombAnimation.AddFrame(TextureLoader::Get()->Load("Bomb-Frame17"), 10);
	m_BombAnimation.AddFrame(TextureLoader::Get()->Load("Bomb-Frame18"), 10);

	// Scene
	m_Scene = scene;
	m_PlayerEntity = m_Scene->CreateEntity("PlayerEntity");

	m_PlayerEntity.AddComponent<SpriteRendererComponent>(glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f }).Texture = TextureLoader::Get()->Load("Player");
	auto& rb = m_PlayerEntity.AddComponent<Rigidbody2DComponent>();
	rb.Type = Rigidbody2DComponent::BodyType::Dynamic;
	rb.Gravity = false;
	rb.FixedRotation = true;

	float scale = 0.96f;
	m_PlayerEntity.GetComponent<TransformComponent>().Scale.x = scale;
	m_PlayerEntity.GetComponent<TransformComponent>().Scale.y = scale;

	auto& start_position_entity = m_Scene->GetEntityByTag("PlayerStartingPositionEntity");
	start_position_entity.GetComponent<TagComponent>().Tag = "Empty";
	m_PlayerEntity.GetComponent<TransformComponent>().Translation = start_position_entity.GetComponent<TransformComponent>().Translation;

	m_PlayerEntity.AddComponent<CircleCollider2DComponent>().Radius = scale / 2.0f;
	m_PlayerEntity.GetComponent<CircleCollider2DComponent>().Friction = 0.0f;

	m_PlayerEntity.AddComponent<NativeScriptComponent>().Bind<PlayerController>();

}

const int maxCount = 5;
bool B_pressed = false;

void Player::OnUpdate(Timestep& ts)
{
	// Bomb Placement
	auto& translation = m_PlayerEntity.GetComponent<TransformComponent>().Translation;

	if (B_pressed)
	{
		B_pressed = false; // One frame per type

		HU_INFO("BombEntity" + std::to_string(count));
		auto& e = m_Scene->CreateEntity("BombEntity");
		Bomb bomb(e, m_BombAnimation);
		e.GetComponent<TransformComponent>().Translation = { glm::round(translation.x), glm::round(translation.y), translation.z };
		count--;
	}
}

bool d = true;

bool Player::OnKeyPressed(KeyPressedEvent& e)
{
	switch (e.GetKeyCode())
	{
		case Key::B:
		{
			if (d)
			{
				d = false;
				B_pressed = true;
				break;
			}
		}
	}
	return false;
}

bool Player::OnKeyReleased(KeyReleasedEvent& e)
{
	switch (e.GetKeyCode())
	{
		case Key::B:
		{
			B_pressed = false;
			d = true;
			break;
		}
	}
	return false;
}

