
#pragma once

// Include Core
#include "Vortex/Core/Logging.h"
#include "Vortex/Core/Base.h"
#include "Vortex/Core/Core.h"
#include "Vortex/Core/Layer.h"
#include "Vortex/Core/LayerStack.h"
#include "Vortex/Core/Input.h"
#include "Vortex/Core/Application.h"
#include "Vortex/Core/TimeStep.h"
#include "Vortex/Core/Math.h"

// Include input codes
#include "Vortex/Core/KeyCodes.h"
#include "Vortex/Core/MouseCodes.h"

// Include debug
#include "Vortex/Debug/Instrumentor.h"

// Include renderer
#include "Graphics/Buffer.h"
#include "Graphics/Shader.h"
#include "Graphics/Texture.h"
#include "Graphics/Renderer.h"
#include "Graphics/Renderer2D.h"
#include "Graphics/RenderCommand.h"
#include "Graphics/OrthographicCamera.h"
#include "Graphics/OrthographicCameraController.h"
#include "Graphics/Framebuffer.h"

// Include Scene
#include "Vortex/Scene/Scene.h"
#include "Vortex/Scene/SceneCamera.h"
#include "Vortex/Scene/Component.h"
#include "Vortex/Scene/Entity.h"
#include "Vortex/Scene/ScriptableEntity.h"
#include "Graphics/EditorCamera.h"
#include "Graphics/Mesh.h"

// Include platform
#include "Platform/Linux/LinuxWindow.h"

// Include imgui
#include "imgui.h"
#include "Vortex/Imgui/ImGuiLayer.h"
#include "Vortex/Imgui/ImGuiLayerBind.h"