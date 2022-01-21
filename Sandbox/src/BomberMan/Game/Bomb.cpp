#include "Bomb.h"

#include "BomberMan/Core/ResourceManager.h"
#include "BomberMan/Game/Grid.h"

#include <box2d/b2_body.h>

static std::vector<Bomb> s_BombList;

void BombManager::Init(Scene* scene)
{
	g_GameScene = scene;
}

////////////////////////////////////////////////////////////////////
///////////////////////  Bomb Manager //////////////////////////////
////////////////////////////////////////////////////////////////////
bool BombManager::PlaceBomb(BombProps& props)
{
	props.GridEntity = m_PlayerGrid;

	for (auto& bomb : s_BombList)
	{
		if (props.GridEntity.Transform().Translation == bomb.Position())
			return false;
	}

	HU_CORE_ASSERT(g_GameScene, "Scene is nullptr!");

	DispatchToAll(GameEventType::BOMB_PLACED, m_PlayerGrid);

	s_BombList.emplace_back(props, g_GameScene);
	return true;
}

void BombManager::OnGameEvent(GameEvent& e)
{
	if (e.Type == GameEventType::VALUE_PLAYER_CHNG_POS_GRID) // Should be an event from grid
	{
		auto& entity = std::any_cast<Entity>(e.Data);
		HU_INFO("x: {0}, y: {1}", entity.Transform().Translation.x, entity.Transform().Translation.y);
		m_PlayerGrid = entity;
	}
}

void BombManager::OnUpdate(Timestep& ts)
{
	auto& itr = s_BombList.begin();
	while (itr != s_BombList.end())
	{
		if (itr->OnTick(ts)) 
		{
			//DispatchToAll(GameEventType::BOMB_ERASED);
			//itr = s_BombList.erase(itr);
		}
		else 
		{
			++itr;
		}
	}
}

///////////////////////////////////////////////////////////////////
///////////////////////  Bomb  ////////////////////////////////////
///////////////////////////////////////////////////////////////////
Bomb::Bomb(const BombProps& props, Scene* scene)
	: m_Properties(props)
{
	g_GameScene = scene;

	m_Handle = scene->CreateEntityWithDrawOrder(1, "BombEntity");

	m_OnTopOfEntity = props.GridEntity;

	m_Handle.Transform().Translation.x = m_Properties.GridEntity.Transform().Translation.x;
	m_Handle.Transform().Translation.y = m_Properties.GridEntity.Transform().Translation.y;

	m_Handle.AddComponent<Rigidbody2DComponent>().Enabled = false;
	m_Handle.AddComponent<BoxCollider2DComponent>();
	m_Handle.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f));
	scene->CreateBody(m_Handle);

	auto& fa = m_Handle.AddCustomComponent<Animator>("Bomb");
	fa.SetTarget(m_Handle);
	fa.Add(ResourceManager::GetAnimation("BombTicking"));
	fa.Add(ResourceManager::GetAnimation("BombCenterExplosion"));

	m_Properties.State = BombState::SET;
}

bool Bomb::OnTick(Timestep& ts)
{
	if (m_Properties.State == BombState::SET)
	{
		m_Handle.GetComponent<Animator>().Play("BombTicking");
		m_Properties.State = BombState::TICKING;
	}

	if (m_Properties.State == BombState::TICKING)
	{
		m_Properties.Time += ts;
		m_Handle.GetComponent<Animator>().OnUpdate(ts);
	}

	if ((m_Properties.Time >= m_Properties.ExplosionTime && m_Properties.State == BombState::TICKING)  || m_ChainExplosion)
	{
		m_Properties.State = BombState::EXPLOSION;

		Expand();
	}

	if (m_Properties.State == BombState::EXPLOSION)
	{
		for (auto& exp : m_SpreadEntities)
		{
			if (exp.HasComponent<Animator>())
			{
				auto& frame_animator = exp.GetComponent<Animator>();
				frame_animator.OnUpdate(ts);

				if (!frame_animator.IsAnyPlaying())
				{
					exp.RemoveComponent<SpriteRendererComponent>();
					exp.RemoveComponent<Animator>();
				}
			}
		}
	}

	return false;
}

