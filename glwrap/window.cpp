#include "window.h"
#include <GL/glew.h>
#include <iostream>

Window::Window(int width, int height)
	: width(width)
	, height(height)
{
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cout << "SDL could not be initialised." << std::endl;
		exit(-1);
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);

	Uint32 flags = SDL_WINDOW_OPENGL;
	window = SDL_CreateWindow("Main Window", 20, 20, width, height, flags);

	if(!window)
	{
		std::cout << "Could not Create the window." << std::endl;
		exit(-1);
	}

	context = SDL_GL_CreateContext(window);

	if(glewInit())
	{
		exit(1);
	}
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	SwapBuffer();
}

void Window::SwapBuffer(void)
{
	SDL_GL_SwapWindow(window);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

Window::~Window(void)
{
}
