#include "Vortex/Core/Core.h"
#include "Vortex/Imgui/ImGuiLayer.h"
#include <glm/gtc/type_ptr.hpp>
#include "EditorLayer.h"

EditorLayer::EditorLayer()
    : Layer("EditorLayer"), m_CameraController(1300.0f / 800.0f, true)
{
    
}

glm::mat4 GetTransformQuad(const glm::vec3 &position, const glm::vec2 &size, const float &rotation)
{
	return glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1)) *
			glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
}

void EditorLayer::OnAttach()
{
	VX_PROFILE_FUNCTION();

	// Frame buffer
	Vortex::FramebufferSpecification fbspec;
	fbspec.Width = 1300.0f;
	fbspec.Height = 800.0f;
	m_Framebuffer = Vortex::Framebuffer::Create(fbspec);

	// Texture registry
    m_CheckerboardTexture = Vortex::Texture2D::Create("/home/pc/dev/engine/Sandbox/assets/Textures/Checkerboard.png");

	// Entity components 
	m_ActiveScene = Vortex::CreateRef<Vortex::Scene>();

	SquareEntity = m_ActiveScene->CreateEntity("Square_Entity");
	SquareEntity.AddComponent<Vortex::SpriteRendererComponent>(m_ImGuiComponents.ObjectColor);
}

void EditorLayer::OnDetach()
{
	VX_PROFILE_FUNCTION();
}

void EditorLayer::OnUpdate(Vortex::TimeStep ts)
{
    // Key event controller for camera
	if (is_ViewPortFocused)
		m_CameraController.OnUpdate(ts);

	m_Framebuffer->Bind();
	Vortex::RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1 });
	Vortex::RenderCommand::ClearBufferBit();

	// Update for scene
	Vortex::Renderer2D::BeginScene(m_CameraController.GetCamera(), m_ImGuiComponents);
	m_ActiveScene->OnUpdate(ts);
	Vortex::Renderer2D::EndScene();
	
	m_Framebuffer->Unbind();
}

void EditorLayer::OnEvent(Vortex::Event &event) 
{
    m_CameraController.OnEvent(event);
}

static void ShowDockingDisabledMessage()
{
    ImGuiIO& io = ImGui::GetIO();
    ImGui::Text("ERROR: Docking is not enabled! See Demo > Configuration.");
    ImGui::Text("Set io.ConfigFlags |= ImGuiConfigFlags_DockingEnable in your code, or ");
    ImGui::SameLine(0.0f, 0.0f);
    if (ImGui::SmallButton("click here"))
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}


