#include <Vortex.h>
#include <Vortex/Core/Math.h>
#include <imgui.h>

glm::mat4 camera(float Translate, glm::vec2 const& Rotate)
{
	glm::mat4 Projection = glm::perspective(glm::pi<float>() * 0.25f, 4.0f / 3.0f, 0.1f, 100.f);
	glm::mat4 View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -Translate));
	View = glm::rotate(View, Rotate.y, glm::vec3(-1.0f, 0.0f, 0.0f));
	View = glm::rotate(View, Rotate.x, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
	return Projection * View * Model;
}

class RenderLayer : public Vortex::Layer  
{
public:
	RenderLayer() : Layer("RenderLayer") {
		glm::mat4 cam = camera(5.0f,  {0.5f, 0.5f});
	}

	~RenderLayer() {

	}

	// virtual void OnImGuiRender() override {
	// 	Vortex::ImGuiRender::Begin("Test");
	// 	Vortex::ImGuiRender::Text("This is sandbox");
	// 	Vortex::ImGuiRender::End();
	// }

	void OnUpdate() override {
		// VX_INFO("RenderLayer::Update()");
		// if (Vortex::Input::IsKeyPressed(Vortex::Key::Tab))
			// VX_WARN("Key {0} tab is pressed");

		// if (Vortex::Input::IsMouseButtonPressed(Vortex::Mouse::ButtonLeft))
			// VX_WARN("Press button Left");
	}

	void OnEvent(Vortex::Event &event) override {
		// VX_TRACE("{0}", event.ToString());
	}
};


class Sandbox : public Vortex::Application 
{
public:
	Sandbox() {
		PushLayer(new RenderLayer());
	}

	~Sandbox () {

	}
};

Vortex::Application* Vortex::CreateApplication()
{
	return new Sandbox();
}