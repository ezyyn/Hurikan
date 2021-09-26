#pragma once

#include "Scene.h"

namespace Hurikan
{
	class SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene>& scene);

		void Serialize(const std::string& filepath);
		void Serialize_Binary(const std::string& filepath);

		bool Deserialize(const std::string& filepath);
		bool Deserialize_Binary(const std::string& filepath);
	private:
		Ref<Scene> m_Scene;
	};
}