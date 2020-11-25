#include <QImageReader>
#include <QColorSpace>

#include "LabelImageSetter.h"

void LabelImageSetter::setLabel(QLabel* newLabel)
{
	imageLabel = newLabel;
}

void LabelImageSetter::setImage(const QImage& image)
{
	imageLabel->setPixmap(QPixmap::fromImage(image));
}