#include "Enemy.h"

#include "BomberMan/Game/Grid.h"
#include "BomberMan/Core/ResourceManager.h"

#include "BomberMan/Core/Navigation.h"

namespace MyMath
{
	static inline float Lerp(float start, float end, float maxDistanceDelta)
	{
		if (glm::abs(end - start) <= maxDistanceDelta)
			return end;

		return start + glm::sign(end - start) * maxDistanceDelta;
	}
}

////////////////////////////////////////////////////////////////////
/////////////////////// Enemy Spawner //////////////////////////////
////////////////////////////////////////////////////////////////////

void EnemySpawner::Init(Scene* scene)
{
	g_GameScene = scene;
}

void EnemySpawner::Spawn(EnemyProps& props)
{
	if (props.Type == EnemyType::REGULAR)
	{
		auto& e = g_GameScene->CreateEntityWithDrawOrder(1);

		auto& enemy = m_Enemies.emplace_back(e, props);
		enemy.Attach(this);
	}
	else
	{
		auto& e = g_GameScene->CreateEntityWithDrawOrder(1);

		auto& enemy = m_Enemies.emplace_back(e, props);
		enemy.Attach(this);

		//HU_CORE_ASSERT(false, "Not implemented yet!");
	}
}

void EnemySpawner::OnGameEvent(GameEvent& e)
{
	if (e.Type == GameEventType::CREATE_NEW_ENEMY) // From grid
	{
		auto& PROPS = std::any_cast<EnemyProps>(e.Data);

		Spawn(PROPS);
	}
	else if (e.Type == GameEventType::VALUE_PLAYER_CHNG_POS_GRID)
	{
		auto& PLAYER_GRID_ENTITY = std::any_cast<Entity>(e.Data);

		m_PlayedGridPosition = PLAYER_GRID_ENTITY;

		UpdatePaths(e.Type);
	}
	else if (e.Type == GameEventType::ENEMY_GRID_MOVEMENT)
	{
		//DispatchToAll(GameEventType::ENEMY_GRID_MOVEMENT, e.Data);
	}
	else if (e.Type == GameEventType::BOMB_PLACED)
	{
		// Bomb is placed so path must be updated
		UpdatePaths(e.Type);
	}
	else if (e.Type == GameEventType::BOMB_VANISHED)
	{
		// Bomb exploded so path must be updated

		UpdatePaths(e.Type);
	}
}

void EnemySpawner::OnUpdate(Timestep& ts)
{
	for (auto& enemy : m_Enemies)
	{
		enemy.OnUpdate(ts);
	}
}

void EnemySpawner::UpdatePaths(GameEventType& type)
{
	for (auto& enemy : m_Enemies)
	{
		if (type != GameEventType::VALUE_PLAYER_CHNG_POS_GRID && enemy.GetProperties().Intelligence == AI::DUMB)
		{
			//auto& path = Navigation::RandomPath(enemy.LastPosition());

			/*for (auto& p : path)
			{
				HU_INFO("[!] {0} {1}", p.Transform().Translation.x, p.Transform().Translation.y);
			}*/
			//enemy.Follow(path);
			continue;
		}


		if (enemy.GetProperties().Intelligence == AI::SMART)
		{
			auto& path = Navigation::Navigate(enemy.LastPosition(), m_PlayedGridPosition);
			enemy.Follow(path);
		}
	}
}

////////////////////////////////////////////////////////////////////
////////////////////////// Enemy ///////////////////////////////////
////////////////////////////////////////////////////////////////////

Enemy::Enemy(Entity& handle, const EnemyProps& props) : m_Handle(handle), m_Props(props)
{
	m_Handle.Transform().Translation = m_Props.SpawnPoint.Transform().Translation;

	HU_INFO("Spawned!");

	m_Handle.AddComponent<SpriteRendererComponent>(glm::vec4(0.3f, 0.3f, 0.3f, 1.0f)).SubTexture = ResourceManager::GetSubTexture("PlayerIdle");
	// Setings up animations
	//auto& fa = m_Handle.AddCustomComponent<FrameAnimator>(m_Handle);

	constexpr float scale = 0.96f;
	auto& scale_cmp = m_Handle.GetComponent<TransformComponent>().Scale;
	scale_cmp.x = scale;
	scale_cmp.y = scale;

	m_LastPos = m_Props.SpawnPoint;

	auto& fa = m_Handle.AddCustomComponent<Animator>();
	fa.Add(ResourceManager::GetAnimation("PlayerLeftAnimation"));
	fa.Add(ResourceManager::GetAnimation("PlayerUpAnimation"));
	fa.Add(ResourceManager::GetAnimation("PlayerDownAnimation"));

	fa.SetTarget(m_Handle);

	//auto& rb2d = m_Handle.AddComponent<Rigidbody2DComponent>();
	//rb2d.Gravity = false;
	//rb2d.FixedRotation = true;
	//rb2d.Type = Rigidbody2DComponent::BodyType::Dynamic;
	//
	//auto& cc2d = m_Handle.AddComponent<CircleCollider2DComponent>();
	//cc2d.Radius = scale / 2.0f;
	//cc2d.Friction = 0.0f;
	//cc2d.IsSensor = true;
}

