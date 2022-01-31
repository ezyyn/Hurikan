#include "ResourceManager.h"

ResourceManager ResourceManager::s_Instance; // TODO: MAKE IT CLEAN

void ResourceManager::Init_Impl()
{
	std::string m_Filepath = "assets/textures/";

	Ref<Texture2D> bombAnimationSpriteSheet = Texture2D::Create(m_Filepath + "bomb_animation/ba_128x128.png");
	Ref<Texture2D> spriteSheetEntities =  Texture2D::Create(m_Filepath + "player_animation/Chr_00_0.png");
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
		m_SubTextureMap["PlayerIdle"] = SubTexture2D::CreateFromCoords(spriteSheetEntities, { 4, 7 }, { 16, 16 });
	}

	// Enemy idle
	{
		//m_SubTextureMap["RegularEnemyIdle"] = SubTexture2D::CreateFromCoords(spriteSheetEntities, { 4, 7 }, { 16, 16 });
	}
	// UI 
	{
		Ref<Texture2D> ui = Texture2D::Create(m_Filepath + "ui/options.png");

		m_SubTextureMap["UI_Continue_Text"] = SubTexture2D::CreateFromCoords(ui, { 0, 1 }, { 111, 11 });
		m_SubTextureMap["UI_NewGame_Text"] = SubTexture2D::CreateFromCoords(ui, { 0, 0 }, { 111, 11 });
		m_TextureMap["SUI_Top"] = Texture2D::Create(m_Filepath + "ui/top_ui.png");

		m_TextureMap["ArrowHead"] = Texture2D::Create(m_Filepath + "ui/arrow_head.png");
		m_TextureMap["ArrowHead2"] = Texture2D::Create(m_Filepath + "ui/arrow_head_2.png");
		m_TextureMap["Heart"] = Texture2D::Create(m_Filepath + "ui/heart.png");

		Ref<Texture2D> heads_SpriteSheet = Texture2D::Create(m_Filepath + "player_animation/heads.png");

		m_TextureMap["EquippedText"] = Texture2D::Create(m_Filepath + "equipped_Text.png"); // TODO: remove
		m_TextureMap["ScoreText"] = Texture2D::Create(m_Filepath + "ui/score.png");

		// Menu
		{
			Ref<Texture2D> equipped_options = Texture2D::Create(m_Filepath + "equipped_options.png");
			m_SubTextureMap["ClassicOpt"] = SubTexture2D::CreateFromCoords(equipped_options, { 0 , 1 }, { 125, 16 });
			m_SubTextureMap["DynamiteOpt"] = SubTexture2D::CreateFromCoords(equipped_options, { 0 , 0 }, { 125, 16 });
			m_SubTextureMap["Clock"] = SubTexture2D::CreateFromCoords(spriteSheet1, { 3, 7 }, { 16, 16 });
		}

		// Numbers
		{
			Ref<Texture2D> fontSpriteSheet = Texture2D::Create(m_Filepath + "/ui/numbers.png");
			m_SubTextureMap["0"] = SubTexture2D::CreateFromCoords(fontSpriteSheet, { 0, 0 }, { 8, 8 });
			m_SubTextureMap["1"] = SubTexture2D::CreateFromCoords(fontSpriteSheet, { 1, 0 }, { 8, 8 });
			m_SubTextureMap["2"] = SubTexture2D::CreateFromCoords(fontSpriteSheet, { 2, 0 }, { 8, 8 });
			m_SubTextureMap["3"] = SubTexture2D::CreateFromCoords(fontSpriteSheet, { 3, 0 }, { 8, 8 });
			m_SubTextureMap["4"] = SubTexture2D::CreateFromCoords(fontSpriteSheet, { 4, 0 }, { 8, 8 });
			m_SubTextureMap["5"] = SubTexture2D::CreateFromCoords(fontSpriteSheet, { 5, 0 }, { 8, 8 });
			m_SubTextureMap["6"] = SubTexture2D::CreateFromCoords(fontSpriteSheet, { 6, 0 }, { 8, 8 });
			m_SubTextureMap["7"] = SubTexture2D::CreateFromCoords(fontSpriteSheet, { 7, 0 }, { 8, 8 });
			m_SubTextureMap["8"] = SubTexture2D::CreateFromCoords(fontSpriteSheet, { 8, 0 }, { 8, 8 });
			m_SubTextureMap["9"] = SubTexture2D::CreateFromCoords(fontSpriteSheet, { 9, 0 }, { 8, 8 });
		}

		Animation
			playerLeftAnimation,
			playerUpAnimation,
			playerDownAnimation;
		{
			playerLeftAnimation.Tag = "LeftAnimation";
			playerLeftAnimation.Repeat = true;
			playerLeftAnimation.Delay = 100.0f;

			std::vector<Frame> frames;
			frames.reserve(3);
			frames.emplace_back(glm::vec2{ 0, 0 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 1, 0 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 2, 0 }, glm::vec4(1.0f));

			playerLeftAnimation.Create(heads_SpriteSheet, { 13,11 }, frames);
		}

		{
			playerUpAnimation.Tag = "UpAnimation";
			playerUpAnimation.Repeat = true;
			playerUpAnimation.Delay = 100.0f;

			std::vector<Frame> frames;
			frames.reserve(3);
			frames.emplace_back(glm::vec2{ 6, 0 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 7, 0 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 8, 0 }, glm::vec4(1.0f));

			playerUpAnimation.Create(heads_SpriteSheet, { 13,11 }, frames);
		}

		{
			playerDownAnimation.Tag = "DownAnimation";
			playerDownAnimation.Repeat = true;
			playerDownAnimation.Delay = 100.0f;

			std::vector<Frame> frames;
			frames.reserve(4);
			frames.emplace_back(glm::vec2{ 3, 0 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 4, 0 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 5, 0 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 4, 0 }, glm::vec4(1.0f));

			playerDownAnimation.Create(heads_SpriteSheet, { 13,11 }, frames);
		}

		m_AnimationMap["HeadUpAnimation"] = playerUpAnimation;
		m_AnimationMap["HeadLeftAnimation"] = playerLeftAnimation;
		m_AnimationMap["HeadDownAnimation"] = playerDownAnimation;

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
			frames.emplace_back(glm::vec2{ 1, 7 }, glm::vec4(1.0f));

			playerLeftAnimation.Create(spriteSheetEntities, { 16, 16 }, frames);
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
			frames.emplace_back(glm::vec2{ 7, 7 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 0, 6 }, glm::vec4(1.0f));

			playerUpAnimation.Create(spriteSheetEntities, { 16, 16 }, frames);
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

			playerDownAnimation.Create(spriteSheetEntities, { 16, 16 }, frames);
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

			playerDeadAnimation.Create(spriteSheetEntities, { 16, 16 }, frames);
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

	// Grid animations
	{
		Animation wallBreakAnimation;
		{
			wallBreakAnimation.Tag = "WallBreakAnimation";
			wallBreakAnimation.Delay = 250.0f;

			std::vector<Frame> frames;
			frames.reserve(6);
			frames.emplace_back(glm::vec2{ 2, 6 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 3, 6 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 4, 6 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 5, 6 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 6, 6 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 7, 6 }, glm::vec4(1.0f));

			wallBreakAnimation.Create(spriteSheet1, { 16, 16 }, frames);

		}
		m_AnimationMap["WallBreakAnimation"] = wallBreakAnimation;
	}
	// Enemy animations
	{

	}
}
