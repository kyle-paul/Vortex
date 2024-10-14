#include "Vortex/Core/Application.h"
#include <iostream>
#include <memory>

namespace Vortex 
{
	Application::Application()
	{

	}

	Application::~Application()
	{
		
	}

	void Application::Run() {
		for (int i=0; i<10; i++) {
			std::cout << "running\n";
		}	
	}
}
