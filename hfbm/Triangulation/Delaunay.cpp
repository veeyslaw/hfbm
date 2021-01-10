#include "Delaunay.h"

Delaunay::Delaunay(const QImage& image, int meshHeight) : Triangulator(image, meshHeight) {}

void Delaunay::run() {
	if (done) { return; }

	auto height = heightMap.getHeight();
	auto width = heightMap.getWidth();

	if (height <= 0 || width <= 0) {
		done = true;
		return;
	}

	points.reserve((long long) height * width);

	

	done = true;
}