#pragma once
#include "mesh.h"

class LineMesh : public Mesh
{
public:
	LineMesh() : Mesh(false)
	{
		wireframe = true;
	}

	template<typename T>
	void AddLine(T &d)
	{
		AddLine(0, 1);
	}

	template<typename T>
	void AddLine(T d0, T d1)
	{
		AddFace(d0, d1, d0);
	}

	void Draw(Program &program)
	{
		glDisable(GL_DEPTH_TEST);
		Mesh::Draw(program);
		glEnable(GL_DEPTH_TEST);
	}
};

class LineCube : public LineMesh
{
public:
	LineCube(glm::vec3 min, glm::vec3 max) : LineMesh()
	{
		AddVertex(min[0], min[1], min[2]);
		AddVertex(min[0], min[1], max[2]);
		AddVertex(min[0], max[1], min[2]);
		AddVertex(min[0], max[1], max[2]);
		AddVertex(max[0], min[1], min[2]);
		AddVertex(max[0], min[1], max[2]);
		AddVertex(max[0], max[1], min[2]);
		AddVertex(max[0], max[1], max[2]);


		AddLine(0, 1);
		AddLine(0, 2);
		AddLine(0, 4);
		AddLine(1, 3);
		AddLine(1, 5);
		AddLine(2, 3);
		AddLine(2, 6);
		AddLine(3, 7);
		AddLine(4, 5);
		AddLine(4, 6);
		AddLine(5, 7);
		AddLine(6, 7);
		InitBuffers();
	}
};