#pragma once

#include "hazel/renderer/Camera.h"

#include <ostream>

namespace Hazel {
	
	class SceneCamera : public CameraComp {
	public:
		enum class ProjectionType { Perspective = 0, Orthographic = 1 };

	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetOrthographic(float size, float nearClip, float farClip);
		void SetPerspective(float verticalFOV, float nearClip, float farClip);

		void SetViewportSize(uint32_t width, uint32_t height);


		const ProjectionType GetProjectionType() const { return m_ProjectionType; }
		void SetProjectionType(ProjectionType projectionType) { m_ProjectionType = projectionType; }

		// Perspective
		float GetPerspectiveVerticalFOV() const { return m_PerspectiveVerticalFOV; }
		float GetPerspectiveNearClip() const { return m_PerspectiveNear; };
		float GetPerspectiveFarClip()  const { return m_PerspectiveFar; };
		void SetPerspectiveVerticalFOV(float verticalFOV) { m_PerspectiveVerticalFOV = verticalFOV; }
		void SetPerspectiveNearClip(float nearClip) { m_PerspectiveNear = nearClip; CalculateProjection(); }
		void SetPerspectiveFarClip(float farClip) { m_PerspectiveFar = farClip; CalculateProjection(); }

		// Orthographic
		float GetOrthographicSize() const { return m_OrthographicSize; }
		float GetOrthographicNearClip() const { return m_OrthographicNear; };
		float GetOrthographicFarClip()  const { return m_OrthographicFar; };
		void SetOrthographicSize(float size) { m_OrthographicSize = size; CalculateProjection(); }
		void SetOrthograhpicNearClip(float nearClip) { m_OrthographicNear = nearClip; CalculateProjection(); }
		void SetOrthograhpicFarClip(float farClip) { m_OrthographicFar = farClip; CalculateProjection(); }

	private:
		void CalculateProjection();

	private:
		ProjectionType m_ProjectionType = ProjectionType::Orthographic;

		// Perspective
		float m_PerspectiveVerticalFOV = glm::radians(45.0f);
		float m_PerspectiveNear = 0.01f, m_PerspectiveFar = 1000.0f;

		// Orthographic
		float m_OrthographicSize = 10.0f;
		float m_OrthographicNear = -1.0f, m_OrthographicFar = 1.0f;
		float m_AspectRatio = 0.0f;
	};
}