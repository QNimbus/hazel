#include "hznpch.h"
#include "EditorLayer.h"

#include "hazel/scenes/SceneSerializer.h"

#include "hazel/math/Math.h"

#include <imgui/imgui.h>
#include <ImGuizmo/ImGuizmo.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Hazel {

	EditorLayer::EditorLayer()
		: Layer("HazelnutLayer")
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
			// Create new empty scene
			m_ActiveScene = CreateRef<Scene>();

			// Create editor camera
			m_EditorCamera = EditorCamera(30.0f, 16.0f / 9.0f, 0.1f, 1000.0f);

			class RandomizeSquareColor : public ScriptableEntity {
				void OnCreate() {
					auto& sprite = GetComponent<SpriteRendererComponent>();

					sprite.Color = glm::vec4(Random::Float(), Random::Float(), Random::Float(), 1.0f);
				}
			};

			class CameraController : public ScriptableEntity {
			public:
				void OnCreate() {
				}

				void OnDestroy() {

				}

				void OnUpdate(Timestep ts) {
					static float speed = 5.0f;

					auto& camera = GetComponent<CameraComponent>();
					auto& transformComponent = GetComponent<TransformComponent>();
					
					if (camera.Primary) {
						if (Input::IsKeyPressed(Key::A))
							transformComponent.Translation.x -= speed * ts;
						else if (Input::IsKeyPressed(Key::D))
							transformComponent.Translation.x += speed * ts;
						if (Input::IsKeyPressed(Key::S))
							transformComponent.Translation.y -= speed * ts;
						else if (Input::IsKeyPressed(Key::W))
							transformComponent.Translation.y += speed * ts;
					}					
				}
			};
		}

		// Panels
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
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
			ImGuiStyle& style = ImGui::GetStyle();
			auto origMinSize = style.WindowMinSize;
			style.WindowMinSize.x = Window::s_HighDPIScaleFactor * 370.0f;
			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
			{
				ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
				ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
			}

			style.WindowMinSize = origMinSize;

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					// Disabling fullscreen would allow the window to be moved to the front of other windows, 
					// which we can't undo at the moment without finer window depth/z control.
					//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

					if (ImGui::MenuItem("New", "Ctrl-N")) {
						HZ_TRACE("Creating new empty scene");

						NewScene();
					}

					if (ImGui::MenuItem("Open...", "Ctrl-O"))
						LoadScene();

					if (ImGui::MenuItem("Save", "Ctrl-S", false, false))
						SaveSceneAs();

					if (ImGui::MenuItem("Save as...", "Ctrl-Shift-S"))
						SaveSceneAs();

					if (ImGui::MenuItem("Exit"))
						Application::Get().Close();

					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();
			}

			// Hierarchy panel
			m_SceneHierarchyPanel.OnImGuiRender();

			// Statistics panel
			{
				// ImGui Stats window
				ImGui::Begin("Statistics");

				auto stats = Renderer2D::GetStatistics();
				ImGui::Text("Renderer2D Stats:");
				ImGui::Text("Draw Calls: %d", stats.DrawCalls);
				ImGui::Text("Quads: %d", stats.QuadCount);
				ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
				ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

				ImGui::End();
			}

			// Viewport
			{
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
				ImGui::Begin("viewport");

				// Determine whether events should be consumed/blocked by ImGui depending on Focus/Hover status
				m_ViewportFocused = ImGui::IsWindowFocused();
				m_ViewportHovered = ImGui::IsWindowHovered();
				Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused && !m_ViewportHovered);
				
				ImVec2 viewPortPanelSize = ImGui::GetContentRegionAvail();
				m_ViewPortSize = { viewPortPanelSize.x, viewPortPanelSize.y };

				uint32_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
				ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{m_ViewPortSize.x, m_ViewPortSize.y}, ImVec2{0, 1}, ImVec2{1, 0});

				// Gizmos
				Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
				if (selectedEntity && m_GizmoType != -1) {
					ImGuizmo::SetOrthographic(false);
					ImGuizmo::SetDrawlist();

					float windowWidth = (float)ImGui::GetWindowWidth();
					float windowHeight = (float)ImGui::GetWindowHeight();

					ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

					// Camera transform and view
					/*auto cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
					const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
					glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());*/
					glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();
					const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();

					// Entity transform
					auto& transformComponent = selectedEntity.GetComponent<TransformComponent>();
					glm::mat4 transform = transformComponent.GetTransform();

					// Snapping
					bool snap = Input::IsKeyPressed(Key::LeftControl);
					float snapValue = 0.5f;
					if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
						snapValue = 45.0f;

					float snapValues[3] = { snapValue, snapValue, snapValue };

					ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
						static_cast<ImGuizmo::OPERATION>(m_GizmoType), ImGuizmo::MODE::LOCAL, glm::value_ptr(transform),
						nullptr, snap ? snapValues : nullptr);

					if (ImGuizmo::IsUsing()) {
						glm::vec3 translation, rotation, scale;

						Math::DecomposeTransform(transform, translation, rotation, scale);

						transformComponent.Translation = translation;
						glm::vec3 deltaRotation = rotation - transformComponent.Rotation;
						transformComponent.Rotation += deltaRotation;
						transformComponent.Scale = scale;
					}
				}

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
				m_FrameBuffer->Resize(static_cast<uint32_t>(m_ViewPortSize.x), static_cast<uint32_t>(m_ViewPortSize.y));
				m_EditorCamera.SetViewportSize(m_ViewPortSize.x, m_ViewPortSize.y);

				m_ActiveScene->OnViewportResize(static_cast<uint32_t>(m_ViewPortSize.x), static_cast<uint32_t>(m_ViewPortSize.y));
			}
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

		// Update
		{
			HZ_PROFILE_SCOPE("OnUpdate::Render::Draw");

			// Update editor camera
			m_EditorCamera.OnUpdate(ts);

			// Update scene
			m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);
			//m_ActiveScene->OnUpdateRuntime(ts);

			m_FrameBuffer->Unbind();
		}
	}

	void EditorLayer::OnEvent(Hazel::Event& event) {
		// Pass events to the editor camera
		m_EditorCamera.OnEvent(event);

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<KeyPressedEvent>(HZ_BIND_EVENT_FN(EditorLayer::OnKeyPressedEvent));
	}

	bool EditorLayer::OnKeyPressedEvent(KeyPressedEvent& event) {
		// Block further event processing for repeated keypresses and also in case a gizmo is used
		// This is to prevent for example Ctrl-N (new scene) during gizmo operation
		if (event.GetRepeatCount() > 0 || ImGuizmo::IsUsing())
			return false;

		bool alt = Input::IsKeyPressed(Key::LeftAlt) || Input::IsKeyPressed(Key::RightAlt);
		bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);
		bool ctrl = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);

		switch (event.GetKeyCode()) {
			// Menu shortcuts
			case Key::N : {
					if (ctrl) {
						NewScene();
					}
					break;
				}
			case Key::O: {
				if (ctrl) {
					LoadScene();
				}
				break;
			}
			case Key::S: {
				if (ctrl && shift) {
					SaveSceneAs();
				}
				break;
			}
			// Gizmo shortcuts
			case Key::Q: {
				m_GizmoType = -1;
				break;
			}
			case Key::W: {
				m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
				break;
			}
			case Key::E: {
				m_GizmoType = ImGuizmo::OPERATION::ROTATE;
				break;
			}
			case Key::R: {
				m_GizmoType = ImGuizmo::OPERATION::SCALE;
				break;
			}
		}

		return false;
	}

	void EditorLayer::NewScene() {
		HZ_PROFILE_FUNCTION();

		// Create new (empty) scene and deserialize selected file into it
		m_ActiveScene = CreateRef<Scene>();

		// Force viewport resize for the first frame
		m_ActiveScene->OnViewportResize(static_cast<uint32_t>(m_ViewPortSize.x), static_cast<uint32_t>(m_ViewPortSize.y));
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::LoadScene() {
		HZ_PROFILE_FUNCTION();

		// Open file dialog
		std::string sceneFile = FileDialogs::OpenFile("Hazel scene files (*.hazel)\0*.hazel\0All Files\0*.*\0\0");

		if (!sceneFile.empty()) {
			NewScene();

			SceneSerializer serializer(m_ActiveScene);
			serializer.Deserialize(sceneFile);
		}
	}

	void EditorLayer::SaveSceneAs() {
		HZ_PROFILE_FUNCTION();

		// Save file dialog
		std::string sceneFile = FileDialogs::SaveFile("Hazel scene files (*.hazel)\0*.hazel\0All Files\0*.*\0\0");

		if (!sceneFile.empty()) {
			// Ensure .hazel file extension
			//std::string::size_type pos = std::string(sceneFile).find(".hazel");
			//if (pos == std::string::npos)
			//	sceneFile.append(".hazel");

			// Serialize active scene into scenefile
			SceneSerializer serializer(m_ActiveScene);
			serializer.Serialize(sceneFile);
		}
	}
}