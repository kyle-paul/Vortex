#pragma once
#include "Graphics/RendererAPI.h"
#include "Graphics/RenderCommand.h"
#include "Graphics/OrthographicCamera.h"
#include "Graphics/OrthographicCamera.h"
#include "Graphics/Shader.h"

namespace Vortex
{
    class Renderer
    {
    public:
        static void BeginScene(OrthographicCamera &camera);
        static void Submit(const std::shared_ptr<Shader> &shader, 
                            const std::shared_ptr<VertexArray>& VertexArrayID,
                            const glm::mat4 &transform = glm::mat4(0.0f));
        static void EndScene();

        inline static RendererAPI::API GetAPI() { return RendererAPI::GetCurrentAPI(); }

    private:
        
        struct SceneData {
            glm::mat4 ViewProjectionMatrix;
        };

        static SceneData* m_SceneData;
    };
}