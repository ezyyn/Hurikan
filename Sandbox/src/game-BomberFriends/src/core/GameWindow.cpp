#include "GameWindow.h"

namespace GameFramework 
{
	//TODO: Mapa -> hr·Ë 

	//TEMP:
	std::string LevelMapString; // w16 x h16
	int w_level = 17;
	int h_level = 17;
	//-----

	Game::Game() : Layer("GameFrameworkLayer"), m_CameraController(1280.0f / 720.0f, false), m_Player(m_CameraController), m_GameGrid(17, 17)
	{
		HU_INFO("Game has started!");
		Init();
	}

	void Game::Init()
	{
		m_GameGrid.Init();

		LevelMapString += "#################";
		LevelMapString += "#--------------O#";
		LevelMapString += "#-#-#-#-#-#-#-#-#";
		LevelMapString += "#---------------#";
		LevelMapString += "#-#-#-#-#-#-#-#-#";
		LevelMapString += "#---------------#";
		LevelMapString += "#-#-#-#-#-#-#-#-#";
		LevelMapString += "#---------------#";
		LevelMapString += "#-#-#-#-#-#-#-#-#";
		LevelMapString += "#---------------#";
		LevelMapString += "#-#-#-#-#-#-#-#-#";
		LevelMapString += "#---------------#";
		LevelMapString += "#-#-#-#-#-#-#-#-#";
		LevelMapString += "#---------------#";
		LevelMapString += "#-#-#-#-#-#-#-#-#";
		LevelMapString += "#---------------#";
		LevelMapString += "#################";

		m_CameraController.SetZoomLevel(9);
		m_CameraController.SetPosition({ 14, 9, 0 });
	}

	void Game::OnUpdate(Timestep ts)
	{
		m_CameraController.OnUpdate(ts);
		m_Player.Update(ts);


		RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f});
		RenderCommand::Clear();

		Renderer2D::BeginScene(m_CameraController.GetCamera());
		
		m_Player.Draw();
		
		for (size_t y = 0; y < w_level; y++)
		{
			for (size_t x = 0; x < h_level; x++)
			{
				uint32_t block_type = LevelMapString[x + y * w_level];
				if(block_type == '#')
					Renderer2D::DrawQuad({ x, h_level - y, 0.0f }, { 1, 1 }, { 0.8,0.6,0.5,1.0 });
				else if (block_type == 'O')
					Renderer2D::DrawQuad({ x, h_level - y, 0.0f }, { 1, 1 }, { 0.0,0.0,0.0,1.0 });
				else
					Renderer2D::DrawQuad({ x, h_level - y, 0.0f }, { 1, 1 }, { 1.0,1.0,1.0,1.0 });
			}
		}

		Renderer2D::EndScene();
	}

	void Game::OnEvent(Event& e)
	{
		m_CameraController.OnEvent(e);
	}
}