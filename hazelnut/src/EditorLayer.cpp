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

		{
			m_ActiveScene = CreateRef<Scene>();

			m_EntityCamera = m_ActiveScene->CreateEntity("Scene camera");
			m_EntityCamera2 = m_ActiveScene->CreateEntity("Clip space camera");
			m_EntitySquare = m_ActiveScene->CreateEntity("Random square");
			m_EntityCamera.AddComponent<CameraComponent>();
			auto& secondCam = m_EntityCamera2.AddComponent<CameraComponent>();
			secondCam.Primary = false;
			secondCam.Camera.SetOrthographicSize(50.0f);
			m_EntitySquare.AddComponent<SpriteRendererComponent>();

			auto& tc1 = m_EntityCamera.GetComponent<TransformComponent>();
			auto& tc2 = m_EntityCamera2.GetComponent<TransformComponent>();

			class RandomizeSquareColor : public ScriptableEntity {
				void OnCreate() {
					auto& sprite = GetComponent<SpriteRendererComponent>();

					sprite.Color = glm::vec4(Random::Float(), Random::Float(), Random::Float(), 1.0f);
				}
			};

			class CameraController : public ScriptableEntity {
			public:
				void OnCreate() {
					auto& transform = GetComponent<TransformComponent>().Transform;
					transform[3].x = (Random::Float() * 12.0f) - 6.0f;
					transform[3].y = (Random::Float() * 9.0f) - 4.5f;
				}

				void OnDestroy() {

				}

				void OnUpdate(Timestep ts) {
					static float speed = 5.0f;

					auto& camera = GetComponent<CameraComponent>();
					auto& transform = GetComponent<TransformComponent>().Transform;
					
					if (camera.Primary) {
						if (Input::IsKeyPressed(KeyCode::A))
							transform[3].x -= speed * ts;
						else if (Input::IsKeyPressed(KeyCode::D))
							transform[3].x += speed * ts;
						if (Input::IsKeyPressed(KeyCode::S))
							transform[3].y -= speed * ts;
						else if (Input::IsKeyPressed(KeyCode::W))
							transform[3].y += speed * ts;
					}					
				}
			};

			m_EntityCamera.AddComponent<NativeScriptComponent>().Bind<CameraController>();
			m_EntityCamera2.AddComponent<NativeScriptComponent>().Bind<CameraController>();
			m_EntitySquare.AddComponent<NativeScriptComponent>().Bind<RandomizeSquareColor>();
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

				if (m_EntitySquare) {
					ImGui::Separator();
					ImGui::Text("%s", m_EntitySquare.GetComponent<TagComponent>().Tag.c_str());

					auto& color = m_EntitySquare.GetComponent<SpriteRendererComponent>().Color;
					ImGui::ColorEdit4("Square Color", glm::value_ptr(color));
					ImGui::Separator();
				}

				ImGui::DragFloat2("Cam1 transform", glm::value_ptr(m_EntityCamera.GetComponent<TransformComponent>().Transform[3]));

				auto& cam1 = m_EntityCamera.GetComponent<CameraComponent>();
				auto& cam2 = m_EntityCamera2.GetComponent<CameraComponent>();

				if (ImGui::Checkbox("Cam1", &m_PrimaryCamera)) {
					cam1.Primary = m_PrimaryCamera;
					cam2.Primary = !m_PrimaryCamera;
				}
				{
					float size = cam2.Camera.GetOrthographicSize();
					if (ImGui::DragFloat("Cam2 size", &size))
						cam2.Camera.SetOrthographicSize(size);

				}

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

				m_ActiveScene->OnViewportResize(static_cast<uint32_t>(m_ViewPortSize.x), static_cast<uint32_t>(m_ViewPortSize.y));
			}
		}

		// Update
		{
			HZ_PROFILE_SCOPE("OnUpdate::CameraController");

			if (m_ViewportFocused)
				m_CameraController.OnUpdate(ts);
		}

		// Reset statistics
		Renderer2D::ResetStatistics();


		// Render preparation
		{
			HZ_PROFILE_SCOPE("OnUpdate::Render::Prep");

			m_FrameBuffer->Bind();

			RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
			RenderCommand::Clear();
		}		

		{
			HZ_PROFILE_SCOPE("OnUpdate::Render::Draw");

			// Draw scene
			m_ActiveScene->OnUpdate(ts);

			m_FrameBuffer->Unbind();
		}

	}

	void EditorLayer::OnEvent(Hazel::Event& event) {
		// Pass events to camera controller
		m_CameraController.OnEvent(event);
	}


}