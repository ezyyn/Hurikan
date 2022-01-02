#include "Enemy.h"

#include "../sceneobjects/Grid.h"
#include "../sceneobjects/Player.h"

// Global Variables, maybe make them extern in #Game.h TODO:?
extern Scene* g_GameScene;
extern Player* g_Player;
extern Grid* g_GameGrid;

////////////////////////////////////////////////////////////////////
/////////////////////// Enemy Spawner //////////////////////////////
////////////////////////////////////////////////////////////////////
void EnemySpawner::Init()
{
}

bool EnemySpawner::Spawn(const MonsterProperties& props)
{
	m_MonsterList.push_back({ props });
	return true;
}

void EnemySpawner::UpdatePaths()
{
	for (auto& monster : m_MonsterList)
	{
		if (glm::distance(g_Player->Position(), monster.Position()) < monster.m_Properties.AttackRadius)
		{
			GridNode* start = g_GameGrid->SearchFor(monster.Transform().Translation);
			monster.Follow(m_PathFinder.NewPath(start, g_GameGrid->WherePlayerAt()));
		}
	}
}

void EnemySpawner::OnUpdate(Timestep ts)
{
	for (auto& monster : m_MonsterList)
		monster.OnUpdate(ts);
}

////////////////////////////////////////////////////////////////////
////////////////////////// Enemy ///////////////////////////////////
////////////////////////////////////////////////////////////////////
Monster::Monster(const MonsterProperties& props) : m_Properties(props)
{
	OnSpawn();
}

void Monster::OnSpawn()
{
	m_Handle = g_GameScene->CreateEntityWithDrawOrder(5, "MonsterEntity");

	m_Handle.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f));
	m_Handle.GetComponent<TransformComponent>().Translation = m_Properties.StartPosition;
	// Setings up animations
	//auto& fa = m_Handle.AddCustomComponent<FrameAnimator>(m_Handle);

	constexpr float scale = 0.8f;
	auto& scale_cmp = m_Handle.GetComponent<TransformComponent>().Scale;
	scale_cmp.x = scale;
	scale_cmp.y = scale;

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

/// <summary>
///  ADEPT NA HURIKAN MATH CLASSU
/// </summary>
/// <param name="start"></param>
/// <param name="end"></param>
/// <param name="maxDistanceDelta"></param>
/// <returns></returns>
inline float mylerp(float start, float end, float maxDistanceDelta)
{
	if (glm::abs(end - start) <= maxDistanceDelta) 
		return end;

	return start + glm::sign(end - start) * maxDistanceDelta;
}

void Monster::OnUpdate(Timestep ts)
{
	if (m_Path.empty())
		return;

	if (m_CurrentIndex >= m_Path.size() - 1)
	{
		// Player hit?
		//m_Path.clear();
		return;
	}

	if (m_IsMoving == false)
	{
		// AXIS MOVEMENT
		m_Horizontal = m_Path[m_CurrentIndex].y == m_Path[m_CurrentIndex + 1].y;
		m_Vertical = m_Path[m_CurrentIndex].x == m_Path[m_CurrentIndex + 1].x;
	}

	if (m_Horizontal)
	{
		glm::vec2 monster_pos = Position();

		float linearX = mylerp(monster_pos.x, m_Path[m_CurrentIndex + 1].x, ts * m_Properties.Speed);
		m_Handle.Transform().Translation.x = linearX;
		m_IsMoving = true;

		if (linearX == m_Path[m_CurrentIndex + 1].x)
		{
			m_CurrentIndex++;
			m_IsMoving = false;
		}
	}
	else
	{
		if (m_Vertical)
		{
			glm::vec2 monster_pos = Position();

			float linearY = mylerp(monster_pos.y, m_Path[m_CurrentIndex + 1].y, ts * m_Properties.Speed);
			m_Handle.Transform().Translation.y = linearY;
			m_IsMoving = true;

			if (linearY == m_Path[m_CurrentIndex + 1].y)
			{
				m_IsMoving = false;
				m_CurrentIndex++;
			}
		}
	}
}

void Monster::Follow(const std::deque<glm::vec2>& path)
{
	m_CurrentIndex = 0;

	if (!m_Path.empty())
	{
		m_Path.clear();
	}

	m_Path = path;
}

void Monster::DestroyItself()
{
	g_GameScene->DestroyEntity(m_Handle);
}
