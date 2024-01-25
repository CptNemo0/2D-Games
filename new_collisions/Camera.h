#pragma once

#include "glm/glm.hpp"

using namespace glm;

class Camera
{
public:

	vec2 position;
	vec2 size;

	Camera(vec2 new_pos, vec2 new_size);
};

