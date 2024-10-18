#include "Vortex/PreHeaders.h"
#include "Graphics/RendererAPI.h"

namespace Vortex
{
    // Select the default API as OpenGL 
    RendererAPI::API RendererAPI::m_API = RendererAPI::API::OpenGL;
}