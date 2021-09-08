#pragma once
#include "hupch.h"

#include "Hurikan/Core/Core.h"
#include "Hurikan/Core/Timestep.h"

#include "Hurikan/Events/Event.h"

namespace Hurikan {
	class Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& e) {}

		inline const std::string& GetName() const { return m_DebugName; }
	protected: 
		std::string m_DebugName;
	};
}
