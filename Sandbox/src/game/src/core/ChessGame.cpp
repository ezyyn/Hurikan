#include "ChessGame.h"

ChessGame::ChessGame() : m_CameraController(1280.0f / 720.0f, false)
{	
	m_TexturePesak = Texture2D::Create("src/game/assets/textures/pesak.png");
	m_TextureKun = Texture2D::Create("src/game/assets/textures/kun.png");
	m_TextureKral = Texture2D::Create("src/game/assets/textures/kral.png");
    m_TextureKralovna = Texture2D::Create("src/game/assets/textures/kralovna.png");
	m_TextureStrelec = Texture2D::Create("src/game/assets/textures/strelec.png");
	m_TextureVez = Texture2D::Create("src/game/assets/textures/vez.png");

	ChessPieceEnum layout[8][8] =
	{
		{VEZ,KUN,STRELEC,KRAL,KRALOVNA,STRELEC,KUN,VEZ},
		{PESAK,PESAK,PESAK,PESAK,PESAK,PESAK,PESAK,PESAK},
		{NIC, NIC, NIC, NIC, NIC, NIC, NIC, NIC},
		{NIC, NIC, NIC, NIC, NIC, NIC, NIC, NIC},
		{NIC, NIC, NIC, NIC, NIC, NIC, NIC, NIC},
		{NIC, NIC, NIC, NIC, NIC, NIC, NIC, NIC},
		{PESAK,PESAK,PESAK,PESAK,PESAK,PESAK,PESAK,PESAK},
		{VEZ,KUN,STRELEC,KRAL,KRALOVNA,STRELEC,KUN,VEZ},
	};

	uint8_t i = 0;
	for (uint8_t y = 0; y < 8; y++)
	{
		for (uint8_t x = 0; x < 8; x++)
		{
			m_ChessField[i] = { {x,y}, layout[y][x] };
			i++;
		}
	}
	for (uint8_t i = 0; i < m_ChessField.size(); i++)
	{
		HU_CORE_TRACE("x: {0}, y: {1}", m_ChessField[i].position.x, m_ChessField[i].position.y);
	}

}
#define CHECKERBOARD
#define FIGURES
uint32_t cursorPosX = 0, cursorPosY = 0;
bool pressed = false;
bool released = false;

