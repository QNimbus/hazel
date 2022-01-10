#include "hzpch.h"
#include "SceneSerializer.h"

#include "hazel/scenes/Entity.h"
#include "hazel/scenes/Components.h"

#include <glm/gtx/string_cast.hpp>

#include <yaml-cpp/yaml.h>

namespace YAML {

	template<>
	struct convert<glm::vec3> {
		static Node encode(const glm::vec3& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs) {
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4> {
		static Node encode(const glm::vec4& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs) {
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};
}

namespace Hazel {

	// Output stream operator for glm::vec3 (alternative to glm::to_string()
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v) {
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	// Output stream operator for glm::vec4 (alternative to glm::to_string()
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v) {
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
		: m_Scene(scene) {}

	static void SerializeEntity(YAML::Emitter& out, Entity entity) {
		out << YAML::BeginMap; // Entity

		out << YAML::Key << "Entity" << YAML::Value << "ebc40900-f3f3-445e-8615-278fb9e900da"; // TODO: Entity GUID here

		// Serialize TagComponent
		if (entity.HasComponent<TagComponent>()) {
			auto& component = entity.GetComponent<TagComponent>();

			out << YAML::Key << "TagComponent" << YAML::BeginMap;
			out << YAML::Key << "Tag" << YAML::Value << component.Tag;
			out << YAML::EndMap;
		}

		// Serialize TransformComponent
		if (entity.HasComponent<TransformComponent>()) {
			auto& component = entity.GetComponent<TransformComponent>();

			out << YAML::Key << "TransformComponent" << YAML::BeginMap;
			out << YAML::Key << "Translation" << YAML::Value << component.Translation << YAML::Comment(glm::to_string(component.Translation));
			out << YAML::Key << "Rotation" << YAML::Value << component.Rotation << YAML::Comment(glm::to_string(component.Rotation));
			out << YAML::Key << "Scale" << YAML::Value << component.Scale << YAML::Comment(glm::to_string(component.Scale));
			out << YAML::EndMap;
		}

		// Serialize CameraComponent
		if (entity.HasComponent<CameraComponent>()) {
			auto& component = entity.GetComponent<CameraComponent>();

			out << YAML::Key << "CameraComponent" << YAML::BeginMap;
			out << YAML::Key << "Primary" << YAML::Value << component.Primary;
			out << YAML::Key << "FixedAspectRatio" << YAML::Value << component.FixedAspectRatio;

			// Camera map
			auto& camera = component.Camera;
			out << YAML::Key << "Camera" << YAML::BeginMap;
			out << YAML::Key << "OrthographicNearClip" << YAML::Value << camera.GetOrthographicNearClip();
			out << YAML::Key << "OrthographicFarClip" << YAML::Value << camera.GetOrthographicFarClip();
			out << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize();
			out << YAML::Key << "PerspectiveNearClip" << YAML::Value << camera.GetPerspectiveNearClip();
			out << YAML::Key << "PerspectiveFarClip" << YAML::Value << camera.GetPerspectiveFarClip();
			out << YAML::Key << "PerspectiveVerticalFOV" << YAML::Value << camera.GetPerspectiveVerticalFOV();
			out << YAML::Key << "ProjectionType" << YAML::Value << static_cast<int>(camera.GetProjectionType()) << YAML::Comment((camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective ? " = Perspective" : "= Orthographic"));
			out << YAML::EndMap;

			out << YAML::EndMap;
		}

		// Serialize SpriteRendererComponent
		if (entity.HasComponent<SpriteRendererComponent>()) {
			auto& component = entity.GetComponent<SpriteRendererComponent>();

			out << YAML::Key << "SpriteRendererComponent" << YAML::BeginMap;
			out << YAML::Key << "Color" << YAML::Value << component.Color << YAML::Comment(glm::to_string(component.Color));
			out << YAML::EndMap;
		}

		out << YAML::EndMap; // Entity
	}

	void SceneSerializer::Serialize(const std::string& filepath) {
		HZ_PROFILE_FUNCTION();

		YAML::Emitter out;

		out << YAML::BeginDoc;

		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << m_Scene->m_SceneName;

		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		m_Scene->m_Registry.each([&](auto entityID) {
			Entity entity = { entityID, m_Scene.get() };

			if (!entity)
				return;

			SerializeEntity(out, entity);
			});

		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();

		HZ_CORE_TRACE("Serializing scene '{0}' to file '{1}'", m_Scene->m_SceneName, filepath);
	}

	void SceneSerializer::SerializeRuntime(const std::string& filepath) {
		// Not implemented
		HZ_CORE_ASSERT(false);
	}

	bool SceneSerializer::Deserialize(const std::string& filepath) {
		HZ_PROFILE_FUNCTION();

		std::ifstream stream(filepath);
		std::stringstream stringStream;
		stringStream << stream.rdbuf();

		YAML::Node data = YAML::Load(stringStream.str());
		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();

		HZ_CORE_TRACE("Deserializing scene '{0}' from file '{1}'", m_Scene->m_SceneName, filepath);

		auto yamlEntities = data["Entities"];
		if (yamlEntities) {
			for (auto yamlEntity : yamlEntities) {
				// Create entity
				Entity deserializedEntity(m_Scene->m_Registry.create(), m_Scene.get());

				// uint64_t uuid = yamlEntity["Entity"].as<uint64_t>(); // TODO
				std::string uuid = yamlEntity["Entity"].as<std::string>();
				std::string entityName;

				auto tagComponent = yamlEntity["TagComponent"];
				auto transformComponent = yamlEntity["TransformComponent"];
				auto cameraComponent = yamlEntity["CameraComponent"];
				auto spriteRendererComponent = yamlEntity["SpriteRendererComponent"];

				// Entities (should) always have a TagComponent
				if (tagComponent) {
					entityName = tagComponent["Tag"].as<std::string>();

					deserializedEntity.AddComponent<TagComponent>(entityName);
				}

				// Entities (should) always have a TransformComponent
				if (transformComponent) {
					TransformComponent component;
					component.Translation = transformComponent["Translation"].as<glm::vec3>();
					component.Rotation    = transformComponent["Rotation"].as<glm::vec3>();
					component.Scale       = transformComponent["Scale"].as<glm::vec3>();

					deserializedEntity.AddComponent<TransformComponent>(component);
				}

				if (cameraComponent) {
					CameraComponent component;
					auto& cameraProps = cameraComponent["Camera"];

					component.Primary = cameraComponent["Primary"].as<bool>();
					component.FixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();

					component.Camera.SetOrthograhpicNearClip(cameraProps["OrthographicNearClip"].as<float>());
					component.Camera.SetOrthograhpicFarClip(cameraProps["OrthographicFarClip"].as<float>());
					component.Camera.SetOrthographicSize(cameraProps["OrthographicSize"].as<float>());

					component.Camera.SetPerspectiveNearClip(cameraProps["PerspectiveNearClip"].as<float>());
					component.Camera.SetPerspectiveFarClip(cameraProps["PerspectiveFarClip"].as<float>());
					component.Camera.SetPerspectiveVerticalFOV(cameraProps["PerspectiveVerticalFOV"].as<float>());

					component.Camera.SetProjectionType(static_cast<SceneCamera::ProjectionType>(cameraProps["ProjectionType"].as<int>()));

					deserializedEntity.AddComponent<CameraComponent>(component);
				}

				if (spriteRendererComponent) {
					SpriteRendererComponent component;
					component.Color = spriteRendererComponent["Color"].as<glm::vec4>();

					deserializedEntity.AddComponent<SpriteRendererComponent>(component);
				}

				HZ_CORE_TRACE("Deserialized entity with ID {0}, name = {1}", uuid, sceneName);
			}
		}
		
		return true;
	}

	bool SceneSerializer::DeserializeRuntime(const std::string& filepath) {
		// Not implemented
		HZ_CORE_ASSERT(false);

		return false;
	}

}