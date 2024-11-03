#pragma once
#include "Vortex/Core/Core.h"
#include "Vortex/PreHeaders.h"
#include "Graphics/VertexArray.h"
#include "Graphics/Buffer.h"

namespace Vortex
{
    class Mesh
    {
    public:
        Mesh();
        ~Mesh();
        void Init(const std::string &filepath);
        void RegisterMesh(const std::string &filepath);
        inline const Ref<VertexArray> GetVertexArray() const { return VA; }
        
    private:
        Ref<VertexArray> VA;
        std::vector<float> pos;
        std::vector<float> norms;
        std::vector<float> texts;
        std::vector<uint32_t> faces;
    };
}