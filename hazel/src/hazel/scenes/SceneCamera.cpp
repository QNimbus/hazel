#include "hzpch.h"
#include "hazel/scenes/SceneCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Hazel {

	SceneCamera::SceneCamera() {
		CalculatorProjection();
	}

	
	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip) {
		m_OrthographicSize = size;
		m_OrthographicNear = nearClip;
		m_OrthographicFar = farClip;

		CalculatorProjection();
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height) {
		m_AspectRatio = static_cast<float>(width) / static_cast<float>(height);

		CalculatorProjection();
	}

	void SceneCamera::CalculatorProjection() {
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


