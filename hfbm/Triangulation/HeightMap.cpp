#include "HeightMap.h"

HeightMap::HeightMap(const QImage& image) : 
	height(image.height()),
	width(image.width()) {

	values.reserve((long long) width * height);
	for (auto y = 0; y < height; y++) {
		for (auto x = 0; x < width; x++) {
			values.push_back(qGray(image.pixel(x, y)) / 255.);
		}
	}
}