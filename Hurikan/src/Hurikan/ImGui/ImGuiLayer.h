#pragma once

#include "Hurikan/Core/Layer.h"

namespace Hurikan {
	class HU_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};
}

