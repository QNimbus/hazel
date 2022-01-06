#pragma once

#include "Core.h"

#include "Window.h"
#include "hazel/core/LayerStack.h"
#include "hazel/events/Event.h"
#include "hazel/imgui/ImGuiLayer.h"
#include "hazel/events/ApplicationEvent.h"

#include "hazel/core/Timestep.h"

#include "hazel/renderer/Shader.h"
#include "hazel/renderer/Buffer.h"
#include "hazel/renderer/VertexArray.h"

namespace Hazel {
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
		void Close();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }

	private:
		bool OnWindowClose(WindowCloseEvent& event);
		bool OnWindowResize(WindowResizeEvent& event);

	private:
		ImGuiLayer* m_ImGuiLayer;
		LayerStack m_LayerStack;
		std::unique_ptr<Window> m_Window;

		bool m_Running = true;
		bool m_Minimized = false;
		float m_LastFrameTime = 0.0f;

	private:
		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}