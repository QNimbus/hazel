#pragma once

#include <Hazel.h>

class GameLayer : public Hazel::Layer {
public:
	GameLayer();
	virtual ~GameLayer() = default;

	void OnEvent(Hazel::Event& event);
	void OnUpdate(Hazel::Timestep ts);
	void OnAttach();
	void OnDetach();

	bool OnWindowResize(Hazel::WindowResizeEvent& event);
	bool OnMouseButtonPressed(Hazel::MouseButtonPressedEvent& event);

private:
	void CreateCamera(uint32_t width, uint32_t height);

private:
	Hazel::Scope<Hazel::OrthographicCamera> m_Camera;
};