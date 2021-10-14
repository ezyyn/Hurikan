#include "Player.h"

void Player::Init(/* [GRIDDATA], */Entity& entity)
{
	m_PlayerEntity = entity;

	m_PlayerTexture = Texture2D::Create("src/game-BomberFriends/assets/textures/c.png");

	m_PlayerEntity.AddComponent<SpriteRendererComponent>(glm::vec4{1.0f, 1.0f, 1.0f, 1.0f}).Texture = m_PlayerTexture;
	m_PlayerEntity.AddComponent<NativeScriptComponent>().Bind<PlayerController>();
}
