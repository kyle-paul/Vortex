#include "Panels/SceneHierarchyPanel.h"
#include <glm/gtc/type_ptr.inl>
#include <imgui_internal.h>

namespace Vortex
{

    static void DrawVec3Control(const std::string &label, glm::vec3 &values, float resetValue = 0.0f, float speed = 0.1f, float columnWidth = 100.0f)
    {
        ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text("%s", label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		if (ImGui::Button("X", buttonSize)) values.x = resetValue;
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, speed, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		if (ImGui::Button("Y", buttonSize)) values.y = resetValue;
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, speed, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		if (ImGui::Button("Z", buttonSize)) values.z = resetValue;
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, speed, 0.0f, 0.0f, "%.2f");

		ImGui::PopItemWidth();
		ImGui::PopStyleVar();
		ImGui::Columns(1);
		ImGui::PopID();
    }

    SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene> &context)
    {
        SetContext(context);
    }

    void SceneHierarchyPanel::SetContext(const Ref<Scene> &context)
    {
        m_Context = context;
    }

    void SceneHierarchyPanel::OnImGuiRender()
    {
        ImGui::Begin("Scene Hierachy");
        m_Context->m_registry.each([&](auto entityID)
        {
            Entity entity {entityID, m_Context.get()};
            DrawEntityNode(entity);
        });

        if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()) {
            m_SelectionContext = {};
        }

        // Right-click on blank space
        if (!m_SelectionContext)
        {
            if (ImGui::BeginPopupContextWindow(0, 1))
            {
                if (ImGui::MenuItem("Create Empty Entity")) m_Context->CreateEntity("Empty Entity");
                ImGui::EndPopup();
            }
        }
        ImGui::End();

        ImGui::Begin("Property");
        if (m_SelectionContext)
        {
            DisplayProperties(m_SelectionContext);
            if (ImGui::Button("Add Component"))
            {
                ImGui::OpenPopup("AddComponent");
            }

            if (ImGui::BeginPopup("AddComponent"))
            {
                if (ImGui::MenuItem("Transform"))
                {
                    if (!m_SelectionContext.HasComponent<TransformComponent>())
                    {
                        m_SelectionContext.AddComponent<TransformComponent>();
                    }
                    ImGui::CloseCurrentPopup();
                }
                else if (ImGui::MenuItem("Sprite Renderer"))
                {
                    if (!m_SelectionContext.HasComponent<SpriteRendererComponent>())
                    {
                        m_SelectionContext.AddComponent<SpriteRendererComponent>();
                    }
                    ImGui::CloseCurrentPopup();
                }
                else if (ImGui::MenuItem("Camera"))
                {
                    if (!m_SelectionContext.HasComponent<CameraComponent>())
                    {
                        m_SelectionContext.AddComponent<CameraComponent>();
                    }
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
        }
        ImGui::End();
    }
    
    void SceneHierarchyPanel::DrawEntityNode(Entity entity)
    {
        auto &tc = entity.GetComponent<TagComponent>();
        ImGui::Text("%s", tc.Tag.c_str());

        ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
        bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, "%s", tc.Tag.c_str());

        if (ImGui::IsItemClicked())
        {
            m_SelectionContext = entity;
        }

        bool entityDeleted = false;
        if (ImGui::BeginPopupContextItem())  // Unique ID using m_EntityHandle
        {
            if (ImGui::MenuItem("Delete Entity")) entityDeleted = true;
            ImGui::EndPopup();
        }

        if (opened)
        {
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
            bool opened = ImGui::TreeNodeEx((void*)25255, flags, "%s", tc.Tag.c_str());
            if (opened)
            {
                ImGui::TreePop();
            }   
            ImGui::TreePop();
        }

        if (entityDeleted)
		{
			m_Context->DestroyEntity(entity);
			if (m_SelectionContext == entity)
            {
				m_SelectionContext = {};
            }
		}
    }

    const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;

    void SceneHierarchyPanel::DisplayProperties(Entity entity)
    {
        if (entity.HasComponent<TagComponent>())
        {
            auto &tc = entity.GetComponent<TagComponent>();
            
            char buffer[256];
            memset(buffer, 0, sizeof(buffer));
            strcpy(buffer, tc.Tag.c_str());

            if (ImGui::InputText("Tag", buffer, sizeof(buffer))) 
            {
                tc.Tag = std::string(buffer);
            }
        }
        
        if (entity.HasComponent<TransformComponent>())
        {
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2 {4, 4});
            bool opened = ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), treeNodeFlags, "Transform");
            ImGui::SameLine(ImGui::GetWindowWidth() - 25.0f);
            
            if (ImGui::Button("+", ImVec2{20, 20}))
            {
                ImGui::OpenPopup("ComponentSettings");
            }
            ImGui::PopStyleVar();

            bool HasRemovedComponent = false; 
            if (ImGui::BeginPopup("ComponentSettings"))
            {
                if (ImGui::MenuItem("Remove component"))
                {
                    HasRemovedComponent = true;
                }
                ImGui::EndPopup();
            }

            if (opened)
            {
                auto &tc = entity.GetComponent<TransformComponent>();
                DrawVec3Control("Position", tc.Translation);
                DrawVec3Control("Rotation", tc.Rotation);
                DrawVec3Control("Scale", tc.Scale, 1.0f);
				ImGui::TreePop();
            }

            if (HasRemovedComponent)
            {
                entity.RemoveComponent<TransformComponent>();
            }
        }

        if (entity.HasComponent<SpriteRendererComponent>())
        {
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2 {4, 4});
            bool opened = ImGui::TreeNodeEx((void*)typeid(SpriteRendererComponent).hash_code(), treeNodeFlags, "Sprite Renderer");
            ImGui::SameLine();
            ImGui::SameLine(ImGui::GetWindowWidth() - 25.0f);
            if (ImGui::Button("+"))
            {
                ImGui::OpenPopup("ComponentSettings");
            }
            ImGui::PopStyleVar();


            bool HasRemovedComponent = false; 
            if (ImGui::BeginPopup("ComponentSettings"))
            {
                if (ImGui::MenuItem("Remove component"))
                {
                    HasRemovedComponent = true;
                }
                ImGui::EndPopup();
            }

            if (opened)
			{
                auto &src = entity.GetComponent<SpriteRendererComponent>();
                ImGui::ColorEdit4("Color Edit", glm::value_ptr(src.Color));
                ImGui::TreePop();
			}

            if (HasRemovedComponent)
            {
                entity.RemoveComponent<SpriteRendererComponent>();
            }
        }

        if (entity.HasComponent<CameraComponent>())
        {
            if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), treeNodeFlags, "Camera"))
            {
                auto &cam_comp = entity.GetComponent<CameraComponent>();
                auto &cam = entity.GetComponent<CameraComponent>().Camera;

                ImGui::Checkbox("Primary", &cam_comp.Primary); // Whether to set this camera to primary camera


                const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
				const char* currentProjectionTypeString = projectionTypeStrings[(int)cam.GetProjectionType()];

				if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
				{
					for (int i = 0; i < 2; i++)
					{
						bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
						if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
						{
							currentProjectionTypeString = projectionTypeStrings[i];
							cam.SetProjectionType((SceneCamera::ProjectionType)i);
						}
						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}


                if (cam.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
                {
                    float orthorSize = cam.GetOrthographicSize();
                    float orthorNear = cam.GetOrthographicNearClip();
                    float orthorFar = cam.GetOrthographicFarClip();

                    if (ImGui::DragFloat("Size", &orthorSize))
                        cam.SetOrthographicSize(orthorSize);
    
                    else if (ImGui::DragFloat("Near clip", &orthorNear))
                        cam.SetOrthographicNearClip(orthorNear);

                    else if (ImGui::DragFloat("Far clip", &orthorFar))
                        cam.SetOrthographicFarClip(orthorFar);
                    
                    ImGui::Checkbox("Fixed Aspect Ratio", &cam_comp.FixedAspectRatio);
                }

                else if (cam.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
                {
                    float perspecFOV = cam.GetPerspectiveVerticalFOV();
                    float perspecNear = cam.GetPerspectiveNearClip();
                    float perspecFar = cam.GetPerspectiveFarClip();

                    if (ImGui::DragFloat("FOV", &perspecFOV))
                        cam.SetPerspectiveVerticalFOV(perspecFOV);
    
                    else if (ImGui::DragFloat("Near clip", &perspecNear))
                        cam.SetPerspectiveNearClip(perspecNear);

                    else if (ImGui::DragFloat("Far clip", &perspecFar))
                        cam.SetPerspectiveFarClip(perspecFar);
                }

                ImGui::TreePop();
            }
        }
    }
}