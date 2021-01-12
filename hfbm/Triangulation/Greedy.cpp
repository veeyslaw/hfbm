#include "Delaunay.h"
#include <stdexcept>
#include <iostream>

Greedy::Greedy(std::unique_ptr<HeightMap> heightMap, int meshHeight, double error) : Triangulator(std::move(heightMap), meshHeight, error) {}

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
	
	// add corner points
	for (auto y = 0; y < height; y+=height-1) {
		for (auto x = 0; x < width; x+=width-1) {
			auto z = heightMap->at((long long)y * width + x);
			points.push_back(glm::fvec3(x, y, z));
		}
	}
	// add initial triangles
	triangles.push_back(Triangle(3, 0, 2));
	triangles.push_back(Triangle(3, 1, 0));

	// add unused points
	std::vector<std::pair<glm::fvec3, double>> unusedPoints;
	unusedPoints.reserve((long long)height * width - 4);
	for (auto y = 0; y < height; y++) {
		for (auto x = 0; x < width; x++) {
			if (x == 0 && y == 0 || x == 0 && y == height - 1 || x == width - 1 && y == 0 || x == width - 1 && y == height - 1) {
				continue;
			}
			auto z = heightMap->at((long long)y * width + x);
			auto point = glm::fvec3(x, y, z);
			auto error = getError(point);
			unusedPoints.push_back(std::make_pair(point, error));
		}
	}
	auto errorComp = [](const std::pair<glm::fvec3, double>& l, const std::pair<glm::fvec3, double>& r)
											{ return l.second < r.second; };
	std::make_heap(unusedPoints.begin(), unusedPoints.end(), errorComp);

	while (notReady(unusedPoints)) {
		// insert point with max error in mesh
		std::pop_heap(unusedPoints.begin(), unusedPoints.end(), errorComp);
		auto point = unusedPoints.back().first;
		unusedPoints.pop_back();
		insert(point);

		// update errors
		for (auto& point: unusedPoints) {
			point.second = getError(point.first);
		}
		std::make_heap(unusedPoints.begin(), unusedPoints.end(), errorComp);
	}

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

bool Greedy::notReady(const std::vector<std::pair<glm::fvec3, double>>& unusedPoints) {
	if (unusedPoints.size() == 0) {
		return false;
	}
	if (unusedPoints.front().second <= error) {
		return false;
	}
	return true;
}

bool areCollinear(glm::fvec3 p1, glm::fvec3 p2, glm::fvec3 p3) {
	return p1.x * (p2.y - p3.y) + p2.x * (p3.y - p1.y) + p3.x * (p1.y - p2.y) == 0;
}

bool isOuterEdge(glm::fvec3 p1, glm::fvec3 p2, int w, int h) {
	return (p1.x == 0 && p2.x == 0 || p1.x == w - 1 && p2.x == w - 1 ||
		p1.y == 0 && p2.y == 0 || p1.y == h - 1 && p2.y == h - 1);
}

void Greedy::insert(glm::fvec3 point) {
	auto newPointIndex = points.size();
	points.push_back(point);
	auto p = points.at(newPointIndex);
	auto triangleIndex = locate(point);
	auto& triangle = triangles.at(triangleIndex);
	auto t1 = triangle.points[0];
	auto p1 = points.at(t1);
	auto t2 = triangle.points[1];
	auto p2 = points.at(t2);
	auto t3 = triangle.points[2];
	auto p3 = points.at(t3);
	
	// split in 3 triangles
	auto height = heightMap->getHeight();
	auto width = heightMap->getWidth();

	std::swap(triangles.at(triangleIndex), triangles.at(triangles.size() - 1));
	triangles.pop_back();

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
	/*
	std::vector<Suspect> suspects;
	// add outer edges as suspects
	suspects.push_back(Suspect(triangleIndex, t1, t2, newPointIndex));
	suspects.push_back(Suspect(secondTriangle, t2, t3, newPointIndex));
	suspects.push_back(Suspect(thirdTriangle, t3, t1, newPointIndex));
	// check edges and flip if necessary, then check recursively outer edges
	while (!suspects.empty()) {
		auto suspect = suspects.back();
		suspects.pop_back();
		throw away if on edge
		auto oppositeSuspect = locate(suspect);
		auto P = points.at(suspect.point);
		auto A = points.at(suspect.edge[0]);
		auto B = points.at(suspect.edge[1]);
		auto C = points.at(oppositeSuspect.second);
		// center formula: https://en.wikipedia.org/wiki/Circumscribed_circle
		B -= A;
		C -= A;
		auto factor = 2 * (B.x * C.y - B.y * C.x);
		auto centerX = (C.y * (B.x * B.x + B.y * B.y) - B.y * (C.x * C.x + C.y * C.y)) / factor + A.x;
		auto centerY = (B.x * (C.x * C.x + C.y * C.y) - C.x * (B.x * B.x + B.y * B.y)) / factor + A.y;

		auto radiusSq = centerX * centerX + centerY * centerY;

		auto xDistSq = centerX - P.x;
		xDistSq *= xDistSq;
		auto yDistSq = centerY - P.y;
		yDistSq *= yDistSq;
		if (xDistSq + yDistSq <= radiusSq) {
			// confirmed suspect
			for (auto& point : triangles.at(suspect.triangle).points) {
				if (point == suspect.point) {
					point = wrong;
				}
			}
		}
	}
	*/
}

