#include "OGLWidget.h"

OGLWidget::OGLWidget(QWidget* parent)
  : QOpenGLWidget(parent) {}

OGLWidget::~OGLWidget() {}

void OGLWidget::initializeGL()
{
  initializeOpenGLFunctions();
  glClearColor(255, 255, 255, 1);
  
}

void OGLWidget::paintGL()
{
  glClear(GL_COLOR_BUFFER_BIT);
}

void OGLWidget::resizeGL(int w, int h)
{

}