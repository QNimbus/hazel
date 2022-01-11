#pragma once

#include "hazel/renderer/Camera.h"

#include "panels/ScenePanel.h"

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

		void NewScene();
		void LoadScene();
		void SaveScene();
		void SaveSceneAs();

	private:
		// Events
		bool OnKeyPressedEvent(KeyPressedEvent& event);

	private:
		std::string m_SceneFileName;
		Ref<Scene> m_ActiveScene;
		Ref<FrameBuffer> m_FrameBuffer;
		//Ref<Texture2D> m_CheckerboardTexture;

		EditorCamera m_EditorCamera;

		glm::vec2 m_ViewPortSize = { 0.0f, 0.0f };
		bool m_ViewportFocused = false, m_ViewportHovered = false;
		int m_GizmoType = -1;

		// Panels
		ScenePanel m_ScenePanel;
	};
}