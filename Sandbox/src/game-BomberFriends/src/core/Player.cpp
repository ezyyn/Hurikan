#include "Player.h"

Player::Player(OrthographicCameraController& controller) : m_Controller(controller)
{
	//m_PlayerTexture = SubTexture2D::CreateFromCoords(Texture2D::Create("src/game-BomberFriends/assets/textures/tileset_64x64.png"), { 5,1 }, { 64, 64 });
	HU_INFO("d");
	m_PlayerTexture = Texture2D::Create("src/game-BomberFriends/assets/textures/circle.png");
	m_PlayerBorder = { {0.0f, 0.0f},1.0f, 1.0f, 1.5f,1.5f,/*radius*/0.5 };
}

void Player::Draw()
{
	Renderer2D::DrawQuad({ m_PlayerBorder.x, m_PlayerBorder.y, 0.1 }, { m_PlayerBorder.radius * 2, m_PlayerBorder.radius * 2 }, m_PlayerTexture);
	//Renderer2D::DrawQuad({ 8, 17 - 15, 0.1 }, { m_PlayerBorder.radius * 2, m_PlayerBorder.radius * 2 }, m_PlayerTexture);
}

/*bool RectVsRect(const PlayerBorder& player, const BoxProperties& collider)
{
	return (player.x < collider.X + collider.Width && player.x + player.width > collider.X &&
			player.y < collider.Y + collider.Height && player.y + player.height > collider.Y);
}*/

float Clamp(float min, float max, float value)
{
	if (value < min)
	{
		return min;
	}
	else if (value > max)
	{
		return max;
	}
	else {
		return value;
	}
}

bool CircleVsRect(const PlayerTransform& player, const BoxProperties& collider)
{
	return false;
}
//bool collision = false;

KeyCode lastKey;
glm::vec2 potentialPos = { 1.0f, 1.0f };

void Player::Update(Timestep& deltaTime)
{
	m_PlayerBorder.velocity = { 0.0f, 0.0f };

	if (Input::IsKeyPressed(Key::A))
	{
		lastKey = Key::A;
	}
	else if (Input::IsKeyPressed(Key::D)) {
		potentialPos.x += m_PlayerStats.Effect.Speed * deltaTime;
		lastKey = Key::D;
	}
	else if (Input::IsKeyPressed(Key::W)) {
		potentialPos.y += m_PlayerStats.Effect.Speed * deltaTime;
		lastKey = Key::W;
	}
	else if (Input::IsKeyPressed(Key::S)) {
		potentialPos.y -= m_PlayerStats.Effect.Speed * deltaTime;
		lastKey = Key::S;
	}

	if (CircleVsRect(m_PlayerBorder, m_GridCollision[15][8])) { HU_INFO("Collision occured!") }
	
	/*
	for (uint32_t y = 0; y < 17; y++)
	{
		for (uint32_t x = 0; x < 17; x++)
		{
			if (CircleVsRect(m_PlayerBorder, m_GridCollision[y][x]) && m_GridCollision[y][x].BoxType == BoxTypeEnum::WALL)
			{
			//	HU_INFO(collision);
				collision = true;
			}
		}
	}
	*/
	
	/*if (Input::IsKeyPressed(Key::A))
	{
		potentialPos.x -= m_PlayerStats.Effect.Speed * deltaTime;
		lastKey = Key::A;
	}
	else if (Input::IsKeyPressed(Key::D)) {
		potentialPos.x += m_PlayerStats.Effect.Speed * deltaTime;
		lastKey = Key::D;
	}
	else if (Input::IsKeyPressed(Key::W)) {
		potentialPos.y += m_PlayerStats.Effect.Speed * deltaTime;
		lastKey = Key::W;
	}
	else if (Input::IsKeyPressed(Key::S)) {
		potentialPos.y -= m_PlayerStats.Effect.Speed * deltaTime;
		lastKey = Key::S;
	}*/

	
	//HU_INFO("{0} {1}", m_PlayerBorder.center_x, m_PlayerBorder.center_y);
}

