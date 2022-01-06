#include "Sandbox2D.h"

#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, true), m_ParticleSystem(50000)
{
}

void Sandbox2D::OnAttach() {
	HZ_PROFILE_FUNCTION();

	// Seed random generator
	{
		HZ_PROFILE_SCOPE("OnAttach::RandomGenerator");

		Hazel::Random::Init();
	}

	// Initialize particle system
	{
		HZ_PROFILE_SCOPE("OnAttach::ParticleSystem");

		// Init here
		m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
		m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
		m_Particle.SizeBegin = 0.4f, m_Particle.SizeVariation = 0.2f, m_Particle.SizeEnd = 0.0f;
		m_Particle.LifeSpan = 3.0f;
		m_Particle.Velocity = { 0.0f, 0.0f };
		m_Particle.VelocityVariation = { 1.5f, 1.5f };
		m_Particle.Position = { 0.0f, 0.0f };
	}

	Hazel::FrameBufferSpecification frameBufferSpecification;
	frameBufferSpecification.Width = 1280;
	frameBufferSpecification.Height = 720;
	m_FrameBuffer = Hazel::FrameBuffer::Create(frameBufferSpecification);

	m_CheckerboardTexture = Hazel::Texture2D::Create("assets/textures/Checkerboard.png");
	m_SpriteSheet = Hazel::Texture2D::Create("assets/game/textures/RPGpack_sheet_2X.png");

	m_SpriteBush = Hazel::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 2, 3 }, { 128, 128 });
	m_SpriteBarrel = Hazel::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 8, 2 }, { 128, 128 });
	m_SpriteTree = Hazel::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 2, 1 }, { 128, 128 }, { 1, 2 });
}

void Sandbox2D::OnDetach() {
	HZ_PROFILE_FUNCTION();
}

void Sandbox2D::OnImGuiRender() {
	HZ_PROFILE_FUNCTION();

	// Note: Switch this to true to enable dockspace
	static bool dockingEnabled = true;
	if (dockingEnabled)
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

				if (ImGui::MenuItem("Exit")) Hazel::Application::Get().Close();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		ImGui::Begin("Settings");

		auto stats = Hazel::Renderer2D::GetStatistics();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

		uint32_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)textureID, ImVec2{ 1280, 720 });
		ImGui::End();

		ImGui::End();
	}
	else
	{
		ImGui::Begin("Settings");

		auto stats = Hazel::Renderer2D::GetStatistics();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

		uint32_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)textureID, ImVec2{ 1280, 720 });
		ImGui::End();
	}
}

void Sandbox2D::OnUpdate(Hazel::Timestep ts) {
	HZ_PROFILE_FUNCTION();

	// Update
	{
		HZ_PROFILE_SCOPE("OnUpdate:: CameraController");
		m_CameraController.OnUpdate(ts);
	}

	//Reset statistics
	Hazel::Renderer2D::ResetStatistics();

	{
		HZ_PROFILE_SCOPE("OnUpdate::Render::Prep");

		m_FrameBuffer->Bind();

		// Render preparation
		Hazel::RenderCommand::SetClearColor({ 0.07f, 0.07f, 0.07f, 1.0f });
		Hazel::RenderCommand::Clear();

	}
	{
		HZ_PROFILE_SCOPE("OnUpdate::Render::Draw");

		static float rotation = 0.0f;
		rotation += ts * 100.0f;

		// Render draw
		Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());
		Hazel::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.5f, 0.5f },  { 0.8f, 0.2f, 0.3f, 1.0f });
		Hazel::Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f }, { 0.5f, 0.5f }, glm::radians(-rotation), { 0.8f, 0.2f, 0.3f, 1.0f });
		Hazel::Renderer2D::DrawRotatedQuad({ 1.0f, 0.0f }, { 0.5f, 0.5f }, glm::radians(rotation), { 0.8f, 0.2f, 0.3f, 1.0f });
		Hazel::Renderer2D::DrawQuad({ 0.5f, -0.5f, 0.1f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 15.0f, 15.0f }, m_CheckerboardTexture, 15.0f);
		Hazel::Renderer2D::EndScene();

		Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());
		for (float y = -5.0; y <= 5.0f; y += 0.5f) {
			for (float x = -5.0; x <= 5.0f; x += 0.5f) {
				glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f };
				Hazel::Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
			}
		}
		Hazel::Renderer2D::EndScene();

		m_FrameBuffer->Unbind();
	}

	{
		Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());
		Hazel::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 1.0f, 1.0f }, m_SpriteBush);
		Hazel::Renderer2D::DrawQuad({  0.0f, 0.0f }, { 1.0f, 2.0f }, m_SpriteTree);
		Hazel::Renderer2D::DrawQuad({  1.0f, 0.0f }, { 1.0f, 1.0f }, m_SpriteBarrel);
		Hazel::Renderer2D::EndScene();
	}

	{
		if (Hazel::Input::IsMouseButtonPressed(HZ_MOUSE_BUTTON_LEFT))
		{
			auto [x, y] = Hazel::Input::GetMousePosition();

			auto width = Hazel::Application::Get().GetWindow().GetWidth();
			auto height = Hazel::Application::Get().GetWindow().GetHeight();

			auto bounds = m_CameraController.GetBounds();

			auto pos = m_CameraController.GetCamera().GetPosition();

			x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
			y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
			m_Particle.Position = { x + pos.x, y + pos.y };
			//m_Particle.Position = { pos.x, pos.y };
			for (int i = 0; i < 5; i++)
				m_ParticleSystem.Emit(m_Particle);
		}
	}

	{
		m_ParticleSystem.OnUpdate(ts);
		m_ParticleSystem.OnRender(m_CameraController.GetCamera());
	}
}

void Sandbox2D::OnEvent(Hazel::Event& event) {
	// Pass events to camera controller
	m_CameraController.OnEvent(event);
}
