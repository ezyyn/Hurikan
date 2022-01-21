#include "ResourceManager.h"

ResourceManager ResourceManager::s_Instance;

void ResourceManager::Init_Impl()
{
	std::string m_Filepath = "assets/textures/";

	Ref<Texture2D> bombAnimationSpriteSheet = Texture2D::Create(m_Filepath + "bomb_animation/ba_128x128.png");
	Ref<Texture2D> spriteSheet =  Texture2D::Create(m_Filepath + "player_animation/Chr_00_0.png");
	Ref<Texture2D> spriteSheet1 = Texture2D::Create(m_Filepath + "tileset_16x16.png");

	// Grid
	{
		m_SubTextureMap["Wall"] = SubTexture2D::CreateFromCoords(spriteSheet1, { 0, 0 }, { 16, 16 });
		m_SubTextureMap["BreakableWall"] = SubTexture2D::CreateFromCoords(spriteSheet1, { 1, 0 }, { 16, 16 });
	}
	// Background
	{
		m_TextureMap["Background"] = Texture2D::Create(m_Filepath + "background.png");
	}

	// Player Idle
	{
		m_SubTextureMap["PlayerIdle"] = SubTexture2D::CreateFromCoords(spriteSheet, { 4, 7 }, { 16, 16 });
	}

	// Animations // 
	// Player
	{
		Animation playerLeftAnimation, playerUpAnimation, playerDownAnimation, playerDeadAnimation;
		{
			playerLeftAnimation.Tag = "LeftAnimation";
			playerLeftAnimation.Repeat = true;
			playerLeftAnimation.Delay = 100.0f;

			std::vector<Frame> frames;
			frames.reserve(3);
			frames.emplace_back(glm::vec2{ 0, 7 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 1, 7 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 2, 7 }, glm::vec4(1.0f));

			playerLeftAnimation.Create(spriteSheet, { 16, 16 }, frames);
		}
		{
			playerUpAnimation.Tag = "UpAnimation";
			playerUpAnimation.Repeat = true;
			playerUpAnimation.Delay = 100.0f;

			std::vector<Frame> frames;
			frames.reserve(3);
			frames.emplace_back(glm::vec2{ 6, 7 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 7, 7 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 0, 6 }, glm::vec4(1.0f));

			playerUpAnimation.Create(spriteSheet, { 16, 16 }, frames);
		}
		{
			playerDownAnimation.Tag = "DownAnimation";
			playerDownAnimation.Repeat = true;
			playerDownAnimation.Delay = 100.0f;

			std::vector<Frame> frames;
			frames.reserve(4);
			frames.emplace_back(glm::vec2{ 3, 7 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 4, 7 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 5, 7 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 4, 7 }, glm::vec4(1.0f));

			playerDownAnimation.Create(spriteSheet, { 16, 16 }, frames);
		}
		{
			playerDeadAnimation.Tag = "PlayerDead";
			playerDeadAnimation.Delay = 300.0f;
			//m_PlayerDead.Animation.emplace_back(FrameData{ nullptr,glm::vec4(0.0f), 300.0f });

			std::vector<Frame> frames;
			frames.reserve(7);
			frames.emplace_back(glm::vec2{ 1, 6 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 2, 6 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 3, 6 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 4, 6 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 5, 6 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 6, 6 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 7, 6 }, glm::vec4(1.0f));

			playerDeadAnimation.Create(spriteSheet, { 16, 16 }, frames);
		}

		m_AnimationMap["PlayerLeftAnimation"] = playerLeftAnimation;
		m_AnimationMap["PlayerUpAnimation"] = playerUpAnimation;
		m_AnimationMap["PlayerDownAnimation"] = playerDownAnimation;
		m_AnimationMap["PlayerDeadAnimation"] = playerDeadAnimation;
	}

	// Bomb animations
	{
		constexpr float bombexpl_anim = 100.0f;

		Animation bombTickingAnimation;
		{
			bombTickingAnimation.Tag = "BombTicking";
			bombTickingAnimation.Repeat = true;
			bombTickingAnimation.Delay = 300.0f;

			std::vector<Frame> frames;
			frames.reserve(4);
			frames.emplace_back(glm::vec2{ 0, 7 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 1, 7 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 2, 7 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 1, 7 }, glm::vec4(1.0f));

			bombTickingAnimation.Create(spriteSheet1, { 16, 16 }, frames);
		}

		Animation
			bombSpreadCenterAnimation,
			bombSpreadMiddleAnimation,
			bombSpreadEndAnimation;
		{
			bombSpreadCenterAnimation.Tag = "BombCenterExplosion";
			bombSpreadCenterAnimation.Delay = bombexpl_anim;
			//bombSpreadCenterAnimation.Repeat = true;

			std::vector<Frame> frames;
			frames.reserve(7);
			frames.emplace_back(glm::vec2{ 0, 1 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 2, 1 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 4, 1 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 6, 1 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 4, 1 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 2, 1 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 0, 1 }, glm::vec4(1.0f));

			bombSpreadCenterAnimation.Create(spriteSheet1, { 16, 16 }, frames);
		}

		{
			bombSpreadMiddleAnimation.Tag = "BombSpreadMiddleExplosion";
			bombSpreadMiddleAnimation.Delay = bombexpl_anim;
			//bombSpreadMiddleAnimation.Repeat = true;

			std::vector<Frame> frames;
			frames.reserve(7);
			frames.emplace_back(glm::vec2{ 0, 3 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 2, 3 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 4, 3 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 6, 3 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 4, 3 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 2, 3 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 0, 3 }, glm::vec4(1.0f));

			bombSpreadMiddleAnimation.Create(spriteSheet1, { 16, 16 }, frames);
		}

		{
			bombSpreadEndAnimation.Tag = "BombEndWingExplosion";
			bombSpreadEndAnimation.Delay = bombexpl_anim;
			//bombSpreadEndAnimation.Repeat = true;

			std::vector<Frame> frames;
			frames.reserve(7);
			frames.emplace_back(glm::vec2{ 0, 5 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 2, 5 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 4, 5 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 6, 5 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 4, 5 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 2, 5 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 0, 5 }, glm::vec4(1.0f));

			bombSpreadEndAnimation.Create(spriteSheet1, { 16, 16 }, frames);

		}

		m_AnimationMap["BombTicking"] = bombTickingAnimation;
		m_AnimationMap["BombCenterExplosion"] = bombSpreadCenterAnimation;
		m_AnimationMap["BombSpreadMiddleExplosion"] = bombSpreadMiddleAnimation;
		m_AnimationMap["BombEndWingExplosion"] = bombSpreadEndAnimation;
	}


}
