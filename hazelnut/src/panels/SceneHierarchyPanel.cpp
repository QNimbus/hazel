#include "hznpch.h"
#include "SceneHierarchyPanel.h"

#include <glm/gtc/type_ptr.hpp>

#include <imgui/imgui.h>

namespace Hazel {

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
		: m_Context(context) {
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context) {
		m_Context = context;
	}

	void SceneHierarchyPanel::OnImGuiRender() {
		ImGui::Begin("Scene Hierarchy");

		/*bool show = true;
		ImGui::ShowDemoWindow(&show);*/

		m_Context->m_Registry.each([&](auto entityId) {
			Entity entity{ entityId, m_Context.get()};

			DrawEntityNode(entity);

			});

		// Handle de-selection of items
		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()) {
			m_SelectionContext = {};
		}

		ImGui::End();

		ImGui::Begin("Properties");

		if (m_SelectionContext) {
			DrawComponents(m_SelectionContext);
		}

		ImGui::End();
	}

	void SceneHierarchyPanel::DrawComponents(const Entity& entity) {
		if (entity.HasComponent<TagComponent>()) {
			if (ImGui::TreeNodeEx("TagComponent", ImGuiTreeNodeFlags_DefaultOpen, "Tag")) {
				auto& tag = entity.GetComponent<TagComponent>().Tag;

				char buffer[256];
				memset(buffer, 0, sizeof(buffer));
				strcpy_s(buffer, sizeof(buffer), tag.c_str());
				//memcpy(buffer, tag.c_str(), tag.size());
				ImGuiInputTextFlags flags = 0; // ImGuiInputTextFlags_ReadOnly;
				if (ImGui::InputText("Tags", buffer, sizeof(buffer), flags)) {
					tag = std::string(buffer);
				}

				ImGui::TreePop();
			}
			ImGui::Separator();
		}

		if (entity.HasComponent<TransformComponent>()) {
			if (ImGui::TreeNodeEx("TransformComponent", ImGuiTreeNodeFlags_DefaultOpen, "Transform")) {
				auto& transform = entity.GetComponent<TransformComponent>().Transform;

				ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.1f);

				ImGui::TreePop();
			}
			ImGui::Separator();
		}

		if (entity.HasComponent<CameraComponent>()) {
			if (ImGui::TreeNodeEx("CameraComponent", ImGuiTreeNodeFlags_DefaultOpen, "Camera")) {
				auto& cameraComponent = entity.GetComponent<CameraComponent>();
				auto& camera = cameraComponent.Camera;

				const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
				const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];

				if (ImGui::BeginCombo("Projection type", currentProjectionTypeString)) {
					for (int i = 0; i < 2; i++) {
						bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
						if (ImGui::Selectable(projectionTypeStrings[i], isSelected)) {
							currentProjectionTypeString = projectionTypeStrings[i];
							camera.SetProjectionType(static_cast<SceneCamera::ProjectionType>(i));
						}

						if (isSelected) {
							ImGui::SetItemDefaultFocus();
						}
					}
					
					ImGui::EndCombo();
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective) {
					float perspectiveVOF = glm::degrees(camera.GetPerspectiveVerticalFOV());
					float perspectiveNear = camera.GetPerspectiveNearClip();
					float perspectiveFar = camera.GetPerspectiveFarClip();

					if (ImGui::DragFloat("FOV", &perspectiveVOF))
						camera.SetPerspectiveVerticalFOV(glm::radians(perspectiveVOF));

					if (ImGui::DragFloat("Near", &perspectiveNear))
						camera.SetPerspectiveNearClip(perspectiveNear);

					if (ImGui::DragFloat("Far", &perspectiveFar))
						camera.SetPerspectiveFarClip(perspectiveFar);
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic) {
					float orthoSize = camera.GetOrthographicSize();
					float orthoNear = camera.GetOrthographicNearClip();
					float orthoFar = camera.GetOrthographicFarClip();

					if (ImGui::DragFloat("Size", &orthoSize))
						camera.SetOrthographicSize(orthoSize);

					if (ImGui::DragFloat("Near", &orthoNear))
						camera.SetOrthograhpicNearClip(orthoNear);

					if (ImGui::DragFloat("Far", &orthoFar))
						camera.SetOrthograhpicFarClip(orthoFar);
				}

				ImGui::TreePop();
			}
			ImGui::Separator();
		}

		if (entity.HasComponent<SpriteRendererComponent>()) {
			if (ImGui::TreeNodeEx("SpriteRendererComponent", ImGuiTreeNodeFlags_DefaultOpen, "Sprite")) {
				auto& color = entity.GetComponent<SpriteRendererComponent>().Color;
				
				ImGui::ColorEdit4("", glm::value_ptr(color));

				ImGui::TreePop();
			}

			ImGui::Separator();
		}

	}

	void SceneHierarchyPanel::DrawEntityNode(const Entity& entity) {
		auto& tagComponent = entity.GetComponent<TagComponent>();
		auto& tag = tagComponent.Tag;

		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = entity;
		}

		if (opened)
		{

			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
			bool opened = ImGui::TreeNodeEx((void*)9817239, flags, tag.c_str());
			if (opened)
				ImGui::TreePop();
			ImGui::TreePop();
		}
	}
}