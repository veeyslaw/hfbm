#include "Image.h"

#include <QImageReader>
#include <QColorSpace>

const QImage& Image::getImage() const
{
	return image;
}

void Image::loadImage(const QString& fileName)
{
	QImageReader reader(fileName);
	const QImage newImage = reader.read();
	if (newImage.isNull()) {
		// TODO handle null image
	}

	image = newImage;

	/* ???????????????????????????????????????????????????????????????????
	if (image.colorSpace().isValid()) {
		image.convertToColorSpace(QColorSpace::SRgb);
	}
	*/
}
