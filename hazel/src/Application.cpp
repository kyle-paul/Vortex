#include "Hazel/Application.h"
#include <iostream>
#include <memory>

namespace Hazel 
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
