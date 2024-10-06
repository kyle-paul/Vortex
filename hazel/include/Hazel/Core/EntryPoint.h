#pragma once

extern Hazel::Application* Hazel::CreateApplication();

int main(int argc, char** argv) 
{
	Hazel::Logging::Init();

	HZ_CORE_WARN("Initialized Logging !");
	int a = 10;
	HZ_INFO("WELCOME! Var={0}", a);

	auto app = Hazel::CreateApplication();
	app->Run();
	delete app;
}