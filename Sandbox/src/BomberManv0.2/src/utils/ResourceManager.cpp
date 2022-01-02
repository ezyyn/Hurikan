#include "ResourceManager.h"

ResourceManager ResourceManager::s_Instance;

void ResourceManager::InitImpl(std::string&& filepath)
{
	m_Filepath = filepath;

	Ref<Texture2D> bombAnimationSpriteSheet = Texture2D::Create(m_Filepath + "bomb_animation/ba_128x128.png");
	Ref<Texture2D> spriteSheet =  Texture2D::Create(m_Filepath + "player_animation/Chr_00_0.png");
	Ref<Texture2D> spriteSheet1 = Texture2D::Create(m_Filepath + "tileset_16x16.png");

	Ref<Texture2D> heads_SpriteSheet = Texture2D::Create(m_Filepath + "player_animation/heads.png");

	m_TextureMap["EquippedText"] = Texture2D::Create(m_Filepath + "equipped_Text.png");
	Ref<Texture2D> equipped_options = Texture2D::Create(m_Filepath + "equipped_options.png");

	m_SubTextureMap["ClassicOpt"] = SubTexture2D::CreateFromCoords(equipped_options, { 0 , 1 }, { 125, 16 });
	m_SubTextureMap["DynamiteOpt"] = SubTexture2D::CreateFromCoords(equipped_options, { 0 , 0 }, { 125, 16 });
	m_SubTextureMap["Clock"] = SubTexture2D::CreateFromCoords(spriteSheet1, { 3, 7 }, { 16, 16 });

	Ref<Texture2D> fontSpriteSheet = Texture2D::Create("assets/textures/font_spritesheet.png");

	// Numbers
	{
		m_SubTextureMap["0"] = SubTexture2D::CreateFromCoords(fontSpriteSheet, { 1, 6 }, { 20, 20 });
		m_SubTextureMap["1"] = SubTexture2D::CreateFromCoords(fontSpriteSheet, { 2, 6 }, { 20, 20 });
		m_SubTextureMap["2"] = SubTexture2D::CreateFromCoords(fontSpriteSheet, { 3, 6 }, { 20, 20 });
		m_SubTextureMap["3"] = SubTexture2D::CreateFromCoords(fontSpriteSheet, { 4, 6 }, { 20, 20 });
		m_SubTextureMap["4"] = SubTexture2D::CreateFromCoords(fontSpriteSheet, { 5, 6 }, { 20, 20 });
		m_SubTextureMap["5"] = SubTexture2D::CreateFromCoords(fontSpriteSheet, { 6, 6 }, { 20, 20 });
		m_SubTextureMap["6"] = SubTexture2D::CreateFromCoords(fontSpriteSheet, { 7, 6 }, { 20, 20 });
		m_SubTextureMap["7"] = SubTexture2D::CreateFromCoords(fontSpriteSheet, { 8, 6 }, { 20, 20 });
		m_SubTextureMap["8"] = SubTexture2D::CreateFromCoords(fontSpriteSheet, { 9, 6 }, { 20, 20 });
		m_SubTextureMap["9"] = SubTexture2D::CreateFromCoords(fontSpriteSheet, { 10, 6 }, { 20, 20 });
	}
	//m_IdleAnimation = SubTexture2D::CreateFromCoords(spriteSheet, { 4, 7 }, { 16, 16 });
	//m_Handle.GetComponent<SpriteRendererComponent>().SubTexture = m_IdleAnimation;
	// PLAYER ANIMATIONS
//	m_PlayerAnimator.SetTarget(m_Handle);

	// Player animations
	{
		AnimationDetails playerLeftAnimation, playerUpAnimation, playerDownAnimation, playerDeadAnimation;
		{
			playerLeftAnimation.Tag = "LeftAnimation";
			playerLeftAnimation.SpriteSize = { 16,16 };
			playerLeftAnimation.SpriteSheet = spriteSheet;
			playerLeftAnimation.Repeat = true;

			playerLeftAnimation.Frames.push_back({ {0, 7}, glm::vec4(1.0f), 100.0f });
			playerLeftAnimation.Frames.push_back({ {1, 7}, glm::vec4(1.0f), 100.0f });
			playerLeftAnimation.Frames.push_back({ {2, 7}, glm::vec4(1.0f), 100.0f });
		}
		{
			playerUpAnimation.Tag = "UpAnimation";
			playerUpAnimation.SpriteSize = { 16,16 };
			playerUpAnimation.SpriteSheet = spriteSheet;
			playerUpAnimation.Repeat = true;

			playerUpAnimation.Frames.push_back({ {6, 7}, glm::vec4(1.0f), 100.0f });
			playerUpAnimation.Frames.push_back({ {7, 7}, glm::vec4(1.0f), 100.0f });
			playerUpAnimation.Frames.push_back({ {0, 6}, glm::vec4(1.0f), 100.0f });
		}
		{
			playerDownAnimation.Tag = "DownAnimation";
			playerDownAnimation.SpriteSize = { 16,16 };
			playerDownAnimation.SpriteSheet = spriteSheet;
			playerDownAnimation.Repeat = true;

			playerDownAnimation.Frames.push_back({ {3, 7}, glm::vec4(1.0f), 100.0f });
			playerDownAnimation.Frames.push_back({ {4, 7}, glm::vec4(1.0f), 100.0f });
			playerDownAnimation.Frames.push_back({ {5, 7}, glm::vec4(1.0f), 100.0f });
			playerDownAnimation.Frames.push_back({ {4, 7}, glm::vec4(1.0f), 100.0f });
		}
		{
			playerDeadAnimation.Tag = "PlayerDead";
			playerDeadAnimation.SpriteSize = { 16,16 };
			playerDeadAnimation.SpriteSheet = spriteSheet;
			//m_PlayerDead.Animation.emplace_back(FrameData{ nullptr,glm::vec4(0.0f), 300.0f });

			playerDeadAnimation.Frames.push_back({ { 1, 6 }, glm::vec4(1.0f), 300.0f });
			playerDeadAnimation.Frames.push_back({ { 2, 6 }, glm::vec4(1.0f), 300.0f });
			playerDeadAnimation.Frames.push_back({ { 3, 6 }, glm::vec4(1.0f), 300.0f });
			playerDeadAnimation.Frames.push_back({ { 4, 6 }, glm::vec4(1.0f), 300.0f });
			playerDeadAnimation.Frames.push_back({ { 5, 6 }, glm::vec4(1.0f), 300.0f });
			playerDeadAnimation.Frames.push_back({ { 6, 6 }, glm::vec4(1.0f), 300.0f });
			playerDeadAnimation.Frames.push_back({ { 7, 6 }, glm::vec4(1.0f), 300.0f });
		}

		m_AnimationMap["PlayerLeftAnimation"] = playerLeftAnimation;
		m_AnimationMap["PlayerUpAnimation"] = playerUpAnimation;
		m_AnimationMap["PlayerDownAnimation"] = playerDownAnimation;
		m_AnimationMap["PlayerDeadAnimation"] = playerDeadAnimation;
	}

	// Bomb animations
	{
		constexpr float bombexpl_anim = 100.0f;

		AnimationDetails bombTickingAnimation;
		{
			bombTickingAnimation.Tag = "BombTicking";
			bombTickingAnimation.SpriteSize = { 16,16 };
			// TODO: create ResouceManager     VVV
			bombTickingAnimation.SpriteSheet = spriteSheet1;
			bombTickingAnimation.Repeat = true;
			//bombTickingAnimation.PlayNow = true;

			bombTickingAnimation.Frames.push_back({ { 0, 7 }, glm::vec4(1.0f), 300.0f });
			bombTickingAnimation.Frames.push_back({ { 1, 7 }, glm::vec4(1.0f), 300.0f });
			bombTickingAnimation.Frames.push_back({ { 2, 7 }, glm::vec4(1.0f), 300.0f });
			bombTickingAnimation.Frames.push_back({ { 1, 7 }, glm::vec4(1.0f), 300.0f });
		}

		AnimationDetails
			bombSpreadCenterAnimation,
			bombSpreadMiddleAnimation,
			bombSpreadEndAnimation;
		{
			bombSpreadCenterAnimation.Tag = "BombCenterExplosion";
			bombSpreadCenterAnimation.SpriteSize = { 16,16 };
			bombSpreadCenterAnimation.SpriteSheet = spriteSheet1;
			//bombSpreadCenterAnimation.Repeat = true;

			bombSpreadCenterAnimation.Frames.push_back({ { 0, 1 }, glm::vec4(1.0f), bombexpl_anim });
			bombSpreadCenterAnimation.Frames.push_back({ { 2, 1 }, glm::vec4(1.0f), bombexpl_anim });
			bombSpreadCenterAnimation.Frames.push_back({ { 4, 1 }, glm::vec4(1.0f), bombexpl_anim });
			bombSpreadCenterAnimation.Frames.push_back({ { 6, 1 }, glm::vec4(1.0f), bombexpl_anim });
			bombSpreadCenterAnimation.Frames.push_back({ { 4, 1 }, glm::vec4(1.0f), bombexpl_anim });
			bombSpreadCenterAnimation.Frames.push_back({ { 2, 1 }, glm::vec4(1.0f), bombexpl_anim });
			bombSpreadCenterAnimation.Frames.push_back({ { 0, 1 }, glm::vec4(1.0f), bombexpl_anim });
		}

		{
			bombSpreadMiddleAnimation.Tag = "BombSpreadMiddleExplosion";
			bombSpreadMiddleAnimation.SpriteSize = { 16,16 };
			bombSpreadMiddleAnimation.SpriteSheet = spriteSheet1;
			//bombSpreadMiddleAnimation.Repeat = true;

			bombSpreadMiddleAnimation.Frames.push_back({ { 0, 3 }, glm::vec4(1.0f), bombexpl_anim });
			bombSpreadMiddleAnimation.Frames.push_back({ { 2, 3 }, glm::vec4(1.0f), bombexpl_anim });
			bombSpreadMiddleAnimation.Frames.push_back({ { 4, 3 }, glm::vec4(1.0f), bombexpl_anim });
			bombSpreadMiddleAnimation.Frames.push_back({ { 6, 3 }, glm::vec4(1.0f), bombexpl_anim });
			bombSpreadMiddleAnimation.Frames.push_back({ { 4, 3 }, glm::vec4(1.0f), bombexpl_anim });
			bombSpreadMiddleAnimation.Frames.push_back({ { 2, 3 }, glm::vec4(1.0f), bombexpl_anim });
			bombSpreadMiddleAnimation.Frames.push_back({ { 0, 3 }, glm::vec4(1.0f), bombexpl_anim });
		}

		{
			bombSpreadEndAnimation.Tag = "BombEndWingExplosion";
			bombSpreadEndAnimation.SpriteSize = { 16,16 };
			bombSpreadEndAnimation.SpriteSheet = spriteSheet1;
			//bombSpreadEndAnimation.Repeat = true;

			bombSpreadEndAnimation.Frames.push_back({ { 0, 5 }, glm::vec4(1.0f), bombexpl_anim });
			bombSpreadEndAnimation.Frames.push_back({ { 2, 5 }, glm::vec4(1.0f), bombexpl_anim });
			bombSpreadEndAnimation.Frames.push_back({ { 4, 5 }, glm::vec4(1.0f), bombexpl_anim });
			bombSpreadEndAnimation.Frames.push_back({ { 6, 5 }, glm::vec4(1.0f), bombexpl_anim });
			bombSpreadEndAnimation.Frames.push_back({ { 4, 5 }, glm::vec4(1.0f), bombexpl_anim });
			bombSpreadEndAnimation.Frames.push_back({ { 2, 5 }, glm::vec4(1.0f), bombexpl_anim });
			bombSpreadEndAnimation.Frames.push_back({ { 0, 5 }, glm::vec4(1.0f), bombexpl_anim });
		}

		m_AnimationMap["BombTicking"] = bombTickingAnimation;
		m_AnimationMap["BombCenterExplosion"] = bombSpreadCenterAnimation;
		m_AnimationMap["BombSpreadMiddleExplosion"] = bombSpreadMiddleAnimation;
		m_AnimationMap["BombEndWingExplosion"] = bombSpreadEndAnimation;
	}

	// Grid animations
	{
		AnimationDetails wallBreakAnimation;
		{
			wallBreakAnimation.Tag = "WallBreakAnimation";
			wallBreakAnimation.SpriteSize = { 16,16 };
			wallBreakAnimation.SpriteSheet = spriteSheet1;

			wallBreakAnimation.Frames.push_back({ { 2,6 }, glm::vec4(1.0f), { 250.0f } });
			wallBreakAnimation.Frames.push_back({ { 3,6 }, glm::vec4(1.0f), { 250.0f } });
			wallBreakAnimation.Frames.push_back({ { 4,6 }, glm::vec4(1.0f), { 250.0f } });
			wallBreakAnimation.Frames.push_back({ { 5,6 }, glm::vec4(1.0f), { 250.0f } });
			wallBreakAnimation.Frames.push_back({ { 6,6 }, glm::vec4(1.0f), { 250.0f } });
			wallBreakAnimation.Frames.push_back({ { 7,6 }, glm::vec4(1.0f), { 250.0f } });
		}
		m_AnimationMap["WallBreakAnimation"] = wallBreakAnimation;
	}
	// UI UI UI UI UI UI UI UI UI UI UI UI UI UI UI UI UI UI UI UI UI UI UI UI UI UI UI UI UI UI UI UI UI 
	{
		AnimationDetails 
			playerLeftAnimation,
			playerUpAnimation,
			playerDownAnimation;
		{
			playerLeftAnimation.Tag = "LeftAnimation";
			playerLeftAnimation.SpriteSize = { 13,11 };
			playerLeftAnimation.SpriteSheet = heads_SpriteSheet;
			playerLeftAnimation.Repeat = true;
							   
			playerLeftAnimation.Frames.push_back({ {0, 0}, glm::vec4(1.0f), 100.0f });
			playerLeftAnimation.Frames.push_back({ {1, 0}, glm::vec4(1.0f), 100.0f });
			playerLeftAnimation.Frames.push_back({ {2, 0}, glm::vec4(1.0f), 100.0f });
		}

		{
			playerUpAnimation.Tag = "UpAnimation";
			playerUpAnimation.SpriteSize = { 13,11 };
			playerUpAnimation.SpriteSheet = heads_SpriteSheet;
			playerUpAnimation.Repeat = true;
							 
			playerUpAnimation.Frames.push_back({ {6, 0}, glm::vec4(1.0f), 100.0f });
			playerUpAnimation.Frames.push_back({ {7, 0}, glm::vec4(1.0f), 100.0f });
			playerUpAnimation.Frames.push_back({ {8, 0}, glm::vec4(1.0f), 100.0f });
		}

		{
			playerDownAnimation.Tag = "DownAnimation";
			playerDownAnimation.SpriteSize = { 13,11 };
			playerDownAnimation.SpriteSheet = heads_SpriteSheet;
			playerDownAnimation.Repeat = true;
							   
			playerDownAnimation.Frames.push_back({ {3, 0}, glm::vec4(1.0f), 100.0f });
			playerDownAnimation.Frames.push_back({ {4, 0}, glm::vec4(1.0f), 100.0f });
			playerDownAnimation.Frames.push_back({ {5, 0}, glm::vec4(1.0f), 100.0f });
			playerDownAnimation.Frames.push_back({ {4, 0}, glm::vec4(1.0f), 100.0f });
		}

		m_AnimationMap["HeadUpAnimation"] = playerUpAnimation;
		m_AnimationMap["HeadLeftAnimation"] = playerLeftAnimation;
		m_AnimationMap["HeadDownAnimation"] = playerDownAnimation;
	}
}

const Ref<Texture2D>& ResourceManager::RequestTextureImpl(std::string&& tag)
{
	return m_TextureMap[tag];
}

const Ref<SubTexture2D>& ResourceManager::RequestSubTextureImpl(const std::string& tag)
{
	return m_SubTextureMap[tag];
}

const Ref<SubTexture2D>& ResourceManager::RequestSubTextureImpl(int number)
{
	return m_NumbersMap[number];
}

const AnimationDetails& ResourceManager::RequestAnimationImpl(std::string&& tag)
{
	return m_AnimationMap[tag];
}


