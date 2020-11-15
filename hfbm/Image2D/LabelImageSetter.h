#pragma once

#include <QLabel>

class LabelImageSetter
{
private:
	QLabel* imageLabel;
public:
	LabelImageSetter(QLabel*);
	bool loadImage(const QString&);
private:
};

