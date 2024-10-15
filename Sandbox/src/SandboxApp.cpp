#include <Vortex.h>
#include <Vortex/Core/Math.h>

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

	void OnUpdate() override {
		if (Vortex::Input::IsKeyPressed(Vortex::Key::Tab)) {
			// VX_WARN("Key {0} tab is pressed");
		}

		if (Vortex::Input::IsMouseButtonPressed(Vortex::Mouse::ButtonLeft)) {
			// VX_WARN("Press button Left");
		}
		// VX_INFO("RenderLayer::Update()");
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
		PushOverLayer(new Vortex::ImGuiLayer());
	}

	~Sandbox () {

	}
};

Vortex::Application* Vortex::CreateApplication()
{
	return new Sandbox();
}