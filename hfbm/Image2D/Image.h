#pragma once

#include <QImage>

class Image
{
private:
	QImage image;
public:
	const QImage& getImage() const;
	void loadImage(const QString&);
};

