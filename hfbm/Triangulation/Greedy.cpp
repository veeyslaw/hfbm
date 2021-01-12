#include "Greedy.h"
#include <stdexcept>
#include <iostream>

Greedy::Greedy(std::unique_ptr<HeightMap> heightMap, int meshHeight, double error) : Triangulator(std::move(heightMap), meshHeight, error),
	used(heightMap->getWidth() * heightMap->getHeight(), false)
{}

void Greedy::run() {
	if (done) { return; }

	auto height = heightMap->getHeight();
	auto width = heightMap->getWidth();

	if (height <= 0 || width <= 0) {
		done = true;
		return;
	}
	points.reserve((long long) height * width);
	triangles.reserve((long long)2 * (height - 1) * (width - 1));
	maxErrors.reserve((long long)2 * (height - 1) * (width - 1));
	neighbors.reserve((long long)6 * (height - 1) * (width - 1));
	
	// add corner points
	for (auto y = 0; y < height; y+=height-1) {
		for (auto x = 0; x < width; x+=width-1) {
			auto z = heightMap->at((long long)y * width + x);
			points.push_back(glm::fvec3(x, y, z));
			used.at(y * width + x) = true;
		}
	}
	// add initial triangles
	addTriangle(Triangle(3, 0, 2), -1, 1, -1, -1);
	addTriangle(Triangle(3, 1, 0), -1, -1, -1, 0);

	for (auto triangle : triangles) {
		auto maxError = getMaxError(triangle);
		maxErrors.push_back(maxError);
	}

	while (true) {
		// insert point with max error in mesh
		auto max = -1.;
		glm::ivec2 pointCoord(-1, -1);
		auto triangleInd = -1;
		for (auto i = 0; i < maxErrors.size(); i++) {
			auto errorPair = maxErrors.at(i);
			if (max < errorPair.first) {
				max = errorPair.first;
				pointCoord = errorPair.second;
				triangleInd = i;
			}
		}

		if (max <= error) {
			break;
		}

		toUpdate.reserve(triangles.size());
		insert(pointCoord, triangleInd);

		// update errors
		for (auto triangleInd: toUpdate) {
			auto maxError = getMaxError(triangles.at(triangleInd));
			maxErrors.at(triangleInd) = maxError;
		}
		toUpdate.clear();
	}

	neighbors.clear();
	maxErrors.clear();
	triangles.shrink_to_fit();
	points.shrink_to_fit();

	mirrorY();

	auto scaleDown = std::max(width, height) / 2;
	for (auto& point : points) {
		point.x -= width / 2;
		point.y -= height / 2;
		point.x /= scaleDown;
		point.y /= scaleDown;
	}

	done = true;
}

void Greedy::insert(glm::ivec2 point, int triangleIndex) {
	const auto height = heightMap->getHeight();
	const auto width = heightMap->getWidth();
	const auto newPointIndex = points.size();
	const auto h = heightMap->at(point.y * width + point.x);
	points.push_back(glm::fvec3(point.x, point.y, h));
	const auto p = points.at(newPointIndex);
  auto& triangle = triangles.at(triangleIndex);
	const auto Aind = triangle.points[0];
	const auto A = points.at(Aind);
	const auto Bind = triangle.points[1];
	const auto B = points.at(Bind);
	const auto Cind = triangle.points[2];
	const auto C = points.at(Cind);
	const auto abn = neighbors.at(triangleIndex);
	const auto bcn = neighbors.at(triangleIndex + 1);
	const auto can = neighbors.at(triangleIndex + 2);
	
	const auto areCollinear = [](glm::fvec3 p1, glm::fvec3 p2, glm::fvec3 p3) {
		return p1.x * (p2.y - p3.y) + p2.x * (p3.y - p1.y) + p3.x * (p1.y - p2.y) == 0;
	};

	auto replacedOriginal = false;
	// check AB
	if (!areCollinear(A, B, p)) {
		addTriangle(Triangle(newPointIndex, Aind, Bind), triangleIndex, abn, -1, -1);
	}
	else {

	}
	// check BC

	// check CA

	// split in 3 triangles
	if (!areCollinear(p1, p2, p)) {
		triangles.push_back(Triangle(newPointIndex, t1, t2));
	}
	else {
		if (!isOuterEdge(p1, p2, width, height)) {
			auto opposite = locate(Suspect(triangleIndex, t1, t2, newPointIndex));
			std::swap(triangles.at(opposite.first), triangles.at(triangles.size() - 1));
			triangles.pop_back();
			triangles.push_back(Triangle(newPointIndex, t1, opposite.second));
			triangles.push_back(Triangle(newPointIndex, opposite.second, t2));
		}
	}
	if (!areCollinear(p2, p3, p)) {
		// auto secondTriangle = triangles.size();
		triangles.push_back(Triangle(newPointIndex, t2, t3));
	}
	else {
		if (!isOuterEdge(p2, p3, width, height)) {
			auto opposite = locate(Suspect(triangleIndex, t2, t3, newPointIndex));
			std::swap(triangles.at(opposite.first), triangles.at(triangles.size() - 1));
			triangles.pop_back();
			triangles.push_back(Triangle(newPointIndex, t2, opposite.second));
			triangles.push_back(Triangle(newPointIndex, opposite.second, t3));
		}
	}
	if (!areCollinear(p3, p1, p)) {
		// auto thirdTriangle = triangles.size();
		triangles.push_back(Triangle(newPointIndex, t3, t1));
	}
	else {
		if (!isOuterEdge(p3, p1, width, height)) {
			auto opposite = locate(Suspect(triangleIndex, t3, t1, newPointIndex));
			std::swap(triangles.at(opposite.first), triangles.at(triangles.size() - 1));
			triangles.pop_back();
			triangles.push_back(Triangle(newPointIndex, t3, opposite.second));
			triangles.push_back(Triangle(newPointIndex, opposite.second, t1));
		}
	}
}

