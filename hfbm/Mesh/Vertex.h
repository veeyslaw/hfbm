#pragma once

#include <glm.hpp>

struct Vertex
{
	Vertex(
		glm::fvec3 position,
		glm::fvec3 normal = glm::fvec3(0, 0, 1),
		glm::fvec3 color = glm::fvec3(0.7)
	) :
		position(position),
		normal(normal),
		color(color) {}

	glm::fvec3 position;
	glm::fvec3 normal;
	glm::fvec3 color;
};

