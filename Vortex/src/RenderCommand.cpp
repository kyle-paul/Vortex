#include "Graphics/RenderCommand.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Vortex
{
    RendererAPI *RenderCommand::m_RendererAPI = new OpenGLRendererAPI();
}