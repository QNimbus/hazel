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
	Hazel::Ref<Hazel::Texture2D> m_CheckerboardTexture;
	Hazel::Ref<Hazel::Texture2D> m_SpriteSheet;
	Hazel::Ref<Hazel::SubTexture2D> m_SpriteBush, m_SpriteBarrel, m_SpriteTree;
	Hazel::Ref<Hazel::FrameBuffer> m_FrameBuffer;

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};