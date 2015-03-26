#pragma once
#include <GL/glew.h>
#include <stdint.h>

class Texture
{
public:
		Texture(const char *filename);
		Texture();

		void ReadPPM(const char *filename);
		void ReadBMP(const char *filename);

		void InitTexture(void);
		void Bind(void);
		void Unbind(void);

		uint8_t   *data;
		uint32_t   width;
		uint32_t   height;
		uint8_t    bpp;

		/* OpenGL State */
		GLuint texture;
		GLuint sampler;
		GLenum format;
};
