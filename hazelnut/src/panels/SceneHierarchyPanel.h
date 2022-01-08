#pragma once

namespace Hazel {

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& scene);

		void SetContext(const Ref<Scene>& scene);

		void OnImGuiRender();

	private:
		void DrawComponents(const Entity& entity);
		void DrawEntityNode(const Entity& entity);

	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	};
}

