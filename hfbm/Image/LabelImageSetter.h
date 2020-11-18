#pragma once

#include <QLabel>

class LabelImageSetter
{
private:
	QLabel* imageLabel;
public:
	void setLabel(QLabel*);
	void setImage(const QImage&);
};

