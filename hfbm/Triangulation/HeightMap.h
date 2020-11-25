#pragma once

#include <QImage>
#include <vector>

class HeightMap
{
public:
	HeightMap(const QImage&);
	int getHeight() const { return height; }
	int getWidth() const { return width; }
	float at(long long index) const {
		return values.at(index);
	}

private:

private:
	int height;
	int width;
	std::vector<float> values;
};

