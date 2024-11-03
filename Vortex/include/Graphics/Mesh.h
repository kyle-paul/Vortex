#pragma once
#include "Vortex/Core/Core.h"
#include "Vortex/PreHeaders.h"
#include "Graphics/VertexArray.h"
#include "Graphics/Utils.h"

namespace Vortex
{
    class Mesh
    {
    public:
        Mesh(const std::string &filepath);
        ~Mesh();

        inline const Ref<VertexArray> GetVertexArray() const { return VA; }
        inline const float GetMaxSize() const { return m_MaxSize; }
        inline const glm::vec3 GetCenter() const { return m_Center; }
        inline const std::string GetFilePath() const { return m_FilePath; }

    private:
        void RegisterMesh(const std::string &filepath);
        
    private:
        Ref<VertexArray> VA;
        std::vector<float> verts;
        std::vector<uint32_t> faces;

        glm::vec3 m_Center;
        float m_MaxSize;

        std::string m_FilePath;
    };
}