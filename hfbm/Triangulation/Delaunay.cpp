#include "Delaunay.h"

Delaunay::Delaunay(const QImage& image, int meshHeight, double error) : Triangulator(image, meshHeight, error) {}

void Delaunay::run() {
	if (done) { return; }

	auto height = heightMap.getHeight();
	auto width = heightMap.getWidth();

	if (height <= 0 || width <= 0) {
		done = true;
		return;
	}
	points.reserve((long long) height * width);
	triangles.reserve((long long)2 * (height - 1) * (width - 1));
	
	// add corner points
	for (auto y = 0; y < height; y+=height-1) {
		for (auto x = 0; x < width; x+=width-1) {
			auto z = scale * heightMap.at((long long)y * width + x);
			points.push_back(glm::fvec3(x, y, z));
		}
	}
	// add initial triangles
	triangles.push_back(Triangle(3, 0, 2));
	triangles.push_back(Triangle(3, 1, 0));

	// add unused points
	std::vector<std::pair<glm::fvec3, double>> unusedPoints;
	points.reserve((long long)height * width - 4);
	for (auto y = 0; y < height; y++) {
		for (auto x = 0; x < width; x++) {
			if (x == 0 && y == 0 || x == 0 && y == height - 1 || x == width - 1 && y == 0 || x == width - 1 && y == height - 1) {
				continue;
			}
			auto z = scale * heightMap.at((long long)y * width + x);
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

bool Delaunay::notReady(const std::vector<std::pair<glm::fvec3, double>>& unusedPoints) {
	if (unusedPoints.size() == 0) {
		return false;
	}
	if (unusedPoints.front().second > error) {
		return false;
	}
	return true;
}

void Delaunay::insert(glm::fvec3 point) {
	points.push_back(point);
	auto triangleIndex = locate(point);
	const auto& triangle = triangles.at(triangleIndex);
	const auto& A = points.at(triangle.points[0]);
	const auto& B = points.at(triangle.points[1]);
	const auto& C = points.at(triangle.points[2]);

	// split in 3
	// check outer edges and flip if necessary, then check recursively those edges
}

int Delaunay::locate(glm::fvec3 point) {
	// find triangle
	int index = -1;

	for (auto i = 0; i < triangles.size(); i++) {
		if (pointInTriangle(point, i)) {
			index = i;
			break;
		}
	}

	return index;
}

double Delaunay::getError(glm::fvec3 point) {
	return std::abs(heightMap.at(point.y * heightMap.getWidth() + point.x) - interpolate(point));
}

double Delaunay::interpolate(glm::fvec3 point) {
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

bool Delaunay::pointInTriangle(glm::fvec3 point, int triangleIndex)
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

	double w1 = (A.x * s1 + s4 * s2 - point.x * s1) / (s3 * s2 - (B.x - A.x) * s1);
	double w2 = (s4 - w1 * s3) / s1;
	return w1 >= 0 && w2 >= 0 && (w1 + w2) <= 1;
}
