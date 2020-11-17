#pragma once

#include <QOpenGLWidget>

class OGLWidget :
    public QOpenGLWidget
{
public:
  OGLWidget(QWidget* parent = Q_NULLPTR);
};

