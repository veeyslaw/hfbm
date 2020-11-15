#include <QImageReader>
#include <QColorSpace>

#include "LabelImageSetter.h"

LabelImageSetter::LabelImageSetter(QLabel* label) : imageLabel(label) {};

bool LabelImageSetter::loadImage(const QString& fileName) {
	QImageReader reader(fileName);
	const QImage image = reader.read();
	if (image.isNull()) {
		// TODO handle null image
		return false;
	}

	setImage(image);
	return true;
}

void LabelImageSetter::setImage(const QImage& newImage) {
	image = newImage;
	if (image.colorSpace().isValid()) {
		image.convertToColorSpace(QColorSpace::SRgb);
	}

	imageLabel->setPixmap(QPixmap::fromImage(image));
}