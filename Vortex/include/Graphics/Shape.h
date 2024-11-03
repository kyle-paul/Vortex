#pragma once
#include "Vortex/Core/Core.h"
#include "Vortex/PreHeaders.h"
#include "Graphics/VertexArray.h"
#include "Graphics/Utils.h"

namespace Vortex
{
    class Shape
    {
    public:
        enum class BasicType
        {
            Square=0, Triangle=1, Circle=2, 
            Cube=3, Sphere=4, Cylinder=5,
            Prism=6, Tetrahedron=7, Pyramid=8,
            Cone=9, Octahedron=10,
        };
        
        Shape(BasicType type);
        ~Shape();

        inline const Ref<VertexArray> GetVertexArray() const { return VA; }
        inline const BasicType GetType() const { return m_Type; }

        inline void SetType(BasicType type) { m_Type = type; }

    private:
        void RegisterShape(const std::string &path);

    private:
        Ref<VertexArray> VA;
        std::vector<float> verts;
        std::vector<uint32_t> indices;

        BasicType m_Type;
    };
}