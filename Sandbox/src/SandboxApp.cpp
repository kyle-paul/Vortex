#include "Vortex.h"
#include "EditorLayer.h"
#include "Vortex/Core/EntryPoint.h"

class VortexEditor : public Vortex::Application 
{
public:
	VortexEditor() : Application("Vortex Application")
	{
		PushLayer(new EditorLayer());
	}

	~VortexEditor () 
	{

	}
};

Vortex::Application* Vortex::CreateApplication()
{
	return new VortexEditor();
}