std::pair<double, glm::ivec2> Greedy::getMaxError(Triangle t) {
	// precalculations
	const auto& A = points.at(t.points[0]);
	const auto& B = points.at(t.points[1]);
	const auto& C = points.at(t.points[2]);

	// for pointInTriangle
	const double s1 = C.y - A.y;
	const double s2 = C.x - A.x;
	const double s3 = B.y - A.y;
	const double s5 = A.x * s1;
	const double s6 = s3 * s2;
	const double s7 = B.x - A.x;

	const auto pointInTriangle = [=](glm::ivec2 point)
	{
		double s4 = point.y - A.y;
		double w1, w2;
		if (s1 != 0) {
			w1 = (s5 + s4 * s2 - point.x * s1) / (s6 - s7 * s1);
			w2 = (s4 - w1 * s3) / s1;
		}
		else {
			w1 = s4 / s3;
			w2 = (point.x - A.x - w1 * s7) / s2;
		}
		return w1 >= 0 && w2 >= 0 && (w1 + w2) <= 1;
	};

	// for interpolation
	// find normal vector to the plane
	auto AB = B - A;
	auto AC = C - A;
	auto n = glm::cross(AB, AC);
	// n = (a, b, c)
	// A = (x0, y0, z0)
	// point = (x, y, z)
	// plane equation given A and n: a*(x-x0) + b*(y-y0) + c*(z-z0) = 0
	// interpolation = z = ...

	const auto interpolate = [=](glm::ivec2 point) {
		return (double)n.x * (A.x - point.x) / n.z + (double)n.y * (A.y - point.y) / n.z + A.z;
	};

	const glm::fvec3 minf = glm::min(glm::min(A, B), C);
	const glm::ivec2 min(minf.x, minf.y);
	const glm::fvec3 maxf = glm::max(glm::max(A, B), C);
	const glm::ivec2 max(maxf.x, maxf.y);

	auto maxError = -1.;
	glm::ivec2 maxCoord(-1, -1);
	for (auto y = min.y; y <= max.y; y++) {
		for (auto x = min.x; x <= max.x; x++) {
			glm::ivec2 point(x, y);
			if (!pointInTriangle(point)) {
				continue;
			}

			auto h = heightMap->at(y * heightMap->getWidth() + x);
			auto error = -1.;
			try {
				auto interpolation = interpolate(point);
				error = (h - interpolation) / meshHeight;
			}
			catch (std::out_of_range oob) {
				// data related errors
			}
			if (error > maxError) {
				maxError = error;
				maxCoord = point;
			}
		}
	}

	return std::make_pair(maxError, maxCoord);
}

void Greedy::addTriangle(Triangle t, int ind, int abn, int bcn, int can) {
	if (ind < 0) {
		ind = triangles.size();
		triangles.push_back(t);
		neighbors.push_back(abn);
		neighbors.push_back(bcn);
		neighbors.push_back(can);
	}
	else {
		std::swap(triangles.at(ind), t);
		neighbors.at(ind) = abn;
		neighbors.at(ind + 1) = bcn;
		neighbors.at(ind + 2) = can;
	}
	toUpdate.push_back(ind);
}