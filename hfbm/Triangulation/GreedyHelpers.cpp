#include "GreedyHelpers.h"

int getEdgeOffset(const Triangle& triangle, std::pair<int, int> edge) {
	return (edge.first == triangle.points[0] || edge.first == triangle.points[1]) && (edge.second == triangle.points[0] || edge.second == triangle.points[1]) ? 0 :
		(edge.first == triangle.points[1] || edge.first == triangle.points[2]) && (edge.second == triangle.points[1] || edge.second == triangle.points[2]) ? 1 : 2;
}

std::int64_t doubleArea(glm::ivec2 p1, glm::ivec2 p2, glm::ivec2 p3) {
	return abs(p1.x * (p2.y - p3.y) + p2.x * (p3.y - p1.y) + p3.x * (p1.y - p2.y));
}