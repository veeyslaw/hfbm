#include <gtc/matrix_transform.hpp>
#include "oglwidget.h"

OGLWidget::OGLWidget(QWidget* parent)
  : QOpenGLWidget(parent) {}

OGLWidget::~OGLWidget() {
  mesh->cleanUp();
}

void OGLWidget::initializeGL()
{
  initializeOpenGLFunctions();

  initOptions();
  initMatrices();

  mesh = std::make_unique<Mesh>(getContext());
  shader = std::make_shared<Shader>(getContext());
}

void OGLWidget::initOptions() {
  glClearColor(1, 1, 1, 1);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHTING);
  // glEnable(GL_COLOR_MATERIAL); // TODO if cool keep, discard - else
}

void OGLWidget::initMatrices() {
  glm::fvec3 camPosition(0, 0, 1);
  glm::fvec3 worldUp(0, 1, 0);
  glm::fvec3 camFront(0, 0, -1);
  viewMatrix = glm::lookAt(camPosition, camPosition + camFront, worldUp);

  float fov = 90;
  float nearPlane = .1;
  float farPlane = 1000;
  projectionMatrix = glm::perspective(glm::radians(fov), static_cast<float>(width()) / height(), nearPlane, farPlane);
}

void OGLWidget::updateUniforms() {
  shader->setMatrix4fv(viewMatrix, "ViewMatrix");
  shader->setMatrix4fv(projectionMatrix, "ProjectionMatrix");
}

void OGLWidget::updateProjectionMatrix(int w, int h) {
  projectionMatrix = glm::fmat4(1);
  float fov = 90;
  float nearPlane = .1;
  float farPlane = 1000;
  projectionMatrix = glm::perspective(glm::radians(fov), static_cast<float>(width()) / height(), nearPlane, farPlane);
}

void OGLWidget::updateViewMatrix() {

}

void OGLWidget::paintGL()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  updateUniforms();
  // TODO if needed | GL_STENCIL_BUFFER_BIT
  mesh->render(shader);
}

void OGLWidget::resizeGL(int w, int h)
{
  glViewport(0, 0, w, h);
  updateProjectionMatrix(w, h);
  /*
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  */
  mesh->rotate(glm::fvec3(80));
}

QOpenGLContext* OGLWidget::getContext() const {
  auto sharedContext = new QOpenGLContext;
  sharedContext->setShareContext(context());
  sharedContext->create();
  return sharedContext;
}