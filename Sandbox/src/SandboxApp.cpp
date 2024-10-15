#include <Vortex.h>

class RenderLayer : public Vortex::Layer  
{
public:
	RenderLayer() : Layer("RenderLayer") {

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