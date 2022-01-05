#include "Sandbox2D.h"

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

#include "platform/OpenGL/OpenGLShader.h"

#include <chrono>

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, true)
{
}

void Sandbox2D::OnAttach() {
	m_CheckerboardTexture = Hazel::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach() {

}

void Sandbox2D::OnImGuiRender() {
	HZ_PROFILE_FUNCTION();

	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square color", glm::value_ptr(this->m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnUpdate(Hazel::Timestep ts) {
	HZ_PROFILE_FUNCTION();

	// Update
	{
		HZ_PROFILE_SCOPE("OnUpdate:: CameraController");
		m_CameraController.OnUpdate(ts);
	}

	{
		HZ_PROFILE_SCOPE("OnUpdate::Render::Prep");

		// Render preparation
		Hazel::RenderCommand::SetClearColor({ 0.07f, 0.07f, 0.07f, 1.0f });
		Hazel::RenderCommand::Clear();

	}
	{
		HZ_PROFILE_SCOPE("OnUpdate::Render::Draw");

		// Render draw
		Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());
		Hazel::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f },  { 0.8f, 0.2f, 0.3f, 1.0f });
		Hazel::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		Hazel::Renderer2D::DrawQuad({ -2.0f, -1.0f, -0.1f }, { 4.0f, 4.0f }, m_CheckerboardTexture, 10.0f, glm::vec4(1.0f, 0.8f, 0.8f, 1.0f));
		//Hazel::Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, glm::radians(80.0f), m_CheckerboardTexture, 15.0f, glm::vec4(1.0f, 0.8f, 0.8f, 1.0f));
		Hazel::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnEvent(Hazel::Event& event) {
	// Pass events to camera controller
	m_CameraController.OnEvent(event);
}
