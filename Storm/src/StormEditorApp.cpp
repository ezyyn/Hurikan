#include "Hurikan.h"
#include "Hurikan/Core/EntryPoint.h"

#include "EditorLayer.h"

namespace Hurikan {

	class Storm : public Application
	{
	public:
		Storm() : Application("Storm Editor")
		{
			PushLayer(new EditorLayer());
		}
		~Storm()
		{

		}
	};

	Application* Hurikan::CreateApplication() {
		return new Storm();
	}
}