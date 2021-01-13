#include "Greedy.h"
#include "GreedyHelpers.h"
#include <stdexcept>
#include <iostream>

Greedy::Greedy(std::unique_ptr<HeightMap> heightMap, int meshHeight, double error) : Triangulator(std::move(heightMap), meshHeight, error),
	used(this->heightMap->getWidth() * this->heightMap->getHeight(), false)
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

	for (auto i = 0; i < 2; i++) {
		auto maxError = getMaxError(triangles.at(i));
		maxErrors.at(i) = maxError;
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

		toUpdate.clear();
		toUpdate.reserve(triangles.size());
		insert(pointCoord, triangleInd);

		// update errors
		for (auto triangleInd: toUpdate) {
			auto maxError = getMaxError(triangles.at(triangleInd));
			maxErrors.at(triangleInd) = maxError;
		}
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

void Greedy::updateNeighbor(int tind, std::pair<int, int> edge, int newNeighbor) {
	if (tind < 0) {
		return;
	}
	const auto& triangle = triangles.at(tind);
	neighbors.at(3 * tind + getEdgeOffset(triangle, edge)) = newNeighbor;
}

int Greedy::getUncommonPointOffset(int tind, std::pair<int, int> edge) {
	const auto& triangle = triangles.at(tind);
	auto edgeOff = getEdgeOffset(triangle, edge);
	return (edgeOff + 2) % 3;
}

void Greedy::insert(glm::ivec2 point, int triangleIndex) {
	const auto height = heightMap->getHeight();
	const auto width = heightMap->getWidth();
	const auto newPointIndex = points.size();
	const auto h = heightMap->at(point.y * width + point.x);
	points.push_back(glm::fvec3(point.x, point.y, h));
	used.at(point.y * width + point.x) = true;
	const auto p = points.at(newPointIndex);
  auto& triangle = triangles.at(triangleIndex);
	const auto Aind = triangle.points[0];
	const auto A = points.at(Aind);
	const auto Bind = triangle.points[1];
	const auto B = points.at(Bind);
	const auto Cind = triangle.points[2];
	const auto C = points.at(Cind);
	const auto abn = neighbors.at(3 * triangleIndex);
	const auto bcn = neighbors.at(3 * triangleIndex + 1);
	const auto can = neighbors.at(3 * triangleIndex + 2);

	const auto areCollinear = [](glm::fvec3 p1, glm::fvec3 p2, glm::fvec3 p3) {
		return p1.x * (p2.y - p3.y) + p2.x * (p3.y - p1.y) + p3.x * (p1.y - p2.y) == 0;
	};

	auto ABPcol = areCollinear(A, B, p);
	auto BCPcol = areCollinear(B, C, p);
	auto CAPcol = areCollinear(C, A, p);

	auto newTriangle = triangles.size();
	if (!(ABPcol || BCPcol || CAPcol)) {
		// no collinears
		auto newTriangle2 = newTriangle + 1;
		addTriangle(Triangle(newPointIndex, Aind, Bind), triangleIndex, newTriangle2, abn, newTriangle);
		addTriangle(Triangle(newPointIndex, Bind, Cind), -1, triangleIndex, bcn, newTriangle2);
		updateNeighbor(bcn, std::make_pair(Bind, Cind), newTriangle);
		addTriangle(Triangle(newPointIndex, Cind, Aind), -1, newTriangle, can, triangleIndex);
		updateNeighbor(can, std::make_pair(Cind, Aind), newTriangle2);
	}
	else {
		// one collinear
		if (ABPcol) {
			auto neighbor = abn;
			long long newTriangle2 = neighbor > -1 ? newTriangle + 1 : -1;
			addTriangle(Triangle(newPointIndex, Bind, Cind), triangleIndex, newTriangle2, bcn, newTriangle);
			addTriangle(Triangle(newPointIndex, Cind, Aind), -1, triangleIndex, can, neighbor);
			updateNeighbor(can, std::make_pair(Cind, Aind), newTriangle);
			if (neighbor > -1) {
				auto DOffset = getUncommonPointOffset(neighbor, std::make_pair(Aind, Bind));
				auto Dind = triangles.at(neighbor).points[DOffset];
				auto ADNeighbor = neighbors.at(3 * neighbor + (DOffset + 2) % 3);
				auto DBNeighbor = neighbors.at(3 * neighbor + DOffset);
				addTriangle(Triangle(newPointIndex, Aind, Dind), neighbor, newTriangle, ADNeighbor, newTriangle2);
				addTriangle(Triangle(newPointIndex, Dind, Bind), -1, neighbor, DBNeighbor, triangleIndex);
				auto DBEdge = std::make_pair(Dind, Bind);
				auto DBEdgeOffset = getEdgeOffset(triangles.at(newTriangle2), DBEdge);
				auto farNeighbor = neighbors.at(3 * newTriangle2 + DBEdgeOffset);
				updateNeighbor(farNeighbor, DBEdge, newTriangle2);
			}
		}
		else if (BCPcol) {
			auto neighbor = bcn;
			long long newTriangle2 = neighbor > -1 ? newTriangle + 1 : -1;
			addTriangle(Triangle(newPointIndex, Cind, Aind), triangleIndex, newTriangle2, can, newTriangle);
			addTriangle(Triangle(newPointIndex, Aind, Bind), -1, triangleIndex, abn, neighbor);
			updateNeighbor(abn, std::make_pair(Aind, Bind), newTriangle);
			if (neighbor > -1) {
				auto DOffset = getUncommonPointOffset(neighbor, std::make_pair(Bind, Cind));
				auto Dind = triangles.at(neighbor).points[DOffset];
				auto BDNeighbor = neighbors.at(3 * neighbor + (DOffset + 2) % 3);
				auto DCNeighbor = neighbors.at(3 * neighbor + DOffset);
				addTriangle(Triangle(newPointIndex, Bind, Dind), neighbor, newTriangle, BDNeighbor, newTriangle2);
				addTriangle(Triangle(newPointIndex, Dind, Cind), -1, neighbor, DCNeighbor, triangleIndex);
				auto DCEdge = std::make_pair(Dind, Cind);
				auto DCEdgeOffset = getEdgeOffset(triangles.at(newTriangle2), DCEdge);
				auto farNeighbor = neighbors.at(3 * newTriangle2 + DCEdgeOffset);
				updateNeighbor(farNeighbor, DCEdge, newTriangle2);
			}
		}
		else if (CAPcol) {
			auto neighbor = can;
			long long newTriangle2 = neighbor > -1 ? newTriangle + 1 : -1;
			addTriangle(Triangle(newPointIndex, Aind, Bind), triangleIndex, newTriangle2, abn, newTriangle);
			addTriangle(Triangle(newPointIndex, Bind, Cind), -1, triangleIndex, bcn, neighbor);
			updateNeighbor(bcn, std::make_pair(Bind, Cind), newTriangle);
			if (neighbor > -1) {
				auto DOffset = getUncommonPointOffset(neighbor, std::make_pair(Cind, Aind));
				auto Dind = triangles.at(neighbor).points[DOffset];
				auto CDNeighbor = neighbors.at(3 * neighbor + (DOffset + 2) % 3);
				auto DANeighbor = neighbors.at(3 * neighbor + DOffset);
				addTriangle(Triangle(newPointIndex, Cind, Dind), neighbor, newTriangle, CDNeighbor, newTriangle2);
				addTriangle(Triangle(newPointIndex, Dind, Aind), -1, neighbor, DANeighbor, triangleIndex);
				auto DAEdge = std::make_pair(Dind, Aind);
				auto DAEdgeOffset = getEdgeOffset(triangles.at(newTriangle2), DAEdge);
				auto farNeighbor = neighbors.at(3 * newTriangle2 + DAEdgeOffset);
				updateNeighbor(farNeighbor, DAEdge, newTriangle2);
			}
		}
	}
}

std::pair<double, glm::ivec2> Greedy::getMaxError(Triangle t) {
	// precalculations
	const auto& A = points.at(t.points[0]);
	const auto A2 = glm::ivec2(A.x, A.y);
	const auto& B = points.at(t.points[1]);
	const auto B2 = glm::ivec2(B.x, B.y);
	const auto& C = points.at(t.points[2]);
	const auto C2 = glm::ivec2(C.x, C.y);

	// for pointInTriangle
	const auto totalArea = doubleArea(A2, B2, C2);

	const auto pointInTriangle = [=](glm::ivec2 point)
	{
		const auto area1 = doubleArea(A2, B2, point);
		const auto area2 = doubleArea(B2, C2, point);
		const auto area3 = doubleArea(C2, A2, point);
		return totalArea == area1 + area2 + area3;
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
	auto width = heightMap->getWidth();
	for (auto y = min.y; y <= max.y; y++) {
		for (auto x = min.x; x <= max.x; x++) {
			glm::ivec2 point(x, y);
			if (used.at(y * width + x) || !pointInTriangle(point)) {
				continue;
			}

			auto h = heightMap->at(y * width + x);
			auto interpolation = interpolate(point);
			auto error = std::abs(h - interpolation) / meshHeight;
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
		maxErrors.push_back(std::make_pair(-1, glm::ivec2(-1, -1)));
	}
	else {
		std::swap(triangles.at(ind), t);
		neighbors.at(3 * ind) = abn;
		neighbors.at(3 * ind + 1) = bcn;
		neighbors.at(3 * ind + 2) = can;
	}
	toUpdate.push_back(ind);
}