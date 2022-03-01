#include "MainMenuSettings.hpp"

#include "BomberMan/Core/ResourceManager.hpp"
#include "BomberMan/Core/SaveLoadSystem.hpp"
#include "BomberMan/Core/AudioManager.hpp"

void MainMenuSettings::Init(Scene* scene, Entity& arrowHead)
{
	m_MusicCounter = (int)(SaveLoadSystem::GetUserSettings().MusicVolume * 100);
	m_SfxCounter = (int)(SaveLoadSystem::GetUserSettings().SfxVolume * 100);

	m_ArrowHead = arrowHead;
	{
		m_SettingsOptions = scene->CreateEntityWithDrawOrder(2);

		auto& transform = m_SettingsOptions.Transform();

		transform.Translation.y = -2.9f;
		transform.Translation.x = 4.5f;
		transform.Scale *= 3.0f;

		m_SettingsOptions.AddComponent<SpriteRendererComponent>(glm::vec4(0.0f)).Texture = ResourceManager::GetTexture("MainMenuSettings");
	}

	auto& music = std::to_string(m_MusicCounter);
	auto& sfx = std::to_string(m_SfxCounter);

	for (size_t i = 0; i < 3; ++i)
	{
		m_MusicPercentage[i] = scene->CreateEntityWithDrawOrder(2);

		m_MusicPercentage[i].AddComponent<SpriteRendererComponent>(glm::vec4(0.0f)).SubTexture = nullptr;

		auto& transform = m_MusicPercentage[i].Transform();
		transform.Translation.x = 5 + i * 0.25f;
		transform.Translation.y = -2.1f;
		transform.Scale *= 0.25f;
	}

	for (size_t i = 0; i < 3; ++i)
	{
		m_SfxPercentage[i] = scene->CreateEntityWithDrawOrder(2);
		m_SfxPercentage[i].AddComponent<SpriteRendererComponent>(glm::vec4(0.0f)).SubTexture = nullptr;
		auto& transform = m_SfxPercentage[i].Transform();
		transform.Translation.x = 5 + i * 0.25f;
		transform.Translation.y = -2.6f;
		transform.Scale *= 0.25f;
	}

	UpdateMusic();
	UpdateSfx();

	for (size_t i = 0; i < 3; ++i)
	{
		m_SfxPercentage[i].GetComponent<SpriteRendererComponent>().Color = glm::vec4(0.0f);
		m_MusicPercentage[i].GetComponent<SpriteRendererComponent>().Color = glm::vec4(0.0f);
	}
}

void MainMenuSettings::Show(bool show)
{
	if (m_Displayed == show)
		return;

	m_Displayed = show;

	m_MusicCounter = (int)(SaveLoadSystem::GetUserSettings().MusicVolume * 100);
	m_SfxCounter = (int)(SaveLoadSystem::GetUserSettings().SfxVolume * 100);
	m_ConfirmedOption = SettingsMenuOption::NONE;
	m_SelectedOption = SettingsMenuOption::MUSIC;

	UpdateMusic();
	UpdateSfx();

	if (m_Displayed)
	{
		for (int i = 0; i < 3; ++i)
		{
			if (m_MusicPercentage[i].GetComponent<SpriteRendererComponent>().SubTexture)
			{
				m_MusicPercentage[i].GetComponent<SpriteRendererComponent>().Color = glm::vec4(1.0f);
			}
			if (m_SfxPercentage[i].GetComponent<SpriteRendererComponent>().SubTexture)
			{
				m_SfxPercentage[i].GetComponent<SpriteRendererComponent>().Color = glm::vec4(1.0f);
			}
		}

		m_SettingsOptions.GetComponent<SpriteRendererComponent>().Color = glm::vec4(1.0f);
	}
	else
	{
		for (int i = 0; i < 3; ++i)
		{
			m_MusicPercentage[i].GetComponent<SpriteRendererComponent>().Color = glm::vec4(0.0f);
			m_SfxPercentage[i].GetComponent<SpriteRendererComponent>().Color = glm::vec4(0.0f);
		}
		m_SettingsOptions.GetComponent<SpriteRendererComponent>().Color = glm::vec4(0.0f);
	}
}

