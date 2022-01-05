#include "ParticleSystem.h"

//#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>

ParticleSystem::ParticleSystem(uint32_t poolSize)
	: m_PoolIndex(poolSize - 1) {
	m_ParticlePool.resize(poolSize);
}

void ParticleSystem::OnUpdate(Hazel::Timestep ts) {
	for (auto& particle : m_ParticlePool)
	{
		if (!particle.Active)
			continue;

		if (particle.LifeRemaining <= 0.0f)
		{
			particle.Active = false;
			continue;
		}

		particle.LifeRemaining -= ts;
		particle.Position += particle.Velocity * (float)ts;
		particle.Rotation += 0.5f * ts;
	}
}

void ParticleSystem::OnRender(Hazel::OrthographicCamera& camera) {
	Hazel::Renderer2D::BeginScene(camera);
	for (auto& particle : m_ParticlePool)
	{
		if (!particle.Active)
			continue;

		// Calculate particle age
		float life = particle.LifeRemaining / particle.LifeSpan;

		// Fade and shrink away particles
		glm::vec4 color = glm::lerp(particle.ColorEnd, particle.ColorBegin, life);
		float size = glm::lerp(particle.SizeEnd, particle.SizeBegin, life);

		// Render
		Hazel::Renderer2D::DrawRotatedQuad({ particle.Position.x, particle.Position.y, 0.9f }, { size, size }, particle.Rotation, color);		
	}
	Hazel::Renderer2D::EndScene();
}

void ParticleSystem::Emit(const ParticleProps& particleProps) {
	// Get particle reference
	Particle& particle = m_ParticlePool[m_PoolIndex];

	// Activate particle
	particle.Active = true;
	particle.Position = particleProps.Position;
	particle.Rotation = Hazel::Random::Float() * 2.0f * glm::pi<float>();

	// Velocity
	particle.Velocity = particleProps.Velocity;
	particle.Velocity.x += particleProps.VelocityVariation.x * (Hazel::Random::Float() - 0.5f); // Variation * [-0.5 -- 0.5]
	particle.Velocity.y += particleProps.VelocityVariation.y * (Hazel::Random::Float() - 0.5f); // Variation * [-0.5 -- 0.5]

	// Color
	particle.ColorBegin = particleProps.ColorBegin;
	particle.ColorEnd = particleProps.ColorEnd;

	// Size
	particle.SizeBegin = particleProps.SizeBegin + (particleProps.SizeVariation * (Hazel::Random::Float() - 0.5f)); // Variation * [-0.5 -- 0.5]
	particle.SizeEnd = particleProps.SizeEnd;

	// Lifespan
	particle.LifeSpan = particleProps.LifeSpan;
	particle.LifeRemaining = particleProps.LifeSpan;

	size_t size = m_ParticlePool.size();
	m_PoolIndex = static_cast<int>((--m_PoolIndex + size) % size);
}