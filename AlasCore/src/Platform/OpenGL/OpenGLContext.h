#pragma once

#include "Renderer/GraphicsContext.h"

struct SDL_Window;

namespace AGS {

	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(SDL_Window* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;
	private:
		SDL_Window* _windowHandle;
	};

}