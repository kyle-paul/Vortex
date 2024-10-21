#pragma once
#include "Vortex/PreHeaders.h"
#include <glm/glm.hpp>

namespace Vortex 
{
    class Shader
    {
    public:
        Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
        ~Shader();

        void Bind();
        void UnBind();

        void SetMat4(const std::string& name, const glm::mat4& value);


    private:
        uint32_t m_RendererID;
    };
}