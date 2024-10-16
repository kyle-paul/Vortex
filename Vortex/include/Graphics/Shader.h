#pragma once
#include "Vortex/PreHeaders.h"

namespace Vortex 
{
    class Shader
    {
    public:
        Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
        ~Shader();

        void Bind();
        void UnBind();
    private:
        unsigned int m_RendererID;
    };
}