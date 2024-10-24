#include "Graphics/Framebuffer.h"
#include "Graphics/Renderer.h"
#include "Graphics/OpenGLFramebuffer.h"

namespace Vortex
{
    Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec) {

        // Decide which API backend for rendering
        switch(Renderer::GetAPI()) 
        {
            case RendererAPI::API::None: {
                VX_CORE_ASSERT(false, "[Vertex Buffer] None API backend is not supported.");
                return nullptr;
            }
            case RendererAPI::API::OpenGL: {
                return CreateRef<OpenGLFramebuffer>(spec);
            }
            case RendererAPI::API::VulCan: {
                VX_CORE_ASSERT(false, "[Vertex Buffer] Vulcan API backend currently is not supported.");
                return nullptr;
            }
            case RendererAPI::API::DirectX: {
                VX_CORE_ASSERT(false, "[Vertex Buffer] DirectX API backend currently is not supported.");
                return nullptr;
            }
        }
        VX_CORE_ASSERT(false, "[Vertex Buffer] Please select an API backend for rendering. Options are [OpenGL, Vulcan, DirectX]");
        return  nullptr;
    }
   
}