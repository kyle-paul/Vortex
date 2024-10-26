#include "Panels/SceneHierarchyPanel.h"
#include <glm/gtc/type_ptr.inl>

namespace Vortex
{
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

        ImGui::End();

        ImGui::Begin("Property");
        
        if (m_SelectionContext)
        {
            DisplayProperties(m_SelectionContext);
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
    }

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

            if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
			{
				auto &transform = entity.GetComponent<TransformComponent>().Transform;
				if(ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.1f))
                {
                }
				ImGui::TreePop();
			}
        }

        if (entity.HasComponent<CameraComponent>())
        {
            if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera"))
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