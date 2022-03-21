#pragma once

#include "BomberMan/Core/Observer.hpp"
#include "BomberMan/Core/SaveLoadSystem.hpp"
#include "BomberMan/Core/ResourceManager.hpp"
#include "BomberMan/Core/Navigation.hpp"

#include <Hurikan/Core/Log.h>
#include <Hurikan/Scene/Entity.h>
using namespace Hurikan;

#include <string>

enum class AI : unsigned short
{
	RANDOM = 0,
	FOLLOW_RANGE,
	FOLLOW
};

struct EnemyProps
{
	EnemyProps() {};

	EnemyProps(unsigned int health, AI intl, std::string name, float speed)
		: Health(health), Intelligence(intl), Name(name), Speed(speed)
	{
	}

	EnemyProps(const EnemyProps&) = default;

	EnemyProps(EnemyProps&& other) noexcept
	{
		Health = std::move(other.Health);
		Intelligence = std::move(other.Intelligence);
		Name = std::move(other.Name);
		Speed = std::move(other.Speed);
	}

	inline EnemyProps& operator=(EnemyProps&& other) noexcept
	{
		Health = std::move(other.Health);
		Intelligence = std::move(other.Intelligence);
		Name = std::move(other.Name);
		Speed = std::move(other.Speed);
		return *this;
	}

	unsigned int Health = 0;
	AI Intelligence = AI::RANDOM;
	std::string Name = std::string();
	float Speed = 0.0f;
};

enum class Direction
{
	UP = 0,
	DOWN,
	LEFT,
	RIGHT,
	NONE
};

class Enemy : public Observable
{
public:
	Enemy(Entity& handle, Entity& grid_entity);
	virtual ~Enemy() {}
	// Pomocné funkce 
	inline const glm::vec3& GetPosition() { return m_Handle.Transform().Translation; }
	inline Entity GetLastPositionOnGrid() { return m_LastPositionOnGrid; }
	inline bool SameAs(Entity& other_handle) { return m_Handle == other_handle; }
	inline const EnemyProps& GetProperties() { return m_Properties; };

	void OnUpdate(Timestep & ts);
	virtual void OnGameEvent(GameEvent& e) = 0;
protected:
	virtual void OnChangeDirection(Direction& dir) = 0;
	virtual void OnUpdateInternal(Timestep& ts) = 0;
	virtual bool EnemyLogic(Timestep& ts) = 0;
	virtual void Follow(const std::list<Entity>& path);
private:
	virtual void OnHitUpdate(Timestep& ts);
protected:
	Direction m_CurrentDirection;

	std::list<Entity> m_Path;
	bool m_Alive = true;
	bool m_Hit = false;
	EnemyProps m_Properties;
	Entity m_Handle;
private:
	Entity m_LastPositionOnGrid;
	bool m_IsRotated = false;
	glm::vec3 m_PreviousPosition;
	glm::vec4 m_HitColor = glm::vec4(1.0f);
};

class EnemySpawner : public Observer, public Observable
{
public:
	~EnemySpawner();
	// Inicializace třídy EnemySpawner
	void Init(Scene* scene);
	// Vytváří jednotlivé nepřítelské entity
	void Spawn(Entity& entity);
	// Funkce zděděná ze třídy Observer
	void OnGameEvent(GameEvent& e) override;
	// Funkce volaná OnUpdate funkcí ze třídy InGame
	void OnUpdate(Timestep& ts);
private:
	// Vektor obsahující všechny nepřítelské jednotky
	std::vector<Enemy*> m_Enemies;
	// List obsahující všechny nepřátelské jednotky, které jsou ve frontě pro jejich zničení.
	std::list<Enemy*> m_ErasePool;
	bool m_GameOver = false; // Indikátor konce hry
private:
	Scene* g_GameScene; // Aktuální scéna
};