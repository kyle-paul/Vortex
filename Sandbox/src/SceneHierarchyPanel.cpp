#include "Panels/SceneHierarchyPanel.h"
#include <glm/gtc/type_ptr.inl>
#include <imgui_internal.h>

namespace Vortex
{
    static void DrawVec3Control(const std::string &label, glm::vec3 &values, float resetValue = 0.0f, float speed = 0.1f, float columnWidth = 100.0f)
    {
        ImGuiIO &io = ImGui::GetIO();
        auto boldFont = io.Fonts->Fonts[1];

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
        ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize)) values.x = resetValue;
        ImGui::PopFont();
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, speed, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
        ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize)) values.y = resetValue;
        ImGui::PopFont();
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, speed, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
        ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize)) values.z = resetValue;
        ImGui::PopFont();
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
        if (m_SelectionContext) {
            m_SelectionContext = {};
        }
    }

    void SceneHierarchyPanel::UpdateTexture(const std::string &TexturePath)
    {
        if (m_SelectionContext) {
            Vortex::Ref<Vortex::Texture2D> new_texture = Vortex::Texture2D::Create(TexturePath);
            auto &text_comp = m_SelectionContext.GetComponent<Vortex::TextureComponent>();
            text_comp.Texture = new_texture;
        }
    }

    void SceneHierarchyPanel::OnImGuiRender()
    {
        ImGui::Begin("Scene Hierachy");
        if (m_Context)
        {
            m_Context->m_registry.each([&](auto entityID)
            {
                Entity entity {entityID, m_Context.get()};
                DrawEntityNode(entity);
            });

            // Click on the blank space
            if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()) 
            {
                m_SelectionContext = {};
            }
        }

        // Right-click on blank space
        if (!m_SelectionContext)
        {
            if (ImGui::BeginPopupContextWindow(0, 1))
            {
                if (ImGui::MenuItem("Create Empty Entity"))
                {
                    auto entity = m_Context->CreateEntity("Empty Entity");
                }

                if (ImGui::MenuItem("Create Camera"))
                {
                    auto Cam = m_Context->CreateEntity("Camera");
                    Cam.AddComponent<CameraComponent>();
                }

                if (ImGui::MenuItem("Create Batch Quad"))
                {
                    auto Quad = m_Context->CreateEntity("Quad");
                    Quad.AddComponent<BatchComponent>();
                    Quad.AddComponent<SpriteRendererComponent>();
                }
                
                if (ImGui::BeginMenu("Create 2D Shapes"))
                {
                    if (ImGui::MenuItem("Create Square"))
                    {
                        Shape ShapeObj = Shape(Shape::BasicType::Square);
                        auto Square = m_Context->CreateEntity("Square");
                        Square.AddComponent<ShapeComponent>(ShapeObj);
                        Square.AddComponent<SpriteRendererComponent>();
                    }

                    else if (ImGui::MenuItem("Create Triangle")) 
                    {
                        Shape ShapeObj = Shape(Shape::BasicType::Triangle);
                        auto Square = m_Context->CreateEntity("Triangle");
                        Square.AddComponent<ShapeComponent>(ShapeObj);
                    }

                    else if (ImGui::MenuItem("Create Circle")) 
                    {
                        Shape ShapeObj = Shape(Shape::BasicType::Circle);
                        auto Square = m_Context->CreateEntity("Circle");
                        Square.AddComponent<ShapeComponent>(ShapeObj);
                    }

                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("Create 3D Mesh"))
                {
                    if (ImGui::MenuItem("Create Cube")) 
                    {
                        Shape ShapeObj = Shape(Shape::BasicType::Cube);
                        auto Cube = m_Context->CreateEntity("Cube");
                        Cube.AddComponent<ShapeComponent>(ShapeObj);
                        Cube.AddComponent<SpriteRendererComponent>();
                    }


                    else if (ImGui::MenuItem("Create Sphere")) 
                    {
                        Shape ShapeObj = Shape(Shape::BasicType::Sphere);
                        auto Square = m_Context->CreateEntity("Sphere");
                        Square.AddComponent<ShapeComponent>(ShapeObj);
                    }


                    else if (ImGui::MenuItem("Create Cylinder")) 
                    {
                        Shape ShapeObj = Shape(Shape::BasicType::Cylinder);
                        auto Square = m_Context->CreateEntity("Cylinder");
                        Square.AddComponent<ShapeComponent>(ShapeObj);
                    }

                    else if (ImGui::MenuItem("Create Prism")) 
                    {
                        Shape ShapeObj = Shape(Shape::BasicType::Prism);
                        auto Square = m_Context->CreateEntity("Prism");
                        Square.AddComponent<ShapeComponent>(ShapeObj);
                    }


                    else if (ImGui::MenuItem("Create Tetrahedron")) 
                    {
                        Shape ShapeObj = Shape(Shape::BasicType::Tetrahedron);
                        auto Square = m_Context->CreateEntity("Tetrahedron");
                        Square.AddComponent<ShapeComponent>(ShapeObj);
                    }

                    else if (ImGui::MenuItem("Create Pyramid")) 
                    {
                        Shape ShapeObj = Shape(Shape::BasicType::Pyramid);
                        auto Square = m_Context->CreateEntity("Pyramid");
                        Square.AddComponent<ShapeComponent>(ShapeObj);
                    }

                    else if (ImGui::MenuItem("Create Cone")) 
                    {
                        Shape ShapeObj = Shape(Shape::BasicType::Cone);
                        auto Square = m_Context->CreateEntity("Cone");
                        Square.AddComponent<ShapeComponent>(ShapeObj);
                    }

                    else if (ImGui::MenuItem("Create Octahedron")) 
                    {
                        Shape ShapeObj = Shape(Shape::BasicType::Octahedron);
                        auto Square = m_Context->CreateEntity("Octahedron");
                        Square.AddComponent<ShapeComponent>(ShapeObj);
                    }
                    
                    ImGui::EndMenu();
                }
                
                if (ImGui::MenuItem("Create Text"))
                {
                    
                }

                ImGui::EndPopup();
            }
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
        flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
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
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
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

    const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

    template <typename T, typename UiFunction>
    static void DrawComponent(const std::string &name, Entity entity, UiFunction uifunction)
    {
        if (entity.HasComponent<T>())
        {
            auto &component = entity.GetComponent<T>();
            ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
            ImGui::Separator();
            bool opened = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, "%s", name.c_str());
            ImGui::PopStyleVar();

            ImGui::SameLine();
            ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
			if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight })) ImGui::OpenPopup("ComponentSettings");

            bool HasRemovedComponent = false; 
            if (ImGui::BeginPopup("ComponentSettings"))
            {
                if (ImGui::MenuItem("Remove component")) HasRemovedComponent = true;
                ImGui::EndPopup();
            }

            if (opened)
			{
                uifunction(component);
                ImGui::TreePop();
			}

            if (HasRemovedComponent) entity.RemoveComponent<T>();
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

            if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
            {
                tc.Tag = std::string(buffer);
            }
        }
        
        {
            ImGui::SameLine();
            ImGui::PushItemWidth(-1);
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

                if (ImGui::MenuItem("Batching"))
                {
                    if (!m_SelectionContext.HasComponent<BatchComponent>())
                    {
                        m_SelectionContext.AddComponent<BatchComponent>();
                    }
                    ImGui::CloseCurrentPopup();
                }

                if (ImGui::MenuItem("Shape"))
                {
                    if (!m_SelectionContext.HasComponent<ShapeComponent>())
                    {
                        Shape ShapeObj = Shape(Shape::BasicType::Cube);
                        m_SelectionContext.AddComponent<ShapeComponent>(ShapeObj);
                    }
                    ImGui::CloseCurrentPopup();
                }

                if (ImGui::MenuItem("Mesh"))
                {
                    if (!m_SelectionContext.HasComponent<MeshComponent>())
                    {
                        // m_SelectionContext.AddComponent<MeshComponent>();
                    }
                    ImGui::CloseCurrentPopup();
                }

                else if (ImGui::MenuItem("Sprite Renderer"))
                {
                    if (!m_SelectionContext.HasComponent<SpriteRendererComponent>())
                    {
                        Ref<Texture2D> texture = Texture2D::Create(1, 1);
                        uint32_t whiteTextureData = 0xffffffff;
                        texture->SetData(&whiteTextureData, sizeof(uint32_t));
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

                // Physics
                if (ImGui::MenuItem("Rigid Body"))
                {
                    if (!m_SelectionContext.HasComponent<Rigidbody2DComponent>())
					{
						m_SelectionContext.AddComponent<Rigidbody2DComponent>();
						ImGui::CloseCurrentPopup();
					}
				}

                if (ImGui::MenuItem("Box Collider"))
                {
				    if (!m_SelectionContext.HasComponent<BoxCollider2DComponent>())					
					{
						m_SelectionContext.AddComponent<BoxCollider2DComponent>();
						ImGui::CloseCurrentPopup();
					}
				}

                ImGui::EndPopup();
            }
            ImGui::PopItemWidth();
        }
        
        DrawComponent<TransformComponent>("Transform", entity, [](auto& component)
		{
			DrawVec3Control("Translation", component.Translation);
			DrawVec3Control("Rotation", component.Rotation);
			DrawVec3Control("Scale", component.Scale, 1.0f);
		});

        DrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [](auto &component)
        {
            ImGui::ColorEdit4("Color Edit", glm::value_ptr(component.Color));
        });

        DrawComponent<TextureComponent>("Texture Renderer", entity, [](auto &component)
        {
            ImGui::Button("Change Texture");
            ImGui::DragFloat("Tiling factor", &component.TilingFactor, 1.0f, 0.0f, 10.0f);
        });


        DrawComponent<Rigidbody2DComponent>("Rigid Body", entity, [](auto &component)
        {
            const char* bodyTypeStrings[] = { "Static", "Dynamic", "Kinematic"};
            const char* currentBodyTypeString = bodyTypeStrings[(int)component.Type];

            if (ImGui::BeginCombo("Body Type", currentBodyTypeString))
			{
				for (int i = 0; i < 3; i++)
				{
					bool isSelected = currentBodyTypeString == bodyTypeStrings[i];
					if (ImGui::Selectable(bodyTypeStrings[i], isSelected))
					{
						currentBodyTypeString = bodyTypeStrings[i];
						component.Type = (Rigidbody2DComponent::BodyType)i;
					}
					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			ImGui::Checkbox("Fixed Rotation", &component.FixedRotation);
        });

        DrawComponent<BoxCollider2DComponent>("Box Collider", entity, [](auto& component)
		{
			ImGui::DragFloat2("Offset", glm::value_ptr(component.Offset));
			ImGui::DragFloat2("Size", glm::value_ptr(component.Size));
			ImGui::DragFloat("Density", &component.Density, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Friction", &component.Friction, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Restitution", &component.Restitution, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Threshold", &component.RestitutionThreshold, 0.01f, 0.0f);
		});


        DrawComponent<CameraComponent>("Camera", entity, [](auto &component)
        {
            auto& camera = component.Camera;
			ImGui::Checkbox("Primary", &component.Primary);

			const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
			const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];

			if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
			{
				for (int i = 0; i < 2; i++)
				{
					bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
					if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
					{
						currentProjectionTypeString = projectionTypeStrings[i];
						camera.SetProjectionType((SceneCamera::ProjectionType)i);
					}

					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}

			if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
			{
				float perspectiveVerticalFov = camera.GetPerspectiveVerticalFOV();
				if (ImGui::DragFloat("Vertical FOV", &perspectiveVerticalFov))
					camera.SetPerspectiveVerticalFOV(glm::radians(perspectiveVerticalFov));

				float perspectiveNear = camera.GetPerspectiveNearClip();
				if (ImGui::DragFloat("Near", &perspectiveNear))
					camera.SetPerspectiveNearClip(perspectiveNear);

				float perspectiveFar = camera.GetPerspectiveFarClip();
				if (ImGui::DragFloat("Far", &perspectiveFar))
					camera.SetPerspectiveFarClip(perspectiveFar);
			}

			if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
			{
				float orthoSize = camera.GetOrthographicSize();
				if (ImGui::DragFloat("Size", &orthoSize))
					camera.SetOrthographicSize(orthoSize);

				float orthoNear = camera.GetOrthographicNearClip();
				if (ImGui::DragFloat("Near", &orthoNear))
					camera.SetOrthographicNearClip(orthoNear);

				float orthoFar = camera.GetOrthographicFarClip();
				if (ImGui::DragFloat("Far", &orthoFar))
					camera.SetOrthographicFarClip(orthoFar);

				ImGui::Checkbox("Fixed Aspect Ratio", &component.FixedAspectRatio);
			}
        });
    }
}