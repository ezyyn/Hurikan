#include "Bomb.h"

#include "../sceneobjects/Grid.h"
#include "../sceneobjects/Player.h"

#include <box2d/b2_body.h>

extern Player* g_Player;
extern Grid* g_GameGrid;
extern Scene* g_GameScene;

std::list<Bomb> BombManager::s_PlacedBombs;

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
	auto& it = s_PlacedBombs.begin();

	while (it != s_PlacedBombs.end())
	{
		bool exploded = it->Tick(ts);

		if (exploded)
		{
			it = s_PlacedBombs.erase(it);
			// Updating the grid again
			g_GameGrid->BombChanged();
		}
		else
			++it;
	}
}



///////////////////////////////////////////////////////////////////
///////////////////////  Bomb  ////////////////////////////////////
///////////////////////////////////////////////////////////////////

Bomb::Bomb(const BombProperties& bombprops) : m_Properties(bombprops)
{
	HU_INFO("BOMB DEPLOYED");
	m_Handle = g_GameScene->CreateEntityWithDrawOrder(1, "BombEntity");

	m_Handle.GetComponent<TransformComponent>().Translation.x = m_Properties.StartPosition.x;
	m_Handle.GetComponent<TransformComponent>().Translation.y = m_Properties.StartPosition.y;

	m_BombPlaced = g_GameGrid->WherePlayerAt();

	m_BombPlaced->Handle.GetComponent<EntityTypeComponent>().Type = EntityType::BOMB;
	m_BombPlaced->Obstacle = true;

	m_Handle.AddCustomComponent<EntityTypeComponent>().Type = EntityType::BOMB;

	m_Handle.AddComponent<Rigidbody2DComponent>().Enabled = false;
	m_Handle.AddComponent<BoxCollider2DComponent>();
	g_GameScene->CreateBody(m_Handle);

	a = ResourceManager::RequestAnimation("BombEndWingExplosion");
	b = ResourceManager::RequestAnimation("BombSpreadMiddleExplosion");
	c = ResourceManager::RequestAnimation("BombCenterExplosion");


	a.SpriteSheet = Texture2D::Create("assets/textures/tileset_16x16.png");
	b.SpriteSheet = Texture2D::Create("assets/textures/tileset_16x16.png");
	c.SpriteSheet = Texture2D::Create("assets/textures/tileset_16x16.png");

	auto& fa = m_Handle.AddCustomComponent<FrameAnimator>();
	fa.SetTarget(m_Handle);
	fa.Add(ResourceManager::RequestAnimation("BombTicking"));
	fa.Add(c);

	m_Properties.State = BombState::SET;
}

Bomb::~Bomb()
{
}

