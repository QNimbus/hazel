#pragma once

#include <glm/glm.hpp>

namespace Hazel{

	class CameraComp {
	public:
		CameraComp() = default;
		CameraComp(const glm::mat4& projection)
			: m_Projection(projection) {};

		//glm::mat4& GetProjection() { return m_Projection; }
		const glm::mat4& GetProjection() const { return m_Projection; }

	private:
		glm::mat4 m_Projection = glm::mat4{ 1.0f };
	};

	class Camera {
	public:
		virtual const glm::vec3& GetPosition() const = 0;
		virtual void SetPosition(const glm::vec3& position) = 0;

		virtual float GetRotation() const = 0;
		virtual void SetRotation(float rotation) = 0;

		virtual const glm::mat4& GetProjectionMatrix() const = 0;
		virtual const glm::mat4& GetViewMatrix() const = 0;
		virtual const glm::mat4& GetViewProjectionMatrix() const = 0;
	};

	class OrthographicCamera : public Camera {
	public:
		OrthographicCamera(float left, float right, float bottom, float top);

		void SetProjection(float left, float right, float bottom, float top);

		virtual const glm::vec3& GetPosition() const { return m_Position; }
		virtual void SetPosition(const glm::vec3& position) { m_Position = position; CalculateViewMatrix(); };

		virtual float GetRotation() const { return m_Rotation; }
		virtual void SetRotation(float rotation) { m_Rotation = rotation; CalculateViewMatrix(); }
		virtual const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		virtual const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		virtual const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

	private:
		void CalculateViewMatrix();

	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f};
		float m_Rotation = 0.0f;
	};

	class PerspectiveCamera : public Camera {
	public:
	private:
	};
}


