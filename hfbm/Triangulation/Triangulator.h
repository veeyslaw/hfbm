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
		return done ? Mesh(points, triangleIndices) :
			Mesh(std::vector<glm::fvec3>(), std::vector<TriangleIndices>());
	}

protected:
	Triangulator::Triangulator(const QImage& image) : heightMap(image) {
		points.reserve((long long) heightMap.getHeight() * heightMap.getWidth());
	}

	void mirrorY() {
		auto width = heightMap.getWidth();

		for (auto& point : points) {
			point[1] = width - point[1];
		}
	}

protected:
	HeightMap heightMap;
	std::vector<glm::fvec3> points;
	std::vector<TriangleIndices> triangleIndices;

	bool done = false;
};

