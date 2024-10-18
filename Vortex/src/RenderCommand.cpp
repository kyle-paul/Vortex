#include "Graphics/RenderCommand.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Vortex
{
    // void RenderCommand::Init() {
        // switch(RendererAPI::GetCurrentAPI()) {
            // case(RendererAPI::API::OpenGL) : 
            // {
                // m_RendererAPI = new OpenGLRendererAPI();
            // }
        // }
    // }
    RendererAPI *RenderCommand::m_RendererAPI = new OpenGLRendererAPI();
}