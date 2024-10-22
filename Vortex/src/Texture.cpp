#include "Vortex/PreHeaders.h"
#include "Graphics/Texture.h"
#include "Graphics/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture2D.h"

namespace Vortex
{
    Ref<Texture2D> Texture2D::Create(const std::string &path) 
    {
        switch(Renderer::GetAPI())
        {
            case RendererAPI::API::None: {
                VX_CORE_ASSERT(false, "[Vertex Array] None API backend is not supported.");
                return nullptr;
            }
            case RendererAPI::API::OpenGL: {
                return std::make_shared<OpenGLTexture2D>(path);
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

    Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height) 
    {
        switch(Renderer::GetAPI())
        {
            case RendererAPI::API::None: {
                VX_CORE_ASSERT(false, "[Vertex Array] None API backend is not supported.");
                return nullptr;
            }
            case RendererAPI::API::OpenGL: {
                return std::make_shared<OpenGLTexture2D>(width, height);
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