// #include "Graphics/Shader.h"
#include "Graphics/VertexArray.h"
#include "Graphics/Renderer.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Vortex
{
    Shader* Shader::Create(const std::string& name, const std::string &vertexSource, const std::string &FragmentSource) 
    {
        switch(Renderer::GetAPI())
        {
            case RendererAPI::API::None: {
                VX_CORE_ASSERT(false, "[Vertex Array] None API backend is not supported.");
                return nullptr;
            }
            case RendererAPI::API::OpenGL: {
                return new OpenGLShader(name, vertexSource, FragmentSource);
            }
            case RendererAPI::API::VulCan: {
                VX_CORE_ASSERT(false, "[Vertex Array] Vulcan API backend currently is not supported.");
                return nullptr;
            }
            case RendererAPI::API::DirectX: {
                VX_CORE_ASSERT(false, "[Vertex Array] DirectX API backend currently is not supported.");
                return nullptr;
            }
        }
        VX_CORE_ASSERT(false, "[Vertex Array] Please select an API backend for rendering. Options are [OpenGL, Vulcan, DirectX]");
        return  nullptr;
    }
}