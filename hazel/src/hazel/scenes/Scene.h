#pragma once

#include "hazel/core/Timestep.h"

#include <entt.hpp>

namespace Hazel {

	// Forward declaration (to prevent circular dependency)
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		void OnUpdate(Timestep ts);
		void OnViewportResize(uint32_t width, uint32_t height);

		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth, m_ViewportHeight;

		friend class Entity;
		friend class SceneHierarchyPanel;
	};
}
