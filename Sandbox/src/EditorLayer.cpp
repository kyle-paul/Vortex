#include "Vortex/Core/Core.h"
#include "Vortex/Imgui/ImGuiLayer.h"
#include <glm/gtc/type_ptr.hpp>
#include "EditorLayer.h"

#include "Vortex/Scene/SceneSerializer.h"
#include "Vortex/Utils/PlatformUtils.h"
#include "Vortex/Core/Math.h"
#include "ImGuizmo.h"

EditorLayer::EditorLayer()
    : Layer("EditorLayer")
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
	fbspec.Attachments = { Vortex::FramebufferTextureFormat::RGBA8, Vortex::FramebufferTextureFormat::RED_INTEGER, Vortex::FramebufferTextureFormat::Depth };
	fbspec.Width = 1300.0f;
	fbspec.Height = 800.0f;
	m_Framebuffer = Vortex::Framebuffer::Create(fbspec);

	// Texture registry
	m_IconPlay = Vortex::Texture2D::Create("Resources/Icons/PlayButton/PlayButton.png");
	m_IconStop = Vortex::Texture2D::Create("Resources/Icons/PlayButton/StopButton.png");

	// Intialize Scene 
	m_ActiveScene = Vortex::CreateRef<Vortex::Scene>();

	// Scene Hierachy Panel
	m_SceneHierarchyPanel = Vortex::CreateRef<Vortex::SceneHierarchyPanel>();
	m_SceneHierarchyPanel->SetContext(m_ActiveScene);
	m_ContentBrowserPanel.SetSceneHierarcyPanel(m_SceneHierarchyPanel);

	// Editor Camera
	m_EditorCamera = Vortex::EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

	// Node Editor
	// m_NodeEditor.OnAttach();
	m_NodePanels.OnAttach();

#if 0
	auto Square1 = m_ActiveScene->CreateEntity("Square Entity");
	Square1.AddComponent<Vortex::SpriteRendererComponent>(m_ImGuiComponents.ObjectColor);

	auto CameraEntity = m_ActiveScene->CreateEntity("Camera Entity");
	CameraEntity.AddComponent<Vortex::CameraComponent>();

	auto CameraEntity2 = m_ActiveScene->CreateEntity("Camera Entity 2");
	auto &cam2 = CameraEntity2.AddComponent<Vortex::CameraComponent>();
	cam2.Primary = false;

	auto Square2 = m_ActiveScene->CreateEntity("Square Entity 2");
	Square2.AddComponent<Vortex::SpriteRendererComponent>(glm::vec4{ 0.2f, 0.2f, 0.8f, 1.0f });
	
	// Native Scripting
	class CameraController : public Vortex::ScriptableEntity
	{
	public:
		void OnCreate()
		{
			auto &translation = GetComponent<Vortex::TransformComponent>().Translation;
			translation.x = rand() % 10 - 5.0f;
		}	

		void OnDestroy()
		{
		}

		void OnUpdate(Vortex::TimeStep ts)
		{
			float speed = 5.0f;
			auto& translation = GetComponent<Vortex::TransformComponent>().Translation;

			if (Vortex::Input::IsKeyPressed(Vortex::Key::A))
				translation.x -= speed * ts;
			if (Vortex::Input::IsKeyPressed(Vortex::Key::D))
				translation.x += speed * ts;
			if (Vortex::Input::IsKeyPressed(Vortex::Key::W))
				translation.y += speed * ts;
			if (Vortex::Input::IsKeyPressed(Vortex::Key::S))
				translation.y -= speed * ts;
		}
	};

	CameraEntity.AddComponent<Vortex::NativeScriptComponent>().Bind<CameraController>();
	CameraEntity2.AddComponent<Vortex::NativeScriptComponent>().Bind<CameraController>();
#endif
}

void EditorLayer::OnDetach()
{
	VX_PROFILE_FUNCTION();
	// m_NodeEditor.OnDetach();
	m_NodePanels.OnDetach();
}

