#pragma once
#include <glm.hpp>

struct Vertex
{
	Vertex(glm::fvec3 position,
		glm::fvec3 normal = glm::fvec3(0, 0, 1),
		glm::fvec3 color = glm::fvec3(0.5),
		glm::fvec2 texcoord = glm::fvec2(0, 0)) :

		position(position),
		normal(normal),
		color(color),
		texcoord(texcoord) {}

	glm::fvec3 position;
	glm::fvec3 normal;
	glm::fvec3 color;
	glm::fvec2 texcoord;
};

