#include "Vortex/Core/Core.h"
#include "Sandbox2D.h"
#include "Vortex/Imgui/ImGuiLayer.h"
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
    : Layer("Sandbox2D"), m_CameraController(1300.0f / 800.0f, true)
{
    
}

void Sandbox2D::OnAttach()
{
	VX_PROFILE_FUNCTION();
	m_ImGuiComponents.colorControl = glm::vec4(0.8f, 0.2f, 0.2f, 0.5f);
    m_CheckerboardTexture = Vortex::Texture2D::Create("/home/pc/dev/engine/Sandbox/assets/Textures/Checkerboard.png");
}

void Sandbox2D::OnDetach()
{
	VX_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(Vortex::TimeStep ts)
{
    // Key event controller for camera
	m_CameraController.OnUpdate(ts);

	{
		VX_PROFILE_SCOPE("Renderer Prep");
		Vortex::RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1 });
		Vortex::RenderCommand::ClearBufferBit();
	}

	{
		VX_PROFILE_SCOPE("Renderer Draw");
		Vortex::Renderer2D::BeginScene(m_CameraController.GetCamera(), m_ImGuiComponents);
		Vortex::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		Vortex::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		Vortex::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_CheckerboardTexture);
		Vortex::Renderer2D::EndScene();
	}	
}

void Sandbox2D::OnEvent(Vortex::Event &event) 
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


void Sandbox2D::ShowDockSpaceApp(bool* p_open)
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

	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_ImGuiComponents.colorControl));	
	ImGui::SliderFloat("Object Rotation", &m_ImGuiComponents.ObjectRotation, 0.0f, 180.0f);
	ImGui::SliderFloat("Board Rotation", &m_ImGuiComponents.BoardRotation, 0.0f, 180.0f);
	ImGui::SliderFloat("Tiling Factor", &m_ImGuiComponents.TilingFactor, 0.0f, 100.0f);
	// uint32_t textureID = m_CheckerboardTexture->GetRendererID();
	// ImGui::Image((void*)(uintptr_t)textureID, ImVec2{ 256.0f, 256.0f });

    ImGui::End();

    ImGui::End();
}


void Sandbox2D::OnImGuiRender()
{
	bool showDockSpace = true;
	ShowDockSpaceApp(&showDockSpace);
}