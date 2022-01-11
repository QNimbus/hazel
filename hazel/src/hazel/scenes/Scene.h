#pragma once

#include "hazel/core/Timestep.h"
#include "hazel/renderer/Camera.h"

#include <entt.hpp>

namespace Hazel {

	// Forward declaration (to prevent circular dependency)
	class Entity;

	class Scene
	{
	public:
		Scene(const std::string& name = "Untitled scene");
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		void OnUpdateRuntime(Timestep ts);
		void OnUpdateEditor(Timestep ts, EditorCamera& camera);
		void OnViewportResize(uint32_t width, uint32_t height);

		Entity GetPrimaryCameraEntity();

		const std::string GetName() const { return m_SceneName; }
		void SetName(const std::string& name) { m_SceneName = name; }

	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth, m_ViewportHeight;
		std::string m_SceneName;

		friend class Entity;
		friend class ScenePanel;
		friend class SceneSerializer;
	};
}
