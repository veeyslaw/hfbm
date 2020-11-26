#include "HeightMap.h"

HeightMap::HeightMap(const QImage& image) : 
	height(image.height()),
	width(image.width()) {

	values.reserve((long long) width * height);
	for (auto y = 0; y < height; y++) {
		for (auto x = 0; x < width; x++) {
			// qGray calculates gray value with (R * 11 + G * 16 + B * 5) / 32 and returns a value between 0 and 255
			values.push_back(qGray(image.pixel(x, y)) / 255.);
		}
	}
}