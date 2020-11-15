#include <QImageReader>
#include <QColorSpace>

#include "ImageViewer.h"

ImageViewer::ImageViewer(QLabel* label) : imageLabel(label) {};

bool ImageViewer::uploadImage(const QString& fileName) {
	QImageReader reader(fileName);
	const QImage image = reader.read();
	if (image.isNull()) {
		// TODO handle null image
		return false;
	}

	setImage(image);
	return true;
}

void ImageViewer::setImage(const QImage& newImage) {
	image = newImage;
	if (image.colorSpace().isValid()) {
		image.convertToColorSpace(QColorSpace::SRgb);
	}

	imageLabel->setPixmap(QPixmap::fromImage(image));
}