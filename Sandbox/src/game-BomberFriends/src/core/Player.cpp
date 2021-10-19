#include "Player.h"

#include "TextureLoader.h"

#include <thread>

void Player::Init(Ref<Scene> scene)
{
	m_BombAnimation.AddFrame(TextureLoader::Get()->Load("Bomb-Frame1"),0);
	m_BombAnimation.AddFrame(TextureLoader::Get()->Load("Bomb-Frame2"),0);
	m_BombAnimation.AddFrame(TextureLoader::Get()->Load("Bomb-Frame3"),0);
	m_BombAnimation.AddFrame(TextureLoader::Get()->Load("Bomb-Frame4"),0);
	m_BombAnimation.AddFrame(TextureLoader::Get()->Load("Bomb-Frame5"),0);
	m_BombAnimation.AddFrame(TextureLoader::Get()->Load("Bomb-Frame6"),0);
	m_BombAnimation.AddFrame(TextureLoader::Get()->Load("Bomb-Frame7"),0);
	m_BombAnimation.AddFrame(TextureLoader::Get()->Load("Bomb-Frame8"),0);
	m_BombAnimation.AddFrame(TextureLoader::Get()->Load("Bomb-Frame9"),0);
	m_BombAnimation.AddFrame(TextureLoader::Get()->Load("Bomb-Frame10"),0);
	m_BombAnimation.AddFrame(TextureLoader::Get()->Load("Bomb-Frame11"),0);
	m_BombAnimation.AddFrame(TextureLoader::Get()->Load("Bomb-Frame12"),0);
	m_BombAnimation.AddFrame(TextureLoader::Get()->Load("Bomb-Frame13"),0);
	m_BombAnimation.AddFrame(TextureLoader::Get()->Load("Bomb-Frame14"),0);
	m_BombAnimation.AddFrame(TextureLoader::Get()->Load("Bomb-Frame15"),0);
	m_BombAnimation.AddFrame(TextureLoader::Get()->Load("Bomb-Frame16"),0);
	m_BombAnimation.AddFrame(TextureLoader::Get()->Load("Bomb-Frame17"),0);
	m_BombAnimation.AddFrame(TextureLoader::Get()->Load("Bomb-Frame18"),0);

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

std::vector<Entity> bombs;

void Player::OnUpdate(Timestep& ts)
{
	// Bomb Placement
	auto& translation = m_PlayerEntity.GetComponent<TransformComponent>().Translation;

	if (B_pressed)
	{
		B_pressed = false; // One frame per type

		if (count == 0)
			return;

		for (auto bomb_entity : bombs)
		{
			if (glm::round(translation.x) == bomb_entity.GetComponent<TransformComponent>().Translation.x
				&& glm::round(translation.y) == bomb_entity.GetComponent<TransformComponent>().Translation.y)
			{
				// Cannot create another bomb entity if already exists on the position
				return;
			}
		}

		auto& e = m_Scene->CreateEntity("BombEntity");
		Bomb bomb(e, m_BombAnimation);
		bombs.push_back(e);
		e.GetComponent<TransformComponent>().Translation = { glm::round(translation.x), glm::round(translation.y), translation.z };
		count--;
	}

	for (size_t i = 0; i < bombs.size(); i++)
	{
		if (!bombs[i].GetComponent<AnimationComponent>().IsPlaying) 
		{
			count++;
			m_Scene->DestroyEntity(bombs[i]);
			bombs.erase(bombs.begin() + i);
		}
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

