#pragma once
#include "BomberMan/Game/Enemy.hpp"

class RegularEnemy : public Enemy
{
public:
	// Inicializace nepřátelské jednotky
	RegularEnemy(Entity& handle, Entity& grid_entity);
	// Zavolá se v přídadě nějaké události
	void OnGameEvent(GameEvent& e) override;
	// Zavolá se při změně směru nepřítele
	void OnChangeDirection(Direction& dir) override;
protected:
	// Volá se každý snímek. zde můžu specifikovat jak se bude nepřítel chovat.
	bool EnemyLogic(Timestep& ts) override;
	// Pro tuto třídu
	void OnUpdateInternal(Timestep& ts) override;
private:
	// Proměnná času kalkulace další náhodné cesty nepřítele
	float m_FNRPC = 0.0f;
};