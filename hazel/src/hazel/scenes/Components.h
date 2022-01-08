#pragma once

#include "hazel/scenes/SceneCamera.h"
#include "hazel/scenes/ScriptableEntity.h"

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
