#include "Image.h"

const QImage& Image::getImage() const
{
	return image;
}

bool Image::loadImage(const QString& fileName)
{
	const QImage newImage(fileName);
	if (newImage.isNull()) {
		return false;
	}

	image = newImage;

	return true;
}
