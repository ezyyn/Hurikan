#include "Hurikan.h"
#include "Hurikan/Core/EntryPoint.h"

#include "EditorLayer.h"

namespace Hurikan {

	class Storm : public Application
	{
	public:
		Storm(ApplicationCommandLineArgs args) : Application("Storm Editor",args, 1600, 900)
		{
			PushLayer(new EditorLayer());
		}
		~Storm()
		{

		}
	};

	Application* Hurikan::CreateApplication(ApplicationCommandLineArgs args) {
		return new Storm(args);
	}
}