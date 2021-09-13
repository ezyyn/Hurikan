#pragma once

#include "Hurikan/Core/Base.h"

namespace Hurikan {
	class Input {
	public: 
		static bool IsKeyPressed(int keycode);
		static bool IsMouseButtonPressed(int button);

		static float GetMouseX();
		static float GetMouseY();

		static std::pair<float, float> GetMousePosition();
	};
}