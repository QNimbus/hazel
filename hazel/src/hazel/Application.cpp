#include "hzpch.h"
#include "Application.h"

#include "hazel/events/ApplicationEvent.h"
#include "hazel/Log.h"

#include "GLFW/glfw3.h"

namespace Hazel {

	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
	}

	Application::~Application()
	{
	}

	void Application::run()
	{
		while (m_Running) {
			glClearColor(0.17f, 0.1f, 0.2f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Window->OnUpdate();
		}
	}
}