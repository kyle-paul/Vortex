#pragma once
#include "Graphics/Texture.h"
#include "Panels/SceneHierarchyPanel.h"
#include <filesystem>

namespace Vortex
{
    class ContentBrowserPanel
    {
    public:
        ContentBrowserPanel();

        void SetSceneHierarcyPanel(Ref<SceneHierarchyPanel> HierarchyPanel);
        void OnImGuiRender();

    private:
        std::filesystem::path m_CurrentDirectory;
        Ref<Texture2D> m_DirectoryIcon;
		Ref<Texture2D> m_FileIcon;

        Ref<SceneHierarchyPanel> m_HierarchyPanel;
        friend class SceneHierarchyPanel;
    };
}