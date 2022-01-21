#include "GameManager.h"

#include "BomberMan/Core/LevelManager.h"

#include <Hurikan/Renderer/Renderer2D.h>
#include <Hurikan/Renderer/RenderCommand.h>

void GameManager::OnAttach()
{
	LevelManager::Init();

	m_InGameScene.Init();
	m_InGameScene.Load();
}

void GameManager::OnDetach()
{
}

void GameManager::OnUpdate(Timestep& ts)
{
	RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	RenderCommand::Clear();
	Renderer2D::ResetStats();

	m_InGameScene.OnUpdate(ts);
}

void GameManager::OnImGuiRender()
{
}

void GameManager::OnEvent(Event& e)
{
}