void MainMenuSettings::OnKeyPressed(KeyCode key)
{
	if (key == Key::Up)
	{
		if (m_ConfirmedOption == m_SelectedOption)
		{
			if (m_ConfirmedOption == SettingsMenuOption::MUSIC && m_MusicCounter < 100)
			{
				m_MusicCounter++;
				UpdateMusic();
			}
			else if (m_ConfirmedOption == SettingsMenuOption::SFX && m_SfxCounter < 100)
			{
				m_SfxCounter++;
				UpdateSfx();
			}
			AudioManager::UpdateVolume(m_MusicCounter / 100.0f, m_SfxCounter / 100.0f);
			return;
		}

		if (m_SelectedOption > SettingsMenuOption::MUSIC)
		{
			m_SelectedOption = (SettingsMenuOption)((int)m_SelectedOption - 1);
			m_ArrowHead.Transform().Translation.y += 0.55f;
		}
	}
	else if (key == Key::Down)
	{
		if (m_ConfirmedOption == m_SelectedOption)
		{
			if (m_ConfirmedOption == SettingsMenuOption::MUSIC && m_MusicCounter > 0)
			{
				m_MusicCounter--;
				UpdateMusic();
			}
			else if (m_ConfirmedOption == SettingsMenuOption::SFX && m_SfxCounter > 0)
			{
				m_SfxCounter--;
				UpdateSfx();
			}
			AudioManager::UpdateVolume(m_MusicCounter / 100.0f, m_SfxCounter / 100.0f);
			return;
		}

		if (m_SelectedOption < SettingsMenuOption::CANCEL)
		{
			m_SelectedOption = (SettingsMenuOption)((int)m_SelectedOption + 1);
			m_ArrowHead.Transform().Translation.y -= 0.55f;
		}
	}
	else if (key == Key::Enter)
	{
		switch (m_SelectedOption)
		{
		case SettingsMenuOption::MUSIC:
		{
			for (int i = 0; i < 3; ++i)
			{
				if (m_MusicPercentage[i].GetComponent<SpriteRendererComponent>().SubTexture)
				{
					m_MusicPercentage[i].GetComponent<SpriteRendererComponent>().Color = m_ConfirmedOptionColor;
				}
				if (m_MusicPercentage[i].GetComponent<SpriteRendererComponent>().SubTexture)
				{
					m_MusicPercentage[i].GetComponent<SpriteRendererComponent>().Color = m_ConfirmedOptionColor;
				}
			}
			break;
		}
		case SettingsMenuOption::SFX:
		{
			for (int i = 0; i < 3; ++i)
			{
				if (m_SfxPercentage[i].GetComponent<SpriteRendererComponent>().SubTexture)
				{
					m_SfxPercentage[i].GetComponent<SpriteRendererComponent>().Color = m_ConfirmedOptionColor;
				}
				if (m_SfxPercentage[i].GetComponent<SpriteRendererComponent>().SubTexture)
				{
					m_SfxPercentage[i].GetComponent<SpriteRendererComponent>().Color = m_ConfirmedOptionColor;
				}
			}
			break;
		}
		case SettingsMenuOption::SAVE:
		{
			m_SelectedOption = SettingsMenuOption::MUSIC;

			auto music = m_MusicCounter / 100.0f;
			auto sfx = m_SfxCounter / 100.0f;
			SaveLoadSystem::SaveUserSettings(music, sfx);

			Dispatch(GameEventType::RETURN_FROM_MM_SETTINGS);
			break;
		}
		case SettingsMenuOption::CANCEL:
		{
			m_SelectedOption = SettingsMenuOption::MUSIC;
			m_MusicCounter = (int)(SaveLoadSystem::GetUserSettings().MusicVolume * 100);
			m_SfxCounter = (int)(SaveLoadSystem::GetUserSettings().SfxVolume * 100);

			AudioManager::UpdateVolume(SaveLoadSystem::GetUserSettings().MusicVolume, SaveLoadSystem::GetUserSettings().SfxVolume);

			Dispatch(GameEventType::RETURN_FROM_MM_SETTINGS);
			break;
		}
		default:
			break;
		}
		if (m_ConfirmedOption == m_SelectedOption)
		{
			m_ConfirmedOption = SettingsMenuOption::NONE;

			if (m_SelectedOption == SettingsMenuOption::MUSIC)
			{
				for (int i = 0; i < 3; ++i)
				{
					if (m_MusicPercentage[i].GetComponent<SpriteRendererComponent>().SubTexture)
					{
						m_MusicPercentage[i].GetComponent<SpriteRendererComponent>().Color = glm::vec4(1.0f);
					}
					if (m_MusicPercentage[i].GetComponent<SpriteRendererComponent>().SubTexture)
					{
						m_MusicPercentage[i].GetComponent<SpriteRendererComponent>().Color = glm::vec4(1.0f);
					}
				}
			}
			else if (m_SelectedOption == SettingsMenuOption::SFX)
			{
				for (int i = 0; i < 3; ++i)
				{
					if (m_SfxPercentage[i].GetComponent<SpriteRendererComponent>().SubTexture)
					{
						m_SfxPercentage[i].GetComponent<SpriteRendererComponent>().Color = glm::vec4(1.0f);
					}
					if (m_SfxPercentage[i].GetComponent<SpriteRendererComponent>().SubTexture)
					{
						m_SfxPercentage[i].GetComponent<SpriteRendererComponent>().Color = glm::vec4(1.0f);
					}
				}
			}
		}
		else
			m_ConfirmedOption = m_SelectedOption;
	}
}