void EditorLayer::OnUpdate(Vortex::TimeStep ts)
{
	
	// ================ Resize frame buffer ================
	if (Vortex::FramebufferSpecification spec = m_Framebuffer->GetSpecification();
		m_ViewPortSize.x > 0.0f && m_ViewPortSize.y > 0.0f &&
		(spec.Width != m_ViewPortSize.x || spec.Height != m_ViewPortSize.y))
	{
		m_Framebuffer->Resize((uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);
		m_EditorCamera.SetViewportSize(m_ViewPortSize.x, m_ViewPortSize.y);
		m_ActiveScene->OnViewPortResize((uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);
	}

	m_EditorCamera.OnUpdate(ts);
	m_Framebuffer->Bind();

	// ================ Clear ================
	Vortex::RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1 });
	Vortex::RenderCommand::ClearBufferBit();
	m_Framebuffer->ClearAttachment(1, -1);

	// ================ Display Scene ================
	switch(m_SceneState)
	{
		case SceneState::Edit:
		{
			m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);
			break;
		}
		case SceneState::Play:
		{
			m_ActiveScene->OnUpdateRuntime(ts);
			break;
		}
	}
	
	// ================ Clicking object ================
	auto[mx, my] = ImGui::GetMousePos();
	mx -= m_ViewportBounds[0].x;
	my -= m_ViewportBounds[0].y;

	glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
	my = viewportSize.y - my;

	int mouseX = (int)mx;
	int mouseY = (int)my;

	if (mouseX > 0 && mouseY > 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y 
		&& Vortex::Input::IsMouseButtonPressed(Vortex::Mouse::ButtonLeft) 
		&& !Vortex::Input::IsKeyPressed(Vortex::Key::LeftAlt)
		&& !ImGuizmo::IsOver())
	{
		CurrentPixelData = m_Framebuffer->ReadPixel(1, mouseX, mouseY);
		if (CurrentPixelData >= 0 && CurrentPixelData <= 100)
		{
			MouseSelectedEntity = Vortex::Entity(static_cast<entt::entity>(CurrentPixelData), m_ActiveScene.get());
			if (MouseSelectedEntity && MouseSelectedEntity.HasComponent<Vortex::TagComponent>())
			{
				m_SceneHierarchyPanel->SetSelectionContext(MouseSelectedEntity);
			}
		}
	}

	m_Framebuffer->Unbind();
}

