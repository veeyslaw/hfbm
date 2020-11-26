#pragma once

#include <vector>
#include <memory>

#include "glm.hpp"
#include "HeightMap.h"
#include "../Mesh/Mesh.h"

class Triangulator
{
public:
	virtual void run() = 0;
	//std::shared_ptr<Mesh> getMesh(std::shared_ptr<QOpenGLContext> context) const {
	std::shared_ptr<Mesh> getMesh(QOpenGLContext* context) const {
		return done ? std::make_shared<Mesh>(context, points, triangles) :
			std::make_shared<Mesh>(context);
	}

protected:
	Triangulator::Triangulator(const QImage& image) : heightMap(image) {
		points.reserve((long long) heightMap.getHeight() * heightMap.getWidth());
	}

	void mirrorY() {
		auto height = heightMap.getHeight();

		for (auto& point : points) {
			point[1] = height - point[1];
		}
	}

protected:
	HeightMap heightMap;
	std::vector<glm::fvec3> points;
	std::vector<Triangle> triangles;

	bool done = false;
};

