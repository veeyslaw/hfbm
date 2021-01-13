#include "../Mesh/Triangle.h"
#include <glm.hpp>

int getEdgeOffset(const Triangle& triangle, std::pair<int, int> edge);

std::int64_t doubleArea(glm::ivec2 p1, glm::ivec2 p2, glm::ivec2 p3);