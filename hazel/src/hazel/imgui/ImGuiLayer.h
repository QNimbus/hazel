#pragma once

#include "hazel/Layer.h"

#include "hazel/events/KeyEvent.h"
#include "hazel/events/MouseEvent.h"
#include "hazel/events/ApplicationEvent.h"

namespace Hazel {
	class ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();

	private:
		float m_Time = 0.0f;
	};
}