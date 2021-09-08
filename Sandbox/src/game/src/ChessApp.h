#pragma once
#include "Hurikan.h"
#include "./core/ChessGame.h"

using namespace Hurikan;

class ChessApp : public Layer
{
public:
	ChessApp();
	virtual ~ChessApp() = default;
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Event & e) override;
private:
	ChessGame m_ChessGame;
};