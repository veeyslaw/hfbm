#include "oglwidget.h"

OGLWidget::OGLWidget(QWidget* parent)
  : QOpenGLWidget(parent) {}

OGLWidget::~OGLWidget() {}

void OGLWidget::initializeGL()
{
  initializeOpenGLFunctions();
  glClearColor(1, 1, 1, 1);
  glEnable(GL_DEPTH_TEST);
  
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHTING);
  // glEnable(GL_COLOR_MATERIAL); // TODO if cool keep, discard - else
}

void OGLWidget::paintGL()
{
  glClear(GL_COLOR_BUFFER_BIT);

  glBegin(GL_TRIANGLES);
  glColor3f(1.0, 0.0, 0.0);
  glVertex3f(-0.5, -0.5, 0);
  glColor3f(0.0, 1.0, 0.0);
  glVertex3f(0.5, -0.5, 0);
  glColor3f(0.0, 0.0, 1.0);
  glVertex3f(0.0, 0.5, 0);
  glEnd();
}

void OGLWidget::resizeGL(int w, int h)
{
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}