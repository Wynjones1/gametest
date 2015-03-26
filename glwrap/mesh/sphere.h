#pragma once
#include "linemesh.h"

#define PI 3.1415926

template<uint32_t X = 20, uint32_t Y = 20>
class Sphere : public LineMesh
{
public:
	Sphere(float r = 1.0f)
		: radius(r)
	{
		for(uint32_t x = 0; x < X; x++)
		{
			for(uint32_t y = 0; y < Y; y++)
			{
				auto phi   = 2 * PI * x / X;
				auto theta = 2 * PI * y / Y;
				AddVertex(r * sin(theta) * sin(phi),
						  r * cos(theta),
						  r * sin(theta) * cos(phi));
				auto next_x = (x + 1) % X;
				auto next_y = (y + 1) % Y;
				uint32_t a = x      * Y + y;
				uint32_t b = next_x * Y + y;
				uint32_t c = next_x * Y + next_y;
				uint32_t d = x      * Y + next_y;
				AddLine(a, b);
				AddLine(b, c);
				AddLine(c, d);
				AddLine(d, a);
			}
		}
		InitBuffers();
	}

	bool Collides(const Sphere &lhs)
	{
		glm::vec3 t = pos - lhs.pos;
		auto l2 = t.x * t.x +
				 t.y * t.y +
				 t.z * t.z;
		auto r2 = radius + lhs.radius;
		r2 *= r2;
		return l2 <= r2;
	}

	float radius;
};

class GravitySphere : public Sphere<20,20>
{
public:
	GravitySphere(const glm::vec3 &pos,
				  const glm::vec3 &speed)
	{
		this->pos   = pos;
		this->speed = speed;
	}

	void Simulate(float dt)
	{
		pos += speed * dt;
	}

	void Draw(Program &program)
	{
		Mesh::Draw(program);
	}


	glm::vec3 speed;
};