void MainMenuSettings::UpdateSfx()
{
	std::string sfx = std::to_string(m_SfxCounter);

	if (m_SfxCounter < 10)
	{
		m_SfxPercentage[0].GetComponent<SpriteRendererComponent>().SubTexture = nullptr;
		m_SfxPercentage[0].GetComponent<SpriteRendererComponent>().Color = glm::vec4(0.0f);

		m_SfxPercentage[1].GetComponent<SpriteRendererComponent>().SubTexture = nullptr;
		m_SfxPercentage[1].GetComponent<SpriteRendererComponent>().Color = glm::vec4(0.0f);

		m_SfxPercentage[2].GetComponent<SpriteRendererComponent>().SubTexture = ResourceManager::GetSubTexture(sfx[0]);
		m_SfxPercentage[2].GetComponent<SpriteRendererComponent>().Color = m_ConfirmedOptionColor;
	}
	else if (m_SfxCounter >= 10 && m_SfxCounter < 100)
	{
		m_SfxPercentage[0].GetComponent<SpriteRendererComponent>().SubTexture = nullptr;
		m_SfxPercentage[0].GetComponent<SpriteRendererComponent>().Color = glm::vec4(0.0f);

		m_SfxPercentage[1].GetComponent<SpriteRendererComponent>().SubTexture = ResourceManager::GetSubTexture(sfx[0]);
		m_SfxPercentage[1].GetComponent<SpriteRendererComponent>().Color = m_ConfirmedOptionColor;

		m_SfxPercentage[2].GetComponent<SpriteRendererComponent>().SubTexture = ResourceManager::GetSubTexture(sfx[1]);
		m_SfxPercentage[2].GetComponent<SpriteRendererComponent>().Color = m_ConfirmedOptionColor;
	}
	else if (m_SfxCounter == 100)
	{
		m_SfxPercentage[0].GetComponent<SpriteRendererComponent>().SubTexture = ResourceManager::GetSubTexture(sfx[0]);
		m_SfxPercentage[0].GetComponent<SpriteRendererComponent>().Color = m_ConfirmedOptionColor;

		m_SfxPercentage[1].GetComponent<SpriteRendererComponent>().SubTexture = ResourceManager::GetSubTexture(sfx[1]);
		m_SfxPercentage[1].GetComponent<SpriteRendererComponent>().Color = m_ConfirmedOptionColor;

		m_SfxPercentage[2].GetComponent<SpriteRendererComponent>().SubTexture = ResourceManager::GetSubTexture(sfx[2]);
		m_SfxPercentage[2].GetComponent<SpriteRendererComponent>().Color = m_ConfirmedOptionColor;
	}
}

void MainMenuSettings::UpdateMusic()
{
	std::string music = std::to_string(m_MusicCounter);

	if (m_MusicCounter < 10)
	{
		m_MusicPercentage[0].GetComponent<SpriteRendererComponent>().SubTexture = nullptr;
		m_MusicPercentage[0].GetComponent<SpriteRendererComponent>().Color = glm::vec4(0.0f);

		m_MusicPercentage[1].GetComponent<SpriteRendererComponent>().SubTexture = nullptr;
		m_MusicPercentage[1].GetComponent<SpriteRendererComponent>().Color = glm::vec4(0.0f);

		m_MusicPercentage[2].GetComponent<SpriteRendererComponent>().SubTexture = ResourceManager::GetSubTexture(music[0]);
		m_MusicPercentage[2].GetComponent<SpriteRendererComponent>().Color = m_ConfirmedOptionColor;
	}
	else if (m_MusicCounter >= 10 && m_MusicCounter < 100)
	{
		m_MusicPercentage[0].GetComponent<SpriteRendererComponent>().SubTexture = nullptr;
		m_MusicPercentage[0].GetComponent<SpriteRendererComponent>().Color = glm::vec4(0.0f);

		m_MusicPercentage[1].GetComponent<SpriteRendererComponent>().SubTexture = ResourceManager::GetSubTexture(music[0]);
		m_MusicPercentage[1].GetComponent<SpriteRendererComponent>().Color = m_ConfirmedOptionColor;

		m_MusicPercentage[2].GetComponent<SpriteRendererComponent>().SubTexture = ResourceManager::GetSubTexture(music[1]);
		m_MusicPercentage[2].GetComponent<SpriteRendererComponent>().Color = m_ConfirmedOptionColor;
	}
	else if (m_MusicCounter == 100)
	{
		m_MusicPercentage[0].GetComponent<SpriteRendererComponent>().SubTexture = ResourceManager::GetSubTexture(music[0]);
		m_MusicPercentage[0].GetComponent<SpriteRendererComponent>().Color = m_ConfirmedOptionColor;

		m_MusicPercentage[1].GetComponent<SpriteRendererComponent>().SubTexture = ResourceManager::GetSubTexture(music[1]);
		m_MusicPercentage[1].GetComponent<SpriteRendererComponent>().Color = m_ConfirmedOptionColor;

		m_MusicPercentage[2].GetComponent<SpriteRendererComponent>().SubTexture = ResourceManager::GetSubTexture(music[2]);
		m_MusicPercentage[2].GetComponent<SpriteRendererComponent>().Color = m_ConfirmedOptionColor;
	}
}

