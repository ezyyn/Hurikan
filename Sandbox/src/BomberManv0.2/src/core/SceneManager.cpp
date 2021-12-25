#include "../core/SceneManager.h"

#include "../ui/GameText.h"

void SceneManager::Init()
{
#ifdef EXPRM
	m_TestScene.Init();
	return;
#endif

	GameText::Init();

	m_MainMenu.Init(this);
	m_Game.Init();
}

void SceneManager::Shutdown()
{
#ifdef EXPRM
	m_TestScene.Shutdown();
	return;
#endif
	m_MainMenu.Shutdown();
	m_Game.Shutdown();
}

void SceneManager::OnUpdate(Timestep ts)
{
	
#ifdef EXPRM
	m_TestScene.OnUpdate(ts);
	return;
#endif
	switch (m_GameState)
	{
	case GameState::MENU:
		m_MainMenu.OnUpdate(ts);
		break;
	case GameState::INGAME:
		m_Game.OnUpdate(ts);
		break;
	default:
		break;
	}
}

void SceneManager::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<KeyPressedEvent>(HU_BIND_EVENT_FN(OnKeyPressed));
	dispatcher.Dispatch<KeyReleasedEvent>(HU_BIND_EVENT_FN(OnKeyReleased));
}

bool F4_Pressed = false;

bool SceneManager::OnKeyPressed(KeyPressedEvent& e)
{
#ifdef EXPRM
	return false;
#endif

	// TODO: maybe move to GameLayer.h because scenemananager should not handle this
	if (e.GetKeyCode() == Key::F4 && !F4_Pressed)
	{
		F4_Pressed = true;

		Application::Get().SetFullScreen(!Application::Get().FullScreenEnabled());
		return true;
	}

	if (m_GameState == GameState::MENU)
		m_MainMenu.OnKeyPressed(e);
	else if (m_GameState == GameState::INGAME)
		m_Game.OnKeyPressed(e);
	return true;
}

bool SceneManager::OnKeyReleased(KeyReleasedEvent& e)
{
#if EXPRM
	return false;
#endif

	F4_Pressed = false;

	if (m_GameState == GameState::MENU)
		m_MainMenu.OnKeyReleased(e);
	else if (m_GameState == GameState::INGAME)
		m_Game.OnKeyReleased(e);
	return true;
}

void SceneManager::Switch(GameState state)
{
	switch (state)
	{
	case GameState::INGAME:
	{
		Level firstLevel;
		/*
		firstLevel.Height = 15;
		firstLevel.Width = 68;
		firstLevel.MapSkeleton += "####################################################################";
		firstLevel.MapSkeleton += "#------BBB---------------------------------------------------------#";
		firstLevel.MapSkeleton += "#-#-#-#-#-#-#------------------------------------------------------#";
		firstLevel.MapSkeleton += "#----BBBB----------------------------------------------------------#";
		firstLevel.MapSkeleton += "#-#-#-#-#-#-#------------------------------------------------------#";
		firstLevel.MapSkeleton += "#--------B---------------------------------------------------------#";
		firstLevel.MapSkeleton += "#-#-#-#-#-#B#------------------------------------------------------#";
		firstLevel.MapSkeleton += "#---BBBBB--BB------------------------------------------------------#";
		firstLevel.MapSkeleton += "#-#-#-#-#-#B#------------------------------------------------------#";
		firstLevel.MapSkeleton += "#------BBBB--------------------------------------------------------#";
		firstLevel.MapSkeleton += "#-#-#-#-#-#-#------------------------------------------------------#";
		firstLevel.MapSkeleton += "#--M---------------------------------------------------------------#";
		firstLevel.MapSkeleton += "#-#-#-#-#-#-#------------------------------------------------------#";
		firstLevel.MapSkeleton += "#P-----------------------------------------------------------------#";
		firstLevel.MapSkeleton += "####################################################################";
		*/
		firstLevel.Height = 15;
		firstLevel.Width = 15;
		firstLevel.MapSkeleton += "###############";
		firstLevel.MapSkeleton += "#------BBB----#";
		firstLevel.MapSkeleton += "#-#-#-#-#-#-#-#";
		firstLevel.MapSkeleton += "#----BBBB-----#";
		firstLevel.MapSkeleton += "#-#-#-#-#-#-#-#";
		firstLevel.MapSkeleton += "#--------B----#";
		firstLevel.MapSkeleton += "#-#-#-#-#-#B#-#";
		firstLevel.MapSkeleton += "#---BBBBB--BB-#";
		firstLevel.MapSkeleton += "#-#-#-#-#-#B#-#";
		firstLevel.MapSkeleton += "#------BBBB---#";
		firstLevel.MapSkeleton += "#-#-#-#-#-#-#-#";
		firstLevel.MapSkeleton += "#--M----------#";
		firstLevel.MapSkeleton += "#-#-#-#-#-#-#-#";
		firstLevel.MapSkeleton += "#P------------#";
		firstLevel.MapSkeleton += "###############";


		m_Game.Play(firstLevel);
		break;
	}
	}

	m_GameState = state;
}
