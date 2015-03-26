#include "player.h"
#include "input_state.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <algorithm>
#define INV_SQRT2 0.70710678118f

Player::Player()
: input_state(InputState::GetInstance())
, rot_speed(0.01f)
, speed(10.0f)
{
	pos[2] = 2.0;
	rot[0] = 0.0f;
	rot[1] = 0.0f;
}

void Player::Simulate(float dt)
{
	if(input_state.LetterPressed('w'))
	{
		MoveForward(dt);
	}
	if(input_state.LetterPressed('s'))
	{
		MoveBackward(dt);
	}
	if(input_state.LetterPressed('a'))
	{
		StrifeLeft(dt);
	}
	if(input_state.LetterPressed('d'))
	{
		StrifeRight(dt);
	}
	if(input_state.mouse_rot[0] || input_state.mouse_rot[1])
	{
		Turn();
	}
}

void Player::Turn(void)
{
	rot[0] += (float) input_state.mouse_rot[0] * rot_speed;
	rot[1] += (float) input_state.mouse_rot[1] * rot_speed;
	//Restrict our ability to look up/down.
	rot[1] = std::min(std::max(rot[1], -3.1415926f / 2), 3.1415926f / 2);
}

void Player::MoveForward(double dt)
{
	float step = 1.0;
	if(input_state.LetterPressed('a') != input_state.LetterPressed('d'))
	{
		step = INV_SQRT2;
	}
	glm::vec4 mov(0, 0, speed * dt * -step, 1);
	mov     = glm::rotate(glm::mat4(), rot[0], glm::vec3(0, 1, 0)) * mov;
	pos[0] += mov[0];
	pos[1] += mov[1];
	pos[2] += mov[2];
}

void Player::MoveBackward(double dt)
{
	float step = 1.0;
	if(input_state.LetterPressed('a') != input_state.LetterPressed('d'))
	{
		step = INV_SQRT2;
	}
	glm::vec4 mov(0, 0, speed * dt * step, 1);
	mov     = glm::rotate(glm::mat4(), rot[0], glm::vec3(0, 1, 0)) * mov;
	pos[0] += mov[0];
	pos[1] += mov[1];
	pos[2] += mov[2];
}

void Player::TurnLeft(double dt)
{
	rot[0] += (float)(rot_speed * dt);
}

void Player::TurnRight(double dt)
{
	rot[0] -= (float)(rot_speed * dt);
}

void Player::TurnUp(double dt)
{
	rot[1] -= (float)(rot_speed * dt);
}

void Player::TurnDown(double dt)
{
	rot[1] += (float)(rot_speed * dt);
}

glm::mat4 Player::GetViewMatrix(void)
{
	//First rotate our view
	auto r = glm::rotate(glm::mat4(1.0), rot[1], glm::vec3(1, 0, 0));
	r = glm::rotate(r, -rot[0], glm::vec3(0, 1, 0));
	//Now Translate to the pos.
	auto t = glm::translate(r, -pos);
	return t;
}

void Player::StrifeLeft(double dt)
{
	float step = 1.0;
	if(input_state.LetterPressed('w') != input_state.LetterPressed('s'))
	{
		step = INV_SQRT2;
	}
	glm::vec4 mov(speed * dt * -step, 0, 0, 1);
	mov     = glm::rotate(glm::mat4(), rot[0], glm::vec3(0, 1, 0)) * mov;
	pos[0] += mov[0];
	pos[1] += mov[1];
	pos[2] += mov[2];
}

void Player::StrifeRight(double dt)
{
	float step = 1.0;
	if(input_state.LetterPressed('w') != input_state.LetterPressed('s'))
	{
		step = INV_SQRT2;
	}
	glm::vec4 mov(speed * dt * step, 0, 0, 1);
	mov     = glm::rotate(glm::mat4(), rot[0], glm::vec3(0, 1, 0)) * mov;
	pos[0] += mov[0];
	pos[1] += mov[1];
	pos[2] += mov[2];
}

void Player::Draw(Program &prog){};