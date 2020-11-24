#pragma once
#include <glm.hpp>

struct Vertex
{
	Vertex(glm::fvec3 position) : position(position) {}

	glm::fvec3 position;
	// glm::fvec3 normal;
	// glm::fvec3 color;
	// glm::fvec2 texcoord;
};

