#pragma once

#include "hazel/scenes/Entity.h"

namespace Hazel {

	struct SelectionType : public std::variant<std::monostate, Entity, Ref<Scene>> {
		operator bool() { return std::holds_alternative<std::monostate>(*this); }

		template<typename T>
		bool is() {
			return std::holds_alternative<T>(*this);
		}

		template<typename T>
		bool operator==(T const& rhs)
		{
			const T* lhs = std::get_if<T>(this);			
			return lhs && *(lhs) == rhs;			
		}
	};

	class ScenePanel
	{
	public:
		ScenePanel() = default;
		ScenePanel(const Ref<Scene>& scene);

		void SetContext(const Ref<Scene>& scene);

		void OnImGuiRender();

		void RenderScenePanel();
		void RenderPropertiesPanel();

		const Entity GetSelectedEntity() { Entity* entity = std::get_if<Entity>(&m_SelectionContext); if (entity) return *entity; else return {}; }

	private:
		template<typename T, typename UIFunction>
		static void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction);
		void DrawComponents(Entity entity);
		void DrawEntityNode(Entity entity);

		void DrawSceneProperties(const Ref<Scene>& scene);

	private:
		Ref<Scene> m_Context;
		SelectionType m_SelectionContext;

		bool m_NewEntity = false;
	};
}

