#include "events.h"
#include "input_state.h"
#include <iostream>

EventHandler::EventHandler()
	: input_state(InputState::GetInstance())
{}

void EventHandler::KeyUp()
{
	switch(event.key.keysym.sym)
	{
	case SDLK_UP:
		input_state.up    = false;
		break;
	case SDLK_DOWN:
		input_state.down  = false;
		break;
	case SDLK_LEFT:
		input_state.left  = false;
		break;
	case SDLK_RIGHT:
		input_state.right = false;
	case SDLK_SPACE:
		input_state.space_key = false;
		break;
	}

	if(event.key.keysym.sym >= 'a' && event.key.keysym.sym <= 'z')
	{
		input_state.SetLetterPressed(event.key.keysym.sym, false);
	}
}

void EventHandler::KeyDown()
{
	switch(event.key.keysym.sym)
	{
	case SDLK_ESCAPE:
		exit(0);
	case SDLK_UP:
		input_state.up    = true;
		break;
	case SDLK_DOWN:
		input_state.down  = true;
		break;
	case SDLK_LEFT:
		input_state.left  = true;
		break;
	case SDLK_RIGHT:
		input_state.right = true;
		break;
	case SDLK_SPACE:
		input_state.space_key = true;
		break;
	}

	if(event.key.keysym.sym >= 'a' && event.key.keysym.sym <= 'z')
	{
		input_state.SetLetterPressed(event.key.keysym.sym, true);
	}
}

void EventHandler::MouseMotion()
{
	input_state.mouse_rot[0] = (float) -event.motion.xrel;
	input_state.mouse_rot[1] = (float) event.motion.yrel;
}

void EventHandler::ResetMouseMotion()
{
	input_state.mouse_rot[0] = 0;
	input_state.mouse_rot[1] = 0;
}

void EventHandler::MouseButtonUp(void)
{
	switch(event.button.button)
	{
		case SDL_BUTTON_LEFT:
			input_state.mouse_buttons[0] = 0;
			break;
		case SDL_BUTTON_MIDDLE:
			input_state.mouse_buttons[1] = 0;
			break;
		case SDL_BUTTON_RIGHT:
			input_state.mouse_buttons[2] = 0;
			break;
	}
}

void EventHandler::MouseButtonDown(void)
{
	switch(event.button.button)
	{
		case SDL_BUTTON_LEFT:
			input_state.mouse_buttons[0] = 1;
			break;
		case SDL_BUTTON_MIDDLE:
			input_state.mouse_buttons[1] = 1;
			break;
		case SDL_BUTTON_RIGHT:
			input_state.mouse_buttons[2] = 1;
			break;
	}
}

void EventHandler::HandleEvents()
{
	ResetMouseMotion();
	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
		case SDL_QUIT:
			exit(0);
		case SDL_KEYDOWN:
			KeyDown();
			break;
		case SDL_KEYUP:
			KeyUp();
			break;
		case SDL_MOUSEMOTION:
			MouseMotion();
			break;
		case SDL_MOUSEBUTTONUP:
			MouseButtonUp();
			break;
		case SDL_MOUSEBUTTONDOWN:
			MouseButtonDown();
			break;
		}
	}
}
