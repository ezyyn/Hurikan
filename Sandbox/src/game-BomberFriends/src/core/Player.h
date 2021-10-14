#pragma once

#include <Hurikan.h>
using namespace Hurikan;

class PlayerController : public ScriptableEntity
{

protected:
	virtual void PlayerController::OnCreate() override
	{
		GetComponent<TransformComponent>().Translation.z = 0.1f;
	}

	virtual void PlayerController::OnDestroy() override
	{
	}

	virtual void PlayerController::OnUpdate(Timestep ts) override
	{
		auto& translation = GetComponent<TransformComponent>().Translation;

		if (Input::IsKeyPressed(Key::A))
			translation.x -= Speed * ts;
		if (Input::IsKeyPressed(Key::D))
			translation.x += Speed * ts;
		if (Input::IsKeyPressed(Key::W))
			translation.y += Speed * ts;
		if (Input::IsKeyPressed(Key::S))
			translation.y -= Speed * ts;
	}
private:
	float Speed = 5.0f;
};

class Player 
{
public:
	Player() = default;
	
	void Init(/* [GRIDDATA], */Entity& entity);

	Entity& GetEntity() { return m_PlayerEntity; }
private:
	Entity m_PlayerEntity;

	Ref<Texture2D> m_PlayerTexture;
};