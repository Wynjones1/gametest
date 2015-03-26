#include "mesh.h"
#include <string.h>
#include <iostream>
#include <errno.h>
#include <limits>
#include <glm/gtc/matrix_transform.hpp>

/* Read in the mesh from a ply file */
void Mesh::ReadHeader(FILE *fp, int &num_vertices, int &num_faces, bool &have_normals, bool &have_tex)
{
	char buf[1024];
	while(fgets(buf, 1024, fp))
	{
		sscanf(buf, "element vertex %d", &num_vertices);
		sscanf(buf, "element face %d", &num_faces);
		if(strncmp("property float nx", buf, strlen("property float nx")) == 0)
		{
			have_normals = true;
		}

		if(strncmp("property float s", buf, strlen("property float s")) == 0)
		{
			have_tex    = true;
		}

		if(strncmp("end_header", buf, strlen("end_header")) == 0)
		{
			return;
		}
	}
}

void Mesh::ReadVertexData(FILE *fp, int num, bool have_normals, bool have_tex)
{
	double n[3];
	double v[3];
	double t[2];
	char buf[1024];
	for(int i = 0; i < num; i++)
	{
		fgets(buf, 1024, fp);
		if(have_normals && have_tex)
		{
			sscanf(buf, "%lf %lf %lf %lf %lf %lf %lf %lf", v, v+2, v+1, n, n+1, n+2, t, t+1);
			v[2] = -v[2];
			t[1] = 1 - t[1];
			AddVertex(v);
			AddNormal(n);
			AddTexcoord(t);
		}
		else if(have_normals)
		{
			sscanf(buf, "%lf %lf %lf %lf %lf %lf", v, v+2, v+1, n, n+1, n+2);
			v[2] = -v[2];
			AddVertex(v);
			AddNormal(n);
		}
		else if(have_tex)
		{
			sscanf(buf, "%lf %lf %lf %lf %lf", v, v+2, v+1, t, t+1);
			v[2] = -v[2];
			t[1] = 1 - t[1];
			AddVertex(v);
			AddTexcoord(t);
		}
		else
		{
			sscanf(buf, "%lf %lf %lf", v, v+2, v + 1);
			v[2] = -v[2];
			AddVertex(v);
		}
	}
}

void Mesh::ReadFaceData(FILE *fp, int num)
{
	int f[4];
	char buf[1024];
	for(int i = 0; i < num; i++)
	{
		fgets(buf, 1024, fp);
		if(sscanf(buf, "%*d %d %d %d %d", f, f+1, f+2, f+3) == 4)
		{
			AddFace(f[0], f[1], f[2]);
			AddFace(f[2], f[3], f[0]);
		}
		else if(sscanf(buf, "%*d %d %d %d" , f, f+1, f+2) == 3)
		{
			AddFace(f[0], f[1], f[2]);
		}
	}
}

Mesh::Mesh(const char *filename, const char *texturename)
	: texture(NULL)
	, wireframe(false)
	, vbuffer(-1)
	, nbuffer(-1)
	, tbuffer(-1)
	, fbuffer(-1)
	, have_init_buffers(false)
{
	FILE *fp = fopen(filename, "r");
	if(!fp)
	{
		std::cout << "Could not open file: " << filename << " :" << strerror(errno) << std::endl;
		exit(-1);
	}

	int  num_vertices, num_faces;
	bool have_normals = false;
	bool have_tex = false;
	//Read the header
	ReadHeader(fp, num_vertices, num_faces, have_normals, have_tex);
	ReadVertexData(fp, num_vertices, have_normals, have_tex);
	ReadFaceData(fp, num_faces);

	InitBuffers();

	if(texturename)
	{
		texture = new Texture(texturename);
	}
}

Mesh::Mesh(bool init)
	: texture(NULL)
	, wireframe(false)
	, vbuffer(-1)
	, nbuffer(-1)
	, tbuffer(-1)
	, fbuffer(-1)
{
	if(init)
	{
		AddVertex(-0.5,-0.5, 0.0);
		AddVertex(-0.5, 0.5, 0.0);
		AddVertex( 0.5, 0.5, 0.0);
		AddVertex( 0.5,-0.5, 0.0);

		AddTexcoord(0, 1);
		AddTexcoord(0, 0);
		AddTexcoord(1, 0);
		AddTexcoord(1, 1);

		AddColour(1.0, 0.0, 0.0);
		AddColour(0.0, 1.0, 0.0);
		AddColour(0.0, 0.0, 1.0);
		AddColour(1.0, 1.0, 1.0);

		AddNormal(0.0, 0.0, 1.0);
		AddNormal(0.0, 0.0, 1.0);
		AddNormal(0.0, 0.0, 1.0);
		AddNormal(0.0, 0.0, 1.0);

		AddFace(0, 1, 2);
		AddFace(2, 3, 0);
		InitBuffers();
	}
}

void Mesh::InitBuffers(void)
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), (float*)vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(VPOSITION_LOCATION);

	if(colours.size() == vertices.size())
	{
		glGenBuffers(1, &cbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, cbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(colours[0]) * colours.size(), colours.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(VCOLOUR_LOCATION);
	}

	if(normals.size() == vertices.size())
	{
		glGenBuffers(1, &nbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, nbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(normals[0]) * normals.size(), normals.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(VNORMAL_LOCATION);
	}

	if(texcoords.size() == vertices.size())
	{
		glGenBuffers(1, &tbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, tbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(texcoords[0]) * texcoords.size(), texcoords.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(VTEXCOORD_LOCATION);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &fbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faces[0]) * faces.size(), faces.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);

	have_init_buffers = true;
}

void Mesh::Draw(Program &program)
{
	if(wireframe)
	{
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	}
	if(texture)
	{
		texture->Bind();
		glUniform1i(program.has_texture, 1);
	}
	else
	{
		glUniform1i(program.has_texture, 0);
		glDisableVertexAttribArray(0);
		glVertexAttrib3fv(VCOLOUR_LOCATION, &colour[0]);
	}
	CalculateModelMatrix();
	glUniformMatrix4fv(program.model, 1, GL_FALSE, &model_matrix[0][0]);
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(faces.size() * 3), GL_UNSIGNED_SHORT, NULL);
	glBindVertexArray(0);
	if(texture)texture->Unbind();
	if(wireframe)
	{
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	}
}