void ChessGame::OnUpdate(Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	Renderer2D::BeginScene(m_CameraController.GetCamera());
		
	HU_CORE_TRACE("{0} {1}", m_CameraController.GetCamera().GetPosition().x, m_CameraController.GetCamera().GetPosition().y);

	Renderer2D::DrawQuad({ 0.0f,0.0f,0.0f }, { 1.0f,1.0f }, { 8.0f,0.3f, 0.5f, 1.0f });
	Renderer2D::DrawQuad({ -1.0f,0.0f,0.0f }, { 1.0f,1.0f }, { 0.1f,0.7f, 0.2f, 1.0f });
	Renderer2D::DrawQuad({ 0.0f,-1.0f,0.0f }, { 1.0f,1.0f }, { 0.3f,0.5f, 0.8f, 1.0f });
	if (pressed)
	{
		HU_CORE_TRACE("x: {0} y: {1}", cursorPosX, cursorPosY);
		int i = 0;
		for (uint8_t y = 0; y < 8; y++)
		{
			for (uint8_t x = 0; x < 8; x++)
			{
				ChessPieceEnum piece = m_ChessField[i].piece;
				float piece_x = m_ChessField[i].position.x;
				float piece_y = m_ChessField[i].position.y;

				if (cursorPosX >= piece_x && cursorPosY >= piece_y && cursorPosX < piece_x + 1 && cursorPosY < piece_y + 1)
				{
					HU_CORE_TRACE(piece);
				}
				i++;
			}
		}
	}

	Renderer2D::EndScene();

	return;

#ifdef CHECKERBOARD
	uint8_t i = 0;
	for (uint8_t y = 0; y < 8; y++)
	{
		for (uint8_t x = 0; x < 8; x++)
		{
			if (y % 2 == 0) {
				if (x % 2 == 0)
				{
					Renderer2D::DrawQuad(
						{ m_ChessField[i].position.x, m_ChessField[i].position.y, 0.0f },
						{ 1.0f,1.0f },
						{ 0.0f,0.0f, 0.0f, 1.0f }
					);
				}
				else {
					Renderer2D::DrawQuad(
						{ m_ChessField[i].position.x, m_ChessField[i].position.y, 0.0f },
						{ 1.0f,1.0f },
						{ 1.0f,1.0f, 1.0f, 1.0f }
					);
				}
			}
			else {
				if (x % 2 == 0)
				{
					Renderer2D::DrawQuad(
						{ m_ChessField[i].position.x, m_ChessField[i].position.y, 0.0f },
						{ 1.0f,1.0f },
						{ 1.0f,1.0f, 1.0f, 1.0f }
					);
				}
				else {
					Renderer2D::DrawQuad(
						{ m_ChessField[i].position.x, m_ChessField[i].position.y, 0.0f },
						{ 1.0f,1.0f },
						{ 0.0f,0.0f, 0.0f, 1.0f }
					);
				}
			}
			i++;
		}
	}
#endif


#ifdef FIGURES
	uint8_t j = 0;
	for (uint8_t y = 0; y < 8; y++)
	{
		for (uint8_t x = 0; x < 8; x++)
		{
			ChessPieceEnum piece = m_ChessField[j].piece;
			switch (piece)
			{
			case NIC:
				break;
			case PESAK:
				Renderer2D::DrawQuad(
					{ m_ChessField[j].position.x, m_ChessField[j].position.y, 1.0f },
					{ 1.0f,1.0f },
					m_TexturePesak
				);
				break;
			case VEZ:
				Renderer2D::DrawQuad(
					{ m_ChessField[j].position.x, m_ChessField[j].position.y, 1.0f },
					{ 1.0f,1.0f },
					m_TextureVez
				);
				break;
			case STRELEC:
				Renderer2D::DrawQuad(
					{ m_ChessField[j].position.x, m_ChessField[j].position.y, 1.0f },
					{ 1.0f,1.0f },
					m_TextureStrelec
				);
				break;
			case KUN:
				Renderer2D::DrawQuad(
					{ m_ChessField[j].position.x, m_ChessField[j].position.y, 1.0f },
					{ 1.0f,1.0f },
					m_TextureKun
				);
				break;
			case KRALOVNA:
				Renderer2D::DrawQuad(
					{ m_ChessField[j].position.x, m_ChessField[j].position.y, 1.0f },
					{ 1.0f,1.0f },
					m_TextureKralovna
				);
				break;
			case KRAL:
				Renderer2D::DrawQuad(
					{ m_ChessField[j].position.x, m_ChessField[j].position.y, 1.0f },
					{ 1.0f,1.0f },
					m_TextureKral
				);
				break;
			};
			j++;
		}
	}

#endif
}

void ChessGame::OnEvent(Event& e)
{
	Hurikan::EventDispatcher dispatcher(e);
	dispatcher.Dispatch<MouseMovedEvent>(HU_BIND_EVENT_FN(ChessGame::OnMouseMoved));
	//Dragging
	if (e.GetEventType() == EventType::MouseButtonPressed)
	{
		released =  false;
		pressed = true;
	}
	if (e.GetEventType() == EventType::MouseButtonReleased && pressed)
	{
		pressed = false;
		released = true;
	}
	//if (pressed) HU_CORE_TRACE("pressed");
	//if (released) HU_CORE_TRACE("released");

	m_CameraController.OnEvent(e);
}
bool ChessGame::OnMouseMoved(MouseMovedEvent& e)
{
	cursorPosX = e.GetX(), cursorPosY = e.GetY();

	glm::mat4 a = m_CameraController.GetCamera().GetViewProjectionMatrix();
	float rel = 0.5625/cursorPosX;
	glm::vec4 clipSpacePos = m_CameraController.GetCamera(). GetProjectionMatrix() * (m_CameraController.GetCamera().GetViewMatrix() * glm::vec4(cursorPosX,cursorPosY, 1.0,0.0));
	return true;
}

