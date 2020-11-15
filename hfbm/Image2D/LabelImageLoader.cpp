#include <QImageReader>
#include <QColorSpace>

#include "LabelImageLoader.h"

LabelImageLoader::LabelImageLoader(QLabel* label) : imageLabel(label) {};

bool LabelImageLoader::loadImage(const QString& fileName) {
	QImageReader reader(fileName);
	const QImage image = reader.read();
	if (image.isNull()) {
		// TODO handle null image
		return false;
	}

	setImage(image);
	return true;
}

void LabelImageLoader::setImage(const QImage& newImage) {
	image = newImage;
	if (image.colorSpace().isValid()) {
		image.convertToColorSpace(QColorSpace::SRgb);
	}

	imageLabel->setPixmap(QPixmap::fromImage(image));
}