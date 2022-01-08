#include "hzpch.h"
#include "hazel/scenes/Scene.h"

#include "hazel/scenes/Entity.h"
#include "hazel/scenes/Components.h"
#include "hazel/renderer/Renderer2D.h"

#include <glm/glm.hpp>

namespace Hazel {
	
	Scene::Scene() {
	}

	Scene::~Scene() {
	}

	Entity Scene::CreateEntity(const std::string& name) {
		Entity entity { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>(name);

		tag.Tag = name.empty() ? "Entity" : name;

		return entity;
	}

	void Scene::OnUpdate(Timestep ts) {

		// Update scripts (and initialize if not exists - this is temporary)
		{
			m_Registry.view<NativeScriptComponent>().each([&, ts](auto entity, auto& scriptComponent) {
				if (scriptComponent.Instance == nullptr) {
					scriptComponent.CreateFunction();

					HZ_CORE_ASSERT(scriptComponent.Instance, "Error instantiating ScriptableEntity");
					
					static_cast<ScriptableEntity*>(scriptComponent.Instance)->m_Entity = Entity{ entity, this };

					if (scriptComponent.OnCreateFunction)
						scriptComponent.OnCreateFunction(scriptComponent.Instance);
				}
				if (scriptComponent.OnUpdateFunction)
					scriptComponent.OnUpdateFunction(scriptComponent.Instance, ts);
				});
		}

		// Render sprites
		CameraComp* mainCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;
		{
			auto group = m_Registry.group<CameraComponent>(entt::get<TransformComponent>);
			for (auto entity : group) {
				auto& [cameraComponent, transformComponent] = group.get<CameraComponent, TransformComponent>(entity);

				if (cameraComponent.Primary) {
					mainCamera = &cameraComponent.Camera;
					cameraTransform = &transformComponent.Transform;
					break;
				}
			}
		}

		if (mainCamera) {
			Renderer2D::BeginScene(mainCamera->GetProjection(), *cameraTransform);

			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group) {
				auto& [transformComponent, spriteComponent] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				Renderer2D::DrawQuad(transformComponent, spriteComponent.Color);
			}

			Renderer2D::EndScene();
		}		
	}
	
	void Scene::OnViewportResize(uint32_t width, uint32_t height) {
		// Resize our non-fixed aspect ratio cameras
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view) {
			auto& cameraComponent = view.get<CameraComponent>(entity);

			if (!cameraComponent.FixedAspectRatio) {
				cameraComponent.Camera.SetViewportSize(width, height);
			}
		}
	}
}
