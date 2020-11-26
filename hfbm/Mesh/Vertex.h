#pragma once

#include <glm.hpp>

struct Vertex
{
	Vertex(
		glm::fvec3 position,
		glm::fvec3 color = glm::fvec3(0.5)
	) :
		position(position),
		color(color) {}

	glm::fvec3 position;
	glm::fvec3 color;
};

