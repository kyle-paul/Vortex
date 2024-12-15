#include "ChessModel.h"

namespace Vortex
{
    void ChessModel::SubmitPlayer(Ref<Scene> ActiveScene, std::string filepath, std::string &name, glm::vec3 &position, bool opponent)
    {
        Mesh MeshObj = Mesh(filepath);
		auto player = ActiveScene->CreateEntity(name);
		player.AddComponent<MeshComponent>(MeshObj);
		player.GetComponent<MeshComponent>().Type = "Quad";
		
		if (opponent)
			player.AddComponent<SpriteRendererComponent>(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
		else
			player.AddComponent<SpriteRendererComponent>(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		
		auto &trans = player.GetComponent<TransformComponent>();
		trans.Translation = position;
		trans.Scale = glm::vec3(0.02f);
    }
}