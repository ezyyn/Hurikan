#include "Bomb.h"

#include "../sceneobjects/Player.h"
#include "../sceneobjects/Grid.h"

#include <box2d/b2_body.h>

extern Player* g_Player;
extern Grid* g_GameGrid;
extern Scene* g_GameScene;

static std::vector<Bomb> s_PlacedBombs;

////////////////////////////////////////////////////////////////////
///////////////////////  Bomb Manager //////////////////////////////
////////////////////////////////////////////////////////////////////
void BombManager::Init(const Level& level)
{
	m_CurrentLevel = level;
}

/// <summary>
/// Calling this method results in placing a new bomb
/// </summary>
/// <param name="bombprops">Properties to a fresh new born bomb</param>
/// <returns>Returns true if a new bomb was placed otherwise false</returns>
bool BombManager::PlaceBomb(const BombProperties& bombprops)
{
	// Checking if a new bomb is not on a top of another bomb
	for (auto& bomb : s_PlacedBombs)
	{
		if (bomb.Position().x == bombprops.StartPosition.x 
			&& bomb.Position().y == bombprops.StartPosition.y)
		{
			return false;
		}
	}

	// Updating the grid
	g_GameGrid->BombChanged();

	// Creating a new bomb
	s_PlacedBombs.push_back({ bombprops });
	return true;
}

void BombManager::OnUpdate(Timestep ts)
{
	for (size_t i = 0; i < s_PlacedBombs.size(); i++)
	{
		bool exploded = s_PlacedBombs[i].Tick(ts);
		if (exploded)
		{
			s_PlacedBombs.erase(s_PlacedBombs.begin() + i);
			// Updating the grid again
			g_GameGrid->BombChanged();
		}
	}
}

///////////////////////////////////////////////////////////////////
///////////////////////  Bomb  ////////////////////////////////////
///////////////////////////////////////////////////////////////////

Bomb::Bomb(const BombProperties& bombprops) : m_Properties(bombprops)
{
	Deploy();
}

