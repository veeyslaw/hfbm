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
  glClearColor(1, 1, 1, 0);
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void OGLWidget::initMatrices() {
  glm::fvec3 worldUp(0, 1, 0);
  glm::fvec3 camFront(0, 0, -1);
  viewMatrix = glm::lookAt(cameraPosition, cameraPosition + camFront, worldUp);

  updateProjectionMatrix(width(), height());
}

void OGLWidget::updateUniforms() {
  shader->setMatrix4fv(viewMatrix, "ViewMatrix");
  shader->setMatrix4fv(projectionMatrix, "ProjectionMatrix");
  shader->setVec3fv(lightPosition, "lightPosition");
  shader->setVec3fv(cameraPosition, "cameraPosition");
}

void OGLWidget::updateProjectionMatrix(int w, int h) {
  projectionMatrix = glm::fmat4(1);
  float fov = 90;
  float nearPlane = .1;
  float farPlane = 1000;
  projectionMatrix = glm::perspective(glm::radians(fov), static_cast<float>(width()) / height(), nearPlane, farPlane);
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
}

QOpenGLContext* OGLWidget::getContext() const {
  auto sharedContext = new QOpenGLContext;
  sharedContext->setShareContext(context());
  sharedContext->create();
  return sharedContext;
}

void OGLWidget::rotateMesh(QMouseEvent* event) {
  // Drag in the direction of the mouse
  auto yDrag = event->pos().y() - lastMousePoint.y();
  auto xDrag = event->pos().x() - lastMousePoint.x();
  auto scale = 0.01;
  mesh->rotate(glm::fvec3(yDrag * scale, xDrag * scale, 0));
  update();
}

void OGLWidget::scaleMesh(QWheelEvent* event) {
  auto degrees = event->angleDelta() / 8;
  auto steps = degrees / 15;
  auto scale = 0.02;
  mesh->scaleUp(scale * steps.y());
  update();
}

void OGLWidget::mousePressEvent(QMouseEvent* event) {
  if (event->button() == Qt::LeftButton) {
    dragging = true;
    lastMousePoint = event->pos();
  }
}

void OGLWidget::mouseMoveEvent(QMouseEvent* event) {
  if (dragging && (event->buttons() & Qt::LeftButton)) {
    rotateMesh(event);
    lastMousePoint = event->pos();
  }
}

void OGLWidget::mouseReleaseEvent(QMouseEvent* event) {
  if (dragging && event->button() == Qt::LeftButton) {
    dragging = false;
  }
}

void OGLWidget::wheelEvent(QWheelEvent* event) {
  scaleMesh(event);
  event->accept();
}