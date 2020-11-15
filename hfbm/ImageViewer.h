#pragma once

#include <QImage>
#include <QLabel>

class ImageViewer
{
private:
	QImage image;
	QLabel* imageLabel;
public:
	ImageViewer(QLabel*);
	bool uploadImage(const QString&);
private:
	void setImage(const QImage&);
};