void Enemy::Follow(const std::list<Entity>& path)
{
	if (m_Path.empty())
	{
		m_Path = path;
	}
	else 
	{
		m_Path.clear();

		m_Path.push_back(m_LastPos);
		for (auto& p : path)
		{
			m_Path.push_back(p);
		}
	}

	/*if (!m_Path.empty())
	{
		HU_INFO(m_Path.front().x);
		HU_INFO(m_Path.front().y);
	}*/
}

void Enemy::OnUpdate(Timestep& ts)
{
	if (!m_Path.empty() && m_Path.front().GetComponent<GridNodeComponent>().Obstacle)
	{
		HU_INFO("IS OBSTACLE");
		// Find new random path
		m_Path.clear();
	}

	if (m_Path.empty() && m_Props.Intelligence == AI::DUMB)
	{
	//	auto& path = Navigation::RandomPath(m_LastPos);

		//Follow(path); // TODO: searching for a path to random point is not probably the best idea

	/*	for (auto& neighbour : m_LastPos.GetComponent<GridNodeComponent>().Neighbours)
		{
			auto& gnc = neighbour.GetComponent<GridNodeComponent>();

			if (!gnc.Obstacle)
			{
				m_Path.push_back(neighbour);
			}
		}*/
	}

	if (m_Path.empty())
		return;

	auto& transform = m_Handle.Transform();

	m_LastPos = m_Path.front();

	prev_x = transform.Translation.x;
	prev_y = transform.Translation.y;

	transform.Translation.x = MyMath::Lerp(m_Handle.Transform().Translation.x, m_Path.front().Transform().Translation.x, ts * 3);
	transform.Translation.y = MyMath::Lerp(m_Handle.Transform().Translation.y, m_Path.front().Transform().Translation.y, ts * 3);

	if (prev_x < transform.Translation.x && m_CurrentDirection != Direction::RIGHT)
	{
		m_CurrentDirection = Direction::RIGHT;
		if(!rotated)
			m_Handle.Transform().Scale.x *= -1;
		rotated = true;
		m_Handle.GetComponent<Animator>().Play("LeftAnimation");
		//HU_INFO("RIGHT");
	} else if(prev_x > transform.Translation.x && m_CurrentDirection != Direction::LEFT)
	{
		m_CurrentDirection = Direction::LEFT;
		m_Handle.Transform().Scale.x = glm::abs(m_Handle.Transform().Scale.x);
		rotated = false;
		m_Handle.GetComponent<Animator>().Play("LeftAnimation");
		//HU_INFO("LEFT");
	}

	if (prev_y < transform.Translation.y && m_CurrentDirection != Direction::UP)
	{
		m_CurrentDirection = Direction::UP;
		//HU_INFO("UP");
		rotated = false;
		m_Handle.Transform().Scale.x = glm::abs(m_Handle.Transform().Scale.x);
		m_Handle.GetComponent<Animator>().Play("UpAnimation");
	}
	else if (prev_y > transform.Translation.y && m_CurrentDirection != Direction::DOWN)
	{
		m_CurrentDirection = Direction::DOWN;
		m_Handle.Transform().Scale.x = glm::abs(m_Handle.Transform().Scale.x);
		rotated = false;
		m_Handle.GetComponent<Animator>().Play("DownAnimation");
		//HU_INFO("DOWn");

	}

	m_Handle.GetComponent<Animator>().OnUpdate(ts);

	if (transform.Translation.x == m_Path.front().Transform().Translation.x && transform.Translation.y == m_Path.front().Transform().Translation.y)
	{
		//HU_INFO("CHANGED GRID LOC! {0} {1}", m_LastPos.Transform().Translation.x, m_LastPos.Transform().Translation.y);
		
		m_CurrentDirection = Direction::NONE;
	//	m_Handle.GetComponent<Animator>().Pause();

		DispatchToAll(GameEventType::ENEMY_GRID_MOVEMENT, m_Handle); // TODO: Replace with m_Path.front();
		m_Path.pop_front();
	}
}