void Bomb::Explode()
{
	m_ChainExplosion = true;
}

void Bomb::Expand()
{
	auto& centerfa = m_Handle.GetComponent<Animator>();
	centerfa.Play("BombCenterExplosion");

	m_SpreadEntities.push_back(m_Handle);

	auto& gnc = m_OnTopOfEntity.GetComponent<GridNodeComponent>();

	int index_x = gnc.Index.x;
	int index_y = gnc.Index.y;

	// Left
	for (int j = 1; j <= m_Properties.Reach; ++j)
	{
		if (index_x - j >= 0)
		{
			auto leftspread = Grid::Get(index_y, index_x - j);

			bool stop_expanding = WingExpand(j, leftspread, 90.0f);

			if (stop_expanding)
				break;
		}
	}
	// Right
	for (int j = 1; j <= m_Properties.Reach; ++j)
	{
		if (index_x + j < LevelManager::GetCurrentLevel().Width)
		{
			auto rightspread = Grid::Get(index_y, index_x + j);

			bool stop_expanding = WingExpand(j, rightspread, 270.0f);

			if (stop_expanding)
				break;
		}
	}
	// Up
	for (int j = 1; j <= m_Properties.Reach; ++j)
	{
		if (index_y - j >= 0)
		{
			auto upspread = Grid::Get(index_y - j, index_x);

			bool stop_expanding = WingExpand(j, upspread, 0.0f);

			if (stop_expanding)
				break;
		}
	}
	// Down
	for (int j = 1; j <= m_Properties.Reach; ++j)
	{
		if (index_y + j < LevelManager::GetCurrentLevel().Height)
		{
			auto downspread = Grid::Get(index_y + j, index_x);
			//auto downspread = g_GameGrid->Get(index_y + j, index_x);

			bool stop_expanding = WingExpand(j, downspread, 180.0f);

			if (stop_expanding)
				break;
		}
	}
}

bool Bomb::WingExpand(int index, Entity& entity, float rotation)
{
	switch (entity.GetComponent<EntityTypeComponent>().Type)
	{
	case EntityType::WALL:
		return true;
	case EntityType::LOOT_WALL:
	{
		entity.GetComponent<Animator>().Play("WallBreakAnimation");
		entity.GetComponent<EntityTypeComponent>().Type = EntityType::EMPTY;

		auto rb = (b2Body*)entity.GetComponent<Rigidbody2DComponent>().RuntimeBody;
		rb->SetEnabled(false);
		entity.GetComponent<GridNodeComponent>().Obstacle = false;
		return true;
	}
	case EntityType::EMPTY:
	{
		Entity spread = g_GameScene->CreateEntityWithDrawOrder(2, "WingEntity");

		spread.AddComponent<SpriteRendererComponent>(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
		spread.GetComponent<TransformComponent>().Translation = entity.Transform().Translation;
		spread.GetComponent<TransformComponent>().Rotation.z = glm::radians(rotation);

		auto& fa = spread.AddCustomComponent<Animator>();
		fa.SetTarget(spread);
		if (index == m_Properties.Reach)
		{
			fa.Add(ResourceManager::GetAnimation("BombEndWingExplosion"));
			fa.Play("BombEndWingExplosion");
		}
		else
		{
			fa.Add(ResourceManager::GetAnimation("BombSpreadMiddleExplosion"));
			fa.Play("BombSpreadMiddleExplosion");
		}

		m_SpreadEntities.emplace_back(spread);
		return false;
	}

	case EntityType::BOMB:
	{
		for (auto& other : s_BombList)
		{
			if (other.m_Properties.State != BombState::EXPLOSION &&
				other.m_Handle.Transform().Translation == m_Handle.Transform().Translation)
			{
				HU_INFO("EXPLDED");
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
