#include <Vortex.h>

class RenderLayer : public Vortex::Layer  
{
public:
	RenderLayer() : Layer("RenderLayer") {

	}

	~RenderLayer() {

	}

	void OnUpdate() override {
		VX_INFO("RenderLayer::Update()");
	}

	void OnEvent(Vortex::Event &event) override {
		VX_TRACE("{0}", event.ToString());
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