#include "Delaunay.h"
#include "GreedyHelpers.h"
#include <stdexcept>
#include <iostream>

Delaunay::Delaunay(std::unique_ptr<HeightMap> heightMap, double meshHeight, double error) : 
	Greedy(std::move(heightMap), meshHeight, error)
{}

void Delaunay::insert(glm::ivec2 point, int triangleIndex) {
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
	
	std::vector<std::pair<int, int>> suspects;
	suspects.reserve(16);
	auto newTriangle = triangles.size();
	if (!(ABPcol || BCPcol || CAPcol)) {
		// no collinears
		auto newTriangle2 = newTriangle + 1;
		addTriangle(Triangle(newPointIndex, Aind, Bind), triangleIndex, newTriangle2, abn, newTriangle);
		suspects.push_back(std::make_pair(triangleIndex, 1));
		addTriangle(Triangle(newPointIndex, Bind, Cind), -1, triangleIndex, bcn, newTriangle2);
		suspects.push_back(std::make_pair(newTriangle, 1));
		updateNeighbor(bcn, std::make_pair(Bind, Cind), newTriangle);
		addTriangle(Triangle(newPointIndex, Cind, Aind), -1, newTriangle, can, triangleIndex);
		suspects.push_back(std::make_pair(newTriangle2, 1));
		updateNeighbor(can, std::make_pair(Cind, Aind), newTriangle2);
	}
	else {
		// one collinear
		if (ABPcol) {
			auto neighbor = abn;
			long long newTriangle2 = neighbor > -1 ? newTriangle + 1 : -1;
			addTriangle(Triangle(newPointIndex, Bind, Cind), triangleIndex, newTriangle2, bcn, newTriangle);
			suspects.push_back(std::make_pair(triangleIndex, 1));
			addTriangle(Triangle(newPointIndex, Cind, Aind), -1, triangleIndex, can, neighbor);
			suspects.push_back(std::make_pair(newTriangle, 1));
			updateNeighbor(can, std::make_pair(Cind, Aind), newTriangle);
			if (neighbor > -1) {
				auto DOffset = getUncommonPointOffset(neighbor, std::make_pair(Aind, Bind));
				auto Dind = triangles.at(neighbor).points[DOffset];
				auto ADNeighbor = neighbors.at(3 * neighbor + (DOffset + 2) % 3);
				auto DBNeighbor = neighbors.at(3 * neighbor + DOffset);
				addTriangle(Triangle(newPointIndex, Aind, Dind), neighbor, newTriangle, ADNeighbor, newTriangle2);
				suspects.push_back(std::make_pair(neighbor, 1));
				addTriangle(Triangle(newPointIndex, Dind, Bind), -1, neighbor, DBNeighbor, triangleIndex);
				suspects.push_back(std::make_pair(newTriangle2, 1));
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
			suspects.push_back(std::make_pair(triangleIndex, 1));
			addTriangle(Triangle(newPointIndex, Aind, Bind), -1, triangleIndex, abn, neighbor);
			suspects.push_back(std::make_pair(newTriangle, 1));
			updateNeighbor(abn, std::make_pair(Aind, Bind), newTriangle);
			if (neighbor > -1) {
				auto DOffset = getUncommonPointOffset(neighbor, std::make_pair(Bind, Cind));
				auto Dind = triangles.at(neighbor).points[DOffset];
				auto BDNeighbor = neighbors.at(3 * neighbor + (DOffset + 2) % 3);
				auto DCNeighbor = neighbors.at(3 * neighbor + DOffset);
				addTriangle(Triangle(newPointIndex, Bind, Dind), neighbor, newTriangle, BDNeighbor, newTriangle2);
				suspects.push_back(std::make_pair(neighbor, 1));
				addTriangle(Triangle(newPointIndex, Dind, Cind), -1, neighbor, DCNeighbor, triangleIndex);
				suspects.push_back(std::make_pair(newTriangle2, 1));
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
			suspects.push_back(std::make_pair(triangleIndex, 1));
			addTriangle(Triangle(newPointIndex, Bind, Cind), -1, triangleIndex, bcn, neighbor);
			suspects.push_back(std::make_pair(newTriangle, 1));
			updateNeighbor(bcn, std::make_pair(Bind, Cind), newTriangle);
			if (neighbor > -1) {
				auto DOffset = getUncommonPointOffset(neighbor, std::make_pair(Cind, Aind));
				auto Dind = triangles.at(neighbor).points[DOffset];
				auto CDNeighbor = neighbors.at(3 * neighbor + (DOffset + 2) % 3);
				auto DANeighbor = neighbors.at(3 * neighbor + DOffset);
				addTriangle(Triangle(newPointIndex, Cind, Dind), neighbor, newTriangle, CDNeighbor, newTriangle2);
				suspects.push_back(std::make_pair(neighbor, 1));
				addTriangle(Triangle(newPointIndex, Dind, Aind), -1, neighbor, DANeighbor, triangleIndex);
				suspects.push_back(std::make_pair(newTriangle2, 1));
				auto DAEdge = std::make_pair(Dind, Aind);
				auto DAEdgeOffset = getEdgeOffset(triangles.at(newTriangle2), DAEdge);
				auto farNeighbor = neighbors.at(3 * newTriangle2 + DAEdgeOffset);
				updateNeighbor(farNeighbor, DAEdge, newTriangle2);
			}
		}
	}

	auto handleSuspect = [&](std::pair<int, int> s) {
		auto opposite = neighbors.at(3 * s.first + s.second);
		if (opposite < 0) {
			return;
		}
		auto commonEdge = neighbors.at(3 * opposite) == s.first ? 0 :
			neighbors.at(3 * opposite + 1) == s.first ? 1 : 2;

		const auto& oppositeTriangle = triangles.at(opposite);
		const auto Aind = oppositeTriangle.points.at((commonEdge + 1) % 3);
		auto A = points.at(Aind);
		const auto Bind = oppositeTriangle.points.at(commonEdge);
		auto B = points.at(Bind);
		const auto Qind = oppositeTriangle.points.at((commonEdge + 2) % 3);
		auto Q = points.at(Qind);
		auto P = p;

		B -= A;
		Q -= A;
		P -= A;
		auto factor = 2 * (B.x * Q.y - B.y * Q.x);
		auto centerX = (Q.y * (B.x * B.x + B.y * B.y) - B.y * (Q.x * Q.x + Q.y * Q.y)) / factor;
		auto centerY = (B.x * (Q.x * Q.x + Q.y * Q.y) - Q.x * (B.x * B.x + B.y * B.y)) / factor;

		auto radiusSq = centerX * centerX + centerY * centerY;

		auto xDistSq = centerX - P.x;
		xDistSq *= xDistSq;
		auto yDistSq = centerY - P.y;
		yDistSq *= yDistSq;
		
		if (xDistSq + yDistSq <= radiusSq) {
			const auto pan = neighbors.at(3 * s.first + (s.second + 2) % 3);
			const auto bpn = neighbors.at(3 * s.first + (s.second + 1) % 3);
			const auto aqn = neighbors.at(3 * opposite + (commonEdge + 1) % 3);
			const auto qbn = neighbors.at(3 * opposite + (commonEdge + 2) % 3);

			addTriangle(Triangle(newPointIndex, Aind, Qind), s.first, pan, aqn, opposite);
			suspects.push_back(std::make_pair(s.first, 1));
			updateNeighbor(aqn, std::make_pair(Aind, Qind), s.first);
			addTriangle(Triangle(newPointIndex, Qind, Bind), opposite, s.first, qbn, bpn);
			suspects.push_back(std::make_pair(opposite, 1));
			updateNeighbor(bpn, std::make_pair(Bind, newPointIndex), opposite);
		}
	};

	while (suspects.size()) {
		auto suspect = suspects.back();
		suspects.pop_back();
		handleSuspect(suspect);
	}
}