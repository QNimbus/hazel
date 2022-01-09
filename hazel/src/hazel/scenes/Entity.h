#pragma once

#include "hazel/scenes/Scene.h"

#include <entt.hpp>

namespace Hazel {

	class Entity {
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);
		Entity(const Entity& other) = default;

		template<typename T>
		bool HasComponent() const {
			return m_Scene->m_Registry.any_of<T>(m_EntityHandle);
		}

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args) const {
			HZ_CORE_ASSERT(!HasComponent<T>(), "Entity already has component");

			T& component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);			
			m_Scene->OnComponentAdded<T>(*this, component);

			return component;
		}

		template<typename T>
		T& GetComponent() const {
			HZ_CORE_ASSERT(HasComponent<T>(), "Entity does not have component");

			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		template<typename T>
		uint32_t RemoveComponent() const {
			HZ_CORE_ASSERT(HasComponent<T>(), "Entity does not have component");

			return m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		inline bool operator ==(const Entity& other) const { return this->m_EntityHandle == other.m_EntityHandle && this->m_Scene == other.m_Scene; }
		inline bool operator !=(const Entity& other) const { return !(*this == other); }

		operator entt::entity() const { return m_EntityHandle; }
		operator bool() const { return m_EntityHandle != entt::null; }
		operator uint32_t() const { return static_cast<uint32_t>(m_EntityHandle); }
		operator uint64_t() const { return static_cast<uint64_t>(m_EntityHandle); }

	private:
		entt::entity m_EntityHandle{ entt::null };
		Scene* m_Scene = nullptr;
	};
}

