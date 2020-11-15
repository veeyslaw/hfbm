#pragma once

#include <QImage>
#include <QLabel>

class LabelImageLoader
{
private:
	QImage image;
	QLabel* imageLabel;
public:
	LabelImageLoader(QLabel*);
	bool loadImage(const QString&);
private:
	void setImage(const QImage&);
};

