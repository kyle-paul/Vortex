#pragma once

#include "Vortex/Core/Core.h"
#include "Scene.h"

namespace Vortex
{
    class SceneSerializer
    {
    public:
        SceneSerializer(const Ref<Scene> &context);

        void Serialize(const std::string &path);
        void SerializeRuntime(const std::string &path);

        bool Deserialize(const std::string &path);
        bool DeserializeRuntime(const std::string &path);

    private:
        Ref<Scene> m_Context;
    };
}