int Greedy::locate(glm::fvec3 point) {
	for (auto i = 0; i < triangles.size(); i++) {
		if (pointInTriangle(point, i)) {
			return i;
			break;
		}
	}

	return -1;
}

std::pair<int, int> Greedy::locate(Suspect suspect) {
	// find triangle that shares the edge
	for (auto i = 0; i < triangles.size(); i++) {
		const auto& triangle = triangles.at(i);
		auto p1 = triangle.points[0];
		auto p2 = triangle.points[1];
		auto p3 = triangle.points[2];
		if (suspect.point != p1 && suspect.point != p2 && suspect.point != p3 &&
			(suspect.edge[0] == p1 || suspect.edge[0] == p2 || suspect.edge[0] == p3) &&
			(suspect.edge[1] == p1 || suspect.edge[1] == p2 || suspect.edge[1] == p3)
		) {
			auto oppositePoint = p1 != suspect.edge[0] && p1 != suspect.edge[1] ? p1 :
				p2 != suspect.edge[0] && p2 != suspect.edge[1] ? p2 :
				p3;
			return std::make_pair(i, oppositePoint);
			break;
		}
	}
	throw "triangle location went wrong";
}

double Greedy::getError(glm::fvec3 point) {
	try {
		auto interpolation = std::abs(heightMap->at(point.y * heightMap->getWidth() + point.x) - interpolate(point)) / meshHeight;
		auto wtf = heightMap->at(point.y * heightMap->getWidth() + point.x);
		auto wtf2 = interpolate(point);
		return interpolation;
	}
	catch(std::out_of_range oob) {
		return 0;
	}
}

double Greedy::interpolate(glm::fvec3 point) {
	auto triangleIndex = locate(point);
	const auto& triangle = triangles.at(triangleIndex);
	const auto& A = points.at(triangle.points[0]);
	const auto& B = points.at(triangle.points[1]);
	const auto& C = points.at(triangle.points[2]);
	// find normal vector to the plane
	auto AB = B - A;
	auto AC = C - A;
	auto n = glm::cross(AB, AC);
	// n = (a, b, c)
	// A = (x0, y0, z0)
	// point = (x, y, z)
	// plane equation given A and n: a*(x-x0) + b*(y-y0) + c*(z-z0) = 0
	// interpolation = z = ...
	return (double)n.x * (A.x - point.x) / n.z + (double)n.y * (A.y - point.y) / n.z + A.z;
}

bool Greedy::pointInTriangle(glm::fvec3 point, int triangleIndex)
{
	// adapted version of: https://github.com/SebLague/Gamedev-Maths/blob/master/PointInTriangle.cs
	const auto& triangle = triangles.at(triangleIndex);
	const auto& A = points.at(triangle.points[0]);
	const auto& B = points.at(triangle.points[1]);
	const auto& C = points.at(triangle.points[2]);
	
	double s1 = C.y - A.y;
	double s2 = C.x - A.x;
	double s3 = B.y - A.y;
	double s4 = point.y - A.y;
	double w1, w2;
	if (s1 != 0) {
		w1 = (A.x * s1 + s4 * s2 - point.x * s1) / (s3 * s2 - (B.x - A.x) * s1);
		w2 = (s4 - w1 * s3) / s1;
	}
	else {
		w1 = s4 / s3;
		w2 = (point.x - A.x - w1 * (B.x - A.x)) / (C.x - A.x);
	}
	return w1 >= 0 && w2 >= 0 && (w1 + w2) <= 1;
}
