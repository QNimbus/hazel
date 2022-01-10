#include "hzpch.h"
#include "hazel/scenes/Scene.h"

#include "hazel/scenes/Entity.h"
#include "hazel/scenes/Components.h"
#include "hazel/renderer/Renderer2D.h"

#include <glm/glm.hpp>

namespace Hazel {
	
	Scene::Scene(const std::string& name /*= "Untitled scene"*/)
		: m_SceneName(name) {}

	Scene::~Scene() {
	}

	Entity Scene::CreateEntity(const std::string& name) {
		Entity entity { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>(name);

		tag.Tag = name.empty() ? "Entity" : name;

		return entity;
	}

	void Scene::DestroyEntity(Entity entity) {
		m_Registry.destroy(entity);
	}

	void Scene::OnUpdate(Timestep ts) {

		// Update scripts (and initialize if not exists - this is temporary)
		{
			m_Registry.view<NativeScriptComponent>().each([&, ts](auto entity, auto& nsc) {
				// TODO: Move to Scene::OnScenePlay
				{
					if (nsc.Instance == nullptr) {
						nsc.Instance = nsc.CreateScript();

						HZ_CORE_ASSERT(nsc.Instance, "Error instantiating ScriptableEntity");

						nsc.Instance->m_Entity = Entity{ entity, this };						
						nsc.Instance->OnCreate();
					}
				}				

				nsc.Instance->OnUpdate(ts);
				});
		}

		// Render sprites
		CameraComp* mainCamera = nullptr;
		glm::mat4 cameraTransform;
		{
			auto group = m_Registry.group<CameraComponent>(entt::get<TransformComponent>);
			for (auto entity : group) {
				auto [cameraComponent, transformComponent] = group.get<CameraComponent, TransformComponent>(entity);

				if (cameraComponent.Primary) {
					mainCamera = &cameraComponent.Camera;
					cameraTransform = transformComponent.GetTransform();
					break;
				}
			}
		}

		if (mainCamera) {
			Renderer2D::BeginScene(*mainCamera, cameraTransform);

			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group) {
				auto [transformComponent, spriteComponent] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				Renderer2D::DrawQuad(transformComponent.GetTransform(), spriteComponent.Color);
			}

			Renderer2D::EndScene();
		}		
	}
	
	void Scene::OnViewportResize(uint32_t width, uint32_t height) {
		// Store viewport dimensions
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		// Resize our non-fixed aspect ratio cameras
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view) {
			auto& cameraComponent = view.get<CameraComponent>(entity);

			if (!cameraComponent.FixedAspectRatio) {
				cameraComponent.Camera.SetViewportSize(width, height);
			}
		}
	}

	Entity Scene::GetPrimaryCameraEntity() {
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view) {
			const auto& cameraComponent = view.get<CameraComponent>(entity);

			if (cameraComponent.Primary)
				return Entity{entity, this};
		}

		return {};
	}

	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component) {
		static_assert(false);
	}

	template<>
	void Scene::OnComponentAdded(Entity entity, TransformComponent& component) {
	}

	template<>
	void Scene::OnComponentAdded(Entity entity, SpriteRendererComponent& component) {
	}

	template<>
	void Scene::OnComponentAdded(Entity entity, CameraComponent& component) {
		component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}

	template<>
	void Scene::OnComponentAdded(Entity entity, NativeScriptComponent& component) {
	}

	template<>
	void Scene::OnComponentAdded(Entity entity, TagComponent& component) {
	}
}
