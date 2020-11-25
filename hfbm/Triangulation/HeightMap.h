#pragma once

#include <QImage>
#include <vector>

class HeightMap
{
public:
	HeightMap(const QImage&);
	long getHeight() const { return height; }
	long getWidth() const { return width; }
	float at(long long index) const {
		return values.at(index);
	}

private:

private:
	long height;
	long width;
	std::vector<float> values;
};

