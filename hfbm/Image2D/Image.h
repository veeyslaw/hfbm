#pragma once

#include <QImage>

class Image
{
private:
	QImage image;
public:
	bool loadImage(const QString&);
private:
	void setImage(const QImage&);
};

