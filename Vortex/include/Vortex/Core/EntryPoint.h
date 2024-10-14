#pragma once

extern Vortex::Application* Vortex::CreateApplication();

int main(int argc, char** argv) 
{
	Vortex::Logging::Init();

	int a = 10;
	VX_CORE_WARN("Initialized Logging !");
	VX_INFO("WELCOME! Var={0}", a);

	auto app = Vortex::CreateApplication();
	app->Run();
	delete app;
}