#pragma once
#include "drawable.h"
#include <glm/gtc/matrix_transform.hpp>

class Object : public Drawable
{
public:
	Object()
	: model_matrix(1.0)
	, alive(true)
	, pos(0.0)
	{}

	virtual void Draw(Program &program) = 0;
	virtual void Simulate(float dt) = 0;

	void CalculateModelMatrix(void)
	{
		model_matrix = glm::translate(glm::mat4(1.0), pos);
	}

	glm::mat4 model_matrix;
	glm::vec3 pos;
	bool alive;
};

void Draw(Object *d, Program &program);

class DynamicObject : public Object
{
public:
};

class StaticObject : public Object
{
public:
	void Simulate(float dt)
	{}
};