#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>

class Shader
{
public:
	Shader(GLenum type, const char *filename);
	GLuint shader_id;
	GLenum type;
};

class Program
{
public:
	Program();
	Program(const char *vertex_shader, const char *fragment_shader);

	void   AddShader(const Shader &shader);
	void   Compile();
	void   Use();

	void SetModel(glm::mat4 mat, bool transpose = false);
	void SetView(glm::mat4 mat, bool transpose = false);
	void SetProjection(glm::mat4 mat, bool transpose = false);


	GLuint program_id;
	GLuint model, view, projection, has_texture;
};