void EditorLayer::ShowDockSpaceApp(bool* p_open)
{

    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    else
    {
        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
    // and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", p_open, window_flags);
    if (!opt_padding)
        ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // Submit the DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }
    else
    {
        ShowDockingDisabledMessage();
    }

    if (ImGui::BeginMenuBar())
    {
		// Menu bar example

		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::BeginMenu("Open file", "Ctrl+O")) {
				ImGui::MenuItem("Open nifti");
				ImGui::MenuItem("Open dicom");
				ImGui::MenuItem("Open nrrd");
				ImGui::EndMenu();
			}
			ImGui::MenuItem("Save", "Ctrl+S");
			ImGui::MenuItem("Save as..");
			if (ImGui::MenuItem("Exit"))
				Vortex::Application::GetApplication().Close();
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit")) {
			ImGui::MenuItem("Settings", "Ctrl+I");
			ImGui::MenuItem("Preferences", "Ctrl+P");
			if (ImGui::BeginMenu("View layout", "Ctrl+O")) {
				ImGui::MenuItem("Standard layout");
				ImGui::MenuItem("3D Interactive Focus");
				ImGui::MenuItem("Augmented Reality");
				ImGui::MenuItem("Virtual Reality");
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help")) {
			ImGui::MenuItem("Examples");
			ImGui::MenuItem("Tutorials");
			ImGui::MenuItem("Documentation");
			ImGui::MenuItem("About Vortex");
			ImGui::EndMenu();
		}


        if (ImGui::BeginMenu("Settings"))
        {
            if (ImGui::BeginMenu("Docking"))
			{	
				// Disabling fullscreen would allow the window to be moved to the front of other windows,
				// which we can't undo at the moment without finer window depth/z control.
				ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
				ImGui::MenuItem("Padding", NULL, &opt_padding);
				ImGui::Separator();

				if (ImGui::MenuItem("Flag: NoDockingOverCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingOverCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingOverCentralNode; }
				if (ImGui::MenuItem("Flag: NoDockingSplit",         "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingSplit) != 0))             { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingSplit; }
				if (ImGui::MenuItem("Flag: NoUndocking",            "", (dockspace_flags & ImGuiDockNodeFlags_NoUndocking) != 0))                { dockspace_flags ^= ImGuiDockNodeFlags_NoUndocking; }
				if (ImGui::MenuItem("Flag: NoResize",               "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0))                   { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
				if (ImGui::MenuItem("Flag: AutoHideTabBar",         "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0))             { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
				if (ImGui::MenuItem("Flag: PassthruCentralNode",    "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
				ImGui::Separator();

				if (ImGui::MenuItem("Close", NULL, false, p_open != NULL))
					*p_open = false;

				ImGui::EndMenu();
			}
			ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
		
    }

	bool showDemo = true;

	ImGui::ShowDemoWindow(&showDemo);

	ImGui::Begin("Settings");
	
	auto &SquareEntityTag = SquareEntity.GetComponent<Vortex::TagComponent>().Tag;
	auto &SquareEntityTransform = SquareEntity.GetComponent<Vortex::TransformComponent>().Transform;
	auto &SquareEntityColor = SquareEntity.GetComponent<Vortex::SpriteRendererComponent>().Color;

	ImGui::Separator();
	ImGui::Text("%s", SquareEntityTag.c_str());
	ImGui::ColorEdit4("Object Color", glm::value_ptr(SquareEntityColor));
	ImGui::SliderFloat3("Object Postion", glm::value_ptr(m_ImGuiComponents.ObjectPosition), -1.0f, 1.0f);
	ImGui::SliderFloat2("Object Size", glm::value_ptr(m_ImGuiComponents.ObjectSize), 0.0f, 10.0f);
	ImGui::SliderFloat("Object Rotation", &m_ImGuiComponents.ObjectRotation, 0.0f, 180.0f);
	ImGui::Separator();
	ImGui::ColorEdit4("Board Color", glm::value_ptr(m_ImGuiComponents.BoardColor));
	ImGui::SliderFloat("Board Rotation", &m_ImGuiComponents.BoardRotation, 0.0f, 180.0f);
	ImGui::SliderFloat("Tiling Factor", &m_ImGuiComponents.TilingFactor, 0.0f, 100.0f);
	ImGui::Separator();

	SquareEntityTransform = GetTransformQuad(m_ImGuiComponents.ObjectPosition, 
										m_ImGuiComponents.ObjectSize,
										m_ImGuiComponents.ObjectRotation);

    ImGui::End();

	ImGui::Begin("Viewport");


	// Viewport focused event
	is_ViewPortFocused = ImGui::IsWindowFocused();
	is_ViewPortHovered = ImGui::IsWindowHovered();
	Vortex::Application::GetApplication().GetImGuiLayer()->SetBlockEvent(!is_ViewPortFocused || !is_ViewPortHovered);

	ImVec2 ViewPortPanelSize = ImGui::GetContentRegionAvail();

	if (m_ViewPortSize != *((glm::vec2*)&ViewPortPanelSize))
	{
		m_Framebuffer->Resize((uint32_t)ViewPortPanelSize.x, (uint32_t)ViewPortPanelSize.y);
		m_ViewPortSize = {ViewPortPanelSize.x, ViewPortPanelSize.y}; 
	}

	uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
	ImGui::Image((void*)(uintptr_t)textureID, ImVec2{ m_ViewPortSize.x, m_ViewPortSize.y }, ImVec2{0, 1}, ImVec2{1, 0});
	ImGui::End();

    ImGui::End();
}


void EditorLayer::OnImGuiRender()
{
	bool showDockSpace = true;
	ShowDockSpaceApp(&showDockSpace);
}