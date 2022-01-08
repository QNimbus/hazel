#pragma once

#include "panels/SceneHierarchyPanel.h"

namespace Hazel {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnImGuiRender() override;
		virtual void OnUpdate(Timestep ts) override;
		virtual void OnEvent(Event& event) override;

	private:
		OrthographicCameraController m_CameraController;

		Ref<Texture2D> m_CheckerboardTexture;
		Ref<FrameBuffer> m_FrameBuffer;

		bool m_PrimaryCamera = true;

		Entity m_EntitySquare;
		Entity m_EntityCamera, m_EntityCamera2;

		Ref<Scene> m_ActiveScene;

		glm::vec2 m_ViewPortSize = { 0.0f, 0.0f };

		bool m_ViewportFocused = false, m_ViewportHovered = false;

		// Panels

		SceneHierarchyPanel m_SceneHierarchyPanel;
	};
}