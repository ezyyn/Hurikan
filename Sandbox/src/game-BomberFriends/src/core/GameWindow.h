#pragma once
#include <Hurikan.h>
#include "Player.h"
#include "GameGrid.h"

using namespace Hurikan;

namespace GameFramework
{
	enum class BombType
	{
		TimeBomb,Dynamite,Rocket
	};

	class GameLayer : public Layer
	{
	public:
		GameLayer();
	private:
		void Init();

		void OnImGuiRender() override;
		void OnUpdate(Timestep& ts) override;
		void OnEvent(Event& e) override;
	private:
		uint32_t m_Width = 1280, m_Height = 720;

		OrthographicCameraController m_CameraController;
		Player m_Player;
		GameGrid m_GameGrid;
		EditorCamera m_EditorCamera;
	};
}