#include "Naive.h"

Naive::Naive(const QImage& image) : Triangulator(image) {}

void Naive::run() {
	if (done) { return; }

	auto height = heightMap.getHeight();
	auto width = heightMap.getWidth();

	if (height <= 0 || width <= 0) {
		done = true;
		return;
	}

	points.reserve((long long) height * width);

	float scale = 1; // TODO parametrize z scaling
	for (auto y = 0; y < height; y++) {
		for (auto x = 0; x < width; x++) {
			auto z = scale * heightMap.at((long long) y * width + x);
			points.push_back(glm::fvec3(x, y, z));
		}
	}

	// 2 triangles each iteration
	triangles.reserve((long long) 2 * (height - 1) * (width - 1));

	for (auto y = 1; y < height; y++) {
		for (auto x = 1; x < width; x++) {
			// |\
			// | \
			// |__\ (x, y) CCW

			auto point1 = (long long) y * width + x;
			auto point2 = (long long) (y - 1) * width + (x - 1);
			auto point3 = (long long) y * width + (x - 1);
			triangles.push_back(Triangle(point1, point2, point3));

			// ____
			// \  |
			//  \ |
			//   \|
			//    (x, y) CCW

			point1 = (long long) y * width + x;
			point2 = (long long) (y - 1) * width + x;
			point3 = (long long) (y - 1) * width + (x - 1);
			triangles.push_back(Triangle(point1, point2, point3));

		}
	}

	mirrorY();

	// TODO move to model matrix
	auto scaleDown = std::max(width, height) / 2;
	for (auto& point : points) {
		point.x -= width / 2;
		point.y -= height / 2;
		point.x /= scaleDown;
		point.y /= scaleDown;
	}

	done = true;
}