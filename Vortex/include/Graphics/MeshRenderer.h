#pragma once
#include "Graphics/Texture.h"
#include "Graphics/EditorCamera.h"
#include "Graphics/Shader.h"
#include "Graphics/RenderCommand.h"
#include "Graphics/Mesh.h"

namespace Vortex
{
    class MeshRenderer
    {
    public:
        MeshRenderer();
        ~MeshRenderer();

        static void Init();

        static void BeginScene(const Camera& camera, const glm::mat4 &transform); // for entity has camera component
        static void BeginScene(EditorCamera& camera); // for the editor main camera
        static void EndScene();

        static void DrawMesh(const Mesh &mesh, const glm::mat4& transform, const glm::vec4& color, const int EntityID, const Ref<Texture2D>& texture);

    private:
        
    };
}