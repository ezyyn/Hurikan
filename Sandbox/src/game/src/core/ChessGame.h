#pragma once
#include "Hurikan.h"

using namespace Hurikan;

enum ChessPieceEnum
{
	NIC = 0, PESAK = 1, VEZ = 2, STRELEC = 3, KUN = 4, KRALOVNA = 5, KRAL = 6
};

struct ChessPiece
{
	glm::vec2 position;
	ChessPieceEnum piece;
};

class ChessGame
{
public:
	ChessGame();
	void OnUpdate(Timestep ts);
	void OnEvent(Event& e);
	bool OnMouseMoved(MouseMovedEvent& e);
private:
	std::array<ChessPiece, 64> m_ChessField;
	Ref<Texture2D> m_TexturePesak;
	Ref<Texture2D> m_TextureVez;
	Ref<Texture2D> m_TextureStrelec;
	Ref<Texture2D> m_TextureKralovna;
	Ref<Texture2D> m_TextureKral;
	Ref<Texture2D> m_TextureKun;

	OrthographicCameraController m_CameraController;
};