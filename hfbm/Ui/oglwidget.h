#pragma once

#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>

class OGLWidget :
    public QOpenGLWidget, protected QOpenGLExtraFunctions
{
public:
  OGLWidget(QWidget* parent);
  ~OGLWidget();

protected:
  void initializeGL();
  void resizeGL(int w, int h);
  void paintGL();

private:
  GLuint vertexbuffer;
};

