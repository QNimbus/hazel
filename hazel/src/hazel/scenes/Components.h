#pragma once

#include "hazel/scenes/SceneCamera.h"
#include "hazel/scenes/ScriptableEntity.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Hazel {

	struct TagComponent {
		std::string Tag;

		TagComponent() = default; // Constructor
		TagComponent(const TagComponent&) = default; // Copy constructor
		TagComponent(const std::string tag)
			: Tag(tag) {}
	};

	struct TransformComponent {
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation    = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale       = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default; // Constructor
		TransformComponent(const TransformComponent&) = default; // Copy constructor
		TransformComponent(const glm::vec3& translation)
			: Translation(translation) {}

		glm::mat4 GetTransform() const {
			/*glm::mat4 rotation =
				  glm::rotate(glm::mat4(1.0f), Rotation.x, { 1.0f, 0.0f, 0.0f })
				* glm::rotate(glm::mat4(1.0f), Rotation.y, { 0.0f, 1.0f, 0.0f })
				* glm::rotate(glm::mat4(1.0f), Rotation.z, { 0.0f, 0.0f, 1.0f });*/

			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

			glm::mat4 transform = glm::translate(glm::mat4(1.0f), Translation)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);

			return transform;
		}
	};

	struct SpriteRendererComponent {
		glm::vec4 Color { 1.0f, 1.0f, 1.0f, 1.0f };

		SpriteRendererComponent() = default; // Constructor
		SpriteRendererComponent(const SpriteRendererComponent&) = default; // Copy constructor
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color) {}
	};

	struct CameraComponent {
		SceneCamera Camera;
		bool Primary = true; // TODO: Think about moving to scene
		bool FixedAspectRatio = false;

		CameraComponent() = default; // Constructor
		CameraComponent(const CameraComponent&) = default; // Copy constructor
	};

	struct NativeScriptComponent {
		ScriptableEntity* Instance = nullptr;

		ScriptableEntity* (*CreateScript)(void);
		void (*DestroyScript)(NativeScriptComponent*);

		template<typename T>
		void Bind() {
			CreateScript = []() { return static_cast<ScriptableEntity*>(new T());  };
			DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
		};

		#if OLD_CODE
		std::function<void()> CreateFunction;
		std::function<void()> DestroyFunction;

		 //To avoid using virtual functions
		std::function<void(ScriptableEntity*)> OnCreateFunction;
		std::function<void(ScriptableEntity*)> OnDestroyFunction;
		std::function<void(ScriptableEntity*, Timestep)> OnUpdateFunction;

		template<typename T>
		void Bind() {
			CreateFunction = [&]() { Instance = new T();  };
			DestroyFunction = [&]() { delete static_cast<T*>(Instance); Instance = nullptr; };

			OnCreateFunction = [](ScriptableEntity* instance) { static_cast<T*>(instance)->OnCreate(); };
			OnDestroyFunction = [](ScriptableEntity* instance) { static_cast<T*>(instance)->OnDestroy(); };
			OnUpdateFunction = [](ScriptableEntity* instance, Timestep ts) { static_cast<T*>(instance)->OnUpdate(ts); };
		};
		#endif
	};
}