void Bomb::Deploy()
{
	m_Handle = g_GameScene->CreateEntityWithDrawOrder(1, "BombEntity");

	m_Handle.GetComponent<TransformComponent>().Translation.x = m_Properties.StartPosition.x;
	m_Handle.GetComponent<TransformComponent>().Translation.y = m_Properties.StartPosition.y;

	m_BombPlaced = g_GameGrid->WherePlayerAt();

	////HU_INFO("Bomb Placed(grid): {0} {1}" , m_BombPlaced->Handle.Transform().Translation.x, m_BombPlaced->Handle.Transform().Translation.y )

	m_BombPlaced->Handle.GetComponent<EntityTypeComponent>().Type = EntityType::BOMB;
	m_BombPlaced->Obstacle = true;

	m_Handle.AddCustomComponent<EntityTypeComponent>().Type = EntityType::BOMB;

	m_Handle.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f));
	m_Handle.AddComponent<Rigidbody2DComponent>().Enabled = false;
	m_Handle.AddComponent<BoxCollider2DComponent>();
	g_GameScene->CreateBody(m_Handle);
	
	m_BombAnimator.SetTarget(m_Handle);

	float bombexpl_anim = 100.0f;

	AnimationBlock bombTickingAnimation;
	{
		bombTickingAnimation.Tag = "BombTicking";
		bombTickingAnimation.SpriteSize = { 16,16 };
		// TODO: create ResouceManager     VVV
		bombTickingAnimation.SpriteSheet = Texture2D::Create("assets/textures/tileset_16x16.png");
		bombTickingAnimation.Repeat = true;

		std::vector<FrameSpecification> framesSpec;
		framesSpec.push_back({ { 0, 7 }, 300.0f });
		framesSpec.push_back({ { 1, 7 }, 300.0f });
		framesSpec.push_back({ { 2, 7 }, 300.0f });
		framesSpec.push_back({ { 1, 7 }, 300.0f });

		bombTickingAnimation.Load(framesSpec);
	}
	{
		m_BombSpreadCenterAnimation.Tag = "BombCenterExplosion";
		m_BombSpreadCenterAnimation.SpriteSize = { 16,16 };
		m_BombSpreadCenterAnimation.SpriteSheet = Texture2D::Create("assets/textures/tileset_16x16.png");

		std::vector<FrameSpecification> framesSpec;
		framesSpec.push_back({ { 0, 1 }, bombexpl_anim });
		framesSpec.push_back({ { 2, 1 }, bombexpl_anim });
		framesSpec.push_back({ { 4, 1 }, bombexpl_anim });
		framesSpec.push_back({ { 6, 1 }, bombexpl_anim });
		framesSpec.push_back({ { 4, 1 }, bombexpl_anim });
		framesSpec.push_back({ { 2, 1 }, bombexpl_anim });
		framesSpec.push_back({ { 0, 1 }, bombexpl_anim });
		framesSpec.push_back({ {}, bombexpl_anim, true });

		m_BombSpreadCenterAnimation.Load(framesSpec);
	}

	{
		m_BombSpreadMiddleAnimation.Tag = "BombWingExplosion";
		m_BombSpreadMiddleAnimation.SpriteSize = { 16,16 };
		m_BombSpreadMiddleAnimation.SpriteSheet = Texture2D::Create("assets/textures/tileset_16x16.png");

		std::vector<FrameSpecification> framesSpec;
		framesSpec.push_back({ { 0, 3 }, bombexpl_anim });
		framesSpec.push_back({ { 2, 3 }, bombexpl_anim });
		framesSpec.push_back({ { 4, 3 }, bombexpl_anim });
		framesSpec.push_back({ { 6, 3 }, bombexpl_anim });
		framesSpec.push_back({ { 4, 3 }, bombexpl_anim });
		framesSpec.push_back({ { 2, 3 }, bombexpl_anim });
		framesSpec.push_back({ { 0, 3 }, bombexpl_anim });
		framesSpec.push_back({ {}, bombexpl_anim, true });

		m_BombSpreadMiddleAnimation.Load(framesSpec);
	}

	{
		m_BombSpreadEndAnimation.Tag = "BombEndWingExplosion";
		m_BombSpreadEndAnimation.SpriteSize = { 16,16 };
		m_BombSpreadEndAnimation.SpriteSheet = Texture2D::Create("assets/textures/tileset_16x16.png");

		std::vector<FrameSpecification> framesSpec;
		framesSpec.push_back({ { 0, 5 }, bombexpl_anim });
		framesSpec.push_back({ { 2, 5 }, bombexpl_anim });
		framesSpec.push_back({ { 4, 5 }, bombexpl_anim });
		framesSpec.push_back({ { 6, 5 }, bombexpl_anim });
		framesSpec.push_back({ { 4, 5 }, bombexpl_anim });
		framesSpec.push_back({ { 2, 5 }, bombexpl_anim });
		framesSpec.push_back({ { 0, 5 }, bombexpl_anim });
		framesSpec.push_back({ {}, bombexpl_anim, true });

		m_BombSpreadEndAnimation.Load(framesSpec);
	}
	m_BombAnimator.Add(bombTickingAnimation);
	m_BombAnimator.Switch("BombTicking");

	m_Properties.State = BombState::TICKING;
}

bool Bomb::Tick(Timestep ts)
{
	if (m_Properties.State == BombState::TICKING)
	{
		m_Properties.Time += ts;
		m_BombAnimator.OnUpdate(ts);

		float distance = glm::distance(g_Player->Position2(), m_Handle.Transform().Translation);
		if (distance > 0.85f)// TODO: doladit
		{
			b2Body* body = (b2Body*)m_Handle.GetComponent<Rigidbody2DComponent>().RuntimeBody;
			body->SetEnabled(true);
			//HU_INFO(distance);
		}
	}
	if (m_Properties.Time >= m_Properties.ExplosionTime && m_Properties.State == BombState::TICKING || m_ChainExplosion)
	{
		m_ChainExplosion = false;

		m_Properties.State = BombState::EXPLOSION;

		m_Handle.GetComponent<SpriteRendererComponent>().Color = glm::vec4(0.0f);

		WingInit();
		return false;
	}

	if (m_Properties.State == BombState::EXPLOSION)
	{
		for (auto& wings : m_Properties.SpreadEntities)
		{
			for (auto& spread : wings)
			{
				auto& position = spread.Transform().Translation;

				float deltaX = glm::abs(g_Player->Position().x - position.x);
				float deltaY = glm::abs(g_Player->Position().y - position.y);

				if (deltaX < 0.9f && deltaY < 0.9f)
				{
					// Bomb explosion => insta-dead
					g_Player->FinalHit();
					goto AAAA;
				}
			}
		}
	AAAA:
		// if all animations are completed call destroy function
		m_AnmCmplt = true;
		for (auto& wings : m_Properties.SpreadEntities)
		{
			for (auto& entity : wings)
			{
				auto& frameanimator = entity.GetComponent<FrameAnimator>();
				entity.GetComponent<SpriteRendererComponent>().Color = glm::vec4(1.0f);
				if (frameanimator.OnUpdate(ts))
				{
					////HU_INFO(entity.GetComponent<TagComponent>().Tag);
					m_AnmCmplt = false;
				}
			}
		}

		if (m_AnmCmplt)
		{
			DestroyItSelf();
			return true;
		}
	}

	return false;
}
void Bomb::DestroyItSelf()
{
	m_BombPlaced->Obstacle = false;
	m_BombPlaced->Handle.GetComponent<EntityTypeComponent>().Type = EntityType::TILE_EMPTY;

	// Destroys entities from the scene
	g_GameScene->DestroyEntity(m_Handle);

	for (auto& wings: m_Properties.SpreadEntities)
	{
		for (auto& spread : wings)
		{
			g_GameScene->DestroyEntity(spread);
		}
	}
}

