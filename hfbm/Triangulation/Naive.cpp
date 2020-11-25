#include "Naive.h"

Naive::Naive(const QImage& image) : Triangulator(image) {}

void Naive::run() {
	if (done) { return; }

	auto height = heightMap.getHeight();
	auto width = heightMap.getWidth();

	for (auto y = 1; y < height; y++) {
		for (auto x = 1; x < width; x++) {
			// |\
			// | \
			// |__\ (x, y)

			auto xp = x;
			auto yp = y;
			auto zp = 255 * heightMap.at((long long) yp * width + xp);
			vertices.push_back(glm::fvec3(xp, yp, zp));

			xp = x - 1;
			yp = y - 1;
			zp = 255 * heightMap.at((long long)yp * width + xp);
			vertices.push_back(glm::fvec3(xp, yp, zp));

			xp = x - 1;
			yp = y;
			zp = 255 * heightMap.at((long long)yp * width + xp);
			vertices.push_back(glm::fvec3(xp, yp, zp));

			// ____
			// \  |
			//  \ |
			//   \|
			//    (x, y)

			xp = x;
			yp = y;
			zp = 255 * heightMap.at((long long)yp * width + xp);
			vertices.push_back(glm::fvec3(xp, yp, zp));

			xp = x;
			yp = y - 1;
			zp = 255 * heightMap.at((long long)yp * width + xp);
			vertices.push_back(glm::fvec3(xp, yp, zp));

			xp = x - 1;
			yp = y - 1;
			zp = 255 * heightMap.at((long long)yp * width + xp);
			vertices.push_back(glm::fvec3(xp, yp, zp));
		}
	}

	done = true;
}