#include "Vortex/PreHeaders.h"
#include "Panels/ContentBrowserPanel.h"
#include <imgui.h>

namespace Vortex
{
    static const std::filesystem::path s_AssetPath = "assets";

    ContentBrowserPanel::ContentBrowserPanel()
        : m_CurrentDirectory(s_AssetPath)
    {
        m_DirectoryIcon = Texture2D::Create("Resources/Icons/ContentBrowser/DirectoryIcon.png");
		m_FileIcon = Texture2D::Create("Resources/Icons/ContentBrowser/FileIcon.png");
    }

    void ContentBrowserPanel::OnImGuiRender()
    {
        ImGui::Begin("Content Browser");
        
        if (m_CurrentDirectory != std::filesystem::path(s_AssetPath))
        {
            if (ImGui::Button("<-"))
            {
                m_CurrentDirectory = m_CurrentDirectory.parent_path();
            }
        }

        static float padding = 16.0f;
		static float thumbnailSize = 128.0f;
		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

        ImGui::Columns(columnCount, 0, false);


		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			const auto& path = directoryEntry.path();
			// std::cout << "path: " << path << "\n";

			auto relativePath = std::filesystem::relative(path, s_AssetPath);
			auto fullPath = std::filesystem::relative(path);

			// std::cout << "fullPath: " << fullPath << "\n";
			// std::cout << "relativePath: " << relativePath << "\n\n";


			std::string filenameString = relativePath.filename().string();
			Ref<Texture2D> icon = directoryEntry.is_directory() ? m_DirectoryIcon : m_FileIcon;
			
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::ImageButton(filenameString.c_str(), reinterpret_cast<ImTextureID>(static_cast<uintptr_t>(icon->GetRendererID())), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });

			if (ImGui::BeginDragDropSource())
			{
				const char *itemPath = path.c_str();
				VX_INFO("{0}", itemPath);
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, strlen(itemPath) + 1);
				ImGui::EndDragDropSource();
			}

			ImGui::PopStyleColor();

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (directoryEntry.is_directory())
					m_CurrentDirectory /= path.filename();
			}
			ImGui::TextWrapped("%s", filenameString.c_str());

			ImGui::NextColumn();

		}

		ImGui::Columns(1);

		ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
		ImGui::SliderFloat("Padding", &padding, 0, 32);

		// TODO: status bar
		ImGui::End();
    }
}