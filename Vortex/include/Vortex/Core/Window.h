#pragma once

#include "Vortex/Core/Core.h"
#include "Vortex/Core/Base.h"
#include "Vortex/Events/Event.h"
#include <sstream>

namespace Vortex {

	struct WindowProps
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;

		WindowProps(const std::string& title = "Vortex Engine",
			        uint32_t width = 1600,
			        uint32_t height = 900)
			: Title(title), Width(width), Height(height)
		{
		}
	};

	// Interface representing a desktop system based Window
	class VORTEX_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;
		virtual ~Window() = default;
		
		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;
		
		static Window* Create(const WindowProps &props = WindowProps());
	};

}