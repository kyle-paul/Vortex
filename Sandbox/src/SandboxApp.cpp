#include "Vortex.h"
#include "GameLayer.h"
#include "Vortex/Core/EntryPoint.h"

class Sandbox : public Vortex::Application 
{
public:
	Sandbox() {
		PushLayer(new GameLayer());
	}

	~Sandbox () {

	}
};

Vortex::Application* Vortex::CreateApplication()
{
	return new Sandbox();
}