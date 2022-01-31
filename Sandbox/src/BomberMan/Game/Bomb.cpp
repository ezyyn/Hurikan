#include "Bomb.h"

#include "BomberMan/Core/ResourceManager.h"
#include "BomberMan/Game/Grid.h"

#include <box2d/b2_body.h>

static std::vector<Bomb> s_BombList;

////////////////////////////////////////////////////////////////////
///////////////////////  Bomb Manager //////////////////////////////
////////////////////////////////////////////////////////////////////

void BombManager::Init(Scene* scene)
{
	g_GameScene = scene;
}
bool BombManager::PlaceBomb(BombProps& props)
{
	props.GridEntity = m_PlayerGrid;

	for (auto& bomb : s_BombList)
	{
		if (props.GridEntity.Transform().Translation == bomb.Position())
			return false;
	}

	HU_CORE_ASSERT(g_GameScene, "Scene is nullptr!");

	DispatchToAll(GameEventType::BOMB_PLACED, props.GridEntity);

	auto& bomb = s_BombList.emplace_back(props, g_GameScene);
	bomb.Attach(this);
	
	return true;
}

void BombManager::OnGameEvent(GameEvent& e)
{
	if (e.Type == GameEventType::VALUE_PLAYER_CHNG_POS_GRID) 
	{
		// Event from GRID
		
		// Updates the position of the PLAYER

		auto& GRID_ENTITY = std::any_cast<Entity>(e.Data);
		//HU_INFO("x: {0}, y: {1}", GRID_ENTITY.Transform().Translation.x, GRID_ENTITY.Transform().Translation.y);
		m_PlayerGrid = GRID_ENTITY;
	}
	else if (e.Type == GameEventType::BOMB_EXPLODED) 
	{
		// Event from BOMB to player if explosion hit the player

		// e.Data => m_SpreadEntities
		
		DispatchToAll(GameEventType::BOMB_EXPLODED, e.Data);
	}
	else if (e.Type == GameEventType::BOMB_VANISHED)
	{
		// Event from BOMB

		// Sends info to GRID that BOMB vanished => GRID can now clear the GRID_ENTITY under the bomb

		//auto& GRID_ENTITY = std::any_cast<Entity>(e.Data);

		DispatchToAll(GameEventType::BOMB_VANISHED, e.Data);
	}
	else if (e.Type == GameEventType::BREAK_WALL)
	{
		// Sends info to GRID that some bomb destroyed a wall, GRID_ENTITY

		DispatchToAll(GameEventType::BREAK_WALL, e.Data);
	}
}

void BombManager::OnUpdate(Timestep& ts)
{
	auto& itr = s_BombList.begin();
	while (itr != s_BombList.end())
	{
		bool destroyable = itr->OnTick(ts);

		if (destroyable) 
		{
			itr = s_BombList.erase(itr);
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
	if ((m_Properties.Time >= m_Properties.ExplosionTime && m_Properties.State == BombState::TICKING) || m_ChainExplosion)
	{
		m_ChainExplosion = false;

		// Creates the explosion
		Expand();

		// Send info to BOMB MANAGER and PLAYER that this bomb exploded
		DispatchToAll(GameEventType::BOMB_EXPLODED, m_SpreadEntities);

		m_Properties.State = BombState::EXPLOSION;
	}

	if (m_Properties.State == BombState::EXPLOSION)
	{
		for (auto& exp : m_SpreadEntities)
		{
			auto& frame_animator = exp.GetComponent<Animator>();
			frame_animator.OnUpdate(ts);

			if (!frame_animator.IsAnyPlaying())
			{
				m_AnimationCompletedCount++;
				//exp.RemoveComponent<SpriteRendererComponent>();
				//exp.RemoveComponent<Animator>();
				if (m_AnimationCompletedCount == m_SpreadEntities.size())
				{
					// Send info to BOMB MANAGER that this bomb exploded
					DispatchToAll(GameEventType::BOMB_VANISHED, m_OnTopOfEntity);
					// ... Then destroy itself
					DestroyItSelf();
					return true;
				}
			}
		}

		return false;
	}

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

	return false;
}

void Bomb::DestroyItSelf()
{
	for (auto& exp : m_SpreadEntities)
	{
		g_GameScene->DestroyEntity(exp);
	}

	m_Properties.State = BombState::DONE;

	m_SpreadEntities.clear();
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
		DispatchToAll(GameEventType::BREAK_WALL, entity);
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
			if (*this != other && other.GetState() < BombState::EXPLOSION &&
				other.m_Handle.Transform().Translation == entity.Transform().Translation)
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
