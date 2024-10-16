#pragma once

namespace Vortex
{
    enum class RendererAPI 
    {
        None = 0,
        OpenGL = 1,
        VulCan = 2,
        DirectX = 3,
    };
    
    class Renderer
    {
    public:
        inline static RendererAPI GetAPI() { return m_RendererAPI;}
    private:
        static RendererAPI m_RendererAPI;
    };
}