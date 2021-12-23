#include "hzpch.h"
#include <Hazel.h>

class ExampleLayer : public Hazel::Layer {
public:
	ExampleLayer()
		: Layer("Example") {

	}

	void OnUpdate() override {
		// HZ_INFO("ExampleLayer::Update");
	}

	void OnEvent(const Hazel::Event& e) {
		HZ_TRACE("{0}", e);
	};
};

class Sandbox : public Hazel::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
	}

	~Sandbox() {

	}
};

Hazel::Application* Hazel::CreateApplication() {
	return new Sandbox();
}