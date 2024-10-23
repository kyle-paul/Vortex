#include "Vortex.h"
#include "Sandbox2D.h"
#include "Vortex/Core/EntryPoint.h"

class Sandbox : public Vortex::Application 
{
public:
	Sandbox() {
		PushLayer(new Sandbox2D());
	}

	~Sandbox () {

	}
};

Vortex::Application* Vortex::CreateApplication()
{
	return new Sandbox();
}