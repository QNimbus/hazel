#include "GameLayer.h"

using namespace Hazel;

GameLayer::GameLayer()
	: Layer("GameLayer") {
	auto& window = Application::Get().GetWindow();
	CreateCamera(window.GetWidth(), window.GetHeight());

	Random::Init();
}

void GameLayer::CreateCamera(uint32_t width, uint32_t height) {
	// Height -8.0 <--> +8.0

	float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
	float camHeight = 8.0f;
	float bottom = -camHeight;
	float top = camHeight;
	float left = bottom * aspectRatio;
	float right = top * aspectRatio;

	m_Camera = CreateScope<OrthographicCamera>(left, right, bottom, top);
}

void GameLayer::OnEvent(Event& event) {
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<WindowResizeEvent>(HZ_BIND_EVENT_FN(GameLayer::OnWindowResize));
	dispatcher.Dispatch<MouseButtonPressedEvent>(HZ_BIND_EVENT_FN(GameLayer::OnMouseButtonPressed));
}

void GameLayer::OnUpdate(Hazel::Timestep ts) {
}

void GameLayer::OnAttach() {
}

void GameLayer::OnDetach() {
}

bool GameLayer::OnWindowResize(WindowResizeEvent& event)
{
	CreateCamera(event.GetWidth(), event.GetHeight());
	return false;
}

bool GameLayer::OnMouseButtonPressed(Hazel::MouseButtonPressedEvent& event)
{
	return false;
}
