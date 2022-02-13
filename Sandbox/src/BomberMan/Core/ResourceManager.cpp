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
		m_SubTextureMap["ExitDoor"] = SubTexture2D::CreateFromCoords(spriteSheet1, { 3, 0 }, { 16, 16 });
		m_SubTextureMap["MoreBombUpgrade"] = SubTexture2D::CreateFromCoords(spriteSheet1, { 4, 0 }, { 16, 16 });
		m_SubTextureMap["MoreSpeedUpgrade"] = SubTexture2D::CreateFromCoords(spriteSheet1, { 5, 0 }, { 16, 16 });
	}
	// Background
	{
		m_TextureMap["Background"] = Texture2D::Create(m_Filepath + "background.png");
		m_TextureMap["Background2"] = Texture2D::Create(m_Filepath + "background2.png");
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

		m_SubTextureMap["UI_Continue_Text"] = SubTexture2D::CreateFromCoords(ui, { 0, 4 }, { 111, 12 });
		m_SubTextureMap["UI_NewGame_Text"] = SubTexture2D::CreateFromCoords(ui, { 0, 3 }, { 111, 12 });
		m_SubTextureMap["UI_Settings_Text"] = SubTexture2D::CreateFromCoords(ui, { 0, 2 }, { 111, 12 });
		m_SubTextureMap["UI_Stats_Text"] = SubTexture2D::CreateFromCoords(ui, { 0, 1 }, { 111, 12 });
		m_SubTextureMap["UI_Exit_Text"] = SubTexture2D::CreateFromCoords(ui, { 0, 0 }, { 111, 12 });
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

			// Logo
			{
				Ref<Texture2D> logo_animation = Texture2D::Create(m_Filepath + "ui/logo_animation.png");

				Animation logo;
				logo.Tag = "LogoAnimation";
				logo.Repeat = true;
				logo.Delay = 100.0f;

				std::vector<Frame> frames;
				frames.reserve(3);
				frames.emplace_back(glm::vec2{ 0, 0 }, glm::vec4(1.0f));
				frames.emplace_back(glm::vec2{ 1, 0 }, glm::vec4(1.0f));
				frames.emplace_back(glm::vec2{ 2, 0 }, glm::vec4(1.0f));
				frames.emplace_back(glm::vec2{ 1, 0 }, glm::vec4(1.0f));

				logo.Create(logo_animation, { 256, 64 }, frames);

				m_AnimationMap["LogoAnimation"] = logo;
			}
			// Background
			{
				m_TextureMap["Background_UnderText"] = Texture2D::Create(m_Filepath + "/ui/Background_MainMenu.png");
			}

			// Pause Menu
			auto pausemenu = Texture2D::Create(m_Filepath + "ui/PauseMenu.png");

			m_SubTextureMap["PauseMenu"] = SubTexture2D::CreateFromCoords(pausemenu, { 0, 0 }, {64, 64});
		}

		// Numbers
		{
			Ref<Texture2D> numbersSpriteSheet = Texture2D::Create(m_Filepath + "/ui/numbers.png");
			m_SubTextureMap["0"] = SubTexture2D::CreateFromCoords(numbersSpriteSheet, { 0, 0 }, { 9, 9 });
			m_SubTextureMap["1"] = SubTexture2D::CreateFromCoords(numbersSpriteSheet, { 1, 0 }, { 9, 9 });
			m_SubTextureMap["2"] = SubTexture2D::CreateFromCoords(numbersSpriteSheet, { 2, 0 }, { 9, 9 });
			m_SubTextureMap["3"] = SubTexture2D::CreateFromCoords(numbersSpriteSheet, { 3, 0 }, { 9, 9 });
			m_SubTextureMap["4"] = SubTexture2D::CreateFromCoords(numbersSpriteSheet, { 4, 0 }, { 9, 9 });
			m_SubTextureMap["5"] = SubTexture2D::CreateFromCoords(numbersSpriteSheet, { 5, 0 }, { 9, 9 });
			m_SubTextureMap["6"] = SubTexture2D::CreateFromCoords(numbersSpriteSheet, { 6, 0 }, { 9, 9 });
			m_SubTextureMap["7"] = SubTexture2D::CreateFromCoords(numbersSpriteSheet, { 7, 0 }, { 9, 9 });
			m_SubTextureMap["8"] = SubTexture2D::CreateFromCoords(numbersSpriteSheet, { 8, 0 }, { 9, 9 });
			m_SubTextureMap["9"] = SubTexture2D::CreateFromCoords(numbersSpriteSheet, { 9, 0 }, { 9, 9 });

			Ref<Texture2D> numbersSpriteSheet2 = Texture2D::Create(m_Filepath + "/font_spritesheet.png");
			m_SubTextureMap["m0"] = SubTexture2D::CreateFromCoords(numbersSpriteSheet2, { 1, 7 }, { 20, 20 });
			m_SubTextureMap["m1"] = SubTexture2D::CreateFromCoords(numbersSpriteSheet2, { 2, 6 }, { 20, 20 });
			m_SubTextureMap["m2"] = SubTexture2D::CreateFromCoords(numbersSpriteSheet2, { 3, 6 }, { 20, 20 });
			m_SubTextureMap["m3"] = SubTexture2D::CreateFromCoords(numbersSpriteSheet2, { 4, 6 }, { 20, 20 });
			m_SubTextureMap["m4"] = SubTexture2D::CreateFromCoords(numbersSpriteSheet2, { 5, 6 }, { 20, 20 });
			m_SubTextureMap["m5"] = SubTexture2D::CreateFromCoords(numbersSpriteSheet2, { 6, 6 }, { 20, 20 });
			m_SubTextureMap["m6"] = SubTexture2D::CreateFromCoords(numbersSpriteSheet2, { 7, 6 }, { 20, 20 });
			m_SubTextureMap["m7"] = SubTexture2D::CreateFromCoords(numbersSpriteSheet2, { 8, 6 }, { 20, 20 });
			m_SubTextureMap["m8"] = SubTexture2D::CreateFromCoords(numbersSpriteSheet2, { 9, 6 }, { 20, 20 });
			m_SubTextureMap["m9"] = SubTexture2D::CreateFromCoords(numbersSpriteSheet2, { 10, 6 },{ 20, 20 });
		}	
		// Level text
		{
			m_SubTextureMap["LevelText"] = SubTexture2D::CreateFromCoords(spriteSheetEntities, { 0, 0 }, { 62, 16 });
		}

		// Key
		{
			m_SubTextureMap["Key"] = SubTexture2D::CreateFromCoords(spriteSheet1, { 7, 0 }, { 16, 16 });
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

	// Enemy animations
	{
		// Regular
		Animation baloonAnimation, baloonDeadAnimation;
		{
			baloonAnimation.Tag = "BaloonMoving";
			baloonAnimation.Repeat = true;
			baloonAnimation.Delay = 100.0f;

			std::vector<Frame> frames;
			frames.reserve(3);
			frames.emplace_back(glm::vec2{ 0, 4 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 1, 4 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 2, 4 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 1, 4 }, glm::vec4(1.0f));

			baloonAnimation.Create(spriteSheetEntities, { 16, 16 }, frames);
		}
		{
			baloonDeadAnimation.Tag = "BaloonDead";
			baloonDeadAnimation.Repeat = false;
			baloonDeadAnimation.Delay = 400.0f;

			std::vector<Frame> frames;
			frames.reserve(3);
			frames.emplace_back(glm::vec2{ 3, 4 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 4, 5 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 5, 5 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 6, 5 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 7, 5 }, glm::vec4(1.0f));

			baloonDeadAnimation.Create(spriteSheetEntities, { 16, 16 }, frames);
		}

		m_AnimationMap["BaloonMovingAnimation"] = baloonAnimation;
		m_AnimationMap["BaloonDeathAnimation"] = baloonDeadAnimation;

		// Rare
		{
			Animation angryBallMoving;
			angryBallMoving.Tag = "AngryBallMove";
			angryBallMoving.Repeat = true;
			angryBallMoving.Delay = 100.0f;

			std::vector<Frame> frames;
			frames.reserve(3);
			frames.emplace_back(glm::vec2{ 4, 3 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 5, 3 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 6, 3 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 5, 3 }, glm::vec4(1.0f));
			angryBallMoving.Create(spriteSheetEntities, { 16, 16 }, frames);

			m_AnimationMap["AngryBallMoveAnimation"] = angryBallMoving;
		}
		{
			Animation angryBallDying;
			angryBallDying.Tag = "AngryBallDead";
			angryBallDying.Repeat = false;
			angryBallDying.Delay = 400.0f;

			std::vector<Frame> frames;
			frames.reserve(3);
			frames.emplace_back(glm::vec2{ 7, 3 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 4, 5 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 5, 5 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 6, 5 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 7, 5 }, glm::vec4(1.0f));
			angryBallDying.Create(spriteSheetEntities, { 16, 16 }, frames);

			m_AnimationMap["AngryBallDeadAnimation"] = angryBallDying;
		}
	}

	// Bomb animations
	{
		constexpr float bombexpl_anim = 75.0f;

		Animation bombTickingAnimation;
		{
			bombTickingAnimation.Tag = "BombTicking";
			bombTickingAnimation.Repeat = true;
			bombTickingAnimation.Delay = 150.0f;

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
	/*// Moving bomb
	{ 
		Ref<Texture2D> moving_bomb_spritesheet = Texture2D::Create(m_Filepath + "/ui/numbers.png");
		m_SubTextureMap["0"] = SubTexture2D::CreateFromCoords(moving_bomb_spritesheet, { 0, 0 }, { 9, 9 });

		Animation
			bombSpreadCenterAnimation,
			bombSpreadMiddleAnimation,
			bombSpreadEndAnimation;

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
	}*/

	// Grid animations
	{
		Animation wallBreakAnimation;
		{
			wallBreakAnimation.Tag = "WallBreakAnimation";
			wallBreakAnimation.Delay = 100.0f;

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
}
