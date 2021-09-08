#include "ChessApp.h"
#include "Hurikan.h"

#include "Hurikan/Core/EntryPoint.h"

ChessApp::ChessApp() : m_ChessGame()
{
}

void ChessApp::OnAttach()
{
}

void ChessApp::OnDetach()
{
}

void ChessApp::OnUpdate(Hurikan::Timestep ts)
{
	RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	RenderCommand::Clear();

	m_ChessGame.OnUpdate(ts);


	Renderer2D::EndScene();
}

void ChessApp::OnImGuiRender()
{
}

void ChessApp::OnEvent(Hurikan::Event& e)
{
	//Zooming
	m_ChessGame.OnEvent(e);
}
