#pragma once

#include "hazel/Layer.h"

#include "hazel/events/KeyEvent.h"
#include "hazel/events/MouseEvent.h"
#include "hazel/events/ApplicationEvent.h"

namespace Hazel {
	class HAZEL_API ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach();
		void OnDetach();
		void OnUpdate();
		void OnEvent(Event& event);

		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& event);
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& event);
		bool OnMouseMovedEvent(MouseMovedEvent& event);
		bool OnMouseScrolledEvent(MouseScrolledEvent& event);
		bool OnKeyPressedEvent(KeyPressedEvent& event);
		bool OnKeyReleasedEvent(KeyReleasedEvent& event);
		bool OnKeyTypedEvent(KeyTypedEvent& event);

		bool OnWindowResizeEvent(WindowResizeEvent& event);

	private:
		float m_Time = 0;
	};
}