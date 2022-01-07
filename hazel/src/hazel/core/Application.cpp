#include "hzpch.h"
#include "hazel/core/Application.h"

#include "hazel/renderer/Renderer.h"

// Temporary

#include <GLFW/glfw3.h>

namespace Hazel {

	Application* Application::s_Instance = nullptr;

	Application::Application(const std::string& name) {
		HZ_CORE_ASSERT(!s_Instance, "Application already instantiated");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create(WindowProps(name)));

		// Send all Window events to the application object so they can be handled by the application
		m_Window->SetEventCallback(HZ_BIND_EVENT_FN(Application::OnEvent));
		m_Window->SetVSync(true);

		Renderer::Init();

		#ifdef HZ_DEBUG
		// Create ImGuiLayer -- Debug only
		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
		#endif
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
		dispatcher.Dispatch<WindowCloseEvent>(HZ_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(HZ_BIND_EVENT_FN(Application::OnWindowResize));

		// Pass events to layers in the layer stack
		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it) {
			if (e.Handled)
				break;
			(*it)->OnEvent(e);
		}
	}

	void Application::Run()
	{
		while (m_Running) {
			float time = static_cast<float>(glfwGetTime()); // Platform::GetTime()
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimized) {
				// Update layers
				for (Layer* layer : m_LayerStack)
					layer->OnUpdate(timestep);
			}			

			#ifdef HZ_DEBUG
			// ImGui -- debug only
			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();
			#endif

			// Update window
			m_Window->OnUpdate();
		}
	}

	void Application::Close()
	{
		m_Running = false;
	}

	bool Application::OnWindowClose(WindowCloseEvent& event) {
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& event) {
		if (event.GetWidth() == 0 || event.GetHeight() == 0) {
			m_Minimized = true;
			return false;
		}
		
		m_Minimized = false;
		Renderer::OnWindowResize(event.GetWidth(), event.GetHeight());
		return false;
	}
}