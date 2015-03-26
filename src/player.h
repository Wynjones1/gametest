#pragma once
#include "object.h"
#include "input_state.h"
#include "mesh.h"

class Player : public DynamicObject
{
public:
	Player();

	InputState &input_state;
	float     rot[2];
	float     rot_speed;
	float     speed;

	void      MoveForward(double dt);
	void      MoveBackward(double dt);
	void      TurnLeft(double dt);
	void      TurnRight(double dt);
	void      TurnUp(double dt);
	void      TurnDown(double dt);
	void      Turn(void);
	void      StrifeLeft(double dt);
	void      StrifeRight(double dt);
	glm::mat4 GetViewMatrix(void);
	void      Simulate(float dt);
	void      Draw(Program &prog);
};
