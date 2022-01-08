#pragma once

#include "hazel/renderer/Camera.h"

#include <glm/glm.hpp>

namespace Hazel {

	struct TagComponent {
		std::string Tag;

		TagComponent() = default; // Constructor
		TagComponent(const TagComponent&) = default; // Copy constructor
		TagComponent(const std::string tag)
			: Tag(tag) {}
	};

	struct TransformComponent {
		glm::mat4 Transform { 1.0f };

		TransformComponent() = default; // Constructor
		TransformComponent(const TransformComponent&) = default; // Copy constructor
		TransformComponent(const glm::mat4& transform)
			: Transform(transform) {}

		operator glm::mat4& () { return Transform; }
		operator const glm::mat4& () const { return Transform; }
	};

	struct SpriteRendererComponent {
		glm::vec4 Color { 1.0f, 1.0f, 1.0f, 1.0f };

		SpriteRendererComponent() = default; // Constructor
		SpriteRendererComponent(const SpriteRendererComponent&) = default; // Copy constructor
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color) {}
	};

	struct CameraComponent {
		Hazel::CameraComp Camera;
		bool Primary = true; // TODO: Think about moving to scene

		CameraComponent() = default; // Constructor
		CameraComponent(const CameraComponent&) = default; // Copy constructor
		CameraComponent(const glm::mat4& projection)
			: Camera(projection) {}

	};
}
