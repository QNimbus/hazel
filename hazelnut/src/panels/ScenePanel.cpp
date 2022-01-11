#include "hznpch.h"
#include "ScenePanel.h"

#include <glm/gtc/type_ptr.hpp>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

namespace Hazel {

	ScenePanel::ScenePanel(const Ref<Scene>& context)
		: m_Context(context) {
	}

	void ScenePanel::SetContext(const Ref<Scene>& context) {
		// Deselect any previously selected entities to prevent error
		m_SelectionContext = { };

		m_Context = context;
	}

	void ScenePanel::OnImGuiRender() {	
		// Scene panel
		RenderScenePanel();

		// Properties panel
		RenderPropertiesPanel();
	}

	void ScenePanel::RenderScenePanel() {
		ImGui::Begin("Scene");

		std::string sceneName = m_Context->m_SceneName;

		// Render: Top level scene
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
		flags |= m_SelectionContext == m_Context ? ImGuiTreeNodeFlags_Selected : 0;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)m_Context.get(), flags, sceneName.c_str());

		// Right-click scene for context menu
		if (ImGui::BeginPopupContextItem()) {
			if (ImGui::MenuItem("Add new entity")) {
				{
					// Ensure item is selected after creation
					m_SelectionContext = { m_Context->CreateEntity("New entity") };

					// Ensure focus for new entity tag field in properties panel					
					m_NewEntity = true;

					ImGui::CloseCurrentPopup();
				}
			}

			ImGui::EndPopup();
		}

		// Scene selection
		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = { m_Context };
		}

		// Scene tree item expanded
		if (opened) {
			// Create namespace to avoid collisions
			ImGui::PushID(sceneName.c_str());

			// Render: Scene entities
			m_Context->m_Registry.each([&](auto entityId) {
			Entity entity{ entityId, m_Context.get() };

			DrawEntityNode(entity);

			});

			ImGui::PopID();

			ImGui::TreePop();
		}

		// Handle de-selection of items
		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()) {
			m_SelectionContext = {};
		}

		ImGui::End();
	}

	void ScenePanel::RenderPropertiesPanel() {
		ImGui::Begin("Properties");

		if (m_SelectionContext.is<Entity>())
		{
			DrawComponents(std::get<Entity>(m_SelectionContext));
		} else
		if (m_SelectionContext.is<Ref<Scene>>())
		{
			DrawSceneProperties(std::get<Ref<Scene>>(m_SelectionContext));
		}

		ImGui::End();
	}

	template<typename T, typename UIFunction>
	static void ScenePanel::DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction) {
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding;
		if (entity.HasComponent<T>()) {
			bool removeComponent = false;
			ImVec2 contentRegionAvail = ImGui::GetContentRegionAvail();
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

			ImGui::PushID(name.c_str());
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });

			ImGui::Separator();

			bool open = ImGui::TreeNodeEx("Component", treeNodeFlags, name.c_str());

			ImGui::PopStyleVar();

			ImGui::SameLine(contentRegionAvail.x - lineHeight * 0.5f);

			if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
				ImGui::OpenPopup("ComponentSettings");

			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove component")) {
					removeComponent = true;
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}

			if (open) {
				auto& component = entity.GetComponent<T>();

				uiFunction(component);

				ImGui::TreePop();
			}

			if (removeComponent)
				entity.RemoveComponent<T>();

			ImGui::PopID();
		}
	}

	static bool DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f, const char* format = "%.2f") {
		bool changed = false;
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[1];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);

		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		// Buttons
		{
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };


			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.20f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			ImGui::PushFont(boldFont);
			if (ImGui::Button("X", buttonSize)) {
				values.x = resetValue;
				changed = true;
			}
			ImGui::PopFont();
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			if (ImGui::DragFloat("##X", &values.x, 0.01f, 0.0f, 0.0f, format))
				changed = true;
			ImGui::PopItemWidth();
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
			ImGui::PushFont(boldFont);
			if (ImGui::Button("Y", buttonSize)) {
				values.y = resetValue;
				changed = true;
			}
			ImGui::PopFont();
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			if (ImGui::DragFloat("##Y", &values.y, 0.01f, 0.0f, 0.0f, format))
				changed = true;
			ImGui::PopItemWidth();
			ImGui::SameLine();

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
			ImGui::PushFont(boldFont);
			if (ImGui::Button("Z", buttonSize)) {
				values.z = resetValue;
				changed = true;
			}
			ImGui::PopFont();
			ImGui::PopStyleColor(3);

			ImGui::SameLine();
			if (ImGui::DragFloat("##Z", &values.z, 0.01f, 0.0f, 0.0f, format))
				changed = true;
			ImGui::PopItemWidth();
		}

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();

		return changed;
	}

	void ScenePanel::DrawSceneProperties(const Ref<Scene>& scene) {
		ImGuiIO& io = ImGui::GetIO();
		auto monospaceFont = io.Fonts->Fonts[2];
		float firstColumnWidth = 180.0f;

		// Scene guid
		ImGui::SetNextItemWidth(firstColumnWidth);
		ImGui::Text("Scene uuid");

		ImGui::SameLine(firstColumnWidth);
		ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);

		ImGui::PushFont(monospaceFont);
		ImGui::Text("ebc40900-f3f3-445e-8615-278fb9e900da");
		ImGui::PopFont();

		// Scene name
		ImGui::SetNextItemWidth(firstColumnWidth);
		ImGui::Text("Scene name");
		
		ImGui::SameLine(firstColumnWidth);
		ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);

		ImGuiInputTextFlags flags = 0;

		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		strcpy_s(buffer, sizeof(buffer), scene->m_SceneName.c_str());

		if (ImGui::InputText("##Scene name", buffer, sizeof(buffer), flags)) {
			scene->m_SceneName = std::string(buffer);
		}
	}

	void ScenePanel::DrawComponents(Entity entity) {
		if (entity.HasComponent<TagComponent>()) {
			auto& tag = entity.GetComponent<TagComponent>().Tag;
			ImGuiInputTextFlags flags = 0;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());

			// If we just created this entity - set keyboard focus
			if (m_NewEntity) {
				m_NewEntity = false;
				ImGui::SetKeyboardFocusHere(0);

			}

			flags |= ImGuiInputTextFlags_AutoSelectAll;
			if (ImGui::InputText("##Tags", buffer, sizeof(buffer), flags)) {
				tag = std::string(buffer);
			}
		}

		{
			ImGui::SameLine();
			ImGui::PushItemWidth(-1);

			if (ImGui::Button("Add component")) {
				ImGui::OpenPopup("AddComponent");
			}

			if (ImGui::BeginPopup("AddComponent")) {
				if (ImGui::MenuItem("Camera")) {
					Entity selectedEntity = std::get<Entity>(m_SelectionContext);
					selectedEntity.AddComponent<CameraComponent>();
					ImGui::CloseCurrentPopup();
				}

				if (ImGui::MenuItem("Sprite")) {
					Entity selectedEntity = std::get<Entity>(m_SelectionContext);
					selectedEntity.AddComponent<SpriteRendererComponent>();
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}

			ImGui::PopItemWidth();
		}

		DrawComponent<CameraComponent>("Camera", entity, [&](CameraComponent& component) {
			auto& camera = component.Camera;

			const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
			const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];

			if (ImGui::Checkbox("Primary", &component.Primary)) {
				auto& view = m_Context->m_Registry.view<CameraComponent>();
				for (auto& e : view) {
					if (entity != e) {
						auto& cam = view.get<CameraComponent>(e);
						cam.Primary = false;
					}
				}
			}

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

				ImGui::Checkbox("Fixed aspect ratio", &component.FixedAspectRatio);
			}
			});

		DrawComponent<TransformComponent>("Transform", entity, [](TransformComponent& component) {
			auto& rotation = glm::degrees(component.Rotation);

			DrawVec3Control("Translation", component.Translation);
			if (DrawVec3Control("Rotation", rotation))
				component.Rotation = glm::radians(rotation);
			DrawVec3Control("Scale", component.Scale, 1.0f);
			});

		DrawComponent<SpriteRendererComponent>("Sprite", entity, [](SpriteRendererComponent& component) {
			ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
			});

		DrawComponent<NativeScriptComponent>("Script", entity, [](NativeScriptComponent& component) {

			});
	}

	void ScenePanel::DrawEntityNode(Entity entity) {
		auto& tagComponent = entity.GetComponent<TagComponent>();
		auto& tag = tagComponent.Tag;
		bool deleteEntity = false;

		ImGuiTreeNodeFlags flags = (m_SelectionContext == entity ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = { entity };
		}

		if (ImGui::BeginPopupContextItem()) {
			if (ImGui::MenuItem("Delete entity"))
				deleteEntity = true;

			ImGui::EndPopup();
		}

		if (opened)
		{

			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
			flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
			bool opened = ImGui::TreeNodeEx((void*)9817239, flags, "children");
			if (opened)
				ImGui::TreePop();
			ImGui::TreePop();
		}

		if (deleteEntity) {
			// If the entity we are about to delete is 'selected' - unselect it first
			if (m_SelectionContext == entity)
				m_SelectionContext = {};

			m_Context->DestroyEntity(entity);
		}
	}
}