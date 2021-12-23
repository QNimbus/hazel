#include "hzpch.h"
#include "Application.h"

#include <glad/glad.h>

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

namespace Hazel {

	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
	}

	Application::~Application()
	{
	}

	void Application::PushLayer(Layer* layer) {
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* overlay) {
		m_LayerStack.PushOverlay(overlay);
	}

	void Application::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);

		// Dispatch a WindowCloseEvent to the Application::OnWindowClose event handler
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));

		// Pass events to layers in the layer stack
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); ) {
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::Run()
	{
		while (m_Running) {
			glClearColor(0.17f, 0.1f, 0.2f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			
			// Update layers
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			// Update window
			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e) {
		m_Running = false;
		return true;
	}
}