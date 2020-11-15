#pragma once

#include <QLabel>

class LabelImageSetter
{
private:
	QLabel* imageLabel;
public:
	LabelImageSetter(QLabel*);
	void setImage(const QImage&);
};

