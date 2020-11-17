#pragma once

#include <QImage>

//image.convertTo(QImage::Format_Grayscale16);

class Image
{
private:
	QImage image;
public:
	const QImage& getImage() const;
	bool loadImage(const QString&);
};

