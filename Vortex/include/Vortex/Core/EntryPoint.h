#pragma once
#include "Vortex/Core/Core.h"

extern Vortex::Application* Vortex::CreateApplication();

int main(int argc, char** argv) 
{
	Vortex::Logging::Init();

	int a = 10;
	VX_CORE_WARN("Initialized Logging !");
	VX_INFO("WELCOME! Var={0}", a);

	VX_PROFILE_BEGIN_SESSION("Startup", "VortexProfile-Startup.json");
	auto app = Vortex::CreateApplication();
	VX_PROFILE_END_SESSION();

	VX_PROFILE_BEGIN_SESSION("Runtime", "VortexProfile-Runtime.json");
	app->Run();
	VX_PROFILE_END_SESSION(); 

	VX_PROFILE_BEGIN_SESSION("Startup", "VortexProfile-Shutdown.json");
	delete app;
	VX_PROFILE_END_SESSION();
}