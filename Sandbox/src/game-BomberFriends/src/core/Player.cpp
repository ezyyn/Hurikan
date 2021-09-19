#include "Player.h"

Player::Player(OrthographicCameraController& controller) : m_Controller(controller)
{
	m_PlayerTexture = SubTexture2D::CreateFromCoords(Texture2D::Create("src/game-BomberFriends/assets/textures/tileset_64x64.png"), { 5,1 }, { 64, 64 });
	m_PlayerBorder = { 1.0f, 1.0f, 1.5f,1.5f,/*radius*/0.5};
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

bool CircleVsRect(const PlayerBorder& player, const BoxProperties& collider)
{
	double line_length = glm::sqrt(glm::pow(collider.X - player.center_x, 2) + glm::pow(collider.Y - player.center_y, 2));
	HU_INFO("{0}", line_length);
	if (line_length < player.radius)
		return true;
	return false;
}
//bool collision = false;

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


KeyCode lastKey;
glm::vec2 potentialPos = { 1.0f, 1.0f };

void Player::Update(Timestep& deltaTime)
{
	bool validMove = true;

	if (CircleVsRect(m_PlayerBorder, m_GridCollision[15][8])) { validMove = false; }
	
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
	
	if (Input::IsKeyPressed(Key::A))
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
	}

	if (validMove)
	{
		m_PlayerBorder.x = potentialPos.x;
		m_PlayerBorder.y = potentialPos.y;
		m_PlayerBorder.center_x = potentialPos.x + m_PlayerBorder.radius;
		m_PlayerBorder.center_y = potentialPos.y + m_PlayerBorder.radius;
	}
	HU_INFO("{0} {1}", m_PlayerBorder.center_x, m_PlayerBorder.center_y);
}

