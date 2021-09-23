#pragma once
#include "Hurikan/Core/Base.h"
#include "Hurikan/Core/Log.h"

#include "Hurikan/Scene/Scene.h"
#include "Hurikan/Scene/Entity.h"



namespace Hurikan
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& context);

		void SetContext(const Ref<Scene>& context);

		void OnImGuiRender();
	private:
		void DrawEntityNode(Entity& entity);
		void DrawComponents(Entity entity);
	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	};
}