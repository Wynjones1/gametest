#pragma once
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "texture.h"
#include "shaders.h"
#include <stdio.h>

static const uint32_t VPOSITION_LOCATION = 0;
static const uint32_t VCOLOUR_LOCATION   = 1;
static const uint32_t VNORMAL_LOCATION   = 2;
static const uint32_t VTEXCOORD_LOCATION = 3;

class Drawable
{
public:
	virtual void Draw(Program &program) = 0;
};