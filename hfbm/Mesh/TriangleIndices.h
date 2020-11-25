#pragma once
#include <glm.hpp>
#include <cstdint>
#include <array>

struct TriangleIndices
{
	TriangleIndices(const std::array<std::int64_t, 3>& points) : points(points) {}
	TriangleIndices(std::int64_t point1, std::int64_t point2, std::int64_t point3) :
		points({point1, point2, point3}) {}

	std::array<std::int64_t, 3> points;
};

