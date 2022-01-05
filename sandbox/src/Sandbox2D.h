#pragma once

#include <Hazel.h>
#include "ParticleSystem.h"

class Sandbox2D : public Hazel::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnImGuiRender() override;
	virtual void OnUpdate(Hazel::Timestep ts) override;
	virtual void OnEvent(Hazel::Event& event) override;

private:
	Hazel::OrthographicCameraController m_CameraController;

	ParticleProps m_Particle;
	ParticleSystem m_ParticleSystem;

	// Temp
	/*Hazel::Ref<Hazel::Shader> m_FlatColorShader;
	Hazel::Ref<Hazel::VertexArray> m_SquareVertexArray;*/

	Hazel::Ref<Hazel::Texture2D> m_CheckerboardTexture;

	struct ProfileResult {
		const char* Name;
		float Time;
	};

	std::vector<ProfileResult> m_ProfileResults;

	//glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};