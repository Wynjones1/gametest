#include "input_state.h"

InputState::InputState(void){}

InputState &InputState::GetInstance()
{
	static InputState instance;
	return instance;
}