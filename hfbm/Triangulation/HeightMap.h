#pragma once

#include <vector>
#include <QImage>

class HeightMap
{
public:
	HeightMap(const QImage&);

	int getHeight() const { return height; }
	int getWidth() const { return width; }
	float at(long long index) const {
		return values.at(index);
	}

	void invert();
	void scale(double);

private:
	int height;
	int width;
	std::vector<float> values;
};