void EditorLayer::OnEvent(Vortex::Event &event) 
{
	m_EditorCamera.OnEvent(event);
	Vortex::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Vortex::KeyPressedEvent>(VX_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
}

bool EditorLayer::OnKeyPressed(Vortex::KeyPressedEvent& event)
{
	// ================ Shortcut ================
	if (event.IsRepeat()) return false;
	bool control = Vortex::Input::IsKeyPressed(Vortex::Key::LeftControl) || Vortex::Input::IsKeyPressed(Vortex::Key::RightControl);
	bool shift = Vortex::Input::IsKeyPressed(Vortex::Key::LeftShift) || Vortex::Input::IsKeyPressed(Vortex::Key::RightShift);

	switch (event.GetKeyCode())
	{
		// ================ File Dialogs ================
		case Vortex::Key::O:
		{
			if (control) LoadScene();
			break;
		}
		case Vortex::Key::L:
		{
			if (control) ClearScene();
			break;
		}
		case Vortex::Key::S:
		{
			if (control && shift) SaveSceneAs();
			else if (control) SaveDefault();
			else m_GizmoType = ImGuizmo::OPERATION::SCALE;
			break;
		}

		// ================ Gizmo ================
		case Vortex::Key::Q: 
		{
			m_GizmoType = -1;
			break;
		}
		case Vortex::Key::T:
		{
			m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
			break;
		}
		case Vortex::Key::R:
		{
			m_GizmoType = ImGuizmo::OPERATION::ROTATE;
			break;			
		}
			
	}
	return false;
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
	// ================ Window & Dockspace ================
	static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
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

    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;
    
    if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", p_open, window_flags);
    if (!opt_padding)
        ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // Submit the DockSpace
    ImGuiIO& io = ImGui::GetIO();
	ImGuiStyle& style = ImGui::GetStyle();
	float minWinSizeX = style.WindowMinSize.x;
	style.WindowMinSize.x = 380.0f;
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }
    else
    {
        ShowDockingDisabledMessage();
    }
	style.WindowMinSize.x = minWinSizeX;

    
	// ================ Menu bar ================
	if (ImGui::BeginMenuBar())
    {
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Clear Scene", "Ctrl+L"))
			{
				ClearScene();
			}

			if (ImGui::BeginMenu("Open file", "Ctrl+O")) {
				ImGui::MenuItem("Open nifti");
				ImGui::MenuItem("Open dicom");
				ImGui::MenuItem("Open nrrd");
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Load Scene", "Crtl+O"))
			{
				LoadScene();
			}
			if (ImGui::MenuItem("Save Scene", "Ctrl+S")) 
			{
				SaveDefault();
			}
			if(ImGui::MenuItem("Save as..", "Ctrl+Shift+S"))
			{
				SaveSceneAs();
			}
			if (ImGui::MenuItem("Exit"))
			{
				Vortex::SceneSerializer serializer(m_ActiveScene);
				serializer.Serialize("assets/Scenes/Vortex.vx");
				Vortex::Application::GetApplication().Close();
			}
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

	// ================ Node Editor ================
	// m_NodeEditor.OnImGuiRender();

	// ================ Scene Hierarchy ================
	m_SceneHierarchyPanel->OnImGuiRender();
	m_ContentBrowserPanel.OnImGuiRender();

	// ================ Demo Window ================
	bool showDemo = true;
	ImGui::ShowDemoWindow(&showDemo);

	// ================ Setting Panel ================
	ImGui::Begin("Settings");
	if (ImGui::Button("Render Brain")) {
		Vortex::Mesh MeshObj = Vortex::Mesh("/home/pc/dev/dataset/medical-image/MbrainS/objects/brain.obj");

		// TODO; This is then load by content browser by taking the path and registert as entity
		auto brain = m_ActiveScene->CreateEntity("Brain");
		brain.AddComponent<Vortex::MeshComponent>(MeshObj);
		brain.AddComponent<Vortex::SpriteRendererComponent>(glm::vec4(0.5f, 0.0f, 0.2f, 1.0f));
		brain.GetComponent<Vortex::TransformComponent>().Scale = glm::vec3(0.2f);
	}

	if (ImGui::Button("Render Robot Arms")) {
		Vortex::Mesh MeshObj = Vortex::Mesh("/home/pc/dev/engine/Sandbox/assets/Meshes/models/RobotArms.obj");

		// TODO; This is then load by content browser by taking the path and registert as entity
		auto robot = m_ActiveScene->CreateEntity("Robot Arms");
		robot.AddComponent<Vortex::MeshComponent>(MeshObj);
		robot.AddComponent<Vortex::SpriteRendererComponent>(glm::vec4(0.5f, 0.0f, 0.2f, 1.0f));
		robot.GetComponent<Vortex::TransformComponent>().Scale = glm::vec3(0.2f);
	}

    ImGui::End(); // Setting

	// ================ Render Viewport ================
	RenderViewPort();

	// ================ UI Toolbar ================
	PlayToolBar();

    ImGui::End(); // DockSpace
}

void EditorLayer::LoadScene()
{
	std::string filepath = Vortex::FileDialogs::OpenFile("Vortex Scene (*.vx)\0*.vortex\0");
	if (!filepath.empty()) 
	{
		OpenScene(filepath);
	}
}

void EditorLayer::OpenScene(const std::string &filepath)
{
	m_ActiveScene = Vortex::CreateRef<Vortex::Scene>();
	m_ActiveScene->OnViewPortResize((uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);
	m_SceneHierarchyPanel->SetContext(m_ActiveScene);
	Vortex::SceneSerializer serializer(m_ActiveScene);
	serializer.Deserialize(filepath);
}

void EditorLayer::ClearScene()
{
	m_ActiveScene = Vortex::CreateRef<Vortex::Scene>();
	m_ActiveScene->OnViewPortResize((uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);
	m_SceneHierarchyPanel->SetContext(m_ActiveScene);
}

void EditorLayer::SaveSceneAs()
{
	std::string filepath = Vortex::FileDialogs::SaveFile("Vortex Scene (*.vx)\0*.vortex\0");
	if (!filepath.empty())
	{
		Vortex::SceneSerializer serializer(m_ActiveScene);
		serializer.Serialize(filepath);
	}
}

void EditorLayer::SaveDefault()
{
	Vortex::SceneSerializer serializer(m_ActiveScene);
	serializer.Serialize("assets/Scenes/default.vx");
}

void EditorLayer::OnImGuiRender()
{
	bool showDockSpace = true;
	ShowDockSpaceApp(&showDockSpace);

	m_NodePanels.OnImGuiRender();
}

void EditorLayer::PlayToolBar()
{
	Vortex::Ref<Vortex::Texture2D> icon = m_SceneState == SceneState::Edit ? m_IconPlay : m_IconStop;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

	// Get default color
	auto& colors = ImGui::GetStyle().Colors;
	const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));

	const auto& buttonActive = colors[ImGuiCol_ButtonActive];
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

	ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoTitleBar);

	float size = ImGui::GetWindowHeight() - 4.0f;
	ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - size * 0.5f);

	if (ImGui::ImageButton("PlayButton", reinterpret_cast<ImTextureID>(static_cast<uintptr_t>(icon->GetRendererID())), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1)))
	{
		if (m_SceneState == SceneState::Edit)
		{
			OnScenePlay();
		}
		else if (m_SceneState == SceneState::Play)
		{
			OnSceneStop();
		}
	}
	ImGui::PopStyleVar(2);
	ImGui::PopStyleColor(3);
	ImGui::End();
}

void EditorLayer::OnScenePlay()
{
	m_SceneState = SceneState::Play;
}

