#include "LevelManager.h"

LevelManager LevelManager::s_Instance;

void LevelManager::Init_Impl()
{
	SaveManager::Init();

	m_Levels = SaveManager::LoadLevels();

	m_CurrentLevel = m_Levels[1];
}

