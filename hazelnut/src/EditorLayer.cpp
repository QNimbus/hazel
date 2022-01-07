#include "EditorLayer.h"

#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Hazel {

	EditorLayer::EditorLayer()
		: Layer("HazelnutLayer"), m_CameraController(1280.0f / 720.0f, true)
	{
	}

	void EditorLayer::OnAttach() {
		HZ_PROFILE_FUNCTION();

		// Seed random generator
		{
			HZ_PROFILE_SCOPE("OnAttach::RandomGenerator");

			Random::Init();
		}

		{
			m_CheckerboardTexture = Hazel::Texture2D::Create("assets/textures/Checkerboard.png");

			FrameBufferSpecification frameBufferSpecification;
			frameBufferSpecification.Width = 1280;
			frameBufferSpecification.Height = 720;
			m_FrameBuffer = FrameBuffer::Create(frameBufferSpecification);
		}
	}

	void EditorLayer::OnDetach() {
		HZ_PROFILE_FUNCTION();
	}

	void EditorLayer::OnImGuiRender() {
		HZ_PROFILE_FUNCTION();

		// Window docking
		{
			static bool dockspaceOpen = true;
			static bool opt_fullscreen_persistant = true;
			bool opt_fullscreen = opt_fullscreen_persistant;
			static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

			// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
			// because it would be confusing to have two docking targets within each others.
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
			if (opt_fullscreen)
			{
				ImGuiViewport* viewport = ImGui::GetMainViewport();
				ImGui::SetNextWindowPos(viewport->Pos);
				ImGui::SetNextWindowSize(viewport->Size);
				ImGui::SetNextWindowViewport(viewport->ID);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
				window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
				window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
			}

			// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
			if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
				window_flags |= ImGuiWindowFlags_NoBackground;

			// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
			// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
			// all active windows docked into it will lose their parent and become undocked.
			// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
			// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
			ImGui::PopStyleVar();

			if (opt_fullscreen)
				ImGui::PopStyleVar(2);

			// DockSpace
			ImGuiIO& io = ImGui::GetIO();
			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
			{
				ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
				ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
			}

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					// Disabling fullscreen would allow the window to be moved to the front of other windows, 
					// which we can't undo at the moment without finer window depth/z control.
					//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

					if (ImGui::MenuItem("Exit")) Application::Get().Close();
					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();
			}

			{
				// ImGui Settings window
				ImGui::Begin("Settings");

				auto stats = Renderer2D::GetStatistics();
				ImGui::Text("Renderer2D Stats:");
				ImGui::Text("Draw Calls: %d", stats.DrawCalls);
				ImGui::Text("Quads: %d", stats.QuadCount);
				ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
				ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

				ImGui::End();
			}

			{
				// ImGui view port
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
				ImGui::Begin("viewport");

				// Determine whether events should be consumed/blocked by ImGui depending on Focus/Hover status
				m_ViewportFocused = ImGui::IsWindowFocused();
				m_ViewportHovered = ImGui::IsWindowHovered();
				Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);
				
#if 0
				// Did viewport dimensions change?
				if (m_ViewPortSize != *((glm::vec2*)&viewPortPanelSize)) {
					m_FrameBuffer->Resize((uint32_t)viewPortPanelSize.x, (uint32_t)viewPortPanelSize.y);
					

					m_CameraController.OnResize(viewPortPanelSize.x, viewPortPanelSize.y);
				}
#endif

				ImVec2 viewPortPanelSize = ImGui::GetContentRegionAvail();
				m_ViewPortSize = { viewPortPanelSize.x, viewPortPanelSize.y };

				uint32_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
				ImGui::Image((void*)textureID, ImVec2{ m_ViewPortSize.x, m_ViewPortSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

				ImGui::End();
				ImGui::PopStyleVar();
			}

			ImGui::End();
		}
	}

	void EditorLayer::OnUpdate(Timestep ts) {
		HZ_PROFILE_FUNCTION();

		// Resize
		{
			HZ_PROFILE_SCOPE("OnUpdate::Resize");

			if (FrameBufferSpecification spec = m_FrameBuffer->GetSpecification();
				m_ViewPortSize.x > 0.0f && m_ViewPortSize.y > 0.0f && // zero sized framebuffer is invalid
				(spec.Width != m_ViewPortSize.x || spec.Height != m_ViewPortSize.y))
			{
				m_FrameBuffer->Resize((uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);
				m_CameraController.OnResize(m_ViewPortSize.x, m_ViewPortSize.y);
			}
		}

		// Update
		{
			HZ_PROFILE_SCOPE("OnUpdate::CameraController");

			if (m_ViewportFocused)
				m_CameraController.OnUpdate(ts);
		}

		//Reset statistics
		Renderer2D::ResetStatistics();

		{
			HZ_PROFILE_SCOPE("OnUpdate::Render::Prep");

			m_FrameBuffer->Bind();

			// Render preparation
			RenderCommand::SetClearColor({ 0.07f, 0.07f, 0.07f, 1.0f });
			RenderCommand::Clear();

		}
		{
			HZ_PROFILE_SCOPE("OnUpdate::Render::Draw");

			static float rotation = 0.0f;
			rotation += ts * 100.0f;

			// Render draw
			Renderer2D::BeginScene(m_CameraController.GetCamera());
			Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.5f, 0.5f }, { 0.8f, 0.2f, 0.3f, 1.0f });
			Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f }, { 0.5f, 0.5f }, glm::radians(-rotation), { 0.8f, 0.2f, 0.3f, 1.0f });
			Renderer2D::DrawRotatedQuad({ 1.0f, 0.0f }, { 0.5f, 0.5f }, glm::radians(rotation), { 0.8f, 0.2f, 0.3f, 1.0f });
			Renderer2D::DrawQuad({ 0.5f, -0.5f, 0.1f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
			Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 15.0f, 15.0f }, m_CheckerboardTexture, 15.0f);
			Renderer2D::EndScene();

			Renderer2D::BeginScene(m_CameraController.GetCamera());
			for (float y = -5.0; y <= 5.0f; y += 0.5f) {
				for (float x = -5.0; x <= 5.0f; x += 0.5f) {
					glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f };
					Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
				}
			}
			Renderer2D::EndScene();

			m_FrameBuffer->Unbind();
		}
	}

	void EditorLayer::OnEvent(Hazel::Event& event) {
		// Pass events to camera controller
		m_CameraController.OnEvent(event);
	}


}