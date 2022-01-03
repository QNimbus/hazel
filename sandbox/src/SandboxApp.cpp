#include <Hazel.h>
// -- Entry Point ---------
#include <hazel/core/Entrypoint.h>
// -- Entry Point ---------

#include "Sandbox2D.h"
#include "GameLayer.h"

class Sandbox : public Hazel::Application {
public:
	Sandbox() {
		//PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
		//PushLayer(new GameLayer());
	}

	~Sandbox() {
	}
};

Hazel::Application* Hazel::CreateApplication() {
	return new Sandbox();
}