void Bomb::WingInit()
{
#define FUCK 1
#if FUCK
	int index_x = static_cast<float>(m_BombPlaced->Position.x);
	int index_y = static_cast<float>(m_BombPlaced->Position.y);

	for (int j = 1; j <= m_Properties.Reach; j++)
	{
		if (index_x - j >= 0)
		{
			GridNode* leftspread = &g_GameGrid->Get()[index_y][index_x - j];

			switch (leftspread->Handle.GetComponent<EntityTypeComponent>().Type)
			{
			case EntityType::TILE_WALL:
				goto RIGHT;
				break;
			case EntityType::TILE_BOX:
			{
				leftspread->Handle.GetComponent<FrameAnimator>().Switch("WallBreakAnimation");
				leftspread->Handle.GetComponent<EntityTypeComponent>().Type = EntityType::TILE_EMPTY;
				goto RIGHT;
				break;
			}
			case EntityType::TILE_EMPTY:
			{
				Entity spread = g_GameScene->CreateEntityWithDrawOrder(2, "WingEntityLeft");

				spread.AddComponent<SpriteRendererComponent>(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
				spread.AddCustomComponent<EntityTypeComponent>().Type = EntityType::BOMB_SPREAD_EXPLOSION;
				spread.GetComponent<TransformComponent>().Translation = leftspread->Handle.Transform().Translation;
				spread.GetComponent<TransformComponent>().Rotation.z = glm::radians(90.0f);
				m_LeftWing.push_back(spread);

				auto& fa = spread.AddCustomComponent<FrameAnimator>();
				fa.SetTarget(spread);
				if (j == m_Properties.Reach)
				{
					fa.Add(m_BombSpreadEndAnimation);
					fa.Switch("BombEndWingExplosion");
				}
				else 
				{
					fa.Add(m_BombSpreadMiddleAnimation);
					fa.Switch("BombWingExplosion");
				}

				break;
			}
			case EntityType::BOMB:
			{
				// Check for chain reaction
				for (auto& other : s_PlacedBombs)
				{
					if (other.m_Properties.State != BombState::EXPLOSION &&
						other.m_BombPlaced->Handle.Transform().Translation == leftspread->Handle.Transform().Translation)
					{
						other.Explode();
					}
				}
				goto RIGHT;
				break;
			}
			default:
				break;
			}
		}
	}
RIGHT:
	//HU_INFO("Right");
	for (int j = 1; j <= m_Properties.Reach; j++)
	{
		if (index_x + j < m_Properties.CurrentLevel.Width)
		{
			GridNode* rightspread = &g_GameGrid->Get()[index_y][index_x + j];
			//HU_INFO(index_x + j);
			switch (rightspread->Handle.GetComponent<EntityTypeComponent>().Type)
			{
			case EntityType::TILE_WALL:
				goto UP;
				break;
			case EntityType::TILE_BOX:
			{
				rightspread->Handle.GetComponent<FrameAnimator>().Switch("WallBreakAnimation");
				rightspread->Handle.GetComponent<EntityTypeComponent>().Type = EntityType::TILE_EMPTY;
				goto UP;
				break;
			}
			case EntityType::TILE_EMPTY:
			{
				Entity spread = g_GameScene->CreateEntityWithDrawOrder(2, "WingEntityRight");
				spread.AddComponent<SpriteRendererComponent>(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
				//spread.AddCustomComponent<EntityTypeComponent>().Type = EntityType::BOMB_SPREAD_EXPLOSION;
				spread.GetComponent<TransformComponent>().Translation.x = rightspread->Handle.Transform().Translation.x;
				spread.GetComponent<TransformComponent>().Translation.y = rightspread->Handle.Transform().Translation.y;
				spread.GetComponent<TransformComponent>().Rotation.z = glm::radians(270.0f);
				m_RightWing.push_back(spread);
				//HU_INFO(" [{0}, {1}] ", spread.Transform().Translation.x, spread.Transform().Translation.y)

				auto& fa = spread.AddCustomComponent<FrameAnimator>();
				fa.SetTarget(spread);
				if (j == m_Properties.Reach)
				{
					fa.Add(m_BombSpreadEndAnimation);
					fa.Switch("BombEndWingExplosion");
				}
				else
				{
					fa.Add(m_BombSpreadMiddleAnimation);
					fa.Switch("BombWingExplosion");
				}

				break;
			}
			case EntityType::BOMB:
			{

				for (auto& other : s_PlacedBombs)
				{
					if (other.m_Properties.State != BombState::EXPLOSION &&
						other.m_BombPlaced->Handle.Transform().Translation == rightspread->Handle.Transform().Translation)
					{
						other.Explode();
					}
				}
				goto UP;
				break;
			}
			default:
				break;
			}
		}
	}
UP:
	//HU_INFO("---------------------");

	//HU_INFO("Up");
	for (int j = 1; j <= m_Properties.Reach; j++)
	{
		if (index_y - j >= 0)
		{
			GridNode* upspread = &g_GameGrid->Get()[index_y - j][index_x];

			switch (upspread->Handle.GetComponent<EntityTypeComponent>().Type)
			{
			case EntityType::TILE_WALL:
			{
				goto DOWN;
				break;
			}
			case EntityType::TILE_BOX:
			{
				upspread->Handle.GetComponent<FrameAnimator>().Switch("WallBreakAnimation");
				upspread->Handle.GetComponent<EntityTypeComponent>().Type = EntityType::TILE_EMPTY;
				goto DOWN;
				break;
			}
			case EntityType::TILE_EMPTY:
			{
				
				Entity spread = g_GameScene->CreateEntityWithDrawOrder(2, "WingEntityUp");
				spread.AddComponent<SpriteRendererComponent>(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
				spread.AddCustomComponent<EntityTypeComponent>().Type = EntityType::BOMB_SPREAD_EXPLOSION;
				spread.GetComponent<TransformComponent>().Translation = upspread->Handle.Transform().Translation;
				spread.GetComponent<TransformComponent>().Rotation.z = 0.0;
				m_UpWing.push_back(spread);

				//HU_INFO(" [{0}, {1}] ", spread.Transform().Translation.x, spread.Transform().Translation.y)
				auto& fa = spread.AddCustomComponent<FrameAnimator>();
				fa.SetTarget(spread);
				if (j == m_Properties.Reach)
				{
					fa.Add(m_BombSpreadEndAnimation);
					fa.Switch("BombEndWingExplosion");
				}
				else
				{
					fa.Add(m_BombSpreadMiddleAnimation);
					fa.Switch("BombWingExplosion");
				}
				break;
			}

			case EntityType::BOMB:
			{
				for (auto& other : s_PlacedBombs)
				{
					if (other.m_Properties.State != BombState::EXPLOSION &&
						other.m_BombPlaced->Handle.Transform().Translation == upspread->Handle.Transform().Translation)
					{
						other.Explode();
					}
				}
				goto DOWN;
				break;
			}
			default:
				break;
			}
		}
	}

	DOWN:
	//HU_INFO("Down");
	for (int j = 1; j <= m_Properties.Reach; j++)
	{
		if (index_y + j < m_Properties.CurrentLevel.Height)
		{
			GridNode* downspread = &g_GameGrid->Get()[index_y + j][index_x];
			switch (downspread->Handle.GetComponent<EntityTypeComponent>().Type)
			{
			case EntityType::TILE_WALL:
				goto BREAKYBREAK;
				break;
			case EntityType::TILE_BOX:
			{
				downspread->Handle.GetComponent<FrameAnimator>().Switch("WallBreakAnimation");
				downspread->Handle.GetComponent<EntityTypeComponent>().Type = EntityType::TILE_EMPTY;
				goto BREAKYBREAK;
				break;
			}
			case EntityType::TILE_EMPTY:
			{
				Entity spread = g_GameScene->CreateEntityWithDrawOrder(2, "WingEntityDown");

				spread.AddComponent<SpriteRendererComponent>(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
				spread.AddCustomComponent<EntityTypeComponent>().Type = EntityType::BOMB_SPREAD_EXPLOSION;
				spread.GetComponent<TransformComponent>().Translation = downspread->Handle.Transform().Translation;
				spread.GetComponent<TransformComponent>().Rotation.z = glm::radians(180.0f);
				m_DownWing.push_back(spread);

				//HU_INFO(" [{0}, {1}] ", spread.Transform().Translation.x, spread.Transform().Translation.y)

				auto& fa = spread.AddCustomComponent<FrameAnimator>();
				fa.SetTarget(spread);
				if (j == m_Properties.Reach)
				{
					fa.Add(m_BombSpreadEndAnimation);
					fa.Switch("BombEndWingExplosion");
				}
				else
				{
					fa.Add(m_BombSpreadMiddleAnimation);
					fa.Switch("BombWingExplosion");
				}
				break;
			}

			case EntityType::BOMB:
			{
				for (auto& other : s_PlacedBombs)
				{
					if (other.m_Properties.State != BombState::EXPLOSION &&
						other.m_BombPlaced->Handle.Transform().Translation == downspread->Handle.Transform().Translation)
					{
						other.Explode();
					}
				}
				goto BREAKYBREAK;
				break;
			}

			default:
				break;
			}
		}
	}

	BREAKYBREAK:

	Entity center = g_GameScene->CreateEntityWithDrawOrder(3, "CenterWingEntity");
	center.Transform().Translation = m_Handle.Transform().Translation;

	center.AddComponent<SpriteRendererComponent>(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
	center.AddCustomComponent<EntityTypeComponent>().Type = EntityType::BOMB_SPREAD_EXPLOSION;
	//center.AddComponent<Rigidbody2DComponent>().Gravity = false;
	//center.AddComponent<BoxCollider2DComponent>().IsSensor = true;
	//g_GameScene->CreateBody(center);

	auto& fa = center.AddCustomComponent<FrameAnimator>();
	fa.SetTarget(center);
	fa.Add(m_BombSpreadCenterAnimation);
	fa.Switch("BombCenterExplosion");

	m_Properties.SpreadEntities.reserve(5);
	m_Properties.SpreadEntities.emplace_back(std::vector<Entity>{ center });
	m_Properties.SpreadEntities.emplace_back(m_LeftWing);
	m_Properties.SpreadEntities.emplace_back(m_UpWing);
	m_Properties.SpreadEntities.emplace_back(m_RightWing);
	m_Properties.SpreadEntities.emplace_back(m_DownWing);
#endif

#define OLD_WAY 0
#if OLD_WAY
	for (int y = 1; y < m_Properties.CurrentLevel.Height - 1; ++y)
	{
		for (int x = 1; x < m_Properties.CurrentLevel.Width - 1; ++x)
		{
			GridNode* gridentity = &g_GameGrid->Get()[y][x];
			// Condition below is searching for the same entity translation coordinates as the bomb.
			// So it can properly know where spread wings should be

			if (gridentity->Handle.Transform().Translation == m_Handle.Transform().Translation)
			{
				for (int j = 0; j < m_Properties.Reach; j++)
				{
					int index = x - (j + 1);
					if (WingInitialization(y, index, index >= 1, 90.0f, m_LeftWing))
						break;
				}
				AddAnimations(m_LeftWing);

				for (int j = 0; j < m_Properties.Reach; j++)
				{
					int index = y - (j + 1);
					if (WingInitialization(index, x, index >= 1, 0.0f, m_UpWing))
						break;
				}
				AddAnimations(m_UpWing);
				for (int j = 0; j < m_Properties.Reach; j++)
				{
					int index = x + (j + 1);
					if (WingInitialization(y, index, index < g_GameGrid->GetLevelWidth(), 270.0f, m_RightWing))
						break;
				}
				AddAnimations(m_RightWing);
				for (int j = 0; j < m_Properties.Reach; j++)
				{
					int index = y + (j + 1);
					if (WingInitialization(index, x, index < g_GameGrid->GetLevelHeight(), 180.0f, m_DownWing))
						break;
				}
				AddAnimations(m_DownWing);
			}
		}
	}

	Entity center = g_GameScene->CreateEntityWithDrawOrder(3, "CenterWingEntity");
	center.Transform().Translation = m_Handle.Transform().Translation;

	center.AddComponent<SpriteRendererComponent>(glm::vec4(0.0f));
	center.AddCustomComponent<EntityTypeComponent>().Type = EntityType::BOMB_SPREAD_EXPLOSION;
	center.AddComponent<Rigidbody2DComponent>().Gravity = false;
	center.AddComponent<BoxCollider2DComponent>().IsSensor = true;
	g_GameScene->CreateBody(center);

	auto& fa = center.AddCustomComponent<FrameAnimator>();
	fa.SetTarget(center);
	fa.Add(m_BombSpreadCenterAnimation);
	fa.Switch("BombCenterExplosion");

	m_Properties.SpreadEntities.reserve(5);
	m_Properties.SpreadEntities.emplace_back(std::vector<Entity>{ center });
	m_Properties.SpreadEntities.emplace_back(m_LeftWing);
	m_Properties.SpreadEntities.emplace_back(m_UpWing);
	m_Properties.SpreadEntities.emplace_back(m_RightWing);
	m_Properties.SpreadEntities.emplace_back(m_DownWing);
#endif
}
bool Bomb::WingInitialization(int y, int distance, bool condition, float rotationZ, std::vector<Entity>& wing)
{
	if (condition)
	{
		// Grid Entity
		Entity gridEntity = g_GameGrid->Get()[y][distance].Handle;

		// Getting GridEntity coordinates so it can create new entity 
		// And eventually adding it to the wing #vector
		auto& grid_translation = gridEntity.GetComponent<TransformComponent>().Translation;

		// Checking the of the GridEntity
		switch (gridEntity.GetComponent<EntityTypeComponent>().Type)
		{
			// Stops expanding due to collision with a wall
		case EntityType::TILE_WALL: return true;

			// Breakable
			// Stops expanding due to collision with a box
		case EntityType::TILE_BOX:
		{
			// When single destoyable wall ( box ) is going to be destroyed by two bomb at once,
			// the second bomb will not add spread explosion entity because, it is not be
			// yet destroyed( marked as TILE_EMPTY ) 
			// This line is necessary and prevents this bug
			gridEntity.GetComponent<EntityTypeComponent>().Type = EntityType::TILE_EMPTY;
			gridEntity.GetComponent<FrameAnimator>().Switch("WallBreakAnimation");

			// Entity is not longer an obstacle
			g_GameGrid->Get()[y][distance].Obstacle = false;
			return true;
		}
		// Expands the explosion
		case EntityType::TILE_EMPTY:
		{
			Entity spread = g_GameScene->CreateEntityWithDrawOrder(3, "WingEntity");

			spread.AddComponent<SpriteRendererComponent>(glm::vec4(0.0f));
			spread.AddCustomComponent<EntityTypeComponent>().Type = EntityType::BOMB_SPREAD_EXPLOSION;

			spread.GetComponent<TransformComponent>().Translation = grid_translation;

			//HU_INFO("x: {0}, y: {1}", spread.GetComponent<TransformComponent>().Translation.x,
				//spread.GetComponent<TransformComponent>().Translation.y);

			spread.GetComponent<TransformComponent>().Rotation.z = glm::radians(rotationZ);

			wing.push_back(spread);
			return false;
		}
		case EntityType::BOMB: return true;
		}
		return false;
	}
	return true;
}

void Bomb::AddAnimations(std::vector<Entity>& wing)
{
	for (size_t i = 0; i < wing.size(); i++)
	{
		//HU_INFO("{0} -> {1} ", wing[i].GetComponent<TagComponent>().Tag, i);

		FrameAnimator* fa = nullptr;
		if(!wing[i].HasComponent<FrameAnimator>())
			fa = &wing[i].AddCustomComponent<FrameAnimator>();
		else
			fa = &wing[i].GetComponent<FrameAnimator>();
		fa->SetTarget(wing[i]);
		if (wing.size() == m_Properties.Reach && i == wing.size() - 1)
		{
			fa->Add(m_BombSpreadEndAnimation);
			fa->Switch("BombEndWingExplosion");
			break;
		}
		fa->Add(m_BombSpreadMiddleAnimation);
		fa->Switch("BombWingExplosion");
	}
}
