#pragma once

#include <memory>
#include <vector>
#include <glm.hpp>
#include "HeightMap.h"
#include "../Mesh/Mesh.h"

class Triangulator
{
public:
	virtual void run() = 0;

	std::unique_ptr<Mesh> getMesh(QOpenGLContext* context) const {
		return done ?
			std::make_unique<Mesh>(context, points, triangles) :
			std::make_unique<Mesh>(context);
	}

protected:
	Triangulator::Triangulator(const QImage& image) : heightMap(image) {
		points.reserve((long long) heightMap.getHeight() * heightMap.getWidth());
	}

	void mirrorY() {
		auto height = heightMap.getHeight();

		for (auto& point : points) {
			point.y = height - point.y;
		}
	}

protected:
	HeightMap heightMap;
	std::vector<glm::fvec3> points;
	std::vector<Triangle> triangles;

	bool done = false;
};

