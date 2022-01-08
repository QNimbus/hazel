#include "hzpch.h"
#include "hazel/scenes/SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Hazel {

	SceneCamera::SceneCamera() {
		CalculateProjection();
	}


	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip) {
		m_ProjectionType = ProjectionType::Orthographic;

		m_OrthographicSize = size;
		m_OrthographicNear = nearClip;
		m_OrthographicFar = farClip;

		CalculateProjection();
	}

	void SceneCamera::SetPerspective(float verticalFOV, float nearClip, float farClip) {
		m_ProjectionType = ProjectionType::Perspective;

		m_PerspectiveVerticalFOV = verticalFOV;
		m_PerspectiveNear = nearClip;
		m_PerspectiveFar = farClip;

		CalculateProjection();
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height) {
		m_AspectRatio = static_cast<float>(width) / static_cast<float>(height);

		CalculateProjection();
	}

	void SceneCamera::CalculateProjection() {
		if (m_ProjectionType == ProjectionType::Perspective) {
			m_Projection = glm::perspective(m_PerspectiveVerticalFOV, m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
		}
		else {
			float orthoRight = m_OrthographicSize * m_AspectRatio * 0.5f;
			float orthoLeft = -m_OrthographicSize * m_AspectRatio * 0.5f;
			float orthoTop = m_OrthographicSize * 0.5f;
			float orthoBottom = -m_OrthographicSize * 0.5f;

			m_Projection = glm::ortho(
				orthoLeft,
				orthoRight,
				orthoBottom,
				orthoTop,
				m_OrthographicNear,
				m_OrthographicFar);
		}		
	}
}


