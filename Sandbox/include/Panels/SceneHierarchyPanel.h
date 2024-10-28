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

        inline const Entity GetSelectedEntity() { return m_SelectionContext; } 
        inline void SetSelectionContext(Entity entity) { m_SelectionContext=entity; }

    private:
        void DrawEntityNode(Entity entity);
        void DisplayProperties(Entity entity);

    private:
        Ref<Scene> m_Context;
        Entity m_SelectionContext;
        friend class Scene;
    };
}