#include "Graphics/VertexArray.h"
#include "Graphics/Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Vortex
{
    VertexArray* VertexArray::Create() 
    {
        switch(Renderer::GetAPI())
        {
            case RendererAPI::API::None: {
                VX_CORE_ASSERT(false, "[Vertex Array] None API backend is not supported.");
                return nullptr;
            }
            case RendererAPI::API::OpenGL: {
                return new OpenGLVertexArray();
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