bool Bomb::Tick(Timestep ts)
{
	if (m_Properties.State == BombState::SET)
	{
		m_Handle.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f));
		m_Handle.GetComponent<FrameAnimator>().Play("BombTicking");
		m_Properties.State = BombState::TICKING;
	}

	if (m_Properties.State == BombState::TICKING)
	{
		//m_Handle.GetComponent<SpriteRendererComponent>().Color = glm::vec4(1.0f);
		m_Properties.Time += ts;
		m_Handle.GetComponent<FrameAnimator>().OnUpdate(ts);

		float distance = glm::distance(g_Player->Position2(), m_Handle.Transform().Translation);
		if (distance > 0.85f)// TODO: doladit
		{
			b2Body* body = (b2Body*)m_Handle.GetComponent<Rigidbody2DComponent>().RuntimeBody;
			body->SetEnabled(true);
			//HU_INFO(distance);
		}
	}
	if ((m_Properties.Time >= m_Properties.ExplosionTime && m_Properties.State == BombState::TICKING) || m_ChainExplosion)
	{
		HU_INFO("EXPLODED");
		m_ChainExplosion = false;

		m_Properties.State = BombState::EXPLOSION;
		WingInit();
	}

	if (m_Properties.State == BombState::EXPLOSION)
	{
		for (auto& spread : m_SpreadEntities)
		{
			auto& position = spread.Transform().Translation;

			float deltaX = glm::abs(g_Player->Position().x - position.x);
			float deltaY = glm::abs(g_Player->Position().y - position.y);

			if (deltaX < 0.9f && deltaY < 0.9f)
			{
				// Bomb explosion => insta-dead
				//g_Player->FinalHit();
				break;
			}
		}
		// if all animations are completed call destroy function
		m_AnmCmplt = true;

		for (auto& spread : m_SpreadEntities)
		{
			auto& frameanimator = spread.GetComponent<FrameAnimator>();

			if (frameanimator.IsAnyPlaying())
			{
				m_AnmCmplt = false;
			}

			frameanimator.OnUpdate(ts);
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
	HU_INFO("DESTROYED!!!")
	m_BombPlaced->Obstacle = false;
	m_BombPlaced->Handle.GetComponent<EntityTypeComponent>().Type = EntityType::TILE_EMPTY;

	// Destroys entities from the scene
	for (auto& spread: m_SpreadEntities)
	{
		g_GameScene->DestroyEntity(spread);
	}
}

void Bomb::Explode()
{
	m_ChainExplosion = true;
}

void Bomb::WingInit()
{
	auto& centerfa = m_Handle.GetComponent<FrameAnimator>();
	centerfa.Play("BombCenterExplosion");

	m_SpreadEntities.push_back(m_Handle);

	int index_x = m_BombPlaced->IndexX;
	int index_y = m_BombPlaced->IndexY;
	// Left
	for (int j = 1; j <= m_Properties.Reach; j++)
	{
		if (index_x - j >= 0)
		{
			GridNode* leftspread = g_GameGrid->Get(index_y, index_x - j);

			bool stop_expanding = SingleWingInit(j, leftspread, 90.0f);

			if (stop_expanding)
				break;
		}
	}
	// Right
	for (int j = 1; j <= m_Properties.Reach; j++)
	{
		if (index_x + j < m_Properties.CurrentLevel.Width)
		{
			GridNode* rightspread = g_GameGrid->Get(index_y, index_x + j);

			bool stop_expanding = SingleWingInit(j, rightspread, 270.0f);

			if (stop_expanding)
				break;
		}
	}
	// Up
	for (int j = 1; j <= m_Properties.Reach; j++)
	{
		if (index_y - j >= 0)
		{
			GridNode* upspread = g_GameGrid->Get(index_y - j, index_x);

			bool stop_expanding = SingleWingInit(j, upspread, 0.0f);

			if (stop_expanding)
				break;
		}
	}
	// Down
	for (int j = 1; j <= m_Properties.Reach; j++)
	{
		if (index_y + j < m_Properties.CurrentLevel.Height)
		{
			GridNode* downspread = g_GameGrid->Get(index_y + j, index_x);

			bool stop_expanding = SingleWingInit(j, downspread, 180.0f);

			if (stop_expanding)
				break;
		}
	}
}

bool Bomb::SingleWingInit(int index, GridNode* node, float rotation)
{
	switch (node->Handle.GetComponent<EntityTypeComponent>().Type)
	{
	case EntityType::TILE_WALL:
		return true;
	case EntityType::TILE_BOX:
	{
		node->Handle.GetComponent<FrameAnimator>().Play("WallBreakAnimation");
		node->Handle.GetComponent<EntityTypeComponent>().Type = EntityType::TILE_EMPTY;
		return true;
	}
	case EntityType::TILE_EMPTY:
	{
		Entity spread = g_GameScene->CreateEntityWithDrawOrder(2, "WingEntity");

		spread.AddComponent<SpriteRendererComponent>(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
		//spread.AddCustomComponent<EntityTypeComponent>().Type = EntityType::BOMB_SPREAD_EXPLOSION;
		spread.GetComponent<TransformComponent>().Translation = node->Handle.Transform().Translation;
		spread.GetComponent<TransformComponent>().Rotation.z = glm::radians(rotation);

		auto& fa = spread.AddCustomComponent<FrameAnimator>();
		fa.SetTarget(spread);
		if (index == m_Properties.Reach)
		{
			fa.Add(a);
			fa.Play("BombEndWingExplosion");
		}
		else
		{
			fa.Add(b);
			fa.Play("BombSpreadMiddleExplosion");
		}

		m_SpreadEntities.push_back(spread);
		return false;
	}

	case EntityType::BOMB:
	{
		for (auto other : BombManager::s_PlacedBombs)
		{
			if (other.m_Properties.State != BombState::EXPLOSION &&
				other.m_BombPlaced->Handle.Transform().Translation == node->Handle.Transform().Translation)
			{
				other.Explode();
			}
		}
		return true;
	}

	default:
		break;
	}
	HU_CORE_ASSERT(false, "");
	return false;
}
