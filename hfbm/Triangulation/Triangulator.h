#pragma once

#include <vector>

#include "glm.hpp"
#include "HeightMap.h"
#include "../Mesh/Mesh.h"

class Triangulator
{
public:
	virtual void run() = 0;
	Mesh getMesh() const {
		return done ? Mesh(vertices) : Mesh(std::vector<glm::vec3>());
	}

protected:
	Triangulator::Triangulator(const QImage& image) : heightMap(image) {
		vertices.reserve((long long) heightMap.getHeight() * heightMap.getWidth());
	}

protected:
	HeightMap heightMap;
	std::vector<glm::fvec3> vertices;

	bool done = false;
};

