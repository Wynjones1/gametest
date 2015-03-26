#pragma once
#include "SDL.h"
#include "input_state.h"

class EventHandler
{
public:
	EventHandler();
	SDL_Event event;
	InputState &input_state;
	void HandleEvents();
	void KeyDown();
	void KeyUp();
	void MouseMotion();
	void MouseButtonUp(void);
	void MouseButtonDown(void);
	void ResetMouseMotion();
};
