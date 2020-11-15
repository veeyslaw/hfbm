#include <QImageReader>
#include <QColorSpace>

#include "LabelImageSetter.h"

LabelImageSetter::LabelImageSetter(QLabel* label) : imageLabel(label) {};

void LabelImageSetter::setImage(const QImage& image)
{
	QImage localImage = image;

	imageLabel->setPixmap(QPixmap::fromImage(image));
}