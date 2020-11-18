#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>

class OGLWidget :
    public QOpenGLWidget, protected QOpenGLFunctions
{
public:
  OGLWidget(QWidget* parent = Q_NULLPTR);
  ~OGLWidget();

protected:
  void initializeGL();
  void resizeGL(int w, int h);
  void paintGL();
};

