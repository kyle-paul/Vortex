#include <yaml-cpp/yaml.h>
#include "Vortex/Scene/SceneSerializer.h"
#include "Vortex/Scene/Entity.h"
#include "Vortex/Scene/Component.h"

namespace YAML {
	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			return node;
		}
		static bool decode(const Node &node, glm::vec2 &rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}
		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}
		static bool decode(const Node& node, glm::vec4& rhs)
		{
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

namespace Vortex
{
	// ========== Emitter Controller ==========
	YAML::Emitter& operator<<(YAML::Emitter &out, const glm::vec2& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}
    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	// ========== Type String Controller ==========
	static std::string RigidBody2DBodyTypeToString(Rigidbody2DComponent::BodyType bodyType)
	{
		switch (bodyType)
		{
			case Rigidbody2DComponent::BodyType::Static:    return "Static";
			case Rigidbody2DComponent::BodyType::Dynamic:   return "Dynamic";
			case Rigidbody2DComponent::BodyType::Kinematic: return "Kinematic";
		}

		VX_CORE_ASSERT(false, "Unknown body type");
		return {};
	}

	static Rigidbody2DComponent::BodyType RigidBody2DBodyTypeFromString(const std::string& bodyTypeString)
	{
		if (bodyTypeString == "Static")    return Rigidbody2DComponent::BodyType::Static;
		if (bodyTypeString == "Dynamic")   return Rigidbody2DComponent::BodyType::Dynamic;
		if (bodyTypeString == "Kinematic") return Rigidbody2DComponent::BodyType::Kinematic;
	
		VX_CORE_ASSERT(false, "Unknown body type");
		return Rigidbody2DComponent::BodyType::Static;
	}

    SceneSerializer::SceneSerializer(const Ref<Scene> &context)
        : m_Context(context)
    {

    }

	// ========== Entity Serialization Controller ==========
    static void SerializeEntity(YAML::Emitter &out, Entity entity)
    {
        out << YAML::BeginMap;
        // out << YAML::Key << "Entity" << YAML::Value << (uint32_t)entity;
        out << YAML::Key << "Entity" << YAML::Value << entity.GetUUID();
        
        if (entity.HasComponent<TagComponent>())
        {
            out << YAML::Key << "TagComponent";
            out << YAML::BeginMap; // TagComponent
            auto& tag = entity.GetComponent<TagComponent>().Tag;
            out << YAML::Key << "Tag" << YAML::Value << tag;
            out << YAML::EndMap; // TagComponent
        }

		if (entity.HasComponent<ShapeComponent>())
        {
            out << YAML::Key << "ShapeComponent";
            out << YAML::BeginMap; // ShapeComponent
            const auto &type = (int)entity.GetComponent<ShapeComponent>().ShapeObj.GetType();
            out << YAML::Key << "Type" << YAML::Value << type;
            out << YAML::EndMap; // ShapeComponent
        }

		else if (entity.HasComponent<MeshComponent>())
		{
			out << YAML::Key << "MeshComponent";
			out << YAML::BeginMap; // MeshComponent
			const auto &path = entity.GetComponent<MeshComponent>().MeshObj.GetFilePath();
            out << YAML::Key << "Path" << YAML::Value << path;
			out << YAML::EndMap; // MeshComponent
		}

        if (entity.HasComponent<TransformComponent>())
        {
            out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap; // TransformComponent
            auto& tc = entity.GetComponent<TransformComponent>();
            out << YAML::Key << "Translation" << YAML::Value << tc.Translation;
			out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
			out << YAML::Key << "Scale" << YAML::Value << tc.Scale;
            out << YAML::EndMap; // TransformComponent
        }

        if (entity.HasComponent<SpriteRendererComponent>())
		{
			out << YAML::Key << "SpriteRendererComponent";
			out << YAML::BeginMap; // SpriteRendererComponent
			auto& spriteRendererComponent = entity.GetComponent<SpriteRendererComponent>();
			out << YAML::Key << "Color" << YAML::Value << spriteRendererComponent.Color;
			out << YAML::EndMap; // SpriteRendererComponent
		}

		if (entity.HasComponent<TextureComponent>())
		{
			out << YAML::Key << "TextureComponent";
			out << YAML::BeginMap; // TextureComponent
			auto& texture = entity.GetComponent<TextureComponent>();
			out << YAML::Key << "Texture" << YAML::Value;
			out << YAML::BeginMap; // TextureProps
			out << YAML::Key << "Path" << YAML::Value << texture.Texture->GetPath();
			out << YAML::EndMap; // TextureProps
			out << YAML::Key << "Tiling factor" << YAML::Value << texture.TilingFactor;
			out << YAML::EndMap; // TextureComponent
		}

        if (entity.HasComponent<CameraComponent>())
		{
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap; // CameraComponent
			auto& cameraComponent = entity.GetComponent<CameraComponent>();
			auto& camera = cameraComponent.Camera;
			out << YAML::Key << "Camera" << YAML::Value;
			out << YAML::BeginMap; // CameraProps
			out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
			out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.GetPerspectiveVerticalFOV();
			out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.GetPerspectiveNearClip();
			out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.GetPerspectiveFarClip();
			out << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize();
			out << YAML::Key << "OrthographicNear" << YAML::Value << camera.GetOrthographicNearClip();
			out << YAML::Key << "OrthographicFar" << YAML::Value << camera.GetOrthographicFarClip();
			out << YAML::EndMap; // CameraProps
			out << YAML::Key << "Primary" << YAML::Value << cameraComponent.Primary;
			out << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent.FixedAspectRatio;
			out << YAML::EndMap; // CameraComponent
		}

		if (entity.HasComponent<Rigidbody2DComponent>())
		{
			out << YAML::Key << "RigidbodyComponent";
			out << YAML::BeginMap; // Rigidbody2DComponent
			auto& rb2dComponent = entity.GetComponent<Rigidbody2DComponent>();
			out << YAML::Key << "BodyType" << YAML::Value << RigidBody2DBodyTypeToString(rb2dComponent.Type);
			out << YAML::Key << "FixedRotation" << YAML::Value << rb2dComponent.FixedRotation;
			out << YAML::EndMap; // Rigidbody2DComponent
		}

		if (entity.HasComponent<BoxCollider2DComponent>())
		{
			out << YAML::Key << "BoxCollider2DComponent";
			out << YAML::BeginMap; // BoxCollider2DComponent
			auto& bc2dComponent = entity.GetComponent<BoxCollider2DComponent>();
			out << YAML::Key << "Offset" << YAML::Value << bc2dComponent.Offset;
			out << YAML::Key << "Size" << YAML::Value << bc2dComponent.Size;
			out << YAML::Key << "Density" << YAML::Value << bc2dComponent.Density;
			out << YAML::Key << "Friction" << YAML::Value << bc2dComponent.Friction;
			out << YAML::Key << "Restitution" << YAML::Value << bc2dComponent.Restitution;
			out << YAML::Key << "RestitutionThreshold" << YAML::Value << bc2dComponent.RestitutionThreshold;
			out << YAML::EndMap; // BoxCollider2DComponent
		}

        out << YAML::EndMap;
    }

    void SceneSerializer::Serialize(const std::string &path)
    {
        YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		m_Context->m_registry.each([&](auto entityID)
		{
			Entity entity = { entityID, m_Context.get() };
			if (!entity)
				return;
			SerializeEntity(out, entity);
		});
		out << YAML::EndSeq;    
		out << YAML::EndMap;
		std::ofstream fout(path);
		fout << out.c_str();
    }

    void SceneSerializer::SerializeRuntime(const std::string &path)
    {
		// Not implemented
        VX_CORE_ASSERT(false);
    }

    bool SceneSerializer::Deserialize(const std::string &path)
    {
        std::ifstream stream(path);
		std::stringstream strStream;
		strStream << stream.rdbuf();

        YAML::Node data = YAML::Load(strStream.str());
		if (!data["Scene"])
			return false;

        std::string sceneName = data["Scene"].as<std::string>();
		VX_CORE_TRACE("Deserializing scene '{0}'", sceneName);
    
        auto entities = data["Entities"];

        if (entities)
        {
            for (auto entity : entities)
            {
                uint64_t uuid = entity["Entity"].as<uint64_t>();

                std::string name;
                auto tagComponent = entity["TagComponent"];
                if (tagComponent) name = tagComponent["Tag"].as<std::string>();

                VX_CORE_TRACE("Deserialized entity with ID = {0}, name = {1}", uuid, name);
                Entity deserializedEntity = m_Context->CreateEntityWithUUID(uuid, name);

				auto shapeComponent = entity["ShapeComponent"];
				auto meshComponent = entity["MeshComponent"];

				if (shapeComponent) 
				{
					Shape::BasicType type = (Shape::BasicType)shapeComponent["Type"].as<int>();
					Shape ShapeObj = Shape(type);
					deserializedEntity.AddComponent<ShapeComponent>(ShapeObj);
					deserializedEntity.GetComponent<ShapeComponent>().ShapeObj.SetType((type));
				}

				else if (meshComponent)
				{
					Vortex::Mesh MeshObj = Vortex::Mesh(meshComponent["Path"].as<std::string>());
					deserializedEntity.AddComponent<Vortex::MeshComponent>(MeshObj);
				}

				auto textureComponent = entity["TextureComponent"];
				if (textureComponent)
				{
					auto& tec = deserializedEntity.GetComponent<TextureComponent>();
					std::string path = textureComponent["Texture"]["Path"].as<std::string>();
					if (path != "white texture")
						tec.Texture = Texture2D::Create(path);	
					tec.TilingFactor = textureComponent["Tiling factor"].as<float>();	
				}

				auto transformComponent = entity["TransformComponent"];
				if (transformComponent)
				{
					auto& tc = deserializedEntity.GetComponent<TransformComponent>();
					tc.Translation = transformComponent["Translation"].as<glm::vec3>();
					tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
					tc.Scale = transformComponent["Scale"].as<glm::vec3>();
				}

                auto spriteRendererComponent = entity["SpriteRendererComponent"];
				if (spriteRendererComponent)
				{
					auto& src = deserializedEntity.AddComponent<SpriteRendererComponent>();
					src.Color = spriteRendererComponent["Color"].as<glm::vec4>();
				}

                auto cameraComponent = entity["CameraComponent"];
				if (cameraComponent)
				{
					auto& cc = deserializedEntity.AddComponent<CameraComponent>();
					const auto& cameraProps = cameraComponent["Camera"];

					cc.Camera.SetProjectionType((SceneCamera::ProjectionType)cameraProps["ProjectionType"].as<int>());

					cc.Camera.SetPerspectiveVerticalFOV(cameraProps["PerspectiveFOV"].as<float>());
					cc.Camera.SetPerspectiveNearClip(cameraProps["PerspectiveNear"].as<float>());
					cc.Camera.SetPerspectiveFarClip(cameraProps["PerspectiveFar"].as<float>());

					cc.Camera.SetOrthographicSize(cameraProps["OrthographicSize"].as<float>());
					cc.Camera.SetOrthographicNearClip(cameraProps["OrthographicNear"].as<float>());
					cc.Camera.SetOrthographicFarClip(cameraProps["OrthographicFar"].as<float>());

					cc.Primary = cameraComponent["Primary"].as<bool>();
					cc.FixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
				}

				auto rigidbodyComponent = entity["RigidbodyComponent"];
				if (rigidbodyComponent)
				{
					auto& rb2d = deserializedEntity.AddComponent<Rigidbody2DComponent>();
					rb2d.Type = RigidBody2DBodyTypeFromString(rigidbodyComponent["BodyType"].as<std::string>());
					rb2d.FixedRotation = rigidbodyComponent["FixedRotation"].as<bool>();
				}

				auto boxCollider2DComponent = entity["BoxCollider2DComponent"];
				if (boxCollider2DComponent)
				{
					auto& bc2d = deserializedEntity.AddComponent<BoxCollider2DComponent>();
					bc2d.Offset = boxCollider2DComponent["Offset"].as<glm::vec2>();
					bc2d.Size = boxCollider2DComponent["Size"].as<glm::vec2>();
					bc2d.Density = boxCollider2DComponent["Density"].as<float>();
					bc2d.Friction = boxCollider2DComponent["Friction"].as<float>();
					bc2d.Restitution = boxCollider2DComponent["Restitution"].as<float>();
					bc2d.RestitutionThreshold = boxCollider2DComponent["RestitutionThreshold"].as<float>();
				}
            }
        }
        return true;
    }

    bool SceneSerializer::DeserializeRuntime(const std::string &path)
    {
        VX_CORE_ASSERT(false);
		return false;
    }
}