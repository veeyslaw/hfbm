#include "oglwidget.h"

OGLWidget::OGLWidget(QWidget* parent)
  : QOpenGLWidget(parent) {}

OGLWidget::~OGLWidget() {
  mesh->cleanUp();
}

void OGLWidget::initializeGL()
{
  initializeOpenGLFunctions();

  glClearColor(1, 1, 1, 1);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHTING);
  // glEnable(GL_COLOR_MATERIAL); // TODO if cool keep, discard - else

  mesh = std::make_unique<Mesh>(getContext());
  shader = std::make_shared<Shader>(getContext());
}

void OGLWidget::paintGL()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // TODO if needed | GL_STENCIL_BUFFER_BIT
  mesh->render(shader);
}

void OGLWidget::resizeGL(int w, int h)
{
  glViewport(0, 0, w, h);
  /*
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  */
}

QOpenGLContext* OGLWidget::getContext() const {
  auto sharedContext = new QOpenGLContext;
  sharedContext->setShareContext(context());
  sharedContext->create();
  return sharedContext;
}