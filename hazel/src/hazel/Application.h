#pragma once

#include "Core.h"

#include "Window.h"
#include "hazel/LayerStack.h"
#include "hazel/events/Event.h"
#include "hazel/imgui/ImGuiLayer.h"
#include "hazel/events/ApplicationEvent.h"

namespace Hazel {
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);

	private:
		ImGuiLayer* m_ImGuiLayer;
		LayerStack m_LayerStack;
		std::unique_ptr<Window> m_Window;

		bool m_Running = true;

	private:
		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}