#include "ResourceManager.hpp"

ResourceManager ResourceManager::s_Instance; // TODO: MAKE IT CLEAN

void ResourceManager::Init_Impl()
{
	std::string m_Filepath = "assets/textures/";

	Ref<Texture2D> entities_16x16 =  Texture2D::Create(m_Filepath + "entities_16x16.png");
	Ref<Texture2D> tileset_16x16 = Texture2D::Create(m_Filepath + "tileset_16x16.png");

	// Grid
	{
		m_SubTextureMap["Wall"] = SubTexture2D::CreateFromCoords(tileset_16x16, { 0, 0 }, { 16, 16 });
		m_SubTextureMap["BreakableWall"] = SubTexture2D::CreateFromCoords(tileset_16x16, { 1, 0 }, { 16, 16 });
		m_SubTextureMap["ExitDoor"] = SubTexture2D::CreateFromCoords(tileset_16x16, { 3, 0 }, { 16, 16 });
		//m_SubTextureMap["BombUpgrade"] = SubTexture2D::CreateFromCoords(spriteSheet1, { 4, 0 }, { 16, 16 });
		m_SubTextureMap["PowerUpgrade"] = SubTexture2D::CreateFromCoords(tileset_16x16, { 4, 0 }, { 16, 16 });
		m_SubTextureMap["SpeedUpgrade"] = SubTexture2D::CreateFromCoords(tileset_16x16, { 5, 0 }, { 16, 16 });
		m_SubTextureMap["Key"] = SubTexture2D::CreateFromCoords(tileset_16x16, { 7, 0 }, { 16, 16 });
		m_SubTextureMap["BombCount"] = SubTexture2D::CreateFromCoords(tileset_16x16, { 0, 7 }, { 16, 16 });
		m_SubTextureMap["BombCountUpgrade"] = SubTexture2D::CreateFromCoords(tileset_16x16, { 7, 7 }, { 16, 16 });
	}
	// Background
	{
		m_TextureMap["Background"] = Texture2D::Create(m_Filepath + "ingame_background2.png");
		m_TextureMap["MainMenu_Background"] = Texture2D::Create(m_Filepath + "mainmenu_background.png");
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
		
		// Simple UI
		{
			//m_TextureMap["SUI_Top"] = Texture2D::Create(m_Filepath + "ui/top_ui.png");
			m_SubTextureMap["Clock"] = SubTexture2D::CreateFromCoords(tileset_16x16, { 3, 7 }, { 16, 16 });
		}
		{
			Ref<Texture2D> ui_32x32 = Texture2D::Create(m_Filepath + "ui/ui_32x32.png");

			m_SubTextureMap["ArrowHead"] = SubTexture2D::CreateFromCoords(ui_32x32, { 0, 0 }, { 32, 32 });
			m_SubTextureMap["Heart"] = SubTexture2D::CreateFromCoords(ui_32x32, { 1, 0 }, { 32, 32 });
			m_SubTextureMap["Background_UnderText"] = SubTexture2D::CreateFromCoords(ui_32x32, { 2, 0 }, { 32, 32 });
		}

		m_TextureMap["ScoreText"] = Texture2D::Create(m_Filepath + "ui/score.png");

		// Menu
		{
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

			// Pause Menu
			{
				auto pausemenu = Texture2D::Create(m_Filepath + "ui/PauseMenuUpdate.png");
				m_SubTextureMap["PauseMenu"] = SubTexture2D::CreateFromCoords(pausemenu, { 0, 0 }, {64, 64});
			}
			// Main Menu Settings
			{
				m_TextureMap["MainMenuSettings"] = Texture2D::Create(m_Filepath + "ui/settings_mainmenu.png");
			}
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
			m_SubTextureMap["LevelText"] = SubTexture2D::CreateFromCoords(entities_16x16, { 0, 0 }, { 62, 16 });
		}
	}

	// Animations // 
	// Player
	{
		// Player Idle
		{
			Animation idle;
			idle.Tag = "PlayerIdle";
			idle.Repeat = true;
			idle.Delay = 100.0f;
			std::vector<Frame> frames;
			frames.emplace_back(glm::vec2{ 4, 7 }, glm::vec4(1.0f));
			idle.Create(entities_16x16, { 16, 16 }, frames);

			m_AnimationMap["PlayerIdleAnimation"] = idle;
		}

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

			playerLeftAnimation.Create(entities_16x16, { 16, 16 }, frames);
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

			playerUpAnimation.Create(entities_16x16, { 16, 16 }, frames);
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

			playerDownAnimation.Create(entities_16x16, { 16, 16 }, frames);
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

			playerDeadAnimation.Create(entities_16x16, { 16, 16 }, frames);
		}

		m_AnimationMap["PlayerLeftAnimation"] = playerLeftAnimation;
		m_AnimationMap["PlayerUpAnimation"] = playerUpAnimation;
		m_AnimationMap["PlayerDownAnimation"] = playerDownAnimation;
		m_AnimationMap["PlayerDeadAnimation"] = playerDeadAnimation;
	}

	// Player Head Animations
	{
		Ref<Texture2D> heads_13x11 = Texture2D::Create(m_Filepath + "ui/heads_13x11.png");
		m_SubTextureMap["HeadIdle"] = SubTexture2D::CreateFromCoords(heads_13x11, { 4,0 }, { 13,11 });

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

			playerLeftAnimation.Create(heads_13x11, { 13,11 }, frames);
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

			playerUpAnimation.Create(heads_13x11, { 13,11 }, frames);
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

			playerDownAnimation.Create(heads_13x11, { 13,11 }, frames);
		}

		m_AnimationMap["HeadUpAnimation"] = playerUpAnimation;
		m_AnimationMap["HeadLeftAnimation"] = playerLeftAnimation;
		m_AnimationMap["HeadDownAnimation"] = playerDownAnimation;
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
			frames.reserve(4);
			frames.emplace_back(glm::vec2{ 0, 4 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 1, 4 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 2, 4 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 1, 4 }, glm::vec4(1.0f));

			baloonAnimation.Create(entities_16x16, { 16, 16 }, frames);
		}
		{
			baloonDeadAnimation.Tag = "BaloonDead";
			baloonDeadAnimation.Repeat = false;
			baloonDeadAnimation.Delay = 400.0f;

			std::vector<Frame> frames;
			frames.reserve(5);
			frames.emplace_back(glm::vec2{ 3, 4 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 4, 5 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 5, 5 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 6, 5 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 7, 5 }, glm::vec4(1.0f));

			baloonDeadAnimation.Create(entities_16x16, { 16, 16 }, frames);
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
			frames.reserve(4);
			frames.emplace_back(glm::vec2{ 4, 3 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 5, 3 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 6, 3 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 5, 3 }, glm::vec4(1.0f));
			angryBallMoving.Create(entities_16x16, { 16, 16 }, frames);

			m_AnimationMap["AngryBallMoveAnimation"] = angryBallMoving;
		}
		{
			Animation angryBallDying;
			angryBallDying.Tag = "AngryBallDead";
			angryBallDying.Repeat = false;
			angryBallDying.Delay = 400.0f;

			std::vector<Frame> frames;
			frames.reserve(5);
			frames.emplace_back(glm::vec2{ 7, 3 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 4, 5 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 5, 5 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 6, 5 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 7, 5 }, glm::vec4(1.0f));
			angryBallDying.Create(entities_16x16, { 16, 16 }, frames);

			m_AnimationMap["AngryBallDeadAnimation"] = angryBallDying;
		}

		{
			Animation blueIceCreamMoving;
			blueIceCreamMoving.Tag = "BlueIceCreamMove";
			blueIceCreamMoving.Repeat = true;
			blueIceCreamMoving.Delay = 100.0f;

			std::vector<Frame> frames;
			frames.reserve(4);
			frames.emplace_back(glm::vec2{ 4, 4 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 5, 4 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 6, 4 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 5, 4 }, glm::vec4(1.0f));
			blueIceCreamMoving.Create(entities_16x16, { 16, 16 }, frames);

			m_AnimationMap["BlueIceCreamMoveAnimation"] = blueIceCreamMoving;
		}
		{
			Animation blueIceCreamDead;
			blueIceCreamDead.Tag = "BlueIceCreamDead";
			blueIceCreamDead.Repeat = false;
			blueIceCreamDead.Delay = 400.0f;

			std::vector<Frame> frames;
			frames.reserve(5);
			frames.emplace_back(glm::vec2{ 7, 4 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 4, 5 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 5, 5 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 6, 5 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 7, 5 }, glm::vec4(1.0f));
			blueIceCreamDead.Create(entities_16x16, { 16, 16 }, frames);

			m_AnimationMap["BlueIceCreamDeadAnimation"] = blueIceCreamDead;
		}

		// Boss
		{

		}
	}

	// Bomb animations
	{

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

			bombTickingAnimation.Create(tileset_16x16, { 16, 16 }, frames);
		}
		constexpr float bombexpl_anim = 60.0f;

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

			bombSpreadCenterAnimation.Create(tileset_16x16, { 16, 16 }, frames);
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

			bombSpreadMiddleAnimation.Create(tileset_16x16, { 16, 16 }, frames);
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

			bombSpreadEndAnimation.Create(tileset_16x16, { 16, 16 }, frames);
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
			wallBreakAnimation.Delay = 100.0f;

			std::vector<Frame> frames;
			frames.reserve(6);
			frames.emplace_back(glm::vec2{ 2, 6 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 3, 6 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 4, 6 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 5, 6 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 6, 6 }, glm::vec4(1.0f));
			frames.emplace_back(glm::vec2{ 7, 6 }, glm::vec4(1.0f));

			wallBreakAnimation.Create(tileset_16x16, { 16, 16 }, frames);

		}
		m_AnimationMap["WallBreakAnimation"] = wallBreakAnimation;
	}
}
