#include "Player.h"

Player::Player(OrthographicCameraController& controller) : m_Controller(controller)
{
}

void Player::Draw()
{
	Renderer2D::DrawQuad({ 1,1, 0.1 }, { 1,1 }, { 0,1,0,1 });
}

void Player::Update(Timestep ts)
{
}

