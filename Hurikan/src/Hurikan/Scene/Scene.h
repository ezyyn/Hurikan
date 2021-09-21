#pragma once
#include "entt.hpp"

#include "Hurikan/Core/Timestep.h"

namespace Hurikan
{
	class Scene
	{
	public:
		Scene();
		~Scene();

		entt::entity CreateEntity();

		//temp
		entt::registry& Reg() { return m_Registry; }

		void OnUpdate(Timestep& ts);
	private:
		entt::registry m_Registry;
	};
}