void EditorLayer::OnSceneStop()
{
	m_SceneState = SceneState::Edit;
}

void EditorLayer::RenderViewPort()
{
	ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoTitleBar);
	auto viewportOffset = ImGui::GetCursorPos();
	
	is_ViewPortFocused = ImGui::IsWindowFocused();
	is_ViewPortHovered = ImGui::IsWindowHovered();
	Vortex::Application::GetApplication().GetImGuiLayer()->SetBlockEvent(!is_ViewPortFocused & !is_ViewPortHovered);

	ImVec2 ViewPortPanelSize = ImGui::GetContentRegionAvail();
	m_ViewPortSize = { ViewPortPanelSize.x, ViewPortPanelSize.y };

	uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
	ImGui::Image((void*)(uintptr_t)textureID, ImVec2{ m_ViewPortSize.x, m_ViewPortSize.y }, ImVec2{0, 1}, ImVec2{1, 0});

	// ================ DragDrop Browser Content ================
	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload *scene_payload = ImGui::AcceptDragDropPayload("SCENE_ITEM");
		if (scene_payload != nullptr && scene_payload->Data != nullptr)
		{
			std::string path(static_cast<const char*>(scene_payload->Data), scene_payload->DataSize - 1);
			OpenScene(path);
		}

		const ImGuiPayload *tex_payload = ImGui::AcceptDragDropPayload("TEXTURE_ITEM");
		if (tex_payload != nullptr && tex_payload->Data != nullptr)
		{
			if (CurrentPixelData != -1)
			{	
				std::string path(static_cast<const char*>(tex_payload->Data), tex_payload->DataSize - 1);
				m_SceneHierarchyPanel->UpdateTexture(path);
			}			
		}

		ImGui::EndDragDropTarget();
	}

	// ================ Viewport Bound ================
	auto windowSize = ImGui::GetWindowSize();
	ImVec2 minBound = ImGui::GetWindowPos();

	minBound.x += viewportOffset.x;
	minBound.y += viewportOffset.y;

	ImVec2 maxBound = { minBound.x + windowSize.x, minBound.y + windowSize.y };
	m_ViewportBounds[0] = { minBound.x, minBound.y };
	m_ViewportBounds[1] = { maxBound.x, maxBound.y };

	// ================ Gizmo ================
	Vortex::Entity SelectedEntity = m_SceneHierarchyPanel->GetSelectedEntity();
	
	if (SelectedEntity && m_GizmoType != -1)
	{
		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist();

		// Get Window
		float windowWidth = (float)ImGui::GetWindowWidth();
		float windowHeight = (float)ImGui::GetWindowHeight();
		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

		// Get Primary Camera (projection + view matrix)
		auto PrimaryCameraEntity = m_ActiveScene->GetPrimaryCameraEntity();

		// glm::mat4 camera_proj = glm::mat4(1.0f);
		// glm::mat4 camera_view = glm::mat4(1.0f);

		// if (PrimaryCameraEntity)
		// {
		// 	const auto &cam = PrimaryCameraEntity.GetComponent<Vortex::CameraComponent>().Camera;
		// 	camera_proj = cam.GetProjection();
		// 	camera_view = glm::inverse(PrimaryCameraEntity.GetComponent<Vortex::TransformComponent>().GetTransform());
		// }

		// Editor camera
		const glm::mat4& camera_proj = m_EditorCamera.GetProjection();
		glm::mat4 camera_view = m_EditorCamera.GetViewMatrix();

		// Entity Transform
		auto &transform_comp = SelectedEntity.GetComponent<Vortex::TransformComponent>();
		glm::mat4 transform_object = transform_comp.GetTransform();

		// Snapping
		bool snap = Vortex::Input::IsKeyPressed(Vortex::Key::LeftControl);
		float snapValue = 0.5f; // Snap to 0.5m for translation/scale
		
		// Snap to 45 degrees for rotation
		if (m_GizmoType == ImGuizmo::OPERATION::ROTATE) snapValue = 45.0f;
		float snapValues[3] = { snapValue, snapValue, snapValue };

		// Manipulate
		ImGuizmo::Manipulate(glm::value_ptr(camera_view), glm::value_ptr(camera_proj), 
			(ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform_object),
			nullptr, snap ? snapValues : nullptr);

		if (ImGuizmo::IsUsing())
		{
			glm::vec3 translation, rotation, scale;
			Vortex::Math::DecomposeTransform(transform_object, translation, rotation, scale);
			glm::vec3 deltaRotation = rotation - transform_comp.Rotation;
			transform_comp.Translation = translation;
			transform_comp.Rotation += deltaRotation;
			transform_comp.Scale = scale;
		}
	}

	ImGui::End(); // Viewport
}