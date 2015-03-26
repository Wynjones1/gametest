#pragma once 
#include "object.h"

class Mesh : public StaticObject
{
public:
	Mesh(bool init = true);
	Mesh(const char *filename, const char *texturename = NULL);

	void Draw(Program &program);

	void ReadHeader(FILE *fp, int &num_vertices, int &num_faces, bool &have_normals, bool &have_tex);
	void ReadVertexData(FILE *fp, int num, bool have_normals, bool have_tex);
	void ReadFaceData(FILE *fp, int num);

	Texture  *texture;
	bool      wireframe;
	glm::vec3 colour;

	void SetColour(const glm::vec3 &colour)
	{
		this->colour = colour;
	}
protected:
	template<typename T>
	void AddVertex(T &d)
	{
		AddVertex(d[0], d[1], d[2]);
	}

	template<typename T>
	void AddVertex(T v0, T v1, T v2)
	{
		vertices.push_back(glm::vec3(v0, v1, v2));
	}
	
	template<typename T>
	void AddNormal(T &data)
	{
		AddNormal(data[0], data[1], data[2]);
	}

	template<typename T>
	void AddNormal(T n0, T n1, T n2)
	{
		normals.push_back(glm::vec3(n0, n1, n2));
	}
	
	template<typename T>
	void AddTexcoord(T &data)
	{
		AddTexcoord(data[0], data[1]);
	}

	template<typename T>
	void AddTexcoord(T t0, T t1)
	{
		texcoords.push_back(glm::vec2(t0, t1));
	}
	
	template<typename T>
	void AddColour(T &data)
	{
		AddColour(data[0], data[1], data[2]);
	}

	template<typename T>
	void AddColour(T c0, T c1, T c2)
	{
		colours.push_back(glm::vec3(c0, c1, c2));
	}

	template<typename T>
	void AddFace(T f0, T f1, T f2, T f3)
	{
		AddFace(f0, f1, f2);
		AddFace(f2, f3, f0);
	}

	template<typename T>
	void AddFace(T &data)
	{
		AddFace(data[0], data[1], data[2]);
	}

	template<typename T>
	void AddFace(T f0, T f1, T f2)
	{
		faces.push_back(glm::u16vec3(f0, f1, f2));
	}

	void InitBuffers(void);

	std::vector<glm::vec3>    vertices;
	std::vector<glm::vec3>    colours;
	std::vector<glm::vec3>    normals;
	std::vector<glm::vec2>    texcoords;
	std::vector<glm::u16vec3> faces;

	bool have_init_buffers;
	GLuint vao, vbuffer, nbuffer, tbuffer, fbuffer, cbuffer;
};

class Cube : public Mesh
{
public:
	Cube(glm::vec3 min, glm::vec3 max)
		: Mesh(false)
	{
		wireframe = true;
		AddVertex(min[0], min[1], min[2]);
		AddVertex(min[0], min[1], max[2]);
		AddVertex(min[0], max[1], min[2]);
		AddVertex(min[0], max[1], max[2]);
		AddVertex(max[0], min[1], min[2]);
		AddVertex(max[0], min[1], max[2]);
		AddVertex(max[0], max[1], min[2]);
		AddVertex(max[0], max[1], max[2]);

		AddColour(0.0, 0.0, 0.0);
		AddColour(0.0, 0.0, 1.0);
		AddColour(0.0, 1.0, 0.0);
		AddColour(0.0, 1.0, 1.0);
		AddColour(1.0, 0.0, 0.0);
		AddColour(1.0, 0.0, 1.0);
		AddColour(1.0, 1.0, 0.0);
		AddColour(1.0, 1.0, 1.0);

		AddFace(0, 2, 6, 4);
		AddFace(4, 6, 7, 5);
		AddFace(5, 7, 3, 1);
		AddFace(1, 3, 2, 0);
		AddFace(2, 3, 7, 6);
		AddFace(0, 1, 5, 4);

		InitBuffers();
	}
};