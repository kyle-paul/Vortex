#include "Graphics/UniformBuffer.h"
#include "Graphics/Renderer.h"
#include "Graphics/OpenGLUniformBuffer.h"

namespace Vortex
{
    Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding)
    {
        switch(Renderer::GetAPI())
        {
            case RendererAPI::API::None:
            {
                VX_CORE_ASSERT(false, "[Uniform Buffer] None API backend is not supported.");
                return nullptr;
            }

            case RendererAPI::API::OpenGL:
            {
                return CreateRef<OpenGLUniformBuffer>(size, binding);
            }

            case RendererAPI::API::VulCan: 
            {
                VX_CORE_ASSERT(false, "[Uniform Buffer] Vulcan API backend currently is not supported.");
                return nullptr;
            }
            
            case RendererAPI::API::DirectX: 
            {
                VX_CORE_ASSERT(false, "[Uniform Buffer] DirectX API backend currently is not supported.");
                return nullptr;
            }
        }

        VX_CORE_ASSERT(false, "Unknown RenderAPI");
        return nullptr;
    }
}