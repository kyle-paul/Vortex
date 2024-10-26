#pragma once
#include "Vortex.h"

namespace Vortex
{
    class SceneHierarchyPanel
    {
    public:
        SceneHierarchyPanel() = default;
        SceneHierarchyPanel(const Ref<Scene> &context);
        
        void SetContext(const Ref<Scene> &context);
        void OnImGuiRender();

    private:
        void DrawEntityNode(Entity entity);

    private:
        Ref<Scene> m_Context;
        Entity m_SelectionContext;
        friend class Scene;
    };
}